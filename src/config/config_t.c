//
// Created by aapo on 3/9/23.
//

#include "config_t.h"

config_t *config_create() {
    config_t* c = calloc(1, sizeof(config_t));
    hashmap_t* route_map = hashmap_create();
    hashmap_t* header_map = hashmap_create();
    c->route_map = route_map;
    c->header_map  = header_map;
    return c;
}

void config_destroy(config_t* c) {
    hashmap_destroy(c->header_map);
    hashmap_destroy(c->route_map);
    free(c);
}
