#pragma once
// TEMPLATE FUNCTION copy
template<class II, class OI> inline
OI My_copy(II _F, II _L, OI _X)
{
	for (; _F != _L; ++_X, ++_F)
		*_X = *_F;
	return (_X);
}
// TEMPLATE FUNCTION copy_backward
template<class BI1, class BI2> inline
BI2 My_copy_backward(BI1 _F, BI1 _L, BI2 _X)
{
	while (_F != _L)
		*--_X = *--_L;
	return (_X);
}
// TEMPLATE FUNCTION equal
template<class II1, class II2> inline
bool My_equal(II1 _F, II1 _L, II2 _X)
{
	return (My_mismatch(_F, _L, _X).first == _L);
}
// TEMPLATE FUNCTION equal WITH PrED
template<class II1, class II2, class Pr> inline
bool My_equal(II1 _F, II1 _L, II2 _X, Pr _P)
{
	return (My_mismatch(_F, _L, _X, _P).first == _L);
}
// TEMPLATE FUNCTION fill
template<class FI, class Ty> inline
void My_fill(FI _F, FI _L, const Ty& _X)
{
	for (; _F != _L; ++_F)
		*_F = _X;
}
// TEMPLATE FUNCTION fill_n
template<class OI, class SZ, class Ty> inline
void My_fill_n(OI _F, SZ _N, const Ty& _X)
{
	for (; 0 < _N; --_N, ++_F)
		*_F = _X;
}
// TEMPLATE FUNCTION lexicographical_compare
template<class II1, class II2> inline
bool My_lexicographical_compare(II1 _F1, II1 _L1,
II2 _F2, II2 _L2)
{
	for (; _F1 != _L1 && _F2 != _L2; ++_F1, ++_F2)
	if (*_F1 < *_F2)
		return (true);
	else if (*_F2 < *_F1)
		return (false);
	return (_F1 == _L1 && _F2 != _L2);
}

template<class II1, class II2, class Pr> inline
bool My_lexicographical_compare(II1 _F1, II1 _L1,
II2 _F2, II2 _L2, Pr _P)
{
	for (; _F1 != _L1 && _F2 != _L2; ++_F1, ++_F2)
	if (_P(*_F1, *_F2))
		return (true);
	else if (_P(*_F2, *_F1))
		return (false);
	return (_F1 == _L1 && _F2 != _L2);
}

#ifndef _MAX
#define _MAX	My_cpp_max
#define _MIN	My_cpp_min
#endif
template<class Ty> inline
const Ty& My_cpp_max(const Ty& _X, const Ty& _Y)
{
	return (_X < _Y ? _Y : _X);
}
// TEMPLATE FUNCTION max WITH PrED
template<class Ty, class Pr> inline
const Ty& My_cpp_max(const Ty& _X, const Ty& _Y, Pr _P)
{
	return (_P(_X, _Y) ? _Y : _X);
}
// TEMPLATE FUNCTION min
template<class Ty> inline
const Ty& My_cpp_min(const Ty& _X, const Ty& _Y)
{
	return (_Y < _X ? _Y : _X);
}
// TEMPLATE FUNCTION min WITH PrED
template<class Ty, class Pr> inline
const Ty& My_cpp_min(const Ty& _X, const Ty& _Y, Pr _P)
{
	return (_P(_Y, _X) ? _Y : _X);
}
// TEMPLATE FUNCTION My_mismatch
template<class II1, class II2> inline
pair<II1, II2> My_mismatch(II1 _F, II1 _L, II2 _X)
{
	for (; _F != _L && *_F == *_X; ++_F, ++_X)
		;
	return (pair<II1, II2>(_F, _X));
}
// TEMPLATE FUNCTION My_mismatch WITH PrED
template<class II1, class II2, class Pr> inline
pair<II1, II2> My_mismatch(II1 _F, II1 _L, II2 _X, Pr _P)
{
	for (; _F != _L && _P(*_F, *_X); ++_F, ++_X)
		;
	return (pair<II1, II2>(_F, _X));
}
// TEMPLATE FUNCTION My_swap
template<class Ty> inline
void My_swap(Ty& _X, Ty& _Y)
{
	Ty _Tmp = _X;
	_X = _Y, _Y = _Tmp;
}