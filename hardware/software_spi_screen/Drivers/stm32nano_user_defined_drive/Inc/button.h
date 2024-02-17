
#ifndef __BUTTON_H__
#define __BUTTON_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

    typedef enum BUTTON_STATUS
    {
        SHORT_TIME_PRESS = 0U,
        LONG_TIME_PRESS,
        DOUBLE_PRESS,
        NO_PRESS
    } BUTTON_STATUS;

    void wait_until_key_release(GPIO_TypeDef *key_port, uint16_t key_pin, GPIO_PinState release_state);

    BUTTON_STATUS get_button_status(GPIO_TypeDef *key_port, uint16_t key_pin, GPIO_PinState release_state);

#ifdef __cplusplus
}
#endif
#endif /*__BUTTON_H__ */
