#pragma once
#define WORD_H
#define INIT 0
#define BUSY 1
typedef struct word
{
	int tag;//���
	int len;//��С
	union 
	{
		struct word* Llink;//��ָ��
		struct word* Uplink;//ͷ�߽�ָ��
	};
	struct word* Rlink;//��ָ��
}word,*Pword;
void init_word(Pword head);
void *get_word(Pword *head,int size);
void del_word(Pword *head,void *ptr);
void printf_init();//��ӡ���п���Ϣ��len tag ��ַ
void printf_busy();//��ӡռ�ÿ���Ϣ��len tag ��ַ


