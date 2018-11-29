/*
 * Project: IAL ...
 * File: loadfile.h
 * Title: 7. Minimum weight spanning tree
 * Description: ...
 * Author: František Balázsy (xbalaz08@stud.feec.vutbr.cz),
 *         Michal Pospíšil (xpospi95@stud.fit.vutbr.cz),
 *         Nikola Timková (xtimko01@stud.fit.vutbr.cz)
 */


#ifndef XIAL_LOADFILE_H
#define XIAL_LOADFILE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>

#define FILETYPE_STR L".xIALAdjMatrix"
#define FILETYPE_STRLEN wcslen(FILETYPE_STR)

#define TOKEN_ALLOC_UNIT 10

typedef struct token {
  wchar_t* string;
  size_t length;
} token_t;

typedef struct adj_matrix {
  int64_t **matrix;
  wchar_t **vertex_names;
  uint8_t vertices;
} adj_matrix_t;

int loadToMatrix(FILE *file_ptr, adj_matrix_t *adj_matrix);
int lexFSM(FILE * file_ptr, uint8_t lex_num, token_t* temp_token, wchar_t *vertex_names[], int64_t edge_weights[]);
int flush_temp_token(token_t *temp_token);
int realloc_temp_token(token_t *temp_token, size_t size);
int token_to_retval(token_t *temp_token, size_t index, char mode, wchar_t *vertex_names[], int64_t edge_weights[]);
void free_matrix(adj_matrix_t *adj_matrix);
#endif //XIAL_LOADFILE_H
