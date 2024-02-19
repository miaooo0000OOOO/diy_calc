#include "retarget.h"

UART_HandleTypeDef *ghuart = NULL;

void retarget_init(UART_HandleTypeDef *huart)
{
    ghuart = huart;
    setvbuf(stdout, NULL, _IONBF, 0);
}

int _write(int fd, char *ptr, int len)
{
    for (int i = 0; i < len; i++)
    {
        while ((ghuart->Instance->SR & 0x40) == 0)
            ;
        ghuart->Instance->DR = (uint8_t *)ptr[i];
    }
    return len;
}

int _read(int fd, const void *buf, size_t count)
{
    HAL_StatusTypeDef hstatus;

    if (fd == STDIN_FILENO)
    {
        hstatus = HAL_UART_Receive(ghuart, (uint8_t *)buf, 1, HAL_MAX_DELAY);
        if (hstatus == HAL_OK)
        {
            return 1;
        }
        else
        {
            return EIO;
        }
    }
    errno = EBADF;
    return 0;
}