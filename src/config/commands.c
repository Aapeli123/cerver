#include "commands.h"



void command_location(char* path, char* location, config_t* config) {

}

void command_port(char* port, config_t* config) {
    int port_i = (int)strtol(port, NULL, 10);
    config->port = port_i;
}

void command_header(char* header_key, char* header_value, config_t* config) {

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