#include<iostream>
using namespace std;
#include"MazeNode.h"
#include"Stack.h"



int main()
{
	int row, col;
	int data;

	cout << "请输入迷宫的行列数(m n)：";
	cin >> row >> col;

	Maze maze(row, col);

	cout << "请输入迷宫的路径:" << endl;
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			cin >> data;
			maze.initMazeNodeValue(i, j, data);
		}
	}

	//初始化所有的路径信息
	maze.initMazeNodeState();
	//开始寻找迷宫路径
	maze.findMazePath();
	//打印结果
	maze.showMazePath();
};
