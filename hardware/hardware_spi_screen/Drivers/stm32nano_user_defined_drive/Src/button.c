#include "button.h"

void wait_until_key_release(GPIO_TypeDef *key_port, uint16_t key_pin, GPIO_PinState release_state)
{
    while (HAL_GPIO_ReadPin(key_port, key_pin) == release_state)
        ;
}

BUTTON_STATUS get_button_status(GPIO_TypeDef *key_port, uint16_t key_pin, GPIO_PinState release_state)
{
    if (HAL_GPIO_ReadPin(key_port, key_pin) != release_state)
    {
        HAL_Delay(130);
        if (HAL_GPIO_ReadPin(key_port, key_pin) != release_state)
        {
            wait_until_key_release(key_port, key_pin, release_state);
            return LONG_TIME_PRESS;
        }
        else
        {
            HAL_Delay(100);
            if (HAL_GPIO_ReadPin(key_port, key_pin) != release_state)
            {
                wait_until_key_release(key_port, key_pin, release_state);
                return DOUBLE_PRESS;
            }
            else
            {
                return SHORT_TIME_PRESS;
            }
        }
    }
    else
    {
        return NO_PRESS;
    }
}