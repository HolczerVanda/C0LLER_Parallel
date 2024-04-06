__kernel void vector_add(__global const float* a,
                         __global const float* b,
                         __global float* result,
                         const unsigned int size) {
    int idx = get_global_id(0);
    if (idx < size) {
        result[idx] = a[idx] + b[idx];
    }
}