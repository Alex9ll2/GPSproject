#include <stdio.h>
#include <stdlib.h>
#include "headers/map.h"
#include "headers/set.h"
#include "headers/utils.h"
#include "headers/graph.h"

#define INITIAL_CAPACITY 100

//gcc main.c graph.c map.c set.c utils.c -o app
//graph
int main()
{

  //graph example
  graph* g = createGraph(INITIAL_CAPACITY, wordHash, wordEquals, wordCompare, printString);
  addEdge(g, "a", "b", 5);
  addEdge(g, "a", "c", 10);
  addEdge(g, "b", "c", 15);
  addEdge(g, "c", "d", 20);
  graph_print(g);
  graph_destroy(g);


  return 0;
}