#include <string.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "at32f403a_407_wk_config.h"
#include "wk_usart.h"
#include "wk_dma.h"
#include "wk_i2c.h"
#include "init_task.h"
#include "i2c_application.h"

#define LOG_TAG "Init_Task"
#include "elog.h"

TaskHandle_t init_handler;

void Logger_init(void) {
  wk_usart1_init();
  wk_dma1_channel4_init();
  memset(uart_tx_buf, 0, BUFFER_SIZE);
  wk_dma_channel_config(DMA1_CHANNEL4, (uint32_t)&USART1->dt, DMA1_CHANNEL4_MEMORY_BASE_ADDR, DMA1_CHANNEL4_BUFFER_SIZE);
  dma_channel_enable(DMA1_CHANNEL4, TRUE);
  elog_init();
  elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
  elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
  elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
  elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
  elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
  elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
  elog_set_text_color_enabled(TRUE);
  elog_start();
}

void init_task_function(void* pvParameters) {
  // Initialization code here

  Logger_init();

  log_i("Init Task Running");
  wk_i2c3_init();

  // while (1) {
  //   //
  //   vTaskDelay(3000);
  // }

  // vTaskDelete(NULL);
}