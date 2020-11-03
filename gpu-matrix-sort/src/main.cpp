// gpu-matrix-sort.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>
#include "matrix.h"
#include "utils.h"
#include "insert_sort.h"

using namespace std;

int main()
{
    gms::matrix<float> mat{ 2, 4 };
    gms::fill_matrix_with_random(mat, -10.0f, 10.0f);
    auto test{ mat(1, 3) };

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
