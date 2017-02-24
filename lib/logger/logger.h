// Header guard.
#ifndef LIB_LOGGER_H_
#define LIB_LOGGER_H_


// Headers.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./tokens/config_tokens.h"
#include "./tokens/metadata_tokens.h"
#include "../bool/bool.h"
#include "../file_io/file_io.h"
#include "../os/config/os_config.h"


// Function prototypes.
bool log_config(os_config*);
bool log_metadata_begin_op(char*, prog_metadata*, double);
bool log_metadata_end_op(char*, prog_metadata*, double);


// End header guard.
#endif