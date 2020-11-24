#pragma once

#include <string_view>

#include "matrix.h"

namespace gms
{

template <typename T>
class gpu_sort_driver
{
public:
	gpu_sort_driver(gms::matrix<T>&& matrix, std::string_view kernel_filename, size_t work_group_size);
	~gpu_sort_driver();
	gpu_sort_driver(const gpu_sort_driver&) = delete;
	gpu_sort_driver& operator=(const gpu_sort_driver&) = delete;

	void sort();
	const gms::matrix<T>& get_matrix() const;
	gms::matrix<T>& get_matrix();

private:
	std::string load_kernel_file(std::string_view kernel_filename);

	gms::matrix<T> m_matrix;

	cl_platform_id platform_id;
	cl_device_id device_id;
	cl_int ret;
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	size_t local_item_size;
	size_t global_item_size;
	cl_kernel kernel;
	cl_mem matrix_buffer;
};

}

#include "gpu_sort_driver.inl"