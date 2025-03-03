#include "FreeRTOS.h"
#include "task.h"
#include "side_card_task.h"

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

uint16_t fan_duty[16] = {0};

SensCardStat_t SensCardStat = {0};

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
    RsCard.tx_idex--;
    RS485_Tx_Data_ISR(&RsCard);
  }
}

void SideCardTaskFunc(void* pvParameters) {
  RsInit(&RsCard);

  RsCard.reg_hdle_stat = SENS_CARD_REG_START;
  RsCard.reg_hdle_end = SENS_CARD_REG_END;
  RsCard.reg_hdle_num = SENS_CARD_TOTAL_REG_NUM;
  RsRegHdle(&RsCard, DataRead_Handler);

  RsCard.reg_hdle_stat = FANS_CARD_REG_START;
  RsCard.reg_hdle_end = FANS_CARD_REG_END;
  RsCard.reg_hdle_num = FANS_CARD_TOTAL_REG_NUM;
  RsRegHdle(&RsCard, FansCardHdle);

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
        log_i("RS485 Read Success");
        elog_hexdump("Card_rx_Data", 32, RsCard.rx_Data, sizeof(RsCard.rx_Data));
        if (RsCard.rx_Func == WRITE_MULTIPLE_REGISTERS) {
          continue;
        }
      }

      err = RS485ReadHandler(&RsCard);

      if (err != RS485_OK) {
        log_e("Error handling RS485: %d", err);
        continue;
      } else {
        log_i("RS485 Handler Success");
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
    }

    notificationValue = ulTaskNotifyTake(pdTRUE, RS485_READ_TIMEOUT);

    if (notificationValue > 0) {
      RsCard.tx_Func = WRITE_MULTIPLE_REGISTERS;
      RsCard.ip_addr = FANS_CARD_ADDR;
      RsCard.reg_hdle_stat = FANS_CARD_WRITE_REG_START;
      RsCard.reg_hdle_num = FANS_CARD_WRITE_NUM;

      memset(fan_duty, 1, sizeof(fan_duty));

      ret = RS485WriteHandler(&RsCard, fan_duty, sizeof(fan_duty));
      if (ret) {
        log_e("Fans Card Write Handler Error %d", ret);
        continue;
      }

      ret = RS485Write(&RsCard);
      if (ret) {
        log_e("Fans Card Write Error %d", ret);
        continue;
      }
    }
  }
  vTaskDelete(NULL);
}
