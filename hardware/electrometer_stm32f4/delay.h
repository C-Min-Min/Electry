#ifndef _DELAY_H
#define _DELAY_H


#include "stm32f4xx.h"

////////////////////////////////////////////////////////////////////////////////

typedef enum waitmsChannel_e{
    channelBtnMatrix = 0,
    channelTime1 = 1,
    channelTime2 = 2,
    channelLed = 3,
    channelButton = 4
} waitmsChannel_t;

////////////////////////////////////////////////////////////////////////////////

void delay_us(unsigned int us);
void delay_ms(unsigned int ms);

void clear_wait_ms();
uint8_t wait_ms(uint32_t time);

void clear_wait_ms_ch(uint8_t channel);
void reset_wait_ms_ch(uint8_t channel);
uint8_t wait_ms_ch(uint8_t channel, uint32_t time);

////////////////////////////////////////////////////////////////////////////////

#endif	// _DELAY_H