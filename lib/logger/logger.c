// Header guard.
#ifndef LIB_LOGGER_C_
#define LIB_LOGGER_C_


// Headers.
#include "logger.h"


// Function prototypes.
bool log_os_to_file(os*, unsigned int*);
bool log_os_to_display(os*, unsigned int*);
bool log_metadata_begin_op_to_file(FILE*, prog_metadata*, double);
bool log_metadata_end_op_to_file(FILE*, os_config*, prog_metadata*, double);
bool log_metadata_begin_op_to_display(prog_metadata*, double);
bool log_metadata_end_op_to_display(os_config*, prog_metadata*, double);
bool compute_metadata_metrics(os*, unsigned int*);



// Log OS config.
bool log_os(os* os_ptr)
{
	// Variables.
	bool was_successful = false;
	unsigned int* cycles_per_metadata_ptr = NULL;


	// Get the log destination.
	switch (os_ptr->config.log_dest)
	{
		// Both?
		case TO_BOTH:
			// Cycles per metadata item.
			cycles_per_metadata_ptr = malloc(os_ptr->pcb.num_metadata * sizeof(unsigned int));


			// Compute metrics and log, store result.
			was_successful = compute_metadata_metrics(os_ptr, cycles_per_metadata_ptr) && log_os_to_file(os_ptr, cycles_per_metadata_ptr) && log_os_to_display(os_ptr, cycles_per_metadata_ptr);
			
			
			// Done.
			break;
		

		// Log to file.
		case TO_FILE:
			// Cycles per metadata item.
			cycles_per_metadata_ptr = malloc(os_ptr->pcb.num_metadata * sizeof(unsigned int));


			// Compute metrics and log, store result.
			was_successful = compute_metadata_metrics(os_ptr, cycles_per_metadata_ptr) && log_os_to_file(os_ptr, cycles_per_metadata_ptr);
			
			
			// Done.
			break;
		

		// Log to display.
		case TO_DISPLAY:
			// Cycles per metadata item.
			cycles_per_metadata_ptr = malloc(os_ptr->pcb.num_metadata);
			memset(cycles_per_metadata_ptr, 0, os_ptr->pcb.num_metadata);


			// Compute metrics and log, store result.
			was_successful = compute_metadata_metrics(os_ptr, cycles_per_metadata_ptr) && log_os_to_display(os_ptr, cycles_per_metadata_ptr);
			
			
			// Done.
			break;
		

		// Default.
		default:
			was_successful = false;
	}


	// Pointer?
	if (cycles_per_metadata_ptr)
	{
		// Free.
		free(cycles_per_metadata_ptr);
	}


	// Return.
	return was_successful;
}


// Log metadata begin operation.
bool log_metadata_begin_op(os_config* config_ptr, prog_metadata* metadata_ptr, double elapsed_time)
{
	// Declare stream pointer.
	FILE* stream_ptr;


	// Switch on log destination.
	switch (config_ptr->log_dest)
	{
		// Both?
		case TO_BOTH:
			// Get stream.
			stream_ptr = open_file(config_ptr->log_file_path, "w");


			// Log to both.
			return log_metadata_begin_op_to_display(metadata_ptr, elapsed_time) &&
				log_metadata_begin_op_to_file(stream_ptr, metadata_ptr, elapsed_time);
		

		// Log to file.
		case TO_FILE:
			// Get stream.
			stream_ptr = open_file(config_ptr->log_file_path, "w");


			// Log to file.
			return log_metadata_begin_op_to_file(stream_ptr, metadata_ptr, elapsed_time);
		

		// Log to display.
		case TO_DISPLAY:
			// Log to display.
			return log_metadata_begin_op_to_display(metadata_ptr, elapsed_time);
		

		// Default.
		default:
			// Abort.
			printf("\n\nInvalid log destination in OS config.\n\n");
			return false;
	}
}


// Log metadata end operation.
bool log_metadata_end_op(os_config* config_ptr, prog_metadata* metadata_ptr, double elapsed_time)
{
	// Declare stream pointer.
	FILE* stream_ptr;


	// Switch on log destination.
	switch (config_ptr->log_dest)
	{
		// Both?
		case TO_BOTH:
			// Get stream.
			stream_ptr = open_file(config_ptr->log_file_path, "w");


			// Log to both.
			return log_metadata_end_op_to_display(config_ptr, metadata_ptr, elapsed_time) &&
				log_metadata_end_op_to_file(stream_ptr,config_ptr, metadata_ptr, elapsed_time);
		

		// Log to file.
		case TO_FILE:
			// Get stream.
			stream_ptr = open_file(config_ptr->log_file_path, "w");


			// Log to file.
			return log_metadata_end_op_to_file(stream_ptr, config_ptr, metadata_ptr, elapsed_time);
		

		// Log to display.
		case TO_DISPLAY:
			// Log to display.
			return log_metadata_end_op_to_display(config_ptr, metadata_ptr, elapsed_time);
		

		// Default.
		default:
			// Abort.
			printf("\n\nInvalid log destination in OS config.\n\n");
			return false;
	}
}


// Log to file.
bool log_os_to_file(os* os_ptr, unsigned int* cycles_ptr)
{
	// Convenience.
	os_config* config_ptr = &os_ptr->config;


	// Get stream.
	FILE* file_ptr = open_file(config_ptr->log_file_path, "w");


	// File stream not obtained?
	if (!file_ptr)
	{
		// Abort.
		return false;
	}


	// Config.
	fprintf(file_ptr,
		"\n\n\
		%s\n\
		%s %u %s\n\
		%s %u %s\n\
		%s %u %s\n\
		%s %u %s\n\
		%s %u %s\n\
		%s %u %s\n\
		%s %u %s\n\
		%s %u %s\n\
		\n\n\
		%s",
		CONFIG_LOG_START,
		CONFIG_PROCESSOR_LABEL, config_ptr->processor_period_ms, CONFIG_CYCLE_UNITS,
		CONFIG_PROCESSOR_LABEL, config_ptr->memory_period_ms, CONFIG_CYCLE_UNITS,
		CONFIG_HDD_LABEL, config_ptr->hdd_period_ms, CONFIG_CYCLE_UNITS,
		CONFIG_KEYBOARD_LABEL, config_ptr->keyboard_period_ms, CONFIG_CYCLE_UNITS,
		CONFIG_MOUSE_LABEL, config_ptr->mouse_period_ms, CONFIG_CYCLE_UNITS,
		CONFIG_SPEAKER_LABEL, config_ptr->speaker_period_ms, CONFIG_CYCLE_UNITS,
		CONFIG_PRINTER_LABEL, config_ptr->printer_period_ms, CONFIG_CYCLE_UNITS,
		CONFIG_MEMORY_LABEL, config_ptr->memory_period_ms, CONFIG_CYCLE_UNITS,
		METADATA_LOG_START
	);


	// Variables.
	prog_metadata* metadata_ptr = os_ptr->pcb.metadata;
	unsigned int total = os_ptr->pcb.num_metadata;
	prog_metadata metadata;

	
	// Log metadata metrics.
	for (unsigned int i = 0; i < total; i++) 
	{
		// Save.
		metadata = metadata_ptr[i];


		// Display.
		fprintf(
			file_ptr,
			"\n%c(%s)%u - %u %s",
			get_code(&metadata), get_descriptor(&metadata), metadata.cycles,
			cycles_ptr[i], METADATA_CYCLE_UNITS
		);
	}


	// Success.
	return true;
}



// Log to display.
bool log_os_to_display(os* os_ptr, unsigned int* cycles_ptr)
{
	// Convenience.
	os_config* config_ptr = &os_ptr->config;


	// Config.
	printf(
		"\n\n\
		%s\n\
		%s %u %s\n\
		%s %u %s\n\
		%s %u %s\n\
		%s %u %s\n\
		%s %u %s\n\
		%s %u %s\n\
		%s %u %s\n\
		%s %u %s\n\
		\n\n\
		%s",
		CONFIG_LOG_START,
		CONFIG_PROCESSOR_LABEL, config_ptr->processor_period_ms, CONFIG_CYCLE_UNITS,
		CONFIG_PROCESSOR_LABEL, config_ptr->memory_period_ms, CONFIG_CYCLE_UNITS,
		CONFIG_HDD_LABEL, config_ptr->hdd_period_ms, CONFIG_CYCLE_UNITS,
		CONFIG_KEYBOARD_LABEL, config_ptr->keyboard_period_ms, CONFIG_CYCLE_UNITS,
		CONFIG_MOUSE_LABEL, config_ptr->mouse_period_ms, CONFIG_CYCLE_UNITS,
		CONFIG_SPEAKER_LABEL, config_ptr->speaker_period_ms, CONFIG_CYCLE_UNITS,
		CONFIG_PRINTER_LABEL, config_ptr->printer_period_ms, CONFIG_CYCLE_UNITS,
		CONFIG_MEMORY_LABEL, config_ptr->memory_period_ms, CONFIG_CYCLE_UNITS,
		METADATA_LOG_START
	);


	// Variables.
	prog_metadata* metadata_ptr = os_ptr->pcb.metadata;
	unsigned int total = os_ptr->pcb.num_metadata;
	prog_metadata metadata;

	
	// Log metadata metrics.
	for (unsigned int i = 0; i < total; i++) 
	{
		// Save.
		metadata = metadata_ptr[i];


		// Display.
		printf(
			"\n%c(%s)%u - %u %s",
			get_code(&metadata), get_descriptor(&metadata), metadata.cycles,
			cycles_ptr[i], METADATA_CYCLE_UNITS
		);
	}


	// Done.
	printf("\n\n");


	// Success.
	return true;
}


// Log metadata begin operation to file.
bool log_metadata_begin_op_to_file(FILE* file_ptr, prog_metadata* metadata_ptr, double elapsed_time)
{
	// Switch on op code.
	switch (metadata_ptr->code)
	{
		// OS.
		case OS:
			// Switch on descriptor.
			switch (metadata_ptr->descriptor)
			{
				// Start.
				case START:
					fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, OS_START_OP_BEGIN_MESSAGE);
					return true;


				// End.
				case END:
					fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, OS_END_OP_BEGIN_MESSAGE);
					return true;

				
				// Default.
				default:
					// Abort.
					printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
					return false;
			}


		// Application.
		case APPLICATION:
			// Switch on descriptor.
			switch (metadata_ptr->descriptor)
			{
				// Start.
				case START:
					fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, APPLICATION_START_OP_BEGIN_MESSAGE);
					return true;


				// End.
				case END:
					fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, APPLICATION_END_OP_BEGIN_MESSAGE);
					return true;

				
				// Default.
				default:
					// Abort.
					printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
					return false;
			}
		

		// Process.
		case PROCESS:
			// Run?
			if (metadata_ptr->descriptor == RUN)
			{
				// Log.
				fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, PROCESS_RUN_OP_BEGIN_MESSAGE);
				return true;
			}


			// Abort.
			printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
			return false;
		

		// Memory.
		case MEMORY:
			// Switch on descriptor.
			switch (metadata_ptr->descriptor)
			{
				// Allocate.
				case ALLOCATE:
					fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, MEMORY_ALLOCATE_OP_BEGIN_MESSAGE);
					return true;


				// Block.
				case BLOCK:
					fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, MEMORY_BLOCK_OP_BEGIN_MESSAGE);
					return true;

				
				// Default.
				default:
					// Abort.
					printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
					return false;
			}
			

		// Default.
		default:
			// Switch on descriptor.
			switch (metadata_ptr->descriptor)
			{
				// HDD.
				case HDD:
					// Switch on code.
					switch (metadata_ptr->code)
					{
						// Input.
						case INPUT:
							fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, INPUT_HDD_OP_BEGIN_MESSAGE);
							return true;


						// Output.
						case OUTPUT:
							fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, OUTPUT_HDD_OP_BEGIN_MESSAGE);
							return true;


						// Default.
						default:
							// Abort.
							printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
							return false;
					}
				

				// Keyboard.
				case KEYBOARD:
					// Input?
					if (metadata_ptr->code == INPUT)
					{
						// Log.
						fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, INPUT_KEYBOARD_OP_BEGIN_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;
				
				
				// Mouse.
				case MOUSE:
					// Input?
					if (metadata_ptr->code == INPUT)
					{
						// Log.
						fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, INPUT_MOUSE_OP_BEGIN_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;
				
				
				// Monitor.
				case MONITOR:
					// Output?
					if (metadata_ptr->code == OUTPUT)
					{
						// Log.
						fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, OUTPUT_MONITOR_OP_BEGIN_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;
				
				
				// Speaker.
				case SPEAKER:
					// Output?
					if (metadata_ptr->code == OUTPUT)
					{
						// Log.
						fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, OUTPUT_SPEAKER_OP_BEGIN_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;


				// Printer.
				case PRINTER:
					// Output?
					if (metadata_ptr->code == OUTPUT)
					{
						// Log.
						fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, OUTPUT_PRINTER_OP_BEGIN_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;


				// Default.
				default:
					// Abort.
					printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
					return false;
			}
	}
}


// Log metadata end operation to file.
bool log_metadata_end_op_to_file(FILE* file_ptr, os_config* config_ptr, prog_metadata* metadata_ptr, double elapsed_time)
{
	// Switch on op code.
	switch (metadata_ptr->code)
	{
		// OS.
		case OS:
			// Switch on descriptor.
			switch (metadata_ptr->descriptor)
			{
				// Start or end?.
				case START:
				case END:
					return true;

				
				// Default.
				default:
					// Abort.
					printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
					return false;
			}


		// Application.
		case APPLICATION:
			// Switch on descriptor.
			switch (metadata_ptr->descriptor)
			{
				// Start.
				case START:
					fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, APPLICATION_START_OP_END_MESSAGE);
					return true;


				// End.
				case END:
					return true;

				
				// Default.
				default:
					// Abort.
					printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
					return false;
			}
		

		// Process.
		case PROCESS:
			// Run?
			if (metadata_ptr->descriptor == RUN)
			{
				// Log.
				fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, PROCESS_RUN_OP_END_MESSAGE);
				return true;
			}


			// Abort.
			printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
			return false;
		

		// Memory.
		case MEMORY:
			// Switch on descriptor.
			switch (metadata_ptr->descriptor)
			{
				// Allocate.
				case ALLOCATE:
					fprintf(
						file_ptr,
						"\n\n%f - %s %x\n\n",
						elapsed_time,
						MEMORY_ALLOCATE_OP_END_MESSAGE,
						alloc_mem(config_ptr->system_memory_bytes)
					);
					return true;


				// Block.
				case BLOCK:
					fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, MEMORY_BLOCK_OP_END_MESSAGE);
					return true;

				
				// Default.
				default:
					// Abort.
					printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
					return false;
			}
			

		// Default.
		default:
			// Switch on descriptor.
			switch (metadata_ptr->descriptor)
			{
				// HDD.
				case HDD:
					// Switch on code.
					switch (metadata_ptr->code)
					{
						// Input.
						case INPUT:
							fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, INPUT_HDD_OP_END_MESSAGE);
							return true;


						// Output.
						case OUTPUT:
							fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, OUTPUT_HDD_OP_END_MESSAGE);
							return true;


						// Default.
						default:
							// Abort.
							printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
							return false;
					}
				

				// Keyboard.
				case KEYBOARD:
					// Input?
					if (metadata_ptr->code == INPUT)
					{
						// Log.
						fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, INPUT_KEYBOARD_OP_END_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;
				
				
				// Mouse.
				case MOUSE:
					// Input?
					if (metadata_ptr->code == INPUT)
					{
						// Log.
						fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, INPUT_MOUSE_OP_END_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;
				
				
				// Monitor.
				case MONITOR:
					// Output?
					if (metadata_ptr->code == OUTPUT)
					{
						// Log.
						fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, OUTPUT_MONITOR_OP_END_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;
				
				
				// Speaker.
				case SPEAKER:
					// Output?
					if (metadata_ptr->code == OUTPUT)
					{
						// Log.
						fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, OUTPUT_SPEAKER_OP_END_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;


				// Printer.
				case PRINTER:
					// Output?
					if (metadata_ptr->code == OUTPUT)
					{
						// Log.
						fprintf(file_ptr, "\n\n%f - %s\n\n", elapsed_time, OUTPUT_PRINTER_OP_END_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;


				// Default.
				default:
					// Abort.
					printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
					return false;
			}
	}
}


// Log metadata begin operation to display.
bool log_metadata_begin_op_to_display(prog_metadata* metadata_ptr, double elapsed_time)
{
	// Switch on op code.
	switch (metadata_ptr->code)
	{
		// OS.
		case OS:
			// Switch on descriptor.
			switch (metadata_ptr->descriptor)
			{
				// Start.
				case START:
					printf("\n\n%f - %s\n\n", elapsed_time, OS_START_OP_BEGIN_MESSAGE);
					return true;


				// End.
				case END:
					printf("\n\n%f - %s\n\n", elapsed_time, OS_END_OP_BEGIN_MESSAGE);
					return true;

				
				// Default.
				default:
					// Abort.
					printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
					return false;
			}


		// Application.
		case APPLICATION:
			// Switch on descriptor.
			switch (metadata_ptr->descriptor)
			{
				// Start.
				case START:
					printf("\n\n%f - %s\n\n", elapsed_time, APPLICATION_START_OP_BEGIN_MESSAGE);
					return true;


				// End.
				case END:
					printf("\n\n%f - %s\n\n", elapsed_time, APPLICATION_END_OP_BEGIN_MESSAGE);
					return true;

				
				// Default.
				default:
					// Abort.
					printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
					return false;
			}
		

		// Process.
		case PROCESS:
			// Run?
			if (metadata_ptr->descriptor == RUN)
			{
				// Log.
				printf("\n\n%f - %s\n\n", elapsed_time, PROCESS_RUN_OP_BEGIN_MESSAGE);
				return true;
			}


			// Abort.
			printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
			return false;
		

		// Memory.
		case MEMORY:
			// Switch on descriptor.
			switch (metadata_ptr->descriptor)
			{
				// Allocate.
				case ALLOCATE:
					printf("\n\n%f - %s\n\n", elapsed_time, MEMORY_ALLOCATE_OP_BEGIN_MESSAGE);
					return true;


				// Block.
				case BLOCK:
					printf("\n\n%f - %s\n\n", elapsed_time, MEMORY_BLOCK_OP_BEGIN_MESSAGE);
					return true;

				
				// Default.
				default:
					// Abort.
					printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
					return false;
			}
			

		// Default.
		default:
			// Switch on descriptor.
			switch (metadata_ptr->descriptor)
			{
				// HDD.
				case HDD:
					// Switch on code.
					switch (metadata_ptr->code)
					{
						// Input.
						case INPUT:
							printf("\n\n%f - %s\n\n", elapsed_time, INPUT_HDD_OP_BEGIN_MESSAGE);
							return true;


						// Output.
						case OUTPUT:
							printf("\n\n%f - %s\n\n", elapsed_time, OUTPUT_HDD_OP_BEGIN_MESSAGE);
							return true;


						// Default.
						default:
							// Abort.
							printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
							return false;
					}
				

				// Keyboard.
				case KEYBOARD:
					// Input?
					if (metadata_ptr->code == INPUT)
					{
						// Log.
						printf("\n\n%f - %s\n\n", elapsed_time, INPUT_KEYBOARD_OP_BEGIN_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;
				
				
				// Mouse.
				case MOUSE:
					// Input?
					if (metadata_ptr->code == INPUT)
					{
						// Log.
						printf("\n\n%f - %s\n\n", elapsed_time, INPUT_MOUSE_OP_BEGIN_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;
				
				
				// Monitor.
				case MONITOR:
					// Output?
					if (metadata_ptr->code == OUTPUT)
					{
						// Log.
						printf("\n\n%f - %s\n\n", elapsed_time, OUTPUT_MONITOR_OP_BEGIN_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;
				
				
				// Speaker.
				case SPEAKER:
					// Output?
					if (metadata_ptr->code == OUTPUT)
					{
						// Log.
						printf("\n\n%f - %s\n\n", elapsed_time, OUTPUT_SPEAKER_OP_BEGIN_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;


				// Printer.
				case PRINTER:
					// Output?
					if (metadata_ptr->code == OUTPUT)
					{
						// Log.
						printf("\n\n%f - %s\n\n", elapsed_time, OUTPUT_PRINTER_OP_BEGIN_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;


				// Default.
				default:
					// Abort.
					printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
					return false;
			}
	}
}


// Log metadata end operation to display.
bool log_metadata_end_op_to_display(os_config* config_ptr, prog_metadata* metadata_ptr, double elapsed_time)
{
	// Switch on op code.
	switch (metadata_ptr->code)
	{
		// OS.
		case OS:
			// Switch on descriptor.
			switch (metadata_ptr->descriptor)
			{
				// Start or end?.
				case START:
				case END:
					return true;

				
				// Default.
				default:
					// Abort.
					printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
					return false;
			}


		// Application.
		case APPLICATION:
			// Switch on descriptor.
			switch (metadata_ptr->descriptor)
			{
				// Start.
				case START:
					printf("\n\n%f - %s\n\n", elapsed_time, APPLICATION_START_OP_END_MESSAGE);
					return true;


				// End.
				case END:
					return true;

				
				// Default.
				default:
					// Abort.
					printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
					return false;
			}
		

		// Process.
		case PROCESS:
			// Run?
			if (metadata_ptr->descriptor == RUN)
			{
				// Log.
				printf("\n\n%f - %s\n\n", elapsed_time, PROCESS_RUN_OP_END_MESSAGE);
				return true;
			}


			// Abort.
			printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
			return false;
		

		// Memory.
		case MEMORY:
			// Switch on descriptor.
			switch (metadata_ptr->descriptor)
			{
				// Allocate.
				case ALLOCATE:
					printf(
						"\n\n%f - %s %x\n\n",
						elapsed_time,
						MEMORY_ALLOCATE_OP_END_MESSAGE,
						alloc_mem(config_ptr->system_memory_bytes)
					);
					return true;


				// Block.
				case BLOCK:
					printf("\n\n%f - %s\n\n", elapsed_time, MEMORY_BLOCK_OP_END_MESSAGE);
					return true;

				
				// Default.
				default:
					// Abort.
					printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
					return false;
			}
			

		// Default.
		default:
			// Switch on descriptor.
			switch (metadata_ptr->descriptor)
			{
				// HDD.
				case HDD:
					// Switch on code.
					switch (metadata_ptr->code)
					{
						// Input.
						case INPUT:
							printf("\n\n%f - %s\n\n", elapsed_time, INPUT_HDD_OP_END_MESSAGE);
							return true;


						// Output.
						case OUTPUT:
							printf("\n\n%f - %s\n\n", elapsed_time, OUTPUT_HDD_OP_END_MESSAGE);
							return true;


						// Default.
						default:
							// Abort.
							printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
							return false;
					}
				

				// Keyboard.
				case KEYBOARD:
					// Input?
					if (metadata_ptr->code == INPUT)
					{
						// Log.
						printf("\n\n%f - %s\n\n", elapsed_time, INPUT_KEYBOARD_OP_END_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;
				
				
				// Mouse.
				case MOUSE:
					// Input?
					if (metadata_ptr->code == INPUT)
					{
						// Log.
						printf("\n\n%f - %s\n\n", elapsed_time, INPUT_MOUSE_OP_END_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;
				
				
				// Monitor.
				case MONITOR:
					// Output?
					if (metadata_ptr->code == OUTPUT)
					{
						// Log.
						printf("\n\n%f - %s\n\n", elapsed_time, OUTPUT_MONITOR_OP_END_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;
				
				
				// Speaker.
				case SPEAKER:
					// Output?
					if (metadata_ptr->code == OUTPUT)
					{
						// Log.
						printf("\n\n%f - %s\n\n", elapsed_time, OUTPUT_SPEAKER_OP_END_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;


				// Printer.
				case PRINTER:
					// Output?
					if (metadata_ptr->code == OUTPUT)
					{
						// Log.
						printf("\n\n%f - %s\n\n", elapsed_time, OUTPUT_PRINTER_OP_END_MESSAGE);
						return true;
					}


					// Abort.
					printf("\n\n%s\n\n", INVALID_CODE_RUNTIME_ERROR_MESSAGE);
					return false;


				// Default.
				default:
					// Abort.
					printf("\n\n%s\n\n", INVALID_DESCRIPTOR_RUNTIME_ERROR_MESSAGE);
					return false;
			}
	}
}


// Compute metadata metrics.
bool compute_metadata_metrics(os* os_ptr, unsigned int* cycles_ptr)
{
	// Variables.
	unsigned int max = os_ptr->pcb.num_metadata;


	// Cycles per metadata.
	for (unsigned int i = 0; i < max; i++)
	{
		// Descriptor?
		switch (os_ptr->pcb.metadata[i].descriptor)
		{
			// Start or end?
			case START:
			case END:
				// No cycles.
				cycles_ptr[i] = 0;
				break;


			// Run?
			case RUN:
				// Processor.
				cycles_ptr[i] = os_ptr->pcb.metadata[i].cycles * os_ptr->config.processor_period_ms;
				break;


			// Allocate or block?
			case ALLOCATE:
			case BLOCK:
				// Memory.
				cycles_ptr[i] = os_ptr->pcb.metadata[i].cycles * os_ptr->config.memory_period_ms;
				break;


			// HDD?
			case HDD:
				// HDD.
				cycles_ptr[i] = os_ptr->pcb.metadata[i].cycles * os_ptr->config.hdd_period_ms;
				break;


			// Keyboard?
			case KEYBOARD:
				// Keyboard.
				cycles_ptr[i] = os_ptr->pcb.metadata[i].cycles * os_ptr->config.keyboard_period_ms;
				break;


			// Mouse?
			case MOUSE:
				// Mouse.
				cycles_ptr[i] = os_ptr->pcb.metadata[i].cycles * os_ptr->config.mouse_period_ms;
				break;
			

			// Monitor?
			case MONITOR:
				// Monitor.
				cycles_ptr[i] =  os_ptr->pcb.metadata[i].cycles * os_ptr->config.monitor_period_ms;
				break;


			// Speaker?
			case SPEAKER:
				// Speaker.
				cycles_ptr[i] = os_ptr->pcb.metadata[i].cycles * os_ptr->config.speaker_period_ms;
				break;


			// Printer?
			case PRINTER:
				// Printer.
				cycles_ptr[i] = os_ptr->pcb.metadata[i].cycles * os_ptr->config.printer_period_ms;
				break;


			// Default.
			default:
				// Abort.
				printf("\n\n\
					ERROR COMPUTING TIME FOR METATADATA %u\
					\n\n", i + 1
				);
				return false;
		}
	}


	// Success.
	return true;
}


// End header guard.
#endif