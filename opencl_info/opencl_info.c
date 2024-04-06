#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

void printDeviceInfo(cl_device_id device) {
    char deviceName[1024];
    clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(deviceName), &deviceName, NULL);
    printf("Device Name: %s\n", deviceName);

    cl_device_type deviceType;
    clGetDeviceInfo(device, CL_DEVICE_TYPE, sizeof(deviceType), &deviceType, NULL);
    printf("Device Type: ");
    if (deviceType & CL_DEVICE_TYPE_CPU)
        printf("CPU\n");
    if (deviceType & CL_DEVICE_TYPE_GPU)
        printf("GPU\n");
    if (deviceType & CL_DEVICE_TYPE_ACCELERATOR)
        printf("Accelerator\n");
    if (deviceType & CL_DEVICE_TYPE_DEFAULT)
        printf("Default\n");

    cl_uint maxComputeUnits;
    clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(maxComputeUnits), &maxComputeUnits, NULL);
    printf("Device Max Compute Units: %u\n", maxComputeUnits);

    cl_ulong globalMemSize;
    clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(globalMemSize), &globalMemSize, NULL);
    printf("Device Global Memory Size: %llu MB\n", (unsigned long long)(globalMemSize / (1024*1024)));

    cl_ulong localMemSize;
    clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(localMemSize), &localMemSize, NULL);
    printf("Device Local Memory Size: %llu KB\n", (unsigned long long)(localMemSize / 1024));

    cl_uint maxClockFrequency;
    clGetDeviceInfo(device, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(maxClockFrequency), &maxClockFrequency, NULL);
    printf("Device Max Clock Frequency: %u MHz\n", maxClockFrequency);

    printf("\n");
}

int main() {
    cl_int err;
    cl_uint numPlatforms;
    cl_platform_id *platforms;
    cl_uint i, j;

    // Get the number of available platforms
    err = clGetPlatformIDs(0, NULL, &numPlatforms);
    if (err != CL_SUCCESS) {
        printf("Error getting platform IDs\n");
        return EXIT_FAILURE;
    }

    printf("Number of Platforms: %u\n\n", numPlatforms);

    // Allocate memory for platform IDs
    platforms = (cl_platform_id *)malloc(numPlatforms * sizeof(cl_platform_id));
    if (platforms == NULL) {
        printf("Error allocating memory for platform IDs\n");
        return EXIT_FAILURE;
    }

    // Get platform IDs
    err = clGetPlatformIDs(numPlatforms, platforms, NULL);
    if (err != CL_SUCCESS) {
        printf("Error getting platform IDs\n");
        free(platforms);
        return EXIT_FAILURE;
    }

    // Iterate through each platform
    for (i = 0; i < numPlatforms; i++) {
        cl_uint numDevices;
        cl_device_id *devices;

        // Get the number of devices for the current platform
        err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
        if (err != CL_SUCCESS) {
            printf("Error getting device IDs for platform %u\n", i);
            continue;
        }

        printf("Platform %u\n", i);

        // Allocate memory for device IDs
        devices = (cl_device_id *)malloc(numDevices * sizeof(cl_device_id));
        if (devices == NULL) {
            printf("Error allocating memory for device IDs\n");
            continue;
        }

        // Get device IDs
        err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);
        if (err != CL_SUCCESS) {
            printf("Error getting device IDs for platform %u\n", i);
            free(devices);
            continue;
        }

        printf("Number of Devices: %u\n", numDevices);

        // Iterate through each device
        for (j = 0; j < numDevices; j++) {
            printf("Device %u\n", j);
            printDeviceInfo(devices[j]);
        }

        free(devices);
        printf("\n");
    }

    free(platforms);

    return EXIT_SUCCESS;
}
