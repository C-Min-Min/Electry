
#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdint.h>


/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO

#define Data_OK 0

////////////////////////////////////////////////////////////////////////////////

typedef struct programData_s {
    uint8_t		startHour;
    uint8_t		startMinute;
	char		separator1;
    uint8_t		endHour;
    uint8_t		endMinute;
	char		separator2;
    struct {
        uint8_t		b_monday		: 1;
        uint8_t		b_tuesday		: 1;
        uint8_t		b_wednesday		: 1;
        uint8_t		b_thursday		: 1;
        uint8_t		b_friday		: 1;
        uint8_t		b_saturday		: 1;
        uint8_t		b_sunday		: 1;
        uint8_t		b_hollidays		: 1;
    } days_of_the_week;
	char		separator3;
	uint8_t		temperature;		
	char		separator4;
	uint8_t		crc;
	char		separator5;
} programData_t;

////////////////////////////////////////////////////////////////////////////////

extern programData_t programs[];
void init_programs_data (programData_t *programs);
int make_file_data (char *buf, uint8_t mode);
char* convert_uint8_charchar(char *buf, uint8_t *number);
char* convert_hex_charchar(char *buf, uint8_t *number);

////////////////////////////////////////////////////////////////////////////////
#endif /* ! _MAIN_H_ */
