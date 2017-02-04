// Header guard.
#ifndef LIB_LOGGER_H_
#define LIB_LOGGER_H_


// Headers.
#include <stdio.h>
#include "./tokens/config_tokens.h"
#include "./tokens/metadata_tokens.h"
#include "../bool/bool.h"
#include "../os_init/os_config.h"


// Function prototypes.
bool log_config(os_config*);


// End header guard.
#endif