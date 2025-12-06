#ifndef SET_H_
#define SET_H_

#include <stdbool.h>
/* Function pointer for the comparison function; this is programmed
by the user in order to use the tree. The user is responsible for 
determining if one object to be stored in the tree is greater than the other.

This function could also be defined as:
typedef int  (*comp_func)(void *, void *);
It's optional to name the variables in function prototypes.
For this example, we use A and B to illustrate the following:

If both objects are equal       = returns 0  
If object A < B                 = returns -1  
If object A > B                 = returns 1 */
typedef int  (*comp_func)(void * A, void * B);

/* Function pointer to a function that prints the data.
The tree doesn't know what it's storing (they are just void *),
so it needs to know how to interpret and print the data. This user-provided
function helps with that task */
typedef void (*print_func)  (void *);

/* Our Set */
typedef struct set_str set;
typedef struct set_iterator_str set_iterator;

/* Public functions of the Set ADT */
set * set_create(comp_func, print_func);
int  set_size(set *);
bool set_add(set *, void *);
bool set_remove(set* s, void* data);
bool set_contains(set *, void *);
void set_print(set *);
void set_destroy(set *);
set_iterator* set_iter_create(set* s);
bool set_iter_has_next(set_iterator* it);
void* set_iter_next(set_iterator* it);
void set_iter_destroy(set_iterator* it);

#endif /* SET_H_ */
