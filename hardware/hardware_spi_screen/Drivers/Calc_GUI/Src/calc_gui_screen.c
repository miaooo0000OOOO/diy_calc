#include "calc_gui_screen.h"

ShowScreen new_screen()
{
    ShowScreen screen;
    screen.cursor_position = 0;
    memset(screen.show_str, '\0', sizeof(screen.show_str));
    memset(screen.result_str, '\0', sizeof(screen.result_str));
    screen.show_str_len = 0;
    screen.cursor_position = 0;
    return screen;
}

void clear_screen(ShowScreen *screen)
{
    memset(screen->show_str, '\0', sizeof(screen->show_str));
    memset(screen->result_str, '\0', sizeof(screen->result_str));
    screen->show_str_len = 0;
    screen->cursor_position = 0;
}

bool screen_del(ShowScreen *screen)
{
    if (screen->cursor_position <= 0)
    {
        return false;
    }
    strcpy(screen->show_str + screen->cursor_position - 1, screen->show_str + screen->cursor_position);
    // screen->show_str[screen->show_str_len - 1] = '\0';
    screen->show_str_len -= 1;
    screen->cursor_position -= 1;
    return true;
}

bool screen_append(ShowScreen *screen, const char *str)
{
    bool res = insert_string(screen->show_str, str, screen->cursor_position, INPUT_BUFFER_MAX_LEN);
    if (!res)
    {
        return false;
    }
    screen->show_str_len += strlen(str);
    screen->cursor_position += strlen(str);
    return true;
}

bool insert_string(char *str, const char *insert, int position, size_t str_size_max)
{
    size_t str_len = strlen(str);
    size_t insert_len = strlen(insert);

    if (position < 0 || position > str_len)
    {
        return false;
    }
    if (str_len + insert_len >= str_size_max - 1)
    {
        return false;
    }

    memmove(str + position + insert_len - 1, str + position - 1, str_len - position + 1);
    memmove(str + position, insert, insert_len);
    str[str_len + insert_len] = '\0';
    return true;
}

bool screen_input_button_pressed(ShowScreen *screen, const BUTTON4048 const *btn)
{
    bool res = screen_append(screen, btn->input_str);
    if (!res)
    {
        return false;
    }
    screen->show_str_len += strlen(btn->input_str);
    return true;
}

bool screen_cursor_left(ShowScreen *screen)
{
    if (screen->cursor_position <= 0)
    {
        return false;
    }
    screen->cursor_position--;
    return true;
}

bool screen_cursor_right(ShowScreen *screen)
{
    if (screen->cursor_position >= screen->show_str_len)
    {
        return false;
    }
    screen->cursor_position++;
    return true;
}

void refresh_cursor(ShowScreen *screen)
{
    int top = 8 + 16 + 2;
    int left = 4 + 8 * screen->cursor_position;
    POINT_COLOR = WHITE;
    LCD_DrawFillRectangle(0, top, lcddev.width, top + 2);
    POINT_COLOR = RED;
    LCD_DrawFillRectangle(left, top, left + 8, top + 2);
}

void refresh_input_aera(ShowScreen *screen)
{
    refresh_cursor(screen);
    LCD_ShowString(4, 8, 16, (u8 *)"                             ", 0);
    POINT_COLOR = BLACK;
    LCD_ShowString(4, 8, 16, (u8 *)screen->show_str, 1);
}

void refresh_result_area(ShowScreen *screen)
{
    LCD_ShowString(4, 8 + 16 + 4, 16, (u8 *)"                             ", 0);
    POINT_COLOR = BLACK;
    LCD_ShowString(4, 8 + 16 + 4, 16, (u8 *)screen->result_str, 1);
}

void refresh_screen(ShowScreen *screen)
{
    refresh_input_aera(screen);
    refresh_result_area(screen);
}