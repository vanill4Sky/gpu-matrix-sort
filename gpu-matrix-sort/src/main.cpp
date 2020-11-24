#include <iostream>

#define FMT_HEADER_ONLY 1

#include <cxxopts.hpp>
#include <fmt/format.h>
#include <stdexcept>
#include <string_view>

#include "matrix.h"
#include "utils.h"
#include "sort.h"
#include "utils.h"
#include "gpu_sort_driver.h"
#include "cpu_sort_driver.h"
#include "timer.h"

bool test_cpu(size_t rows, size_t cols, unsigned int thread_count, size_t iterations, std::string_view output_path)
{
	fmt::print("Sorting on CPU using: {} threads\n", thread_count);
	fmt::print("Matrix size (rows, cols): ({}, {})\n", rows, cols);
	fmt::print("The output data will be saved in: {}\n", output_path);

	gms::csv_writer csv("iteracja", "wiersze", "kolumny", "liczba rdzeni", "czas sortowania [ns]");

	gms::timer timer;
	gms::matrix<float> m{ rows, cols };
	gms::cpu_sort_driver cpu_sort_driver{ std::move(m), thread_count };
	for (size_t i = 0; i < iterations; ++i)
	{
		fmt::print("Iteration {} of {}.\n", i + 1, iterations);

		gms::fill_matrix_with_random(cpu_sort_driver.get_matrix(), 0.0f, 10.0f);
		timer.start();
		cpu_sort_driver.run();
		timer.stop();
		const auto sort_time = timer.get_time();
		csv.add_row(i, rows, cols, cpu_sort_driver.get_thread_count(), sort_time);
	}

	return csv.save_to_file(output_path);
}

bool test_gpu(size_t rows, size_t cols, std::string_view kernel_name,
	unsigned int wg_size, size_t iterations, std::string_view output_path)
{
	fmt::print("Sorting on GPU, work group size: {}\n", wg_size);
	fmt::print("Matrix size (rows, cols): ({}, {})\n", rows, cols);
	fmt::print("The output data will be saved in: {}\n", output_path);

	gms::csv_writer csv("iteracja", "wiersze", "kolumny", "rozmiar grupy roboczej", 
		"czas zapisu bufora[ns]", "czas sortowania[ns]", "czas odczytu bufora[ns]");

	gms::matrix<float> m{ rows, cols };
	gms::gpu_sort_driver gpu_sort_driver{ std::move(m), kernel_name, wg_size };
	for (size_t i = 0; i < iterations; ++i)
	{
		fmt::print("Iteration {} of {}.\n", i + 1, iterations);

		gms::fill_matrix_with_random(gpu_sort_driver.get_matrix(), 0.0f, 10.0f);
		gpu_sort_driver.sort();
		//csv.add_row(i, rows, cols, cpu_sort_driver.get_thread_count(), sort_time);
	}

	return csv.save_to_file(output_path);
}

int main(int argc, char* argv[])
{
	cxxopts::Options options("gpu_matrix_sort", "Test program that sorts in parallel the elements in rows of a 2d matrix.");
	options.add_options()
		("b,both", "Test both GPU and CPU")
		("c,cpu", "Test CPU only")
		("g,gpu", "Test GPU only")
		("p,profiling", "Test best size of work group <1, 512>")
		("l,local", "Use GPU local memory", cxxopts::value<bool>()->default_value("false"))
		("w,workgroupsize", "Size of the work grupy (power of 2)", cxxopts::value<size_t>()->default_value("64"))
		("t,threads", "Number of CPU threads used", cxxopts::value<unsigned int>()->default_value("1"))
		("r,rows", "Number of rows in the 2d matrix.", cxxopts::value<size_t>()->default_value("1024"))
		("d,cols", "Number of columns in the 2d matrix.", cxxopts::value<size_t>()->default_value("256"))
		("i,iterations", "Number of iterations.", cxxopts::value<size_t>()->default_value("1"))
		("o,outputpath", "Output data file path.", cxxopts::value<std::string>()->default_value("test.csv"))
		("h,help", "Print usage");

	auto result = options.parse(argc, argv);

	bool local;
	size_t work_group_size, rows, cols, iterations;
	unsigned int threads; 
	std::string output_path;

	try
	{
		local = result["local"].as<bool>();
		work_group_size = result["workgroupsize"].as<size_t>();
		threads = result["threads"].as<unsigned int>();
		rows = result["rows"].as<size_t>();
		cols = result["cols"].as<size_t>();
		iterations = result["iterations"].as<size_t>();
		output_path = result["outputpath"].as<std::string>();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "\n";
		return -1;
	}

	if (result.count("help"))
	{
		fmt::print("{}\n", options.help());
		return 0;
	}

	gms::matrix<float> mat{ rows, cols };
	gms::fill_matrix_with_random(mat, 0.0f, 9.0f);

	if (result.count("both"))
	{

	}
	else if (result.count("cpu"))
	{
		test_cpu(rows, cols, threads, iterations, output_path);

		//const auto is_sorted_correctly = gms::validate_sorting(cpu_sort_driver.get_matrix(), gms::order::ascending);
		//fmt::print("Matrix sorted correctly: {}\n", is_sorted_correctly);
	}
	else if (result.count("gpu"))
	{

	}
	else if (result.count("profiling"))
	{

	}

	//gms::timer timer;
	//timer.start();

	//gms::matrix<double> mat{ 100, 10};
	//gms::fill_matrix_with_random(mat, 0.0, 9.0);

	//// choose one method of sorting cpu/gpu
	//gms::gpu_sort_driver gpu_sort_driver{ mat, "src/matrix_insert_sort.cl" };
	//gpu_sort_driver.sort();

	////gms::cpu_sort_driver cpu_sort_driver(mat, 6);
	////cpu_sort_driver.run();

	//gms::print_matrix_fixed<double>(gpu_sort_driver.get_matrix(), 9);

	//timer.stop();
	//std::cout << timer.get_time_as_string() << "\n";

	return 0;
}
