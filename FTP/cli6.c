#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<assert.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<openssl/md5.h>

#define SER_PORT 6000
#define SER_IP "127.0.0.1"
#define MD5_LEN 16
#define BUFF_SIZE 1024*16

void fun_md5(int fd,char* md5buff)
{
    MD5_CTX ctx;
    unsigned char md[MD5_LEN] = {0};
    MD5_Init(&ctx);
    
    unsigned long len = 0;
    char buff[BUFF_SIZE] = {0};
    while((len = read(fd,buff,BUFF_SIZE)) > 0)
    {
        MD5_Update(&ctx,buff,len);
    }
    
    MD5_Final(md,&ctx);
    int i = 0;
    for(;i<MD5_LEN;++i)
    {
    sprintf(md5buff+2*i,"%02x",md[i]);
    }
}
void recv_file(int sockfd,char *name)
{
    char buff[64] = {0};
    if(recv(sockfd,buff,63,0) <= 0)
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
    if(size == 0)
    {
        return;
    }
    printf(" str szie:=%s,size=%d\n",buff+3,size);

    int fd = open(name, O_WRONLY|O_CREAT,0600);
    if(fd == -1)
    {
        send(sockfd,"err",3,0);
        return;
    }
    send(sockfd,"ok",2,0);

    char data_size[512] = {0};
    int num = 0;

    int curr_size = 0;
    while((num = recv(sockfd,data_size,512,0)) > 0)
    {
        write(fd,data_size,num);
        curr_size += num;

        float f = curr_size * 100.0 / size;
        printf("down:%.2f%%\r",f);
        fflush(stdout);

        if(curr_size == size)
        {
        printf("\n:down finished\n");
        close(fd);
        break;
        }
    }
}
int mystrcmp(char buf[])
{
    char *buff[] = {"ls","cp","rm","mv","touch"};
    int size = sizeof(buff)/sizeof(buff[0]);
    if(strncmp(buf,"ls",2) == 0)
    {
        strcpy(buf,"ls --color -l");
        //buff = "ls --color -l";
        return 1;
    }
    int i = 0;
    for(;i<size;++i)
    {
        if(strncmp(buf,buff[i],strlen(buff[i])) == 0)
        {
            return 1;
        }
    }
    return 0;
}
int main()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    assert(sockfd != -1);

    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(SER_PORT);
    saddr.sin_addr.s_addr = inet_addr(SER_IP);

    int res = connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    assert(res != -1);
    while(1)
    {
        char buff[128] = {0};
        printf("Connect success>> ");
        fflush(stdout);

        fgets(buff,128,stdin);//ls  cp  mv 
        buff[strlen(buff) -1] = 0;

        char cmd[128] = {0};
        strcpy(cmd,buff);
        char *s = strtok(cmd," ");
        if( s == NULL)
        {
            continue;
        }
        if(strcmp(s,"exit") == 0)
        {
            break;
        }
        else if(strcmp(s,"get") == 0)
        {
            s = strtok(NULL," ");
            if( s == NULL)
            {
                continue;
            }
            send(sockfd,buff,strlen(buff),0);

            recv_file(sockfd,s);
        }
        else if(strcmp(s,"put") == 0)
        
        {
            s = strtok(NULL," ");
            if(s == NULL)
            {
                continue;
            }
            /*int f = mysql_query(name);
            if(f != 0)
            {
                continue;
            }*/
            int fd = open(s,O_RDONLY);
            if(fd == -1)
            {
                send(sockfd,"err",3,0);
                continue;
            }


            send(sockfd,buff,strlen(buff),0);
            printf("buff = %s\n",buff);

            
            char statu1[32] = {0};

            if(recv(sockfd,statu1,31,0) <= 0)
            {
                break;
            }


            char md5buff1[128] ={0};

            fun_md5(fd,md5buff1);
            printf("%s\n",md5buff1); 
            send(sockfd,md5buff1,strlen(md5buff1),0);
           //////////////////////////////////////////////////// 
            /*
             s = strtok(NULL," ");
            if(s == NULL)
            {
                continue;
            }
            send(sockfd,s,1,0);//send md5 to ser
            */
            int size = lseek(fd,0,SEEK_END);
            lseek(fd,0,SEEK_SET);

            char file_size[64] = {"ok#"};
            sprintf(file_size+3,"%d",size);

            send(sockfd,file_size,strlen(file_size),0);

            if(size == 0)
            {
                continue;
            }
            char statu[32] = {0};

            if(recv(sockfd,statu,31,0) <= 0)
            {
                break;
            }   
            if(strcmp(statu,"broken") == 0)
            {
                send(sockfd,"brk",3,0);
                
                char bsize[32] ={0};
                if(recv(sockfd,bsize,31,0)<0)
                {
                    return;    
                }
                int oldsize=0;
                sscanf(bsize,"%d",&oldsize);
                lseek(fd,oldsize,SEEK_SET);
                
                send(sockfd,"ok",2,0);

                char recv_buff[32]={0};
                if(recv(sockfd,recv_buff,31,0)<0)
                {
                    return;    
                }
                if(strcmp(recv_buff,"ok")!=0)
                {
                    return;    
                }


                int num = 0;
                char data[512] = {0};
                int cur_size = oldsize;
                while((num = read(fd,data,512))>0)
                {
                    send(sockfd,data,num,0);    
                    cur_size+=num;
                    float f = cur_size*100.0/size;
                    printf("put:%.2f%%\r",f);
                    fflush(stdout);
                    if(cur_size == size)
                    {
                        printf("\nput finished\n");
                        close(fd);    
                        break;
                    }
                }
            }

            if(strcmp(statu,"ok") != 0)
            {
                continue;
            }

            //sendfile(sockfd,fd,NULL,size);
            int num = 0;
            char data[512] = {0};
            int cur_size = 0;
            while((num = read(fd,data,512))>0)
            {
                send(sockfd,data,num,0);    
                cur_size+=num;
                float f = cur_size*100.0/size;
                printf("put:%.2f%%\r",f);
                fflush(stdout);
                if(cur_size == size)
                {
                    printf("\nput finished\n");
                    close(fd);    
                    break;
                }

            }
            
            



        }
        else if(mystrcmp(buff))
        {
            send(sockfd,buff,strlen(buff),0);

            char recvbuff[1024] = {0};
            
            if(recv(sockfd,recvbuff,1023,0) <= 0)
            {
                continue;
            }
            if(strncmp(recvbuff,"ok#",3) == 0)
            {
                printf("%s\n",recvbuff+3);
            }
            else
            {
                printf("error:%s",recvbuff);
            }
        }
        else
        {
            printf("input error\n");
        }
    }
    close(sockfd);
}
