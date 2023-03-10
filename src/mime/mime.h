#ifndef MIME_H
#define MIME_H

#include "../hashmap/hashmap.h"

#define DEFAULT_MIME "text/plain"

extern hashmap_t* CONTENT_TYPES;

void mime_table_init();
char* get_content_type(char* filetype);
char* mime_for_filename(char* filename);
void mime_table_destroy();


#endif