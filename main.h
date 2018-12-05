/*
 * Project: IAL Replacement project
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
#include <time.h>


void print_help(void);

typedef struct edge {
  uint8_t from;
  uint8_t to;
  uint32_t weight;
} edge_t;

typedef struct tree {
struct edge * edges;
uint32_t weight;
} tree_t;

typedef struct graph{
    edge_t * edges;
    uint16_t noOfEdges;
    uint8_t noOfVertices;
} graph_t;

typedef struct partition {
  uint8_t * status;
  uint8_t * inMST;
  uint32_t weight;
  bool found;
  struct partition * next;
} partition_t;

void loadEdges(adj_matrix_t *matrix, graph_t *graph);
void dealloc(graph_t *graph);
void execute(adj_matrix_t *matrix);
void printMST(graph_t *graph, partition_t *partition, adj_matrix_t * matrix);
void partition(partition_t *tmp, graph_t *graph, partition_t **pListHead);
void pListAppendNode(partition_t **pListHead, partition_t *node);
void pListRemoveNode(partition_t **pListHead, partition_t **pListTMP);
void sortEdges(graph_t *graph);
void printList(partition_t ** pListHead, graph_t * graph);

#endif //XIAL_MAIN_H
