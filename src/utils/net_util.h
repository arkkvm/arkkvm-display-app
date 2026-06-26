#ifndef NET_UTIL_H
#define NET_UTIL_H 

#include <stdbool.h>   

typedef enum {
    IPv6_TYPE_UNKNOWN = 0,
    IPv6_TYPE_GLOBAL,
    IPv6_TYPE_UNIQUE_LOCAL,
    IPv6_TYPE_LINK_LOCAL,
    IPv6_TYPE_LOOPBACK,
    IPv6_TYPE_UNSPECIFIED,
    IPv6_TYPE_MULTICAST
} IPv6AddressType;

// Address state enum
typedef enum {
    IPv6_STATE_UNKNOWN = -1,
    IPv6_STATE_PREFERRED = 0,
    IPv6_STATE_DEPRECATED = 1,
    IPv6_STATE_INVALID = 2
} IPv6AddressState;

typedef struct {
    char address[300];
    int valid_lifetime;
    int preferred_lifetime;
    int address_type;  // IPv6AddressType enum
    int flags;
    bool is_stable;
    int state;  // IPv6AddressState enum
    int scope;
} IPv6AddressInfo; 


char * handle_ipv6_report(const char * json_str);


#endif