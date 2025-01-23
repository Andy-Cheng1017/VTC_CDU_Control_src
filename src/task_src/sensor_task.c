#include <stdio.h>
#include <string.h>
#include "sensor_task.h"

#define LOG_TAG "Sensor_Task"
#include "elog.h"

TaskHandle_t sensor_handler;

SensStat_t SensStat = {0};
SensCtrl_t SensCtrl = {
    .porpo_1_pwm = 0,
    .porpo_2_pwm = 0,
    .pt100_1_raw_l_val = 26433,
    .pt100_2_raw_l_val = 28333,
    .pt100_3_raw_l_val = 26306,
    .pt100_4_raw_l_val = 25476,
    .pt100_1_raw_h_val = 92256,
    .pt100_2_raw_h_val = 94654,
    .pt100_3_raw_h_val = 92379,
    .pt100_4_raw_h_val = 90957,
    .pt100_ideal_l_val = 25684,
    .pt100_ideal_h_val = 90770,
};

int32_t raw_val = 0;
CalParam_t PtCal_1 = {
    .offset = 0.0f,
    .slope = 1.0f,
    .raw_l = &SensCtrl.pt100_1_raw_l_val,
    .raw_h = &SensCtrl.pt100_1_raw_h_val,
    .ideal_l = &SensCtrl.pt100_ideal_l_val,
    .ideal_h = &SensCtrl.pt100_ideal_h_val,
};
CalParam_t PtCal_2 = {
    .offset = 0.0f,
    .slope = 1.0f,
    .raw_l = &SensCtrl.pt100_2_raw_l_val,
    .raw_h = &SensCtrl.pt100_2_raw_h_val,
    .ideal_l = &SensCtrl.pt100_ideal_l_val,
    .ideal_h = &SensCtrl.pt100_ideal_h_val,
};
CalParam_t PtCal_3 = {
    .offset = 0.0f,
    .slope = 1.0f,
    .raw_l = &SensCtrl.pt100_3_raw_l_val,
    .raw_h = &SensCtrl.pt100_3_raw_h_val,
    .ideal_l = &SensCtrl.pt100_ideal_l_val,
    .ideal_h = &SensCtrl.pt100_ideal_h_val,
};
CalParam_t PtCal_4 = {
    .offset = 0.0f,
    .slope = 1.0f,
    .raw_l = &SensCtrl.pt100_4_raw_l_val,
    .raw_h = &SensCtrl.pt100_4_raw_h_val,
    .ideal_l = &SensCtrl.pt100_ideal_l_val,
    .ideal_h = &SensCtrl.pt100_ideal_h_val,
};

void sensor_task_function(void* pvParameters) {
  if (Cal_CalcParams(&PtCal_1)) log_e("PtCal_1 CalcParams failed");
  if (Cal_CalcParams(&PtCal_2)) log_e("PtCal_2 CalcParams failed");
  if (Cal_CalcParams(&PtCal_3)) log_e("PtCal_3 CalcParams failed");
  if (Cal_CalcParams(&PtCal_4)) log_e("PtCal_4 CalcParams failed");

  MCP342x_generalCallReset();
  vTaskDelay(5);

  while (1) {
    vTaskDelay(2000);
    long result;

    err = MCP342x_convertAndRead(MCP342x_CHANNEL_1, &result);
    if (err == errorNone) {
      raw_val = PT100_CalcTemp(3, result);
      SensStat.pt100_1_temp_m = Cal_Apply(&PtCal_1, raw_val);
      log_i("pt100_1_temp_m: %d", SensStat.pt100_1_temp_m);
    } else {
      log_e("MCP342x_convertAndRead error: %d", err);
    }

    err = MCP342x_convertAndRead(MCP342x_CHANNEL_2, &result);
    if (err == errorNone) {
      raw_val = PT100_CalcTemp(3, result);
      SensStat.pt100_2_temp_m = Cal_Apply(&PtCal_2, raw_val);
      log_i("pt100_2_temp_m: %d", SensStat.pt100_2_temp_m);
    } else {
      log_e("MCP342x_convertAndRead error: %d", err);
    }

    err = MCP342x_convertAndRead(MCP342x_CHANNEL_3, &result);
    if (err == errorNone) {
      raw_val = PT100_CalcTemp(3, result);
      SensStat.pt100_3_temp_m = Cal_Apply(&PtCal_3, raw_val);
      log_i("pt100_3_temp_m: %d", SensStat.pt100_3_temp_m);
    } else {
      log_e("MCP342x_convertAndRead error: %d", err);
    }

    err = MCP342x_convertAndRead(MCP342x_CHANNEL_4, &result);
    if (err == errorNone) {
      raw_val = PT100_CalcTemp(3, result);
      SensStat.pt100_4_temp_m = Cal_Apply(&PtCal_4, raw_val);
      log_i("pt100_4_temp_m: %d", SensStat.pt100_4_temp_m);
    } else {
      log_e("MCP342x_convertAndRead error: %d", err);
    }
  }
  vTaskDelete(NULL);
}
