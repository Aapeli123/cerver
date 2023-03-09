#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../hashmap/hashmap.h"
#include "config_t.h"
#include "commands.h"


void config_read(const char* path, config_t* config);
#endif