#ifndef MIME_H
#define MIME_H

#include "../hashmap/hashmap.h"


extern hashmap_t* CONTENT_TYPES;

void mime_table_init();
char* get_content_type(char* filetype);
void mime_table_destroy();


#endif