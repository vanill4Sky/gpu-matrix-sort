#include <cassert>
#include "sort.h"

template<typename T>
inline void gms::insert_sort(
	const typename std::vector<T>::iterator first,
	const typename std::vector<T>::iterator last,
	const std::function<bool(const T&, const T&)>& compare)
{
	if (first == last)
	{
		return;
	}
	
	for (auto it{ first + 1 }; it != last; ++it)
	{
		const auto key{ *it };

		auto prev{ it };
		while ((prev != first) && compare(*(prev - 1), key))
		{
			*prev = *(prev - 1);
			--prev;
		}
		*prev = key;
	}
}


template<typename T, typename SortFunction, typename CompareFunction>
void gms::sort_matrix(gms::matrix<T>& matrix, const SortFunction& sort, const CompareFunction& compare)
{
	for (size_t i{ 0 }; i < matrix.rows(); ++i)
	{
		auto [a, b] = matrix.row(i);
		sort(a, b, compare);
	}
}

template<typename T, typename CompareFunction>
bool gms::validate_sorting(const gms::matrix<T>& matrix, const CompareFunction& compare)
{
	for (size_t i{ 0 }; i < matrix.rows(); ++i)
	{
		auto [a, b] = matrix.row(i);
		
		for (auto it{ a }; it != (b - 1); ++it)
		{
			if (compare(*it, *(it + 1)))
			{
				return false;
			}
		}
	}

	return true;
}
