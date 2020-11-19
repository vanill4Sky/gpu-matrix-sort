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

/*https://stackoverflow.com/questions/24326432/convenient-way-to-show-opencl-error-codes*/
std::string get_error_string(cl_int result)
{
    switch (result) {
        // run-time and JIT compiler errors
    case 0: return "CL_SUCCESS";
    case -1: return "CL_DEVICE_NOT_FOUND";
    case -2: return "CL_DEVICE_NOT_AVAILABLE";
    case -3: return "CL_COMPILER_NOT_AVAILABLE";
    case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5: return "CL_OUT_OF_RESOURCES";
    case -6: return "CL_OUT_OF_HOST_MEMORY";
    case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case -8: return "CL_MEM_COPY_OVERLAP";
    case -9: return "CL_IMAGE_FORMAT_MISMATCH";
    case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case -11: return "CL_BUILD_PROGRAM_FAILURE";
    case -12: return "CL_MAP_FAILURE";
    case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    case -15: return "CL_COMPILE_PROGRAM_FAILURE";
    case -16: return "CL_LINKER_NOT_AVAILABLE";
    case -17: return "CL_LINK_PROGRAM_FAILURE";
    case -18: return "CL_DEVICE_PARTITION_FAILED";
    case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

        // compile-time errors
    case -30: return "CL_INVALID_VALUE";
    case -31: return "CL_INVALID_DEVICE_TYPE";
    case -32: return "CL_INVALID_PLATFORM";
    case -33: return "CL_INVALID_DEVICE";
    case -34: return "CL_INVALID_CONTEXT";
    case -35: return "CL_INVALID_QUEUE_PROPERTIES";
    case -36: return "CL_INVALID_COMMAND_QUEUE";
    case -37: return "CL_INVALID_HOST_PTR";
    case -38: return "CL_INVALID_MEM_OBJECT";
    case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case -40: return "CL_INVALID_IMAGE_SIZE";
    case -41: return "CL_INVALID_SAMPLER";
    case -42: return "CL_INVALID_BINARY";
    case -43: return "CL_INVALID_BUILD_OPTIONS";
    case -44: return "CL_INVALID_PROGRAM";
    case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
    case -46: return "CL_INVALID_KERNEL_NAME";
    case -47: return "CL_INVALID_KERNEL_DEFINITION";
    case -48: return "CL_INVALID_KERNEL";
    case -49: return "CL_INVALID_ARG_INDEX";
    case -50: return "CL_INVALID_ARG_VALUE";
    case -51: return "CL_INVALID_ARG_SIZE";
    case -52: return "CL_INVALID_KERNEL_ARGS";
    case -53: return "CL_INVALID_WORK_DIMENSION";
    case -54: return "CL_INVALID_WORK_GROUP_SIZE";
    case -55: return "CL_INVALID_WORK_ITEM_SIZE";
    case -56: return "CL_INVALID_GLOBAL_OFFSET";
    case -57: return "CL_INVALID_EVENT_WAIT_LIST";
    case -58: return "CL_INVALID_EVENT";
    case -59: return "CL_INVALID_OPERATION";
    case -60: return "CL_INVALID_GL_OBJECT";
    case -61: return "CL_INVALID_BUFFER_SIZE";
    case -62: return "CL_INVALID_MIP_LEVEL";
    case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
    case -64: return "CL_INVALID_PROPERTY";
    case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
    case -66: return "CL_INVALID_COMPILER_OPTIONS";
    case -67: return "CL_INVALID_LINKER_OPTIONS";
    case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

        // extension errors
    case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
    case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
    case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
    case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
    case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
    case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
    default: return "Unknown OpenCL error";
    }
}

void check_error(cl_int result, std::string_view message = "")
{
	if (result != CL_SUCCESS)
	{
		std::cerr << message << ": " << get_error_string(result) << '\n';
	}
}

int main()
{
    double upper_bound = 9.0;
	gms::matrix<double> mat{ 100, 10};
	gms::fill_matrix_with_random(mat, 0.0, upper_bound);

	auto rows{ static_cast<int>(mat.rows()) };
	auto cols{ static_cast<int>(mat.cols()) };
	size_t bytes = rows * cols * sizeof(double);

	// Get platform and device information
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;

	//Bind to platform
	cl_int ret = clGetPlatformIDs(1, &platform_id, NULL);
    check_error(ret, "clGetPlatformIDs");

	//Get ID for the device
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
    check_error(ret, "clGetDeviceIDs");

	// Create an OpenCL context
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    check_error(ret, "clCreateContext");

	// Create a command queue
	cl_command_queue queue = clCreateCommandQueue(context, device_id, 0, &ret);
    check_error(ret, "clCreateCommandQueue");

	// Create a program from the kernel source
	const auto tmp{ load_kernel_file("src/matrix_insert_sort.cl") };
	const char* tmp1{ tmp.data() };
	const char** kernel_source{ &tmp1 };

	cl_program program = clCreateProgramWithSource(context, 1, kernel_source, NULL, &ret);
    check_error(ret, "clCreateProgramWithSource");

	// Build the program
	clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

    char buff[2048];
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buff), buff, nullptr);
    std::cout << buff;

	// Create the OpenCL kernel
	cl_kernel kernel = clCreateKernel(program, "matrix_insert_sort", &ret);
    check_error(ret, "clCreateKernel");

	// Create memory buffers on the device for each vector 
	cl_mem matrix_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);

	// Copy our data set into the input array in device memory
	ret = clEnqueueWriteBuffer(queue, matrix_buffer, CL_TRUE, 0, bytes, mat.data(), 0, NULL, NULL);
    check_error(ret);

	// Set the arguments of the kernel
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &matrix_buffer);
	ret |= clSetKernelArg(kernel, 1, sizeof(decltype(rows)), &rows);
	ret |= clSetKernelArg(kernel, 2, sizeof(decltype(cols)), &cols);
    check_error(ret);

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

	gms::print_matrix_fixed(mat, upper_bound);

	return 0;
}
