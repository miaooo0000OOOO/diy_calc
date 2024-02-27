#include "calc_gui.h"

BUTTON4048 btn1;
BUTTON4048 btn2;
BUTTON4048 btn3;
BUTTON4048 btn4;
BUTTON4048 btn5;
BUTTON4048 btn6;
BUTTON4048 btn7;
BUTTON4048 btn8;
BUTTON4048 btn9;
BUTTON4048 btn0;
BUTTON4048 btn_e;
BUTTON4048 btn_pi;
BUTTON4048 btn_add;
BUTTON4048 btn_sub;
BUTTON4048 btn_div;
BUTTON4048 btn_mul;
BUTTON4048 btn_pow;
BUTTON4048 btn_eq;
BUTTON4048 btn_lp;
BUTTON4048 btn_rp;
BUTTON4048 btn_mod;
BUTTON4048 btn_x;
BUTTON4048 btn_dot;

BUTTON4048 btn_char_a;
BUTTON4048 btn_char_b;
BUTTON4048 btn_char_c;
BUTTON4048 btn_char_d;
BUTTON4048 btn_char_e;
BUTTON4048 btn_char_f;

BUTTON4048 btn_del;
BUTTON4048 btn_ac;

BUTTON4048 btn_123;
BUTTON4048 btn_abc;

BUTTON4048 btn_calc;
BUTTON4048 btn_solve;
BUTTON4048 btn_left;
BUTTON4048 btn_right;

BUTTON4048 btn_sqrt;
BUTTON4048 btn_sin;
BUTTON4048 btn_cos;
BUTTON4048 btn_log;
BUTTON4048 btn_ln;
BUTTON4048 btn_tan;

BUTTON4048 *input_buttons[] = {&btn1, &btn2, &btn3, &btn4, &btn5, &btn6, &btn7, &btn8, &btn9, &btn0, &btn_e, &btn_pi,
                               &btn_add, &btn_sub, &btn_mul, &btn_div, &btn_pow, &btn_eq, &btn_lp, &btn_rp, &btn_mod, &btn_x, &btn_dot,
                               &btn_char_a, &btn_char_b, &btn_char_c, &btn_char_d, &btn_char_e, &btn_char_f,
                               &btn_sqrt, &btn_sin, &btn_cos, &btn_log, &btn_ln, &btn_tan};
u16 input_buttons_len;

BUTTON4048 *cmd_buttons[] = {&btn_del, &btn_ac, &btn_123, &btn_abc, &btn_calc, &btn_solve, &btn_left, &btn_right};
u16 cmd_buttons_len;

BUTTON4048 *all_buttons[] = {&btn1, &btn2, &btn3, &btn4, &btn5, &btn6, &btn7, &btn8, &btn9, &btn0, &btn_e, &btn_pi,
                             &btn_char_a, &btn_char_b, &btn_char_c, &btn_char_d, &btn_char_e, &btn_char_f,
                             &btn_add, &btn_sub, &btn_mul, &btn_div, &btn_pow, &btn_eq, &btn_lp, &btn_rp, &btn_mod, &btn_x, &btn_dot,
                             &btn_del, &btn_ac, &btn_123, &btn_abc, &btn_calc, &btn_solve, &btn_left, &btn_right,
                             &btn_sqrt, &btn_sin, &btn_cos, &btn_log, &btn_ln, &btn_tan};
u16 all_buttons_len;

// u16 i;
// u16 keybroad_state = 1;
bool refresh_keybroad = true;

char gui_input_str[40] = {0};
char gui_temp_str[40] __attribute__((aligned(8))) = {0};
u8 gui_input_str_len = 0;

void init_gui()
{
    btn1 = new_button(BUTTON_POS(0, 6), BLUE, GRAY, WHITE, "1", "1", true);
    btn2 = new_button(BUTTON_POS(1, 6), BLUE, GRAY, WHITE, "2", "2", true);
    btn3 = new_button(BUTTON_POS(2, 6), BLUE, GRAY, WHITE, "3", "3", true);
    btn4 = new_button(BUTTON_POS(0, 5), BLUE, GRAY, WHITE, "4", "4", true);
    btn5 = new_button(BUTTON_POS(1, 5), BLUE, GRAY, WHITE, "5", "5", true);
    btn6 = new_button(BUTTON_POS(2, 5), BLUE, GRAY, WHITE, "6", "6", true);
    btn7 = new_button(BUTTON_POS(0, 4), BLUE, GRAY, WHITE, "7", "7", true);
    btn8 = new_button(BUTTON_POS(1, 4), BLUE, GRAY, WHITE, "8", "8", true);
    btn9 = new_button(BUTTON_POS(2, 4), BLUE, GRAY, WHITE, "9", "9", true);
    btn0 = new_button(BUTTON_POS(0, 7), BLUE, GRAY, WHITE, "0", "0", true);
    btn_e = new_button(BUTTON_POS(3, 4), BLUE, GRAY, WHITE, "e", "e", true);
    btn_pi = new_button(BUTTON_POS(3, 5), BLUE, GRAY, WHITE, "pi", "pi", true);

    btn_add = new_button(BUTTON_POS(6, 6), BLUE, GRAY, WHITE, "+", "+", true);
    btn_sub = new_button(BUTTON_POS(7, 6), BLUE, GRAY, WHITE, "-", "-", true);
    btn_div = new_button(BUTTON_POS(7, 5), BLUE, GRAY, WHITE, "/", "/", true);
    btn_mul = new_button(BUTTON_POS(6, 5), BLUE, GRAY, WHITE, "*", "*", true);
    btn_pow = new_button(BUTTON_POS(5, 2), GRAYBLUE, GRAY, WHITE, "x`", "^", true);
    btn_eq = new_button(BUTTON_POS(1, 3), GRAYBLUE, GRAY, WHITE, "=", "=", true);
    btn_lp = new_button(BUTTON_POS(2, 3), GRAYBLUE, GRAY, WHITE, "(", "(", true);
    btn_rp = new_button(BUTTON_POS(3, 3), GRAYBLUE, GRAY, WHITE, ")", ")", true);
    btn_mod = new_button(BUTTON_POS(4, 3), GRAYBLUE, GRAY, WHITE, "mod", "%", true);
    btn_x = new_button(BUTTON_POS(2, 7), BLUE, GRAY, WHITE, "x", "x", true);
    btn_dot = new_button(BUTTON_POS(1, 7), BLUE, GRAY, WHITE, ".", ".", true);

    btn_del = new_button(BUTTON_POS(6, 4), BLUE, GRAY, WHITE, "DEL", NULL, true);
    btn_ac = new_button(BUTTON_POS(7, 4), BLUE, GRAY, WHITE, "AC", NULL, true);

    btn_123 = new_button(BUTTON_POS(1, 2), GRAYBLUE, GRAY, WHITE, "123", NULL, true);
    btn_abc = new_button(BUTTON_POS(0, 2), GRAYBLUE, GRAY, WHITE, "ABC", NULL, true);

    btn_calc = new_button(BUTTON_POS(7, 7), BLUE, GRAY, WHITE, "=", NULL, true);
    btn_solve = new_button(BUTTON_POS(0, 3), GRAYBLUE, GRAY, WHITE, "SOLVE", NULL, true);
    btn_left = new_button(BUTTON_POS(2, 2), GRAYBLUE, GRAY, WHITE, "<", NULL, true);
    btn_right = new_button(BUTTON_POS(3, 2), GRAYBLUE, GRAY, WHITE, ">", NULL, true);

    btn_sqrt = new_button(BUTTON_POS(4, 2), GRAYBLUE, GRAY, WHITE, "sqrt", "sqrt(", true);
    btn_sin = new_button(BUTTON_POS(5, 3), GRAYBLUE, GRAY, WHITE, "sin", "sin(", true);
    btn_cos = new_button(BUTTON_POS(6, 3), GRAYBLUE, GRAY, WHITE, "cos", "cos(", true);
    btn_log = new_button(BUTTON_POS(6, 2), GRAYBLUE, GRAY, WHITE, "log", "log(", true);
    btn_ln = new_button(BUTTON_POS(7, 2), GRAYBLUE, GRAY, WHITE, "ln", "ln(", true);
    btn_tan = new_button(BUTTON_POS(7, 3), GRAYBLUE, GRAY, WHITE, "tan", "tan(", true);

    btn_char_a = new_button(BUTTON_POS(0, 1), GRAYBLUE, GRAY, WHITE, "A", "A", true);
    btn_char_b = new_button(BUTTON_POS(1, 1), GRAYBLUE, GRAY, WHITE, "B", "B", true);
    btn_char_c = new_button(BUTTON_POS(2, 1), GRAYBLUE, GRAY, WHITE, "C", "C", true);
    btn_char_d = new_button(BUTTON_POS(3, 1), GRAYBLUE, GRAY, WHITE, "D", "D", true);
    btn_char_e = new_button(BUTTON_POS(4, 1), GRAYBLUE, GRAY, WHITE, "E", "E", true);
    btn_char_f = new_button(BUTTON_POS(5, 1), GRAYBLUE, GRAY, WHITE, "F", "F", true);

    // *input_buttons[] = {&btn1, &btn2, &btn3, &btn4, &btn5, &btn6, &btn7, &btn8, &btn9, &btn0,
    //                    &btn_add, &btn_sub, &btn_mul, &btn_div, &btn_pow, &btn_eq, &btn_lp, &btn_rp, &btn_mod, &btn_x};
    input_buttons_len = sizeof(input_buttons) / sizeof(input_buttons[0]);

    // *cmd_buttons[] = {&btn_del, &btn_ac, &btn_123, &btn_abc, &btn_calc};
    cmd_buttons_len = sizeof(cmd_buttons) / sizeof(cmd_buttons[0]);

    // *all_buttons[] = {&btn1, &btn2, &btn3, &btn4, &btn5, &btn6, &btn7, &btn8, &btn9, &btn0,
    //                   &btn_add, &btn_sub, &btn_mul, &btn_div, &btn_pow, &btn_eq, &btn_lp, &btn_rp, &btn_mod, &btn_x,
    //                   &btn_del, &btn_ac, &btn_123, &btn_abc, &btn_calc,
    //                   &btn_sqrt, &btn_sin, &btn_cos};
    all_buttons_len = sizeof(all_buttons) / sizeof(all_buttons[0]);
    // u16 i = 0;
    // u16 keybroad_state = 1;
    // bool refresh_keybroad = true;

    // u8 gui_input_str[40] = {0};
    // u8 gui_temp_str[40] = {0};
    // u8 gui_input_str_len = 0;
}

void input_button_pressed_callback(BUTTON4048 *btn)
{
    u8 btn_input_str_len = strlen(btn->input_str);
    if (gui_input_str_len + btn_input_str_len >= INPUT_BUFFER_MAX_LEN - 1)
    {
        return;
    }
    strcat(gui_input_str, btn->input_str);
    gui_input_str_len += btn_input_str_len;
}