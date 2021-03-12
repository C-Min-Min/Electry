#include "mcp39f511.h"
#include "stm32f4xx.h"
#include<stdio.h>

////////////////////////////////////////////////////////////////////////////////

char UART1_RX_Data[35];

////////////////////////////////////////////////////////////////////////////////

void USART1_IRQHandler (void){
    static uint8_t i = 0;

    GPIO_SetBits(GPIOA, GPIO_Pin_8);
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);

    if (USART1->SR & USART_CR1_RXNEIE){
	//USART1->SR &= ~USART_CR1_RXNEIE;
	
        char n = (char) USART_ReceiveData(USART1);
        UART1_RX_Data[i] = n;
        if(i++ == 34) {
            i = 0;
            printf("USART1_IRQHandler: %0x%0x%0x\n", UART1_RX_Data[0], UART1_RX_Data[1], UART1_RX_Data[2]);
        }
    }
}

void read_registers(){
    static RTC_TimeTypeDef time1;
    //static RTC_DateTypeDef date;
    static int sec;

    RTC_GetTime(RTC_Format_BIN, &time1);
    //RTC_GetDate(RTC_Format_BIN, &date);

    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0xA5);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0x08);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0x41);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0x00);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0x02);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0x4E);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0x20);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0x5E);
    while (!(USART1->SR & USART_SR_TC));

    if(sec != time1.RTC_Seconds){
        sec = time1.RTC_Seconds;
        //printf("Hours-Minutes-Seconds: %d:%d:%d\n", time.RTC_Hours, time.RTC_Minutes, time.RTC_Seconds);
    }
}

void Init_Mcp39f511(){

    read_registers();
}