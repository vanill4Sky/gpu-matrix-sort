#include "matrix.h"
#include "utils.h"
#include "sort.h"
#include "utils.h"
#include "gpu_sort_driver.h"

int main()
{
	gms::matrix<double> mat{ 100, 10};
	gms::fill_matrix_with_random(mat, 0.0, 9.0);

	gms::gpu_sort_driver gpu_sort_driver{ mat, "src/matrix_insert_sort.cl" };
	gpu_sort_driver.sort();
	gms::print_matrix_fixed<double>(gpu_sort_driver.get_matrix(), 9);

	return 0;
}
