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
				//把路径节点所有方向都初始化成不能走
				for (int k = 0; k < WAY_SIZE; ++k)
				{
					_pMaze[i][j].setWayState(k, WALK_ERR);
				}
			}
		}
	}

	void initMazeNodeValue(int i, int j, int val);
	

	//初始化所有迷宫路径节点的上下左右的行走状态信息
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
