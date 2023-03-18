#ifndef DIR_H
#define DIR_H

#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


// Define path max to be something so this compiles on alpine with musl
#ifndef PATH_MAX
#define PATH_MAX 4086
#endif

struct Directory {
    int files;
    char** file_list;
};

typedef struct Directory dir_t;

dir_t *read_directory(char * path, bool recurse);
void destroy_directory(dir_t* dir);

#endif