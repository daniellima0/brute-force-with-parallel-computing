// MPI

// MPI

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

// 97 to 122 use only lowercase letters
// 65 to 90 use only capital letters
// 48 to 57 use only numbers
#define START_CHAR 48
#define END_CHAR 122
#define MAXIMUM_PASSWORD 20

void bruteForce(char *pass);
long long my_pow(long long x, int y);
double t1, t2;
double dif;

int main(int argc, char **argv) {
    char password[MAXIMUM_PASSWORD];
    strcpy(password, argv[1]);

    MPI_Init(&argc, &argv);

    t1 = MPI_Wtime();
    bruteForce(password);

    return 0;
}

/*Check and increase the digits if you don't find the password...*/
void bruteForce(char *pass) {
    int size;
    int pass_b26[MAXIMUM_PASSWORD];
    long long int j;
    long long int pass_decimal = 0;
    int base = END_CHAR - START_CHAR + 2;

    size = strlen(pass);

    printf("Estamos tentando quebrar: %s\n", pass);

    for (int i = 0; i < size; i++) {
        pass_b26[i] = (int)pass[i] - START_CHAR + 1;  //+1 pois o vazio é o zero e o 'a' é 1
    }

    for (int i = size - 1; i > -1; i--) {
        pass_decimal += (long long int)pass_b26[i] * my_pow(base, i);
    }

    long long int max = my_pow(base, 9);
    char s[10];

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int numberOfProcesses;
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

    long long int amountPerProcess = max / numberOfProcesses;

    // 4 processos
    // 0  1  2  3
    // c  c  c  c

    // 0: amountPerProcess*0 ate amountPerProcess*1
    // 1: amountPerProcess*1 ate amountPerProcess*2
    // 2: amountPerProcess*2 ate amountPerProcess*3
    // 3: amountPerProcess*3 ate amountPerProcess*4

    for (int n = 0; n < numberOfProcesses; n++) {
        if (rank == n) {
            for (j = amountPerProcess * n; j < amountPerProcess * (n + 1); j++) {
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
                    t2 = MPI_Wtime();

                    dif = t2 - t1;

                    MPI_Finalize();

                    printf("\n%1.2f seconds\n", dif);
                    MPI_Abort(MPI_COMM_WORLD, 1);
                }
            }
        }
    }
}

long long my_pow(long long x, int y) {
    long long res = 1;
    if (y == 0)
        return res;
    else
        return x * my_pow(x, y - 1);
}