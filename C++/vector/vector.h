#ifndef VECTOR_H
#define VECTOR_H

template<typename T>
class myallocator
{
public:


	void construct(void *ptr, const T &val)
	{
		new (ptr) T(val);  //��λnew ֻ�����ڴ治��������

	}
	void destroy(T *ptr)
	{
		ptr->~T();
	}
	//�����ڴ�
	T* allocate(size_t size)
	{
		return (T*)malloc(size);
	}
	//�ͷ��ڴ�
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
	//Ĭ�Ϲ����vector���ײ�û������ڴ�0
	Vector() :mpVec(NULL), mSize(0), mCur(0) {}
	//size��ʾ��ʼ���ڴ��С��val��ʾ�ڴ��ʼֵ
	Vector(int size, const T &val = T());

	//��������
	Vector(const Vector<T> &src);

	Vector<T>& operator=(const Vector<T> &src);

	~Vector();

	//ĩβ���Ԫ��    push_front   pop_front  O(n)
	void push_back(const T &val);

	//ĩβɾ��
	void pop_back();

	T front()const { return mpVec[0]; }
	T back()const { return mpVec[mCur - 1]; }

	bool full()const { return mCur == mSize; }
	bool empty()const { return mCur == 0; }

	T& operator[](int index) { return mpVec[index]; }

	//�ڴ���2����ʽ����
	void reSize();

	int size()const { return mCur; }

	//���嵱ǰ�����ĵ���������  �����ã�����������������(���������ײ�����ݽṹ)
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
	ɾ��������
	*/
	void erase(const T &val);//��ֵɾ��

	void erase(const iterator &it);//��������ɾ��

private:
	T *mpVec;
	int mSize;
	int mCur;
	Allocator _allocator;
};


//���صķ����㷨mysort���ṩ�û��Զ��������ʽ
//��mysort���ذ汾���ֱ��������Ԫ�ؽ���С���󣬴�С�������Ҵ�ӡ����
template<typename T>
bool greatpfunc(T a, T b)//pfunc  bool (*)(int,int) ����ȷ�����ĸ����� �޷�����
{
	return a > b;
}
template<typename T>
bool lesspfunc(T a, T b)//pfunc  bool (*)(int,int) ����ȷ�����ĸ����� �޷�����
{
	return a < b;
}
//һԪ��������
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

//���������������ָ�����ƣ�ֻ����Ҫʵ�ֳ���
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
			//1������C�ĺ���ָ����������⣬ȱ�ݣ�Ч��̫���ˣ��޷����� =��Ŀ�ģ�������
			//2����¼�ص�����������һЩ��Ϣ�����ô��������õ�ʱ��/���� =��Ŀ�ģ������¼�������Ϣ
			if(pfunc(first[j],first[j + 1]))//pfunc�������� pfunc.operator()(a,b);
			{
				//����ģ�����ͣ�
				iteratorT::vaule_type tmp = first[j];
				first[j] = first[j+1];
				first[j+1] = tmp;
			}
		}
	}
}
//�����㷨 
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
//������ʵ��
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
//ȡ������ʵ��
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
//����ģ�壬ͳһ��������Ԫ�صı������
template<typename Container>
void showContainer(Container &con);

//���������ǲ����ܵ���ʹ�õģ�����Ҫ�����ڷ����㷨��������չ�����㷨����Ϊ��
/*��������
���������         
���������
*/
#endif // !VECTOR_H

