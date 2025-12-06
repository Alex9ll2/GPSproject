#ifndef MAP_H_
#define MAP_H_
#include <stdbool.h>


typedef int  (*hash_func)(void *);
typedef bool (*equals_func)(void *, void *);
typedef struct map_str map;

map *  map_create  (int m, hash_func, equals_func);
int  map_size    (map * m);
void map_put     (map * m, void * key, void * value);
bool map_contains(map * m, void * key);
void * map_get     (map * m, void * key);

#endif /* MAP_H_ */
