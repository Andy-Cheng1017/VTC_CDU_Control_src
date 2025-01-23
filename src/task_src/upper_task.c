#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "upper_task.h"

#define LOG_TAG "UPPER_Task"
#include "elog.h"

TaskHandle_t upper_handler;

RsFunc_t UPPER_rx_Func = 0;
uint8_t UPPER_rx_Data[UPPER_DATA_MAX_SIZE] = {0};
uint8_t UPPER_rx_Data_len = 0;

RsFunc_t UPPER_tx_Func = 0;
uint8_t UPPER_tx_Data[UPPER_DATA_MAX_SIZE] = {0};
uint8_t UPPER_tx_Data_len = 0;

bool UPPER_Unpkg_Flag = FALSE;
bool UPPER_Decode_Flag = FALSE;

Rs485_t RS485_UPPER = {
    .UART = UART8,
    .Mode = SLAVE,
    .BaudRate = BR_115200,
    .DataBit = USART_DATA_8BITS,
    .StopBit = USART_STOP_1_BIT,
    .ip_addr = MY_485_ADDR,
};

void upper_task_function(void* pvParameters) {
  RsInit(&RS485_UPPER);
  log_i("UPPER Task Running");
  RS485_UPPER.reg_hdle_stat = 0x00;
  RS485_UPPER.reg_hdle_end = 0x2F;
  RsRegHdle(&RS485_UPPER, SysStat_Handler);
  RS485_UPPER.reg_hdle_stat = 0x30;
  RS485_UPPER.reg_hdle_end = 0x5F;
  RsRegHdle(&RS485_UPPER, DataRead_Handler);
  RS485_UPPER.reg_hdle_stat = 0x60;
  RS485_UPPER.reg_hdle_end = 0x7F;
  RsRegHdle(&RS485_UPPER, DevCtrl_Handler);
  RS485_UPPER.reg_hdle_stat = 0x80;
  RS485_UPPER.reg_hdle_end = 0x8F;
  RsRegHdle(&RS485_UPPER, EthConfig_Handler);

  while (1) {
    if (RS485_UPPER.rx_pkg_cplt_f == TRUE) {
      memset(UPPER_rx_Data, 0, UPPER_DATA_MAX_SIZE);
      RsError_t ret = RsUnpkg(&RS485_UPPER, &UPPER_rx_Func, UPPER_rx_Data, &UPPER_rx_Data_len);

      if (ret == UNPKG_FINISH) {
        log_i("485 Unpkg Finish");
      } else {
        log_i("UPPER rx Func: %d", UPPER_rx_Func);
        elog_hexdump("UPPER_rx_Data", 16, UPPER_rx_Data, sizeof(UPPER_rx_Data));
        if (!ret) {
          log_i("UPPER Unpkg Success");
          UPPER_Unpkg_Flag = TRUE;
        } else if (ret == UNPKG_OVER_PACKGE_SIZE) {
          log_e("485 Over Package Size");
          UPPER_Unpkg_Flag = FALSE;
        } else if (ret == CRC_ERROR) {
          log_e("485 CRC Error");
          UPPER_Unpkg_Flag = FALSE;
        } else if (ret == OTHER_ADDR)
          log_i("485 Not My Address");
      }
    }

    if (UPPER_rx_Func != 0 && UPPER_Unpkg_Flag) {
      memset(UPPER_tx_Data, 0, UPPER_DATA_MAX_SIZE);
      RsError_t ret = RsDecd(&RS485_UPPER, UPPER_rx_Func, UPPER_rx_Data, UPPER_rx_Data_len, 
                                       &UPPER_tx_Func, UPPER_tx_Data, &UPPER_tx_Data_len);
      log_i("UPPER tx Func: %d", UPPER_tx_Func);
      elog_hexdump("UPPER_tx_Data", 16, UPPER_tx_Data, sizeof(UPPER_tx_Data));
      if (!ret) {
        log_i("UPPER Decode Success");
      } else if (ret == ILLIGAL_FUNC)
        log_e("485 Illegal Function");
      else if (ret == ILLIGAL_DATA_ADDR)
        log_e("485 Illegal Data Address");
      else if (ret == ILLIGAL_DATA_VALUE)
        log_e("485 Illegal Data Value");
      else if (ret == SLAVE_DEVICE_FAILURE)
        log_e("485 Slave Device Failure");
      UPPER_rx_Func = 0;
      UPPER_rx_Data_len = 0;
      memset(UPPER_rx_Data, 0, UPPER_DATA_MAX_SIZE);
      UPPER_Unpkg_Flag = FALSE;
    }

    if (UPPER_tx_Func != 0) {
      RS485_Pkg(&RS485_UPPER, MY_485_ADDR, UPPER_tx_Func, UPPER_tx_Data, UPPER_tx_Data_len);
      UPPER_tx_Func = 0;
      UPPER_tx_Data_len = 0;
      memset(UPPER_tx_Data, 0, UPPER_DATA_MAX_SIZE);
    }
    vTaskDelay(25);
  }
  vTaskDelete(upper_handler);
}