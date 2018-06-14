#include<iostream>
using namespace std;
#include"MazeNode.h"
#include"Stack.h"



int main()
{
	int row, col;
	int data;

	cout << "�������Թ���������(m n)��";
	cin >> row >> col;

	Maze maze(row, col);

	cout << "�������Թ���·��:" << endl;
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			cin >> data;
			maze.initMazeNodeValue(i, j, data);
		}
	}

	//��ʼ�����е�·����Ϣ
	maze.initMazeNodeState();
	//��ʼѰ���Թ�·��
	maze.findMazePath();
	//��ӡ���
	maze.showMazePath();
};
