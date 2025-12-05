/*
 * Este archivo es el header que conectará
 * a city.c con el usuario
*/

#ifndef CITIES_H_
    #include <stdbool.h>
    #define CITIES_H_
    typedef struct city City;
    
    City * createCity(char name[], int neighbors); // Crear una ciudad desde ceros
    bool connectCities(City * C1, City * C2); // Connect to cities

#endif