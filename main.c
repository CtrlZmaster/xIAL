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

#include "main.h"
#include "loadfile.h"

int main(int argc, char *argv[]) {
  // Resets locale from C standard to environment locale (particularly ASCII to UTF-8)
  setlocale(LC_ALL, "");

  int err_code = 0;

  // Checking parameters
  if(argc != 2) {
      fprintf(stderr, "ERROR: Wrong parameter count. Rerun with -h for help.\n");
  }

  FILE * file_ptr;
  if(!strcmp(argv[1], "-h")) {
      print_help();
      return 0;
  }
  else {
      // Open the file and check the return code
      if((file_ptr = fopen(argv[1], "r")) == NULL) {
          fprintf(stderr, "ERROR: File with name \"%s\" couldn't be opened.\n", argv[1]);
          return -1;
      }
  }

  // Allocate matrix structure where the adjacency matrix will be stored
  adj_matrix_t *matrix = malloc(sizeof(adj_matrix_t));
  if(matrix == NULL) {
    fprintf(stderr, "ERROR: Allocation error. Check the amount of free memory on your system.\n");
    return -5;
  }

  // Fill the matrix from the file
  err_code = loadToMatrix(file_ptr, matrix);
  if(err_code) {
    return err_code;
  }

  //TODO: Kruskal/Prim
  // Matrix is loaded in variable "matrix" - feel free to rename it :)


  // Freeing the memory
  free_matrix(matrix);
  return err_code;
}


// Prints help to standard output
void print_help(void) {
    puts("NAME\n\t./sptree - list all minimum weight spanning trees of a graph\n");
    puts("SYNOPSIS\n\t./sptree [-h]\n\t         [<filename>]\n");
    puts("DESCRIPTION\n\tThis program prints all minimum weight spanning trees in a graph loaded from a text file. "
         "Program requires only one argument - name of the file. Please, use UTF-8 encoding for your files. "
         "Alternatively, you can use -h switch to display this help.\n");
    puts("AUTHORS\n\tFrantišek Balázsy (xbalaz08@stud.feec.vutbr.cz),\n"
         "\tMichal Pospíšil (xpospi95@stud.fit.vutbr.cz),\n"
         "\tNikola Timková (xtimko01@stud.fit.vutbr.cz)\n");
}