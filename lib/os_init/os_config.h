// Header guard.
#ifndef LIB_OS_CONFIG_
#define LIB_OS_CONFIG_


// Headers.
#include "os_metadata.h"


// Definitions.
#define STRING_SIZE (size_t) 128
#define MAX_OPERATIONS (size_t) 128


// Log destination.
typedef enum { TO_BOTH, TO_FILE, TO_DISPLAY } destination;


// OS config.
typedef struct
{
	float version;
	char metadata_file_path[STRING_SIZE];
	char log_file_path[STRING_SIZE];
	destination log_dest;
	unsigned int processor_period_ms;
	unsigned int memory_period_ms;
	unsigned int hdd_period_ms;
	unsigned int monitor_period_ms;
	unsigned int speaker_period_ms;
	unsigned int printer_period_ms;
	unsigned int keyboard_period_ms;
	unsigned int mouse_period_ms;
	os_metadata metadata[MAX_OPERATIONS];
	int num_metadata;
} os_config;


// End header guard.
#endif