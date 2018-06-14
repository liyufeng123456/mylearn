#include"MazeNode.h"

void MazeNode::setX(int x) { _x = x; }
void MazeNode::setY(int y) { _y = y; }
void MazeNode::setVal(int val) { _val = val; }
void MazeNode::setWayState(int way, int state)
{
	_wayState[way] = state;
}

int MazeNode::getX()const { return _x; }
int MazeNode::getY()const { return _y; }
int MazeNode::getVal()const { return _val; }
int MazeNode::getWayState(int way) { return _wayState[way]; }