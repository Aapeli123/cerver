#include "commands.h"

static void wildcard_location_parser(char* path, char* location, config_t* config) {
    // TODO Either the path or location string has wildcard and needs to be processed further

}

void command_location(char* path, char* location, config_t* config) {
    if(strchr(path, '*') != NULL || strchr(location, '*') != NULL) {
        wildcard_location_parser(path, location, config);
        return;
    }
    hashmap_add(config->route_map, path, "TODO");
}

void command_port(char* port, config_t* config) {
    int port_i = (int)strtol(port, NULL, 10);
    config->port = port_i;
}

void command_header(char* header_key, char* header_value, config_t* config) {
    if(config->headers == NULL) {
        config->headers = malloc(1 * sizeof (struct http_header*));
    } else {
        config->headers = realloc(config->headers, (config->header_count + 1)*sizeof(struct http_header*));
    }
    struct http_header *header =  calloc(1, sizeof (struct http_header));

    strcpy(header->key, header_key);
    strcpy(header->value, header_value);
    config->headers[config->header_count] = header;
    config->header_count += 1;
}


void command_root(char* root, config_t* config) {
    config->root_dir = malloc((strlen(root) + 1 )*sizeof(char));
    strcpy(config->root_dir, root);
}

void command_fallback(char* fallback_file_path, config_t* config) {
    config->fallback_page = malloc((strlen(fallback_file_path) + 1 )*sizeof(char));
    strcpy(config->root_dir, fallback_file_path);
}

void command_threads(char* thread_count, config_t* config) {
    int threads = (int)strtol(thread_count, NULL, 10);
    config->threads = threads;
}