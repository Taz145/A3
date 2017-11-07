#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

bool isNum (char *string);
void singleRun(int num_threads, int matrix_size);
void multiRun();

int main(int argc, char *argv[]) {
    bool test = false;
    int num_threads, matrix_size;

    if (argc == 1) {
        printf("Usage: ./a3 <number of threads> <matrix size>\n ./a3 -g -This mode runs the program through multiple tests\n");
        return 1;
    } else if (argc == 2) {
        if (strcmp(argv[1], "-g") == 0) {
            test = true;
        } else {
            printf("Usage: ./a3 <number of threads> <matrix size>\n ./a3 -g -This mode runs the program through multiple tests\n");
            return 1;
        }
    } else if (isNum(argv[1]) && isNum(argv[2])) {
        num_threads = strtol(argv[1], NULL, 10);
        matrix_size = strtol(argv[2], NULL, 10);
    } else {
        printf("Invalid values entered. Exiting.\n");
        return 1;
    }

    srand(time(NULL));

    if (test) {
        multiRun();
    } else {
        singleRun(num_threads, matrix_size);
    }

    return 0;
}

void multiRun () {
    printf("Size\t\tThreads\n");
    printf("\t\t1\t2\t4");
    printf("\n100");

    singleRun(1,100);
    singleRun(2,100);
    singleRun(4,100);

    printf("\n1000");

    singleRun(1,1000);
    singleRun(2,1000);
    singleRun(4,1000);
    printf("\n10000");

    singleRun(1,10000);
    singleRun(2,10000);
    singleRun(4,10000);

    printf("\n20000");

    singleRun(1,20000);
    singleRun(2,20000);
    singleRun(4,20000);
    printf("/n");
}

void singleRun (int num_threads, int matrix_size) {


    float *matrix_A, **matrix_B; //A is a 1xSIZE so 1D while B is SIZExSIZE so needs to be 2D
    float *matrix_AB; //result matrix

    matrix_A = calloc(1,sizeof(float) * matrix_size);

    matrix_B = calloc(1,sizeof(float *) * matrix_size);

    matrix_AB = calloc(1,sizeof(float) * matrix_size);
    //add pragma for openMP here. should be the only llel needed for the program

    //create the columns for matrix_B and populate both matrices
    for (int i = 0; i < matrix_size; i++) {
        matrix_B[i] = calloc(1,sizeof(float) * matrix_size);
        float r = ((float) rand())/RAND_MAX;
        matrix_A[i] = r;
        for (int j = 0; j < matrix_size; j++) {
            r = ((float) rand())/RAND_MAX;
            matrix_B[i][j] = r;
        }
    }
    #pragma omp parallel for collapse(2) num_threads(num_threads)
    for (int i = 0; i < matrix_size; i++) { //traverse rows
        for (int j = 0; j < matrix_size; j++) { //traverse columns
            matrix_AB[i] += matrix_A[i] * matrix_B[i][j];
        }
    }




    for (int i = 0; i < matrix_size; i++) {
        free(matrix_B[i]);
    }
    free(matrix_A);
    free(matrix_B);
    free(matrix_AB);

}


bool isNum (char *string) {
    for (int i = 0; i <strlen(string); i++) {
        if (!isdigit(string[i])) return false;
    }
    return true;
}