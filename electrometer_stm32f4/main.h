#ifndef __MAIN_H
#define __MAIN_H


#include<stdio.h>
#include<string.h>
//#include<stdlib.h>
#include "stm32f4xx.h"
//#include "stm32f4xx_conf.h"

////////////////////////////////////////////////////////////////////////////////

typedef struct event_measurement_struct_s {
    uint8_t id_consumer;
    uint16_t id_event;
    uint8_t id_measurement;
    char timestamp_time_start[14];
    uint16_t power;
} event_measurement_struct_t;

////////////////////////////////////////////////////////////////////////////////

//RTC_TimeTypeDef time;
//extern int sec1;


#endif  // __MAIN_H