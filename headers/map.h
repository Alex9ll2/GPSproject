#ifndef MAP_H_
#define MAP_H_
#include <stdbool.h>
#include "utils.h"


typedef struct map_str map;

map *  map_create  (int m, hash_func, equals_func);
int  map_size    (map * m);
int  map_capacity(map * m);
void map_put     (map * m, void * key, void * value);
bool map_contains(map * m, void * key);
void * map_get     (map * m, void * key);
void map_destroy(map * m);
void map_print(map* m, print_func printKey, print_func printValue);


/* map iterator */
typedef struct map_iterator_str map_iterator;
map_iterator * map_iter_create(map * m);
bool map_iter_has_next(map_iterator * it);
void * map_iter_next(map_iterator * it);
void map_iter_destroy(map_iterator * it);


#endif /* MAP_H_ */
