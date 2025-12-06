#include <stdio.h>
#include <stdlib.h>
#include "headers/map.h"
#include "headers/set.h"
#include "headers/pq.h"
#include "headers/utils.h"

static comp_func GLOBAL_VERTEX_CMP = NULL;


typedef struct Edge {
    void* dest;
    int distance;
} Edge;

// Hashmap: key = void*, value = set<Edge*>
typedef struct graph {
    map* adjacencyList;
    comp_func compareFunc;
    equals_func equalsFunc;
    print_func printFunc;
} graph;

// Compara exclusivamente por destino usando equalsFunc
int compareEdge(void* a, void* b) 
{
    Edge* e1 = (Edge*) a;
    Edge* e2 = (Edge*) b;

    if (GLOBAL_VERTEX_CMP(a, b) == 0) return 0;
    return (e1->dest < e2->dest) ? -1 : 1;
}

graph* createGraph(int capacity, hash_func hashF, equals_func equalsF, comp_func compareF, print_func printF) 
{
    graph* g = malloc(sizeof(graph));
    g->adjacencyList = map_create(capacity, hashF, equalsF);
    g->compareFunc = compareF;
    g->equalsFunc = equalsF;
    g->printFunc = printF;
    GLOBAL_VERTEX_CMP = compareF;
    return g;
}

bool addEdge(graph* g, void* from, void* to, int distance) 
{
    if (!g || !from || !to) return false;

    // printf("Adding edge from ");
    // g->printFunc(from);
    // printf(" to ");
    // g->printFunc(to);
    // printf(" with distance %d\n", distance);

    // FROM → TO
    set* edgesSet = map_get(g->adjacencyList, from);
    if (!edgesSet) {
        edgesSet = set_create(compareEdge, NULL);
        map_put(g->adjacencyList, from, edgesSet);
    }



    Edge temp;
    temp.dest = to;
    
    // evitar duplicados
    if (!set_contains(edgesSet, &temp)) {
        Edge* e = malloc(sizeof(Edge));
        e->dest = to;
        e->distance = distance;
        set_add(edgesSet, e);
    }

    // TO → FROM
    edgesSet = map_get(g->adjacencyList, to);


    if (!edgesSet) {
        edgesSet = set_create(compareEdge, NULL);
        map_put(g->adjacencyList, to, edgesSet);
    }



    temp.dest = from;

    if (!set_contains(edgesSet, &temp)) {
        Edge* r = malloc(sizeof(Edge));
        r->dest = from;
        r->distance = distance;
        set_add(edgesSet, r);
    }



    return true;
}

set* getNeighbors(graph* g, void* key)
{
    if (!g || !key) return NULL;
    return map_get(g->adjacencyList, key);
}

bool removeEdge(graph* g, void* from, void* to)
{
    if (!g || !from || !to) return false;

    bool removed = false;

    // FROM → TO
    set* edgesFrom = map_get(g->adjacencyList, from);
    if (edgesFrom)
    {
        set_iterator* it = set_iter_create(edgesFrom);
        while (set_iter_has_next(it))
        {
            Edge* e = set_iter_next(it);
            if (g->equalsFunc(e->dest, to))
            {
                set_remove(edgesFrom, e);
                free(e);    // primero remove, luego free
                removed = true;
                break;
            }
        }
        set_iter_destroy(it);
    }

    // TO → FROM
    set* edgesTo = map_get(g->adjacencyList, to);
    if (edgesTo)
    {
        set_iterator* it = set_iter_create(edgesTo);
        while (set_iter_has_next(it))
        {
            Edge* e = set_iter_next(it);
            if (g->equalsFunc(e->dest, from))
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

void graph_print(graph* g)
{
    if (!g) return;

    map_iterator* it = map_iter_create(g->adjacencyList);

    while (map_iter_has_next(it)) {
        void* key = map_iter_next(it);

        set* edgesSet = map_get(g->adjacencyList, key);
        if (!edgesSet) continue;

        printf("Node: ");
        g->printFunc(key);
        printf(" -> ");

        set_iterator* sit = set_iter_create(edgesSet);
        while (set_iter_has_next(sit)) {
            Edge* edge = set_iter_next(sit);
            printf("[To: ");
            g->printFunc(edge->dest);
            printf(", Dist: %d] ", edge->distance);
        }
        set_iter_destroy(sit);

        printf("\n");
    }

    map_iter_destroy(it);
}

void graph_destroy(graph* g)
{
    if (!g) return;

    // 1) recolectar todas las claves del mapa en un array
    int n = map_size(g->adjacencyList);
    void ** keys = malloc(sizeof(void*) * (n > 0 ? n : 1)); // evita malloc(0)
    int idx = 0;

    map_iterator * it = map_iter_create(g->adjacencyList);
    while (map_iter_has_next(it)) {
        void* k = map_iter_next(it);
        keys[idx++] = k;
    }
    map_iter_destroy(it);

    // 2) destruir sets
    for (int i = 0; i < idx; i++) {
        set* edgesSet = map_get(g->adjacencyList, keys[i]);
        if (!edgesSet) continue;

        set_iterator* sit = set_iter_create(edgesSet);

        while (set_iter_has_next(sit)) 
        {
            Edge* e = set_iter_next(sit);
            g->printFunc(e->dest);
            free(e);
        }

        set_iter_destroy(sit);

        set_destroy(edgesSet); // libera nodos internos
        free(edgesSet);        // libera struct set creado por set_create
    }

    free(keys);

    map_destroy(g->adjacencyList);
    free(g);
}
