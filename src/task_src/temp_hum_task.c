#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#define SINGLE_DATA_MAX_SIZE 16
#define MAX_CIRCLE_BUFFER_SIZE 32
#define MAX_PKG_SIZE 16

#include "RS485.h"
#include "sensor_task.h"
#include "temp_hum_task.h"

#define LOG_TAG "Temp_Hum_Task"
#include "elog.h"

#define SINGLE_DATA_MAX_SIZE 16

#define TEMP_HUM_TASK_PERIOD 500

#define TEMP_HUM_ADDR 0x01
#define TEMP_HUM_REG_START 0x00
#define TEMP_HUM_TOTAL_REG_NUM 3

#define TEMP_HUM_EXITS_BIT 2
#define SET_BIT_TO(var, bit, value) ((var) = (value) ? ((var) | (1 << (bit))) : ((var) & ~(1 << (bit))))

TaskHandle_t temp_hum_handler;

DECLARE_RS485_BUFFERS(RsTempHum, SINGLE_DATA_MAX_SIZE);

Rs485_t RsTempHum = {
    .UART = USART3,
    .Mode = MASTER,
    .BaudRate = BR_9600,
    .DataBit = USART_DATA_8BITS,
    .StopBit = USART_STOP_1_BIT,
    .ip_addr = TEMP_HUM_ADDR,
    .root = true,
    .tx_Func = READ_HOLDING_REGISTERS,
    .reg_hdle_num = TEMP_HUM_TOTAL_REG_NUM,

    RS485_BUFFERS_INIT(RsTempHum, SINGLE_DATA_MAX_SIZE),

};

void USART3_IRQHandler(void) {
  if (usart_interrupt_flag_get(RsTempHum.UART, USART_RDBF_FLAG) != RESET) {
    usart_flag_clear(RsTempHum.UART, USART_RDBF_FLAG);
    RS485_Rx_Data_ISR(&RsTempHum);

  } else if (usart_interrupt_flag_get(RsTempHum.UART, USART_IDLEF_FLAG) != RESET) {
    usart_flag_clear(RsTempHum.UART, USART_IDLEF_FLAG);
    RS485_Rx_Cplt_ISR(&RsTempHum);

  } else if (usart_interrupt_flag_get(RsTempHum.UART, USART_TDBE_FLAG) != RESET) {
    usart_flag_clear(RsTempHum.UART, USART_TDBE_FLAG);
    usart_interrupt_enable(RsTempHum.UART, USART_TDBE_INT, FALSE);
    RS485_Tx_Data_ISR(&RsTempHum);
  }
}

void temp_hum_task_function(void *pvParameters) {
  log_i("Temp Hum Task Started");

  RsInit(&RsTempHum);

  TickType_t xLastWakeTime = xTaskGetTickCount();
  RsError_t ret;
  while (1) {
    vTaskDelayUntil(&xLastWakeTime, TEMP_HUM_TASK_PERIOD);

    RsTempHum.reg_hdle_stat = TEMP_HUM_REG_START;

    ret = RS485WriteHandler(&RsTempHum, NULL, NULL);
    if (ret) {
      log_e("Sens Card Write Handler Error %d", ret);
      continue;
    }

    ret = RS485Write(&RsTempHum);
    if (ret) {
      log_e("Sens Card Write Error %d", ret);
      continue;
    }

    TickType_t start_time = xTaskGetTickCount();

    while (1) {
      if (RsChkAvailable(&RsTempHum)) {
        ret = RS485Read(&RsTempHum);

        if (ret == UNPKG_FINISH) {
          break;
        } else if (ret != RS485_OK) {
          log_e("Error reading from RS485: %d", ret);
          break;
        } else {
          //   log_i("RS485 Read Success %X %X", RsTempHum.ip_addr, RsTempHum.rx_Func);
          //   elog_hexdump("Card_rx_Data", 32, RsTempHum.rx_Data, sizeof(RsTempHum.rx_Data) / 2);
        }
        RsTempHum.reg_hdle_stat = 0X0068;

        ret = RS485ReadHandler(&RsTempHum);

        if (ret != RS485_OK) {
          log_e("Error handling RS485: %d", ret);
        }
        SET_BIT_TO(SensStat.device_connected, TEMP_HUM_EXITS_BIT, true);
        break;
      }

      if ((xTaskGetTickCount() - start_time) > TEMP_HUM_TASK_PERIOD) {
        // log_e("RS485 read timeout");
        SET_BIT_TO(SensStat.device_connected, TEMP_HUM_EXITS_BIT, false);
        break;
      }

      vTaskDelay(25);
    }
  }
}