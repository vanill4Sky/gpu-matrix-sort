#pragma once

#include "matrix.h"

#include <thread>
#include <atomic>

namespace gms
{

template <typename T>
class cpu_sort_driver
{
public:
	cpu_sort_driver(gms::matrix<T>&& matrix, unsigned int thread_count);
	cpu_sort_driver(const gms::matrix<T>& matrix, unsigned int thread_count);

	void run();
	void set_thread_count(unsigned int thread_count);
	unsigned int get_thread_count() const;
	const gms::matrix<T>& get_matrix() const;
	gms::matrix<T>& get_matrix();

private:
	void sequential_sort();
	void parallel_sort();
	void sort_routine();

	gms::matrix<T> m_matrix;
	unsigned int m_thread_count;
	std::atomic<size_t> m_current_row;
};

}

#include "cpu_sort_driver.inl"