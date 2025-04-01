#include <stdio.h>
#include <string.h>
#include "pt100_task.h"
#include "pt100.h"
#include "MCP342x_wrap.h"
#include "Two_Pt_Cal.h"

#define LOG_TAG "Pt100_Task"
#include "elog.h"

#define PT100_TASK_PERIOD 1000

MCP342x_error_t err;

TaskHandle_t pt100_handler;

Pt100I2cParam_t Pt100I2cParam = {
    .i2c_handle =
        {
            .i2cx = I2C3,
            .timeout = 0xFFFFFFFF,
        },
    .mcp_i2c_addr = 0xD0,
    .adc_gain = 3,
};

Pt100Stat_t Pt100Stat = {0};

Pt100TwoCal_t Pt100TwoCal = {
    .pt100_1_raw_l_val = 26433,
    .pt100_2_raw_l_val = 28333,
    .pt100_3_raw_l_val = 26306,
    .pt100_4_raw_l_val = 25476,
    .pt100_1_raw_h_val = 92256,
    .pt100_2_raw_h_val = 94654,
    .pt100_3_raw_h_val = 92379,
    .pt100_4_raw_h_val = 90957,
    .pt100_ideal_l_val = 25684,  // 110ohm
    .pt100_ideal_h_val = 90770,  // 135ohm
};

static int32_t raw_val = 0;
CalParam_t PtCal_1 = {
    .offset = 0.0f,
    .slope = 1.0f,
    .raw_l = &Pt100TwoCal.pt100_1_raw_l_val,
    .raw_h = &Pt100TwoCal.pt100_1_raw_h_val,
    .ideal_l = &Pt100TwoCal.pt100_ideal_l_val,
    .ideal_h = &Pt100TwoCal.pt100_ideal_h_val,
    .data_type = DATA_TYPE_INT32,
};
CalParam_t PtCal_2 = {
    .offset = 0.0f,
    .slope = 1.0f,
    .raw_l = &Pt100TwoCal.pt100_2_raw_l_val,
    .raw_h = &Pt100TwoCal.pt100_2_raw_h_val,
    .ideal_l = &Pt100TwoCal.pt100_ideal_l_val,
    .ideal_h = &Pt100TwoCal.pt100_ideal_h_val,
    .data_type = DATA_TYPE_INT32,
};
CalParam_t PtCal_3 = {
    .offset = 0.0f,
    .slope = 1.0f,
    .raw_l = &Pt100TwoCal.pt100_3_raw_l_val,
    .raw_h = &Pt100TwoCal.pt100_3_raw_h_val,
    .ideal_l = &Pt100TwoCal.pt100_ideal_l_val,
    .ideal_h = &Pt100TwoCal.pt100_ideal_h_val,
    .data_type = DATA_TYPE_INT32,
};
CalParam_t PtCal_4 = {
    .offset = 0.0f,
    .slope = 1.0f,
    .raw_l = &Pt100TwoCal.pt100_4_raw_l_val,
    .raw_h = &Pt100TwoCal.pt100_4_raw_h_val,
    .ideal_l = &Pt100TwoCal.pt100_ideal_l_val,
    .ideal_h = &Pt100TwoCal.pt100_ideal_h_val,
    .data_type = DATA_TYPE_INT32,
};

void pt100_task_function(void* pvParameters) {
  log_i("Pt100 Task Running");
  if (Cal_CalcParams(&PtCal_1)) log_e("PtCal_1 CalcParams failed");
  if (Cal_CalcParams(&PtCal_2)) log_e("PtCal_2 CalcParams failed");
  if (Cal_CalcParams(&PtCal_3)) log_e("PtCal_3 CalcParams failed");
  if (Cal_CalcParams(&PtCal_4)) log_e("PtCal_4 CalcParams failed");

  PT100_Init(&Pt100I2cParam);
  
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while (1) {
    vTaskDelayUntil(&xLastWakeTime, PT100_TASK_PERIOD);

    err = PT100_MCP_ReadAndCalcTemp(&Pt100I2cParam, MCP342x_CHANNEL_1, &raw_val);
    if (err == errorNone) {
      Pt100Stat.pt100_3_temp_m = Cal_Apply(&PtCal_1, raw_val);
      // log_i("pt100_1_temp_m: %d", Pt100Stat.pt100_1_temp_m);
    } else {
      log_e("MCP342x_convertAndRead error: %d", err);
    }

    err = PT100_MCP_ReadAndCalcTemp(&Pt100I2cParam, MCP342x_CHANNEL_2, &raw_val);
    if (err == errorNone) {
      Pt100Stat.pt100_4_temp_m = Cal_Apply(&PtCal_2, raw_val);
      // log_i("pt100_2_temp_m: %d", Pt100Stat.pt100_2_temp_m);
    } else {
      log_e("MCP342x_convertAndRead error: %d", err);
    }

    err = PT100_MCP_ReadAndCalcTemp(&Pt100I2cParam, MCP342x_CHANNEL_3, &raw_val);
    if (err == errorNone) {
      Pt100Stat.pt100_1_temp_m = Cal_Apply(&PtCal_3, raw_val);
      // log_i("pt100_3_temp_m: %d", Pt100Stat.pt100_3_temp_m);
    } else {
      log_e("MCP342x_convertAndRead error: %d", err);
    }

    err = PT100_MCP_ReadAndCalcTemp(&Pt100I2cParam, MCP342x_CHANNEL_4, &raw_val);
    if (err == errorNone) {
      Pt100Stat.pt100_2_temp_m = Cal_Apply(&PtCal_4, raw_val);
      // log_i("pt100_4_temp_m: %d", Pt100Stat.pt100_4_temp_m);
    } else {
      log_e("MCP342x_convertAndRead error: %d", err);
    }
  }
  vTaskDelete(NULL);
}
