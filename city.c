/*
 * ------ Descripción ------------
 * Este archivo contiene la configuración de una
 * ciudad, el usuario no accederá a esta configuración.
 * Funciones para manipular City:
 * -    City * createCity(char, int): Crear una ciudad
 * -    bool connectCities(City*, City*): Conectar ciudades
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../headers/cities.h"

/*
 * -------- Estructura ciudad ------------
 * name-> Nombre de la ciudad
 * neighbors-> Cantidad de vecinos que tiene la ciudad
 * around_neighbors-> Mapa de ciudades. El nombre de la ciudad
 * es la key, y el valor es el peso a esa ciudad
 */

typedef struct Edge {
    struct City* destino;
    float distancia;
} Edge;

typedef struct City {
    char name[35];
    int neighbors;      // número actual de vecinos
    int capacity;       // tamaño del arreglo
    Edge* around;       // arreglo dinámico de aristas
} City;


City* createCity(const char* name) 
{
    City* c = calloc(1, sizeof(City));
    strcpy(c->name, name);

    c->capacity = 4; // capacidad inicial
    c->around = calloc(c->capacity, sizeof(Edge));
    c->neighbors = 0;

    return c;
}


bool connectCities(City* A, City* B, float distancia) 
{
    if (!A || !B || A == B) 
    {
        return false;
    }

    // redimensionar si es necesario
    if (A->neighbors == A->capacity) 
    {
        A->capacity *= 2;
        A->around = realloc(A->around, A->capacity * sizeof(Edge));
    }

    if (B->neighbors == B->capacity) 
    {
        B->capacity *= 2;
        B->around = realloc(B->around, B->capacity * sizeof(Edge));
    }

    // grafo no dirigido
    A->around[A->neighbors].destino = B;
    A->around[A->neighbors].distancia = distancia;
    A->neighbors++;

    B->around[B->neighbors].destino = A;
    B->around[B->neighbors].distancia = distancia;
    B->neighbors++;

    return true;
}

