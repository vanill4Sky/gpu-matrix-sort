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

	auto rows{ static_cast<int>(mat.rows()) };
	auto cols{ static_cast<int>(mat.cols()) };

	gms::gpu_sort_driver(mat, "src/matrix_insert_sort.cl", rows, cols);



	return 0;
}
