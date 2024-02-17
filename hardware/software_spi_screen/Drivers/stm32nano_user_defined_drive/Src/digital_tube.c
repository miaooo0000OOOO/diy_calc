
#include "digital_tube.h"

/****** 定量定义 ******/
/*74HC595芯片输入*/
const uint8_t seven_segment_number[10][2] = {
    {0xfc, 0xfd}, // 0
    {0x60, 0x61}, // 1
    {0xda, 0xdb}, // 2
    {0xf2, 0xf3}, // 3
    {0x66, 0x67}, // 4
    {0xb6, 0xb7}, // 5
    {0xbe, 0xbf}, // 6
    {0xe0, 0xe1}, // 7
    {0xfe, 0xff}, // 8
    {0xf6, 0xf7}  // 9
};                // 数字

const uint8_t seven_segment_word[] = {
    0xee, // A
    0x3e, // b
    0x9c, // C
    0x7a, // d
    0x9e, // E
    0x8e, // F
    0xbc, // G
    0x6e, // H
    0x00, // *I
    0x70, // J
    0x00, // *K
    0x1c, // L
    0x00, // *M
    0x2a, // n
    0xfc, // O
    0xce, // P
    0xfd, // Q
    0x0a, // r
    0xb6, // S
    0x1e, // t
    0x7c, // U
    0x00, // *V
    0x00, // *W
    0x00, // *X
    0x76, // y
    0x00, // *Z
};        // 字母
const uint8_t seven_segment_edge[8] = {
    0x01,
}; // 单边【未完成】

/*74HC595芯片输入*/

/*74HC138芯片输入*/
const bool digital_tube_decimal_to_binary[8][3] = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0}, {0, 0, 1}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1}}; // 74HC138芯片片选
/*74HC138芯片输入*/
/****** 定量定义 ******/

/****** 变量定义 ******/
uint8_t output_seven_segent_history[8]; // 输出流
uint8_t output_seven_segent[8];         // 下轮刷新流
/****** 变量定义 ******/

/****** 内部函数 ******/
void digital_tube_push_into_output_stream();                           // 将刷新流推入输出流
void digital_tube_clean_hc595();                                       // 清空74HC595
void digital_tube_push_into_hc595(uint8_t the_number_of_digital_tube); // 将输出流推入74HC595
void digital_tube_push_into_hc138(uint8_t the_number_of_digital_tube); // 将输出流推入74HC138
/****** 内部函数 ******/

/****** 函数实现 ******/
/*将刷新流推入输出流*/
void digital_tube_push_into_output_stream()
{
    uint8_t i = 0;

    for (i = 0; i < 8; i++)
    {
        output_seven_segent_history[i] = output_seven_segent[i];
    }
}
/*将刷新流推入输出流*/

/*清空74HC595*/
void digital_tube_clean_hc595()
{
    uint8_t i = 0;

    // 给寄存器推为空
    for (i = 0; i < 8; i++)
    {
        DIGITAL_TUBE_DS(GPIO_PIN_RESET);
        DIGITAL_TUBE_SCK(0);
        DIGITAL_TUBE_SCK(1);
    }
}
/*清空74HC595*/

/*将输出流推入74HC595*/
void digital_tube_push_into_hc595(uint8_t the_number_of_digital_tube)
{
    uint8_t i = 0;
    uint8_t k = 0;
    uint8_t output = output_seven_segent_history[the_number_of_digital_tube]; // 获取HC595_DATA

    // 输出数字
    for (i = 0; i < 8; i++)
    {
        k = ((output >> i) & 0x01);
        DIGITAL_TUBE_DS((GPIO_PinState)k);
        DIGITAL_TUBE_SCK(0);
        DIGITAL_TUBE_SCK(1);
    }
}
/*将输出流推入74HC595*/

/*将输出流推入74HC138*/
void digital_tube_push_into_hc138(uint8_t the_number_of_digital_tube)
{
    DIGITAL_TUBE_A0(digital_tube_decimal_to_binary[the_number_of_digital_tube][0]);
    DIGITAL_TUBE_A1(digital_tube_decimal_to_binary[the_number_of_digital_tube][1]);
    DIGITAL_TUBE_A2(digital_tube_decimal_to_binary[the_number_of_digital_tube][2]);
}
/*将输出流推入74HC138*/

/*数码管初始化*/
void digital_tube_init()
{

    // 初始化数据线
    DIGITAL_TUBE_A0(0);
    DIGITAL_TUBE_A1(0);
    DIGITAL_TUBE_A2(0);
    DIGITAL_TUBE_DS(0);
    DIGITAL_TUBE_LCLK(0);
    DIGITAL_TUBE_SCK(0);

    // 刷新流置空
    digital_tube_all_none();

    // 将刷新流推入输出流
    digital_tube_push_into_output_stream();

    // 数码管刷新
    digital_tube_refresh();
}
/*数码管初始化*/

/*数码管刷新*/
void digital_tube_refresh()
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        // 清空该数据管
        digital_tube_clean_hc595();
        DIGITAL_TUBE_LCLK(0);
        DIGITAL_TUBE_LCLK(1);
        DIGITAL_TUBE_LCLK(0);

        // 第i位进入移位寄存器
        digital_tube_push_into_hc138(i);
        digital_tube_push_into_hc595(i);

        // 移位寄存器数据进入存储寄存器
        DIGITAL_TUBE_LCLK(0);
        DIGITAL_TUBE_LCLK(1);
        DIGITAL_TUBE_LCLK(0);
    }

    // 清除最后一位
    digital_tube_clean_hc595();
    DIGITAL_TUBE_LCLK(0);
    DIGITAL_TUBE_LCLK(1);
    DIGITAL_TUBE_LCLK(0);

    // 将新的刷新流推入输出流
    digital_tube_push_into_output_stream();
}
/*数码管刷新*/

/*显示整数*/

Result digital_tube_show_interger_number(int value)
{
    uint32_t k;
    bool neg;
    if (value >= 0)
    {
        if (value > DIGITAL_TUBE_8_9)
            return Err;
        neg = false;
    }
    else
    {
        if (-value > DIGITAL_TUBE_7_9)
            return Err;
        neg = true;
        value = -value;
    }

    k = DIGITAL_TUBE_7_9 + 1;
    for (uint8_t i = 1; i <= 8; i++, k /= 10)
    {
        if (i == 8 && value == 0)
        {
            digital_tube_change_number(i, 0, false);
        }
        else if (value >= k)
            digital_tube_change_number(i, (value / k) % 10, false);
        else
            digital_tube_change_none(i);
    }
    if (neg)
    {
        output_seven_segent[0] = SEVEN_SEGMENT_MINUS;
    }
    return Ok;
}
/*显示整数*/

/*显示数组*/
Result digital_tube_show_array(char string[], bool point[])
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (string[i] >= '0' && string[i] <= '9')
        {
            digital_tube_change_number(i + 1, string[i] - '0', point[i]);
        }
        else if (string[i] == ' ')
        {
            digital_tube_change_none(i + 1);
        }
        else
        {
            return Err;
        }
    }
    return Ok;
}
/*显示数组*/

/*修改（1-8号)号码管显示（空白）*/
void digital_tube_change_none(uint8_t the_number_of_digital_tube)
{
    assert(1 <= the_number_of_digital_tube && the_number_of_digital_tube <= 8);
    output_seven_segent[the_number_of_digital_tube - 1] = SEVEN_SEGMENT_NONE;
}
/*修改（1-8号)号码管显示（空白）*/

/*修改（1-8号)号码管显示（全满）*/
void digital_tube_change_full(uint8_t the_number_of_digital_tube)
{
    assert(1 <= the_number_of_digital_tube && the_number_of_digital_tube <= 8);
    output_seven_segent[the_number_of_digital_tube - 1] = SEVEN_SEGMENT_FULL;
}
/*修改（1-8号)号码管显示（全满）*/

/*修改（1-8号)号码管显示（数字）（是否带小数点）*/
void digital_tube_change_number(uint8_t the_number_of_digital_tube, uint8_t value, bool point)
{
    assert(1 <= the_number_of_digital_tube && the_number_of_digital_tube <= 8);
    assert(0 <= value && value <= 9);
    output_seven_segent[the_number_of_digital_tube - 1] = seven_segment_number[value][point];
}
/*修改（1-8号)号码管显示（数字）（是否带小数点）*/

/*修改（1-8号)号码管显示任意符号*/
void digital_tube_change_bits(uint8_t the_number_of_digital_tube, uint8_t bits)
{
    assert(1 <= the_number_of_digital_tube && the_number_of_digital_tube <= 8);
    output_seven_segent[the_number_of_digital_tube - 1] = bits;
}
/*修改（1-8号)号码管显示任意符号*/

/*八个号码管全显示全满*/
void digital_tube_all_full()
{
    uint8_t i = 0;

    for (i = 0; i < 8; i++)
    {
        output_seven_segent[i] = SEVEN_SEGMENT_FULL;
    }
}
/*八个号码管全显示全满*/

/*八个号码管全显示空白*/
void digital_tube_all_none()
{
    uint8_t i = 0;

    for (i = 0; i < 8; i++)
    {
        output_seven_segent[i] = SEVEN_SEGMENT_NONE;
    }
}
/*八个号码管全显示空白*/

/*八个号码管全显示数字（是否带小数点）*/
void digital_tube_all_number(uint8_t value, bool point)
{
    assert(0 <= value && value <= 9);
    uint8_t i = 0;
    for (i = 0; i < 8; i++)
    {
        output_seven_segent[i] = seven_segment_number[value][point];
    }
}
/*八个号码管全显示数字（是否带小数点）*/

/*八个号码管显示数组数字（是否带小数点）*/
void digital_tube_array_number(uint8_t value_array[], bool point_array[])
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        digital_tube_change_number(8 - i, value_array[i], point_array[i]);
    }
}
/*八个号码管显示数组数字（是否带小数点）*/
/****** 函数实现 ******/
