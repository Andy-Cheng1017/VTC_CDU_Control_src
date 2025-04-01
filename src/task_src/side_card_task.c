#include "FreeRTOS.h"
#include "task.h"
#include "side_card_task.h"
#include "main.h"
#include "at32f403a_407_wk_config.h"
#include "RS485.h"
#include "RS485_Region_handler.h"

#define LOG_TAG "Side_Card_Task"
#include "elog.h"

TaskHandle_t SideCardHandler;

TaskHandle_t ReadCardHandler;

Rs485_t RsCard = {
    .UART = USART2,
    .Mode = MASTER,
    .BaudRate = BR_115200,
    .DataBit = USART_DATA_8BITS,
    .StopBit = USART_STOP_1_BIT,
    .root = true,
};

SensCardStat_t SensCardStat = {0};
SensCardCtrl_t SensCardCtrl = {0};

FansCardCtrl_t FansCardCtrl = {0};
FansCardStat_t FansCardStat = {0};

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

void SideCardTaskFunc(void* pvParameters) {
  RsInit(&RsCard);

  // RsCard.reg_hdle_stat = SENS_CARD_REG_START;
  // RsCard.reg_hdle_end = SENS_CARD_REG_END;
  // // RsCard.reg_hdle_num = SENS_CARD_TOTAL_REG_NUM;
  // RsRegHdle(&RsCard, DataRead_Handler);

  // RsCard.reg_hdle_stat = FANS_CARD_REG_START;
  // RsCard.reg_hdle_end = FANS_CARD_REG_END;
  // // RsCard.reg_hdle_num = FANS_CARD_TOTAL_REG_NUM;
  // RsRegHdle(&RsCard, FansCardHdle);

  xTaskCreate((TaskFunction_t)ReadCardTaskFunc, (const char*)"Read Card Task Func", (uint16_t)READ_CARD_STK_SIZE, (void*)NULL,
              (UBaseType_t)READ_CARD_TASK_PRIO, (TaskHandle_t*)&ReadCardHandler);
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
        // log_i("RS485 Read Success %X %X", RsCard.ip_addr, RsCard.rx_Func);
        // elog_hexdump("Card_rx_Data", 32, RsCard.rx_Data, sizeof(RsCard.rx_Data) / 2);
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
        // log_i("RS485 Handler Success");
        xTaskNotifyGive(ReadCardHandler);
      }
    }

    vTaskDelay(25);
  }
}

void ReadCardTaskFunc(void* pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();

  RsError_t ret;

  uint32_t notificationValue = 0;

  while (1) {
    vTaskDelayUntil(&xLastWakeTime, RS485_SIDECARD_READ_PERIOD);

    xSemaphoreTake(RS485RegionMutex, RS485_SEMAPHORE_TIMEOUT);

    RsCard.tx_Func = READ_HOLDING_REGISTERS;
    RsCard.ip_addr = SENS_CARD_ADDR;
    RsCard.reg_hdle_stat = SENS_CARD_REG_START;
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
    notificationValue = ulTaskNotifyTake(pdTRUE, RS485_READ_TIMEOUT);

    if (notificationValue > 0) {
    }
    xSemaphoreGive(RS485RegionMutex);

    vTaskDelay(100);

    xSemaphoreTake(RS485RegionMutex, RS485_SEMAPHORE_TIMEOUT);

    RsCard.tx_Func = WRITE_MULTIPLE_REGISTERS;
    RsCard.ip_addr = FANS_CARD_ADDR;
    RsCard.reg_hdle_stat = FANS_CARD_WRITE_REG_START;
    RsCard.reg_hdle_num = FANS_CARD_WRITE_NUM;

    ret = RS485WriteHandler(&RsCard, FansCardCtrl.fan_duty, sizeof(FansCardCtrl.fan_duty));
    if (ret) {
      log_e("Fans Card Write Handler Error %d", ret);
      continue;
    }

    ret = RS485Write(&RsCard);
    if (ret) {
      log_e("Fans Card Write Error %d", ret);
      continue;
    } else {
      // log_i("Fans Card Write Success: %X %X", RsCard.ip_addr, RsCard.tx_Func);
      // elog_hexdump("FansCardCtrl", 32, RsCard.tx_Data, sizeof(RsCard.tx_Data) / 2);
    }

    notificationValue = ulTaskNotifyTake(pdTRUE, RS485_READ_TIMEOUT);

    xSemaphoreGive(RS485RegionMutex);

    vTaskDelay(100);

    xSemaphoreTake(RS485RegionMutex, RS485_SEMAPHORE_TIMEOUT);

    if (notificationValue > 0) {

      RsCard.tx_Func = READ_HOLDING_REGISTERS;
      RsCard.ip_addr = FANS_CARD_ADDR;
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
      }else{
        // log_i("Fans Card Write Success: %X %X", RsCard.ip_addr, RsCard.tx_Func);
        // elog_hexdump("FansCardCtrl", 32, RsCard.tx_Data, sizeof(RsCard.tx_Data) / 2);
      }

      notificationValue = ulTaskNotifyTake(pdTRUE, RS485_READ_TIMEOUT);
      if (notificationValue > 0) {
      }
    }

    xSemaphoreGive(RS485RegionMutex);
  }
  vTaskDelete(NULL);
}
