#include "esp32_Rx_Tx.h"
#include "mcp39f511.h"
#include "delay.h"
#include "main.h"


/////////////////////////////// INCLUDES ///////////////////////////////////////

#define MAX_VAL 4000
#define MIN_VAL 50
#define KOEF_VAL (MAX_VAL-MIN_VAL)/100

////////////////////////////// DEFINITIONS /////////////////////////////////////

void Init_RCC();
void Init_RTC();
void Init_USART1();
void Init_USART2();
void Init_GPIO();
void ConfigureADC();
void ConfigureDAC();
void ConfigureTIM10();

event_measurement_struct_t event_measurement[100];	// Roll buffer for send to ESP32
MCP_measurement_struct_t shot_measurement[100];	// 100 measurements (N = 0) for started event
MCP_measurement_struct_t MCP_measurement;	// For measurements
event_control_struct_t event_control;

uint8_t first_empty_element_of_roll_buffer = 0;	// For event_measurement
uint8_t first_element_for_transmit_of_roll_buffer = 0;	// To ESP32 (STM32 - USART2)
uint8_t first_empty_element_of_shot_measurement = 0;	// For shot_measurement

////////////////////////////////////////////////////////////////////////////////

void Init_RCC(){
    RCC_HSICmd(ENABLE);
    RCC_PLLCmd(DISABLE);
    RCC_PLLConfig(RCC_PLLSource_HSE, 0x04, 336, 0x04, 0x0F);
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    RCC_HCLKConfig(RCC_SYSCLK_Div4);
    RCC_PCLK1Config(RCC_HCLK_Div2);
    RCC_PCLK2Config(RCC_HCLK_Div1);
    
    RCC_ClocksTypeDef rcc_freq;
    RCC_GetClocksFreq(&rcc_freq);
    printf("\nRCC INITIALIZATION: \nSystem clock freq - %d\nHCLK clock freq - %d\nPCLK1 clock freq - %d\nPCLK2 clock freq - %d\n", rcc_freq.SYSCLK_Frequency, rcc_freq.HCLK_Frequency, rcc_freq.PCLK1_Frequency, rcc_freq.PCLK2_Frequency);
}

void SysTickInit(uint16_t frequency){
    RCC_ClocksTypeDef RCC_Clocks;

    RCC_GetClocksFreq(&RCC_Clocks);
    (void) SysTick_Config(RCC_Clocks.HCLK_Frequency / frequency);
}

void Init_RTC(){
    ///INITIALIZE RTC_Init structure and error variable///
    RTC_InitTypeDef rtc_struct;
    ErrorStatus possible_error;    

    ///Enable Power Control Unit to enable shadow register reading///
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);

    ///Make RTC Reset, Smooth Calibration, Bypass of LSI through Smooth Calibration Unit, Enable LSI, make LSI check///
    RCC_RTCCLKCmd(DISABLE);
    RCC_RTCCLKCmd(ENABLE);
    RTC_SmoothCalibConfig(RTC_SmoothCalibPeriod_8sec, RTC_SmoothCalibPlusPulses_Set, 0x07F);
    RCC_LSEConfig(RCC_LSE_ON);
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

    ///Configure RTC to use LSE, fill RTC_Init structure///
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    rtc_struct.RTC_AsynchPrediv = (uint32_t)0x7F;
    rtc_struct.RTC_SynchPrediv = (uint32_t)0xFF;
    rtc_struct.RTC_HourFormat = RTC_HourFormat_24;

    ///ENABLE RTC Unit and wait for synchronisation///
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForSynchro();

    ///Initialize RTC Unit and check init status///
    possible_error = RTC_Init(&rtc_struct);
    printf("\n\nStatus in Initialization: %d", possible_error);   

    ///Set RTC Date and check error status///
    RTC_DateTypeDef rtc_date;
    rtc_date.RTC_WeekDay = RTC_Weekday_Monday;
    rtc_date.RTC_Month = RTC_Month_March;
    rtc_date.RTC_Date = 1;
    rtc_date.RTC_Year = 51;
    possible_error = RTC_SetDate(RTC_Format_BIN, &rtc_date);
    printf("\nStatus in setting date: %d", possible_error);

    ///Set RTC Time and check error status///
    RTC_TimeTypeDef rtc_time;
    rtc_time.RTC_Hours = 0;
    rtc_time.RTC_Minutes = 0;
    rtc_time.RTC_Seconds = 0;
    rtc_time.RTC_H12 = RTC_H12_AM;
    possible_error = RTC_SetTime(RTC_Format_BIN, &rtc_time);
    printf("\nStatus in setting time: %d\n\n", possible_error);
}

void Init_USART1(){
    GPIO_InitTypeDef gpio;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    gpio.GPIO_Pin = GPIO_Pin_9;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gpio);
    
    gpio.GPIO_Pin = GPIO_Pin_10;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gpio);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    USART_InitTypeDef usart1_config;
    usart1_config.USART_BaudRate = 115200;
    usart1_config.USART_WordLength = USART_WordLength_8b;
    usart1_config.USART_StopBits = USART_StopBits_1;
    usart1_config.USART_Parity = USART_Parity_No;
    usart1_config.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart1_config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &usart1_config);
    
    //USART_OneBitMethodCmd(USART1, ENABLE);
    USART_OverSampling8Cmd(USART1, DISABLE);

    NVIC_InitTypeDef nvic_init;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    nvic_init.NVIC_IRQChannel = USART1_IRQn;
    nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init.NVIC_IRQChannelSubPriority = 0;
    nvic_init.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART1, ENABLE);
}

void Init_USART2(){
    GPIO_InitTypeDef gpio;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

    gpio.GPIO_Pin = GPIO_Pin_2;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gpio);

    gpio.GPIO_Pin = GPIO_Pin_3;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gpio);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    USART_SetPrescaler(USART2, 0x04);
    USART_OverSampling8Cmd(USART2, ENABLE);

    USART_InitTypeDef usart2_config;
    usart2_config.USART_BaudRate = 115200;
    usart2_config.USART_WordLength = USART_WordLength_8b;
    usart2_config.USART_StopBits = USART_StopBits_1;
    usart2_config.USART_Parity = USART_Parity_No;
    usart2_config.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    usart2_config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2, &usart2_config);

    NVIC_InitTypeDef nvic_init;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    nvic_init.NVIC_IRQChannel = USART2_IRQn;
    nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init.NVIC_IRQChannelSubPriority = 0;
    nvic_init.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART2, ENABLE);
}

void Init_GPIO(){
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef gpio;

    gpio.GPIO_Pin = GPIO_Pin_0;
    gpio.GPIO_Mode = GPIO_Mode_IN;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &gpio);

    gpio.GPIO_Pin = GPIO_Pin_1;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gpio);

    gpio.GPIO_Pin = GPIO_Pin_8;                 // For "PA8 Test"
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOA, &gpio);

    gpio.GPIO_Pin = GPIO_Pin_5;                 // For "PC8 Test"
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &gpio);
}

void MCU_Init(){
    Init_RCC();
	SysTickInit(1000);
    if(!RTC_FLAG_INITS) Init_RTC();
    Init_USART1();
    Init_USART2();
    Init_GPIO();
    Init_Mcp39f511();
    //ConfigureADC();
    //ConfigureDAC();
    //ConfigureTIM10();
}

#if(0)
void ConfigureADC(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);////LEROLEROLEROLEROLERO
    ///
    GPIO_InitTypeDef adc_gpio;
    adc_gpio.GPIO_Pin = GPIO_Pin_6;
    adc_gpio.GPIO_Mode = GPIO_Mode_AN;
    adc_gpio.GPIO_Speed = GPIO_Speed_100MHz;
    adc_gpio.GPIO_OType = GPIO_OType_PP;
    adc_gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &adc_gpio);
    ///
    ADC_InitTypeDef adc_init;
    adc_init.ADC_Resolution = ADC_Resolution_12b;
    adc_init.ADC_ScanConvMode = DISABLE;
    adc_init.ADC_ContinuousConvMode = /*DISABLE*/ENABLE;
    adc_init.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;//ADC_ExternalTrigConvEdge_None;//ADC_ExternalTrigConvEdge_Rising;
    adc_init.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;
    adc_init.ADC_DataAlign = ADC_DataAlign_Right;
    adc_init.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &adc_init);
    ///
    ADC_Cmd(ADC1, ENABLE);
    ///
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_112Cycles);
    ///
    ADC_ContinuousModeCmd(ADC1, ENABLE);
    /*ADC_DiscModeChannelCountConfig(ADC1, 5);
    ADC_DiscModeCmd(ADC1, ENABLE);*/  //Discontinuous mode
    ADC_SoftwareStartConv(ADC1);
}

void ConfigureDAC(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    
    GPIO_InitTypeDef dac_gpio;
    dac_gpio.GPIO_Pin = GPIO_Pin_4;
    dac_gpio.GPIO_Mode = GPIO_Mode_AN;
    dac_gpio.GPIO_Speed = GPIO_Speed_100MHz;
    dac_gpio.GPIO_OType = GPIO_OType_PP;
    dac_gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &dac_gpio);

    DAC_InitTypeDef dac_init;
    dac_init.DAC_Trigger = DAC_Trigger_None;
    dac_init.DAC_WaveGeneration = DAC_WaveGeneration_None;
    dac_init.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
    dac_init.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_1, &dac_init);
    
    DAC_SetChannel1Data(DAC_Align_12b_R, 400);
    DAC_Cmd(DAC_Channel_1, ENABLE);
}

void ConfigureTIM10(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);

    TIM_TimeBaseInitTypeDef tim_init;
    NVIC_InitTypeDef nvic_init;

    tim_init.TIM_Prescaler = 0xFFFF;
    tim_init.TIM_CounterMode = TIM_CounterMode_Up;
    tim_init.TIM_Period = 0x0000;
    tim_init.TIM_ClockDivision = TIM_CKD_DIV4;
    tim_init.TIM_RepetitionCounter = 0x0000;

    TIM_TimeBaseInit(TIM10, &tim_init);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    nvic_init.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
    nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init.NVIC_IRQChannelSubPriority = 0;
    nvic_init.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init);

    TIM_ITConfig(TIM10, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM10, ENABLE);
}

void TIM1_UP_TIM10_IRQHandler(){
  static int counter = 0;
  int value = ADC_GetConversionValue(ADC1);
  ++counter;
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 750 + ((int)(value/((MAX_VAL-MIN_VAL)/100)))*8);
  if(counter == 0 || counter % (int)(value/250 + 1) == 0) GPIO_ResetBits(GPIOA, GPIO_Pin_1);
  else  GPIO_SetBits(GPIOA, GPIO_Pin_1);

  TIM_ClearFlag(TIM10, 0x0);
}
#endif



void main(){
    MCU_Init();
	
    //RTC_TimeTypeDef time1;
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;

    event_control.event_present = 0;
    event_control.has_a_shot = 0;
    event_control.id_consumer = 0;
    event_control.event_created = 0;

    uint8_t transmit_to_esp32_flag = 10;	// TEST

/*	while(!wait_ms_ch(channelTime1, 1000));
	RTC_GetTime(RTC_Format_BIN, &time1);
	//printf("Time is: %d:%d:%d\n", time1.RTC_Hours, time1.RTC_Minutes, time1.RTC_Seconds);
	while(!wait_ms_ch(channelTime1, 1000));*/

    while(1){
        if(READ_RxSTATUS_FLAG){
              printf("\nDATA: %s\n", rx_data);
              strcpy(rx_data, "");
              READ_RxSTATUS_FLAG = ~READ_RxSTATUS_FLAG;
        }
        if(transmit_to_esp32_flag){
            event_measurement[0].id_consumer = 9;
            event_measurement[0].id_event = 0;
            event_measurement[0].id_measurement = 0;
            strcat(event_measurement[0].timestamp_time_start, "20210309131315");
            event_measurement[0].power = 9000;

            Fill_Tx_buffer(&event_measurement[0]);
            Transmit_to_esp32();
            transmit_to_esp32_flag--;
            delay_ms(1000);
            strcpy(event_measurement[0].timestamp_time_start, "");
        }
    }
}