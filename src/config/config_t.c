//
// Created by aapo on 3/9/23.
//

#include "config_t.h"

config_t *config_create() {
    config_t* c = calloc(1, sizeof(config_t));
    hashmap_t* route_map = hashmap_create();
    hashmap_t* redirect_map = hashmap_create();
    // struct http_header* headers = NULL;
    // hashmap_t* header_map = hashmap_create();
    c->route_map = route_map;
    c->redirect_map = redirect_map;
    c->headers = NULL;
    c->threads = 0;
    c->root_dir = NULL;
    c->fallback_page = NULL;
    c->port = 8080;
    c->header_count = 0;
    c->pubkey = NULL;
    c->privkey = NULL;
    // c->header_map  = header_map;
    return c;
}

void config_destroy(config_t* c) {
    printf("Freeing config\n");
    // hashmap_destroy(c->header_map);
    for (int i = 0; i < c->header_count; ++i) {
        free(c->headers[i]->key);
        free(c->headers[i]->value);
        free(c->headers[i]);
    }
    free(c->headers);
    hashmap_destroy(c->route_map);
    hashmap_destroy(c->redirect_map);
    free(c->fallback_page);
    free(c->root_dir);
    free(c->pubkey);
    free(c->privkey);
    free(c);
}
