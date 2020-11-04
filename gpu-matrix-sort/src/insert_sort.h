#pragma once

#include <functional>
#include <vector>

namespace gms
{
	template <typename T>
	void insert_sort(
		const typename std::vector<T>::iterator first, 
		const typename std::vector<T>::iterator last, 
		const std::function<bool(T, T)>& compare);
}

#include "insert_sort.inl"
