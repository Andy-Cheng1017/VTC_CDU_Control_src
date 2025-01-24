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
};

void sensor_task_function(void* pvParameters) {
  log_i("Sensor Task Running");

  vTaskDelete(NULL);
}
