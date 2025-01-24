#ifndef PT100_TASK_H
#define PT100_TASK_H

#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "pt100.h"
#include "MCP342x_wrap.h"
#include "Two_Pt_Cal.h"

extern TaskHandle_t pt100_handler;

typedef struct {
  int32_t pt100_1_temp_m;
  int32_t pt100_2_temp_m;
  int32_t pt100_3_temp_m;
  int32_t pt100_4_temp_m;
} Pt100Stat_t;

extern Pt100Stat_t Pt100Stat;

typedef struct {
  uint32_t pt100_1_raw_l_val;
  uint32_t pt100_2_raw_l_val;
  uint32_t pt100_3_raw_l_val;
  uint32_t pt100_4_raw_l_val;
  uint32_t pt100_1_raw_h_val;
  uint32_t pt100_2_raw_h_val;
  uint32_t pt100_3_raw_h_val;
  uint32_t pt100_4_raw_h_val;
  uint32_t pt100_ideal_l_val;
  uint32_t pt100_ideal_h_val;
} Pt100Ctrl_t;

extern Pt100Ctrl_t Pt100Ctrl;

MCP342x_error_t err;

void pt100_task_function(void* pvParameters);

#endif // PT100_TASK_H