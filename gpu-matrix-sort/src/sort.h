#pragma once

#include <functional>
#include <vector>

#include "matrix.h"

/**
 * @brief Main namespace of the project.
*/
namespace gms
{

namespace order
{
	static const auto ascending = [](const auto a, const auto b) {
		return a > b;
	};

	static const auto descending = [](const auto a, const auto b) {
		return a < b;
	};
}

/**
 * @brief Sorts the elements in the range [first, last). A sequence is sorted with respect to a compare function.
 * @tparam T 
 * @param first lower bound of the range of elements to sort.
 * @param last upper bound of the range of elements to sort.
 * @param compare callable comparision object.
*/
template <typename T>
void insert_sort(
	const typename std::vector<T>::iterator first, 
	const typename std::vector<T>::iterator last, 
	const std::function<bool(const T&, const T&)>& compare);

/**
 * @brief Sorts the elements in rows of a 2d matrix. The elements are sorted with respect to a compare function.
 * @tparam T 
 * @tparam SortFunction 
 * @tparam CompareFunction 
 * @param matrix 
 * @param sort sorting function with following signature:
 * \code{.cpp}
 *		void sort(const Iterator first, const Iterator last, const CompareFunction& compare);
 * \endcode
 * @param compare comparision function with following signature:
 * \code{.cpp}
 *		bool compare(const T& a, const T& b);
 * \endcode
*/
template <typename T, typename SortFunction, typename CompareFunction>
void sort_matrix(gms::matrix<T>& matrix, const SortFunction& sort, const CompareFunction& compare);

}

#include "sort.inl"
