#include <stdio.h>
#include <stdlib.h>
#include "headers/map.h"
#include "headers/set.h"
#include "headers/utils.h"
#include "headers/graph.h"

#define INITIAL_CAPACITY 100

//gcc main.c graph.c map.c set.c pq.c utils.c -o app
//graph
int main()
{

  //graph example
  graph* g = createGraph(INITIAL_CAPACITY, wordHash, wordEquals, wordCompare, printString);
  addEdge(g, "A", "C", 2);
  addEdge(g, "A", "B", 5);
  addEdge(g, "C", "B", 7);
  addEdge(g, "B", "D", 8);
  addEdge(g, "C", "D", 4);
  addEdge(g, "C", "E", 8);
  addEdge(g, "E", "D", 6);
  addEdge(g, "E", "F", 3);
  addEdge(g, "D", "F", 4);

  printf("Graph:\n");
  graph_print(g);

  char* origin1 = "A";
  char* origin2 = "B";
  char* origin3 = "C";
  char* origin4 = "D";
  char* origin5 = "E";
  char* origin6 = "F";

  map* m1 = dijkstra(g, origin1);
  map* m2 = dijkstra(g, origin2);
  map* m3 = dijkstra(g, origin3);
  map* m4 = dijkstra(g, origin4);
  map* m5 = dijkstra(g, origin5);
  map* m6 = dijkstra(g, origin6);

  printf("Dijkstra, origin: %s\n", origin1);
  map_print(m1, printString, printInt);

  printf("Dijkstra, origin: %s\n", origin2);
  map_print(m2, printString, printInt);

  printf("Dijkstra, origin: %s\n", origin3);
  map_print(m3, printString, printInt);

  printf("Dijkstra, origin: %s\n", origin4);
  map_print(m3, printString, printInt);

  printf("Dijkstra, origin: %s\n", origin5);
  map_print(m3, printString, printInt);

  printf("Dijkstra, origin: %s\n", origin6);
  map_print(m3, printString, printInt);

  map_destroy(m1);
  map_destroy(m2);
  map_destroy(m3);
  map_destroy(m4);
  map_destroy(m5);
  map_destroy(m6);
  
  graph_destroy(g);

  return 0;
}