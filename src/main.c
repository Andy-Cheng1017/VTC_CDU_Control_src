#include <string.h>
#include "i2c_application.h"
#include "RX8025T_wrap.h"
#include "at32f403a_407_wk_config.h"
#include "wk_adc.h"
#include "wk_crc.h"
#include "wk_debug.h"
#include "wk_exint.h"
#include "wk_i2c.h"
#include "wk_spi.h"
#include "wk_tmr.h"
#include "wk_usart.h"
#include "wk_dma.h"
#include "wk_gpio.h"
#include "wk_system.h"
#include "FreeRTOS.h"
#include "task.h"
#include "network_task.h"
#include "LCD_task.h"
#include "upper_task.h"
#include "sensor_task.h"
#include "pump_task.h"
#include "RTC_task.h"
#include "warning_task.h"
#include "store_data_task.h"
#include "side_card_task.h"
#include "pt100_task.h"
// #include "RS485.h"
// #include "pt100.h"
// #include "Two_Pt_Cal.h"
// #include "NTC.h"
// #include "SensConvVal.h"
// #include "FG_RPM.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */

#define LOG_TAG "main"
#include "elog.h"
/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */

#define START_TASK_PRIO 1
#define START_STK_SIZE 128

#define NETWORK_TASK_PRIO 3
#define NETWORK_STK_SIZE 1024 * 2

#define LCD_TASK_PRIO 3
#define LCD_STK_SIZE 1024

#define UPPER_TASK_PRIO 3
#define UPPER_STK_SIZE 1024

#define SENSOR_TASK_PRIO 4
#define SENSOR_STK_SIZE 1024

#define PUMP_TASK_PRIO 5
#define PUMP_STK_SIZE 1024

#define RTC_TASK_PRIO 2
#define RTC_STK_SIZE 512

#define WARNNING_TASK_PRIO 4
#define WARNNING_STK_SIZE 512

#define SIDE_CARD_TASK_PRIO 4
#define SIDE_CARD_STK_SIZE 512

#define PT100_TASK_PRIO 3
#define PT100_STK_SIZE 256

/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */

/* add user code end private variables */

TaskHandle_t StartTask_Handler;
void start_task(void* pvParameters);

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */

void Logger_init(void) {
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

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

/* add user code end 0 */

/**
 * @brief main function.
 * @param  none
 * @retval none
 */

int main(void) {
  /* add user code begin 1 */
  /* add user code end 1 */

  /* system clock config. */
  wk_system_clock_config();

  /* config periph clock. */
  wk_periph_clock_config();

  /* init debug function. */
  wk_debug_config();

  /* nvic config. */
  wk_nvic_config();

  /* timebase config. */
  wk_timebase_init();

  /* init spi1 function. */
  // wk_spi1_init();

  /* init exint function. */
  wk_exint_config();

  /* init adc1 function. */
  wk_tmr1_init();
  wk_dma1_channel1_init();
  wk_adc1_init();

  /* init tmr3 function. */
  wk_tmr3_init();

  /* init tmr9 function. */
  wk_tmr9_init();

  /* init tmr4 function. */
  wk_tmr4_init();

  /* init crc function. */
  wk_crc_init();

  /* init gpio function. */
  wk_gpio_config();

  /* add user code begin 2 */
  wk_usart2_init();
  wk_uart4_init();

  wk_usart1_init();
  wk_dma1_channel4_init();
  Logger_init();

  wk_i2c3_init();

  wk_uart8_init();

  xTaskCreate((TaskFunction_t)start_task, (const char*)"start_task", (uint16_t)START_STK_SIZE, (void*)NULL, (UBaseType_t)START_TASK_PRIO,
              (TaskHandle_t*)&StartTask_Handler);
  vTaskStartScheduler();
}

void start_task(void* pvParameters) {
  vTaskDelay(100);
  // taskENTER_CRITICAL();
  // xTaskCreate((TaskFunction_t)network_task_function, (const char*)"Network_task", (uint16_t)NETWORK_STK_SIZE, (void*)NULL,
  //             (UBaseType_t)NETWORK_TASK_PRIO, (TaskHandle_t*)&network_handler);
  vTaskDelay(50);
  // xTaskCreate((TaskFunction_t)LCD_task_function, (const char*)"LCD_task", (uint16_t)LCD_STK_SIZE, (void*)NULL, (UBaseType_t)LCD_TASK_PRIO,
  //             (TaskHandle_t*)&LCD_handler);
  vTaskDelay(50);
  xTaskCreate((TaskFunction_t)upper_task_function, (const char*)"Upper_task", (uint16_t)UPPER_STK_SIZE, (void*)NULL, (UBaseType_t)UPPER_TASK_PRIO,
              (TaskHandle_t*)&upper_handler);
  vTaskDelay(50);
  xTaskCreate((TaskFunction_t)sensor_task_function, (const char*)"Sensor_task", (uint16_t)SENSOR_STK_SIZE, (void*)NULL, (UBaseType_t)SENSOR_TASK_PRIO,
              (TaskHandle_t*)&sensor_handler);
  vTaskDelay(50);
  xTaskCreate((TaskFunction_t)pump_task_function, (const char*)"Pump_task", (uint16_t)PUMP_STK_SIZE, (void*)NULL, (UBaseType_t)PUMP_TASK_PRIO,
              (TaskHandle_t*)&pump_handler);
  vTaskDelay(50);
  xTaskCreate((TaskFunction_t)RTC_task_function, (const char*)"RTC_task", (uint16_t)RTC_STK_SIZE, (void*)NULL, (UBaseType_t)RTC_TASK_PRIO,
              (TaskHandle_t*)&RTC_handler);
  vTaskDelay(50);
  xTaskCreate((TaskFunction_t)pt100_task_function, (const char*)"PT100_task", (uint16_t)PT100_STK_SIZE, (void*)NULL, (UBaseType_t)PT100_TASK_PRIO,
              (TaskHandle_t*)&pt100_handler);
  // xTaskCreate((TaskFunction_t)SideCardTaskFunc, (const char*)"SideCard_task", (uint16_t)SIDE_CARD_STK_SIZE, (void*)NULL,
  //             (UBaseType_t)SIDE_CARD_TASK_PRIO, (TaskHandle_t*)&SideCardHandler);
  // xTaskCreate((TaskFunction_t)warning_task_function, (const char*)"Warning_task", (uint16_t)WARNNING_STK_SIZE, (void*)NULL,
  //             (UBaseType_t)WARNNING_TASK_PRIO, (TaskHandle_t*)&warning_handler);
  // xTaskCreate((TaskFunction_t)store_data_task_function, (const char*)"Store_data_task", (uint16_t)WARNNING_STK_SIZE, (void*)NULL,
  //             (UBaseType_t)WARNNING_TASK_PRIO, (TaskHandle_t*)&store_data_handler);

  vTaskDelete(StartTask_Handler);
  // taskEXIT_CRITICAL();
}
