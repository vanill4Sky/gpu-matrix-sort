#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#define MAX_COLUMN_COUNT 256

__kernel void matrix_insert_sort(__global float* data, const int rows, const int cols)
{
	__local float local_buffer[WORK_GROUP_SIZE * MAX_COLUMN_COUNT];

    const int global_row_idx = get_global_id(0);
	const int offset = get_local_id(0) * cols;
	const int work_group_offset = get_group_id(0) * WORK_GROUP_SIZE * cols;
	
	if (global_row_idx < rows)
    {
		event_t e = async_work_group_copy(local_buffer, data + work_group_offset, WORK_GROUP_SIZE * cols, 0);
		wait_group_events(1, &e);

		for (int i = 1; i < cols; ++i)
		{
			float key = local_buffer[offset + i];
			int j = i - 1;
			while (j >= 0 && local_buffer[offset + j] > key)
			{
				local_buffer[offset + j + 1] = local_buffer[offset + j];
				j = j - 1;
			}
			
			local_buffer[offset + j + 1] = key;
		}

		e = async_work_group_copy(data + work_group_offset, local_buffer, WORK_GROUP_SIZE * cols, 0);
		wait_group_events(1, &e);
    }                                
}
