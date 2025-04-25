#include <stdio.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "at32f403a_407_wk_config.h"
#include "FG_RPM.h"
#include "Duty_PWM.h"
#include "pump_task.h"
#include "main_task.h"

#define LOG_TAG "Pump_Task"
#include "elog.h"

#define Fan_FG_READ_PERIOD 500

TaskHandle_t pump_handler;

pump_control_type pump_control = {0};
pump_status_type pump_status = {0};

FgParam_t Pump1_Fg = {
    .timer_count = 3,
    .tmr_list = {TMR11, TMR10, TMR9},
    .exint_line = EXINT_LINE_6,
    .motor_phase = 6,
};

FgParam_t Pump2_Fg = {
    .timer_count = 3,
    .tmr_list = {TMR11, TMR10, TMR9},
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

void EXINT9_5_IRQHandler(void) {
  if (exint_interrupt_flag_get(EXINT_LINE_6)) {
    exint_flag_clear(EXINT_LINE_6);
    FgExintIntSampling(&Pump1_Fg);
  } else if (exint_interrupt_flag_get(EXINT_LINE_7)) {
    exint_flag_clear(EXINT_LINE_7);
    FgExintIntSampling(&Pump2_Fg);
  }
}

void pump_task_function(void* pvParameters) {
  log_i("Pump Task Started");
  TickType_t xLastWakeTime = xTaskGetTickCount();

  FgInit(&Pump1_Fg);
  FgInit(&Pump2_Fg);

  PwmInit(&Pump1_PwmParam);
  PwmInit(&Pump2_PwmParam);

  while (1) {
    vTaskDelayUntil(&xLastWakeTime, Fan_FG_READ_PERIOD);
    FgGetRPM(&Pump1_Fg, &pump_status.pump_1_FB);
    FgGetRPM(&Pump2_Fg, &pump_status.pump_2_FB);

    if (SysInform.power_on_setting) {
      if (pump_control.pump_1_rpm < SysParaSet.pump_min_duty) pump_control.pump_1_rpm = SysParaSet.pump_min_duty;
      if (pump_control.pump_2_rpm < SysParaSet.pump_min_duty) pump_control.pump_2_rpm = SysParaSet.pump_min_duty;
    }

    PwmSetDuty(&Pump1_PwmParam, pump_control.pump_1_rpm);
    PwmSetDuty(&Pump2_PwmParam, pump_control.pump_2_rpm);
  }
}