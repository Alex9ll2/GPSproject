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

struct city{
    char name[35];
    int neighbors;
    struct city ** around_neighbors;
};

City * createCity(char name[], int neighbors){
    City * newCity = (City *)calloc(1, sizeof(City));
    strcpy(newCity->name, name);
    newCity->neighbors = neighbors;
    City ** cities = (City **)calloc(neighbors, sizeof(City *));
    newCity->around_neighbors = cities;
    return newCity;
}

// Conectar ciudades
bool connectCities(City * C1, City * C2){
    // Corner cases
    if(C1 == C2) return false;
    if(C1 == NULL || C2 == NULL) return false;

    // Key generation
    unsigned long key_City1 = hashFunction(C1->name);
    unsigned long key_City2 = hashFunction(C2->name);

    // Adding the cities in their in
    int index2 = key_City2 / C1->neighbors;
    int index1 = key_City1 / C2->neighbors;

    C1->around_neighbors[index2] = C2;
    C2->around_neighbors[index1] = C1;  

    return true;
}

// Funcion hash de Dan Bernsteing AKA DJB2
unsigned long hashFunction(char cityName[]){
    unsigned long hash = 5381;
    int c;
    while (c = *cityName++)
        hash = ((hash << 5) + hash) + c;

    return hash;  
}
