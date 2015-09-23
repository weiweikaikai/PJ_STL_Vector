#pragma once
#include<iostream>
using namespace std;
#include"utility.h"
#include"xutility.h"
#include"xmemory.h"

#define THROW(x, y)	throw x(y)//异常抛出的宏
template<typename T,typename A= My_allocator<T> >
class My_vector
{
public:
	typedef My_vector<T, A>  Myt;
	typedef typename A::size_type size_type;
	typedef typename A::difference_type difference_type;
	typedef typename A::value_type value_type;
	typedef typename A::pointer Tptr;
	typedef typename A::const_pointer Ctptr;
	typedef typename A::reference reference;
	typedef typename A::const_reference const_reference;
	typedef A allocator_type;
	typedef Tptr iterator; //随机迭代器
	typedef Ctptr const_iterator;


	typedef reverse_iter<iterator, value_type,
		reference, Tptr, difference_type>
		reverse_iterator;//反向迭代器
	typedef reverse_iter<const_iterator, value_type,
		const_reference, Tptr, difference_type>
		const_reverse_iterator;

	explicit My_vector(const A &a = A()) :allocator(a), First(0), Last(0), End(0)
	{}
	explicit My_vector(size_type N, const T&v = T(), const A&a = A()) :allocator(a)
	{//可以一次创建多个相同的元素
		First = allocator.My_allocate(N, (void*)0);
		My_Ufill(First, N, v);
		Last = First + N;
		End = Last;
	}//
	My_vector(const_iterator F, const_iterator L, const A&a = A()) :allocator(a), First(0), Last(0), End(0)
	{
		insert(begin(), F, L);
	}
	My_vector(const Myt&X) :allocator(X.allocator)
	{
		First = allocator.My_allocate(X.size(), (void*)0);
		Last = My_Ucopy(X.begin(), X.end(), First);
		End = Last;
	}
	My_vector& operator=(const Myt&X)
	{
		if (this == &X)
			;
		else if (X.size() <= size())
		{
			iterator S = My_copy(X.begin(), X.end(), First);//include "xutility"返回拷贝后的空间的最后一个字节地址
			MyDestory(S, Last);
			Last = First + X.size();
		}
		else if (X.size() <= capacity())
		{
			const_iterator S = X.begin() + size();//获得X中和*this相同大小的数据
			My_copy(X.begin(), S, First);//直接拷贝，无需重新调用构造函数
			My_Ucopy(S, X.end(), Last);//把剩下的那些需要重新构造的对象再调用拷贝构造函数进行构造
			Last = First + X.size();
		}
		else//*this中无法存储下X对象，此庙无法存储大佛，要重新分配空间:>
		{
			MyDestory(First, Last);//将*this中的对象释放
			allocator.My_deallocate(First, End - First);
			First = allocator.My_allocate(X.size(), (void *)0);//重新分配空间
			Last = My_Ucopy(X.begin(), X.end(), First);//重新构造对象
			End = Last;
		}
		return *this;

	}
	~My_vector()
	{
		MyDestory(First, Last);
	}

	void reserve(size_t N)
	{//重新设置向量的内存空间，建议在加入数据的时候人为的重置一下足够的向量大小减少迭代器失效的可能
		if (capacity() < N)
		{
			iterator S = allocator.My_allocator(N, (void*)0);//创建空间
			My_Ucopy(First, Last, S);//拷贝原来的
			MyDestory(First, Last);//析构原来的对象
			allocator.My_deallocate(First, End - First);//释放原来的空间
			End = S + N;
			Last = S + size();
			First = S;
		}
	}
	void resize(size_type N, const T& X = T())//重置向量中的数据为N个相同的X对象
	{
		if (size() < N)
		{
			insert(end(), N - size(), X);
		}
		else if (N < size())
		{
			erase(begin() + N, end());
		}

	}
	iterator begin()
	{
		return First;
	}
	iterator end()
	{
		return End;
	}
	const_iterator begin()const
	{
		return First;
	}
	const_iterator end()const
	{
		return End;
	}
	reverse_iterator rbegin()
	{
		return (reverse_iterator(end()));
	}
	reverse_iterator rend()
	{
		return (reverse_iterator(begin()));
	}
	const_reverse_iterator rbegin()const
	{
		return (const_reverse_iterator(end()));
	}
	const_reverse_iterator rend()const
	{
		return (const_reverse_iterator(begin()));
	}
	reference at(size_t P)
	{
		if (size() <= P)
			xran();
		return (*(begin() + P));
	}
	const_reference at(size_t P)const//进行越界检查的下标访问函数
	{
		if (size() <= P)
			xran();
		return (*(begin() + P));
	}
	reference operator[](size_t P)
	{
		return (*(begin() + P));
	}
	const_reference operator[](size_t P)const
	{
		return (*(begin() + P));
	}
	reference front()
	{
		return (*begin());
	}
	const_reference front() const
	{
		return (*begin());
	}
	reference back()
	{
		return (*(end() - 1));
	}
	const_reference back() const
	{
		return (*(end() - 1));
	}
	size_type Maxsize()const//最大允许存放多少个此种类型的对象
	{
		return allocator.My_maxsize();
	}
	size_type size()const//当前已经使用的空间大小
	{
		return (First == 0) ? 0 : (Last - First);
	}
	size_type capacity()const//当前容量
	{
		return (First == 0) ? 0 : (End - First);
	}
	bool empty() const
	{
		return (size() == 0);
	}
	A get_allocator() const
	{
		return (allocator);
	}
	void push_back(const T&X)
	{
		insert(end(), X);
	}
	//这种设计很不错，因为常对象就应该不变，不可以执行向里面插入或者删除的操作
	iterator insert(iterator P, const T&X)//常对象不可调用此函数所有依赖于此操作的函数常对象也不可操作，就是这里iterator不能是const_iterator的原因
	{
		size_type len = P - begin();
		insert(P, 1, X);//有可能会出现重新分配空间的情况，导致迭代器失效
		return (begin() + len);//再加上这里的的P不是常量可能被改变，所以不可以直接返回P
	}
	void insert(iterator P, size_t M, const T&v)
	{
		if (End - Last < (int)M)//这个条件进行空间的重新分配
		{
			size_type N = size() + (M < size() ? size() : M);
			iterator S = allocator.My_allocate(N, (void*)0);
			iterator Q = My_Ucopy(First, P, S);
			My_Ufill(Q, M, v);//my_uncopy函数和my_ufill函数都会调用构造函数进行对象的构造
			My_Ucopy(P, Last, Q + M);
			MyDestory(First, Last);//原来空间的对象进行析构
			allocator.My_deallocate(First, End - First);//原来的空间进行释放
			End = S + N;
			Last = S + size() + M;
			First = S;
		}
		else if (Last - P < (int)M)//这个条件中无需进行空间的重新分配
		{
			My_Ucopy(P, Last, P + M);
			My_Ufill(Last, M - (Last - P), v);//这些事从未进行构造的对象，所以要单独进行构造
			//my_uncopy函数和my_ufill函数都会调用构造函数进行对象的构造
			My_fill(P, Last, v);//构造过的只是进行赋值即可以
			Last += M;
		}
		else if (0 < M)
		{
			My_Ucopy(Last - M, Last, Last);//放在last后边有空间但是需要调用构造函数构造对象
			My_copy_backward(P, Last - M, Last);
			My_fill(P, P + M, v);//不需要调用构造函数
			Last += M;
		}
	}

	void pop_back()
	{
		erase(end() - 1);
	}
	iterator erase(iterator P)
	{
		copy(P + 1, end(), P);
	}
	void assign(const_iterator F, const_iterator L)
	{
		erase(begin(), end());
		insert(begin(), F, L);
	}
	iterator erase(iterator F, iterator L)
	{
		iterator S = copy(L, end(), F);
		MyDestroy(S, end());
		Last = S;
		return (F);
	}
	void insert(iterator P, iterator F, iterator L)
	{
		size_t M = 0;
		My_Distance(F, L, M);
		if (End - Last < (int)M)//空间不足够进行重新分配空间
		{
			size_t N = size() + (size() < M ? M : size());
			iterator S = allocator.My_allocate(N, (void*)0);
			iterator Q = My_Ucopy(First, P, S);
			Q = My_Ucopy(F, L, Q);
			My_Ucopy(P, Last, Q);
			MyDestory(First, Last);
			allocator.My_deallocate(First, End - First);
			End = S + N;
			Last = S + size() + M;
			First = S;
		}
		else if (Last - P < (int)M)//空间足够的情况下要拷贝进来的向量中有的对象需要重新构造有的不需要重新构造
		{
			My_Ucopy(P, Last, P + M);//分段进行拷贝的目的是因为其中有的对象需要重新构造，有的不需要重新构造
			My_Ucopy(F + (Last - P), L, Last);
			My_copy(F, F + (Last - P), P);//不调用构造函数
			Last += M;
		}
		else if(0 <  M)//End-Last>=M  Last-P>=M  
		{
			My_Ucopy(Last - M, Last, Last);//调用构造函数
			My_copy_backward(P,Last-M,Last);//将P 到Last-M的对象拷贝到从Last开始向前进行拷贝，相当于进行从后向前后移，为新加入的数据腾地方
			My_copy(F, L, P);
		    Last += M;
	    }
	}

	void assign(size_type N, const T &X)
	{
		erase(begin(), end());
		insert(begin(), N, X);
	}
	void clear()
	{
		erase(begin(), end());
	}
	bool Eq(const Myt& X) const//比较两个对象是不是相等
	{
		return (size() == X.size()
			&& My_equal(begin(), end(), X.begin()));
	}
	bool Lt(const Myt& X) const
	{
		return (My_lexicographical_compare(begin(), end(),
			X.begin(), X.end()));
	}
	void swap(Myt& X)
	{
		if (allocator == X.allocator)
		{
			std::swap(First, X.First);
			std::swap(Last, X.Last);
			std::swap(End, X.End);
		}
		else
		{
			Myt Ts = *this; *this = X, X = Ts;
		}
	}
	friend void swap(Myt& X, Myt& Y)
	{
		X.swap(Y);
	}
protected:
	void MyDestory(iterator F, iterator L)//依次调用析构函数释放对象
	{
		for (; F != L; ++F)
		{
			allocator.My_destory(F);
		}
	}
	iterator My_Ucopy(const_iterator F, const_iterator L,iterator P)//拷贝这一段向量到以P开始的地址中
	{
		for (; F != L; ++F, ++P)
		{
			allocator.My_construct(P, *F);
		}
		return(P);
	}
	void My_Ufill(iterator F, size_type N, const T&v)//在创建的空间上边进行数值对象的填充
	{
		for (; 0 < N; ++F, --N)
		{
			allocator.My_construct(F, v);
		}
	}
	void Xran() const//异常抛出函数
	{
		THROW(out_of_range, "invalid vector<T> subscript");
	}
private:
	A allocator;//包含一个空间配置器的对象
	iterator First, Last, End;
	//First执行这块向量内存空间的首字节的地址
	//Last指向已经有元素的空间的最后一个字节的地址
	//End指向的是这块内存空间的最后一个字节的地址
};