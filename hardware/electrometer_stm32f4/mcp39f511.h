#ifndef MCP39F511
#define MCP39F511

#include "stm32f4xx.h"


#define CURRENT_RMS_OFFSET_VALUE 0x0000006E


#define CALIB_CURRENT_VALUE 0x000003
#define CALIB_VOLTAGE_VALUE 0x0277
#define CALIB_POWER_ACTIVE_VALUE 0x00000251

////////////////////////////////////////////////////////////////////////////////

/* MCP39F511 specifics */
#define FRAME_HEADER 0xA5
#define HEADERBYTE1 0xAB
#define HEADERBYTE2 0xCD
#define HEADERBYTE3 0xEF

/* MCP39F511 responses */
#define RESP_ACK 0x06
#define RESP_NAK 0x15
#define RESP_CSFAIL 0x51

/* Output registers locations */
#define MCP_OUTPUT_REG_INSTRUCTION_POINTER 0x0000
#define MCP_OUTPUT_REG_SYSTEM_STATUS 0x0002
#define MCP_OUTPUT_REG_SYSTEM_VERSION 0x0004
#define MCP_OUTPUT_REG_VOLTAGE_RMS 0x0006
#define MCP_OUTPUT_REG_LINE_FREQUENCY 0x0008
#define MCP_OUTPUT_REG_ANALOG_INPUT_VOLTAGE 0x000A
#define MCP_OUTPUT_REG_POWER_FACTOR 0x000C
#define MCP_OUTPUT_REG_CURRENT_RMS 0x000E
#define MCP_OUTPUT_REG_ACTIVE_POWER 0x0012
#define MCP_OUTPUT_REG_REACTIVE_POWER 0x0016
#define MCP_OUTPUT_REG_APPARENT_POWER 0x001A

/* System status register bits */
#define MCP_SYSTEM_STATUS_EVENT2  (1<<11)
#define MCP_SYSTEM_STATUS_EVENT1 (1<<10)
#define MCP_SYSTEM_STATUS_OVERTEMP (1<<6)
#define MCP_SYSTEM_STATUS_SIGN_PR (1<<5)
#define MCP_SYSTEM_STATUS_SIGN_PA (1<<4)
#define MCP_SYSTEM_STATUS_OVERPOW (1<<3)
#define MCP_SYSTEM_STATUS_OVERCUR (1<<2)
#define MCP_SYSTEM_STATUS_VSURGE (1<<1)
#define MCP_SYSTEM_STATUS_VSAG (1<<0)

#define MCP_OUTPUT_REGISTERS_SIZE sizeof(McpOutputRegisters)
#define MCP_OUTPUT_REGISTERS_START 0x0000
typedef struct __attribute__((packed)) {
	uint16_t instruction_pointer;
	uint16_t system_status;
	uint16_t system_version;
	uint16_t voltage_RMS;
	uint16_t line_frequency;
	uint16_t analog_input_voltage;
	int16_t   power_factor;
	uint32_t current_RMS;
	uint32_t active_power;
	uint32_t reactive_power;
	uint32_t apparent_power;
} McpOutputRegisters;

/* Energy import / export counter register locations */
#define MCP_RECORD_IMP_ACTIVE_ENERGY_COUNTER 0x001E
#define MCP_RECORD_EXP_ACTIVE_ENERGY_COUNTER 0x0026
#define MCP_RECORD_IMP_REACTIVE_ENERGY_COUNTER 0x002E
#define MCP_RECORD_EXP_REACTIVE_ENERGY_COUNTER 0x0036

#define MCP_ENERGY_COUNTER_REGISTERS_SIZE sizeof(McpEnergyCounterRegisters)
#define MCP_ENERGY_COUNTER_REGISTERS_START 0x001E
typedef struct __attribute__((packed)) {
	uint64_t import_active_energy_counter;
	uint64_t export_active_energy_counter;
	uint64_t import_reactive_energy_counter;
	uint64_t export_reactive_energy_counter;
} McpEnergyCounterRegisters;

/* Record register locations */
#define MCP_RECORD_MIN_RECORD_1 0x003E
#define MCP_RECORD_MIN_RECORD_2 0x0042
#define MCP_RECORD_RESERVED_1 0x0046
#define MCP_RECORD_RESERVED_2 0x004A
#define MCP_RECORD_MAX_RECORD_1 0x004E
#define MCP_RECORD_MAX_RECORD_2 0x0052
#define MCP_RECORD_RESERVED_3 0x0056
#define MCP_RECORD_RESERVED_4 0x005A

#define MCP_RECORD_REGISTERS_SIZE sizeof(McpRecordRegisters)
#define MCP_RECORD_REGISTERS_START 0x003E
typedef struct __attribute__((packed)) {
	uint32_t minimum_record_1;
	uint32_t minimum_record_2;
	uint32_t reserved_1;
	uint32_t reserved_2;
	uint32_t maximum_record_1;
	uint32_t maximum_record_2;
	uint32_t reserved_3;
	uint32_t reserved_4;
} McpRecordRegisters;

/* Calibration register locations */
#define MCP_CALIB_REG_DELIMETER 0x005E
#define MCP_CALIB_GAIN_CURRENT_RMS 0x0060
#define MCP_CALIB_GAIN_VOLTAGE_RMS 0x0062
#define MCP_CALIB_GAIN_ACTIVE_POWER 0x0064
#define MCP_CALIB_GAIN_REACTIVE_POWER 0x0066
#define MCP_CALIB_OFFSET_CURRENT_RMS 0x0068
#define MCP_CALIB_OFFSET_ACTIVE_POWER 0x006C
#define MCP_CALIB_OFFSET_REACTIVE_POWER 0x0070
#define MCP_CALIB_DC_OFFSET_CURRENT 0x0074
#define MCP_CALIB_PHASE_COMPENSATION 0x0076
#define MCP_CALIB_APPARENT_POWER_DIV 0x0078

#define MCP_CALIBRATION_REGISTERS_SIZE sizeof(McpCalibrationRegisters)
#define MCP_CALIBRATION_REGISTERS_START 0x005E
typedef struct __attribute__((packed)) {
	uint16_t calibration_register_delimieter;
	uint16_t gain_current_RMS;
	uint16_t gain_voltage_RMS;
	uint16_t gain_active_power;
	uint16_t gain_reactive_power;
	int32_t   offset_current_RMS;
	int32_t   offset_active_power;
	int32_t   offset_reactive_power;
	int16_t   DC_offset_current;
	int16_t   phase_compensation;
	uint16_t apparent_power_divisor;
} McpCalibrationRegisters;

/* System configuration register 1 locations */
#define MCP_CONFIG_1_SYSTEM_CONFIG 0x007A
#define MCP_CONFIG_1_EVENT_CONFIG 0x007E
#define MCP_CONFIG_1_RANGE 0x0082
#define MCP_CONFIG_1_CALIB_CURRENT 0x0086
#define MCP_CONFIG_1_CALIB_VOLTAGE 0x008A
#define MCP_CONFIG_1_CALIB_POWER_ACTIVE 0x008C
#define MCP_CONFIG_1_CALIB_POWER_REACTIVE 0x0090
#define MCP_CONFIG_1_LINE_FREQ_REF 0x0094

/* System config register bits */
#define MCP_SYSTEM_CONFIG_PGA_CH1 (1<<26)
#define MCP_SYSTEM_CONFIG_PGA_CH2 (1<<23)
#define MCP_SYSTEM_CONFIG_VREFCAL (1<<15)
#define MCP_SYSTEM_CONFIG_UART (1<<13)
#define MCP_SYSTEM_CONFIG_ZCD_INV (1<<12)
#define MCP_SYSTEM_CONFIG_ZCD_PULS (1<<11)
#define MCP_SYSTEM_CONFIG_ZCD_OUTPUT_DIS (1<<10)
#define MCP_SYSTEM_CONFIG_SINGLE_WIRE (1<<8)
#define MCP_SYSTEM_CONFIG_TEMPCOMP (1<<7)
#define MCP_SYSTEM_CONFIG_RESET (1<<5)
#define MCP_SYSTEM_CONFIG_SHUTDOWN (1<<3)
#define MCP_SYSTEM_CONFIG_VREFEXT (1<<2)

/* Event config register */
#define MCP_EVENT_CONFIG_OVERTEMP_PIN2 (1<<27)
#define MCP_EVENT_CONFIG_OVERTEMP_PIN1 (1<<26)
#define MCP_EVENT_CONFIG_OVERTEMP_CL (1<<25)
#define MCP_EVENT_CONFIG_OVERTEMP_LA (1<<24)
#define MCP_EVENT_CONFIG_OVERTEMP_ST (1<<23)

#define MCP_EVENT_CONFIG_OVERPOW_PIN2 (1<<22)
#define MCP_EVENT_CONFIG_OVERCUR_PIN2 (1<<21)
#define MCP_EVENT_CONFIG_VSURGE_PIN2 (1<<20)
#define MCP_EVENT_CONFIG_VSAG_PIN2 (1<<19)
#define MCP_EVENT_CONFIG_OVERPOW_PIN1 (1<<18)
#define MCP_EVENT_CONFIG_OVERCUR_PIN1 (1<<17)
#define MCP_EVENT_CONFIG_VSURGE_PIN1 (1<<16)
#define MCP_EVENT_CONFIG_VSAG_PIN1 (1<<15)

#define MCP_EVENT_CONFIG_OVERCUR_CL (1<<10)
#define MCP_EVENT_CONFIG_OVERPOW_CL (1<<9)
#define MCP_EVENT_CONFIG_VSAG_CL (1<<8)
#define MCP_EVENT_CONFIG_VSUR_LA (1<<7)

#define MCP_EVENT_CONFIG_VSAG_LA (1<<6)
#define MCP_EVENT_CONFIG_OVERPOW_LA (1<<5)
#define MCP_EVENT_CONFIG_OVERCUR_LA (1<<4)
#define MCP_EVENT_CONFIG_VSUR_TST (1<<3)
#define MCP_EVENT_CONFIG_VSAG_TST (1<<2)
#define MCP_EVENT_CONFIG_OVERPOW_TST (1<<1)
#define MCP_EVENT_CONFIG_OVERCUR_TST (1<<0)

#define MCP_CONFIG_REGISTERS_1_SIZE sizeof(McpConfigurationRegisters1)
#define MCP_CONFIG_REGISTERS_1_START 0x007A
typedef struct __attribute__((packed)) {
	uint32_t system_configuration;
	uint16_t event_configuration;
	
    uint16_t reserved;
    
    uint8_t  range_voltage;
    uint8_t  range_current;
    uint8_t  range_power;
    uint8_t  range_reserved;

	uint32_t calibration_current;
	uint16_t calibration_voltage;
	uint32_t calibration_power_active;
	uint32_t calibration_power_reactive;
	uint16_t line_frequency_reference;
} McpConfigurationRegisters1;

/* System configuration register 2 locations */
#define MCP_CONFIG_2_RESERVED_1 0x0096
#define MCP_CONFIG_2_RESERVED_2 0x009A
#define MCP_CONFIG_2_ACCUMULATION_INTERVAL 0x009E
#define MCP_CONFIG_2_VOLTAGE_SAG_LIMIT 0x00A0
#define MCP_CONFIG_2_VOLTAGE_SURGE_LIMIT 0x00A2
#define MCP_CONFIG_2_OVERCURRENT_LIMIT 0x00A4
#define MCP_v_2_OVERPOWER_LIMIT 0x00A8

#define MCP_CONFIG_REGISTERS_2_SIZE sizeof(McpConfigurationRegisters2)
#define MCP_CONFIG_REGISTERS_2_START 0x0096
typedef struct __attribute__((packed)) {
	uint32_t reserved_1;
	uint32_t reserved_2;
	uint16_t accumulation_interval_parameter;
	uint16_t voltage_sag_limit;
	uint16_t voltage_surge_limit;
	uint32_t overcurrent_limit;
	uint32_t overpower_limit;
} McpConfigurationRegisters2;

/* Temperature compensation and peripheral control registers */
#define MCP_COMP_PERIPH_TEMP_COMP_FREQUENCY 0x00C6
#define MCP_COMP_PERIPH_TEMP_COMP_CURRENT 0x00C8
#define MCP_COMP_PERIPH_TEMP_COMP_POWER 0x00CA
#define MCP_COMP_PERIPH_REG_AMB_TEMP_REF_VOLTS 0x00CC
#define MCP_COMP_PERIPH_REG_PWM_PERIOD 0x00CE
#define MCP_COMP_PERIPH_REG_PWM_DUTY 0x00D0
#define MCP_COMP_PERIPH_RESERVED_1 0x00D2
#define MCP_COMP_PERIPH_MIN_MAX_PTR_1 0x00D4
#define MCP_COMP_PERIPH_MIN_MAX_PTR_2 0x00D6
#define MCP_COMP_PERIPH_OVERTEMP_LIMIT 0x00D8
#define MCP_COMP_PERIPH_RESERVED_2 0x00DA
#define MCP_COMP_PERIPH_ENERGY_CONTROL 0x00DC
#define MCP_COMP_PERIPH_REG_PWM_CONTROL 0x00DE
#define MCP_COMP_PERIPH_NO_LOAD_THRESHOLD 0x00E0

/* PWM period register */
#define MCP_PWM_PERIOD_PWM_P_POS 8 /* 8-bit value shifted to top of 16-bit register */
#define MCP_PWM_PERIOD_PWM_P_MASK 0xFF
#define MCP_PWM_PERIOD_PRE_POS 0
#define MCP_PWM_PERIOD_PRE_MASK 0x03
#define MCP_PWM_DUTY_UPPER_MASK 0x3FC /* upper 10-bit mask prior to shift */
#define MCP_PWM_DUTY_UPPER_POS 6 /* 10-bit value shifted to top of 16-bit register */
#define MCP_PWM_DUTY_LOWER_MASK 0x03 /* lower 2-bit mask */
#define MCP_PWM_DUTY_LOWER_POS 0

/* PWM control register */
#define MCP_PWM_CONTROL_PWM_CNTRL (1 << 0)

#define MCP_COMP_PERIPH_REGISTERS_SIZE sizeof(McpCompPeriphRegisters)
#define MCP_COMP_PERIPH_REGISTERS_START 0x00C6
typedef struct __attribute__((packed)) {
	uint16_t temperature_compensation_for_frequency;
	uint16_t temperature_compensation_for_current;
	uint16_t temperature_compensation_for_power;
	uint16_t ambient_temperature_reference_voltage;
	uint16_t PWM_period;
	uint16_t PWM_duty_cycle;
	uint16_t reserved1;
	uint16_t min_max_pointer1;
	uint16_t min_max_pointer2;
	uint16_t overtemperature_limit;
	uint16_t reserved2;
	uint16_t energy_control;
	uint16_t PWM_control;
	uint16_t no_load_threshold;
} McpCompPeriphRegisters;

////////////////////////////////////////////////////////////////////////////////

/* MCP39F511 command bytes */
typedef enum {
	MCP_CMD_IDLE,
	MCP_CMD_REGISTER_READ = 0x4E,
	MCP_CMD_REGISTER_WRITE = 0x4D,
	MCP_CMD_SET_ADDRESS_POINTER = 0x41,
	MCP_CMD_SAVE_REGISTERS_TO_FLASH = 0x53,
	MCP_CMD_PAGE_READ_EEPROM = 0x42,
	MCP_CMD_PAGE_WRITE_EEPROM = 0x50,
	MCP_CMD_BULK_ERASE_EEPROM = 0x4F,
	MCP_CMD_AUTO_CALIBRATE_GAIN = 0x5A,
	MCP_CMD_AUTO_CALIBRATE_REACTIVE_GAIN = 0x7A,
	MCP_CMD_AUTO_CALIBRATE_FREQUENCY = 0x76
} mcp39F511_command;

////////////////////////////////////////////////////////////////////////////////

void Init_Mcp39f511();

#endif  // MCP39F511