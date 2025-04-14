#include "side_card_task.h"

#include "FreeRTOS.h"
#include "RS485.h"
#include "RS485_Region_handler.h"
#include "at32f403a_407_wk_config.h"
#include "main.h"
#include "task.h"

#define LOG_TAG "Side_Card_Task"
#include "elog.h"

#define SINGLE_DATA_MAX_SIZE 128

#define READ_CARD_TASK_PRIO 2
#define READ_CARD_STK_SIZE 512

#define RS485_READ_TIMEOUT 50
#define RS485_SIDECARD_READ_PERIOD 500

TaskHandle_t SideCardHandler;

TaskHandle_t ReadCardHandler;

TaskHandle_t WriteCardHandler;

DECLARE_RS485_BUFFERS(RsCard, SINGLE_DATA_MAX_SIZE);

Rs485_t RsCard = {
    .UART = USART2,
    .Mode = MASTER,
    .BaudRate = BR_115200,
    .DataBit = USART_DATA_8BITS,
    .StopBit = USART_STOP_1_BIT,
    .root = true,

    RS485_BUFFERS_INIT(RsCard, SINGLE_DATA_MAX_SIZE),

};

SensCardStat_t SensCardStat = {0};
SensCardCtrl_t SensCardCtrl = {0};

FansCardCtrl_t FansCardCtrl = {0};
FansCardStat_t FansCardStat = {0};

FanCardSysSet_t FanCardSysSet = {0};
FanCardSysDisp_t FanCardSysDisp = {0};

void USART2_IRQHandler(void) {
  if (usart_interrupt_flag_get(RsCard.UART, USART_RDBF_FLAG) != RESET) {
    usart_flag_clear(RsCard.UART, USART_RDBF_FLAG);
    RS485_Rx_Data_ISR(&RsCard);

  } else if (usart_interrupt_flag_get(RsCard.UART, USART_IDLEF_FLAG) != RESET) {
    usart_flag_clear(RsCard.UART, USART_IDLEF_FLAG);
    RS485_Rx_Cplt_ISR(&RsCard);

  } else if (usart_interrupt_flag_get(RsCard.UART, USART_TDBE_FLAG) != RESET) {
    usart_flag_clear(RsCard.UART, USART_TDBE_FLAG);
    usart_interrupt_enable(RsCard.UART, USART_TDBE_INT, FALSE);
    RS485_Tx_Data_ISR(&RsCard);
  }
}

void ReadCardTaskFunc(void* pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();

  RsError_t ret;

  while (1) {
    vTaskDelayUntil(&xLastWakeTime, RS485_SIDECARD_READ_PERIOD);

    xSemaphoreTake(RS485RegionMutex, RS485_SEMAPHORE_TIMEOUT);

    RsCard.tx_Func = READ_HOLDING_REGISTERS;
    RsCard.ip_addr = SENS_RS485_ADDR;
    RsCard.reg_hdle_stat = SENS_CARD_DATAREAD_REG_START;
    RsCard.reg_hdle_num = SENS_CARD_TOTAL_REG_NUM;

    ret = RS485WriteHandler(&RsCard, NULL, NULL);
    if (ret) {
      log_e("Sens Card Write Handler Error %d", ret);
      continue;
    }

    ret = RS485Write(&RsCard);
    if (ret) {
      log_e("Sens Card Write Error %d", ret);
      continue;
    }
    ulTaskNotifyTake(pdTRUE, RS485_READ_TIMEOUT);

    xSemaphoreGive(RS485RegionMutex);

    vTaskDelay(100);

    xSemaphoreTake(RS485RegionMutex, RS485_SEMAPHORE_TIMEOUT);

    RsCard.tx_Func = READ_HOLDING_REGISTERS;
    RsCard.ip_addr = FAN_RS485_ADDR;
    RsCard.reg_hdle_stat = FANS_CARD_REG_START;
    RsCard.reg_hdle_num = FANS_CARD_TOTAL_REG_NUM;

    ret = RS485WriteHandler(&RsCard, NULL, NULL);
    if (ret) {
      log_e("Fans Card Write Handler Error %d", ret);
      continue;
    }

    ret = RS485Write(&RsCard);
    if (ret) {
      log_e("Fans Card Write Error %d", ret);
      continue;
    }

    ulTaskNotifyTake(pdTRUE, RS485_READ_TIMEOUT);

    xSemaphoreGive(RS485RegionMutex);

    vTaskDelay(100);

    xSemaphoreTake(RS485RegionMutex, RS485_SEMAPHORE_TIMEOUT);

    RsCard.tx_Func = READ_HOLDING_REGISTERS;
    RsCard.ip_addr = FAN_RS485_ADDR;
    RsCard.reg_hdle_stat = FANS_CARD_SYS_SET_REG_START;
    RsCard.reg_hdle_num = FANS_CARD_SYS_DISP_REG_END - FANS_CARD_SYS_SET_REG_START + 1;

    ret = RS485WriteHandler(&RsCard, NULL, NULL);
    if (ret) {
      log_e("Fans Card Write Handler Error %d", ret);
      continue;
    }

    ret = RS485Write(&RsCard);
    if (ret) {
      log_e("Fans Card Write Error %d", ret);
      continue;
    }

    ulTaskNotifyTake(pdTRUE, RS485_READ_TIMEOUT);

    xSemaphoreGive(RS485RegionMutex);
  }
  vTaskDelete(NULL);
}

void WriteCardTaskFunc(void* pvParameters) {
  RsError_t ret;

  while (1) {
    if (ulTaskNotifyTake(pdTRUE, RS485_READ_TIMEOUT)) {
      xSemaphoreTake(RS485RegionMutex, RS485_SEMAPHORE_TIMEOUT);

      RsCard.tx_Func = WRITE_SINGLE_REGISTER;
      RsCard.ip_addr = write_ip;
      RsCard.reg_hdle_stat = write_card_address;

      ret = RS485WriteHandler(&RsCard, &write_card_data, sizeof(&write_card_data));
      if (ret) {
        log_e("Fans Card Write Handler Error %d", ret);
        continue;
      }

      ret = RS485Write(&RsCard);
      if (ret) {
        log_e("Fans Card Write Error %d", ret);
        continue;
      }

      xSemaphoreGive(RS485RegionMutex);
    }
  }
  vTaskDelete(NULL);
}

void SideCardTaskFunc(void* pvParameters) {
  log_i("Side Card Task Start");

  RsInit(&RsCard);

  xTaskCreate((TaskFunction_t)ReadCardTaskFunc, (const char*)"Read Card Task Func", (uint16_t)READ_CARD_STK_SIZE, (void*)NULL,
              (UBaseType_t)READ_CARD_TASK_PRIO, (TaskHandle_t*)&ReadCardHandler);
  vTaskDelay(100);
  xTaskCreate((TaskFunction_t)WriteCardTaskFunc, (const char*)"Write Card Task Func", (uint16_t)READ_CARD_STK_SIZE, (void*)NULL,
              (UBaseType_t)READ_CARD_TASK_PRIO, (TaskHandle_t*)&WriteCardHandler);
  vTaskDelay(100);

  RsError_t err;

  while (1) {
    if (RsChkAvailable(&RsCard)) {
      err = RS485Read(&RsCard);

      if (err == UNPKG_FINISH) {
        continue;
      } else if (err != RS485_OK) {
        log_e("Error reading from RS485: %d", err);
        continue;
      } else {
        if (RsCard.rx_Func == WRITE_MULTIPLE_REGISTERS) {
          xTaskNotifyGive(ReadCardHandler);
          continue;
        }
      }

      err = RS485ReadHandler(&RsCard);

      if (err != RS485_OK) {
        log_e("Error handling RS485: %d", err);
        continue;
      } else {
        xTaskNotifyGive(ReadCardHandler);
      }
    }

    vTaskDelay(25);
  }
}
