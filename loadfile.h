/*
 * Project: IAL Replacement project
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

// String that is in the beginning of a file to distinguish it from other file types
#define FILETYPE_STR L".xIALAdjMatrix"
// Length of the filetype string
#define FILETYPE_STRLEN wcslen(FILETYPE_STR)

#define TOKEN_ALLOC_UNIT 10

// Holds a lexeme from input while it's being loaded - one struct is reused for every lexeme internally
typedef struct token {
  wchar_t* string;   // What is being loaded from file
  size_t length;     // Current length of something being loaded
} token_t;

// Holds the loaded matrix from a file
typedef struct adj_matrix {
  uint32_t **matrix;           // The adjacency matrix
  wchar_t **vertex_names;     // Vertex names
  uint8_t vertices;           // Number of vertices in a file
} adj_matrix_t;

// Main loading function - loads matrix from a file named filename and then passes
// matrix (if the file is valid) to adj_matrix. This should be called from outside of this module.
int loadToMatrix(FILE *file_ptr, adj_matrix_t *adj_matrix);

// Reads a line from file and divides it into lexemes. Internally uses two modes to distinguish between reading vertex names,
// or edge weights - mode is detected based on passing NULL to vertex_names or edge_weights.
int lexFSM(FILE * file_ptr, uint8_t lex_num, token_t* temp_token, wchar_t *vertex_names[], uint32_t edge_weights[]);

// Destroys contents of a previous token
int flush_temp_token(token_t *temp_token);

// Enlarges token to allow storing bigger input "strings"
int realloc_temp_token(token_t *temp_token, size_t size);

// Converts and writes a token (edge weight or vertex name) to a corresponding array
int token_to_retval(token_t *temp_token, size_t index, char mode, wchar_t *vertex_names[], uint32_t edge_weights[]);

// Function checks that matrix represents an undirected graph
int is_graph(adj_matrix_t *adj_matrix);

// Frees the matrix
void free_matrix(adj_matrix_t *adj_matrix);

// Writes error message to stderr
void loaderror(int retval);


#endif //XIAL_LOADFILE_H
