#ifndef STORE_DATA_TASK_H
#define STORE_DATA_TASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "sensor_task.h"
#include "pump_task.h"
#include "side_card_task.h"
#include "pt100_task.h"

extern TaskHandle_t store_data_handler;

typedef struct {
  SensStat_t SensStat;
  SensCtrl_t SensCtrl;
  Pt100Stat_t Pt100Stat;
  Pt100TwoCal_t Pt100TwoCal;
  pump_status_type pump_status;
  pump_control_type pump_control;
  FansCardStat_t FansCardStat;
  FansCardCtrl_t FansCardCtrl;
} data_sec_t;

typedef struct {
  SensStat_t SensStat;
  SensCtrl_t SensCtrl;
  Pt100Stat_t Pt100Stat;
  Pt100TwoCal_t Pt100TwoCal;
  pump_status_type pump_status;
  pump_control_type pump_control;
  FansCardStat_t FansCardStat;
  FansCardCtrl_t FansCardCtrl;
} data_ten_t;

typedef struct {
  SensStat_t SensStat;
  SensCtrl_t SensCtrl;
  Pt100Stat_t Pt100Stat;
  Pt100TwoCal_t Pt100TwoCal;
  pump_status_type pump_status;
  pump_control_type pump_control;
  FansCardStat_t FansCardStat;
  FansCardCtrl_t FansCardCtrl;
} data_min_t;

void store_data_task_function(void* pvParameters);

#endif  // STORE_DATA_TASK_H