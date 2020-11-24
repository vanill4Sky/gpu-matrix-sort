#pragma OPENCL EXTENSION cl_khr_fp64 : enable

__kernel void matrix_insert_sort(__global float* data, const int rows, const int cols)
{                                                                                            
    int id = get_global_id(0);                                
                                                                                  
    if (id < rows)
    {
		int offset = id * cols;
		for (int i = 1; i < cols; i++)
		{
			float key = data[offset + i];
			int j = i - 1;

			while (j >= 0 && data[offset + j] > key)
			{
				data[offset + j + 1] = data[offset + j];
				j = j - 1;
			}
			data[offset + j + 1] = key;
		}
    }                                
}
