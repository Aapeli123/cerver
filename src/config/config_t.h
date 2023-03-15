//
// Created by aapo on 3/9/23.
//

#ifndef CONFIG_T_H
#define CONFIG_T_H
#include "../hashmap/hashmap.h"
#include "../http/http.h"

struct Config
{
    int port;
    int threads;
    char* root_dir;
    char* fallback_page;
    hashmap_t* route_map;
    struct http_header** headers;
    int header_count;
    bool ssl;
    char* pubkey;
    char* privkey;
};

typedef struct Config config_t;
config_t* config_create();
void config_destroy(config_t* c);


#endif
