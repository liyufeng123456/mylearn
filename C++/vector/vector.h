#ifndef VECTOR_H
#define VECTOR_H

template<typename T>
class myallocator
{
public:


	void construct(void *ptr, const T &val)
	{
		new (ptr) T(val);  //定位new 只分配内存不产生对象

	}
	void destroy(T *ptr)
	{
		ptr->~T();
	}
	//开辟内存
	T* allocate(size_t size)
	{
		return (T*)malloc(size);
	}
	//释放内存
	void deallocate(void *ptr)
	{
		free(ptr);
	}
};

template<typename T, typename Allocator = myallocator<T>>
class Vector
{
public:
	typedef T value_type;
	//默认构造的vector，底层没分配过内存0
	Vector() :mpVec(NULL), mSize(0), mCur(0) {}
	//size表示初始的内存大小，val表示内存初始值
	Vector(int size, const T &val = T());

	//拷贝构造
	Vector(const Vector<T> &src);

	Vector<T>& operator=(const Vector<T> &src);

	~Vector();

	//末尾添加元素    push_front   pop_front  O(n)
	void push_back(const T &val);

	//末尾删除
	void pop_back();

	T front()const { return mpVec[0]; }
	T back()const { return mpVec[mCur - 1]; }

	bool full()const { return mCur == mSize; }
	bool empty()const { return mCur == 0; }

	T& operator[](int index) { return mpVec[index]; }

	//内存以2倍方式增长
	void reSize();

	int size()const { return mCur; }

	//定义当前容器的迭代器类型  的作用，就是来遍历容器的(遍历容器底层的数据结构)
	class iterator
	{
	public:
		typedef T vaule_type;
		iterator(T *p = NULL)
		{
			ptr = p;
		}
		bool operator!=(const iterator &it)
		{
			return ptr != it.ptr;
		}
		bool operator==(const iterator &it)
		{
			return ptr == it.ptr;
		}
		int operator-(const iterator &it)
		{
			return ptr - it.ptr;
		}
		iterator operator+(const int val)
		{
			return iterator(ptr + val);
		}
		void operator++()
		{
			++ptr;
		}
		void operator--()
		{
			--ptr;
		}
		iterator operator-(int index)
		{
			return ptr - index;
		}

		T& operator*()
		{
			return *ptr;
		}
		T& operator[](int index)
		{
			return ptr[index];
		}
	private:
		T *ptr;
	};

	iterator begin() { return iterator(mpVec); }
	iterator end() { return iterator(mpVec + mCur); }

	void insert(const iterator &it, const T &val);
	/*
	删除的容器
	*/
	void erase(const T &val);//按值删除

	void erase(const iterator &it);//按迭代器删除

private:
	T *mpVec;
	int mSize;
	int mCur;
	Allocator _allocator;
};


//重载的泛型算法mysort，提供用户自定义的排序方式
//该mysort重载版本，分别对容器的元素进行小到大，大到小的排序并且打印出来
template<typename T>
bool greatpfunc(T a, T b)//pfunc  bool (*)(int,int) 不能确定是哪个函数 无法内联
{
	return a > b;
}
template<typename T>
bool lesspfunc(T a, T b)//pfunc  bool (*)(int,int) 不能确定是哪个函数 无法内联
{
	return a < b;
}
//一元函数对象
template<typename T>
class TestLess
{
public:
	TestLess(T v = T()) :val(v) {}
	bool operator()(const T &a)
	{
		return a < val;
	}
private:
	int val;
};

//和上面的两个函数指针类似，只不过要实现成类
template<typename T>
class MYBase
{
public:
	typedef T value_type;
};
template<typename T>
class MYGreater : public MYBase<T>
{
public:
	bool operator()(T a, T b)
	{
		return a > b;
	}
private:
	int count;
};

template<typename T>
class MYLess : public MYBase<T>
{
public:
	bool operator()(const T &a, const T &b)
	{
		return a < b;
	}
private:
	int count;
};
template<typename iteratorT,typename Compare>
void mysort(iteratorT first, iteratorT last, Compare &pfunc)
{
	int size = last - first;
	for (int i = 0;i < size -1; ++i)
	{
		
		for (int j= 0;j < size -1 -i;++j)
		{
			//if (first[j] > first[j+1])
			//1、引入C的函数指针来解决问题，缺陷：效率太低了，无法内联 =》目的：能内联
			//2、记录回调函数其它的一些信息：调用次数，调用的时间/日期 =》目的：方便记录更多的信息
			if(pfunc(first[j],first[j + 1]))//pfunc函数对象 pfunc.operator()(a,b);
			{
				//保留模板类型，
				iteratorT::vaule_type tmp = first[j];
				first[j] = first[j+1];
				first[j+1] = tmp;
			}
		}
	}
}
//泛型算法 
template<typename iteratorT, typename T>
iteratorT& myfind(iteratorT first, iteratorT last,const T &val)
{
	
	for (;first != last;++first)
	{
		if (*first == val)
		{
			return first;
		}
		else
		{
			return last;
		}
	}
}
template<typename iteratorT, typename Compare>
iteratorT myfind_if(iteratorT first, iteratorT last, Compare &comp)
{
	for (;first != last;++first)
	{
		if (comp(*first))//comp.operator()(*first)
		{
			return first;
		}
	}
	return last;
}
//绑定器的实现
template<typename Compare,typename T>
class _mybind1st 
{
public:
	_mybind1st(Compare &comp,const T &val) :_comp(comp), _val(val) {}
	bool operator()(const T &a)
	{
		return _comp.operator()(_val,a);
	}
private:
	Compare &_comp;
	T _val;
};

template<typename Compare, typename T>
_mybind1st<Compare, T>mybind1st(Compare &comp,const T &val)
{
	return _mybind1st<Compare, T>(comp,val);
}
//取反器的实现
template<typename Compare>
class _mynot1
{
public:
	_mynot1(Compare &comp):_comp(comp) {}
	bool operator()(const Compare &val)
	{
		return !_comp(val);
	}
private:
	Compare *_comp;
};
template<typename Compare>
_mynot1<Compare>mynot1(Compare &comp)
{
	return _mynot1<Compare>(comp);
}
template<typename iterator1,typename iterator2>
void mycopy(iterator1 first, iterator1 last, iterator2 dest)
{
	while (first != last)
	{
		*dest = *first;
		++first;
		++dest;
	}
}
//mycopy(vec1.begin(),vec1.end(),myback_inserter(vec2))
template<typename Container>
class _myback_inserter
{
public:
	typedef typename Container::value_type T;
	_myback_inserter(Container &con) :_con(con) {}
	_myback_inserter<Container>& operator*()
	{
		return *this;
	}
	_myback_inserter<Container>& operator=(const T &val)
	{
		_con.push_back(val);
		return *this;
	}
	_myback_inserter<Container>& operator++()
	{
		return *this;
	}
private:
	Container &_con;
};
template<typename Container>
_myback_inserter<Container> myback_inserter(Container &con)
{
	return _myback_inserter<Container>(con);
}
//函数模板，统一进行容器元素的遍历输出
template<typename Container>
void showContainer(Container &con);

//函数对象是不可能单独使用的，它主要适用于泛型算法，用来扩展泛型算法的行为。
/*迭代器：
正向迭代器         
反向迭代器
*/
#endif // !VECTOR_H

