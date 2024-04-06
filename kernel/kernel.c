#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readKernelSourceFile(const char* filename) {
    // Összefűzzük a fájlnevet a 'kernels/' előtaggal
    char path[100] = "kernels/";
    strcat(path, filename);

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Unable to open file: %s\n", path);
        return NULL;
    }

    // Determine the size of the file
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    // Allocate memory for the source code string
    char* sourceCode = (char*)malloc(fileSize + 1);
    if (sourceCode == NULL) {
        fprintf(stderr, "Unable to allocate memory\n");
        fclose(file);
        return NULL;
    }

    // Read the source code from the file
    size_t bytesRead = fread(sourceCode, 1, fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Error reading file: %s\n", path);
        fclose(file);
        free(sourceCode);
        return NULL;
    }

    // Add null terminator
    sourceCode[bytesRead] = '\0';

    fclose(file);
    return sourceCode;
}

int main() {
    const char* filename = "hello_kernel.cl"; // Replace "hello_kernel.cl" with your filename

    // Read kernel source code from file
    char* kernelSource = readKernelSourceFile(filename);
    if (kernelSource == NULL) {
        fprintf(stderr, "Failed to read kernel source file\n");
        return EXIT_FAILURE;
    }

    // Print the kernel source code
    printf("Kernel Source Code:\n");
    printf("%s\n", kernelSource);

    // Remember to free the allocated memory
    free(kernelSource);

    return EXIT_SUCCESS;
}
