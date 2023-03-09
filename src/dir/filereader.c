#include "filereader.h"


// Read file to buffer and return the buffer
char* read_file_to_memory(char* filepath) {
    FILE* fp = fopen(filepath, "r");
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    rewind(fp);
    char* buffer = calloc(1, size * sizeof(char ));
    fread(buffer, size, 1, fp);
    return buffer;
}