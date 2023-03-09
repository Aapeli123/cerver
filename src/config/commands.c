#include "commands.h"

static char* getFileName(char* filepath) {
    char* token = strtok(filepath, "/");
    char* prev = NULL;
    while(token != NULL) {
        prev = token;
        token = strtok(NULL, "/");
    }
    return prev;
}

static void wildcard_location_parser(char* path, char* location, config_t* config) {
    // Path is the requested path and location is the location of the file to be served:
    bool path_has_wildcard = strchr(path, '*') != NULL;
    bool path_has_double_wildcard = strstr(path, "**") != NULL;

    bool location_has_wildcard = strchr(location, '*') != NULL;
    bool location_has_double_wildcard = strstr(location, "**") != NULL;

    if(path_has_wildcard && !location_has_wildcard) {
        char* contents = read_file_to_memory(path);
        hashmap_add(config->route_map, path, contents);
        free(contents);
        return;
    }
    if(!path_has_wildcard && location_has_wildcard) {
        printf("Arguments to loc %s %s don't make any sense!\n", path, location);
        return;
    }
    if(path_has_wildcard && location_has_wildcard) {
        char* path_prefix = strtok(path, "*");
        char* loc_prefix = strtok(location, "*");


        char* dir_path = calloc(strlen(path_prefix) + strlen(config->root_dir) + 1, sizeof(char));
        strcat(dir_path, config->root_dir);
        strcat(dir_path, path_prefix);
        dir_t* dir = read_directory(dir_path, path_has_double_wildcard);
        for (int i = 0; i < dir->files; i++) {
            char* file = dir->file_list[i];
            char* contents = read_file_to_memory(file);
            if(location_has_double_wildcard) {
                char* filepath_relative = &file[strlen(config->root_dir)];
                hashmap_add(config->route_map, filepath_relative, contents);
                free(contents);
                continue;
            }
            char* filename = getFileName(file);
            printf("%s\n", file);
            int len = (int)strlen(filename) + (int)strlen(path_prefix) + 1;
            char data[len];
            memset(data, 0, len);
            sprintf(data, "%s%s", path_prefix, filename);
            hashmap_add(config->route_map, data, contents);
            free(contents);
        }
        destroy_directory(dir);
        free(dir_path);
    }

}

void command_location(char* path, char* location, config_t* config) {
    if(strchr(path, '*') != NULL || strchr(location, '*') != NULL) {
        wildcard_location_parser(path, location, config);
        return;
    }
    char file_path_abs[strlen(config->root_dir) + strlen(location)];
    memset(file_path_abs, 0, strlen(config->root_dir) + strlen(location));
    memcpy(file_path_abs, config->root_dir, strlen(config->root_dir));
    memcpy(&(file_path_abs[strlen(config->root_dir)]), location, strlen(location) - 1);
    char* contents = read_file_to_memory(file_path_abs);
    hashmap_add(config->route_map, path, contents);
    free(contents);
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
    config->root_dir = malloc((strlen(root) )*sizeof(char) - 1);
    strncpy(config->root_dir, root, strlen(root ) - 1);
}

void command_fallback(char* fallback_file_path, config_t* config) {
    config->fallback_page = malloc((strlen(fallback_file_path) + 1 )*sizeof(char));
    strcpy(config->fallback_page, fallback_file_path);
}

void command_threads(char* thread_count, config_t* config) {
    int threads = (int)strtol(thread_count, NULL, 10);
    config->threads = threads;
}