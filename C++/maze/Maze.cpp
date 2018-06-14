#include"MazeNode.h"
#include"Stack.h"
#include"Maze.h"
#include<iostream>
using namespace std;

void Maze::initMazeNodeValue(int i, int j, int val)
{
	_pMaze[i][j].setVal(val);
}

//��ʼ�������Թ�·���ڵ���������ҵ�����״̬��Ϣ
void Maze::initMazeNodeState()
{
	for (int i = 0; i < _row; ++i)
	{
		for (int j = 0; j < _col; ++j)
		{
			//�鿴��ǰ�ڵ����������ĸ������ֵ���޸���Ӧ��״̬
			if (i > 0 && _pMaze[i - 1][j].getVal() == 0)
			{
				_pMaze[i][j].setWayState(WAY_UP, WALK_OK);
			}

			if (i <_row - 1 && _pMaze[i + 1][j].getVal() == 0)
			{
				_pMaze[i][j].setWayState(WAY_DOWN, WALK_OK);
			}

			if (j > 0 && _pMaze[i][j - 1].getVal() == 0)
			{
				_pMaze[i][j].setWayState(WAY_LEFT, WALK_OK);
			}

			if (j < _col - 1 && _pMaze[i][j + 1].getVal() == 0)
			{
				_pMaze[i][j].setWayState(WAY_RIGHT, WALK_OK);
			}
		}
	}
}

void Maze::findMazePath()
{
	_stack.push(_pMaze[0][0]);

	while (!_stack.empty())
	{
		MazeNode node = _stack.getTop();
		int i = node.getX();
		int j = node.getY();
		//�ҵ����½ǵ�·���ڵ���
		if (node.getX() == _row - 1 && node.getY() == _col - 1)
		{
			break;
		}

		//��
		if (node.getWayState(WAY_RIGHT) == WALK_OK)
		{
			node.setWayState(WAY_RIGHT, WALK_ERR);
			_pMaze[i][j + 1].setWayState(WAY_LEFT, WALK_ERR);
			_stack.push(_pMaze[i][j + 1]);
			continue;
		}
		//��
		if (node.getWayState(WAY_RIGHT) == WALK_OK)
		{
			node.setWayState(WAY_RIGHT, WALK_ERR);
			_pMaze[i+1][j].setWayState(WAY_LEFT, WALK_ERR);
			_stack.push(_pMaze[i+1][j]);
			continue;
		}
		//��
		if (node.getWayState(WAY_RIGHT) == WALK_OK)
		{
			node.setWayState(WAY_RIGHT, WALK_ERR);
			_pMaze[i][j - 1].setWayState(WAY_LEFT, WALK_ERR);
			_stack.push(_pMaze[i][j - 1]);
			continue;
		}
		//��
		if (node.getWayState(WAY_RIGHT) == WALK_OK)
		{
			node.setWayState(WAY_RIGHT, WALK_ERR);
			_pMaze[i-1][j].setWayState(WAY_LEFT, WALK_ERR);
			_stack.push(_pMaze[i-1][j]);
			continue;
		}

		_stack.pop();
	}
}

void Maze::showMazePath()
{
	if (_stack.empty())
	{
		cout << "��ǰ�Թ������ڿ�ͨ����·��" << endl;
	}
	else
	{
		//��ӡ�Թ�·��������·���ڵ��node��valueֵ��ӡ��*

		while (!_stack.empty())
		{
			int val;
			MazeNode p = _stack.getTop();
			_pMaze[p.getX()][p.getY()].setVal(val) == WALK_OK;
			_stack.pop();
		}
		cout << "·�����£�" << endl;
		for (int i = 0;i < _row;++i)
		{
			for (int j = 0;j < _col;++j)
			{
				if (_pMaze[i][j].getWayState)
				{
					cout << "*" << " ";
				}
				else
				{
					cout << _pMaze[i][j].getval() << " ";
				}
			}
			cout << endl;
		}
		cout << endl;
	}
}