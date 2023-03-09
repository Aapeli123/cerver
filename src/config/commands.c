#include "commands.h"

static void wildcard_location_parser(char* path, char* location, config_t* config) {
    // Path is the requested path and location is the location of the file to be served:
    bool path_has_wildcard = strchr(path, '*') != NULL;
    bool path_has_double_wildcard = strstr(path, "**") != NULL;

    bool location_has_wildcard = strchr(location, '*') != NULL;
    bool location_has_double_wildcard = strstr(location, "**") != NULL;

    if(path_has_wildcard && !location_has_wildcard) {
        hashmap_add(config->route_map, path, "TODO");
        return;
    }
    if(!path_has_wildcard && location_has_wildcard) {
        printf("Arguments to loc %s %s don't make any sense!\n", path, location);
        return;
    }
    if(path_has_wildcard && location_has_wildcard) {
        // map the paths to match
    }

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
    header->key = calloc(1, sizeof (char)* strlen(header_key) + 1);
    header->value = calloc(1, sizeof (char)* strlen(header_value) + 1);
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
    strcpy(config->fallback_page, fallback_file_path);
}

void command_threads(char* thread_count, config_t* config) {
    int threads = (int)strtol(thread_count, NULL, 10);
    config->threads = threads;
}