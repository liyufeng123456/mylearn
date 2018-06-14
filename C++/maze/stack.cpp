
#include"Stack.h"
#include"MazeNode.h"
void Stack::push(MazeNode node)
{
	if (full())
		resize();
	_pstack[++_top] = node;
}
void Stack::pop()
{
	if (empty())
		return;
	--_top;
}
//返回引用还是返回值
MazeNode Stack::getTop()const
{
	return _pstack[_top];
}
bool Stack::empty()const { return _top == -1; }
bool Stack::full()const { return _top == _size; }