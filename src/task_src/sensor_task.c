#include <stdio.h>
#include <string.h>
#include "sensor_task.h"

#define LOG_TAG "Sensor_Task"
#include "elog.h"

TaskHandle_t sensor_handler;

NtcTwoCal_t NtcTwoCal = {
    .ntc_1_raw_l_val = 25000,
    .ntc_2_raw_l_val = 25000,
    .ntc_3_raw_l_val = 25000,
    .ntc_4_raw_l_val = 25000,
    .ntc_1_raw_h_val = 99874,
    .ntc_2_raw_h_val = 99874,
    .ntc_3_raw_h_val = 99874,
    .ntc_4_raw_h_val = 99874,
    .ntc_ideal_l_val = 25000,  // 10kohm
    .ntc_ideal_h_val = 99874,  // 700ohm
};

PressTwoCal_t PressTwoCal = {
    .press_1_raw_l_val = 0,
    .press_2_raw_l_val = 0,
    .press_3_raw_l_val = 0,
    .press_4_raw_l_val = 0,
    .press_1_raw_h_val = 2000,
    .press_2_raw_h_val = 2000,
    .press_3_raw_h_val = 2000,
    .press_4_raw_h_val = 2000,
    .press_ideal_l_val = 0,     // 4ma
    .press_ideal_h_val = 2000,  // 7.2ma
};
 

CalParam_t NtcCal_1 = {
    .offset = 0.0f,
    .slope = 1.0f,
    .raw_l = &NtcTwoCal.ntc_1_raw_l_val,
    .raw_h = &NtcTwoCal.ntc_1_raw_h_val,
    .ideal_l = &NtcTwoCal.ntc_ideal_l_val,
    .ideal_h = &NtcTwoCal.ntc_ideal_h_val,
    .data_type = DATA_TYPE_INT32,
};

CalParam_t NtcCal_2 = {
    .offset = 0.0f,
    .slope = 1.0f,
    .raw_l = &NtcTwoCal.ntc_2_raw_l_val,
    .raw_h = &NtcTwoCal.ntc_2_raw_h_val,
    .ideal_l = &NtcTwoCal.ntc_ideal_l_val,
    .ideal_h = &NtcTwoCal.ntc_ideal_h_val,
    .data_type = DATA_TYPE_INT32,
};

CalParam_t NtcCal_3 = {
    .offset = 0.0f,
    .slope = 1.0f,
    .raw_l = &NtcTwoCal.ntc_3_raw_l_val,
    .raw_h = &NtcTwoCal.ntc_3_raw_h_val,
    .ideal_l = &NtcTwoCal.ntc_ideal_l_val,
    .ideal_h = &NtcTwoCal.ntc_ideal_h_val,
    .data_type = DATA_TYPE_INT32,
};

CalParam_t NtcCal_4 = {
    .offset = 0.0f,
    .slope = 1.0f,
    .raw_l = &NtcTwoCal.ntc_4_raw_l_val,
    .raw_h = &NtcTwoCal.ntc_4_raw_h_val,
    .ideal_l = &NtcTwoCal.ntc_ideal_l_val,
    .ideal_h = &NtcTwoCal.ntc_ideal_h_val,
    .data_type = DATA_TYPE_INT32,
};

CalParam_t PressCal_1 = {
    .offset = 0.0f,
    .slope = 1.0f,
    .raw_l = &PressTwoCal.press_1_raw_l_val,
    .raw_h = &PressTwoCal.press_1_raw_h_val,
    .ideal_l = &PressTwoCal.press_ideal_l_val,
    .ideal_h = &PressTwoCal.press_ideal_h_val,
    .data_type = DATA_TYPE_INT16,
};

CalParam_t PressCal_2 = {
    .offset = 0.0f,
    .slope = 1.0f,
    .raw_l = &PressTwoCal.press_2_raw_l_val,
    .raw_h = &PressTwoCal.press_2_raw_h_val,
    .ideal_l = &PressTwoCal.press_ideal_l_val,
    .ideal_h = &PressTwoCal.press_ideal_h_val,
    .data_type = DATA_TYPE_INT16,
};

CalParam_t PressCal_3 = {
    .offset = 0.0f,
    .slope = 1.0f,
    .raw_l = &PressTwoCal.press_3_raw_l_val,
    .raw_h = &PressTwoCal.press_3_raw_h_val,
    .ideal_l = &PressTwoCal.press_ideal_l_val,
    .ideal_h = &PressTwoCal.press_ideal_h_val,
    .data_type = DATA_TYPE_INT16,
};

CalParam_t PressCal_4 = {
    .offset = 0.0f,
    .slope = 1.0f,
    .raw_l = &PressTwoCal.press_4_raw_l_val,
    .raw_h = &PressTwoCal.press_4_raw_h_val,
    .ideal_l = &PressTwoCal.press_ideal_l_val,
    .ideal_h = &PressTwoCal.press_ideal_h_val,
    .data_type = DATA_TYPE_INT16,
};

ErrNtc_t err_ntc;

ErrConv_t err_conv;
SensConvVal_t PressConv = {
    .volt_min = 0.6f,
    .volt_max = 3.0f,
    .sens_min = 0.0f,
    .sens_max = 10000.0f,
    .adc_res = 4095,
    .adc_ref = 3.3f,
};

SensStat_t SensStat = {0};
SensCtrl_t SensCtrl = {
    .porpo_1_pwm = 0,
    .porpo_2_pwm = 0,
};

void sensor_task_function(void* pvParameters) {
  log_i("Sensor Task Running");
  int32_t raw_val;

  Conv_Init(&PressConv);

  if (Cal_CalcParams(&NtcCal_1)) log_e("NtcCal_1 CalcParams failed");
  if (Cal_CalcParams(&NtcCal_2)) log_e("NtcCal_2 CalcParams failed");
  if (Cal_CalcParams(&NtcCal_3)) log_e("NtcCal_3 CalcParams failed");
  if (Cal_CalcParams(&NtcCal_4)) log_e("NtcCal_4 CalcParams failed");
  if (Cal_CalcParams(&PressCal_1)) log_e("PressCal_1 CalcParams failed");
  if (Cal_CalcParams(&PressCal_2)) log_e("PressCal_2 CalcParams failed");
  if (Cal_CalcParams(&PressCal_3)) log_e("PressCal_3 CalcParams failed");
  if (Cal_CalcParams(&PressCal_4)) log_e("PressCal_4 CalcParams failed");

  vTaskDelay(5);

  while (1) {
    adc_ordinary_software_trigger_enable(ADC1, TRUE);
    while (dma_flag_get(DMA1_FDT1_FLAG) == RESET);
    dma_flag_clear(DMA1_FDT1_FLAG);

    err_conv = Conv_GetVal_Volt(&PressConv, (((float)adc1_ordinary_valuetab[0]) / adc1_ordinary_valuetab[8]) * 1.2f, &raw_val);
    if (err_conv == VAL_OK) SensStat.press_1_val = (int16_t)Cal_Apply(&PressCal_1, raw_val);
    // else
    //   log_e("Conv_GetVal_Volt error: %d", err_conv);

    err_conv = Conv_GetVal_Volt(&PressConv, (((float)adc1_ordinary_valuetab[1]) / adc1_ordinary_valuetab[8]) * 1.2f, &raw_val);
    if (err_conv == VAL_OK) SensStat.press_2_val = (int16_t)Cal_Apply(&PressCal_2, raw_val);
    // else
    //   log_e("Conv_GetVal_Volt error: %d", err_conv);

    err_conv = Conv_GetVal_Volt(&PressConv, (((float)adc1_ordinary_valuetab[2]) / adc1_ordinary_valuetab[8]) * 1.2f, &raw_val);
    if (err_conv == VAL_OK) SensStat.press_3_val = (int16_t)Cal_Apply(&PressCal_3, raw_val);
    // else
    //   log_e("Conv_GetVal_Volt error: %d", err_conv);

    err_conv = Conv_GetVal_Volt(&PressConv, (((float)adc1_ordinary_valuetab[3]) / adc1_ordinary_valuetab[8]) * 1.2f, &raw_val);
    if (err_conv == VAL_OK) SensStat.press_4_val = (int16_t)Cal_Apply(&PressCal_4, raw_val);
    // else
    //   log_e("Conv_GetVal_Volt error: %d", err_conv);

    err_ntc = Ntc_ConvertToC(adc1_ordinary_valuetab[4], &raw_val);
    if (err_ntc == NTC_OK) SensStat.ntc_1_temp = Cal_Apply(&NtcCal_1, raw_val);
    // else
    // log_e("Ntc_ConvertToC error: %d", err_ntc);

    err_ntc = Ntc_ConvertToC(adc1_ordinary_valuetab[5], &raw_val);
    if (err_ntc == NTC_OK) SensStat.ntc_2_temp = Cal_Apply(&NtcCal_2, raw_val);
    // else
    // log_e("Ntc_ConvertToC error: %d", err_ntc);

    err_ntc = Ntc_ConvertToC(adc1_ordinary_valuetab[6], &raw_val);
    if (err_ntc == NTC_OK) SensStat.ntc_3_temp = Cal_Apply(&NtcCal_3, raw_val);
    // else
    // log_e("Ntc_ConvertToC error: %d", err_ntc);

    err_ntc = Ntc_ConvertToC(adc1_ordinary_valuetab[7], &raw_val);
    if (err_ntc == NTC_OK) SensStat.ntc_4_temp = Cal_Apply(&NtcCal_4, raw_val);
    // else
    // log_e("Ntc_ConvertToC error: %d", err_ntc);

    vTaskDelay(1000);
  }

  vTaskDelete(NULL);
}
