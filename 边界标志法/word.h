#pragma once
#define WORD_H
#define INIT 0
#define BUSY 1
typedef struct word
{
	int tag;//标记
	int len;//大小
	union 
	{
		struct word* Llink;//左指针
		struct word* Uplink;//头边界指针
	};
	struct word* Rlink;//右指针
}word,*Pword;
void init_word(Pword head);
void *get_word(Pword *head,int size);
void del_word(Pword *head,void *ptr);
void printf_init();//打印空闲块信息，len tag 地址
void printf_busy();//打印占用块信息，len tag 地址


