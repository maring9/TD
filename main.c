#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

// Function to read final data from the struct
void print_data()
{
    printf("\n");
    for(int i = 0; i < command_data.line_count; ++i)
    {
        printf("%s \n", command_data.data[i]);
    }
}

// Wrapper for testing for EOF
int eof_reached(FILE *file)
{
    return feof(file);
}

// Wrapper for reading a line from the stream
void read_line(char *buffer, FILE *file)
{
    fgets(buffer, AT_COMMAND_MAX_LINE_SIZE + 1, file);
}

int machine_not_ready(STATE_MACHINE_RETURN_VALUE return_value)
{
    return return_value == STATE_MACHINE_NOT_READY;
}

// For readability
int chars_left_in_string(int length, char *string)
{
    return length < strlen(string);
}

// For readability
int machine_exit_with_error(STATE_MACHINE_RETURN_VALUE return_value)
{
    return return_value == STATE_MACHINE_READY_WITH_ERROR;
}

// For readability
int machine_exit_ok(STATE_MACHINE_RETURN_VALUE return_value)
{
    return return_value == STATE_MACHINE_READY_OK;
}

int machine_full(STATE_MACHINE_RETURN_VALUE return_value)
{
    return return_value == STATE_MACHINE_FULL;
}

// For readability
int wrong_number_of_args(int argc)
{
    return argc != 2;
}

int main(int argc, char *argv[])
{
    if (wrong_number_of_args(argc))
    {
        perror("Usage: <test_file>\n");
        return 1;
    }

    // Buffer to read a line into
    char line_read[AT_COMMAND_MAX_LINE_SIZE + 1];

    // For storing the char to be parsed
    uint8_t char_read;
    
    // Initialization
    STATE_MACHINE_RETURN_VALUE return_state = STATE_MACHINE_NOT_READY;
    command_data.line_count = 0;


    FILE *file = fopen(argv[1], "rb");
    
    if (!file)
    {
        perror("Error opening file\n");
        return -1;
    }
    
    // Main loop to read from file
    while (!eof_reached(file))
    {
        // Index of char in read string
        int char_index = 0;

        read_line(line_read, file);
        
        if(eof_reached(file))
        {
            // printf("EOF REACHED\n");
            break;
        }
        
        // Loop through the chars in the string till a final state is achieved
        while(machine_not_ready(return_state) && chars_left_in_string(char_index, line_read))
        {

            // Read and parse char
            char_read = line_read[char_index];
            return_state = at_command_parse(char_read);

            char_index++;

            if(machine_exit_with_error(return_state))
            {
                printf("Ready with ERROR\n");
                return_state = STATE_MACHINE_NOT_READY;
                return 0;
            }
            if(machine_exit_ok(return_state))
            {
                printf("Ready OK\n");
                return_state = STATE_MACHINE_NOT_READY;
            }
            if(machine_full(return_state))
            {
                printf("State machine full, not saving any further data\n");
            }
        }
    }
    
    // print_data();

    fclose(file);

    return 0;
}