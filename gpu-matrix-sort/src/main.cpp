#include <iostream>
#include <iterator>
#include <algorithm>

#include "matrix.h"
#include "utils.h"
#include "insert_sort.h"

int main()
{
    gms::matrix<float> mat{ 2, 4 };
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
}
