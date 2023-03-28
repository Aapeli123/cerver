#ifndef COMMANDS_H
#define COMMANDS_H

#include "config_t.h"
#include "../dir/dir.h"
#include "../dir/filereader.h"
// Set location for a path
void command_location(char* path, char* location, config_t* config);
// Setup a http 301 redirect
void command_redirect(char* path, char* to, config_t* config);
// Set the server port
void command_port(char* port, config_t* config);
// Set a http header for all requests
void command_header(char* header_key, char* header_value, config_t* config);
// Set the root folder where to look for files
void command_root(char* root, config_t* config);
// Set the fallback page (Error 404)
void command_fallback(char* fallback_file_path, config_t* config);
// Set the threadpool thread count
void command_threads(char* thread_count, config_t* config);
// Enable ssl
void command_ssl(config_t* config);
// Set SSL keys:
void command_keys(char* pubkey, char* privkey, config_t* config);
// Page to serve for internal server errors
void command_errpage(char* page, config_t* config);
#endif