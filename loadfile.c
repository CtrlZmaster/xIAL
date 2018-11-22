/*
 * Project: IAL ...
 * File: loadfile.c
 * Title: 7. Minimum weight spanning tree
 * Description: ...
 * Author: František Balázsy (xbalaz08@stud.feec.vutbr.cz),
 *         Michal Pospíšil (xpospi95@stud.fit.vutbr.cz),
 *         Nikola Timková (xtimko01@stud.fit.vutbr.cz)
 */


#include "loadfile.h"

/*
 * Function loads file with name filename in current directory.
 * Return codes:
 * 0 = SUCCESS
 * -1 = Can't open file.
 * -2 = Unexpected EOL
 * -3 =
 * -4 = Wrong file type.
 * -100 = Syntax error.
 * -101 = Too many vertices.
 */
int loadToMatrix(char *filename, uint64_t *matrix[], uint8_t *vertices) {
  FILE * file_ptr;
  // Open the file and check the return code
  if((file_ptr = fopen(filename, "r")) == NULL) {
    return -1;
  }

  // Check syntax of the opened file
      //Check the first line with file type declaration
  char filetype_str[FILETYPE_STRLEN + 1];
  if(fgets(filetype_str, FILETYPE_STRLEN + 1,file_ptr) == NULL) {
    return -2;
  }
  if(strcmp(filetype_str, FILETYPE_STR) != 0) {
    return -4;
  }


  // Reading number of vertices from file
  int cur_char;
  char size[4] = { 0 };
  // Checking for syntax errors and really big values
  for(uint8_t i = 0; (cur_char = fgetc(file_ptr)) != '\n'; i++) {
    if(i > 3) {
      return -101;
    }

    if(cur_char == EOF ||) {
      return -2;
    }

    if(cur_char < '0' || cur_char > '9') {
      return -100;
    }

    size[i] = cur_char;
  }
  // Using atoi() is fine - string was checked for errors before, now checking if number fits into receiver type
  if(atoi(size) > UINT8_MAX) {
    return -101;
  }
  *vertices = atoi(size);


}
int checkMatrixSyntaxL1(char *line, uint64_t *matrix[]) {

}

int checkMatrixSyntaxLothers(char* line, uint64_t *matrix[]) {

}



