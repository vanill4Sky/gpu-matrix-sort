#include "gpu_sort_driver.hpp"

#include <algorithm>
#include <fstream>

template<typename T, typename U>
inline gms::gpu_sort_driver<T, U>::gpu_sort_driver(gms::matrix<T> matrix, std::string_view kernel_filename, U rows, U cols)
	: m_matrix{ std::move(matrix)}, m_rows{ std::move(rows)}, m_cols{ std::move(cols) }
{
	bytes = m_rows * m_cols * sizeof(double);

	// Get platform and device information
	platform_id = NULL;
	device_id = NULL;

	//Bind to platform
	ret = clGetPlatformIDs(1, &platform_id, NULL);
	gms::check_error(ret, "clGetPlatformIDs");

	//Get ID for the device
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	gms::check_error(ret, "clGetDeviceIDs");

	// Create an OpenCL context
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	gms::check_error(ret, "clCreateContext");

	// Create a command queue
	queue = clCreateCommandQueue(context, device_id, 0, &ret);
	gms::check_error(ret, "clCreateCommandQueue");

	// Create a program from the kernel source
	const auto tmp{ load_kernel_file(kernel_filename) };
	const char* tmp1{ tmp.data() };
	const char** kernel_source{ &tmp1 };

	program = clCreateProgramWithSource(context, 1, kernel_source, NULL, &ret);
	gms::check_error(ret, "clCreateProgramWithSource");

	//Number of work items in each local work group
	local_item_size = 64;

	//Number of total work items
	global_item_size = ceil(m_cols / (float)local_item_size)*local_item_size;


	// Build the program
	clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

	char buff[2048];
	clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buff), buff, nullptr);
	std::cout << buff;

	// Create the OpenCL kernel
	kernel = clCreateKernel(program, "matrix_insert_sort", &ret);
	gms::check_error(ret, "clCreateKernel");

	// Create memory buffers on the device for each vector 
	matrix_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);

	// Copy our data set into the input array in device memory
	ret = clEnqueueWriteBuffer(queue, matrix_buffer, CL_TRUE, 0, bytes, m_matrix.data(), 0, NULL, NULL);
	gms::check_error(ret);

	// Set the arguments of the kernel
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &matrix_buffer);
	ret |= clSetKernelArg(kernel, 1, sizeof(decltype(m_rows)), &m_rows);
	ret |= clSetKernelArg(kernel, 2, sizeof(decltype(m_cols)), &m_cols);
	gms::check_error(ret);

}

template<typename T, typename U>
inline gms::gpu_sort_driver<T, U>::~gpu_sort_driver()
{
	//Clean up
	clFlush(queue);
	clFinish(queue);

	clReleaseMemObject(matrix_buffer);

	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	gms::print_matrix(m_matrix);
}

template<typename T, typename U>
inline void gms::gpu_sort_driver<T, U>::sort()
{
	//Execute the kernel over the entire range of the data set
	ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

	//Wait for the queue to finish
	clFinish(queue);

	//Read the results
	clEnqueueReadBuffer(queue, matrix_buffer, CL_TRUE, 0, bytes, m_matrix.data(), 0, NULL, NULL);
}

template<typename T, typename U>
inline const gms::matrix<T>& gms::gpu_sort_driver<T, U>::get_sorted_matrix() const
{
	return m_matrix;
}

template<typename T, typename U>
inline std::string gms::gpu_sort_driver<T, U>::load_kernel_file(std::string_view kernel_filename)
{
	std::ifstream is{ kernel_filename.data() };
	std::string file_content;

	is.seekg(0, std::ios::end);
	file_content.reserve(is.tellg());
	is.seekg(0, std::ios::beg);

	file_content.assign(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>());

	return file_content;
}
