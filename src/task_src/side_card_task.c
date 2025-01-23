#include "FreeRTOS.h"
#include "task.h"
#include "side_card_task.h"

#define LOG_TAG "Side_Card_Task"
#include "elog.h"

TaskHandle_t SideCardHandler;
TaskHandle_t SensCardHandler;
TaskHandle_t FansHandler;

RsFunc_t CardRxFunc = 0;
uint8_t card_rx_data[CARD_DATA_MAX_SIZE] = {0};
uint8_t card_rx_data_len = 0;

RsFunc_t CardTxFunc = 0;
uint8_t card_tx_tata[CARD_DATA_MAX_SIZE] = {0};
uint8_t card_tx_tata_len = 0;

static bool Unpkg_Flag = FALSE;

Rs485_t RsCard = {
    .UART = UART4,
    .Mode = MASTER,
    .BaudRate = BR_115200,
    .DataBit = USART_DATA_8BITS,
    .StopBit = USART_STOP_1_BIT,
};

uint8_t data_or_num[2] = {SENS_CARD_TOTLA_REG_NUM};

SensCardStat_t SensCardStat = {0};

FansCardCtrl_t FansCardCtrl = {0};
FansCardStat_t FansCardStat = {0};

void SideCardTaskFunc(void* pvParameters) {
  RsInit(&RsCard);
  RsCard.reg_hdle_stat = 0x0010;
  RsCard.reg_hdle_end = 0x001F;
  RsRegHdle(&RsCard, SensCardHdle);
  RsCard.reg_hdle_stat = 0x0020;
  RsCard.reg_hdle_end = 0x0033;
  RsRegHdle(&RsCard, FansCardHdle);

  xTaskCreate((TaskFunction_t)SensCardTaskFunc, (const char*)"Sens Card Task Func", (uint16_t)SENS_STK_SIZE, (void*)NULL, (UBaseType_t)SENS_TASK_PRIO,
              (TaskHandle_t*)&SensCardHandler);
  vTaskDelay(100);
  xTaskCreate((TaskFunction_t)FansCardTaskFunc, (const char*)"Fans Card Task Func", (uint16_t)FANS_STK_SIZE, (void*)NULL, (UBaseType_t)FANS_TASK_PRIO,
              (TaskHandle_t*)&FansHandler);
  vTaskDelay(100);
  while (1) {
    if (RsCard.rx_pkg_cplt_f == TRUE) {
      memset(card_rx_data, 0, CARD_DATA_MAX_SIZE);
      card_rx_data_len = 0;
      RsError_t ret = RsUnpkg(&RsCard, &CardRxFunc, card_rx_data, &card_rx_data_len);

      if (ret == UNPKG_FINISH) {
        log_i("485 Unpkg Finish");
      } else {
        log_i("sens Func: %d", CardRxFunc);
        elog_hexdump("card_rx_Data", 16, card_rx_data, sizeof(card_rx_data));
        if (!ret) {
          log_i("sens Unpkg Success");
          Unpkg_Flag = TRUE;
        } else if (ret == UNPKG_OVER_PACKGE_SIZE) {
          log_e("485 Over Package Size");
          Unpkg_Flag = FALSE;
        } else if (ret == CRC_ERROR) {
          log_e("485 CRC Error");
          Unpkg_Flag = FALSE;
        } else if (ret == OTHER_ADDR)
          log_i("485 Not My Address");
      }
    }

    if (CardRxFunc != 0 && Unpkg_Flag) {
      RsError_t ret = RsDecd(&RsCard, CardRxFunc, card_rx_data, card_rx_data_len, NULL, NULL, NULL);
      if (!ret) {
        log_i("sens Decode Success");
        log_i("sens Func: %d", CardTxFunc);
        log_i("sens Data: %d", card_tx_tata);
      } else if (ret == ILLIGAL_FUNC)
        log_e("485 Illegal Function");
      else if (ret == ILLIGAL_DATA_ADDR)
        log_e("485 Illegal Data Address");
      else if (ret == ILLIGAL_DATA_VALUE)
        log_e("485 Illegal Data Value");
      else if (ret == SLAVE_DEVICE_FAILURE)
        log_e("485 Slave Device Failure");
      CardRxFunc = 0;
      card_rx_data_len = 0;
      memset(card_rx_data, 0, CARD_DATA_MAX_SIZE);
      Unpkg_Flag = FALSE;
    }
    vTaskDelay(10);
  }
}

void SensCardTaskFunc(void* pvParameters) {
  while (1) {
    CardTxFunc = READ_HOLDING_REGISTERS;
    RsCard.ip_addr = SENS_CARD_ADDR;
    RsError_t ret = RsEncd(&RsCard, CardTxFunc, SENS_CARD_REG_START, data_or_num, NULL, card_tx_tata, &card_tx_tata_len);
    if (!ret) {
      log_i("Sensor Card Encode Success");
      RS485_Pkg(&RsCard, SENS_CARD_ADDR, CardTxFunc, card_tx_tata, card_tx_tata_len);
    } else if (ret == ENCODE_FOR_NUMBER)
      log_e("Sensor Card Encode ERRO For Number");
    else if (ret == ENCODE_FOR_SINGLE_DATA)
      log_e("Sensor Card Encode ERRO For Single Data");

    vTaskDelay(1000);
  }
  vTaskDelete(NULL);
}

void FansCardTaskFunc(void* pvParameters) {
  while (1) {
    CardTxFunc = READ_HOLDING_REGISTERS;
    RsCard.ip_addr = FANS_CARD_ADDR;
    RsError_t ret = RsEncd(&RsCard, CardTxFunc, SENS_CARD_REG_START, data_or_num, NULL, card_tx_tata, &card_tx_tata_len);
    if (!ret) {
      log_i("Fans Card Encode Success");
      RS485_Pkg(&RsCard, FANS_CARD_ADDR, CardTxFunc, card_tx_tata, card_tx_tata_len);
    } else if (ret == ENCODE_FOR_NUMBER)
      log_e("Fans Card Encode ERRO For Number");
    else if (ret == ENCODE_FOR_SINGLE_DATA)
      log_e("Fans Card Encode ERRO For Single Data");
    vTaskDelay(1000);
  }
  vTaskDelete(NULL);
}