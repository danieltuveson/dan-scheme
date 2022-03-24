#ifndef LINKED_LIST
#define LINKED_LIST

typedef struct 
{
    struct LinkedList *prev;
    struct LinkedList *next;
} LinkedList;


#include "linked_list.c.generated.h"
#endif
