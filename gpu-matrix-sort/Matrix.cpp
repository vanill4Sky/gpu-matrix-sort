#include "Matrix.h"
#include "Random.h"
#include <iostream>
#include <iomanip>

using namespace std;

Matrix::Matrix(int rows, int columns, int min, int max)
{
	this->rows = rows;
	this->columns = columns;
	setWidthOfFirstColumn(rows);
	setWidthOfColumns(columns, max);
	Random random = Random();

	// declare matrix
	matrix = new int* [rows];
	for (int i = 0; i < rows; i++)
	{
		matrix[i] = new int[columns];
	}
	// fill matrix
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			matrix[i][j] = random.getRand(min, max);
		}
	}
}

Matrix::~Matrix()
{
}

void Matrix::print()
{
	for (int i = 0; i < widthOfFirstColumn; i++)
	{
		cout << " ";
	}
	cout << "|";
	for (int i = 0; i < columns; i++)
	{
		cout << setw(widthOfColumns) << i << "|";
	}
	cout << endl;
	for (int i = 0; i < rows; i++)
	{
		cout << setw(widthOfFirstColumn) << i << "|";
		for (int j = 0; j < columns; j++)
		{
			cout << setw(widthOfColumns) << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

void Matrix::setWidthOfFirstColumn(int rows)
{
	if (rows > 10000)
	{
		widthOfFirstColumn = 5;
		return;
	}
	else if (rows > 1000)
	{
		widthOfFirstColumn = 4;
		return;
	}
	else if (rows > 100)
	{
		widthOfFirstColumn = 3;
		return;
	}
	else if (rows > 10)
	{
		widthOfFirstColumn = 2;
		return;
	}
	else
	{
		widthOfFirstColumn = 1;
	}
}

void Matrix::setWidthOfColumns(int columns, int max)
{
	if (columns > 10000 || max > 9999)
	{
		widthOfColumns = 5;
		return;
	}
	else if ( columns > 1000 || max > 999)
	{
		widthOfColumns = 4;
		return;
	}
	else if (columns > 100 || max > 99)
	{
		widthOfColumns = 3;
		return;
	}
	else if (columns > 10 || max > 9)
	{
		widthOfColumns = 2;
		return;
	}
	else
	{
		widthOfColumns = 1;
	}
}