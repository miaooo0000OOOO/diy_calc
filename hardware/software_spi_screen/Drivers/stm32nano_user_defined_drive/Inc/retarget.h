
#ifndef __RETARGET_H__
#define __RETARGET_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_uart.h"
#include "stdio.h"
#include "sys/unistd.h"
#include "errno.h"

    extern UART_HandleTypeDef *ghuart;
    void retarget_init(UART_HandleTypeDef *huart);
    int _write(int fd, char *ptr, int len);
    int _read(int fd, const void *buf, size_t count);

#ifdef __cplusplus
}
#endif
#endif /*__RETARGET_H__ */
