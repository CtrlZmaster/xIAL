//
// Created by michal on 29.11.2018.
//
#include <stdio.h>

#include "loadfile.h"

// Simple test program for loading files
// DO NOT SUBMIT!!!

int main(int argc, char *argv[]) {
    int err_code = 0;
    // Resets locale from C standard to environment locale (particularly ASCII to UTF-8)
    setlocale(LC_ALL, "");

    FILE * file_ptr;
    if((file_ptr = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "ERROR: File with name \"%s\" couldn't be opened.\n", argv[1]);
        return -1;
    }

    // Fill the matrix
    adj_matrix_t *matrix = malloc(sizeof(adj_matrix_t));
    if(matrix == NULL) {
        return -5;
    }

    err_code = loadToMatrix(file_ptr, matrix);
    if(err_code) {
        printf("File loading failed...\n");
        return err_code;
    }

    for(uint8_t i = 0; i < matrix->vertices; i++) {
        printf("%ls, ",matrix->vertex_names[i]);
    }
    putchar('\n');
    for(uint8_t i = 0; i < matrix->vertices; i++) {
        for(uint8_t j = 0; j < matrix->vertices; j++) {
            printf("%" PRIu64 ",",matrix->matrix[i][j]);
        }
        putchar('\n');
    }

    free_matrix(matrix);
    return err_code;
}