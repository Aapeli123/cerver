#include "dir.h"

static int count_dir_files(char* path, bool recurse) {
    int count = 0;
    DIR* directory = opendir(path);
    if(directory == NULL) {
        return -1;
    }
    struct dirent *entry;
    struct stat filestat;

    while((entry = readdir(directory))) {
        char* filepth = calloc(PATH_MAX, sizeof(char));
        sprintf(filepth,"%s/%s", path,entry->d_name);
        stat(filepth, &filestat);
        free(filepth);
        if(S_ISDIR(filestat.st_mode)) {
            if(!recurse) continue;
            if(!strcmp(".", entry->d_name) || !strcmp("..", entry->d_name)) continue;
            char* r_path = calloc(PATH_MAX, sizeof(char));
            sprintf(r_path,"%s/%s", path,entry->d_name);

            count += count_dir_files(r_path, true);
            free(r_path);
        } else {
            count++;
        }
    }
    closedir(directory);
    return count;
}




static void get_dir_files(char* path, bool recurse, char** files, int offset) {
    DIR* directory = opendir(path);
    if(directory == NULL) {
        return;
    }
    struct dirent *entry;
    struct stat filestat;
    int i = 0;
    int file_count = count_dir_files(path, false);
    while((entry = readdir(directory))) {
        char* filepth = calloc(PATH_MAX, sizeof(char));
        sprintf(filepth,"%s%s", path,entry->d_name);
        stat(filepth, &filestat);
        free(filepth);
        if(S_ISDIR(filestat.st_mode)) {
            if(!recurse) continue;
            if(!strcmp(".", entry->d_name) || !strcmp("..", entry->d_name)) continue;
            char* r_path = calloc(PATH_MAX, sizeof(char));
            sprintf(r_path,"%s%s/", path,entry->d_name);
            get_dir_files(r_path, true, files,offset + file_count);
            free(r_path);
        } else {
            char* dest = calloc(1,(strlen(entry->d_name))*sizeof(char) + strlen(path) *sizeof(char) + 1);
            sprintf(dest, "%s%s",path,entry->d_name);
            // strcpy(dest, entry->d_name);
            files[offset + i] = dest;
            i+=1;

        }
    }
    closedir(directory);
}

dir_t *read_directory(char * path, bool recurse) {
    dir_t* dir = calloc(1, sizeof (dir_t));
    int files = count_dir_files(path, recurse);
    if(files == -1) {
        return NULL;
    }
    char** file_list = calloc(files, sizeof (char*));
    dir->files= files;
    dir->file_list = file_list;

    get_dir_files(path, recurse, file_list, 0);

    return dir;
}


void destroy_directory(dir_t* dir) {
    for(int i = 0; i < dir->files; i++) {
        free(dir->file_list[i]);
    }
    free(dir->file_list);
    free(dir);
}