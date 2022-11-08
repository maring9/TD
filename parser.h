#ifndef __AT_COMMAND_PARSER__
#define __AT_COMMAND_PARSER__

#include <stdint.h>

#define AT_COMMAND_MAX_LINES 100
#define AT_COMMAND_MAX_LINE_SIZE 128

typedef struct
{
    uint8_t data[AT_COMMAND_MAX_LINES][AT_COMMAND_MAX_LINE_SIZE + 1];
    uint8_t flag; // 1 - OK , 0 - ERROR
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

extern int current_state;
extern int num_transmissions;
extern int aux_row;
extern int line_count;
extern int column_index;

#endif
