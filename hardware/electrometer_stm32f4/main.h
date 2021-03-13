#ifndef __MAIN_H
#define __MAIN_H


#include<stdio.h>
#include<string.h>
//#include<stdlib.h>
#include "stm32f4xx.h"

////////////////////////////////////////////////////////////////////////////////

typedef struct event_measurement_struct_s {
    uint8_t id_consumer;
    uint16_t id_event;
    uint8_t id_measurement;
    char timestamp_time_start[14];
    uint16_t power;
} event_measurement_struct_t;

typedef struct MCP_measurement_struct_s {
    uint32_t CURRENT_RMS;
    uint16_t VOLTAGE_RMS;
    uint32_t ACTIVE_POWER;
	uint32_t REACTIVE_POWER;
	uint16_t LINE_FREQUENCY;
    //char timestamp_time_start[14];
} MCP_measurement_struct_t;

typedef struct event_control_struct_s {
    uint8_t event_present;
    uint8_t has_a_shot;
    RTC_TimeTypeDef start_time;
    RTC_DateTypeDef start_date;
    uint32_t CURRENT_RMS;
    uint32_t ACTIVE_POWER;
	uint32_t REACTIVE_POWER;
    uint8_t id_consumer;
    uint16_t id_event;
    uint8_t id_measurement;
	uint8_t event_created;
} event_control_struct_t;

////////////////////////////////////////////////////////////////////////////////

extern event_measurement_struct_t event_measurement[100];
extern MCP_measurement_struct_t MCP_measurement;
extern MCP_measurement_struct_t shot_measurement[100];
extern event_control_struct_t event_control;

extern uint8_t first_empty_element_of_roll_buffer;
extern uint8_t first_element_for_transmit_of_roll_buffer;
extern uint8_t first_empty_element_of_shot_measurement;


#endif  // __MAIN_H
