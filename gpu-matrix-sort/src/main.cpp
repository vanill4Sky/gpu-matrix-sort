#include <iostream>
#include <iterator>
#include <algorithm>

#include "matrix.h"
#include "utils.h"
#include "sort.h"

int main()
{
    gms::matrix<int> mat{ 10, 10 };
    gms::fill_matrix_with_random(mat, 0, 9);
    gms::sort_matrix(mat, gms::insert_sort<int>, gms::order::descending);
    gms::print_matrix(mat);
}
