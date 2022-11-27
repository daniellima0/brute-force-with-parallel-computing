// CUDA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <cuda.h>
#include <unistd.h>

// 97 to 122 use only lowercase letters
// 65 to 90 use only capital letters
// 48 to 57 use only numbers
#define START_CHAR 48
#define END_CHAR 122
#define MAXIMUM_PASSWORD 20

__global__ void bruteForce(char *pass, unsigned int size);
__host__ __device__ long long my_pow(long long x, int y);
__host__ __device__ unsigned int my_strlen(char *s);

int main(int argc, char **argv) {
    size_t size = sizeof(char) * MAXIMUM_PASSWORD;
    char *password;
    cudaMallocManaged(&password, size);
    strcpy(password, argv[1]);

    size_t threadsPerBlock = 1024;
    size_t numberOfBlocks = 40;

    printf("Estamos tentando quebrar: %s\n", password);

    unsigned int numberOfCharacters = my_strlen(password);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start);
    bruteForce<<<numberOfBlocks, threadsPerBlock>>>(password, numberOfCharacters);
    cudaEventRecord(stop);

    cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);

    printf("\n%1.2f seconds\n", milliseconds / 1000);

    return 0;
}

/*Check and increase the digits if you don't find the password...*/
__global__ void bruteForce(char *pass, unsigned int size) {
    int pass_b26[MAXIMUM_PASSWORD];
    long long int j;
    long long int pass_decimal = 0;
    int base = END_CHAR - START_CHAR + 2;

    for (int i = 0; i < size; i++) {
        pass_b26[i] = (int)pass[i] - START_CHAR + 1;  //+1 pois o vazio é o zero e o 'a' é 1
    }

    for (int i = size - 1; i > -1; i--) {
        pass_decimal += (long long int)pass_b26[i] * my_pow(base, i);
    }

    long long int max = my_pow(base, 9);
    char s[10];
    for (j = blockIdx.x * blockDim.x + threadIdx.x; j < max; j += blockDim.x * gridDim.x) {
        if (j == pass_decimal) {
            printf("Encontrou o password!\n");
            int index = 0;

            printf("O número que estamos tentando encontrar (password na base decimal): %lli\n", j);
            while (j > 0) {
                s[index++] = START_CHAR + j % base - 1;
                j /= base;
            }
            s[index] = '\0';
            printf("Password encontrado: %s\n", s);
            break;
        } else if (j > pass_decimal) {
            break;
        }
    }
}

__host__ __device__ long long my_pow(long long x, int y) {
    long long res = 1;
    if (y == 0)
        return res;
    else
        return x * my_pow(x, y - 1);
}

unsigned int my_strlen(char *palavra) {
    int i = 0;

    while (palavra[i] != '\0') {
        i++;
    }

    return i;
}