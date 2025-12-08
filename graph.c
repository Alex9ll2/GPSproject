#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/map.h"
#include "headers/set.h"
#include "headers/pq.h"
#include "headers/utils.h"

static comp_func GLOBAL_VERTEX_CMP = NULL;


typedef struct Edge {
    void* node;
    int distance;
} Edge;

// Hashmap: key = void*, value = set<Edge*>
typedef struct graph {
    map* adjacencyList;
    comp_func compareFunc;
    hash_func hashFunc;
    equals_func equalsFunc;
    print_func printFunc;
} graph;


int compareEdge(void* a, void* b) 
{
    Edge* e1 = a;
    Edge* e2 = b;

    return GLOBAL_VERTEX_CMP(e1->node, e2->node);
}


int compareEdgeDistance(void* a, void* b) 
{
    Edge* e1 = (Edge*) a;
    Edge* e2 = (Edge*) b;

    return e1->distance - e2->distance;
}

Edge* edge_create(void* node, int distance)
{
    Edge* edge = malloc(sizeof(Edge));
    edge->node = node;
    edge->distance = distance;

    return edge;
}

graph* createGraph(int capacity, hash_func hashF, equals_func equalsF, comp_func compareF, print_func printF) 
{
    graph* g = malloc(sizeof(graph));
    g->adjacencyList = map_create(capacity, hashF, equalsF);
    g->compareFunc = compareF;
    g->hashFunc = hashF;
    g->equalsFunc = equalsF;
    g->printFunc = printF;
    GLOBAL_VERTEX_CMP = compareF;
    return g;
}

void graph_print(graph* g)
{
    if (!g) 
    {
        return;
    }

    map_iterator* it = map_iter_create(g->adjacencyList);

    while (map_iter_has_next(it)) 
    {
        void* key = map_iter_next(it);

        set* edgesSet = map_get(g->adjacencyList, key);

        if (!edgesSet) 
        {
            continue;
        }

        printf("Node: ");
        g->printFunc(key);
        printf(" -> ");

        set_iterator* sit = set_iter_create(edgesSet);

        while (set_iter_has_next(sit)) 
        {
            Edge* edge = set_iter_next(sit);
            printf("[To: ");
            g->printFunc(edge->node);
            printf(", Dist: %d] ", edge->distance);
        }

        set_iter_destroy(sit);

        printf("\n");
    }

    map_iter_destroy(it);
}

bool addEdge(graph* g, void* from, void* to, int distance) 
{
    if (!g || !from || !to) 
    {
        return false;
    }

    // from -> to
    set* edgesSet = map_get(g->adjacencyList, from);
    if (!edgesSet) 
    {
        edgesSet = set_create(compareEdge, NULL);
        map_put(g->adjacencyList, from, edgesSet);
    }


    Edge temp;
    temp.node = to;
    temp.distance = 0;
    
    // evitar duplicados
    if (!set_contains(edgesSet, &temp)) 
    {
        Edge* e = edge_create(to, distance);
        set_add(edgesSet, e);
    }

    // to -> from
    edgesSet = map_get(g->adjacencyList, to);

    if (!edgesSet) 
    {
        edgesSet = set_create(compareEdge, NULL);
        map_put(g->adjacencyList, to, edgesSet);
    }

    temp.node = from;

    if (!set_contains(edgesSet, &temp)) 
    {
        Edge* r = edge_create(from, distance);
        set_add(edgesSet, r);
    }


    return true;
}

set* getNeighbors(graph* g, void* key)
{
    if (!g || !key) 
    {
        return NULL;
    }
    return map_get(g->adjacencyList, key);
}

bool removeEdge(graph* g, void* from, void* to)
{
    if (!g || !from || !to) 
    {
        return false;
    }

    bool removed = false;

    // from -> to
    set* edgesFrom = map_get(g->adjacencyList, from);
    if (edgesFrom)
    {
        set_iterator* it = set_iter_create(edgesFrom);
        while (set_iter_has_next(it))
        {
            Edge* e = set_iter_next(it);
            if (g->equalsFunc(e->node, to))
            {
                set_remove(edgesFrom, e);
                free(e);    
                removed = true;
                break;
            }
        }
        set_iter_destroy(it);
    }

    // to -> from
    set* edgesTo = map_get(g->adjacencyList, to);
    if (edgesTo)
    {
        set_iterator* it = set_iter_create(edgesTo);
        while (set_iter_has_next(it))
        {
            Edge* e = set_iter_next(it);
            if (g->equalsFunc(e->node, from))
            {
                set_remove(edgesTo, e);
                free(e);
                break;
            }
        }
        set_iter_destroy(it);
    }

    return removed;
}


void graph_destroy(graph* g)
{
    if (!g) 
    {
        return;
    }

    // 1) recolectar todas las claves del mapa en un array
    int n = map_size(g->adjacencyList);
    void ** keys = malloc(sizeof(void*) * (n > 0 ? n : 1)); // evita malloc(0)
    int idx = 0;

    map_iterator * it = map_iter_create(g->adjacencyList);

    while (map_iter_has_next(it)) 
    {
        void* k = map_iter_next(it);
        keys[idx++] = k;
    }

    map_iter_destroy(it);

    // 2) destruir sets
    for (int i = 0; i < idx; i++) 
    {
        set* edgesSet = map_get(g->adjacencyList, keys[i]);
        if (!edgesSet) continue;

        set_iterator* sit = set_iter_create(edgesSet);

        while (set_iter_has_next(sit)) 
        {
            Edge* e = set_iter_next(sit);
            free(e);
        }

        set_iter_destroy(sit);

        set_destroy(edgesSet); 
    }

    free(keys);

    map_destroy(g->adjacencyList);
    free(g);
}

//retorna un hashmap<node, int>
//con las distancias más cercanas del origin a cada nodo
map* dijkstra(graph* g, void* origin)
{
    if(map_get(g->adjacencyList, origin) == NULL) 
    {
        return NULL;
    }

    // hashmap <node, int*>
    map* distances = map_create(100, g->hashFunc, g->equalsFunc);

    // priority queue de edges
    pq* pQueue = pq_create(100, compareEdgeDistance);

    // set de nodos
    set* visited = set_create(g->compareFunc, g->printFunc);


    map_iterator* it = map_iter_create(g->adjacencyList);
    while (map_iter_has_next(it)) 
    {
        void* vertex = map_iter_next(it);
        int* dist = malloc(sizeof(int));
        *dist = INT_MAX;
        map_put(distances, vertex, dist);
    }

    int* originDist = malloc(sizeof(int));
    *originDist = 0;
    map_put(distances, origin, originDist);
    Edge* start = edge_create(origin, 0);
    pq_offer(pQueue, start);


    map_iter_destroy(it);

    while (pq_peek(pQueue) != NULL) 
    {
        Edge* current = pq_poll(pQueue);

        int bestDist = *((int*) map_get(distances, current->node));
        if (current->distance != bestDist) 
        {
            //entrada obsoleta
            free(current);
            continue;
        }

        if (set_contains(visited, current->node)) 
        {
            free(current);
            continue;
        }

        set_add(visited, current->node);

        set* neighborsSet = map_get(g->adjacencyList, current->node);
        set_iterator* sit = set_iter_create(neighborsSet);

        while (set_iter_has_next(sit)) 
        {
            Edge* neighbor = set_iter_next(sit);   // edge original del grafo
            void* v = neighbor->node;

            int oldDist = *((int*) map_get(distances, v));
            int newDist = current->distance + neighbor->distance;

            if (newDist < oldDist) 
            {
                int* d = malloc(sizeof(int));
                *d = newDist;
                map_put(distances, v, d);

                Edge* temp = edge_create(v, newDist);
                pq_offer(pQueue, temp);
            }
        }

        set_iter_destroy(sit);
        free(current);     
    }

    set_destroy(visited);
    pq_destroy(pQueue);

    return distances; 
}
