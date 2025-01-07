#ifndef STORE_DATA_TASK_H
#define STORE_DATA_TASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "sensor_task.h"
#include "pump_task.h"
#include "fans_task.h"

extern TaskHandle_t store_data_handler;

typedef struct {
    sensor_status_type sensor_status;
    sensor_control_type sensor_control;
    pump_status_type pump_status;
    pump_control_type pump_control;
    fans_status_type fans_status;
    fans_control_type fans_control;
} data_sec_t;

typedef struct {
    sensor_status_type sensor_status;
    sensor_control_type sensor_control;
    pump_status_type pump_status;
    pump_control_type pump_control;
    fans_status_type fans_status;
    fans_control_type fans_control;
} data_ten_t;

typedef struct {
    sensor_status_type sensor_status;
    sensor_control_type sensor_control;
    pump_status_type pump_status;
    pump_control_type pump_control;
    fans_status_type fans_status;
    fans_control_type fans_control;
} data_min_t;

void store_data_task_function(void* pvParameters);

#endif  // STORE_DATA_TASK_H