/*
 * Project: IAL ...
 * File: main.h
 * Title: 7. Minimum weight spanning tree
 * Description: ...
 * Author: František Balázsy (xbalaz08@stud.feec.vutbr.cz),
 *         Michal Pospíšil (xpospi95@stud.fit.vutbr.cz),
 *         Nikola Timková (xtimko01@stud.fit.vutbr.cz)
 */

#ifndef XIAL_MAIN_H
#define XIAL_MAIN_H
#include "loadfile.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>


void print_help(void);

typedef struct edge {
  uint8_t from;
  uint8_t to;
  int32_t weight;
} edge_t;

typedef struct tree {
struct edge * edges;
int64_t weight;
} tree_t;

typedef struct graph{
    edge_t * edges;
    uint16_t noOfEdges;
    uint8_t noOfVertices;
} graph_t;

typedef struct partition {
  uint8_t * status;
  uint8_t * inMST;
  uint64_t weight;
  bool found;
  struct partition * next;
} partition_t;



#endif //XIAL_MAIN_H
