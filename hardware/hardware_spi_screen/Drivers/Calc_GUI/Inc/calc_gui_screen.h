
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

#define INPUT_BUFFER_MAX_LEN 40
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

    ShowScreen new_screen();
    void clear_screen(ShowScreen *screen);
    bool screen_del(ShowScreen *screen);
    bool screen_append(ShowScreen *screen, const char *str);
    bool insert_string(char *str, const char *insert, int position, size_t str_size_max);
    bool screen_input_button_pressed(ShowScreen *screen, const BUTTON4048 const *btn);
    bool screen_cursor_left(ShowScreen *screen);
    bool screen_cursor_right(ShowScreen *screen);
    void refresh_cursor(ShowScreen *screen);
    void refresh_input_aera(ShowScreen *screen);
    void refresh_result_area(ShowScreen *screen);
    void refresh_screen(ShowScreen *screen);

#ifdef __cplusplus
}
#endif
#endif /*__CALC_GUI_SCREEN_H__ */
