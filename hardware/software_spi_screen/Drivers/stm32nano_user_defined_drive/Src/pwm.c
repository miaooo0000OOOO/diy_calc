#include "pwm.h"

uint32_t get_tim_fHz(const TIM_HandleTypeDef *htim)
{
    return (HAL_RCC_GetSysClockFreq() / (htim->Init.Prescaler + 1) / (htim->Init.Period + 1));
}

void set_duty_ratio(TIM_HandleTypeDef *htim, const uint32_t channel, const double ratio)
{
    assert(0. <= ratio && ratio <= 1.);
    __HAL_TIM_SET_COMPARE(htim, channel, htim->Init.Period * (1 - ratio));
}

void set_tim_counter(const TIM_HandleTypeDef *htim, const double fHz, uint32_t *tim_counter_max, uint32_t *tim_counter)
{
    assert(fHz > 0);
    *tim_counter_max = get_tim_fHz(htim) / fHz;
    *tim_counter = 0;
}

bool tim_step(const uint32_t *tim_counter_max, uint32_t *tim_counter)
{
    *tim_counter += 1;
    if (*tim_counter >= *tim_counter_max)
    {
        *tim_counter = 0;
        return true;
    }
    else
    {
        return false;
    };
}
