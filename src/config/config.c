#include "config.h"

static void parse_config_line(char* line) {

}

void config_read(const char* path) {
    char* buffer = calloc(1, sizeof(char)*1024);   
    FILE* file = fopen(path, "r");
    while(fgets(buffer, 1024 - 1, file) != NULL) {
        parse_config_line(buffer);
    }
    free(buffer);
}

