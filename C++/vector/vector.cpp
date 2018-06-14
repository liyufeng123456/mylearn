#include <iostream>
using namespace std;
#include"vector.h"
#include <time.h>
#include <string>


	//size表示初始的内存大小，val表示内存初始值
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
	//拷贝构造
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

//末尾添加元素    push_front   pop_front  O(n)
template<typename T, typename Allocator = myallocator<T>>
void Vector<T, Allocator>::push_back(const T &val)
{
	if (full())
		reSize();
	_allocator.construct(mpVec + mCur, val);
	mCur++;
}
	//末尾删除
template<typename T, typename Allocator = myallocator<T>>
void Vector<T, Allocator>::pop_back()
{
	if (empty())
		return;
	--mCur;
	_allocator.destroy(mpVec + mCur); //把删除的对象一定要析构
}
//内存以2倍方式增长
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
	vector容器的增加和删除
	*/
	/*
	C++17次课作业：完成下面的三个接口
	在it迭代器的位置，插入val元素
	it位置合法：it在【first，last】范围
	特殊情况：容器已经满了，插入要先扩容
	it位置不合法: 插入失败，抛异常
	*/
template<typename T, typename Allocator = myallocator<T>>
void Vector<T, Allocator>::insert(const iterator &it, const T &val)
{
	if (this->end() - it < 0 || this->begin() - it > 0)
	{
		throw "插入失败";
	}
	if (full())
	{
		reSize();
	}
	iterator it1 = this->end();
	iterator it2(this->end());
	for (;it1 != it;--it1)
	{
		//插入
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
	删除的容器，按值删除，按迭代器删除
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
				_allocator.construct(mpVec + j, mpVec[j + 1]);//在j位置放val
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

//函数模板，统一进行容器元素的遍历输出
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

	//默认从小到大排序   vector<int>::iterator  list<int>::iteartor
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
		cout << "第一个比40小的数是：" << *it<<endl;
	}*/
	/*Vector<int>::iterator it = myfind_if(vec1.begin(), vec1.end(), mybind1st(MYGreater<int>(),40));
	if (it == vec1.end())
	{
		cout << "not found !" << endl;
	}
	else
	{
		cout << "第一个比40小的数是：" << *it << endl;
	}*/
	/*Vector<int>::iterator it = myfind_if(vec1.begin(), vec1.end(), mynot1(mybind1st(MYGreater<int>(), 40)));
	if (it == vec1.end())
	{
		cout << "not found !" << endl;
	}
	else
	{
		cout << "第一个比40小的数是：" << *it << endl;
	}*/
	//
	Vector<int> vec2;
	mycopy(vec1.begin(), vec1.end(), myback_inserter(vec2));
	showContainer(vec2);
}
/*
C++ 18
迭代器模式 =》统一遍历容器 ++it
STL：
1、容器  vector（向量容器） push_back/pop_back O(1)  insert/erase O(n)
2、迭代器
3、泛型算法 =》操作容器的  模板、参数都是接受迭代器

C++ 19节课

C++ STL 库比较元素大小相关的函数对象，只有二元的函数对象
但是泛型算法，有的时候需要一元函数对象


*/