#include <iostream>
using namespace std;
#include"vector.h"
#include <time.h>
#include <string>


	//size��ʾ��ʼ���ڴ��С��val��ʾ�ڴ��ʼֵ
template<typename T, typename Allocator = myallocator<T>>
Vector<T, Allocator>::Vector(int size, const T &val = T())
	:mSize(size), mCur(size)
{
	mpVec = _allocator.allocate(mSize * sizeof(T));
	for (int i = 0; i < mSize; ++i)
	{
		_allocator.construct(mpVec + i, val);
	}
}
	//��������
template<typename T, typename Allocator = myallocator<T>>
Vector<T, Allocator>::Vector(const Vector<T> &src)
	:mSize(src.mSize), mCur(src.mCur)
{
	mpVec = _allocator.allocate(sizeof(T)*mSize);
	for (int i = 0; i < mCur; ++i)
	{
		_allocator.construct(mpVec + i, src.mpVec[i]);
	}
}
template<typename T, typename Allocator = myallocator<T>>
Vector<T>& Vector<T, Allocator>::operator=(const Vector<T> &src)
{
	if (this == &src)
		return *this;

	for (int i = 0; i < mCur; ++i)
	{
		_allocator.destroy(mpVec + i);
	}
	_allocator.deallocate(mpVec);

	mpVec = _allocator.allocate(sizeof(T)*mSize);
	for (int i = 0; i < mCur; ++i)
	{
		_allocator.construct(mpVec + i, src.mpVec[i]);
	}

	return *this;
}
template<typename T, typename Allocator = myallocator<T>>
Vector<T, Allocator>::~Vector()
{
	for (int i = 0; i < mCur; ++i)
	{
		_allocator.destroy(mpVec + i);
	}
	_allocator.deallocate(mpVec);
	mpVec = NULL;
}

//ĩβ���Ԫ��    push_front   pop_front  O(n)
template<typename T, typename Allocator = myallocator<T>>
void Vector<T, Allocator>::push_back(const T &val)
{
	if (full())
		reSize();
	_allocator.construct(mpVec + mCur, val);
	mCur++;
}
	//ĩβɾ��
template<typename T, typename Allocator = myallocator<T>>
void Vector<T, Allocator>::pop_back()
{
	if (empty())
		return;
	--mCur;
	_allocator.destroy(mpVec + mCur); //��ɾ���Ķ���һ��Ҫ����
}
//�ڴ���2����ʽ����
template<typename T, typename Allocator = myallocator<T>>
void Vector<T, Allocator>::reSize()
{
	if (mSize == 0)
	{
		mpVec = _allocator.allocate(sizeof(T));
		mSize = 1;
		mCur = 0;
	}
	else
	{
		T *ptmp = _allocator.allocate(mSize * 2 * sizeof(T));
		for (int i = 0; i < mCur; ++i)
		{
			_allocator.construct(ptmp + i, mpVec[i]);
		}
		mSize *= 2;
		for (int i = 0; i < mCur; ++i)
		{
			_allocator.destroy(mpVec + i);
		}
		_allocator.deallocate(mpVec);
		mpVec = ptmp;
	}
}
	/*
	vector���������Ӻ�ɾ��
	*/
	/*
	C++17�ο���ҵ���������������ӿ�
	��it��������λ�ã�����valԪ��
	itλ�úϷ���it�ڡ�first��last����Χ
	��������������Ѿ����ˣ�����Ҫ������
	itλ�ò��Ϸ�: ����ʧ�ܣ����쳣
	*/
template<typename T, typename Allocator = myallocator<T>>
void Vector<T, Allocator>::insert(const iterator &it, const T &val)
{
	if (this->end() - it < 0 || this->begin() - it > 0)
	{
		throw "����ʧ��";
	}
	if (full())
	{
		reSize();
	}
	iterator it1 = this->end();
	iterator it2(this->end());
	for (;it1 != it;--it1)
	{
		//����
		it2 = it1;
		++it2;
		*it2 = *it1;
	}
	it2 = it1;
	++it2;
	*it2 = *it1;
	*it1 = val;
	mCur++;
}
	/*
	ɾ������������ֵɾ������������ɾ��
	*/
template<typename T, typename Allocator = myallocator<T>>
void Vector<T, Allocator>::erase(const T &val)
{
	if (empty())
	{
		return;
	}
	for (int i = 0; i < mCur; ++i)
	{
		if (mpVec[i] == val)
		{
			for (int j = i;j < mCur;++j)
			{
				_allocator.construct(mpVec + j, mpVec[j + 1]);//��jλ�÷�val
			}
			this->pop_back();
		}
	}
}
template<typename T, typename Allocator = myallocator<T>>
void Vector<T, Allocator>::erase(const iterator &it)
{
	if (empty())
	{
		return;
	}
	iterator it1 = it;
	for (;it1 != this->end();++it1)
	{
		iterator it2 = it1;
		++it2;
		*it1 = *it2;
	}
	this->pop_back();
}

//����ģ�壬ͳһ��������Ԫ�صı������
template<typename Container>
void showContainer(Container &con)
{
	Container::iterator it = con.begin();
	for (; it != con.end(); ++it)
	{
		cout << *it << " ";
	}
	cout << endl;
}

int main()
{
	Vector<int> vec1;

	srand(time(0));
	for (int i = 0; i < 20; ++i) {
		vec1.push_back(rand() % 100);
	}
	showContainer(vec1);

	//Ĭ�ϴ�С��������   vector<int>::iterator  list<int>::iteartor
	//mysort(vec1.begin(), vec1.end());
	//showContainer(vec1); 

	/*vec1.erase(vec1.begin() + 1);
	showContainer(vec1);

	vec1.insert(vec1.begin(), 6);
	showContainer(vec1);
	
	vec1.insert(vec1.end(), 7);
	showContainer(vec1);*/
	
	/*mysort(vec1.begin(), vec1.end(),greatpfunc<int>);
	showContainer(vec1);

	mysort(vec1.begin(), vec1.end(), lesspfunc<int>);
	showContainer(vec1);*/

	//mysort(vec1.begin(), vec1.end(), MYGreater<int>());
	//showContainer(vec1);

	mysort(vec1.begin(), vec1.end(), MYLess<int>());
	showContainer(vec1);

	/*Vector<int>::iterator it = myfind_if(vec1.begin(), vec1.end(),TestLess<int>(40));
	if (it == vec1.end())
	{
		cout << "not found !" << endl;
	}
	else
	{
		cout << "��һ����40С�����ǣ�" << *it<<endl;
	}*/
	/*Vector<int>::iterator it = myfind_if(vec1.begin(), vec1.end(), mybind1st(MYGreater<int>(),40));
	if (it == vec1.end())
	{
		cout << "not found !" << endl;
	}
	else
	{
		cout << "��һ����40С�����ǣ�" << *it << endl;
	}*/
	/*Vector<int>::iterator it = myfind_if(vec1.begin(), vec1.end(), mynot1(mybind1st(MYGreater<int>(), 40)));
	if (it == vec1.end())
	{
		cout << "not found !" << endl;
	}
	else
	{
		cout << "��һ����40С�����ǣ�" << *it << endl;
	}*/
	//
	Vector<int> vec2;
	mycopy(vec1.begin(), vec1.end(), myback_inserter(vec2));
	showContainer(vec2);
}
/*
C++ 18
������ģʽ =��ͳһ�������� ++it
STL��
1������  vector������������ push_back/pop_back O(1)  insert/erase O(n)
2��������
3�������㷨 =������������  ģ�塢�������ǽ��ܵ�����

C++ 19�ڿ�

C++ STL ��Ƚ�Ԫ�ش�С��صĺ�������ֻ�ж�Ԫ�ĺ�������
���Ƿ����㷨���е�ʱ����ҪһԪ��������


*/