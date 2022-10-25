#include "parser.h"
#include <stdio.h>
#include <string.h>

#define CR 0x0D
#define LF 0x0A

AT_COMMAND_DATA command_data;

int char_is(uint8_t expected_char, uint8_t current_char)
{
    return current_char == expected_char;
}

// Wrappers for readability
// Not used
int char_is_CR(uint8_t current_char)
{
    return current_char == 0x0D;
}

int char_is_LF(uint8_t current_char)
{
    return current_char == 0x0A;
}

int char_is_valid_ascii(uint8_t current_char)
{
    return current_char >= 32 && current_char <= 126;
}

int space_to_save_lines(int num_lines)
{
    return num_lines < AT_COMMAND_MAX_LINES;
}

int space_to_save_chars(int num_chars)
{
    return num_chars < AT_COMMAND_MAX_LINE_SIZE;
}

int current_state = 0;
int num_transmissions = 0;

// Initial state of FSM
void initialize_fsm()
{
    current_state = 0;
    command_data.line_count = -1;
    command_data.column_index = 0;
    command_data.rows = 0;
}

// Prtin FSM data
void print_data() {
    printf("Transmission %d:\n", num_transmissions);
    for (int i = 0; i <= command_data.rows; i++) {
        printf("%s\n", command_data.data[i]);
    }
}

// Reset data from the FSM
void reset_saved_data()
{
    for (int i = 0; i <= command_data.rows; i++)
    {
        memset(command_data.data[i], 0, sizeof command_data.data[i]);
    }
}

// Reset FSM to initial state
void reset_fsm() {
    // current_state = 0;
    command_data.line_count = -1;
    command_data.column_index = 0;
    command_data.rows = 0;
    reset_saved_data();
}

STATE_MACHINE_RETURN_VALUE at_command_parse(uint8_t current_char)
{
    initialize_fsm();
    printf("%d %d %d %d ", current_state, command_data.line_count, command_data.column_index, command_data.rows = 0);
    printf("Got char: %d\n", current_char);

    if (space_to_save_lines(command_data.line_count) && space_to_save_chars(command_data.column_index)) {
            if (char_is(LF, current_char)) {
                command_data.line_count++;
                command_data.column_index = 0;
            } else if (!char_is(CR, current_char)) {
                command_data.data[command_data.line_count][command_data.column_index] = current_char;
                command_data.column_index++;
            }
        }
    if (command_data.line_count > command_data.rows && command_data.line_count < AT_COMMAND_MAX_LINES) {
        command_data.rows = command_data.line_count;
    }

    switch (current_state)
    {
        case 0:
        {
            if (char_is(CR, current_char))
            {
                current_state = 1;
                printf("New state: %d\n", current_state);
                // printf("Current state: 0, got char %d\n", current_char);
                return STATE_MACHINE_NOT_READY;
            }
            else
            {
                printf("Error in state 0, expected CR, got %d\n", current_char);
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }
        case 1:
        {
            if (char_is(LF, current_char))
            {
                current_state = 2;
                // printf("Current state: 1, got char %d\n", current_char);
                return STATE_MACHINE_NOT_READY;
            }
            else
            {
                printf("Error in state 1, expected LF, got %c\n", current_char);
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }
        case 2:
        {
            if (char_is('O', current_char))
            {
                current_state = 3;
                // printf("Current state: 2, got char %c\n", current_char);
                return STATE_MACHINE_NOT_READY;
            }
            else if (char_is('E', current_char))
            {
                current_state = 7;
                // printf("Current state: 2, got char %c\n", current_char);
                return STATE_MACHINE_NOT_READY;
            }
            else if (char_is('+', current_char))
            {
                current_state = 12;
                // printf("Current state: 2, got char %c\n", current_char);
                return STATE_MACHINE_NOT_READY;
            }
            else
            {
                printf("Error in state 2, expected + , E, O, got char %c\n", current_char);
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }
        case 3:
        {
            if (char_is('K', current_char))
            {
                current_state = 4;
                // printf("Current state: 3, got char %c\n", current_char);
                return STATE_MACHINE_NOT_READY;
            }
            else
            {
                printf("Error in state 3, expected K, got char %c\n", current_char);
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }
        case 4:
        {
            if (char_is(CR, current_char))
            {
                current_state = 5;
                // printf("Current state: 4, got char %d\n", current_char);
                return STATE_MACHINE_NOT_READY;
            }
            else
            {
                printf("Error in state 4, expected CR, got char %c\n", current_char);
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }
        case 5:
        {
            if (char_is(LF, current_char))
            {
                print_data();
                reset_fsm();

                num_transmissions++;
                // printf("Current state: 5, got char %d\n", current_char);
                // printf("No data - parsed CRLF OK CRLF\n");
                return STATE_MACHINE_READY_OK;
            }
            else
            {
                printf("Error in state 5, expected LF, got char %c\n", current_char);
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }
        case 7:
        {
            if (char_is('R', current_char))
            {
                current_state = 8;
                // printf("Current state: 7, got char %c\n", current_char);
                return STATE_MACHINE_NOT_READY;
            }
            else
            {
                printf("Error in state 7, expected R, got char %c\n", current_char);
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }
        case 8:
        {
            if (char_is('R', current_char))
            {
                current_state = 9;
                // printf("Current state: 8, got char %c\n", current_char);
                return STATE_MACHINE_NOT_READY;
            }
            else
            {
                printf("Error in state 8, expected R, got char %c\n", current_char);
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }
        case 9:
        {
            if (char_is('O', current_char))
            {
                current_state = 10;
                // printf("Current state: 9, got char %c\n", current_char);
                return STATE_MACHINE_NOT_READY;
            }
            else
            {
                printf("Error in state 9, expected O, got char %c\n", current_char);
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }
        case 10:
        {
            if (char_is('R', current_char))
            {
                current_state = 4;
                // printf("Current state: 10, got char %c\n", current_char);
                return STATE_MACHINE_NOT_READY;
            }
            else
            {
                printf("Error in state 10, expected R, got char %c\n", current_char);
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }
        case 11:
        {
            if (char_is(CR, current_char))
            {
                current_state = 12;
                // printf("Current state: 11, got char %d\n", current_char);
                return STATE_MACHINE_NOT_READY;
            }
            else
            {
                printf("Error in state 11, expected CR, got char %c\n", current_char);
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }
        case 12:
        {
            if (char_is_valid_ascii(current_char))
            {
                return STATE_MACHINE_NOT_READY;
            }
            else if (char_is(CR, current_char))
            {
                current_state = 13;
                return STATE_MACHINE_NOT_READY;
            }
            else
            {
                printf("Error in state 12, expected LF, got char %c\n", current_char);
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }
        case 13:
        {
            if (char_is(LF, current_char))
            {
                current_state = 14;
                return STATE_MACHINE_NOT_READY;
            }
            else
            {
                printf("Error in state 13, expected LF, got %c\n", current_char);
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            // printf("Current state: 13, transmision ready\n");
            // return STATE_MACHINE_READY_OK;
        }
        case 14:
        {
            if (char_is(CR, current_char))
            {
                current_state = 15;
                return STATE_MACHINE_NOT_READY;
            }
            else if (char_is('+', current_char))
            {
                current_state = 12;
                return STATE_MACHINE_NOT_READY;
            }
            else
            {
                printf("Error in state 14, expected CR, +, got char %c\n", current_char);
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }
        case 15:
        {   
            if (char_is(LF, current_char))
            {
                current_state = 16;
                return STATE_MACHINE_NOT_READY;
            }
            else
            {
                printf("Error in state 15, expected LF, got char %c\n", current_char);
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }
        case 16:
        {
            if (char_is('O', current_char))
            {
                current_state = 3;
                return STATE_MACHINE_NOT_READY;
            }
            else if (char_is('E', current_char))
            {
                current_state = 7;
                return STATE_MACHINE_NOT_READY;
            }

            else
            {
                printf("Error in state 16, expected O, E, got char %c\n", current_char);
                return STATE_MACHINE_READY_WITH_ERROR;
            }
        }

        default: return STATE_MACHINE_NOT_READY;
    }
}

#ifndef __AT_COMMAND_PARSER__
#define __AT_COMMAND_PARSER__

#include <stdint.h>

#define AT_COMMAND_MAX_LINES 100
#define AT_COMMAND_MAX_LINE_SIZE 128

typedef struct
{
    uint8_t data[AT_COMMAND_MAX_LINES][AT_COMMAND_MAX_LINE_SIZE + 1];
    int8_t line_count;
    uint8_t column_index;
    uint8_t rows;
} AT_COMMAND_DATA;

extern AT_COMMAND_DATA command_data;

typedef enum
{
    STATE_MACHINE_NOT_READY,
    STATE_MACHINE_READY_OK,
    STATE_MACHINE_READY_WITH_ERROR,
    STATE_MACHINE_FULL,
} STATE_MACHINE_RETURN_VALUE;

STATE_MACHINE_RETURN_VALUE at_command_parse(uint8_t current_char);


#endif