#include<stdio.h>
#include"word.h"
#include<stdlib.h>
#include<assert.h>
Pword Phead;//�ڴ��ϱ߽�
Pword Ptail;//�ڴ��±߽�
int main()
{
	//����100M��
	Pword head = (Pword)malloc(1024*1024*100);
	assert(head != NULL);
	Phead = head;
	Ptail = head + 6553600 - 1;
	//��ʼ���ڴ��
	init_word(head);
	//��word������һ���ڴ�
	void *ptr = get_word(&head,100);
	void *p = get_word(&head, 100);
	printf_init();
	printf_busy();
	//�ͷ��ڴ�
	del_word(&head,ptr);
	del_word(&head, p);
	free(head);
	return 0;
}