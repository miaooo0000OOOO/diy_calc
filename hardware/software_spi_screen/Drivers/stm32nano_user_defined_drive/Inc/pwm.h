
#ifndef __PWM_H__
#define __PWM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
#include "stdbool.h"
#include "assert.h"

    uint32_t get_tim_fHz(const TIM_HandleTypeDef *htim);

    void set_duty_ratio(TIM_HandleTypeDef *htim, const uint32_t channel, const double ratio);

    void set_tim_counter(const TIM_HandleTypeDef *htim, const double fHz, uint32_t *tim_counter_max, uint32_t *tim_counter);

    bool tim_step(const uint32_t *tim_counter_max, uint32_t *tim_counter);

#ifdef __cplusplus
}
#endif
#endif /*__PWM_H__ */
