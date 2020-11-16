#include <cassert>
#include "matrix.h"

template<typename T>
inline gms::matrix<T>::matrix(size_t rows, size_t cols)
	: m_rows{ rows }
	, m_cols{ cols }
	, m_data(rows * cols)
{
}

template<typename T>
inline T* gms::matrix<T>::data()
{
	return m_data.data();
}

template<typename T>
inline typename std::vector<typename T>::iterator gms::matrix<T>::begin()
{
	return m_data.begin();
}

template<typename T>
inline typename std::vector<T>::iterator gms::matrix<T>::end()
{
	return m_data.end();
}

template<typename T>
inline T& gms::matrix<T>::operator()(size_t row, size_t col)
{
	assert(row < m_rows);
	assert(col < m_cols);

	return m_data[row * m_cols + col];
}

template<typename T>
inline const T& gms::matrix<T>::operator()(size_t row, size_t col) const
{
	assert(row < m_rows);
	assert(col < m_cols);

	return m_data[row * m_cols + col];
}

template<typename T>
inline std::pair<typename std::vector<T>::iterator, typename std::vector<T>::iterator> gms::matrix<T>::row(size_t row)
{
	assert(row < m_rows);

	const auto begin_offset{ row * m_cols };
	const auto end_offset{ begin_offset + m_cols };

	return { m_data.begin() + begin_offset, m_data.begin() + end_offset };
}

template<typename T>
inline size_t gms::matrix<T>::rows() const
{
	return m_rows;
}

template<typename T>
inline size_t gms::matrix<T>::cols() const
{
	return m_cols;
}
