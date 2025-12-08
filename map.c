  #include "headers/map.h"
  #include "headers/utils.h"
  #include <stdlib.h>
  #include <stdio.h>
  
  struct node_str {
    void * key, * value;
    struct node_str * next;
  };
  
  typedef struct node_str node;
  
  struct map_str {
    hash_func hash;
    equals_func key_equals;
    int M, size;
    node ** hashTable;
  };
  
  /* 
     Creates the memory for map struct, also creates
     the memory for the array of linked list for our table with M number
     of buckets. Stores the hash function and the comparision function inside
     the map. Finally intialize all buckets to NULL, to mark that all possible
     linked list are empty */
  map * map_create(int M, hash_func hf, equals_func eq) 
  {
    map * m = (map *) malloc(sizeof(struct map_str));
    m->M = M;
    m->hash = hf;
    m->key_equals = eq;
    m->size = 0;
    m->hashTable = (node **) malloc(M * sizeof(node *));


    //using calloc would do this in one step? the answer is yes
    for(int i = 0; i < M ; i ++) 
      m->hashTable[i] = NULL;
    return m;
  }
  
  /* returns map size */
  int map_size (map * m) 
  {
    return m->size;
  }

  int map_capacity (map * m) 
  {
    return m->M;
  }
  
  /* creates node and stores data in it */
  node * createNode(void * key, void * value) 
  {
    node * n = (node *) malloc(sizeof(struct node_str));
    n->key = key;
    n->value = value;
    n->next = NULL;
    return n;
  }
  
  /* Stores a key-value pair relationship in them map
    1) for a given key, generate hash code
    2) do mod M on hash code to make sure it fits in the table
    3) Traverse that linked list to insert our update our element
       3.1) if not found, insert on the list
  */
  void map_put(map * m, void * key, void * value)
  {
    /* get bucket number to insert */
    int hashcode = m->hash(key);
    int bucket = (hashcode % m->M + m->M) % m->M;
    // printf("map_put: hashing key = %d\n", hashcode);
    // printf("map_put: bucket = hash mod %d = %d\n", m->M, bucket);
   
    node * n = m->hashTable[bucket];
    //printf("map_put:start, looking in bucket %d\n", bucket);

    if(m->hashTable[bucket] == NULL) 
    {
      /* case: the bucket is empty, no elements in the list */
      //printf("map_put: List is NULL, creating first node at bucket %d\n", bucket);
      m->hashTable[bucket] = createNode(key, value);
      m->size ++;
      return;
    } 
  
    /* traverse linked list */
    while(n != NULL) 
    {
      /* for every element, review if its the one we are trying to insert
         or replace, using the equals function */
      if(m->key_equals(key, n->key))
      { 
        /* Update value (overwrites) and return */
        //printf("map_put: Found, update!\n");
        n->value = value;
        return;
      }
      else
      {
        /* not in this node? continue with the next */
        //printf("map_put: Not here, keep searching\n");
        n = n->next;
      }
    }

    /* if we reach this point we have traversed the whole bucket list without
      finding our key, so lets insert a new one, lets put it in the header
      of the list */

    //printf("map_put:key not found create new node\n");
    node * new = createNode(key, value);
    new->next = m->hashTable[bucket];
    m->hashTable[bucket] = new;
    m->size ++;
    
  }

  bool map_remove(map* m, void* key)
  {
      if (!m || !key)
          return false;

      int hashcode = m->hash(key);
      int bucket = (hashcode % m->M + m->M) % m->M;

      node* curr = m->hashTable[bucket];
      node* prev = NULL;

      while (curr != NULL)
      {
          if (m->key_equals(curr->key, key))
          {
              // si es el primer nodo del bucket
              if (prev == NULL)
              {
                  m->hashTable[bucket] = curr->next;
              }
              else
              {
                  prev->next = curr->next;
              }

              free(curr);
              m->size--;

              return true;
          }

          prev = curr;
          curr = curr->next;
      }

      return false; 
  }
  
  /* returns TRUE if map contains the key */
  bool map_contains(map *m, void * key) 
  {
    int bucket = (m->hash(key) % m->M + m->M) % m->M;
    node * n = m->hashTable[bucket];
    while(n != NULL) 
    {
      if(m->key_equals(n->key, key)) 
        return true;
      else
        n = n->next;
    }
    return false;
  }
  
  void * map_get(map *m, void *key) 
  {
    /* 
       Searches the key in the map, if found returns the data it maps to
       if not found returns a void NULL pointer
       
       we need to determine in which of the many linked list we need to search,
       so compute bucket number just as we did with insert map_put*/
    
    int bucket = (m->hash(key) % m->M + m->M) % m->M;
    node * n = m->hashTable[bucket];

    /* traverse all list until a node with equal key is found.
    if node exists, return node -> value, otherwise return NULL */
    while(n != NULL) 
    {
      if(m->key_equals(n->key, key)) 
        break;
      else
        n = n->next;
    }
    return n != NULL ? n->value : NULL;
  }


void map_destroy(map * m) 
{
  /* free all nodes in all buckets */
  for(int i = 0; i < m->M ; i ++) 
  {
    node * n = m->hashTable[i];
    while(n != NULL) 
    {
      node * temp = n;
      n = n->next;
      free(temp);
    }
  }
  free(m->hashTable);
  free(m);
}


void map_print(map* m, print_func printKey, print_func printValue) 
{
    if (!m) {
        printf("Map is NULL\n");
        return;
    }

    printf("Map {size=%d, capacity=%d}\n", m->size, m->M);

    for (int i = 0; i < m->M; i++) 
    {
        node* n = m->hashTable[i];

        if (!n) continue;  // bucket vacío

        printf("  Bucket %d:\n", i);

        while (n != NULL) 
        {
            printf("    [");
            printKey(n->key);
            printf("] -> ");
            printValue(n->value);
            printf("\n");

            n = n->next;
        }
    }
}

  
//map iterator
struct map_iterator_str {
  map * m;
  int bucketIndex;
  node * currentNode;
};

// posiciona en el primer nodo válido o currentNode = null si no hay
map_iterator * map_iter_create(map * m) 
{
  map_iterator * it = malloc(sizeof(map_iterator));
  it->m = m;
  it->bucketIndex = 0;
  it->currentNode = NULL;

  // find first non-empty bucket
  while (it->bucketIndex < it->m->M && it->m->hashTable[it->bucketIndex] == NULL) 
  {
    it->bucketIndex++;
  }

  if (it->bucketIndex < it->m->M) 
  {
    it->currentNode = it->m->hashTable[it->bucketIndex];
  } 
  else 
  {
    it->currentNode = NULL;
  }

  return it;
}

// devuelve true si hay un elemento disponible ahora o en algún bucket futuro
bool map_iter_has_next(map_iterator * it) 
{
  return it->currentNode != NULL;
}

// devuelve la key actual y avanza el iterador (o null si no hay mas)
void * map_iter_next(map_iterator * it) 
{
  if (!it->currentNode)
  { 
    return NULL;
  }

  void * result = it->currentNode->key;

  // avanzar en la lista actual
  if (it->currentNode->next != NULL) 
  {
    it->currentNode = it->currentNode->next;
    return result;
  }

  // sino buscar el siguiente bucket no vacio
  int idx = it->bucketIndex + 1;
  it->currentNode = NULL;

  while (idx < it->m->M) 
  {
    if (it->m->hashTable[idx] != NULL) 
    {
      it->bucketIndex = idx;
      it->currentNode = it->m->hashTable[idx];
      break;
    }
    idx++;
  }

  return result;
}

void map_iter_destroy(map_iterator * it) 
{
  free(it);
}
