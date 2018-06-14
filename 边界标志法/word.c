#include<stdio.h>
#include"word.h"
#include<stdlib.h>
#include<assert.h>
extern Pword Phead;//内存上边界
extern Pword Ptail;//内存下边界
static Pword get_tail(Pword head)
{
	return head + head->len + 1;
}
void init_word(Pword head)
{
	head[0].len = 6553598;
	head[0].Llink = head;
	head[0].Rlink = head;

	head[0].tag = 0;
	Pword tail = get_tail(head);
	tail->Uplink = head;
	tail->tag = 0;
}
void *change_word(Pword *head,Pword ptr, int len)
{
	if (ptr->len - len - 2 < 1)
	{
		//从空闲链表中删除ptr
		ptr->Llink->Rlink = ptr->Rlink;
		ptr->Rlink->Llink = ptr->Llink;
		//改变标志位
		ptr->tag = 1;
		get_tail(ptr)->tag = 1;
		//判断该内存块是否为最后一个可用的内存块
		if (*head == ptr)
		{
			*head = NULL;
		}
		return ptr + 1;
	}
	else
	{
		//获取剪切部分的指针
		Pword pre = ptr + ptr->len - len;
		//修改剪切部分的指针
		pre->len = len;
		pre->tag = 1;
		get_tail(pre)->Uplink = pre;
		get_tail(pre)->tag = 1;
		//修改剩余部分内存块的信息
		ptr->len = ptr->len - len - 2;
		get_tail(ptr)->Uplink = ptr;
		//返回用户操作空间的指针
		return pre + 1;
	}
}
void *get_word(Pword *head,int size)
{
	if (head == NULL || size <= 0)
	{
		return NULL;
	}
	int len = (size + 15) / 16;//计算要用的内存格子数
	//检测出第一个可用内存块
	Pword ptr;
	for ( ptr= (*head)->Llink;;ptr = ptr->Llink)
	{
		if (ptr->len >= len || ptr == (*head)->Rlink)
		{
			break;
		}
	}
	//判断内存是否可用
	if (ptr->len >= len)
	{
		//修改内存信息，并返回内存块
		return change_word(head, ptr, len);
	}
	//没有可用内存块
	return NULL;
}
void del_word(Pword *head, void* ptr)
{
	Pword p = (Pword)ptr - 1;
	if (p = Phead)//释放的内存为上边界
	{
		if ((get_tail(p)+1)->tag == 0)//下空闲
		{
			Pword next = get_tail(p) + 1;//获得q的下一块空闲区
			p->tag = 0;//修改释放区信息
			get_tail(p)->tag = 0;
			p->len = p->len + next->len;
			get_tail(next)->Uplink = p;//修改q下一块空闲区的信息
			next->Llink->Rlink = next->Rlink;//删除q的下一块结点
			next->Rlink->Llink = next->Llink;
			p->Rlink = *head;//将所有释放的空闲区加到空闲链表中
			p->Llink = (*head)->Llink;
			p->Llink->Rlink = p;
			(*head)->Llink = p;
		}
		else//下占用
		{
			p->tag = 0;//修改释放区信息
			get_tail(p)->tag = 0;
			p->Rlink = *head;//将释放区加到空闲链表中
			p->Llink = (*head)->Llink;
			p->Llink->Rlink = p;
			(*head)->Llink = p;
		}
	}
	else if (get_tail(p) == Ptail)//释放内存为下边界
	{
		if ((p - 1)->tag == 0)//上空闲
		{
			(p - 1)->Uplink->len = (p - 1)->Uplink->len + p->len;//合并释放区
			p->tag = 0;//修改释放区
			get_tail(p)->tag = 0;
			get_tail(p)->Uplink = (p - 1)->Uplink;
		}
		else//上占用
		{
			p->tag = 0;//修改释放区信息
			get_tail(p)->tag = 0;
			p->Rlink = *head;//将释放区加到空闲链表中
			p->Llink = (*head)->Llink;
			p->Llink->Rlink = p;
			(*head)->Llink = p;
		}
	}
	else if ((p-1)->tag == 0 && (get_tail(p)+1)->tag == 0)//上边界空闲，下边界空闲
	{
		Pword pre = (p - 1)->Uplink;//获取上一块空闲区
		Pword next = get_tail(p) + 1;//获取下一块空闲区
		pre->len = pre->len + p->len + next->len;//修改释放区信息
		p->tag = 0;
		get_tail(p)->tag = 0;
		get_tail(next)->Uplink = pre;
		next->Llink->Rlink = next->Rlink;//删除next结点
		next->Rlink->Llink = next->Llink;
	}
	else if ((p-1)->tag == 1 && (get_tail(p) + 1)->tag == 0)//上边界占用，下边界空闲
	{
		Pword next = get_tail(p) + 1;//获得q的下一块空闲区
		p->tag = 0;//修改释放区信息
		get_tail(p)->tag = 0;
		p->len = p->len + next->len;
		get_tail(next)->Uplink = p;//修改q下一块空闲区的信息
		next->Llink->Rlink = next->Rlink;//删除q的下一块结点
		next->Rlink->Llink = next->Llink;
		p->Rlink = *head;//将所有释放的空闲区加到空闲链表中
		p->Llink = (*head)->Llink;
		p->Llink->Rlink = p;
		(*head)->Llink = p;
	}
	else if ((p-1)->tag == 0 && (get_tail(p) + 1)->tag == 1)//上边界空闲，下边界占用
	{
		(p - 1)->Uplink->len = (p - 1)->Uplink->len + p->len;//合并释放区
		p->tag = 0;//修改释放区
		get_tail(p)->tag = 0;
		get_tail(p)->Uplink = (p - 1)->Uplink;
	}
	else//上边界占用，下边界占用
	{
		p->tag = 0;//修改释放区信息
		get_tail(p)->tag = 0;
		p->Rlink = *head;//将释放区加到空闲链表中
		p->Llink = (*head)->Llink;
		p->Llink->Rlink = p;
		(*head)->Llink = p;
	}
}
void printf_init()//打印空闲块信息，len tag 地址
{
	Pword p;
	for (p = Phead;;p = get_tail(p) + 1)
	{
		if (p->tag == 0)
		{
			printf("空闲：len=%d tag=%d add=%d\n", p->len, p->tag, (unsigned int)p);
		}
		if (get_tail(p) == Ptail)
		{
			break;
		}
	}
}
void printf_busy()//打印占用块信息，len tag 地址
{
	Pword p;
	for (p = Phead;;p = get_tail(p) + 1)
	{
		if (p->tag == 1)
		{
			printf("占用：len=%d tag=%d address=%x\n", p->len, p->tag, (unsigned int)p);
		}
		if (get_tail(p) == Ptail)
		{
			break;
		}
	}
}