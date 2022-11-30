#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

uint8_t extract_CSQ()
{
    uint8_t res[] =  "";
    int j = 0;
    int stop = 0;
    for(int i = 0; i < aux_row; ++i)
    {
        if (stop)
        {
            break;
        }
        for(int k = 0; k < strlen(command_data.data[i]); ++k)
        {
            if (command_data.data[i][k] == ',')
            {
                stop = 1;
                break;
            }
            if (command_data.data[i][k] >= 48 && command_data.data[i][k] <= 57)
            {
                res[j] += command_data.data[i][k];
                j++;
            }
        }
    }

    uint8_t rssi = atoi(res);

    return rssi;
}

uint8_t extract_CREG()
{
    uint8_t res;
    int k;
    int stop = 0;
    for(int i = 0; i < aux_row; ++i)
    {
        if (stop)
        {
            break;
        }
        for(k = 0; k < strlen(command_data.data[i]); ++k)
        {
            if (command_data.data[i][k] == ',')
            {
                res = command_data.data[i][k+1];
                stop = 1;
                break;
            }
        }
    }
    uint8_t stat = res - '0';

    return stat;
}

uint8_t cops[50];

void extract_COPS(uint8_t* result)
{
    uint8_t res[50];
    int start = 0, stop = 0;
    int k;
    int idx = 0;

    for(k = 0; k < strlen(command_data.data[0]); ++k)
    {
        if (command_data.data[0][k] == '"')
        {
            if (start != 0)
            {
                stop = k;
                break;
            }
            start = k;
        }
    }
    for (int j = start + 1; j < stop; ++j)
    {
        res[idx] = command_data.data[0][j];
        idx++;
    }
    strcpy(result, res);
}

uint8_t imei[30];

void extract_GSN(uint8_t* imei)
{
    uint8_t res[20];
    int j = 0;
    int stop = 0;
    for(int i = 0; i < aux_row; ++i)
    {
        if (stop)
        {
            break;
        }
        for(int k = 0; k < strlen(command_data.data[i]); ++k)
        {
            if (command_data.data[i][k] == 13)
            {
                stop = 1;
                break;
            }
            res[j] = command_data.data[i][k];
            j++;
        }
    }

    strcpy(imei, res);

}

uint8_t manufacturer[30];

void extract_GMI(uint8_t* manufacturer)
{
    uint8_t res[20];
    int j = 0;
    int stop = 0;
    for(int i = 0; i < aux_row; ++i)
    {
        if (stop)
        {
            break;
        }
        for(int k = 0; k < strlen(command_data.data[i]); ++k)
        {
            if (command_data.data[i][k] == 13)
            {
                stop = 1;
                break;
            }
            res[j] = command_data.data[i][k];
            j++;
        }
    }

    strcpy(manufacturer, res);

}

uint8_t software_version[50];

void extract_GMR(uint8_t* version)
{
    uint8_t res[50];
    int start = 0;
    int k;
    int idx = 0;

    for(k = 0; k < strlen(command_data.data[0]); ++k)
    {
        if (command_data.data[0][k] == ':')
        {
            start = k;
        }
    }
    for (int j = start + 1; j < strlen(command_data.data[0]); ++j)
    {
        res[idx] = command_data.data[0][j];
        idx++;
    }
    strcpy(version, res);
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
                // printf("Ready with ERROR\n");

                return_state = STATE_MACHINE_NOT_READY;

                return 0;
            }
            if(machine_exit_ok(return_state))
            {
                // printf("Ready OK\n");
                if (command_data.flag == 1)
                {
                    printf("Parse OK\n\n");
                }
                else
                {
                    printf("Parse ERROR\n\n");
                }
                //uint8_t rssi = extract_data();
                //printf("RSSI: %d\n", rssi);
                //uint8_t stat = extract_CREG();
                //printf("State of registration: %d\n", stat);
                //extract_COPS(cops);
                // printf("Network: %s\n", cops);
                //extract_GSN(imei);
                //printf("%s\n", imei);
                //extract_GMI(manufacturer);
                //printf("%s\n", manufacturer);
                extract_GMR(software_version);
                printf("%s\n", software_version);
                return_state = STATE_MACHINE_NOT_READY;
            }
        }
    }
    
    fclose(file);

    return 0;
}