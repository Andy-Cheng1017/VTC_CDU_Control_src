#include <stdio.h>
#include <string.h>
#include "sensor_task.h"
#include "wk_adc.h"
#include "wk_dma.h"
#include "NTC.h"
#include "SensConvVal.h"
#include "Two_Pt_Cal.h"

#define LOG_TAG "Sensor_Task"
#include "elog.h"

uint16_t adc1_ordinary_valuetab[ADC1_SAMPLE_NUM][ADC1_CHANNEL_NUM] = {0};

TaskHandle_t sensor_handler;

NtcTwoCal_t NtcTwoCal = {
    .ntc_1_raw_l_val = 24987,
    .ntc_2_raw_l_val = 24987,
    .ntc_3_raw_l_val = 24962,
    .ntc_4_raw_l_val = 24836,
    .ntc_1_raw_h_val = 103662,
    .ntc_2_raw_h_val = 103529,
    .ntc_3_raw_h_val = 103529,
    .ntc_4_raw_h_val = 103529,
    .ntc_ideal_l_val = 25000,   // 10kohm
    .ntc_ideal_h_val = 103779,  // 900ohm
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

  int32_t raw_val = 0;

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
    // adc_ordinary_software_trigger_enable(ADC1, TRUE);
    // while (dma_flag_get(DMA1_FDT1_FLAG) == RESET);
    // dma_flag_clear(DMA1_FDT1_FLAG);

    uint32_t adc_sum_val[ADC1_CHANNEL_NUM] = {0};
    for (int i = 0; i < ADC1_CHANNEL_NUM; i++) {
      for (int j = 0; j < ADC1_SAMPLE_NUM; j++) {
        adc_sum_val[i] += adc1_ordinary_valuetab[j][i];
      }
    }

    err_conv = Conv_GetVal_Volt(&PressConv, (((float)(adc_sum_val[0] >> SMP_NUM_PWR)) / (adc_sum_val[8] >> SMP_NUM_PWR)) * 1.2f, &raw_val);
    SensStat.press_1_val = (int16_t)Cal_Apply(&PressCal_1, raw_val);

    err_conv = Conv_GetVal_Volt(&PressConv, (((float)(adc_sum_val[1] >> SMP_NUM_PWR)) / (adc_sum_val[8] >> SMP_NUM_PWR)) * 1.2f, &raw_val);
    SensStat.press_2_val = (int16_t)Cal_Apply(&PressCal_2, raw_val);

    err_conv = Conv_GetVal_Volt(&PressConv, (((float)(adc_sum_val[2] >> SMP_NUM_PWR)) / (adc_sum_val[8] >> SMP_NUM_PWR)) * 1.2f, &raw_val);
    SensStat.press_3_val = (int16_t)Cal_Apply(&PressCal_3, raw_val);

    err_conv = Conv_GetVal_Volt(&PressConv, (((float)(adc_sum_val[3] >> SMP_NUM_PWR)) / (adc_sum_val[8] >> SMP_NUM_PWR)) * 1.2f, &raw_val);
    SensStat.press_4_val = (int16_t)Cal_Apply(&PressCal_4, raw_val);

    err_ntc = Ntc_ConvertToC(adc_sum_val[4] >> SMP_NUM_PWR, &raw_val);
    SensStat.ntc_1_temp = Cal_Apply(&NtcCal_1, raw_val);

    err_ntc = Ntc_ConvertToC(adc_sum_val[5] >> SMP_NUM_PWR, &raw_val);
    SensStat.ntc_2_temp = Cal_Apply(&NtcCal_2, raw_val);

    err_ntc = Ntc_ConvertToC(adc_sum_val[6] >> SMP_NUM_PWR, &raw_val);
    SensStat.ntc_3_temp = Cal_Apply(&NtcCal_3, raw_val);

    err_ntc = Ntc_ConvertToC(adc_sum_val[7] >> SMP_NUM_PWR, &raw_val);
    SensStat.ntc_4_temp = Cal_Apply(&NtcCal_4, raw_val);

    vTaskDelay(500);
  }

  vTaskDelete(NULL);
}
