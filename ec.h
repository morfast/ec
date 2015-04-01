#ifndef _H_EC_
#define _H_EC_

/* initialization.
   eps: input, the allowed estimation error
   return: the storage size */
uint32_t init_es(double e);

/* add an ip address to storage for the ES algorithm */
/* ipaddr: input, the ip address to add */
/* return: 0 on success, -1 on error */
int add_site(uint32_t ipaddr);

/* query for the most frequent ip addresses
   s: input, the ip addresses with a frequncy greater than s will be gathered
   result_ipaddrs: input/output, the result ip addresses. 
                   This array should be allocated before calling query_es
   n_ip: output, the number of the result ip addresses */
void query_es(double s, uint32_t *result_ipaddrs, uint32_t *n_ip);

#endif
