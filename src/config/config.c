#include "config.h"

static void parse_config_line(char* line) {
    char* command = strtok(line, " ");
    if(!strcmp(command, "loc")) {
        // Configure page(s) for a location
        char* path = strtok(NULL, " ");
        char* location = strtok(NULL, " ");
        command_location(path, location);
    } else if(!strcmp(command, "port")) {
        // Set the server port
        char* port = strtok(NULL, " ");
        command_port(port);
    } else if(!strcmp(command, "header")) {
        // Set http header
        char* key = strtok(NULL, " ");
        char* val = strtok(NULL, " ");
        command_header(key, val);
    } else if(!strcmp(command, "fallback")) {
        // Fallback page (Error 404)
        char* errpage = strtok(NULL, " ");
        command_fallback(errpage);
    } else if(!strcmp(command, "root")) {
        // Set the root dir where to look for pages
        char* rootpage = strtok(NULL, " ");
        command_root(rootpage);
    } else if(!strcmp(command, "include")) {
        // Include another config file
        char* path = strtok(NULL, " ");
        config_read(path); // Just recursively parse the file
    } else if(!strcmp(command, "ssl") || !strcmp(command, "tls")){
        // Use tls
        printf("SSL is not yet implemented");
        return;
    } else {
        printf("Failed to parse line: %s. Unknown command %s.\n", line, command);
        return;
    }
}

void config_read(const char* path) {
    char* buffer = calloc(1, sizeof(char)*1024);   
    FILE* file = fopen(path, "r");
    while(fgets(buffer, 1024 - 1, file) != NULL) {
        parse_config_line(buffer);
    }
    free(buffer);
    fclose(file);
}



