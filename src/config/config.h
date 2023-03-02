#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../hashmap/hashmap.h"

#include "commands.h"

struct Config
{
    int port;
    int threads;
    char* root_dir;
    char* fallback_page;
    hashmap_t* route_map;
    hashmap_t* header_map;
    bool ssl;
};

typedef struct Config config_t;


void config_read(const char* path);
#endif