#include<stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


struct tuple
{
    uint32_t elem;
    uint64_t f;
    uint64_t ne;
    uint64_t df;
    struct tuple *next;
};

static uint64_t N;
static uint32_t Dsize;
static uint32_t Dcount;
static struct tuple **D;
static double es_s, es_eps;

/* check if e exist in D
   yes : return 1; 
   no: return 0 
*/
void print_D(struct tuple **D);

uint32_t get_key(struct tuple *e, uint32_t size)
{
    return e->elem % size;
}

//void append_list(struct tuple *list, struct tuple *e)
//{
//    while (list != NULL) {
//        list = list->next;
//    }
//    list = e;
//}

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


int check_and_add(struct tuple *D[], uint32_t size,  struct tuple *e)
{
    struct tuple *list;
    uint32_t key;

    key = get_key(e, size);
    list = D[key];
    while (list != NULL) {
        if (list->elem == e->elem) {
            list->f++;
            return 1;
        }
        list = list->next;
    }
    return 0;
}

void add_set(struct tuple *D[], uint32_t size, struct tuple *e)
{
    struct tuple *list;
    uint32_t key;

    key = get_key(e, size);
    if (D[key] == NULL) {
        D[key] = e;
    } else {
        list = D[key];
        while (list->next != NULL) {
            list = list->next;
        }
        list->next = e;
    }
}

uint32_t init(double eps)
{ 
    uint32_t size, i;

    size = (uint32_t)1.0/eps;
    D = (struct tuple **)malloc(sizeof(struct tuple *) * size);
    if (D == NULL) return 0;

    for(i = 0; i < size; i++) {
        D[i] = NULL;
    }

    Dsize = size;
    Dcount = 0;

    return size;
    
}

int notexsitf0(struct tuple **D, uint32_t size)
{
    uint32_t i;
    struct tuple *t;

    for (i = 0; i < size; i++) {
        t = D[i];
        while (t != NULL) {
            if (t->f == 0) {
                return 0;
            }
            t = t->next;
        }
    }

    return 1;
}

void decreasef(struct tuple **D, uint32_t size)
{
    uint32_t i;
    struct tuple *t;

    for (i = 0; i < size; i++) {
		t = D[i];
		while (t != NULL) {
            if (t->f < 0) {
                printf("aaaaa\n");
                exit(1);
            }
		    t->f--;
		    t->df++;
            t = t->next;
		}
    }
}

int delete_f0_tuples(struct tuple **D, uint32_t size)
{ 
    uint32_t i;
    struct tuple *t;
    struct tuple *tmp;

    for (i = 0; i < size; i++) {
		//head node
		while ((D[i] != NULL) && (D[i]->f == 0)) {
			tmp = D[i];
			D[i] = D[i]->next;
			free(tmp);
            Dcount--;
		}
			
		t = D[i];
	    while ((t != NULL) && (t->next != NULL)) {
			if (t->next->f == 0) {
				tmp = t->next;
				t->next = t->next->next;
				free(tmp);
                Dcount--;
			}
            t = t->next;
	    }
    }
		
}


void ecount(struct tuple **D, uint32_t size, struct tuple *e)
{
    N++;

    if (check_and_add(D, size, e)) {
        Dcount++;
        return;
    } else {
        /* D is not full */
        if (Dcount <= size) {
            /* add e to D */
            e->ne = N;
            add_set(D, size, e);
            Dcount++;
        } else {
			while (notexsitf0(D, size)) {
				decreasef(D, size);
			}
			delete_f0_tuples(D, size);
            add_set(D, size, e);
            Dcount++;
        }
    }
}

int query(struct tuple **D, uint32_t size, double s, double eps)
{
	int i;
	struct tuple *t;

	for (i = 0; i < size; i++) {
		t = D[i];
		while (t != NULL) {
			if ((t->f + t->df) > (s - eps)*N) {
				/* output t */
				printf("%u, %u\n", t->elem, t->f);
			}
            t = t->next;
		}
	}
}

void init_es(double e)
{
    es_eps = e;
    Dsize = init(es_eps);
}

int add_site(uint32_t ipaddr)
{
    uint32_t size;
    struct tuple *e;

    size = Dsize;

    e = (struct tuple *)malloc(sizeof(struct tuple));
    if (e == NULL) {
        return -1;
    }
    e->elem = ipaddr;
    e->f = 1;
    e->df = 0;
    e->next = NULL;
    e->ne = 0;
    
    ecount(D, size, e);

    return 0;
}

void query_es(double s)
{
    printf("query with s=%lf, eps=%lf\n", s, es_eps);
    query(D, Dsize, s, es_eps);
}

void print_D(struct tuple **D)
{
    uint32_t i;
    struct tuple *list;

    for (i = 0; i < Dsize; i++) {
        list = D[i];
        if (list == NULL) {
            continue;
        }
        printf("= %d =  ", i);
        while(list) {
            printf("%d %d, ", list->elem, list->f);
            list = list->next;
        }
        printf("\n");
    }
}


int main()
{
    uint32_t ip;
    int i;

    ip = 1;
    double s, eps;
    eps = 0.001;
    init_es(eps);
    srand((unsigned)time(NULL));  

    for (i = 0; i < 100; i++) {
        add_site((uint32_t)rand() % 100);
    }
    for (i = 0; i < 700; i++) {
        //printf("========================\n");
        //print_D(D);
        add_site(1000 + ((uint32_t)rand() % 10));
    }
    //for (i = 0; i < 80; i++) {
    //    add_site((uint32_t)(i % 100));
    //}
    //for (i = 0; i < 80; i++) {
    //    add_site((uint32_t)(i*i % 100));
    //}
    //for (i = 0; i < 80; i++) {
    //    add_site((uint32_t)(i*3 % 100));
    //}

    print_D(D);

    s = 0.05;
    query_es(s);

    return 0;
}
