#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Duty_PWM.h"

void PwmSetup(PwmParam_t *pwm, uint16_t duty) {
  uint16_t pulse = (uint16_t)(((uint32_t)duty * (pwm->tmr_period_val - 1)) / 1000);

  tmr_channel_value_set(pwm->tmr_x, pwm->channel, pulse);
}