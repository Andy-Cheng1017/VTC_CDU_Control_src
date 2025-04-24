#ifndef SIDE_CARD_TASK_H
#define SIDE_CARD_TASK_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define FG_SAMPLE_COUNT_MAX 32

extern TaskHandle_t SideCardHandler;
extern TaskHandle_t ReadCardHandler;
extern TaskHandle_t WriteCardHandler;

typedef struct {
  int32_t pt100_1_temp_m;
  int32_t pt100_2_temp_m;
  int32_t pt100_3_temp_m;
  int32_t pt100_4_temp_m;
  int16_t press_1_val_kpa;
  int16_t press_2_val_kpa;
  uint8_t leak_sensor;
  int16_t temperature;
  uint16_t humidity;
} SensCardStat_t;

extern SensCardStat_t SensCardStat;

typedef struct {
  uint16_t pressure_pump;
} SensCardCtrl_t;

extern SensCardCtrl_t SensCardCtrl;

typedef struct {
  uint16_t fan_pwm[16];
} FansCardCtrl_t;

extern FansCardCtrl_t FansCardCtrl;

typedef struct {
  uint16_t fan_fg[16];
} FansCardStat_t;

extern FansCardStat_t FansCardStat;

typedef enum {
  ONLY_WARNING_FAN,
  WARNING_AND_STOP_FAN,
} AlarmAction_FAN_t;

typedef struct {
  AlarmAction_FAN_t act;
  int16_t delay;
  uint16_t fan_low_speed_warning_threshold;
  uint16_t fan_fg_difference_warning_threshold;
} FanCardAlarm_t;

typedef struct {
  bool auto_control;
  uint16_t auto_control_target_speed;
  uint16_t fan_installation_status;

  FanCardAlarm_t fan_alarm;

  uint16_t fan_speed_sampling_interval_ms;
  uint8_t weighted_moving_average_count;

} FanCardSysSet_t;

extern FanCardSysSet_t FanCardSysSet;

typedef struct {
  uint16_t fan_fault_status;
  uint16_t fan_status_on_fan_board_bitfield_0_15;
  uint8_t fan_count;
} FanCardSysDisp_t;

extern FanCardSysDisp_t FanCardSysDisp;

void SideCardTaskFunc(void* pvParameters);
// void ReadCardTaskFunc(void* pvParameters);

#define MAX_REG_COUNT 16
typedef struct {
  uint8_t  function_code;    // 單寫或連寫
  uint8_t  ip_addr;          // 目標卡 IP
  uint16_t reg_addr;         // 起始寄存器地址
  uint16_t quantity;         // 寫入暫存器個數 (1 表示單寫)
  uint16_t data[MAX_REG_COUNT]; // 寫入資料，單寫用 data[0]
} RS485WriteRequest_t;

extern QueueHandle_t xWriteReqQueue;

#define WRITE_CARD_SINGLE(ip, addr, val)                            \
    do {                                                            \
        RS485WriteRequest_t _req;                                   \
        _req.function_code = WRITE_SINGLE_REGISTER;                 \
        _req.ip_addr       = (uint8_t)(ip);                         \
        _req.reg_addr      = (uint16_t)(addr);                      \
        _req.quantity      = 1;                                     \
        _req.data[0]       = (uint16_t)(val);                       \
        if (xWriteReqQueue) {                                       \
            xQueueSend(xWriteReqQueue, &_req, pdMS_TO_TICKS(50));  \
        }                                                           \
    } while (0)

// 多重寄存器連續寫入
#define WRITE_CARD_MULTIPLE(ip, addr, count, ptr)                   \
    do {                                                            \
        RS485WriteRequest_t _req;                                   \
        _req.function_code = WRITE_MULTIPLE_REGISTERS;              \
        _req.ip_addr       = (uint8_t)(ip);                         \
        _req.reg_addr      = (uint16_t)(addr);                      \
        _req.quantity      = (uint16_t)(count);                     \
        /* 複製資料到本地 buffer */                                    \
        for (uint16_t _i = 0; _i < (count) && _i < MAX_REG_COUNT; ++_i) { \
            _req.data[_i] = (ptr)[_i];                              \
        }                                                           \
        if (xWriteReqQueue) {                                       \
            xQueueSend(xWriteReqQueue, &_req, pdMS_TO_TICKS(50));  \
        }                                                           \
    } while (0)

#endif  // SIDE_CARD_TASK_H