#include "config.h"

static void parse_config_line(char* line) {
    char* command = strtok(line, " ");
    if(!strcmp(command, "loc")) {
        // Configure page(s) for a location
    } else if(!strcmp(command, "port")) {
        // Set the server port
    } else if(!strcmp(command, "header")) {
        // Set http header
    } else if(!strcmp(command, "fallback")) {
        // Fallback page (Error 404)
    } else if(!strcmp(command, "root")) {
        // Set the root dir where to look for pages
    } else if(!strcmp(command, "include")) {
        // Include another config file
    } else {
        printf("Failed to parse line: %s. Unknown command.\n", line);
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

