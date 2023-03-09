#ifndef DIR_H
#define DIR_H

#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
struct Directory {
    int files;
    char** file_list;
};

typedef struct Directory dir_t;

dir_t *read_directory(char * path, bool recurse);
void destroy_directory(dir_t* dir);

#endif