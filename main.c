#include<stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


struct tuple
{
    uint32_t elem;
    int f;
    int ne;
    int df;
    struct tuple *next;
};

uint32_t N;
struct tuple **D;

/* check if e exist in D
   yes : return 1; 
   no: return 0 
*/

uint32_t get_key(struct tuple *e, uint32_t size)
{
    return e->elem % size;
}

int search_list(struct tuple *list, struct tuple *e)
{
    while (list != NULL) {
        if (list->elem == e->elem) {
            return 1;
        }
        list = list->next;
    }
    return 0;
}

void append_list(struct tuple *list, struct tuple *e)
{
    while (list != NULL) {
        list = list->next;
    }
    list = e;
}

int in_set(struct tuple *D[], uint32_t size,  struct tuple *e)
{

    return search_list(D[get_key(e, size)], e);
}

void add_set(struct tuple *D[], uint32_t size, struct tuple *e)
{
    append_list(D[get_key(e, size)], e);
}

int delete_f0_tuples(struct tuple *D)
{}

int init(double eps)
{ 
    uint32_t size, i;

    size = (uint32_t)1/eps;
    D = (struct tuple **)malloc(sizeof(struct tuple *) * size);
    if (D == NULL) return -1;

    for(i = 0; i < size; i++) {
        D[i] = NULL;
    }

    return size;
    
}

int exsitf0(struct tuple **D, uint32_t size)
{
    uint32_t i;
    struct tuple *t;

    for (i = 0; i < size; i++) {
        t = D[i];
        while (t != NULL) {
            if (t->f == 0) {
                return 1;
            }
            t = t->next;
        }
    }

    return 0;
}


int ecount(struct tuple **D, uint32_t size, struct tuple *e)
{
    N++;

    if (in_set(D, size, e)) {
        e->f++;
    } else {
        /* D is not full */
        if (N <= size) {
            /* add e to D */
            e->ne = N;
            add_set(D, size, e);
        } else {

        }
    }
            


}

    


int main()
{
    return 0;
}
