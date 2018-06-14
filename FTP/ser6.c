#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<assert.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<pthread.h>
#include<fcntl.h>
#include<mysql/mysql.h>

#define SER_PORT 6000
#define SER_IP "127.0.0.1"
#define PATH "/home/lyf/mydir/gy0522/serfile/"


typedef struct broken_file
{
    char passwd[128];//md5
    int size;    
}broken_file;

broken_file p[1000]={0};
static int count_p =0;



void * thread_fun(void *arg);
void mysql_connect();
void insert();
int query();
int socket_create();
int main()
{
    int sockfd = socket_create();

    struct sockaddr_in caddr;
    while(1)
    {
        int len = sizeof(len);
        int c = accept(sockfd,(struct sockaddr*)&caddr,&len);
        if(c<0)
        {
            continue;
        }
        printf("accept c = %d\n",c);
        
        mysql_connect();
        pthread_t id;
        pthread_create(&id,NULL,thread_fun,(void *)c);
    }
}
void mysql_connect()
{
    MYSQL mysql;
    mysql_init(&mysql);//chuushihua  MYSQL
    if(!mysql_real_connect(&mysql,"localhost","root","111111","liyufeng",0,NULL,0))
    {
        printf("Error connecting to Mysql!\n");;
        return;
    }

    printf("Connected Mysql successful!\n");
}
int query(char *name)
{
    MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *query;
    int flag,t;
    mysql_connect();
    query = "select * from md5 where filename = name";
    flag = mysql_real_query(&mysql,query,(unsigned int)strlen(query));
    if(flag)
    {
        printf("Query failed!\n");
        return 1;
    }
    else
    {
        printf("[%s] made ...\n",query);
    }
    //jiangsuoyou result read to client
    res = mysql_store_result(&mysql);
    while(row = mysql_fetch_row(res))
    {
        for(t=0;t < mysql_num_fields(res);++t)
        {
            printf("%s\t",row[t]);
        }
        printf("\n");
    }
    return 0;
}
void insert(const char *md5_value,const char *filename)
{
    char buff[64] = {0};
    char *query;
    int flag;
    MYSQL mysql;
    mysql_connect();
    sprintf(buff,"%s%s",md5_value,filename);
    
    flag = mysql_real_query(&mysql,query,(unsigned int)strlen(buff));
    if(flag)
    {
        printf("Insert data failed!\n");
        return;
    }
    else
    {
        printf("Insert data success!\n");
    }
    
}
int socket_create()

{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1)
    {
        return -1;
    }

    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(SER_PORT);
    saddr.sin_addr.s_addr = inet_addr(SER_IP);

    int res = bind(sockfd,(struct sockaddr *)&saddr,sizeof(saddr));
    assert(res != -1);
    
    listen(sockfd,5);

    return sockfd;
}
int search_broken(char *md5_value)
{
    int i = 0;
    for(;i<count_p;++i)
    {
        if(strcmp(p[i].passwd,md5_value) == 0)
        {
            return p[i].size;
        }
    }
    return 0;
}

int search_broken_index(char *md5_value)
{
    int i = 0;
    for(;i<count_p;++i)
    {
        if(strcmp(p[i].passwd,md5_value) == 0)
        {
            return i;
        }
    }
    return -1;
}

void recv_file(int c,char *filename)
{

    send(c,"ok",2,0);
    char md5buff[128]={0};

    if(recv(c,md5buff,127,0)<0)
    {
        return;    
    }
    printf("buff = %s\n",md5buff);
        





//////////////////////////////////////////////


    char buff[128] = {0};
    //printf("buff = %s\n",buff+3);
    char *md5_value;
    if(recv(c,buff,127,0) <= 0)
    {
        return;
    }
    if(strncmp(buff,"ok#",3) != 0)
    {
        printf("Error:%s\n",buff+3);
        return;
    }

    int size = 0;
    sscanf(buff+3,"%d",&size);
    printf(" str size:=%s,size=%d\n",buff+3,size);

    //if(!query(filename))//find to table 
    //{
      //  insert(md5_value,filename);
      //  printf("md5 not find,");

    int oldsize =0;
    if((oldsize =search_broken(md5buff))!=0 )//size  or 0
    {

        send(c,"broken",6,0);//brk
        char recv_buff[128] ={0};
        if(recv(c,recv_buff,127,0)<0)
        {
            return;    
        }
        memset(recv_buff,0,sizeof(recv_buff));
        sprintf(recv_buff,"%d",oldsize);

        send(c,recv_buff,strlen(recv_buff),0);

        if(recv(c,recv_buff,127,0)<0)
        {
            return ;
        }

        int fd = open(filename,O_WRONLY);
        if(fd == -1)
        {
            send(c,"err",3,0);
            return;
        }
        send(c,"ok",2,0);

        lseek(fd,0,SEEK_END);

        char data_size[512] = {0};
        int num = 0;
        int cur_size = oldsize; 
        int index = search_broken_index(md5buff);
        while((num = recv(c,data_size,512,0)) > 0)
        {
            cur_size+=num;

            write(fd,data_size,num);
            if(cur_size == size)
            {
                strcpy(p[index].passwd,"");
                close(fd);
                return;    
            }
        }
        // i 
        close(fd);
        if(cur_size != size )
        {
            p[index].size=cur_size;
        }
    }

    int fd = open(filename,O_WRONLY|O_CREAT,0600);
    if(fd == -1)
    {
        send(c,"err",3,0);
        return;
    }
    send(c,"ok",2,0);

    char data_size[512] = {0};
    int num = 0;
    int cur_size = 0; 
    while((num = recv(c,data_size,512,0)) > 0)
    {
        cur_size+=num;

        write(fd,data_size,num);
        if(cur_size == size)
        {
            close(fd);
            return;    
        }
    }
    if(cur_size != size )
    {
        strcpy(p[count_p].passwd,md5buff);
        p[count_p++].size = cur_size;
    }
} 
void * thread_fun(void *arg)
{
    int c = (int)arg;
    while(1)
    {
        char buff [128] = {0};
        int n = recv(c,buff,127,0);//rm a.c
        //printf("buff  = %s",buff);
        if(n <= 0)
        {
            break;
            //close(c);
            //printf("one client over\n");
            //pthread_exit(NULL);
        }

        char *myargv[10] = {0};
        char * ptr = NULL;
        char *s = strtok_r(buff," ",&ptr);

        if(s == NULL)
        {
            send(c,"ok#",3,0);//send ok#  fangzhi  cli  zuse
            continue;
        }
         
        int i = 1;
        myargv[0] = s;

        while(( s = strtok_r(NULL," ",&ptr )) != NULL )
        {
            myargv[i++] = s;
        }
        if(strcmp(myargv[0],"get") == 0)
        {
            int fd = open(myargv[1],O_RDONLY);
            if(fd == -1)
            {
                send(c,"err",3,0);
                continue;
            }

            int size = lseek(fd,0,SEEK_END);
            lseek(fd,0,SEEK_SET);

            char file_size[64] = {"ok#"};
            sprintf(file_size+3,"%d",size);

            send(c,file_size,strlen(file_size),0);//send size to client

            if(size == 0)
            {
                printf("file is empty!\n");
                return;
            }

            char statu[32] = {0};

            if(recv(c,statu,31,0) <= 0)
            {
                break;
            }

            if(strcmp(statu,"ok") != 0)
            {
                continue;
            } 

            //sendfile
            sendfile(c,fd,NULL,size);
            //char data[512] = {0};
            //int num = 0;
            //while((num = read(fd,data,512)) > 0)
            //{
            //    send(c,data,num,0);//send data to client
            //}
            close(fd);
        }
        else if(strcmp(myargv[0],"put") == 0)
        {
            //PUT();
            //printf("put1\n");
            recv_file(c,myargv[1]);
        }
        else
        {
            int pipefd[2];
            pipe(pipefd);
            pid_t pid = fork();
            assert(pid != -1);

            if(pid == 0)
            {
                dup2(pipefd[1],1);
                dup2(pipefd[1],2);

                execvp(myargv[0],myargv);
                perror("execvp error");
                exit(0);
            }
            else
            {
                close(pipefd[1]);
                wait(NULL);

                char readbuff[1024] = {"ok#"};
                int pipe_num = read(pipefd[0],readbuff+3,1020);
                send(c,readbuff,pipe_num+3,0);
                close(pipefd[0]);
            }
        }
        //strtok_r -> rm a.c -> myargv[];
        //1) get -> send_file
        //2)put -> recv_file
        //3)rm cp ls ->  chuangjian pipe   ->   fork()
        //parent -> read(pipefd[0]);
        //child -> dup(pipefd[1],1);
        //         dup(pipefd[1],2)
        //exec(rm);
        //exit();
        //printf("buff=%s\n",buff);
        //send(c,"ok",2,0);
    }
}
