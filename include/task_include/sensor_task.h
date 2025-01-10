#ifndef SENSOR_TASK_H
#define SENSOR_TASK_H
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "RS485.h"
#include "RS485_Region_handler.h"

#define SENSOR_CARD_ADDR 0x22
#define SENS_DATA_MAX_SIZE 32

#define SENS_CARD_REG_START 0x0010
#define SENS_CARD_TOTLA_TEG_NUM 7

#define FANS_TASK_PRIO 4
#define FANS_STK_SIZE 512

extern TaskHandle_t sensor_handler;
extern TaskHandle_t sensor_card_handler;
extern TaskHandle_t fans_handler;

typedef struct {
  int16_t PT100_1_temp; //0x0010
  int16_t PT100_2_temp; //0x0011
  uint16_t AI1; //0x0012
  uint16_t AI2; //0x0013
  int16_t Temperture;   //0x0014
  uint16_t Humidity;  //0x0015
  bool DI1; //0x0016
  bool DI2;
  bool DI3;
  bool DI4;
} sens_card_stat_t;

extern sens_card_stat_t sens_card_stat;


typedef struct {
  int16_t PT100_1_temp;
  int16_t PT100_2_temp;
  int16_t PT100_3_temp;
  int16_t PT100_4_temp;
  int16_t NTC_1_temp;
  int16_t NTC_2_temp;
  int16_t NTC_3_temp;
  int16_t NTC_4_temp;
  int16_t Presure_1_val;
  int16_t Presure_2_val;
  int16_t Presure_3_val;
  int16_t Presure_4_val;
  int16_t Flow_val;
  uint16_t voltage_input;
  uint16_t current_input;
  uint32_t power_input;
} sens_stat_t;

extern sens_stat_t sens_stat;

typedef struct {
  uint16_t Porpo_1_PWM;
  uint16_t Porpo_2_PWM;
} sens_ctrl_t;

extern sens_ctrl_t sensor_control;

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

void sensor_task_function(void* pvParameters);
void Sensor_Card_Task(void* pvParameters);
void fans_task_function(void* pvParameters);

#endif  // SENSOR_TASK_H