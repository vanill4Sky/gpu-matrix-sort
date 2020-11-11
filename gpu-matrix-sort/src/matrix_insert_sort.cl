__kernel void vecAdd(__global int *data, const unsigned int rows, const unsigned int cols)                   
{                                                                                            
    int id = get_global_id(0);                                
                                                                                  
    if (id < rows)
    {
		int offset = id * cols;
		for (int i = 1; i < cols; i++)
		{
			int key = data[offset + i];
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
