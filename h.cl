 #pragma OPENCL EXTENSION cl_amd_printf : enable
__kernel void hello() 
{
     	int i = get_global_id(0);
  	printf("\nhi i am inside gpu of thread %d ", i);
   
}
