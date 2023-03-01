#ifndef COMMANDS_H
#define COMMANDS_H

// Set location for a path
void command_location(char* path, char* location);
// Set the server port
void command_port(char* port);
// Set a http header for all requests
void command_header(char* header_key, char* header_value);
// Set the root folder where to look for files
void command_root(char* root);
// Set the fallback page (Error 404)
void command_fallback(char* fallback_file_path);

#endif