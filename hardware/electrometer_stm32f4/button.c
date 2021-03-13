#include "delay.h"
#include "button.h"

////////////////////////////////////////////////////////////////////////////////

uint8_t readBtnSetup()
{
    static uint8_t _btn;
    
    _btn = btnCode_noBtnPress;
    if (BUTTON1_Pin)  _btn |= (uint8_t)BUTTON1;
    //if (MENU_Pin)  _btn |= (uint8_t)MENU;
    //if (UP_Pin)    _btn |= (uint8_t)UP;
    //if (DOWN_Pin)  _btn |= (uint8_t)DOWN;
    
    return(_btn);
}

////////////////////////////////////////////////////////////////////////////////
uint8_t readBtnSetupMatrix()
{
    static uint8_t newKey, oldKey, currentKey;

    newKey = readBtnSetup();
    if(newKey != oldKey){
      oldKey = newKey;
      clear_wait_ms();
    } else if (wait_ms_ch(channelBtnMatrix, guard_time))  currentKey = newKey;
    //} else if (wait_ms(10)) currentKey = newKey;

    return(currentKey);
}
////////////////////////////////////////////////////////////////////////////////
//==============================================================================