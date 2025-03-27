#include <string.h>
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
#include "LCD_task.h"
#include "upper_task.h"
#include "sensor_task.h"
#include "pump_task.h"
#include "side_card_task.h"
#include "pt100_task.h"
#include "main_task.h"

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

#define LCD_TASK_PRIO 3
#define LCD_STK_SIZE 1024

#define UPPER_TASK_PRIO 3
#define UPPER_STK_SIZE 1024

#define SENSOR_TASK_PRIO 4
#define SENSOR_STK_SIZE 1024

#define PUMP_TASK_PRIO 5
#define PUMP_STK_SIZE 1024

#define SIDE_CARD_TASK_PRIO 4
#define SIDE_CARD_STK_SIZE 1024

#define PT100_TASK_PRIO 3
#define PT100_STK_SIZE 512

#define MAIN_TASK_PRIO 2
#define MAIN_STK_SIZE 512

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

void USART1_IRQHandler(void) {
  if (usart_interrupt_flag_get(USART1, USART_TDBE_FLAG) != RESET) {
    usart_interrupt_enable(USART1, USART_TDBE_INT, FALSE);
    ELogDmaTransfer();
  }
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

  /* init dma1 channel1 */
  wk_dma1_channel1_init();
  /* config dma channel transfer parameter */
  /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
  wk_dma_channel_config(DMA1_CHANNEL1, (uint32_t)&ADC1->odt, DMA1_CHANNEL1_MEMORY_BASE_ADDR, DMA1_CHANNEL1_BUFFER_SIZE);
  dma_channel_enable(DMA1_CHANNEL1, TRUE);

  /* init dma1 channel4 */
  wk_dma1_channel4_init();
  /* config dma channel transfer parameter */
  /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
  wk_dma_channel_config(DMA1_CHANNEL4, (uint32_t)&USART1->dt, DMA1_CHANNEL4_MEMORY_BASE_ADDR, DMA1_CHANNEL4_BUFFER_SIZE);
  dma_channel_enable(DMA1_CHANNEL4, TRUE);

  /* init usart1 function. */
  wk_usart1_init();

  /* init usart2 function. */
  wk_usart2_init();

  /* init usart3 function. */
  wk_usart3_init();

  /* init uart4 function. */
  wk_uart4_init();

  /* init uart8 function. */
  wk_uart8_init();

  /* init spi1 function. */
  wk_spi1_init();

  /* init i2c2 function. */
  wk_i2c2_init();

  /* init i2c3 function. */
  wk_i2c3_init();

  /* init adc1 function. */
  wk_adc1_init();

  /* init exint function. */
  wk_exint_config();

  /* init tmr1 function. */
  wk_tmr1_init();

  /* init tmr4 function. */
  wk_tmr4_init();

  /* init tmr9 function. */
  wk_tmr9_init();

  /* init tmr10 function. */
  wk_tmr10_init();

  /* init tmr11 function. */
  wk_tmr11_init();

  /* init crc function. */
  wk_crc_init();

  /* init gpio function. */
  wk_gpio_config();

  xTaskCreate((TaskFunction_t)start_task, (const char*)"start_task", (uint16_t)START_STK_SIZE, (void*)NULL, (UBaseType_t)START_TASK_PRIO,
              (TaskHandle_t*)&StartTask_Handler);

  Logger_init();

  vTaskStartScheduler();
}

void start_task(void* pvParameters) {
  vTaskDelay(100);
  xTaskCreate((TaskFunction_t)LCD_task_function, (const char*)"LCD_task", (uint16_t)LCD_STK_SIZE, (void*)NULL, (UBaseType_t)LCD_TASK_PRIO,
              (TaskHandle_t*)&LCD_handler);
  vTaskDelay(100);
  xTaskCreate((TaskFunction_t)upper_task_function, (const char*)"Upper_task", (uint16_t)UPPER_STK_SIZE, (void*)NULL, (UBaseType_t)UPPER_TASK_PRIO,
              (TaskHandle_t*)&upper_handler);
  vTaskDelay(100);
  xTaskCreate((TaskFunction_t)SideCardTaskFunc, (const char*)"SideCard_task", (uint16_t)SIDE_CARD_STK_SIZE, (void*)NULL,
              (UBaseType_t)SIDE_CARD_TASK_PRIO, (TaskHandle_t*)&SideCardHandler);
  vTaskDelay(100);
  xTaskCreate((TaskFunction_t)sensor_task_function, (const char*)"Sensor_task", (uint16_t)SENSOR_STK_SIZE, (void*)NULL, (UBaseType_t)SENSOR_TASK_PRIO,
              (TaskHandle_t*)&sensor_handler);
  vTaskDelay(100);
  xTaskCreate((TaskFunction_t)pump_task_function, (const char*)"Pump_task", (uint16_t)PUMP_STK_SIZE, (void*)NULL, (UBaseType_t)PUMP_TASK_PRIO,
              (TaskHandle_t*)&pump_handler);
  vTaskDelay(100);
  xTaskCreate((TaskFunction_t)main_task_function, (const char*)"Main_task", (uint16_t)MAIN_STK_SIZE, (void*)NULL, (UBaseType_t)MAIN_TASK_PRIO,
              (TaskHandle_t*)&main_handler);
  vTaskDelay(100);
  xTaskCreate((TaskFunction_t)pt100_task_function, (const char*)"PT100_task", (uint16_t)PT100_STK_SIZE, (void*)NULL, (UBaseType_t)PT100_TASK_PRIO,
              (TaskHandle_t*)&pt100_handler);
  vTaskDelete(StartTask_Handler);
  // taskEXIT_CRITICAL();
}
