#pragma once
class Matrix
{
private:
	// All private is used for printing matrix
	int widthOfColumns;
	int widthOfFirstColumn;
	void setWidthOfFirstColumn(int rows);
	void setWidthOfColumns(int columns, int max);

public:
	int rows;					// 1. dimention of matrix
	int columns;				// 2. dimention of matrix
	int** matrix;

	// range of values in matrix is [min..max]; default is [0..9]
	Matrix(int rows, int columns, int min = 0, int max = 9);
	~Matrix();
	void print();
};

