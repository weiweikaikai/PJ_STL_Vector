#pragma once
//utility.h

template<class RI, class D> inline
void  My_Distance(RI _F, RI _L, D& _N)
{
	_N += _L - _F;
}

template<class RI,
class T,
class Rt = T&,
class Pt = T *,
class D = int>

class reverse_iter//·´Ïòµü´úÆ÷
{
public:
	typedef reverse_iter<RI, T, Rt, Pt, D> Myt;
	typedef RI iter_type;
	typedef Rt reference_type;
	typedef Pt pointer_type;
	reverse_iter()
	{}
	explicit reverse_iter(RI _X)
		: current(_X) {}
	RI base() const
	{
		return (current);
	}
	Rt operator*() const
	{
		return (*(current - 1));
	}
	Pt operator->() const
	{
		return (&**this);
	}
	Myt& operator++()
	{
		--current;
		return (*this);
	}
	Myt operator++(int)
	{
		Myt _Tmp = *this;
		--current;
		return (_Tmp);
	}
	Myt& operator--()
	{
		++current;
		return (*this);
	}
	Myt operator--(int)
	{
		Myt _Tmp = *this;
		++current;
		return (_Tmp);
	}
	Myt& operator+=(D _N)
	{
		current -= _N;
		return (*this);
	}
	Myt operator+(D _N) const
	{
		return (Myt(current - _N));
	}
	Myt& operator-=(D _N)
	{
		current += _N;
		return (*this);
	}
	Myt operator-(D _N) const
	{
		return (Myt(current + _N));
	}
	Rt operator[](D _N) const
	{
		return (*(*this + _N));
	}
protected:
	RI current;
};