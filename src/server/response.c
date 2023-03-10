#include "response.h"
int write_response(int client_fd, char* response, int response_bytes){
    return send(client_fd, response, response_bytes, 0);
}
