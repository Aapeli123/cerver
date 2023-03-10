#include "mime.h"

hashmap_t* CONTENT_TYPES = NULL;

#define ADD_TYPE(filetype, content_type) HASHMAP_ADD_STRING(CONTENT_TYPES, filetype, content_type)
void mime_table_init() {
    CONTENT_TYPES = hashmap_create();
    ADD_TYPE("html","text/html");
    ADD_TYPE("js","text/javascript");
    ADD_TYPE("css","text/css");
    ADD_TYPE("txt","text/plain");
    ADD_TYPE("json","application/json");
    ADD_TYPE("jpg","image/jpeg");
    ADD_TYPE("png","image/png");
    ADD_TYPE("pdf","application/pdf");
}
#undef ADD_TYPE

char* get_content_type(char* filetype) {
    char* mime = hashmap_get(CONTENT_TYPES, filetype);
    if(mime == NULL)
        mime = DEFAULT_MIME;
    return mime;
}

char* mime_for_filename(char* filename) {
    strtok(filename, ".");
    char* fileEnd = strtok(NULL, ".");
    if(fileEnd == NULL)
        return "text/html";
    return get_content_type(fileEnd);
}

void mime_table_destroy() {
    if(CONTENT_TYPES != NULL)
        hashmap_destroy(CONTENT_TYPES);
}
