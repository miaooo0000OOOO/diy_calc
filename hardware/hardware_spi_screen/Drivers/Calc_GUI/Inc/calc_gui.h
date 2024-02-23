
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
#include "string.h"

#include "calc_gui_button.h"
#include "calc_gui_screen.h"

    void init_gui();

    extern BUTTON4048 btn1;
    extern BUTTON4048 btn2;
    extern BUTTON4048 btn3;
    extern BUTTON4048 btn4;
    extern BUTTON4048 btn5;
    extern BUTTON4048 btn6;
    extern BUTTON4048 btn7;
    extern BUTTON4048 btn8;
    extern BUTTON4048 btn9;
    extern BUTTON4048 btn0;
    extern BUTTON4048 btn_add;
    extern BUTTON4048 btn_sub;
    extern BUTTON4048 btn_div;
    extern BUTTON4048 btn_mul;
    extern BUTTON4048 btn_pow;
    extern BUTTON4048 btn_eq;
    extern BUTTON4048 btn_lp;
    extern BUTTON4048 btn_rp;
    extern BUTTON4048 btn_mod;
    extern BUTTON4048 btn_x;
    extern BUTTON4048 btn_dot;

    extern BUTTON4048 btn_char_a;
    extern BUTTON4048 btn_char_b;
    extern BUTTON4048 btn_char_c;
    extern BUTTON4048 btn_char_d;
    extern BUTTON4048 btn_char_e;
    extern BUTTON4048 btn_char_f;

    extern BUTTON4048 btn_del;
    extern BUTTON4048 btn_ac;

    extern BUTTON4048 btn_123;
    extern BUTTON4048 btn_ext;
    extern BUTTON4048 btn_abc;

    extern BUTTON4048 btn_calc;
    extern BUTTON4048 btn_solve;
    extern BUTTON4048 btn_left;
    extern BUTTON4048 btn_right;

    extern BUTTON4048 btn_sqrt;
    extern BUTTON4048 btn_sin;
    extern BUTTON4048 btn_cos;
    extern BUTTON4048 btn_log;
    extern BUTTON4048 btn_ln;
    extern BUTTON4048 btn_tan;

    extern BUTTON4048 *input_buttons[];
    extern u16 input_buttons_len;

    extern BUTTON4048 *cmd_buttons[];
    extern u16 cmd_buttons_len;

    extern BUTTON4048 *all_buttons[];
    extern u16 all_buttons_len;

    // extern u16 i;
    // u16 keybroad_state = 1;
    extern bool refresh_keybroad;

    extern char gui_input_str[INPUT_BUFFER_MAX_LEN];
    extern char gui_temp_str[INPUT_BUFFER_MAX_LEN];
    extern u8 gui_input_str_len;

#ifdef __cplusplus
}
#endif
#endif /*__CALC_GUI_H__ */
