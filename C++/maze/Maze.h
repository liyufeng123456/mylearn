#ifndef MAZE_H
#define MAZE_H
#include"MazeNode.h"
#include"Stack.h"
class Maze
{
public:
	Maze(int r, int c)
		: _row(r)
		, _col(c)
	{
		_pMaze = new MazeNode*[_row];
		for (int i = 0; i < _row; ++i)
		{
			_pMaze[i] = new MazeNode[_col];
			for (int j = 0; j < _col; ++j)
			{
				_pMaze[i][j].setX(i);
				_pMaze[i][j].setY(j);
				//��·���ڵ����з��򶼳�ʼ���ɲ�����
				for (int k = 0; k < WAY_SIZE; ++k)
				{
					_pMaze[i][j].setWayState(k, WALK_ERR);
				}
			}
		}
	}

	void initMazeNodeValue(int i, int j, int val);
	

	//��ʼ�������Թ�·���ڵ���������ҵ�����״̬��Ϣ
	void initMazeNodeState();
	
	void findMazePath();
	
	void showMazePath();
	
private:
	int _row;
	int _col;
	MazeNode **_pMaze;
	Stack _stack;
};
#endif
