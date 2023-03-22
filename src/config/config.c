#include "config.h"

static void parse_config_line(char* line, config_t* config) {
    if(strchr(line, '\r') != NULL) {
        line = strtok(line, "\r\n");
    } else {
        line = strtok(line, "\n");
    }
    char* command = strtok(line, " ");
    if(!strcmp(command, "loc")) {
        // Configure page(s) for a location
        char* path = strtok(NULL, " ");
        char* location = strtok(NULL, " ");
        command_location(path, location, config);
    } else if(!strcmp(command, "redirect")){
        // Configure 300 redirect
        char* path = strtok(NULL, " ");
        char* to = strtok(NULL, " ");
        command_redirect(path, to, config);
    } else if(!strcmp(command, "port")) {
        // Set the server port
        char* port = strtok(NULL, " ");
        command_port(port, config);
    } else if(!strcmp(command, "header")) {
        // Set http header
        char* key = strtok(NULL, " ");
        char* val = strtok(NULL, " ");
        command_header(key, val, config);
    } else if(!strcmp(command, "fallback")) {
        // Fallback page (Error 404)
        char* errpage = strtok(NULL, " ");
        command_fallback(errpage, config);
    } else if(!strcmp(command, "root")) {
        // Set the root dir where to look for pages
        char* rootpage = strtok(NULL, " ");
        command_root(rootpage, config);
    } else if(!strcmp(command, "threads")) {
        char* threads = strtok(NULL, " ");
        command_threads(threads, config);
    } else if(!strcmp(command, "include")) {
        // Include another config file
        char* path = strtok(NULL, " ");
        config_read(path, config); // Just recursively parse the file
    }  else if(!strcmp(command, "ssl") || !strcmp(command, "tls")){
        // Use tls
        command_ssl(config);
        return;
    }  else if (!strcmp(command, "sslkeys")) {
        char* pub = strtok(NULL, " ");
        char* priv = strtok(NULL, " ");
        command_keys(pub, priv, config);
    }else {
        printf("Failed to parse line: %s. Unknown command %s.\n", line, command);
        return;
    }
}

void config_read(const char* path, config_t* config) {
    printf("\nReading config file %s\n\n", path);
    char* buffer = calloc(1, sizeof(char)*1024);   
    FILE* file = fopen(path, "r");
    while(fgets(buffer, 1024 - 1, file) != NULL) {
        // Ignore newlines
        if(!strcmp(buffer, "\n") || !strcmp(buffer, "\n\r"))
            continue;
        // Ignore comments
        if(!strncmp(buffer, "#", 1))
            continue;

        printf("Parsing line: %s",buffer);
        parse_config_line(buffer, config);
    }

    printf("\nConfig reading done...\n\n");

    free(buffer);
    fclose(file);
}