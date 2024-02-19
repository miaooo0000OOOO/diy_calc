
#ifndef __CALC_GUI_H__
#define __CALC_GUI_H__

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

#define BUTTON_H 48
#define BUTTON_W 40

#define BUTTON_UNPRESSED_COLOR BLACK
#define BUTTON_PRESSED_COLOR BLUE
#define BUTTON_WIREFRAME_COLOR BLACK

    typedef struct BUTTON4048
    {
        u16 left;
        u16 top;
        u16 bg_color_unpressed;
        u16 bg_color_pressed;
        u16 font_color;
        u8 *display_str;
        bool pressed;
        bool edge;    // 按钮状态是否发生改变
        bool display; // 按钮是否显示
    } BUTTON4048;

    BUTTON4048 new_button(u16 left, u16 top, u16 bg_color_unpressed, u16 bg_color_pressed, u16 font_color, u8 *display_str, bool display);
    void draw_button4048(BUTTON4048 *button);
    bool button_pressed(BUTTON4048 *button);

#ifdef __cplusplus
}
#endif
#endif /*__CALC_GUI_H__ */
