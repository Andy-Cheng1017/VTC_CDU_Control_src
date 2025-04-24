#ifndef SENSOR_TASK_H
#define SENSOR_TASK_H
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"

#define ADC1_CHANNEL_COUNT 9
#define ADC1_SAMPLE_COUNT 32
#define SMP_CNT_PWR (__builtin_ctz(ADC1_SAMPLE_COUNT))

// 確保 ADC1_SAMPLE_COUNT 是 2 的冪次（只有 1 個位元是 1）
_Static_assert((ADC1_SAMPLE_COUNT & (ADC1_SAMPLE_COUNT - 1)) == 0, "ADC1_SAMPLE_COUNT must be a power of 2");

extern uint16_t adc1_ordinary_valuetab[ADC1_SAMPLE_COUNT][ADC1_CHANNEL_COUNT];
extern TaskHandle_t sensor_handler;

typedef struct {
  int32_t ntc_1_temp_m;
  int32_t ntc_2_temp_m;
  int32_t ntc_3_temp_m;
  int32_t ntc_4_temp_m;
  int16_t press_1_val_kpa;
  int16_t press_2_val_kpa;
  int16_t press_3_val_kpa;
  int16_t press_4_val_kpa;
  uint16_t Flow_val;
  uint16_t voltage_input;
  uint16_t current_input;
  int32_t power_input;
  uint16_t leak_sensor;
  int16_t temperature;
  uint16_t humidity;
  int16_t dew_temp;

  uint16_t device_connected;
} SensStat_t;

extern SensStat_t SensStat;

typedef struct {
  uint16_t porpo_1_duty;
  uint16_t porpo_2_duty;
} SensCtrl_t;

extern SensCtrl_t SensCtrl;

typedef struct {
  int32_t ntc_1_raw_l_val;
  int32_t ntc_2_raw_l_val;
  int32_t ntc_3_raw_l_val;
  int32_t ntc_4_raw_l_val;
  int32_t ntc_1_raw_h_val;
  int32_t ntc_2_raw_h_val;
  int32_t ntc_3_raw_h_val;
  int32_t ntc_4_raw_h_val;
  int32_t ntc_ideal_l_val;
  int32_t ntc_ideal_h_val;
} NtcTwoCal_t;

extern NtcTwoCal_t NtcTwoCal;

typedef struct {
  int16_t press_1_raw_l_val;
  int16_t press_2_raw_l_val;
  int16_t press_3_raw_l_val;
  int16_t press_4_raw_l_val;
  int16_t press_1_raw_h_val;
  int16_t press_2_raw_h_val;
  int16_t press_3_raw_h_val;
  int16_t press_4_raw_h_val;
  int16_t press_ideal_l_val;
  int16_t press_ideal_h_val;
} PressTwoCal_t;

extern PressTwoCal_t PressTwoCal;

void sensor_task_function(void* pvParameters);

#endif  // SENSOR_TASK_H