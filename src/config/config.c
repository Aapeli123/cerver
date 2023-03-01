#include "config.h"

static void parse_config_line(char* line) {
    char* command = strtok(line, " ");
    if(strcmp(command, "loc") == 0) {
        // Configure page(s) for a location
        char* path = strtok(NULL, " ");
        char* location = strtok(NULL, " ");
        command_location(path, location);
    } else if(strcmp(command, "port") == 0) {
        // Set the server port
        char* port = strtok(NULL, " ");
        command_port(port);
    } else if(strcmp(command, "header") == 0) {
        // Set http header
        char* key = strtok(NULL, " ");
        char* val = strtok(NULL, " ");
        command_header(key, val);
    } else if(strcmp(command, "fallback") == 0) {
        // Fallback page (Error 404)
        char* errpage = strtok(NULL, " ");
        command_fallback(errpage);
    } else if(strcmp(command, "root") == 0) {
        // Set the root dir where to look for pages
        char* rootpage = strtok(NULL, " ");
        command_root(rootpage);
    } else if(strcmp(command, "include") == 0) {
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
    printf("Reading config file %s\n", path);
    char* buffer = calloc(1, sizeof(char)*1024);   
    FILE* file = fopen(path, "r");
    while(fgets(buffer, 1024 - 1, file) != NULL) {
        // Ignore newlines
        if(!strcmp(buffer, "\n"))
            continue;
        // Ignore comments
        if(!strncmp(buffer, "#", 1))
            continue;

        printf("Parsing line: %s",buffer);
        parse_config_line(buffer);
    }

    printf("\nConfig reading done...\n");

    free(buffer);
    fclose(file);
}



