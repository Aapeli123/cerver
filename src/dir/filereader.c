#include "filereader.h"


// Read file to buffer and return the buffer
char* read_file_to_memory(char* filepath) {
    FILE* fp = fopen(filepath, "r");
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    rewind(fp);
    char* buffer = calloc(size + 1,  sizeof(char ));
    int n = fread(buffer, sizeof(char), size, fp);
    if(n != size) {
        free(buffer);
        return NULL;
    }
    return buffer;
}