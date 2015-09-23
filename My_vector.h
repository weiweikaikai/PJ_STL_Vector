#pragma once
#include<iostream>
using namespace std;
#include"utility.h"
#include"xutility.h"
#include"xmemory.h"

#define THROW(x, y)	throw x(y)//�쳣�׳��ĺ�
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
	typedef Tptr iterator; //���������
	typedef Ctptr const_iterator;


	typedef reverse_iter<iterator, value_type,
		reference, Tptr, difference_type>
		reverse_iterator;//���������
	typedef reverse_iter<const_iterator, value_type,
		const_reference, Tptr, difference_type>
		const_reverse_iterator;

	explicit My_vector(const A &a = A()) :allocator(a), First(0), Last(0), End(0)
	{}
	explicit My_vector(size_type N, const T&v = T(), const A&a = A()) :allocator(a)
	{//����һ�δ��������ͬ��Ԫ��
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
			iterator S = My_copy(X.begin(), X.end(), First);//include "xutility"���ؿ�����Ŀռ�����һ���ֽڵ�ַ
			MyDestory(S, Last);
			Last = First + X.size();
		}
		else if (X.size() <= capacity())
		{
			const_iterator S = X.begin() + size();//���X�к�*this��ͬ��С������
			My_copy(X.begin(), S, First);//ֱ�ӿ������������µ��ù��캯��
			My_Ucopy(S, X.end(), Last);//��ʣ�µ���Щ��Ҫ���¹���Ķ����ٵ��ÿ������캯�����й���
			Last = First + X.size();
		}
		else//*this���޷��洢��X���󣬴����޷��洢���Ҫ���·���ռ�:>
		{
			MyDestory(First, Last);//��*this�еĶ����ͷ�
			allocator.My_deallocate(First, End - First);
			First = allocator.My_allocate(X.size(), (void *)0);//���·���ռ�
			Last = My_Ucopy(X.begin(), X.end(), First);//���¹������
			End = Last;
		}
		return *this;

	}
	~My_vector()
	{
		MyDestory(First, Last);
	}

	void reserve(size_t N)
	{//���������������ڴ�ռ䣬�����ڼ������ݵ�ʱ����Ϊ������һ���㹻��������С���ٵ�����ʧЧ�Ŀ���
		if (capacity() < N)
		{
			iterator S = allocator.My_allocator(N, (void*)0);//�����ռ�
			My_Ucopy(First, Last, S);//����ԭ����
			MyDestory(First, Last);//����ԭ���Ķ���
			allocator.My_deallocate(First, End - First);//�ͷ�ԭ���Ŀռ�
			End = S + N;
			Last = S + size();
			First = S;
		}
	}
	void resize(size_type N, const T& X = T())//���������е�����ΪN����ͬ��X����
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
	const_reference at(size_t P)const//����Խ������±���ʺ���
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
	size_type Maxsize()const//��������Ŷ��ٸ��������͵Ķ���
	{
		return allocator.My_maxsize();
	}
	size_type size()const//��ǰ�Ѿ�ʹ�õĿռ��С
	{
		return (First == 0) ? 0 : (Last - First);
	}
	size_type capacity()const//��ǰ����
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
	//������ƺܲ�����Ϊ�������Ӧ�ò��䣬������ִ��������������ɾ���Ĳ���
	iterator insert(iterator P, const T&X)//�����󲻿ɵ��ô˺������������ڴ˲����ĺ���������Ҳ���ɲ�������������iterator������const_iterator��ԭ��
	{
		size_type len = P - begin();
		insert(P, 1, X);//�п��ܻ�������·���ռ����������µ�����ʧЧ
		return (begin() + len);//�ټ�������ĵ�P���ǳ������ܱ��ı䣬���Բ�����ֱ�ӷ���P
	}
	void insert(iterator P, size_t M, const T&v)
	{
		if (End - Last < (int)M)//����������пռ�����·���
		{
			size_type N = size() + (M < size() ? size() : M);
			iterator S = allocator.My_allocate(N, (void*)0);
			iterator Q = My_Ucopy(First, P, S);
			My_Ufill(Q, M, v);//my_uncopy������my_ufill����������ù��캯�����ж���Ĺ���
			My_Ucopy(P, Last, Q + M);
			MyDestory(First, Last);//ԭ���ռ�Ķ����������
			allocator.My_deallocate(First, End - First);//ԭ���Ŀռ�����ͷ�
			End = S + N;
			Last = S + size() + M;
			First = S;
		}
		else if (Last - P < (int)M)//���������������пռ�����·���
		{
			My_Ucopy(P, Last, P + M);
			My_Ufill(Last, M - (Last - P), v);//��Щ�´�δ���й���Ķ�������Ҫ�������й���
			//my_uncopy������my_ufill����������ù��캯�����ж���Ĺ���
			My_fill(P, Last, v);//�������ֻ�ǽ��и�ֵ������
			Last += M;
		}
		else if (0 < M)
		{
			My_Ucopy(Last - M, Last, Last);//����last����пռ䵫����Ҫ���ù��캯���������
			My_copy_backward(P, Last - M, Last);
			My_fill(P, P + M, v);//����Ҫ���ù��캯��
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
		if (End - Last < (int)M)//�ռ䲻�㹻�������·���ռ�
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
		else if (Last - P < (int)M)//�ռ��㹻�������Ҫ�����������������еĶ�����Ҫ���¹����еĲ���Ҫ���¹���
		{
			My_Ucopy(P, Last, P + M);//�ֶν��п�����Ŀ������Ϊ�����еĶ�����Ҫ���¹��죬�еĲ���Ҫ���¹���
			My_Ucopy(F + (Last - P), L, Last);
			My_copy(F, F + (Last - P), P);//�����ù��캯��
			Last += M;
		}
		else if(0 <  M)//End-Last>=M  Last-P>=M  
		{
			My_Ucopy(Last - M, Last, Last);//���ù��캯��
			My_copy_backward(P,Last-M,Last);//��P ��Last-M�Ķ��󿽱�����Last��ʼ��ǰ���п������൱�ڽ��дӺ���ǰ���ƣ�Ϊ�¼���������ڵط�
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
	bool Eq(const Myt& X) const//�Ƚ����������ǲ������
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
	void MyDestory(iterator F, iterator L)//���ε������������ͷŶ���
	{
		for (; F != L; ++F)
		{
			allocator.My_destory(F);
		}
	}
	iterator My_Ucopy(const_iterator F, const_iterator L,iterator P)//������һ����������P��ʼ�ĵ�ַ��
	{
		for (; F != L; ++F, ++P)
		{
			allocator.My_construct(P, *F);
		}
		return(P);
	}
	void My_Ufill(iterator F, size_type N, const T&v)//�ڴ����Ŀռ��ϱ߽�����ֵ��������
	{
		for (; 0 < N; ++F, --N)
		{
			allocator.My_construct(F, v);
		}
	}
	void Xran() const//�쳣�׳�����
	{
		THROW(out_of_range, "invalid vector<T> subscript");
	}
private:
	A allocator;//����һ���ռ��������Ķ���
	iterator First, Last, End;
	//Firstִ����������ڴ�ռ�����ֽڵĵ�ַ
	//Lastָ���Ѿ���Ԫ�صĿռ�����һ���ֽڵĵ�ַ
	//Endָ���������ڴ�ռ�����һ���ֽڵĵ�ַ
};