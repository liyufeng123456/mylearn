#ifndef STACK_H
#define STACK_H
#include"MazeNode.h"
/*
ջ�Ķ���
*/
class Stack
{
public:
	Stack(int size = 10)
		: _size(size)
		, _top(-1)
	{
		_pstack = new MazeNode[_size];
	}
	~Stack()
	{
		delete[]_pstack;
	}
	void push(MazeNode node);
	
	void pop();
	
	//�������û��Ƿ���ֵ
	MazeNode getTop()const;
	
	bool empty()const;
	bool full()const;
private:
	MazeNode *_pstack;
	int _size;
	int _top;

	//��ֹ�ⲿʹ��stack�Ŀ�������͸�ֵ����ֹǳ����
	Stack(const Stack&);
	void operator=(const Stack&);

	//ջ���ڴ���������2���ķ�ʽ����
	void resize()
	{
		MazeNode *ptmp = new MazeNode[_size * 2];
		for (int i = 0; i < _size; ++i)
		{
			ptmp[i] = _pstack[i];
		}
		_size *= 2;
		delete[]_pstack;
		_pstack = ptmp;
	}
};
#endif
