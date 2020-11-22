#include <iostream>

#include "matrix.h"
#include "utils.h"
#include "sort.h"
#include "utils.h"
#include "gpu_sort_driver.h"
#include "cpu_sort_driver.h"
#include "timer.h"

int main()
{
	gms::timer timer;
	timer.start();

	gms::matrix<double> mat{ 100, 10};
	gms::fill_matrix_with_random(mat, 0.0, 9.0);

	// choose one method of sorting cpu/gpu
	gms::gpu_sort_driver gpu_sort_driver{ mat, "src/matrix_insert_sort.cl" };
	gpu_sort_driver.sort();

	//gms::cpu_sort_driver cpu_sort_driver(mat, 6);
	//cpu_sort_driver.run();

	gms::print_matrix_fixed<double>(gpu_sort_driver.get_matrix(), 9);

	timer.stop();
	std::cout << timer.get_time_as_string() << "\n";

	return 0;
}
