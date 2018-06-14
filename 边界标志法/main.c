#include<stdio.h>
#include"word.h"
#include<stdlib.h>
#include<assert.h>
Pword Phead;//内存上边界
Pword Ptail;//内存下边界
int main()
{
	//申请100M；
	Pword head = (Pword)malloc(1024*1024*100);
	assert(head != NULL);
	Phead = head;
	Ptail = head + 6553600 - 1;
	//初始化内存块
	init_word(head);
	//从word上申请一块内存
	void *ptr = get_word(&head,100);
	void *p = get_word(&head, 100);
	printf_init();
	printf_busy();
	//释放内存
	del_word(&head,ptr);
	del_word(&head, p);
	free(head);
	return 0;
}