#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include "map.h"

typedef struct graph graph;

graph* createGraph(int capacity, 
                    int (*hash)(void *t), 
                    bool (*equals)(void * t1, void * t2), 
                    int (*compare)(void * t1, void * t2),
                    void (*print)(void * t));
                    
bool addEdge(graph* g, void* from, void* to, int weight);
void graph_print(graph* g);
bool removeEdge(graph* g, void* from, void* to);
graph* getNeighbors(graph* g, void* key);
void graph_destroy(graph* g);
map* dijkstra(graph* g, void* origin);


    
#endif