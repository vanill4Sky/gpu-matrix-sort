#include <iostream>


#include "matrix.h"
#include "utils.h"
#include "sort.h"
#include "utils.h"
#include "gpu_sort_driver.h"

int main()
{
	gms::matrix<double> mat{ 10000, 1000 };
	gms::fill_matrix_with_random(mat, 0.0, 9.0);

	gms::gpu_sort_driver gpu_sort_driver{ mat, "src/matrix_insert_sort.cl" };
	gpu_sort_driver.sort();
	//gms::print_matrix<double>(gpu_sort_driver.get_matrix());

	const auto timing_info = gpu_sort_driver.get_timing_info();
	std::cout << "wt[ns]: " << timing_info.write_time << '\n';
	std::cout << "et[ns]: " << timing_info.execution_time << '\n';
	std::cout << "rt[ns]: " << timing_info.read_time << '\n';

	return 0;
}
