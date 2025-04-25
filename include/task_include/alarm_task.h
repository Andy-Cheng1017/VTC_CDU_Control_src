#ifndef ALARM_TASK_H
#define ALARM_TASK_H

#include "FreeRTOS.h"
#include "task.h"

#define INLET_PRESSURE_CHANNEL SensStat.press_1_val_kpa
#define OUTLET_PRESSURE_CHANNEL SensStat.press_2_val_kpa
#define RETURN_PRESSURE_CHANNEL SensCardStat.press_1_val_kpa

#define FLOW_CHANNEL SensStat.Flow_val

#define OUTLET_TEMP_CHANNEL SensCardStat.pt100_temp_m[0]
#define AMBIENT_TEMP_CHANNEL SensStat.temperature

#define LEAK_SENSOR_CHANNEL SensStat.leak_sensor
#define CUD_LEAK_CHANNEL SensCardStat.leak_sensor
#define SERVER_LEAK_CHANNEL SensCardStat.leak_sensor

extern TaskHandle_t alarm_handler;

void alarm_task_function(void* pvParameters);

#endif