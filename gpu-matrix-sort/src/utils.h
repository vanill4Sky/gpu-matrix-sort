#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <random>
#include <type_traits>
#include <iomanip>

#include "matrix.h"

namespace gms
{

template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr>
void fill_matrix_with_random(gms::matrix<T>& matrix, T lower_bound, T upper_bound);

template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
void fill_matrix_with_random(gms::matrix<T>& matrix, T lower_bound, T upper_bound);

namespace detail
{
	template <typename DistributionType, typename T>
	void fill_matrix_with_random_impl(gms::matrix<T>& matrix, T lower_bound, T upper_bound);
}

template <typename T>
void print_matrix(gms::matrix<T>& matrix, std::string_view col_delim = ", ", std::string_view row_delim = "\n");

template <typename T>
void print_matrix_fixed(gms::matrix<T>& matrix, int upper_bound);

namespace detail
{
	int get_number_of_digit(size_t n);
}

}

template <typename T, typename std::enable_if_t<std::is_integral_v<T>>*>
void gms::fill_matrix_with_random(gms::matrix<T>& matrix, T lower_bound, T upper_bound)
{
	detail::fill_matrix_with_random_impl<std::uniform_int_distribution<T>, T>(matrix, lower_bound, upper_bound);
}

template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>*>
void gms::fill_matrix_with_random(gms::matrix<T>& matrix, T lower_bound, T upper_bound)
{
	detail::fill_matrix_with_random_impl<std::uniform_real_distribution<T>, T>(matrix, lower_bound, upper_bound);
}

template <typename DistributionType, typename T>
void gms::detail::fill_matrix_with_random_impl(gms::matrix<T>& matrix, T lower_bound, T upper_bound)
{
	assert(lower_bound <= upper_bound);

	std::mt19937 gen{ std::random_device{}() };
	DistributionType dist{ lower_bound, upper_bound };
	const auto random_value_beetwen = [&] {
		return dist(gen);
	};

	std::generate(matrix.begin(), matrix.end(), random_value_beetwen);
}

template<typename T>
void gms::print_matrix(gms::matrix<T>& matrix, std::string_view col_delim, std::string_view row_delim)
{
	for (size_t i{ 0 }; i < matrix.rows(); ++i)
	{
		auto [a, b] = matrix.row(i);
		std::copy(a, b, std::ostream_iterator<T>{ std::cout, col_delim.data() });
		std::cout << row_delim;
	}
}

template<typename T>
void gms::print_matrix_fixed(gms::matrix<T>& matrix, int upper_bound)
{
	using namespace std;

	cout.setf(ios::fixed);

	int row_digits = gms::detail::get_number_of_digit(matrix.rows());
	int col_digits = gms::detail::get_number_of_digit(upper_bound) + 7;

	cout << setw(row_digits + 1) << "|";
	for (int i = 0; i < matrix.cols(); i++)
	{
		cout << setw(col_digits) << i << "|";
	}
	cout << endl;
	for (int i = 0; i < matrix.rows(); i++)
	{
		cout << setw(row_digits) << i << "|";
		for (int j = 0; j < matrix.cols(); j++)
		{
			cout << setw(col_digits) << matrix(i, j) << " ";
		}
		cout << endl;
	}
	cout.unsetf(ios::fixed);
}

int gms::detail::get_number_of_digit(size_t n)
{
	int count = 0;
	while (n != 0)
	{
		n = n / 10;
		++count;
	}
	return count;
}