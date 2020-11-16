#pragma once

#include <string_view>

#include "matrix.h"

namespace gms
{

template <typename T>
class gpu_sort_driver
{
public:
	gpu_sort_driver(gms::matrix<T> matrix, std::string_view kernel_filename);
	~gpu_sort_driver();
	gpu_sort_driver(const gpu_sort_driver&) = delete;
	gpu_sort_driver& operator=(const gpu_sort_driver&) = delete;

	void sort();
	const gms::matrix<T>& matrix() const;

private:
	std::string load_kernel_file(std::string_view kernel_filename);

	gms::matrix<T> m_matrix;
};

}

#include "gpu_sort_driver.inl"