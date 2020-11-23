#pragma once

#include <string_view>

#include "matrix.h"

namespace gms
{

struct timing_info
{
	cl_ulong execution_time{ 0 };
	cl_ulong read_time{ 0 };
	cl_ulong write_time{ 0 };
};

template <typename T>
class gpu_sort_driver
{
public:
	gpu_sort_driver(gms::matrix<T> matrix, std::string_view kernel_filename);
	~gpu_sort_driver();
	gpu_sort_driver(const gpu_sort_driver&) = delete;
	gpu_sort_driver& operator=(const gpu_sort_driver&) = delete;

	void sort();
	const gms::matrix<T>& get_matrix() const;
	gms::timing_info get_timing_info() const;

private:
	std::string load_kernel_file(std::string_view kernel_filename);
	cl_ulong get_delta_time(cl_event event) const;

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

	cl_event m_timing_event{ nullptr };
	timing_info m_timing_info;
};

}

#include "gpu_sort_driver.inl"