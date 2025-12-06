#ifndef UTILS_H
#define UTILS_H

typedef int  (*hash_func)(void *);
typedef bool (*equals_func)(void *, void *);
typedef int  (*comp_func)(void * A, void * B);
typedef void (*print_func)  (void *);



int wordHash(void *t);
bool wordEquals(void * t1, void * t2);
int wordCompare(void * t1, void * t2);
bool intEquals(void* t1, void* t2);
void printString(void * t);
void printInt(void * t);


#endif 


