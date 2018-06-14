#include<stdio.h>
#include"word.h"
#include<stdlib.h>
#include<assert.h>
extern Pword Phead;//�ڴ��ϱ߽�
extern Pword Ptail;//�ڴ��±߽�
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
		//�ӿ���������ɾ��ptr
		ptr->Llink->Rlink = ptr->Rlink;
		ptr->Rlink->Llink = ptr->Llink;
		//�ı��־λ
		ptr->tag = 1;
		get_tail(ptr)->tag = 1;
		//�жϸ��ڴ���Ƿ�Ϊ���һ�����õ��ڴ��
		if (*head == ptr)
		{
			*head = NULL;
		}
		return ptr + 1;
	}
	else
	{
		//��ȡ���в��ֵ�ָ��
		Pword pre = ptr + ptr->len - len;
		//�޸ļ��в��ֵ�ָ��
		pre->len = len;
		pre->tag = 1;
		get_tail(pre)->Uplink = pre;
		get_tail(pre)->tag = 1;
		//�޸�ʣ�ಿ���ڴ�����Ϣ
		ptr->len = ptr->len - len - 2;
		get_tail(ptr)->Uplink = ptr;
		//�����û������ռ��ָ��
		return pre + 1;
	}
}
void *get_word(Pword *head,int size)
{
	if (head == NULL || size <= 0)
	{
		return NULL;
	}
	int len = (size + 15) / 16;//����Ҫ�õ��ڴ������
	//������һ�������ڴ��
	Pword ptr;
	for ( ptr= (*head)->Llink;;ptr = ptr->Llink)
	{
		if (ptr->len >= len || ptr == (*head)->Rlink)
		{
			break;
		}
	}
	//�ж��ڴ��Ƿ����
	if (ptr->len >= len)
	{
		//�޸��ڴ���Ϣ���������ڴ��
		return change_word(head, ptr, len);
	}
	//û�п����ڴ��
	return NULL;
}
void del_word(Pword *head, void* ptr)
{
	Pword p = (Pword)ptr - 1;
	if (p = Phead)//�ͷŵ��ڴ�Ϊ�ϱ߽�
	{
		if ((get_tail(p)+1)->tag == 0)//�¿���
		{
			Pword next = get_tail(p) + 1;//���q����һ�������
			p->tag = 0;//�޸��ͷ�����Ϣ
			get_tail(p)->tag = 0;
			p->len = p->len + next->len;
			get_tail(next)->Uplink = p;//�޸�q��һ�����������Ϣ
			next->Llink->Rlink = next->Rlink;//ɾ��q����һ����
			next->Rlink->Llink = next->Llink;
			p->Rlink = *head;//�������ͷŵĿ������ӵ�����������
			p->Llink = (*head)->Llink;
			p->Llink->Rlink = p;
			(*head)->Llink = p;
		}
		else//��ռ��
		{
			p->tag = 0;//�޸��ͷ�����Ϣ
			get_tail(p)->tag = 0;
			p->Rlink = *head;//���ͷ����ӵ�����������
			p->Llink = (*head)->Llink;
			p->Llink->Rlink = p;
			(*head)->Llink = p;
		}
	}
	else if (get_tail(p) == Ptail)//�ͷ��ڴ�Ϊ�±߽�
	{
		if ((p - 1)->tag == 0)//�Ͽ���
		{
			(p - 1)->Uplink->len = (p - 1)->Uplink->len + p->len;//�ϲ��ͷ���
			p->tag = 0;//�޸��ͷ���
			get_tail(p)->tag = 0;
			get_tail(p)->Uplink = (p - 1)->Uplink;
		}
		else//��ռ��
		{
			p->tag = 0;//�޸��ͷ�����Ϣ
			get_tail(p)->tag = 0;
			p->Rlink = *head;//���ͷ����ӵ�����������
			p->Llink = (*head)->Llink;
			p->Llink->Rlink = p;
			(*head)->Llink = p;
		}
	}
	else if ((p-1)->tag == 0 && (get_tail(p)+1)->tag == 0)//�ϱ߽���У��±߽����
	{
		Pword pre = (p - 1)->Uplink;//��ȡ��һ�������
		Pword next = get_tail(p) + 1;//��ȡ��һ�������
		pre->len = pre->len + p->len + next->len;//�޸��ͷ�����Ϣ
		p->tag = 0;
		get_tail(p)->tag = 0;
		get_tail(next)->Uplink = pre;
		next->Llink->Rlink = next->Rlink;//ɾ��next���
		next->Rlink->Llink = next->Llink;
	}
	else if ((p-1)->tag == 1 && (get_tail(p) + 1)->tag == 0)//�ϱ߽�ռ�ã��±߽����
	{
		Pword next = get_tail(p) + 1;//���q����һ�������
		p->tag = 0;//�޸��ͷ�����Ϣ
		get_tail(p)->tag = 0;
		p->len = p->len + next->len;
		get_tail(next)->Uplink = p;//�޸�q��һ�����������Ϣ
		next->Llink->Rlink = next->Rlink;//ɾ��q����һ����
		next->Rlink->Llink = next->Llink;
		p->Rlink = *head;//�������ͷŵĿ������ӵ�����������
		p->Llink = (*head)->Llink;
		p->Llink->Rlink = p;
		(*head)->Llink = p;
	}
	else if ((p-1)->tag == 0 && (get_tail(p) + 1)->tag == 1)//�ϱ߽���У��±߽�ռ��
	{
		(p - 1)->Uplink->len = (p - 1)->Uplink->len + p->len;//�ϲ��ͷ���
		p->tag = 0;//�޸��ͷ���
		get_tail(p)->tag = 0;
		get_tail(p)->Uplink = (p - 1)->Uplink;
	}
	else//�ϱ߽�ռ�ã��±߽�ռ��
	{
		p->tag = 0;//�޸��ͷ�����Ϣ
		get_tail(p)->tag = 0;
		p->Rlink = *head;//���ͷ����ӵ�����������
		p->Llink = (*head)->Llink;
		p->Llink->Rlink = p;
		(*head)->Llink = p;
	}
}
void printf_init()//��ӡ���п���Ϣ��len tag ��ַ
{
	Pword p;
	for (p = Phead;;p = get_tail(p) + 1)
	{
		if (p->tag == 0)
		{
			printf("���У�len=%d tag=%d add=%d\n", p->len, p->tag, (unsigned int)p);
		}
		if (get_tail(p) == Ptail)
		{
			break;
		}
	}
}
void printf_busy()//��ӡռ�ÿ���Ϣ��len tag ��ַ
{
	Pword p;
	for (p = Phead;;p = get_tail(p) + 1)
	{
		if (p->tag == 1)
		{
			printf("ռ�ã�len=%d tag=%d address=%x\n", p->len, p->tag, (unsigned int)p);
		}
		if (get_tail(p) == Ptail)
		{
			break;
		}
	}
}