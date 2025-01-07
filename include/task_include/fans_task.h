#ifndef FANS_TASK_H
#define FANS_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t fans_handler;

typedef struct {
  uint16_t fans_speed;
} fans_control_type;

extern fans_control_type fans_control;

typedef struct {
  uint16_t fan1_FB;
  uint16_t fan2_FB;
  uint16_t fan3_FB;
  uint16_t fan4_FB;
  uint16_t fan5_FB;
  uint16_t fan6_FB;
  uint16_t fan7_FB;
  uint16_t fan8_FB;
  uint16_t fan9_FB;
  uint16_t fan10_FB;
  uint16_t fan11_FB;
  uint16_t fan12_FB;
  uint16_t fan13_FB;
  uint16_t fan14_FB;
  uint16_t fan15_FB;
  uint16_t fan16_FB;
} fans_status_type;

extern fans_status_type fans_status;

void fans_task_function(void* pvParameters);

#endif  // FANS_TASK_H