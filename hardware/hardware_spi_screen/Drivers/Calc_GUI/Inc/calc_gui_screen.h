
#ifndef __CALC_GUI_SCREEN_H__
#define __CALC_GUI_SCREEN_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
// MSP3526 libraries
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "touch.h"
#include "GUI.h"
#include "test.h"

#include "stdbool.h"

#include "calc_gui.h"

#define GUI_SHOW_SCREEN_HEIGHT_PIXEL 120
#define GUI_SHOW_SCREEN_WIDTH_PIXEL 480

    typedef struct ShowScreen
    {
        char show_str[INPUT_BUFFER_MAX_LEN];
        char result_str[INPUT_BUFFER_MAX_LEN];
        u8 cursor_position;
        u8 show_str_len;
    } ShowScreen;

#ifdef __cplusplus
}
#endif
#endif /*__CALC_GUI_SCREEN_H__ */
