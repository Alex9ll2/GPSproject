#include <stdio.h>
#include <stdlib.h>
#include "headers/cities.h"
#include "headers/map.h"
#include "headers/set.h"
#include "headers/pq.h"
#include "headers/utils.h"


map* dijkstra(map* adjacencyList, City* origin)
{
    map* distances = map_create(100, wordHash, wordEquals);
    pq* pQueue = pq_create(100, wordEquals);
    set* treeSet = set_create(wordCompare, printString);

    for(int i = 0; i < map_capacity(adjacencyList); i++)
    {
        //inicializar todas las distancias a infinito
        char* cityName = (char*)map_get(distances, i);
        float* distance = malloc(sizeof(float));
        *distance = __FLT_MAX__;
        map_put(distances, cityName, distance);
    }
}