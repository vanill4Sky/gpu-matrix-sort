#include "gpu_sort_driver.hpp"

#include <algorithm>
#include <fstream>

template<typename T>
inline gms::gpu_sort_driver<T>::gpu_sort_driver(gms::matrix<T> matrix, std::string_view kernel_filename)
	: m_matrix{ std::move(matrix) }
{
}

template<typename T>
inline gms::gpu_sort_driver<T>::~gpu_sort_driver()
{
}

template<typename T>
inline void gms::gpu_sort_driver<T>::sort()
{
}

template<typename T>
inline const gms::matrix<T>& gms::gpu_sort_driver<T>::get_sorted_matrix() const
{
	return m_matrix;
}

template<typename T>
inline std::string gms::gpu_sort_driver<T>::load_kernel_file(std::string_view kernel_filename)
{
	std::ifstream is{ kernel_filename.data() };
	std::string file_content;

	is.seekg(0, std::ios::end);
	file_content.reserve(is.tellg());
	is.seekg(0, std::ios::beg);

	file_content.assign(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>());

	return file_content;
}
