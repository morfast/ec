#ifndef _H_EC_
#define _H_EC_

uint32_t init_es(double e);
int add_site(uint32_t ipaddr);
void query_es(double s, uint32_t *result_ipaddrs, uint32_t *n_ip);

#endif
