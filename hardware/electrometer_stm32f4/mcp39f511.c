#include "delay.h"
#include "button.h"
#include <stdbool.h>
#include "mcp39f511.h"
#include "main.h"


////////////////////////////////////////////////////////////////////////////////

//char UART1_RX_Data[35];
uint8_t UART1_RX_Data[35];

////////////////////////////////////////////////////////////////////////////////

void USART1_IRQHandler (void){
    static uint8_t i = 0;
	static uint8_t mode = 0;
	static uint32_t CURRENT_RMS;
	static uint16_t VOLTAGE_RMS;
	static uint32_t ACTIVE_POWER;
	static uint32_t REACTIVE_POWER;
	static uint16_t LINE_FREQUENCY;
	static uint8_t CHECKSUM = 0;

	static RTC_TimeTypeDef start_time;
	static RTC_DateTypeDef start_date;

	static uint8_t t = 0;	//TEST

	GPIO_SetBits(GPIOA, GPIO_Pin_8);	// TEST
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);	// TEST

    if (USART1->SR & USART_CR1_RXNEIE){
	//USART1->SR &= ~USART_CR1_RXNEIE;

		uint8_t n = (uint8_t)USART_ReceiveData(USART1);
		
		if(i == 0){
			if(n == HEADERBYTE1){
				CHECKSUM += n;
				i++;
				mode = 1;
			} else if(n == RESP_ACK){
			} else if(n == RESP_NAK){
			} else if(n == RESP_CSFAIL){
			} else {
			}
		} else if(i == 1){
			if(n == HEADERBYTE2 || mode == 1){
				CHECKSUM += n;
				i++;
			} else {
				i = 0;
				mode = 0;
			}
		} else if(i == 2){
			if(n == HEADERBYTE3 && mode == 1){
				CHECKSUM += n;
				i++;
			} else {
				i = 0;
				mode = 0;
			}
		} else if(i == 3){
			if(mode == 1){
				CHECKSUM += n;
				i++;
				CURRENT_RMS = ((uint32_t)n & 0x000000FF);
			} else {
			}
		} else if(i == 4){
			if(mode == 1){
				CHECKSUM += n;
				i++;
				CURRENT_RMS |= (((uint32_t)n << 8) & 0x0000FF00);
			} else {
			}
		} else if(i == 5){
			if(mode == 1){
				CHECKSUM += n;
				i++;
				CURRENT_RMS |= (((uint32_t)n << 16) & 0x00FF0000);
			} else {
			}
		} else if(i == 6){
			if(mode == 1){
				CHECKSUM += n;
				i++;
				CURRENT_RMS |= (((uint32_t)n << 24) & 0xFF000000);
			} else {
			}
		} else if(i == 7){
			if(mode == 1){
				CHECKSUM += n;
				i++;
				VOLTAGE_RMS = ((uint16_t)n & 0x00FF);
			} else {
			}
		} else if(i == 8){
			if(mode == 1){
				CHECKSUM += n;
				i++;
				VOLTAGE_RMS |= (((uint16_t)n << 8) & 0xFF00);
			} else {
			}
		} else if(i == 9){
			if(mode == 1){
				CHECKSUM += n;
				i++;
				ACTIVE_POWER = ((uint32_t)n & 0x000000FF);
			} else {
			}
		} else if(i == 10){
			if(mode == 1){
				CHECKSUM += n;
				i++;
				ACTIVE_POWER |= (((uint32_t)n << 8) & 0x0000FF00);
			} else {
			}
		} else if(i == 11){
			if(mode == 1){
				CHECKSUM += n;
				i++;
				ACTIVE_POWER |= (((uint32_t)n << 16) & 0x00FF0000);
			} else {
			}
		} else if(i == 12){
			if(mode == 1){
				CHECKSUM += n;
				i++;
				ACTIVE_POWER |= (((uint32_t)n << 24) & 0xFF000000);
			} else {
			}
		} else if(i == 13){
			if(mode == 1){
				CHECKSUM += n;
				i++;
				REACTIVE_POWER = ((uint32_t)n & 0x000000FF);
			} else {
			}
		} else if(i == 14){
			if(mode == 1){
				CHECKSUM += n;
				i++;
				REACTIVE_POWER |= (((uint32_t)n << 8) & 0x0000FF00);
			} else {
			}
		} else if(i == 15){
			if(mode == 1){
				CHECKSUM += n;
				i++;
				REACTIVE_POWER |= (((uint32_t)n << 16) & 0x00FF0000);
			} else {
			}
		} else if(i == 16){
			if(mode == 1){
				CHECKSUM += n;
				i++;
				REACTIVE_POWER |= (((uint32_t)n << 24) & 0xFF000000);
			} else {
			}
		} else if(i == 17){
			if(mode == 1){
				CHECKSUM += n;
				i++;
				LINE_FREQUENCY = ((uint16_t)n & 0x00FF);
			} else {
			}
		} else if(i == 18){
			if(mode == 1){
				CHECKSUM += n;
				i++;
				LINE_FREQUENCY |= (((uint16_t)n << 8) & 0xFF00);
			} else {
			}
		} else if(i == 19){
			if(mode == 1){
				i = 0;
				if(n == CHECKSUM){
					CHECKSUM = 0;
					// OK

					/********************************************************************************/
					/* The code below is responsible for event recognition for electrical consumers */
					/********************************************************************************/
					if(CURRENT_RMS > CURRENT_RMS_OFFSET_VALUE){
						
						if(!event_control.event_present){
							event_control.event_present = 1;	// event_measurement started
							first_empty_element_of_shot_measurement = 0;	// Initialize shot_measurement index
							write_registerd16(MCP_COMP_PERIPH_ENERGY_CONTROL, 0x0001);	// Energy Control enable

							RTC_GetTime(RTC_Format_BCD, &start_time);	// Set event_start time
							event_control.start_time = start_time;
							RTC_GetDate(RTC_Format_BCD, &start_date);	// Set event_start date
							event_control.start_date = start_date;

							shot_measurement[first_empty_element_of_shot_measurement].CURRENT_RMS = CURRENT_RMS;
							shot_measurement[first_empty_element_of_shot_measurement].VOLTAGE_RMS = VOLTAGE_RMS;
							shot_measurement[first_empty_element_of_shot_measurement].ACTIVE_POWER = ACTIVE_POWER;
							shot_measurement[first_empty_element_of_shot_measurement].REACTIVE_POWER = REACTIVE_POWER;
							shot_measurement[first_empty_element_of_shot_measurement].LINE_FREQUENCY = LINE_FREQUENCY;
							first_empty_element_of_shot_measurement++;
						} else {
							if(!event_control.has_a_shot){
								shot_measurement[first_empty_element_of_shot_measurement].CURRENT_RMS = CURRENT_RMS;
								shot_measurement[first_empty_element_of_shot_measurement].VOLTAGE_RMS = VOLTAGE_RMS;
								shot_measurement[first_empty_element_of_shot_measurement].ACTIVE_POWER = ACTIVE_POWER;
								shot_measurement[first_empty_element_of_shot_measurement].REACTIVE_POWER = REACTIVE_POWER;
								shot_measurement[first_empty_element_of_shot_measurement].LINE_FREQUENCY = LINE_FREQUENCY;
								if(++first_empty_element_of_shot_measurement == SHOT_MAX_NUMBER){
									first_empty_element_of_shot_measurement = 0;
									event_control.has_a_shot = 1;
									
									event_control.CURRENT_RMS = CURRENT_RMS;
									event_control.ACTIVE_POWER = ACTIVE_POWER;
									event_control.REACTIVE_POWER = REACTIVE_POWER;
								}
							} else {
//								if(the_same_CURRENT_RMS){
//								}
							}
						}

/*						MCP_measurement[first_empty_element_of_roll_buffer].CURRENT_RMS = CURRENT_RMS;
						MCP_measurement[first_empty_element_of_roll_buffer].VOLTAGE_RMS = VOLTAGE_RMS;
						MCP_measurement[first_empty_element_of_roll_buffer].ACTIVE_POWER = ACTIVE_POWER;
						MCP_measurement[first_empty_element_of_roll_buffer].REACTIVE_POWER = REACTIVE_POWER;
						MCP_measurement[first_empty_element_of_roll_buffer].LINE_FREQUENCY = LINE_FREQUENCY;*/

						MCP_measurement.CURRENT_RMS = CURRENT_RMS;
						MCP_measurement.VOLTAGE_RMS = VOLTAGE_RMS;
						MCP_measurement.ACTIVE_POWER = ACTIVE_POWER;
						MCP_measurement.REACTIVE_POWER = REACTIVE_POWER;
						MCP_measurement.LINE_FREQUENCY = LINE_FREQUENCY;

						if(++first_empty_element_of_roll_buffer == 20) first_empty_element_of_roll_buffer = 0;

						if(first_empty_element_of_roll_buffer == first_element_for_transmit_of_roll_buffer){
							if(first_empty_element_of_roll_buffer){
								first_empty_element_of_roll_buffer--;
							} else first_empty_element_of_roll_buffer = 19;
							// ERR roll_buffer
/*							while((first_empty_element_of_roll_buffer != first_element_for_transmit_of_roll_buffer) && (t == 0)){
								printf("I,V,Pa,Pr: %0d,%0d,%0d,%d\n", \
									MCP_measurement[first_element_for_transmit_of_roll_buffer].CURRENT_RMS, \
									MCP_measurement[first_element_for_transmit_of_roll_buffer].VOLTAGE_RMS, \
									MCP_measurement[first_element_for_transmit_of_roll_buffer].ACTIVE_POWER, \
									MCP_measurement[first_element_for_transmit_of_roll_buffer].REACTIVE_POWER);
								first_element_for_transmit_of_roll_buffer++;
							}
							t = 1;*/
						}
					} else {
						if(event_control.event_present){
						} else {
						}
					}
					/********************************************************************************/
					/*	end  ************************************************************************/
					/********************************************************************************/
				} else {
					// ERR Single-wire transmission frame
					printf("USART1_Rx: ERR\n");	// TEST
				}

			} else {
			}
		} else {
		}
    }

    GPIO_SetBits(GPIOA, GPIO_Pin_8);	// TEST
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);	// TEST
}


void read_registerds(uint16_t address, uint8_t Number_of_Bytes_to_Read){
  	uint8_t CHECKSUM = FRAME_HEADER + 0x08 + MCP_CMD_SET_ADDRESS_POINTER + \
		address + MCP_CMD_REGISTER_READ + Number_of_Bytes_to_Read;

	while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, FRAME_HEADER);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0x08);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, MCP_CMD_SET_ADDRESS_POINTER);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0x00);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, (uint8_t)address);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, MCP_CMD_REGISTER_READ);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, Number_of_Bytes_to_Read);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, CHECKSUM);
    while (!(USART1->SR & USART_SR_TC));
}

void write_registerd16(uint16_t address, int16_t data){
  	uint8_t CHECKSUM;

	while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, FRAME_HEADER);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0x0A);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, MCP_CMD_SET_ADDRESS_POINTER);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0x00);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, (uint8_t)address);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, MCP_CMD_REGISTER_WRITE);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0x02);
    while (!(USART1->SR & USART_SR_TC));
	uint8_t d = (uint8_t)(data & 0x00FF);
	CHECKSUM = FRAME_HEADER + 0x0A + MCP_CMD_SET_ADDRESS_POINTER + (uint8_t)address + MCP_CMD_REGISTER_WRITE + 0x02 + d;
    USART_SendData(USART1, d);
    while (!(USART1->SR & USART_SR_TC));
	d = (uint8_t)((data >> 8) & 0x00FF);
	CHECKSUM = CHECKSUM + d;
    USART_SendData(USART1, d);
    while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, CHECKSUM);
}

void write_registerd32(uint16_t address, int32_t data){
  	uint8_t CHECKSUM;
  	uint32_t registerd;

	while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, FRAME_HEADER);

	while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0x0C);

	while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, MCP_CMD_SET_ADDRESS_POINTER);

	while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0x00);

	while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, (uint8_t)address);

	while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, MCP_CMD_REGISTER_WRITE);
 
	while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0x04);

	while (!(USART1->SR & USART_SR_TC));
	registerd = data;
	uint8_t d = (uint8_t)(registerd & 0x000000FF);
	CHECKSUM = FRAME_HEADER + 0x0C + MCP_CMD_SET_ADDRESS_POINTER + (uint8_t)address + MCP_CMD_REGISTER_WRITE + 0x04 + d;
    USART_SendData(USART1, d);

	while (!(USART1->SR & USART_SR_TC));
	registerd = data;
	d = (uint8_t)((registerd >> 8) & 0x000000FF);
	CHECKSUM = CHECKSUM + d;
    USART_SendData(USART1, d);

	while (!(USART1->SR & USART_SR_TC));
	registerd = data;
	d = (uint8_t)((registerd >> 16) & 0x000000FF);
	CHECKSUM = CHECKSUM + d;
    USART_SendData(USART1, d);

	while (!(USART1->SR & USART_SR_TC));
	registerd = data;
	d = (uint8_t)((registerd >> 24) & 0x000000FF);
	CHECKSUM = CHECKSUM + d;
    USART_SendData(USART1, d);

	while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, CHECKSUM);
}

void Save_Registers_To_Flash(){

	while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, FRAME_HEADER);

	while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0x04);

	while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, MCP_CMD_SAVE_REGISTERS_TO_FLASH);

	while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0xFC);
}

void Auto_Calibrate_Gain(){

	while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, FRAME_HEADER);

	while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0x04);

	while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, MCP_CMD_AUTO_CALIBRATE_GAIN);

	while (!(USART1->SR & USART_SR_TC));
    USART_SendData(USART1, 0x03);
}


void Init_Mcp39f511(){
	bool flag_calibration = 0;

	/* Read BUTTON after reset for the single-point CALIBRATION */
	uint8_t r = readBtnSetupMatrix();	// initialize readBtnSetupMatrix()
	while(!wait_ms_ch(channelButton, guard_time + 1));	// waiting for readiness
	if(readBtnSetupMatrix() == BUTTON1){
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);	// Led on if the button is pressed
		while(readBtnSetupMatrix() == BUTTON1);	// Wait while the button is released
		while(readBtnSetupMatrix() != BUTTON1){	// Wait while the button is pressed
			if(wait_ms_ch(channelLed, 1000)) GPIO_ToggleBits(GPIOA, GPIO_Pin_1);	// the LED flashes
		}
		flag_calibration = true;	// to perform calibration
		GPIO_SetBits(GPIOA, GPIO_Pin_1);	// Led off & exit
	} else {
		GPIO_SetBits(GPIOA, GPIO_Pin_1);	// else led off
	}

	write_registerd32(MCP_CONFIG_1_SYSTEM_CONFIG, 0x03000000);	// Single-wire transmission off
	(void) wait_ms_ch(channelTime2, 1);	// Specific for while() only
	while(!wait_ms_ch(channelTime2, 100));

	/* Only for FIRST (after factory) inicialization for MCP39F511 */
#if(0)
	write_registerd32(MCP_CONFIG_1_RANGE, 0x1B120B12);	// Set Range
	while(!wait_ms_ch(channelTime2, 10));

	write_registerd32(MCP_CONFIG_1_SYSTEM_CONFIG, 0x03000000);	// Single-wire transmission off
	while(!wait_ms_ch(channelTime2, 10));

	write_registerd16(MCP_CALIB_GAIN_VOLTAGE_RMS, 0xB554);	// Set GANE V
	while(!wait_ms_ch(channelTime2, 10));

	write_registerd16(MCP_CALIB_GAIN_ACTIVE_POWER, 0x88A9);	// Set GANE Pa
	while(!wait_ms_ch(channelTime2, 10));

	//write_registerd32(MCP_CALIB_OFFSET_CURRENT_RMS, 0x00000006);	// Set OFFSET I
	//while(!wait_ms_ch(channelTime2, 10));

	//write_registerd32(MCP_CALIB_OFFSET_ACTIVE_POWER, 0x00000000);	// Set OFFSET Pa
	//while(!wait_ms_ch(channelTime2, 10));

	//write_registerd32(MCP_CALIB_OFFSET_REACTIVE_POWER, 0x00000000);	// Set OFFSET Pr
	//while(!wait_ms_ch(channelTime2, 10));

	Save_Registers_To_Flash();	// Save all registers
	while(!wait_ms_ch(channelTime2, 100));
#endif

	/* Calibration subroutine */
	if(flag_calibration){
		write_registerd32(MCP_CONFIG_1_CALIB_CURRENT, CALIB_CURRENT_VALUE);
		while(!wait_ms_ch(channelTime2, 10));

		write_registerd16(MCP_CONFIG_1_CALIB_VOLTAGE, CALIB_VOLTAGE_VALUE);
		while(!wait_ms_ch(channelTime2, 10));

		write_registerd32(MCP_CONFIG_1_CALIB_POWER_ACTIVE, CALIB_POWER_ACTIVE_VALUE);
		while(!wait_ms_ch(channelTime2, 10));

		Auto_Calibrate_Gain();
		while(!wait_ms_ch(channelTime2, 100));
	}

	/* Only if we want to read some registers of MCP39F511 */
#if(1)
	read_registerds(MCP_CONFIG_1_RANGE, 4);
	while(!wait_ms_ch(channelTime2, 10));
	read_registerds(MCP_OUTPUT_REG_CURRENT_RMS, 4);		// CURRENT_RMS value
	while(!wait_ms_ch(channelTime2, 10));
	read_registerds(MCP_OUTPUT_REG_VOLTAGE_RMS, 2);		// VOLTAGE_RMS value
	while(!wait_ms_ch(channelTime2, 10));
	read_registerds(MCP_OUTPUT_REG_ACTIVE_POWER, 4);	// ACTIVE_POWER value
	while(!wait_ms_ch(channelTime2, 10));
	read_registerds(MCP_CALIB_GAIN_CURRENT_RMS, 2);		// GANE I value
	while(!wait_ms_ch(channelTime2, 10));
	read_registerds(MCP_CALIB_GAIN_VOLTAGE_RMS, 2);		// GANE V value
	while(!wait_ms_ch(channelTime2, 10));
	read_registerds(MCP_CALIB_GAIN_ACTIVE_POWER, 2);	// GANE Pa value
	while(!wait_ms_ch(channelTime2, 10));
	read_registerds(MCP_CALIB_OFFSET_CURRENT_RMS, 4);	// OFFSET CURRENT_RMS value
	while(!wait_ms_ch(channelTime2, 10));
	read_registerds(MCP_CALIB_OFFSET_ACTIVE_POWER, 4);	// OFFSET ACTIVE_POWER value
	while(!wait_ms_ch(channelTime2, 10));
	read_registerds(MCP_CALIB_OFFSET_REACTIVE_POWER, 4);// OFFSET REACTIVE_POWER value
	while(!wait_ms_ch(channelTime2, 100));
#endif

	write_registerd16(MCP_CONFIG_2_ACCUMULATION_INTERVAL, 0x0000);	// N number = 0.	The accumulation interval is defined as
	while(!wait_ms_ch(channelTime2, 10));							//							   an 2^N number of line cycles

	write_registerd32(MCP_CONFIG_1_SYSTEM_CONFIG, 0x03000100);	// Single-wire transmission on
	while(!wait_ms_ch(channelTime2, 10));
}