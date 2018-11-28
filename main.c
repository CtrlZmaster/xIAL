/*
 * Project: IAL ...
 * File: main.c
 * Title: 7. Minimum weight spanning tree
 * Description: ...
 * Author: František Balázsy (xbalaz08@stud.feec.vutbr.cz),
 *         Michal Pospíšil (xpospi95@stud.fit.vutbr.cz),
 *         Nikola Timková (xtimko01@stud.fit.vutbr.cz)
 */

#include <stdio.h>

#include "loadfile.h"

int main(int argc, char *argv[]) {
  int err_code = 0;
  // Resets locale from C standard to environment locale (particularly ASCII to UTF-8)
  setlocale(LC_ALL, "");

  // Fill the matrix
  adj_matrix_t *matrix = malloc(sizeof(adj_matrix_t));
  if(matrix == NULL) {
    return -5;
  }
  // Number of vertices will be returned to this variable
  uint8_t vertices;
  err_code = loadToMatrix(argv[1], matrix, &vertices);
  if(err_code) {
    return err_code;
  }

  for(uint8_t i = 0; i < vertices; i++) {
    printf("%ls, ",matrix->vertex_names[i]);
  }
  putchar('\n');
  for(uint8_t i = 0; i < vertices; i++) {
    for(uint8_t j = 0; j < vertices; j++) {
      printf("%" PRIu64 ",",matrix->matrix[i][j]);
    }
    putchar('\n');
  }

  free_matrix(matrix, vertices);
  return err_code;
}