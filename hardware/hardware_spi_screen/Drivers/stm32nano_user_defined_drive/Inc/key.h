#ifndef __KEY_H__
#define __KEY_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define SHORT_TIME 10U // 短按时间 = 200 * (1. / get_tim_freq_Hz(&htimx)) / 1000
#define LONG_TIME 25U  // 长按时间 = 500 * (1. / get_tim_freq_Hz(&htimx)) / 1000

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

    typedef struct KeyScan
    {
        GPIO_PinState last_state;
        uint32_t press_time;      // 按下时间
        uint32_t release_time;    // 松开时间
        uint32_t last_press_time; // 上一次按压时间
    } KeyScan;

    typedef enum KeyState
    {
        InstancePress, // 瞬间按下
        ShortPress,    // 短按
        LongPress,     // 长按
        DoublePress,   // 双击
        NoPress,       // 没按
        Pressing,      // 正在按
    } KeyState;

    KeyState get_key_state(GPIO_PinState read_key, KeyScan *key);

#ifdef __cplusplus
}
#endif
#endif /*__KEY_H__ */