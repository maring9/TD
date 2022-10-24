#include "parser.h"
#include <stdio.h>

AT_COMMAND_DATA command_data;

// Wrappers for readability
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

STATE_MACHINE_RETURN_VALUE at_command_parse(uint8_t current_char)
{
    // Initialization
    static uint8_t current_state = 0;
    static uint8_t column_index = 0;

    switch (current_state)
    {
    case 0:
    {
        if (char_is_CR(current_char))
        {
            current_state = 1;

            // printf("Current state: 0, got char %d\n", current_char);
            return STATE_MACHINE_NOT_READY;
        }
        else
        {
            printf("Error in state 0, expected CR, got %d\n", current_char);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case 1:
    {
        if (char_is_LF(current_char))
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
        break;
        
    }
    case 2:
    {
        if (current_char == 'O')
        {
            current_state = 3;
            // printf("Current state: 2, got char %c\n", current_char);
            return STATE_MACHINE_NOT_READY;
        }
        else if (current_char == 'E')
        {
            current_state = 7;
            // printf("Current state: 2, got char %c\n", current_char);
            return STATE_MACHINE_NOT_READY;
        }
        else if (current_char == '+')
        {
            current_state = 15;
            // printf("Current state: 2, got char %c\n", current_char);
            return STATE_MACHINE_NOT_READY;
        }
        else
        {
            printf("Error in state 2, expected + , E, O, got char %c\n", current_char);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case 3:
    {
        if (current_char == 'K')
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
        break;
    }
    case 4:
    {
        if (char_is_CR(current_char))
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
        break;
    }
    case 5:
    {
        if (char_is_LF(current_char))
        {
            current_state = 0;
            // printf("Current state: 5, got char %d\n", current_char);
            printf("No data - parsed CRLF OK CRLF\n");
            return STATE_MACHINE_READY_OK;
        }
        else
        {
            printf("Error in state 5, expected LF, got char %c\n", current_char);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
    }
    // Not needed
    case 6:
    {
        // printf("Current state: 6, transmision ready\n");
        return STATE_MACHINE_READY_OK;
    }
    case 7:
    {
        if (current_char == 'R')
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
        break;
    }
    case 8:
    {
        if (current_char == 'R')
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
        break;
    }
    case 9:
    {
        if (current_char == 'O')
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
        break;
    }
    case 10:
    {
        if (current_char == 'R')
        {
            current_state = 11;
            // printf("Current state: 10, got char %c\n", current_char);
            return STATE_MACHINE_NOT_READY;
        }
        else
        {
            printf("Error in state 10, expected R, got char %c\n", current_char);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case 11:
    {
        if (char_is_CR(current_char))
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
        break;
    }
    case 12:
    {
        if (char_is_LF(current_char))
        {
            current_state = 0;
            // printf("Current state: 12, got char %d\n", current_char);
            printf("No data - parsed CRLF ERROR CRLF\n");
            return STATE_MACHINE_READY_OK;
        }
        else
        {
            printf("Error in state 12, expected LF, got char %c\n", current_char);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
    }
    // Same as state 6
    case 13:
    {
        // printf("Current state: 13, transmision ready\n");
        return STATE_MACHINE_READY_OK;
    }
    case 15:
    {
        // printf("In case 15\n");
        if (char_is_valid_ascii(current_char))
        {
            // printf("Char is valid ascii\n");
            if (space_to_save_lines(command_data.line_count))
            {
                // printf("Still space to save lines\n");
                if (space_to_save_chars(column_index))
                {
                    // printf("Still space to save chars\n");
                    // Save data
                    // printf("Current state: 15, adding %c at line %d col %d\n", current_char, command_data.line_count, column_index);
                    command_data.data[command_data.line_count][column_index] = current_char;
                    column_index++;
                    // //printf("New char will be placed at %d %d\n", command_data.line_count, column_index);
                    current_state = 16;
                    return STATE_MACHINE_NOT_READY;
                }
            }
        }
        else
        {
            printf("Error in state 15, expected ASCII [32, 126], got char %d\n", current_char);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case 16:
    {   
        // printf("In case 16\n");
        if (char_is_valid_ascii(current_char))
        {   
            // printf("Char is valid ascii\n");
            if (space_to_save_lines(command_data.line_count))
            {
                // printf("Still space to save lines\n");
                if (space_to_save_chars(column_index))
                {
                    // printf("Still space to save chars\n");
                    // Save data
                    command_data.data[command_data.line_count][column_index] = current_char;
                    // printf("Current state: 16, adding %c at line %d col %d\n", current_char, command_data.line_count, column_index);
                    column_index++;
                    // printf("New char will be placed at %d %d\n", command_data.line_count, column_index);
                    current_state = 16;
                    return STATE_MACHINE_NOT_READY;
                }
            }
            
        }
        else if (char_is_CR(current_char))
        {
            // printf("Got CR\n");
            if (space_to_save_lines(command_data.line_count))
            {
                // printf("Still space to save lines\n");
                if (space_to_save_chars(column_index))
                {
                    // printf("Still space to save chars\n");
                    current_state = 17;
                    // printf("Current state: 16, got char %d\n", current_char);
                    return STATE_MACHINE_NOT_READY;
                }
            }
            else
            {
                return STATE_MACHINE_FULL;
            }
            
        }
        else
        {
            printf("Error in state 16, expected CR, got char %d\n", current_char);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case 17:
    {
        // printf("In case 17\n");
        if (char_is_LF(current_char)) 
        {
            // printf("Char is LF\n");
            current_state = 18;
            // printf("Current state: 17, got char %d\n", current_char);
            return STATE_MACHINE_NOT_READY;
        }
        else
        {
            printf("Error in state 17, expected LF, got char %c\n", current_char);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case 18:
    {
        // printf("In case 18\n");
        if (char_is_CR(current_char))
        {
            //printf("Got char CR\n");
            // Save data
            command_data.data[command_data.line_count][column_index] = '\0';
            // printf("String ended on %d %d\n", command_data.line_count, column_index);
            command_data.line_count++;

            column_index = 0;
            // printf("New char will be placed at %d %d\n", command_data.line_count, column_index);
            
            current_state = 19;
            // printf("Current state: 18, got char %d\n", current_char);
            return STATE_MACHINE_NOT_READY;
        }
        else if (current_char == '+')
        {
            // printf("Got char +\n");
            // Finished parsing a line, resetting column indx
            command_data.line_count++;

            column_index = 0;
            // printf("New char will be placed at %d %d\n", command_data.line_count, column_index);
            current_state = 16;
            // printf("Current state: 18, got char %c\n", current_char);
            return STATE_MACHINE_NOT_READY;
        }
        else
        {
            printf("Error in state 18, expected +, CR got char %c\n", current_char);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case 19:
    {
        if (char_is_LF(current_char))
        {
            current_state = 20;
            //printf("Current state: 19, got char %d\n", current_char);
            return STATE_MACHINE_NOT_READY;
        }
        else
        {
            printf("Error in state 19, expected LF, got char %c\n", current_char);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
    case 20:
    {
        if (current_char == 'O')
        {
            current_state = 3;
            //printf("Current state: 20, got char %c\n", current_char);
            return STATE_MACHINE_NOT_READY;
        }
        else if (current_char == 'E')
        {
            current_state = 7;
            //printf("Current state: 20, got char %c\n", current_char);
            return STATE_MACHINE_NOT_READY;
        }
        else
        {
            printf("Error in state 20, expected O, E, got char %c\n", current_char);
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }

    }
    //return STATE_MACHINE_NOT_READY;
}