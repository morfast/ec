#include<stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "ec.h"


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
static double es_eps;
static prime_list[] = 
    {1009, 2003, 5003, 10007, 20011, 50021, 100003, -1};

void print_D(struct tuple **D);

uint32_t get_key(struct tuple *e, uint32_t size)
{
    return e->elem % size;
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

    i = 0;
    while (prime_list[i] > 0) {
        if (prime_list[i] > size) {
            size = prime_list[i];
            break;
        }
        i++;
    }

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

void delete_f0_tuples(struct tuple **D, uint32_t size)
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

void query(struct tuple **D, uint32_t size, double s, double eps, 
                                uint32_t *resip, uint32_t *ressize)
{
	int i;
	struct tuple *t;
    *ressize = 0;

	for (i = 0; i < size; i++) {
		t = D[i];
		while (t != NULL) {
			if ((t->f + t->df) > (s - eps)*N) {
                *resip = t->elem;
                resip++;
                (*ressize)++;
			}
            t = t->next;
		}
	}
}

uint32_t init_es(double e)
{
    es_eps = e;
    Dsize = init(es_eps);
    return Dsize;
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

void query_es(double s, uint32_t *result_ipaddrs, uint32_t *n_ip)
{
    printf("query with s=%lf, eps=%lf\n", s, es_eps);
    query(D, Dsize, s, es_eps, result_ipaddrs, n_ip);
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
    int i,j;
    uint32_t ip_addr;
    uint32_t *result_ips, n_resip;
    uint32_t store_size;

    double s, eps;
    eps = 0.0001;
    store_size = init_es(eps);
    srand((unsigned)time(NULL));  

    FILE * p = fopen("data","w");

	j = 0;
	while (j++ < 400) {
    	for (i = 0; i < 1000; i++) {
    	    ip_addr = (uint32_t)rand() % 10000;
    	    fprintf(p, "%d\n", ip_addr);
    	    add_site(ip_addr);
    	}
    	for (i = 0; i < 9000; i++) {
    	    ip_addr = (uint32_t)rand() % 30 + 10000;
    	    fprintf(p, "%d\n", ip_addr);
    	    add_site(ip_addr);
    	}
	}

	print_D(D);

    result_ips = (uint32_t *)malloc(sizeof(uint32_t) * store_size);
    s = 0.028;
    query_es(s, result_ips, &n_resip);

    for(i = 0; i < n_resip; i++) {
        printf("%d\n", result_ips[i]);
    }

    return 0;
}
