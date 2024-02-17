#include "key.h"

KeyState get_key_state(GPIO_PinState read_key, KeyScan *key)
{
    KeyState state;
    if (read_key == GPIO_PIN_RESET)
    {
        if (key->last_state == GPIO_PIN_SET)
        { // 按下
            key->press_time = 0;
            state = InstancePress;
        }
        if (key->press_time == LONG_TIME) // 20*25按压过程中超过25
            state = LongPress;            // 长按
        else if (key->press_time != 0)
            state = Pressing; // 否则表示按压中
        key->press_time++;
    }
    else // read_key == GPIO_PIN_SET
    {    // 没按
        state = NoPress;
        if (key->last_state == GPIO_PIN_RESET)
        { // 松开
            if (key->release_time < SHORT_TIME && key->last_press_time > 1)
            {
                state = DoublePress;
            }
            key->last_press_time = key->press_time;
            key->press_time = key->release_time = 0;
            if (state == DoublePress)
            {
                key->release_time = SHORT_TIME + 1;
            }
        }
        if (key->release_time == SHORT_TIME && key->last_press_time < LONG_TIME && key->last_press_time > 1)
        {
            state = ShortPress;
        }
        key->release_time += 1;
    }
    key->last_state = read_key; // 上一次按压状态
    return state;
}
