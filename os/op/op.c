// Header guard.
#ifndef OS_OP_C_
#define OS_OP_C_


// Header.
#include "op.h"


// Functon prototypes.
static inline int set_op_type(op* this, prog_metadata* metadata_ptr);
static inline int set_op_device(op* this, prog_metadata* metadata_ptr);


// Create operation.
int create_op(op* this, const prog_metadata* metadata_ptr)
{
    // Set type.
    if (set_op_type(this, metadata_ptr))
    {
        // Alert.
        printf("\n\nFailed to set the operation type.\n\n");


        // Abort.
        return 1;
    }


    // Import cycles directly.
    this->cycles_left = metadata_ptr->cycles;


    // I/O operation?
    if (this->type == INPUT || this->type == OUTPUT)
    {
        // Set device.
        if (set_op_device(this, metadata_ptr))
        {
            // Alert.
            printf("\n\nFailed to set the operation device.\n\n");


            // Abort.
            return 1;
        }
    }


    // Done.
    return 0;
}


// Set operation type.
int set_op_type(op* this, prog_metadata* metadata_ptr)
{
    // Switch on metadata code.
    switch (metadata_ptr->code)
    {
        // Input?
        case INPUT:
            // Set.
            this->type = OP_INPUT;


            // Done.
            return 0;


        // Output?
        case OUTPUT:
            // Set.
            this->type = OP_OUTPUT;


            // Done.
            return 0;


        // Process?
        case PROCESS:
            // Set.
            this->type = OP_CPU;


            // Done.
            return 0;


        // Memory?
        case MEMORY:
            // Set.
            this->type = OP_MEMORY;


            // Done.
            return 0;


        // No match?
        default:
            // Alert.
            printf("\n\nCould not convert metadata to operation.\n\n");


            // Abort.
            return 1;
    }
}


// Set operation device.
int set_op_device(op* this, prog_metadata* metadata_ptr)
{
    // Switch on metadata descriptor.
    switch (metadata_ptr->descriptor)
    {
        // HDD?
        case HDD:
            // Set.
            this->device = OP_HDD;


            // Done.
            return 0;


        // Keyboard?
        case KEYBOARD:
            // Set.
            this->device = OP_KEYBOARD;


            // Done.
            return 0;


        // Mouse?
        case MOUSE:
            // Set.
            this->device = OP_MOUSE;


            // Done.
            return 0;


        // Monitor?
        case MONITOR:
            // Set.
            this->device = OP_MONITOR;


            // Done.
            return 0;

        
        // Speaker?
        case SPEAKER:
            // Set.
            this->device = OP_SPEAKER;


            // Done.
            return 0;
        

        // Printer?
        case PRINTER:
            // Set.
            this->device = OP_PRINTER;


            // Done.
            return 0;


        // No match?
        default:
            // Alert.
            printf("\n\nCould not convert metadata to operation.\n\n");


            // Abort.
            return 1;
    }
}


// End header guard.
#endif