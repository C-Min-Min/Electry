#ifndef BUTTON
#define BUTTON

#include "stm32f4xx.h"
////////////////////////////////////////////////////////////////////////////////

#define guard_time 10
#define BUTTON1_Pin GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)

typedef struct btnData_s {
    uint8_t    newCode;
    uint8_t    oldCode;
    uint8_t    currentCode;
    struct {
        uint8_t    b_btnIsUp           : 1;
        uint8_t    b_btnIsDown         : 1;
        uint8_t    b_btnIsPressed      : 1;
        uint8_t    b_btnIsClick        : 1;
        uint8_t    b_btnEdgeDetected   : 1;
        uint8_t    b_btnEdgeUp2Down    : 1;
        uint8_t    b_btnEdgeDown2Up    : 1;
        uint8_t    b_one_time          : 1;
    } flags;
} btnData_t;

typedef enum buttonSpecialCode_e {
    btnCode_noBtnPress_internal = 0,
    //btnCode_noBtnPress = 0xFF
    btnCode_noBtnPress = 0x00  // !! new value
} buttonSpecialCode_t;

typedef enum buttonSetupCode_e {
    BUTTON1	= 0x01, //    btnCodeSET1 = 0b00000001
    MENU 	= 0x01, //    btnCodeSET1 = 0b00000001
    UP   	= 0x02, //    btnCodeSET2 = 0b00000010
    DOWN 	= 0x04  //    btnCodeSET3 = 0b00000100
} buttonSetupCode_t;

////////////////////////////////////////////////////////////////////////////////

uint8_t readBtnSetupMatrix();

////////////////////////////////////////////////////////////////////////////////


#endif	// BUTTON