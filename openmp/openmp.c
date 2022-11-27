// OpenMP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <omp.h>

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
    t1 = omp_get_wtime();
    bruteForce(password);

    return 0;
}

/*Check and increase the digits if you don't find the password...*/
void bruteForce(char *pass) {
    int size;
    char force[MAXIMUM_PASSWORD];
    int palavra[9];
    int pass_b26[9];
    long long int j;
    long long int pass_decimal = 0;
    int base = END_CHAR - START_CHAR + 2;

    size = strlen(pass);

    for (int i = 0; i < MAXIMUM_PASSWORD; i++)
        force[i] = '\0';

    printf("Estamos tentando quebrar: %s\n", pass);

    for (int i = 0; i < strlen(pass); i++) {
        pass_b26[i] = (int)pass[i] - START_CHAR + 1;  //+1 pois o vazio é o zero e o 'a' é 1
    }

    for (int i = strlen(pass) - 1; i > -1; i--) {
        pass_decimal += (long long int)pass_b26[i] * my_pow(base, i);
    }

    long long int max = my_pow(base, 9);
    char s[10];

    int flag = 0;
#pragma omp parallel for private(j) shared(flag)
    for (j = 0; j < max; j++) {
        if (flag == 1) {
            exit(1);
        }
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
            t2 = omp_get_wtime();
            dif = t2 - t1;
            printf("\n%1.2lf seconds\n", dif);
            flag = 1;
            exit(1);
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