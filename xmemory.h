#pragma once
template<typename T>inline
T* My_Allocate(size_t N, T*)
{
	if (0 > N)
	{
		N = 0;
	}
	return (T*)operator new (N*sizeof(T));
}
template<typename T1, typename T2 >inline
void My_Construct( T1*P, const T2&v )
{
	new (P) T1(v);
}
template<class T>
void My_Destory(T *P)
{
	P->~T();
}
template<typename T>
class My_allocator
{
public:
	typedef size_t size_type;
	typedef int  difference_type;
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;

	pointer My_address(reference X) const
	{
		return (&_X);
	}
	const_pointer My_address(const_reference _X) const
	{
		return (&_X);
	}
	pointer My_allocate(size_type N,const void*)
	{
		return My_Allocate((difference_type)N, (pointer)0);
	}
	char  *My_Charalloc(size_type N)
	{
		return (My_Allocate((difference_type)N,(char*)0));
	}
	void My_deallocate(void *P, size_type)
	{
		operator delete(P);
	}
	void My_construct(pointer P,const T&v)
	{
		My_Construct(P,v);
	}
	
	void My_destory(pointer P)
	{
		My_Destory(P);
	}
	size_type My_maxsize()const
	{
		size_type N = (size_type)(-1) / sizeof(T);
		return (N > 0) ? N : 1;
	}

};
template<class Ty, class U> inline
bool operator==(const allocator<Ty>&, const allocator<U>&)
{
	return (true);
}
template<class Ty, class U> inline
bool operator!=(const allocator<Ty>&, const allocator<U>&)
{
	return (false);
}