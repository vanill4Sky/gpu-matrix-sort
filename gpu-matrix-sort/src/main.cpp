#include <iostream>
#include <algorithm>
#include <iterator>
#include <CL/cl.hpp>

#include "matrix.h"
#include "utils.h"
#include "insert_sort.h"

int main()
{
	size_t rows = 2;
	size_t cols = 4; //n

	size_t bytes = cols * sizeof(double);

	//Host input vectors with allocated memory (depends on num of rows)
	double *h_a = (double*)malloc(bytes);
	double *h_b = (double*)malloc(bytes);

	//Host output vectors with allocated memory
	double *h_c = (double*)malloc(bytes);
	double *h_d = (double*)malloc(bytes);

	// Get platform and device information
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;

	//Bind to platform
	cl_int ret = clGetPlatformIDs(1, &platform_id, NULL);

	//Get ID for the device
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);

	// Create an OpenCL context
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	// Create a command queue
	cl_command_queue queue = clCreateCommandQueue(context, device_id, 0, &ret);

	// Create a program from the kernel source
	cl_program program = clCreateProgramWithSource(context, 1, (const char **) & kernelSource, NULL, &ret);

	// Build the program
	clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

	// Create the OpenCL kernel
	cl_kernel kernel = clCreateKernel(program, "matrix_sort", &ret);


	// Create memory buffers on the device for each vector 
	cl_mem d_a = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);
	cl_mem d_b = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);
	cl_mem d_c = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);
	cl_mem d_d = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);


	// Copy our data set into the input array in device memory
	ret = clEnqueueWriteBuffer(queue, d_a, CL_TRUE, 0, bytes, h_a, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(queue, d_b, CL_TRUE, 0, bytes, h_b, 0, NULL, NULL);


	// Set the arguments of the kernel
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_a);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_b);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_c);
	ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), &d_d);

	//Number of work items in each local work group
	size_t local_item_size = 64; 

	//Number of total work items
	size_t global_item_size = ceil(cols / (float)local_item_size)*local_item_size;

	//Execute the kernel over the entire range of the data set
	ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

	//Wait for the queue to finish
	clFinish(queue);

	//Read the results
	clEnqueueReadBuffer(queue, d_c, CL_TRUE, 0, bytes, h_c, 0, NULL, NULL);
	clEnqueueReadBuffer(queue, d_d, CL_TRUE, 0, bytes, h_d, 0, NULL, NULL);


    gms::matrix<float> mat{ rows, cols };
    gms::fill_matrix_with_random(mat, -10.0f, 10.0f);

    std::vector<int> v{ 3, 1, 5, 2, 9, -10 };
    const auto compare = [](int a, int b) {
        return a > b;
    };
    gms::insert_sort<int>(v.begin(), v.end(), compare);
    std::copy(v.begin(), v.end(), std::ostream_iterator<decltype(v)::value_type>{ std::cout, ", " });



    //srand(time(NULL));

    //int rows = 30;
    //int columns = 20;

    //Matrix matrix = Matrix(rows, columns);
    //InsertSort sorter = InsertSort();
    //matrix.print();
    //cout << endl;

    //for (int i = 0; i < rows; i++)
    //{
    //    sorter.insertSort(matrix.matrix[i], matrix.columns);
    //}
    //matrix.print();


	//Clean up
	clFlush(queue);
	clFinish(queue);

	clReleaseMemObject(d_a);
	clReleaseMemObject(d_b);
	clReleaseMemObject(d_c);
	clReleaseMemObject(d_d);

	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	free(h_a);
	free(h_b);
	free(h_c);
	free(h_d);

	return 0;

}
