/*
 * Project: IAL ...
 * File: loadfile.c
 * Title: 7. Minimum weight spanning tree
 * Description: Module contains all functions for loading a file
 * Author: František Balázsy (xbalaz08@stud.feec.vutbr.cz),
 *         Michal Pospíšil (xpospi95@stud.fit.vutbr.cz),
 *         Nikola Timková (xtimko01@stud.fit.vutbr.cz)
 */

/*
 * This module implements loading the file (format specified in documentation) and its syntax check. Matrix is then
 * checked again, to confirm that it represents a graph.
 *
 * Return codes in this module:
 *  0 = SUCCESS
 * -1 = Can't open file.
 * -2 = Unexpected EOF.
 * -3 = Wrong call parameters.
 * -4 = Wrong file type.
 * -5 = Memory allocation error.
 * -100 = Syntax error.
 * -101 = Vertex count out of range.
 * -102 = Value out of range.
 * -103 = Edge starts from the same vertex as it ends in.
 * -104 = Mirrored index contains different weight.
 * -105 = Line is missing one or more elements.
 * -106 = Line has more elements than specified.
 * -107 = File contains unexpected content.
 * -108 = Disconnected vertex found.
 */


#include "loadfile.h"


// Main loading function - loads matrix from a file named filename and then passes
// matrix (if the file is valid) to adj_matrix. This should be called from outside of this module.
int loadToMatrix(FILE *file_ptr, adj_matrix_t *adj_matrix) {
  int err_code = 0;

  // Check syntax of the opened file
  // Check the first line with file type declaration
  wchar_t filetype_str[FILETYPE_STRLEN + 1];
  if(fgetws(filetype_str, FILETYPE_STRLEN + 1,file_ptr) == NULL) {
    return -2;
  }
  if(wcscmp(filetype_str, FILETYPE_STR) != 0) {
    return -4;
  }
  wchar_t cur_char = fgetwc(file_ptr);
  if(cur_char != L'\n') {
    // Check that there is a new line after filetype string
    return -4;
  }

  // Reading number of vertices from file - reuses cur_cahr
  wchar_t size[4] = { 0 };
  cur_char = fgetwc(file_ptr);
  // Checking for syntax errors and really big values
  for(uint8_t i = 0; cur_char != L'\n'; i++) {
    if(i > 3) {
      return -101;
    }

    if(cur_char == WEOF) {
      return -2;
    }

    if(cur_char < L'0' || cur_char > L'9') {
      return -100;
    }

    size[i] = cur_char;
    cur_char = fgetwc(file_ptr);
  }
  unsigned long ul_size = wcstoul(size, NULL, 10);
  if(ul_size > UINT8_MAX) {
    // Checking that number fits into receiver type
    return -101;
  }
  adj_matrix->vertices = (uint8_t)ul_size;

  // Can't use fgetws - don't know length of the line!!!
  // Prepare token for first use of lexFSM
  token_t *temp_token = malloc(sizeof(token_t));
  if(temp_token == NULL) {
    return -5;
  }
  temp_token->length = 0;
  temp_token->string = calloc(TOKEN_ALLOC_UNIT, sizeof(wchar_t));
  if(temp_token->string == NULL) {
    return -5;
  }

  // Create final destination for vertex names
  adj_matrix->vertex_names = calloc(adj_matrix->vertices, sizeof(wchar_t*));
  if(adj_matrix->vertex_names == NULL) {
    return -5;
  }
  // Fill it with vertex names
  err_code = lexFSM(file_ptr, adj_matrix->vertices, temp_token, adj_matrix->vertex_names, NULL);
  if(err_code) {
    return err_code;
  }

  // Allocate rows
  adj_matrix->matrix = malloc(adj_matrix->vertices * sizeof(int64_t*));
  if(adj_matrix->matrix == NULL) {
    return -5;
  }
  // Iterate through rows
  for(uint8_t i = 0; i < adj_matrix->vertices; i++) {
    // Allocate all columns (create array of cells)
    adj_matrix->matrix[i] = malloc(adj_matrix->vertices * sizeof(int64_t));
    if(adj_matrix->matrix == NULL) {
      return -5;
    }
    // Fill the matrix
    err_code = lexFSM(file_ptr, adj_matrix->vertices, temp_token, NULL, adj_matrix->matrix[i]);
    if(err_code) {
      return err_code;
    }
  }

  if(fgetwc(file_ptr) != WEOF) {
    return -107;
  }

  free(temp_token->string);
  free(temp_token);

  err_code = is_graph(adj_matrix);
  return err_code;
}

// Reads a line from file and divides it into lexemes. Internally uses two modes to distinguish between reading vertex names,
// or edge weights - mode is detected based on passing NULL to vertex_names or edge_weights.
int lexFSM(FILE * file_ptr, uint8_t lex_num, token_t* temp_token, wchar_t *vertex_names[], int64_t edge_weights[]) {
  // Return values with error codes are saved here
  int err_code = 0;
  // Current char read from a file
  wchar_t cur_char;
  // Keeping track of current lexeme index
  uint8_t cur_lex_num = 0;
  // State flags
  char esc_flag, num_flag, sign_flag;
  esc_flag = num_flag = sign_flag = 0;
  // Function mode - what type of line is read from a file
  char mode;
  if(vertex_names == NULL) {
    // Edge weights mode
    mode = 1;
  }
  else {
    if(edge_weights == NULL) {
      // Vertex names mode
      mode = 0;
    }
    else {
      return -3;
    }
  }
  // Iterates through a whole line and fills the matrix/vertex names
  for(int lex_len = 0;  (cur_char = fgetwc(file_ptr)) != '\n'; lex_len++) {
    // Checks for lines longer than number of vertices
    if(cur_lex_num > lex_num - 1) {
      return -106;
    }
    // FSM
    switch(cur_char) {
      case WEOF:
        // There shouldn't be an EOF while calling this function
        return -2;
      case ',':
        if(esc_flag) {
          // If the ESC flag is set, it will be reset and " will be written
          // to the lexeme after the switch
          esc_flag = 0;
        }
        else {
          // Skip to the next lexeme
          lex_len = -1; // -1 because of ++ after every iteration
          err_code = token_to_retval(temp_token, cur_lex_num, mode, vertex_names, edge_weights);
          if(err_code) {
            return err_code;
          }
          cur_lex_num++;
          err_code = flush_temp_token(temp_token);
          if(err_code) {
            return err_code;
          }
          continue; // Skip writing to current lexeme (go to next char)
        }
        break;
      case '\\':
        if(esc_flag) {
          // If the ESC flag is set, it will be reset and '\' will be written
          // to the lexeme after the switch
          esc_flag = 0;
        }
        else {
          esc_flag = 1;
          lex_len--;
          continue;
        }
        break;
      case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8':
      case '9': case '0':
        num_flag = 1;
        esc_flag = 0;
        break;
      case '-': case '+':
        sign_flag = 1;
        esc_flag = 0;
        break;
      default:
        esc_flag = 0;
        break;
    }

    if(lex_len != 0 && sign_flag == 1 && mode == 1) {
      return -100; // Sign inside or behind the number
    }

    if(num_flag == 0 && mode == 1) {
      if(sign_flag == 0) {
        return -100; // Something other than a number/sign in edge_weights
      }
    }
    //Reset flags for next char
    num_flag = 0;
    sign_flag = 0;

    // Space is running out, need to alloc new space to token (+1 <=> lex_len is indexed from 0
    // and realloc is needed when rewriting last index - represented by TOKEN_ALLOC_UNIT length)
    if((lex_len + 1) % TOKEN_ALLOC_UNIT == 0) {
      realloc_temp_token(temp_token, lex_len + 1 + TOKEN_ALLOC_UNIT);
    }
    // Current char is correct and is written to temporary  token
    temp_token->string[lex_len] = cur_char;
    temp_token->length++;
  }

  // Converting last token when \n is detected
  err_code = token_to_retval(temp_token, cur_lex_num, mode, vertex_names, edge_weights);
  if(err_code) {
    return err_code;
  }
  err_code = flush_temp_token(temp_token);
  if(err_code) {
    return err_code;
  }

  // Check that the line had enough lexemes
  if(cur_lex_num + 1 != lex_num) {
    return -105;
  }

  return err_code;
}

// Destroys contents of a previous token
int flush_temp_token(token_t *temp_token) {
  free(temp_token->string);
  temp_token->length = 0;

  temp_token->string = calloc(TOKEN_ALLOC_UNIT, sizeof(wchar_t));
  if(temp_token->string == NULL) {
    return -5;
  }

  return 0;
}

// Enlarges token to allow storing bigger input "strings"
int realloc_temp_token(token_t *temp_token, size_t size) {
  // Copy data to local stash
  wchar_t temp_data[size + 1];
  wcsncpy(temp_data, temp_token->string, temp_token->length);

  // Free old memory
  free(temp_token->string);

  // Allocate new memory
  temp_token->string = calloc(sizeof(wchar_t), size);
  if(temp_token->string == NULL) {
    return -5;
  }

  // Copy back to bigger memory from stash
  wcsncpy(temp_token->string, temp_data, temp_token->length);

  return 0;
}

// Converts and writes a token (edge weight or vertex name) to a corresponding array
int token_to_retval(token_t *temp_token, size_t index, char mode, wchar_t *vertex_names[], int64_t edge_weights[]) {
  if(mode) {
    // Mode = 1 - Edge weights mode
    edge_weights[index] = wcstoll(temp_token->string, NULL, 10);
    if(errno == ERANGE) {
      return -102;
    }
  }
  else {
    // Mode = 0 - Vertex names mode
    vertex_names[index] = calloc(temp_token->length + 1, sizeof(wchar_t));
    if(vertex_names[index] == NULL) {
      return -5;
    }
    wcsncpy(vertex_names[index], temp_token->string, temp_token->length);
  }

  return 0;
}

// Function checks that matrix represents an undirected graph
int is_graph(adj_matrix_t *adj_matrix) {
  // Test for disconnected vertices (max 255 vertices - can't init VLAs)
  int64_t *rows = calloc(adj_matrix->vertices, sizeof(int64_t));
  if(rows == NULL) {
    return -5;
  }

  // Goes through half of the matrix
  for(int i = 0; i < adj_matrix->vertices; i++) {
    for(int j = i; j < adj_matrix->vertices; j++) {
      // Checks that there are zeroes on the diagonal
      if(i == j) {
        if(adj_matrix->matrix[i][j] != 0) {
          return -103;
        }
      }
      // Comparing mirrored edge weight
      if(adj_matrix->matrix[i][j] != adj_matrix->matrix[j][i]) {
        return -104;
      }

    }
  }
  // Looking for disconnected vertices - "bitwise or"-ing through lines - 0 if there are no connections
  // Can be used only in undirected graphs - relies on symmetry in the matrix
  for(int i = 0; i < adj_matrix->vertices; i++) {
    for(int j = 0; j < adj_matrix->vertices; j++) {
      rows[i] |= adj_matrix->matrix[i][j];
    }
  }
  // Looking for rows with no connections
  for(int i = 0; i < adj_matrix->vertices; i++) {
    if(rows[i] == 0) {
      free(rows);
      return -108;
    }
  }

  free(rows);
  return 0;
}


// Frees the matrix
void free_matrix(adj_matrix_t *adj_matrix) {
  //Free vertex names
  for(uint8_t i = 0; i < adj_matrix->vertices; i++) {
    free(adj_matrix->vertex_names[i]);
  }

  // Free vertex name pointers
  free(adj_matrix->vertex_names);

  // Iterate through rows
  for(uint8_t i = 0; i < adj_matrix->vertices; i++) {
    free(adj_matrix->matrix[i]);
  }
  // Clean row pointers
  free(adj_matrix->matrix);

  // Free the struct
  free(adj_matrix);
}

// Writes error message to stderr
void loaderror(int retval) {
  fprintf(stderr, "ERROR: ");
  switch(retval) {
    case 0:
      break;
    case -1:
      fprintf(stderr, "Can't open the specified file.\n");
      break;
    case -2:
      fprintf(stderr, "Unexpected end of file.\n");
      break;
    case -3:
      fprintf(stderr, "Wrong function call parameters (internal error).\n");
      break;
    case -4:
      fprintf(stderr, "Unsupported file type.\n");
      break;
    case -5:
      fprintf(stderr, "Memory allocation error.\n");
      break;
    case -100:
      fprintf(stderr, "Syntax error in graph representation.\n");
      break;
    case -101:
      fprintf(stderr, "Vertex count out of range.\n");
      break;
    case -102:
      fprintf(stderr, "Value out of range.\n");
      break;
    case -103:
      fprintf(stderr, "Edge starts from the same vertex as it ends in.\n");
      break;
    case -104:
      fprintf(stderr, "Mirrored index contains different weight.\n");
      break;
    case -105:
      fprintf(stderr, "Line is missing one or more elements.\n");
      break;
    case -106:
      fprintf(stderr, "Line has more elements than specified.\n");
      break;
    case -107:
      fprintf(stderr, "File contains unexpected content.\n");
    case -108:
      fprintf(stderr, "Disconnected vertex found.\n");
    break;
    default:
      break;
  }
}