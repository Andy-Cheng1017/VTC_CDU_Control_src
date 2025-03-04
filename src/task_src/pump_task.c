#include <stdio.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "at32f403a_407_wk_config.h"
#include "FG_RPM.h"
#include "Duty_PWM.h"
#include "pump_task.h"

#define LOG_TAG "Pump_Task"
#include "elog.h"

TaskHandle_t pump_handler;

pump_control_type pump_control = {0};
pump_status_type pump_status = {0};

FgParam_t Pump1_FgParam = {
    .h_tmr_x = TMR10,
    .l_tmr_x = TMR9,
    .exint_line = EXINT_LINE_6,
    .motor_phase = 6,
};

FgParam_t Pump2_FgParam = {
    .h_tmr_x = TMR10,
    .l_tmr_x = TMR9,
    .exint_line = EXINT_LINE_7,
    .motor_phase = 6,
};

PwmParam_t Pump1_PwmParam = {
    .channel = TMR_SELECT_CHANNEL_3,
    .tmr_x = TMR4,
    .tmr_period_val = 59999,
};

PwmParam_t Pump2_PwmParam = {
    .channel = TMR_SELECT_CHANNEL_4,
    .tmr_x = TMR4,
    .tmr_period_val = 59999,
};

void TMR1_BRK_TMR9_IRQHandler(void) {
  if (tmr_flag_get(TMR9, TMR_OVF_FLAG)) {
    tmr_flag_clear(TMR9, TMR_OVF_FLAG);
    FgLowTmrIntHandler(&Pump1_FgParam);
    FgLowTmrIntHandler(&Pump2_FgParam);
  }
}

void TMR1_OVF_TMR10_IRQHandler(void) {
  if (tmr_flag_get(TMR10, TMR_OVF_FLAG)) {
    tmr_flag_clear(TMR10, TMR_OVF_FLAG);
    FgHighTmrIntHandler(&Pump1_FgParam);
    FgHighTmrIntHandler(&Pump1_FgParam);
  }
}

void EXINT9_5_IRQHandler(void) {
  if (exint_interrupt_flag_get(EXINT_LINE_6)) {
    exint_flag_clear(EXINT_LINE_6);

    FgExintIntSampling(&Pump1_FgParam);
  } else if (exint_interrupt_flag_get(EXINT_LINE_7)) {
    exint_flag_clear(EXINT_LINE_7);

    FgExintIntSampling(&Pump2_FgParam);
  }
}

void pump_task_function(void* pvParameters) {
  log_i("Pump Task Started");

  FgInit(&Pump1_FgParam);
  FgInit(&Pump2_FgParam);

  PwmInit(&Pump1_PwmParam);
  PwmInit(&Pump2_PwmParam);

  while (1) {
    FgGetRPM(&Pump1_FgParam, &pump_status.pump_1_FB);
    FgGetRPM(&Pump2_FgParam, &pump_status.pump_2_FB);

    PwmSetDuty(&Pump1_PwmParam, pump_control.pump_1_rpm);
    PwmSetDuty(&Pump2_PwmParam, pump_control.pump_2_rpm);
    vTaskDelay(100);
  }
}