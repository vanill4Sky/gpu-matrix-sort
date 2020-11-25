#include "cpu_sort_driver.h"

#include <algorithm>

#include "sort.h"

template<typename T>
inline gms::cpu_sort_driver<T>::cpu_sort_driver(gms::matrix<T>&& matrix, unsigned int thread_count)
	: m_matrix{ std::move(matrix) }
{
	set_thread_count(thread_count);
}

template<typename T>
inline gms::cpu_sort_driver<T>::cpu_sort_driver(const gms::matrix<T>& matrix, unsigned int thread_count)
	: m_matrix{ matrix }
{
	set_thread_count(thread_count);
}

template<typename T>
inline void gms::cpu_sort_driver<T>::run()
{
	if (m_thread_count <= 1)
	{
		sequential_sort();
	}
	else
	{
		parallel_sort();
	}
}

template<typename T>
inline void gms::cpu_sort_driver<T>::set_thread_count(unsigned int thread_count)
{
	const auto min_thread_count = std::max(1u, thread_count);
	m_thread_count = std::min(std::thread::hardware_concurrency(), min_thread_count);
}

template<typename T>
inline unsigned int gms::cpu_sort_driver<T>::get_thread_count() const
{
	return m_thread_count;
}

template<typename T>
inline const gms::matrix<T>& gms::cpu_sort_driver<T>::get_matrix() const
{
	return m_matrix;
}

template<typename T>
inline gms::matrix<T>& gms::cpu_sort_driver<T>::get_matrix()
{
	return m_matrix;
}

template<typename T>
inline void gms::cpu_sort_driver<T>::sequential_sort()
{
	gms::sort_matrix<T>(m_matrix, gms::insert_sort<T>, gms::order::ascending);
}

template<typename T>
inline void gms::cpu_sort_driver<T>::parallel_sort()
{
	std::vector<std::thread> workers;

	for (unsigned int i{ 0 }; i < m_thread_count; ++i)
	{
		workers.emplace_back([&]() {
			sort_routine();
		});
	}

	for (auto& w : workers)
	{
		w.join();
	}
}

template<typename T>
inline void gms::cpu_sort_driver<T>::sort_routine()
{
	while (m_current_row < m_matrix.rows())
	{
		const auto row_idx = m_current_row++;
		if (row_idx < m_matrix.rows())
		{
			auto [begin, end] = m_matrix.row(row_idx);
			gms::insert_sort<T>(begin, end, gms::order::ascending);
		}
	}
	m_current_row = 0;
}
