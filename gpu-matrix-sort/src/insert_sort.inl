#include <cassert>

template<typename T>
inline void gms::insert_sort(
	const typename std::vector<T>::iterator first,
	const typename std::vector<T>::iterator last,
	const std::function<bool(T, T)>& compare)
{
	if (first == last)
	{
		return;
	}
	
	for (auto it{ first + 1 }; it != last; ++it)
	{
		const auto key{ *it };

		auto prev{ it };
		while ((prev != first) && compare(*(prev - 1), key))
		{
			*prev = *(prev - 1);
			--prev;
		}
		*prev = key;
	}
}