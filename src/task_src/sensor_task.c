#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "sensor_task.h"

#define LOG_TAG "Sensor_Task"
#include "elog.h"

TaskHandle_t sensor_handler;
TaskHandle_t sensor_card_handler;
TaskHandle_t fans_handler;

rs485_func_t sens_rx_Func = 0;
uint8_t sens_rx_Data[SENS_DATA_MAX_SIZE] = {0};
uint8_t sens_rx_Data_len = 0;

rs485_func_t sens_tx_Func = 0;
uint8_t sens_tx_Data[SENS_DATA_MAX_SIZE] = {0};
uint8_t sens_tx_Data_len = 0;

rs485_t RS485_sens = {
    .UART = UART4,
    .Mode = MASTER,
    .BaudRate = BR_115200,
    .DataBit = USART_DATA_8BITS,
    .StopBit = USART_STOP_1_BIT,
    .IpAddr = SENSOR_CARD_ADDR,
};

uint8_t Data_or_Num[2] = {SENS_CARD_TOTLA_TEG_NUM};

sens_stat_t sens_stat = {0};
sens_ctrl_t sensor_control = {0};

sens_card_stat_t sens_card_stat = {0};

fans_control_type fans_control = {0};
fans_status_type fans_status = {0};

void sensor_task_function(void* pvParameters) {
  xTaskCreate((TaskFunction_t)Sensor_Card_Task, "Sensor_Card_Task", 256, NULL, 2, (TaskHandle_t*)&sensor_handler);
  xTaskCreate((TaskFunction_t)fans_task_function, (const char*)"Fans_task", (uint16_t)FANS_STK_SIZE, (void*)NULL, (UBaseType_t)FANS_TASK_PRIO,
              (TaskHandle_t*)&fans_handler);
  while (1) {
    vTaskDelay(1000);
  }
  vTaskDelete(NULL);
}

void Sensor_Card_Task(void* pvParameters) {
  RS485_init(&RS485_sens);
  RS485_sens.RegHdlerStat = 0x0010;
  RS485_sens.RegHdlerEnd = 0x001F;
  RS485_RegisterHandler(&RS485_sens, SensCard_Handler);

  while (1) {
    sens_tx_Func = READ_HOLDING_REGISTERS;
    rs485_error_t ret = RS485_Encode(&RS485_sens, sens_tx_Func, SENS_CARD_REG_START, Data_or_Num, NULL, sens_tx_Data, &sens_tx_Data_len);
    if (!ret) {
      log_i("Sensor Card Encode Success");
      RS485_Pkg(&RS485_sens, SENSOR_CARD_ADDR, sens_tx_Func, sens_tx_Data, sens_tx_Data_len);
    } else if (ret == ENCODE_FOR_NUMBER)
      log_e("Sensor Card Encode ERRO For Number");
    else if (ret == ENCODE_FOR_SINGLE_DATA)
      log_e("Sensor Card Encode ERRO For Single Data");

    if (RS485_sens.RxPkgCpltFlag == TRUE) {
      memset(sens_rx_Data, 0, SENS_DATA_MAX_SIZE);
      rs485_error_t ret = RS485_Unpkg(&RS485_sens, &sens_rx_Func, sens_rx_Data, &sens_rx_Data_len);
      if (!ret) {
        log_i("sens Unpkg Success");
        log_i("sens Func: %d", sens_rx_Func);
        log_i("sens Data: %d", sens_rx_Data);
      } else if (ret == UNPKG_OVER_PACKGE_SIZE)
        log_e("485 Over Package Size");
      else if (ret == CRC_ERROR)
        log_e("485 CRC Error");
      else if (ret == NOT_MY_ADDR)
        log_i("485 Not My Address");
    }

    if (sens_rx_Func != 0) {
      rs485_error_t ret = RS485_Decode(&RS485_sens, sens_rx_Func, sens_rx_Data, sens_rx_Data_len, NULL, NULL, NULL);
      if (!ret) {
        log_i("sens Decode Success");
        log_i("sens Func: %d", sens_tx_Func);
        log_i("sens Data: %d", sens_tx_Data);
      } else if (ret == ILLIGAL_FUNC)
        log_e("485 Illegal Function");
      else if (ret == ILLIGAL_DATA_ADDR)
        log_e("485 Illegal Data Address");
      else if (ret == ILLIGAL_DATA_VALUE)
        log_e("485 Illegal Data Value");
      else if (ret == SLAVE_DEVICE_FAILURE)
        log_e("485 Slave Device Failure");
      sens_rx_Func = 0;
      sens_rx_Data_len = 0;
      memset(sens_rx_Data, 0, SENS_DATA_MAX_SIZE);
    }

    vTaskDelay(1000);
  }
  vTaskDelete(NULL);
}

void fans_task_function(void* pvParameters) {
    while (1) {
        vTaskDelay(500);
    }
}