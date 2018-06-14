#ifndef STACK_H
#define STACK_H
#include"MazeNode.h"
/*
栈的定义
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
	
	//返回引用还是返回值
	MazeNode getTop()const;
	
	bool empty()const;
	bool full()const;
private:
	MazeNode *_pstack;
	int _size;
	int _top;

	//防止外部使用stack的拷贝构造和赋值，防止浅拷贝
	Stack(const Stack&);
	void operator=(const Stack&);

	//栈的内存增长，以2倍的方式增长
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
