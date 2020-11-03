#pragma once

#include <algorithm>
#include <random>
#include <type_traits>
#include <cassert>

#include "matrix.h"

namespace gms
{

template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr>
void fill_matrix_with_random(gms::matrix<T>& matrix, T lower_bound, T upper_bound);

template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
void fill_matrix_with_random(gms::matrix<T>& matrix, T lower_bound, T upper_bound);

template <typename DistributionType, typename T>
void fill_matrix_with_random_impl(gms::matrix<T>& matrix, T lower_bound, T upper_bound);

}

template <typename T, typename std::enable_if_t<std::is_integral_v<T>>*>
void gms::fill_matrix_with_random(gms::matrix<T>& matrix, T lower_bound, T upper_bound)
{
	fill_matrix_with_random_impl<std::uniform_int_distribution<T>, T>(matrix, lower_bound, upper_bound);
}

template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>*>
void gms::fill_matrix_with_random(gms::matrix<T>& matrix, T lower_bound, T upper_bound)
{
	fill_matrix_with_random_impl<std::uniform_real_distribution<T>, T>(matrix, lower_bound, upper_bound);
}

template <typename DistributionType, typename T>
void gms::fill_matrix_with_random_impl(gms::matrix<T>& matrix, T lower_bound, T upper_bound)
{
	assert(lower_bound <= upper_bound);

	std::mt19937 gen{ std::random_device{}() };
	DistributionType dist{ lower_bound, upper_bound };
	const auto random_value_beetwen = [&] {
		return dist(gen);
	};

	std::generate(matrix.begin(), matrix.end(), random_value_beetwen);
}