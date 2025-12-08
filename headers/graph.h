#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include "map.h"

typedef struct graph graph;

graph* graph_create(int capacity, 
                    int (*hash)(void *t), 
                    bool (*equals)(void * t1, void * t2), 
                    int (*compare)(void * t1, void * t2),
                    void (*print)(void * t));
                    
bool graph_addEdge(graph* g, void* from, void* to, int weight);
void graph_print(graph* g);
bool graph_removeEdge(graph* g, void* from, void* to);
bool graph_removeVertex(graph* g, void* vertex);
graph* getNeighbors(graph* g, void* key);
void graph_destroy(graph* g);
map* graph_dijkstra(graph* g, void* origin);


    
#endif