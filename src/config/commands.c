#include "commands.h"

static char* getAbsPath(char* root, char* path) {
    char* abs_path = calloc(strlen(root) * strlen(path) + 1, sizeof(char));
    strcat(abs_path, root);
    strcat(abs_path, path);
    return abs_path;
}

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
        char* locpath = strtok(location, "\n");
        char* abs_path = calloc(sizeof(char),(strlen(locpath) + strlen(config->root_dir)) + 1);
        strcat(abs_path, config->root_dir);
        strcat(abs_path, locpath);
        char* contents = read_file_to_memory(abs_path);
        HASHMAP_ADD_STRING(config->route_map, path, contents);
        //hashmap_add(config->route_map, path, contents, strlen(contents) + 1);
        free(contents);
        free(abs_path);
        return;
    }
    if(!path_has_wildcard && location_has_wildcard) {
        printf("Arguments to loc %s %s don't make any sense!\n", path, location);
        return;
    }
    if(path_has_wildcard && location_has_wildcard) {
        char* path_prefix = strtok(path, "*");

        char* dir_path = calloc(strlen(path_prefix) + strlen(config->root_dir) + 1, sizeof(char));
        strcat(dir_path, config->root_dir);
        strcat(dir_path, path_prefix);
        dir_t* dir = read_directory(dir_path, path_has_double_wildcard);
        for (int i = 0; i < dir->files; i++) {
            char* file = dir->file_list[i];
            char* contents = read_file_to_memory(file);
            if(location_has_double_wildcard) {
                char* filepath_relative = &file[strlen(config->root_dir)];
                HASHMAP_ADD_STRING(config->route_map, filepath_relative, contents);
                // hashmap_add(config->route_map, filepath_relative, contents);
                free(contents);
                continue;
            }
            char* filename = getFileName(file);
            printf("%s\n", file);
            int len = (int)strlen(filename) + (int)strlen(path_prefix) + 1;
            char data[len];
            memset(data, 0, len);
            sprintf(data, "%s%s", path_prefix, filename);
            HASHMAP_ADD_STRING(config->route_map, data, contents);
            // hashmap_add(config->route_map, data, contents);
            free(contents);
        }
        destroy_directory(dir);
        free(dir_path);
    }

}

void wildcard_redirect_parser(char* path, char* location, config_t* config) {
    // TODO
}

void command_location(char* path, char* location, config_t* config) {
    if(strchr(path, '*') != NULL || strchr(location, '*') != NULL) {
        wildcard_location_parser(path, location, config);
        return;
    }
    char file_path_abs[strlen(config->root_dir) + strlen(location) + 1];
    memset(file_path_abs, 0, strlen(config->root_dir) + strlen(location) + 1);
    memcpy(file_path_abs, config->root_dir, strlen(config->root_dir));
    memcpy(&(file_path_abs[strlen(config->root_dir)]), location, strlen(location) + 1);
    char* contents = read_file_to_memory(file_path_abs);
    HASHMAP_ADD_STRING(config->route_map, path, contents);
    // hashmap_add(config->route_map, path, contents);
    free(contents);
}

void command_redirect(char* path, char* to, config_t* config) {
    if(strchr(path, '*') != NULL || strchr(to, '*') != NULL) {
        wildcard_redirect_parser(path, to, config);
        return;
    }
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
    config->root_dir = calloc((strlen(root) + 1),sizeof(char));
    strncpy(config->root_dir, root, strlen(root ));
}

void command_fallback(char* fallback_file_path, config_t* config) {
    char* fp = strtok(fallback_file_path,"\n");
    char* abspath = getAbsPath(config->root_dir, fp);
    char* fallback_content = read_file_to_memory(abspath);
    free(abspath);
    config->fallback_page = fallback_content;
}

void command_threads(char* thread_count, config_t* config) {
    int threads = (int)strtol(thread_count, NULL, 10);
    config->threads = threads;
}

void command_ssl(config_t* config) {
   config->ssl = true;
}

void command_keys(char* pubkey, char* privkey,config_t* config) {
    config->pubkey = malloc(strlen(pubkey) + 1);
    strcpy(config->pubkey, pubkey);

    config->privkey = malloc(strlen(privkey) + 1);
    strcpy(config->privkey, privkey);
}