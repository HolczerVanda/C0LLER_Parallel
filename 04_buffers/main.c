#include "kernel_loader.h"

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>

#include <stdio.h>
#include <stdlib.h>

const int SAMPLE_SIZE = 1000;

int main(void)
{
    int i;
    cl_int err;
    int error_code;

    // Get platform
    cl_uint n_platforms;
	cl_platform_id platform_id;
    err = clGetPlatformIDs(1, &platform_id, &n_platforms);
	if (err != CL_SUCCESS) {
		printf("[ERROR] Error calling clGetPlatformIDs. Error code: %d\n", err);
		return 0;
	}

    // Get device
	cl_device_id device_id;
	cl_uint n_devices;
	err = clGetDeviceIDs(
		platform_id,
		CL_DEVICE_TYPE_GPU,
		1,
		&device_id,
		&n_devices
	);
	if (err != CL_SUCCESS) {
		printf("[ERROR] Error calling clGetDeviceIDs. Error code: %d\n", err);
		return 0;
	}

    // Create OpenCL context
    cl_context context = clCreateContext(NULL, n_devices, &device_id, NULL, NULL, NULL);

    // Build the program
    const char* kernel_code = load_kernel_source("kernels/sample.cl", &error_code);
    if (error_code != 0) {
        printf("Source code loading error!\n");
        return 0;
    }
    cl_program program = clCreateProgramWithSource(context, 1, &kernel_code, NULL, NULL);
    const char options[] = "";
    err = clBuildProgram(
        program,
        1,
        &device_id,
        options,
        NULL,
        NULL
    );
    if (err != CL_SUCCESS) {
        printf("Build error! Code: %d\n", err);
        size_t real_size;
        err = clGetProgramBuildInfo(
            program,
            device_id,
            CL_PROGRAM_BUILD_LOG,
            0,
            NULL,
            &real_size
        );
        char* build_log = (char*)malloc(sizeof(char) * (real_size + 1));
        err = clGetProgramBuildInfo(
            program,
            device_id,
            CL_PROGRAM_BUILD_LOG,
            real_size + 1,
            build_log,
            &real_size
        );
        // build_log[real_size] = 0;
        printf("Real size : %d\n", real_size);
        printf("Build log : %s\n", build_log);
        free(build_log);
        return 0;
    }
    cl_kernel kernel = clCreateKernel(program, "sample_kernel", NULL);

    // Create the host buffer and initialize it
    int* host_buffer = (int*)malloc(SAMPLE_SIZE * sizeof(int));
    for (i = 0; i < SAMPLE_SIZE; ++i) {
        host_buffer[i] = i;
    }

    // Create the device buffer
    cl_mem device_buffer = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
        SAMPLE_SIZE * sizeof(int),
        host_buffer,
        &err
    );
    if (err != CL_SUCCESS) {
        printf("Unable to create buffer! Code: %d\n", err);
        return 0;
    }

    cl_buffer_region region;
    region.origin = 256 * sizeof(int);
    region.size = 256 * sizeof(int);

    

    cl_mem sub_buffer = clCreateSubBuffer(
        device_buffer,
        CL_MEM_READ_WRITE,
        CL_BUFFER_CREATE_TYPE_REGION,
        &region,
        &err
    );
    if (err != CL_SUCCESS) {
        printf("Unable to create sub buffer! Code: %d\n", err);
        return 0;
    }

    void* device_buffer_ptr;
    clGetMemObjectInfo(
        device_buffer,
        CL_MEM_HOST_PTR,
        sizeof(device_buffer_ptr),
        &device_buffer_ptr,
        NULL
    );
    printf("Device buffer address: %p\n", device_buffer_ptr);

    size_t device_buffer_size;
    clGetMemObjectInfo(
        device_buffer,
        CL_MEM_SIZE,
        sizeof(device_buffer_size),
        &device_buffer_size,
        NULL
    );
    printf("Device buffer size: %d\n", device_buffer_size);

    void* sub_buffer_ptr;
    clGetMemObjectInfo(
        sub_buffer,
        CL_MEM_HOST_PTR,
        sizeof(sub_buffer_ptr),
        &sub_buffer_ptr,
        NULL
    );
    printf("Sub buffer address: %p\n", sub_buffer_ptr);

    size_t sub_buffer_size;
    clGetMemObjectInfo(
        sub_buffer,
        CL_MEM_SIZE,
        sizeof(sub_buffer_size),
        &sub_buffer_size,
        NULL
    );
    printf("Sub buffer size: %d\n", sub_buffer_size);

    // Set kernel arguments
    clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&device_buffer);
    clSetKernelArg(kernel, 1, sizeof(int), (void*)&SAMPLE_SIZE);

    // Create the command queue
    cl_command_queue command_queue = clCreateCommandQueue(
        context, device_id, CL_QUEUE_PROFILING_ENABLE, NULL);

    // Host buffer -> Device buffer
    clEnqueueWriteBuffer(
        command_queue,
        device_buffer,
        CL_FALSE,
        0,
        SAMPLE_SIZE * sizeof(int),
        host_buffer,
        0,
        NULL,
        NULL
    );

    // Size specification
    size_t local_work_size = 256;
    size_t n_work_groups = (SAMPLE_SIZE + local_work_size + 1) / local_work_size;
    size_t global_work_size = n_work_groups * local_work_size;

    // Apply the kernel on the range
    cl_event event;
    clEnqueueNDRangeKernel(
        command_queue,
        kernel,
        1,
        NULL,
        &global_work_size,
        &local_work_size,
        0,
        NULL,
        &event
    );
    clFinish(command_queue);

    // Host buffer <- Device buffer
    clEnqueueReadBuffer(
        command_queue,
        device_buffer,
        CL_TRUE,
        0,
        SAMPLE_SIZE * sizeof(int),
        host_buffer,
        0,
        NULL,
        NULL
    );

    for (i = 0; i < SAMPLE_SIZE; ++i) {
        // printf("[%d] = %d, ", i, host_buffer[i]);
    }

    // Release the resources
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseMemObject(sub_buffer);
    clReleaseMemObject(device_buffer);
    clReleaseContext(context);
    clReleaseDevice(device_id);

    free(host_buffer);
}
