#include <stdio.h>
#include <string.h>
#include "sensor_task.h"
#include "wk_adc.h"
#include "wk_dma.h"
#include "NTC.h"
#include "SensConvVal.h"
#include "Two_Pt_Cal.h"
#include "FG_RPM.h"

#define LOG_TAG "Sensor_Task"
#include "elog.h"

#define SET_BIT_TO(var, bit, value) ((var) = (value) ? ((var) | (1 << (bit))) : ((var) & ~(1 << (bit))))

#define PUMP_1_EXITS_BIT 0
#define PUMP_2_EXITS_BIT 1

uint16_t adc1_ordinary_valuetab[ADC1_SAMPLE_COUNT][ADC1_CHANNEL_COUNT] = {0};

TaskHandle_t sensor_handler;

FgParam_t Flow_Fg = {
    .timer_count = 3,
    .tmr_list = {TMR11, TMR10, TMR9},
    .exint_line = EXINT_LINE_15,
    .motor_phase = 6,
};

void EXINT15_10_IRQHandler(void) {
  if (exint_interrupt_flag_get(EXINT_LINE_12) != RESET) {
    exint_flag_clear(EXINT_LINE_12);
    SET_BIT_TO(SensStat.device_connected, PUMP_1_EXITS_BIT, !gpio_input_data_bit_read(GPIOD, GPIO_PINS_12));
  } else if (exint_interrupt_flag_get(EXINT_LINE_13) != RESET) {
    exint_flag_clear(EXINT_LINE_13);
    SET_BIT_TO(SensStat.device_connected, PUMP_2_EXITS_BIT, !gpio_input_data_bit_read(GPIOD, GPIO_PINS_13));
  } else if (exint_interrupt_flag_get(EXINT_LINE_14) != RESET) {
    exint_flag_clear(EXINT_LINE_14);
    SET_BIT_TO(SensStat.leak_sensor, 0, gpio_input_data_bit_read(GPIOE, GPIO_PINS_14));
  } else if (exint_interrupt_flag_get(EXINT_LINE_15)) {
    exint_flag_clear(EXINT_LINE_15);
    FgExintIntSampling(&Flow_Fg);
  }
}

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
    .porpo_1_duty = 0,
    .porpo_2_duty = 0,
};

void sensor_task_function(void* pvParameters) {
  log_i("Sensor Task Running");

  exint_interrupt_enable(EXINT_LINE_12, TRUE);
  exint_software_interrupt_event_generate(EXINT_LINE_12);
  vTaskDelay(1);
  exint_interrupt_enable(EXINT_LINE_13, TRUE);
  exint_software_interrupt_event_generate(EXINT_LINE_13);
  vTaskDelay(1);
  exint_interrupt_enable(EXINT_LINE_14, TRUE);
  exint_software_interrupt_event_generate(EXINT_LINE_14);
  vTaskDelay(1);

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

  FgInit(&Flow_Fg);

  vTaskDelay(5);

  while (1) {
    uint32_t adc_sum_val[ADC1_CHANNEL_COUNT] = {0};
    for (int i = 0; i < ADC1_CHANNEL_COUNT; i++) {
      for (int j = 0; j < ADC1_SAMPLE_COUNT; j++) {
        adc_sum_val[i] += adc1_ordinary_valuetab[j][i];
      }
    }

    err_conv = Conv_GetVal_Volt(&PressConv, (((float)(adc_sum_val[0] >> SMP_CNT_PWR)) / (adc_sum_val[8] >> SMP_CNT_PWR)) * 1.2f, &raw_val);
    SensStat.press_1_val_kpa = (int16_t)Cal_Apply(&PressCal_1, raw_val);

    err_conv = Conv_GetVal_Volt(&PressConv, (((float)(adc_sum_val[1] >> SMP_CNT_PWR)) / (adc_sum_val[8] >> SMP_CNT_PWR)) * 1.2f, &raw_val);
    SensStat.press_2_val_kpa = (int16_t)Cal_Apply(&PressCal_2, raw_val);

    err_conv = Conv_GetVal_Volt(&PressConv, (((float)(adc_sum_val[2] >> SMP_CNT_PWR)) / (adc_sum_val[8] >> SMP_CNT_PWR)) * 1.2f, &raw_val);
    SensStat.press_3_val_kpa = (int16_t)Cal_Apply(&PressCal_3, raw_val);

    err_conv = Conv_GetVal_Volt(&PressConv, (((float)(adc_sum_val[3] >> SMP_CNT_PWR)) / (adc_sum_val[8] >> SMP_CNT_PWR)) * 1.2f, &raw_val);
    SensStat.press_4_val_kpa = (int16_t)Cal_Apply(&PressCal_4, raw_val);

    err_ntc = Ntc_ConvertToC(adc_sum_val[4] >> SMP_CNT_PWR, &raw_val);
    SensStat.ntc_1_temp_m = Cal_Apply(&NtcCal_1, raw_val);

    err_ntc = Ntc_ConvertToC(adc_sum_val[5] >> SMP_CNT_PWR, &raw_val);
    SensStat.ntc_2_temp_m = Cal_Apply(&NtcCal_2, raw_val);

    err_ntc = Ntc_ConvertToC(adc_sum_val[6] >> SMP_CNT_PWR, &raw_val);
    SensStat.ntc_3_temp_m = Cal_Apply(&NtcCal_3, raw_val);

    err_ntc = Ntc_ConvertToC(adc_sum_val[7] >> SMP_CNT_PWR, &raw_val);
    SensStat.ntc_4_temp_m = Cal_Apply(&NtcCal_4, raw_val);

    FgGetRPM(&Flow_Fg, &SensStat.Flow_val);

    vTaskDelay(500);
  }

  vTaskDelete(NULL);
}
