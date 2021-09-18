#pragma once

#include <algorithm>
#include <functional>

template <class T, class UnaryPredicate>
void filter(std::vector<T> &v, UnaryPredicate p)
{
	v.erase(std::remove_if(v.begin(), v.end(), std::not1(p)), v.end());
}