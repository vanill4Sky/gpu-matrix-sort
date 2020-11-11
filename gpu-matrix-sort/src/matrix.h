#pragma once

#include <vector>
#include <utility>

namespace gms
{

template <typename T>
class matrix
{
public:
	matrix(size_t rows, size_t cols);

	T* data();
	typename std::vector<T>::iterator begin();
	typename std::vector<T>::iterator end();
	T& operator()(size_t row, size_t col);
	const T& operator()(size_t row, size_t col) const;
	std::pair<typename std::vector<T>::iterator, typename std::vector<T>::iterator> row(size_t row);
	size_t rows() const;
	size_t cols() const;

private:
	size_t m_rows;
	size_t m_cols;
	std::vector<T> m_data;
};

}

#include "matrix.inl"