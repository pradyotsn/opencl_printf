#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
 
#define MAX_SOURCE_SIZE (0x100000)
 
int main(void) {
      	
    // Load the kernel source code into the array source_str
    	FILE *fp;
    	char *source_str;
    	size_t source_size;
 
    	fp = fopen("h.cl", "r");
    	if (!fp) {
        	fprintf(stderr, "Failed to load kernel.\n");
        	exit(1);
    	}
    	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_str[MAX_SOURCE_SIZE+1]='\0';
    	source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
    	fclose( fp );

	cl_int ret;
	int j;


    	cl_platform_id *platform_id;
	char* info;
    	size_t infoSize;
    	cl_uint ret_num_platforms;

//To print the selected platform



 	ret = clGetPlatformIDs(5, NULL, &ret_num_platforms);

	printf("\n the total no. of platforms detected is :%d\n",ret_num_platforms);


	platform_id = (cl_platform_id *)alloca(sizeof(cl_platform_id) * ret_num_platforms);

 	ret = clGetPlatformIDs(ret_num_platforms, platform_id, NULL);

 	// get platform attribute value size
       	clGetPlatformInfo(platform_id[0], CL_PLATFORM_NAME, 0, NULL, &infoSize);
       	info = (char*) malloc(infoSize);
 	// get platform attribute value
    	clGetPlatformInfo(platform_id[0], CL_PLATFORM_NAME, infoSize, info, NULL);
   	printf("\n  Name of the platform selected: %s ", info);

    	cl_device_id device_id=NULL;   
    	cl_uint ret_num_devices=1;
    	ret = clGetDeviceIDs( platform_id[0], CL_DEVICE_TYPE_GPU, 1,&device_id, NULL);



		char* value;
    		size_t valueSize;
		clGetDeviceInfo(device_id, CL_DEVICE_NAME, 0, NULL, &valueSize);
            	value = (char*) malloc(valueSize);
            	clGetDeviceInfo(device_id, CL_DEVICE_NAME, valueSize, value, NULL);
            	printf("\nDevice selected name is: %s \n",value);
            	free(value);



	cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);


	if (ret != CL_SUCCESS)
	{
		printf( "\n\n Could not create GPU context");
	
	}
	else
	{
		//printf("\n\nno error in creatng context");
	}
    

    	cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	if (command_queue == NULL)
	{
		printf("\n\nFailed to create commandQueue for device 0");
		exit(0);
	}
	else
	{
		//printf("\n\n no error in creating commandQueue");
	}


    	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

	if (program == NULL)
	{
		printf("Failed to create CL program from source.");
	
	}
	else
	{
		//printf("\n\n no error in creating CL program from source");
	}



	
	ret = clBuildProgram(program, 1, &device_id, 0, 0, 0);
	//printf("\n\nbuild pgm returns code:%d\n\n",ret);

	if (ret != CL_SUCCESS)
	{
		// Determine the reason for the error
	
	

char buildLog[32000];
clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG,sizeof(buildLog), buildLog, NULL);
printf("\nError in kernel BUILD PGM: %s\n ",buildLog);

	}
	
	
    	cl_kernel kernel = clCreateKernel(program, "hello", &ret);
 

 if (ret != CL_SUCCESS)
    {
        printf("Error at: clCreateKernel(get kernel function failed):\n");
        
    }

    	size_t global_item_size = 50;


  	
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, NULL, 0, NULL, NULL);
 
   	if(ret== CL_SUCCESS) {
        	//printf("\n\nNO Error in clEnqueueNDRangeKernel\n");
    	} 

	else {
        	printf("\n Error clEnqueueNDRangeKernel\n");
    	}


    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
   
    return 0;
}



/*
gcc h.c -o h -lOpenCL

*/
