#include "calc_gui_button.h"

BUTTON4048 new_button(u16 left, u16 top, u16 bg_color_unpressed, u16 bg_color_pressed, u16 font_color, char *display_str, char *input_str, bool display)
{
    BUTTON4048 button;
    button.left = left;
    button.top = top;
    button.pressed = false;
    button.bg_color_unpressed = bg_color_unpressed;
    button.bg_color_pressed = bg_color_pressed;
    button.font_color = font_color;
    button.display_str = display_str;
    button.input_str = input_str;
    button.edge = true;
    button.display = display;
    return button;
}

void draw_button4048(BUTTON4048 *button)
{
    if (!button->display)
        return;
    if (!button->edge)
        return;
    u16 left, top;
    left = button->left;
    top = button->top;
    button->edge = false;

    // 绘制按钮底色
    POINT_COLOR = button->pressed ? button->bg_color_pressed : button->bg_color_unpressed;
    LCD_DrawFillRectangle(left, top, left + BUTTON_W, top + BUTTON_H);

    // 绘制按钮边框
    POINT_COLOR = BLACK;
    LCD_DrawRectangle(left, top, left + BUTTON_W, top + BUTTON_H);

    // 绘制文字
    POINT_COLOR = button->font_color;
    if (strlen(button->display_str) >= 5)
    {
        LCD_ShowString(left, top + 16, 16, button->display_str, 1);
    }
    else
    {
        LCD_ShowString(left + 8, top + 16, 16, button->display_str, 1);
    }
}

bool button_pressed(BUTTON4048 *button)
{
    if (!button->display)
        return;
    u16 left = button->left;
    u16 top = button->top;
    button->edge = false;
    if ((tp_dev.sta) & (1 << 0)) // 判断是否有点触摸？
        if (left <= tp_dev.x[0] && tp_dev.x[0] <= left + BUTTON_W && top <= tp_dev.y[0] && tp_dev.y[0] <= top + BUTTON_H)
        {
            if (!button->pressed)
            {
                button->pressed = true;
                button->edge = true;
            }
            return true;
        }
    if (button->pressed)
    {
        button->pressed = false;
        button->edge = true;
    }
    return false;
}