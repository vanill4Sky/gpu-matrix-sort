// gpu-matrix-sort.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>
#include "Matrix.h"
#include "InsertSort.h"

using namespace std;

int main()
{
    srand(time(NULL));

    int rows = 30;
    int columns = 20;

    Matrix matrix = Matrix(rows, columns);
    InsertSort sorter = InsertSort();
    matrix.print();
    cout << endl;

    for (int i = 0; i < rows; i++)
    {
        sorter.insertSort(matrix.matrix[i], matrix.columns);
    }
    matrix.print();

}





