/*
 * Project: IAL ...
 * File: main.c
 * Title: 7. Minimum weight spanning tree
 * Description: ...
 * Author: František Balázsy (xbalaz08@stud.feec.vutbr.cz),
 *         Michal Pospíšil (xpospi95@stud.fit.vutbr.cz),
 *         Nikola Timková (xtimko01@stud.fit.vutbr.cz)
 */

#include "main.h"
#include "loadfile.h"

void loadEdges(adj_matrix_t *matrix, graph_t *graph);
void dealloc(graph_t *graph);
void execute(adj_matrix_t *matrix);
void printMST(graph_t *graph, partition_t *partition);
void partition(partition_t *tmp, graph_t *graph, partition_t **pListHead);
void pListAppendNode(partition_t **pListHead, partition_t *node);
void pListRemoveNode(partition_t **pListHead, partition_t **pListTMP);
void sortEdges(graph_t *graph);
void printList(partition_t ** pListHead, graph_t * graph);


void loadEdges(adj_matrix_t *matrix, graph_t *graph)
{
    graph->noOfVertices = matrix->vertices;

    uint16_t edgeCount = 0;
    for (uint8_t x = 0; x < graph->noOfVertices; x++)
    {
        for (uint8_t y = 0; y < x; y++)
        {
            if (matrix->matrix[x][y] > 0)
            {
                edgeCount++;
            }
        }
    }

    graph->noOfEdges = edgeCount;
    graph->edges = malloc(sizeof(edge_t) * edgeCount);

    edgeCount = 0;
    for (uint8_t x = 0; x < graph->noOfVertices; x++)
    {
        for (uint8_t y = 0; y < x; y++)
        {
            if (matrix->matrix[x][y] > 0)
            {
                graph->edges[edgeCount].from = x;
                graph->edges[edgeCount].to = y;
                graph->edges[edgeCount].weight = matrix->matrix[x][y];
                edgeCount++;
            }
        }
    }
}

void sortEdges(graph_t *graph)
{

    edge_t tmp;

    for (uint8_t i = 1; i < graph->noOfEdges; i++)
    {
        for (uint8_t j = 0; j < graph->noOfEdges - 1; j++)
        {

            if (graph->edges[j].weight > graph->edges[j + 1].weight)
            {

                tmp = graph->edges[j];
                graph->edges[j] = graph->edges[j + 1];
                graph->edges[j + 1] = tmp;
            }
        }
    }
}

/**
 * Finds MST of a given partition with weight less or equal to maxWeight
 * 
 * typedef struct partition {
 *   char * status;
 *   uint64_t weight;
 *   bool found;
 *   struct partition * next;
 * } partition_t;
 * 
 * 
 * 
 * if MST found, sets partition->found
 * 
 * MST is then represented by partition->status  (included, excluded, open)
 * MST weight is partition->weight
**/

void kruskal(graph_t *graph, partition_t *partition, uint64_t maxWeight)
{

    uint16_t i;
    uint64_t mincost = 0;
    uint8_t included = 0;

    // Every vertice is its own parent, no edges yet
    uint8_t parent[graph->noOfVertices];
    for (i = 0; i < graph->noOfVertices; i++)
    {
        parent[i] = i;
    }

    // First cycle; search for must-be-included/excluded edges
    for (i = 0; i < graph->noOfEdges; i++)
    {
        if (partition->status[i] == 'i')
        {
            parent[graph->edges[i].from] = parent[graph->edges[i].to];
            mincost = mincost + graph->edges[i].weight;
            included++;
            partition->inMST[i] = 1;
        }
        else             partition->inMST[i] = 0;

    }

    // Second cycle, solving open edges
    for (i = 0; i < graph->noOfEdges; i++)
    {

        if (partition->status[i] == 'o')
        {

            // edge can be included
            if (parent[graph->edges[i].from] != parent[graph->edges[i].to])
            {
                parent[graph->edges[i].from] = parent[graph->edges[i].to];
                mincost = mincost + graph->edges[i].weight;
                if (mincost > maxWeight)
                {
                    break;
                }
                partition->inMST[i] = 1;
                included++;
            }
        }
        if (included >= graph->noOfVertices - 1)
        {
            break;
        }
    }

    if (included == graph->noOfVertices - 1)
    {
        partition->weight = mincost;
        partition->found = true;
    }
    else
        partition->found = false;


// printf("\n\n\n");
//     for (int x = 0; x < graph->noOfEdges; x++){
//                     printf("  (f: %d, t: %d, w: %d,s: %c, inMST: %d)", graph->edges[x].from, graph->edges[x].to,graph->edges[x].weight, partition->status[x], partition->inMST[x]);

//     }
// printf("\n\n\n");


}

int main(int argc, char *argv[])
{
    // Resets locale from C standard to environment locale (particularly ASCII to UTF-8)
    setlocale(LC_ALL, "");

    int err_code = 0;

    // Checking parameters
    if (argc != 2)
    {
        fprintf(stderr, "ERROR: Wrong parameter count. Rerun with -h for help.\n");
    }

    FILE *file_ptr;
    if (!strcmp(argv[1], "-h"))
    {
        print_help();
        return 0;
    }
    else
    {
        // Open the file and check the return code
        if ((file_ptr = fopen(argv[1], "r")) == NULL)
        {
            fprintf(stderr, "ERROR: File with name \"%s\" couldn't be opened.\n", argv[1]);
            return -1;
        }
    }

    // Allocate matrix structure where the adjacency matrix will be stored
    adj_matrix_t *matrix = malloc(sizeof(adj_matrix_t));
    if (matrix == NULL)
    {
        fprintf(stderr, "ERROR: Allocation error. Check the amount of free memory on your system.\n");
        return -5;
    }

    // Fill the matrix from the file
    err_code = loadToMatrix(file_ptr, matrix);
    if (err_code)
    {
        return err_code;
    }

    fclose(file_ptr);

    //TODO: Kruskal/Prim

    execute(matrix);

    // Freeing the memory
    free_matrix(matrix);
    return err_code;
}

void execute(adj_matrix_t *matrix)
{

    uint64_t minWeight = UINT64_MAX;
    graph_t graph;
    loadEdges(matrix, &graph);

    sortEdges(&graph);

    partition_t * pListHead = malloc(sizeof(partition_t));
    //TODO check malloc

    pListHead->found = false;
    pListHead->status = NULL;
    pListHead->inMST = NULL;
    pListHead->next = NULL;
    pListHead->status = malloc(sizeof(uint8_t) * graph.noOfEdges);
    pListHead->inMST = malloc(sizeof(uint8_t) * graph.noOfEdges);

    if (pListHead->status == NULL)
    {
        ;
    }
    //TODO check malloc

    for (uint16_t i = 0; i < graph.noOfEdges; i++)
    {
        pListHead->status[i] = 'o';
    }

    partition_t *pListTMP = pListHead;


    kruskal(&graph, pListTMP, minWeight);


    while (1)
    {

 //printList(&pListHead, &graph);

                pListTMP = pListHead;

        bool foundMinWeight = false;

        // Calculate_MST of partition in the list
        // while (pListTMP != NULL)
        // {
        //     if (pListTMP->found && pListTMP->weight <= minWeight)
        //     {
        //         printf("kruskal found new MST, weight: %ld\n", pListTMP->weight);
        //         minWeight = pListTMP->weight;
        //         foundMinWeight = true;
        //         pListTMP = pListTMP->next;
        //     }
        //     else
        //     { // remove it
        //         printf("remove node start\n");

        //         pListRemoveNode(&pListHead, &pListTMP);
        //         printf("remove node end\n");
        //         if (pListTMP)
        //             printf("tmp not null\n");
        //         else
        //             printf("tmp is null\n");
        //     }
        // }

        // if (!foundMinWeight)
        //     break; // no ST found or all ST's are heavier than minimum
        pListTMP = pListHead;
        //  Get partition Ps ∈ List that contains the smallest spanning tree
        while (pListTMP != NULL)
        {
            if (pListTMP->found && pListTMP->weight <= minWeight)
            {
                minWeight = pListTMP->weight;
                foundMinWeight = true;

                // Write MST of Ps to Output_File
                printMST(&graph, pListTMP);

                // Remove Ps from List
                partition_t *tmp = pListTMP;



                // Partition Ps
                partition(tmp, &graph, &pListHead);

                pListRemoveNode(&pListHead, &pListTMP);

                //free(tmp);
                break;
            }
            pListTMP = pListTMP->next;
        }
        if (!foundMinWeight) {

            break;
        }
    }

    free(graph.edges);
}

void partition(partition_t *tmp, graph_t *graph, partition_t **pListHead)
{
    tmp->found = false;

    partition_t p1;
    partition_t p2;
    p1.found = false;
    p1.next = NULL;

    p1.status = malloc(sizeof(uint8_t) * graph->noOfEdges);
    p1.inMST = malloc(sizeof(uint8_t) * graph->noOfEdges);


    memcpy(p1.status, tmp->status, sizeof(uint8_t) * graph->noOfEdges);

    p2.found = false;
    p2.next = NULL;
    p2.status = malloc(sizeof(uint8_t) * graph->noOfEdges);
    p2.inMST = malloc(sizeof(uint8_t) * graph->noOfEdges);

    memcpy(p2.status, tmp->status, sizeof(uint8_t) * graph->noOfEdges);

    /*p2.found = false;
    p2.next = NULL;
    //p2.status = malloc(sizeof(char) * graph->noOfEdges);
    
    if (p2.status == NULL){
     printf("malloc null\n");

    }
       memcpy(p2.status, tmp->status, sizeof(char) * graph->noOfEdges);
    printf("memcpy\n");
*/
    //for each edge i in P do
    for (uint16_t i = 0; i < graph->noOfEdges; i++)
    {
        if (tmp->inMST[i]){
        //if i not included in P and not excluded from P then
        if (tmp->status[i] == 'o')
        {
            //make i excluded from P1;
            p1.status[i] = 'e';
            // make i included in P2;
            p2.status[i] = 'i';

            //Calculate_MST (P1);
            kruskal(graph, &p1, tmp->weight);


// printf("\n--------------------------------------------------------------------------");
//     for (int x = 0; x < graph->noOfEdges; x++){
//                     printf("  (f: %d, t: %d, w: %d,s: %c, inMST: %d)", graph->edges[x].from, graph->edges[x].to,graph->edges[x].weight, p1.status[x], p1.inMST[x]);

//     }
// printf("\n-------------------------------------------------------------");

            //if Connected (P1) then
            if (p1.found)
            {
                // add P1 to List;
                pListAppendNode(pListHead, &p1);

              //  printf("found new partition\n");
                          p1.status = malloc(sizeof(char) * graph->noOfEdges);
            p1.inMST = malloc(sizeof(char) * graph->noOfEdges);

            }

            //p1 = p2;
            p1.found = false;

            memcpy(p1.status, p2.status, graph->noOfEdges);
        }
        }
    }
            free (p1.status);
    free(p1.inMST);
    
    free (p2.status);
    free(p2.inMST);
}

void pListAppendNode(partition_t **pListHead, partition_t *node)
{
    partition_t * newNode = malloc(sizeof(partition_t));
    newNode->next = NULL;
    newNode->status = node->status;
    newNode->inMST = node->inMST;
    newNode->weight = node->weight;
    newNode->found = node->found;

    partition_t *tmp = *pListHead;
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = newNode;
}

void pListRemoveNode(partition_t ** pListHead, partition_t **pListTMP)
{
    if (pListTMP == NULL)
    {
        printf("tried removing null\n");
        return;
    }
    else if (*pListHead == *pListTMP)
    {

        partition_t *del = *pListHead;
        *pListHead = (*pListHead)->next;
        pListTMP = pListHead;

        // if (pListTMP)
        //     printf("head not null\n");
        // else
        //     printf("head is null\n");
        //         printf("%d", (int) pListTMP);

        free(del->status);
        free(del->inMST);
        free(del);
        return;
    }
    else
    { // not a head
        printf("removing node\n");

        partition_t *tmp = *pListHead;
        while (tmp->next != *pListTMP)
        {
            tmp = tmp->next;
        }
        tmp->next = (*pListTMP)->next;
        free((*pListTMP)->status);
        free((*pListTMP)->inMST);
        free(pListTMP);
        *pListTMP = tmp->next;
        return;
    }
}

void printMST(graph_t *graph, partition_t *partition)
{
    printf("Found MST with weight %ld:\n", partition->weight);
    for (uint16_t i = 0; i < graph->noOfEdges; i++)
    {
        if (partition->inMST[i])
        {
            printf("(%d , %d) - weight %d\n", graph->edges[i].from, graph->edges[i].to, graph->edges[i].weight);
        }
    }
    printf("\n\n");
}

void printList(partition_t ** pListHead, graph_t * graph){
    partition_t * tmp = *pListHead;
    int i = 0;
    printf("\nprinting list: \n");
    while(tmp != NULL){
        printf("node %d (w= %ld, found = %d):\n", i++, tmp->weight, tmp->found);
        for(int x = 0; x < graph->noOfEdges; x++){
            printf("  (f: %d, t: %d, w: %d,s: %c, inMST: %d)", graph->edges[x].from, graph->edges[x].to,graph->edges[x].weight, tmp->status[x], tmp->inMST[x]);
        }
        tmp = tmp->next;
            printf("\n");

    }
    printf("\n\n");
}

// Prints help to standard output
void print_help(void)
{
    puts("NAME\n\t./sptree - list all minimum weight spanning trees of a graph\n");
    puts("SYNOPSIS\n\t./sptree [-h]\n\t         [<filename>]\n");
    puts("DESCRIPTION\n\tThis program prints all minimum weight spanning trees in a graph loaded from a text file. "
         "Program requires only one argument - name of the file. Please, use UTF-8 encoding for your files. "
         "Alternatively, you can use -h switch to display this help.\n");
    puts("AUTHORS\n\tFrantišek Balázsy (xbalaz08@stud.feec.vutbr.cz),\n"
         "\tMichal Pospíšil (xpospi95@stud.fit.vutbr.cz),\n"
         "\tNikola Timková (xtimko01@stud.fit.vutbr.cz)\n");
}
