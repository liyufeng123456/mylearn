
#ifndef MazeNode_H
#define MazeNode_H

/*
常量定义
代表四个方向
*/
const int WAY_RIGHT = 0;
const int WAY_DOWN = 1;
const int WAY_LEFT = 2;
const int WAY_UP = 3;
const int WAY_SIZE = 4;

/*
代表方向行走的状态
*/
const int WALK_OK = 0;
const int WALK_ERR = 1;

/*
迷宫路径节点类型的定义
*/
class MazeNode
{
public:
	MazeNode() {}

	void setX(int x) { _x = x; }
	void setY(int y) { _y = y; }
	void setVal(int val) { _val = val; }
	void setWayState(int way, int state)
	{
		_wayState[way] = state;
	}

	int getX()const { return _x; }
	int getY()const { return _y; }
	int getVal()const { return _val; }
	int getWayState(int way) { return _wayState[way]; }
private:
	int _x;
	int _y;
	int _val;
	int _wayState[WAY_SIZE];
};

#endif

