/*
    这是NANO STM32F1开发板板载数码管的驱动。

    需要打开的设置为：
    PB3、PB4、PB5、PC10、PC11、PC12引脚。
    初始化为GPIO_OUTPUT、GPIO_PIN_RESET、OUTPUT Push Pull、NO pull-up and no pull-down

    若使用TIM作为数码管刷新，建议刷新间隔小于0.1毫秒，（719和19，72MHz）
*/

#ifndef __DIGITAL_TUBE_H__
#define __DIGITAL_TUBE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stdbool.h"
#include "assert.h"

/****** 宏定义 ******/
#define DIGITAL_TUBE_A0(val) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, (GPIO_PinState)val)  // A0数据线
#define DIGITAL_TUBE_A1(val) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, (GPIO_PinState)val)  // A1数据线
#define DIGITAL_TUBE_A2(val) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, (GPIO_PinState)val)  // A2数据线
#define DIGITAL_TUBE_DS(val) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, (GPIO_PinState)val)   // 数据传输线
#define DIGITAL_TUBE_LCLK(val) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, (GPIO_PinState)val) // 存储寄存器时钟线
#define DIGITAL_TUBE_SCK(val) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (GPIO_PinState)val)  // 移位寄存器时钟线
#define DIGITAL_TUBE_8_9 99999999L
#define DIGITAL_TUBE_7_9 9999999L


#define SEVEN_SEGMENT_NONE ((char)0x00)                                 // 空 编译器优化后类型为uint8_t
#define SEVEN_SEGMENT_FULL ((char)0xff)                                 // 满 编译器优化后类型为uint8_t
#define SEVEN_SEGMENT_MINUS ((char)0x02)                                // 减号
#define WITH_POINT(digital_tube_bits) (digital_tube_bits | (char)0x01U) // 加小数点 相信编译器的智慧 会优化成立即数

#define BITS_CHAR_A ((char)0xee)
#define BITS_CHAR_b ((char)0x3e)
#define BITS_CHAR_C ((char)0x9c)
#define BITS_CHAR_d ((char)0x7a)
#define BITS_CHAR_E ((char)0x9e)
#define BITS_CHAR_F ((char)0x8e)
#define BITS_CHAR_G ((char)0xbc)
#define BITS_CHAR_H ((char)0x6e)
#define BITS_CHAR_J ((char)0x70)
#define BITS_CHAR_L ((char)0x1c)
#define BITS_CHAR_n ((char)0x2a)
#define BITS_CHAR_O ((char)0xfc)
#define BITS_CHAR_o ((char)0x3a)
#define BITS_CHAR_P ((char)0xce)
#define BITS_CHAR_Q ((char)0xfd)
#define BITS_CHAR_r ((char)0x0a)
#define BITS_CHAR_S ((char)0xb6)
#define BITS_CHAR_t ((char)0x1e)
#define BITS_CHAR_U ((char)0x7c)
#define BITS_CHAR_u ((char)0x38)
#define BITS_CHAR_y ((char)0x76)
    /****** 宏定义 ******/

    typedef enum Result
    {
        Ok = 0U,
        Err
    } Result;

    /*数码管初始化*/
    void digital_tube_init();

    /*数码管刷新*/
    void digital_tube_refresh();

    /*显示整数*/
    Result digital_tube_show_interger_number(int value);

    /*显示数组*/
    Result digital_tube_show_array(char string[], bool point[]);

    /*修改（1-8号)号码管显示（空白）*/
    void digital_tube_change_none(uint8_t the_number_of_digital_tube);

    /*修改（1-8号)号码管显示（全满）*/
    void digital_tube_change_full(uint8_t the_number_of_digital_tube);

    /*修改（1-8号)号码管显示（数字）（是否带小数点）*/
    void digital_tube_change_number(uint8_t the_number_of_digital_tube, uint8_t value, bool point);

    /*修改（1-8号)号码管显示任意符号*/
    void digital_tube_change_bits(uint8_t the_number_of_digital_tube, uint8_t bits);

    /*八个号码管全显示全满*/
    void digital_tube_all_full();

    /*八个号码管全显示空白*/
    void digital_tube_all_none();

    /*八个号码管全显示数字（是否带小数点）*/
    void digital_tube_all_number(uint8_t value, bool point);

    /*八个号码管显示数组数字（是否带小数点）大端模式*/
    void digital_tube_array_number(uint8_t value_array[], bool point_array[]);

#ifdef __cplusplus
}
#endif

#endif /* __DIGITAL_TUBE_H__ */
