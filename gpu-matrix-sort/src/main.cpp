#include <algorithm>
#include <CL/cl.hpp>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>

#include "matrix.h"
#include "utils.h"
#include "sort.h"
#include "utils.h"
#include "gpu_sort_driver.hpp"

std::string load_kernel_file(std::string_view filename)
{
	std::ifstream is{ filename.data() };
	std::string file_content;

	is.seekg(0, std::ios::end);
	file_content.reserve(is.tellg());
	is.seekg(0, std::ios::beg);

	file_content.assign(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>());

	return file_content;
}

int main()
{
	gms::matrix<double> mat{ 100, 10};
	gms::fill_matrix_with_random(mat, 0.0, 9.0);

	gms::gpu_sort_driver(mat, "src/matrix_insert_sort.cl");

	auto rows{ static_cast<int>(mat.rows()) };
	auto cols{ static_cast<int>(mat.cols()) };
	size_t bytes = rows * cols * sizeof(double);

	// Get platform and device information
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;

	//Bind to platform
	cl_int ret = clGetPlatformIDs(1, &platform_id, NULL);
	gms::check_error(ret, "clGetPlatformIDs");

	//Get ID for the device
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	gms::check_error(ret, "clGetDeviceIDs");

	// Create an OpenCL context
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	gms::check_error(ret, "clCreateContext");

	// Create a command queue
	cl_command_queue queue = clCreateCommandQueue(context, device_id, 0, &ret);
	gms::check_error(ret, "clCreateCommandQueue");

	// Create a program from the kernel source
	const auto tmp{ load_kernel_file("src/matrix_insert_sort.cl") };
	const char* tmp1{ tmp.data() };
	const char** kernel_source{ &tmp1 };

	cl_program program = clCreateProgramWithSource(context, 1, kernel_source, NULL, &ret);
	gms::check_error(ret, "clCreateProgramWithSource");

	// Build the program
	clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

    char buff[2048];
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buff), buff, nullptr);
    std::cout << buff;

	// Create the OpenCL kernel
	cl_kernel kernel = clCreateKernel(program, "matrix_insert_sort", &ret);
	gms::check_error(ret, "clCreateKernel");

	// Create memory buffers on the device for each vector 
	cl_mem matrix_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);

	// Copy our data set into the input array in device memory
	ret = clEnqueueWriteBuffer(queue, matrix_buffer, CL_TRUE, 0, bytes, mat.data(), 0, NULL, NULL);
	gms::check_error(ret);

	// Set the arguments of the kernel
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &matrix_buffer);
	ret |= clSetKernelArg(kernel, 1, sizeof(decltype(rows)), &rows);
	ret |= clSetKernelArg(kernel, 2, sizeof(decltype(cols)), &cols);
	gms::check_error(ret);

	//Number of work items in each local work group
	size_t local_item_size = 64; 

	//Number of total work items
	size_t global_item_size = ceil(cols / (float)local_item_size)*local_item_size;

	//Execute the kernel over the entire range of the data set
	ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

	//Wait for the queue to finish
	clFinish(queue);

	//Read the results
	clEnqueueReadBuffer(queue, matrix_buffer, CL_TRUE, 0, bytes, mat.data(), 0, NULL, NULL);

	//Clean up
	clFlush(queue);
	clFinish(queue);

	clReleaseMemObject(matrix_buffer);

	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	gms::print_matrix(mat);

	return 0;
}
