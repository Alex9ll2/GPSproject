/*
 * Este archivo es el header que conectará
 * a city.c con el usuario
*/

#ifndef CITIES_H_
#define CITIES_H_

    #include <stdbool.h>
    typedef struct city City;
    
    City* createCity(const char* name);  //crear una ciudad desde ceros
    bool connectCities(City* A, City* B, float distancia); //conectar dos ciudades

#endif