#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "headers/graph.h"
#include "headers/utils.h"

int wordHash(void *t)
{
    char *key = (char *)t;
    unsigned int hash = 5381;
    
    for (int i = 0; key[i] != '\0'; i++) 
    {
        hash = 33 * hash + (unsigned char)key[i];
    }
    
    return hash;
}

bool wordEquals(void * t1, void * t2) 
{
  char *c1 = (char *)t1;
  char *c2 = (char *)t2;
  //printf("comparing %s with %s\n", c1, c2);
  return strcmp(c1, c2) == 0;
}

//funcion para comparar strings -1, si la primera es menor a la otra, 0 si son iguales y 1 si la primera es mayor
int wordCompare(void * t1, void * t2) 
{
  char *c1 = (char *)t1;
  char *c2 = (char *)t2;
  return strcmp(c1, c2); //retorna <0 si c1 < c2, 0 si son iguales, >0 si c1 > c2
}

bool intEquals(void* t1, void* t2)
{
  int* i1 = (int*)t1;
  int* i2 = (int*)t2;

  return *i1 == *i2;
}

void printString(void * t) 
{
  char* c = (char*) t;
  printf("%s", c);
}

void printInt(void * t) 
{
  int* i = (int*) t;
  printf("%d", *i);
}


