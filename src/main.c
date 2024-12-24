/* add user code begin Header */
/**
 **************************************************************************
 * @file     main.c
 * @brief    main program
 **************************************************************************
 *                       Copyright notice & Disclaimer
 *
 * The software Board Support Package (BSP) that is made available to
 * download from Artery official website is the copyrighted work of Artery.
 * Artery authorizes customers to use, copy, and distribute the BSP
 * software and its related documentation for the purpose of design and
 * development in conjunction with Artery microcontrollers. Use of the
 * software is governed by this copyright notice and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
 * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
 * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
 * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
 * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
 *
 **************************************************************************
 */
/* add user code end Header */

/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407_wk_config.h"
#include "at32f403a_407_board.h"
#include "wk_adc.h"
#include "wk_crc.h"
#include "wk_dac.h"
#include "wk_debug.h"
#include "wk_emac.h"
#include "wk_i2c.h"
#include "wk_rtc.h"
#include "wk_spi.h"
#include "wk_tmr.h"
#include "wk_usart.h"
#include "wk_gpio.h"
#include "wk_system.h"
#include "FreeRTOS.h"
#include "task.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */

/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */

/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */
int LED0_count = 0;
int LED1_count = 0;
/* add user code end private variables */

extern void tcpip_stack_init(void);
extern void udpecho_init(void);

#define START_TASK_PRIO 1
#define START_STK_SIZE 128
TaskHandle_t StartTask_Handler;
void start_task(void* pvParameters);

#define LED0_TASK_PRIO 4
#define LED0_STK_SIZE 128
TaskHandle_t LED0Task_Handler;
void led0_task(void* pvParameters);

#define LED1_TASK_PRIO 3
#define LED1_STK_SIZE 128
TaskHandle_t LED1Task_Handler;
void led1_task(void* pvParameters);

TaskHandle_t network_handler;
void network_task_function(void *pvParameters);


/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */
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
  // nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* timebase config. */
  wk_timebase_init();

  /* init usart1 function. */
  // wk_usart1_init();
  uart_print_init(115200);
  printf("USART1 Init Success\r\n");

  /* init usart2 function. */
  // wk_usart2_init();

  /* uart4 already supports printf. */
  /* init uart4 function. */
  // wk_uart4_init();

  /* init uart8 function. */
  // wk_uart8_init();

  /* init spi1 function. */
  // wk_spi1_init();

  /* init i2c3 function. */
  // wk_i2c3_init();

  /* init adc1 function. */
  // wk_adc1_init();

  /* init rtc function. */
  // wk_rtc_init();

  /* init tmr3 function. */
  // wk_tmr3_init();

  /* init tmr4 function. */
  // wk_tmr4_init();

  /* init dac function. */
  // wk_dac_init();

  /* init crc function. */
  // wk_crc_init();

  /* init gpio function. */
  wk_gpio_config();

  /* add user code begin 2 */
  at32_led_init(LED2);
  at32_led_init(LED3);
  at32_led_init(LED4);

  at32_led_on(PHY_LAN_nRST);

  xTaskCreate((TaskFunction_t)start_task, (const char*)"start_task", (uint16_t)START_STK_SIZE, (void*)NULL, (UBaseType_t)START_TASK_PRIO,
              (TaskHandle_t*)&StartTask_Handler);
  vTaskStartScheduler();
  /* add user code end 2 */
}

/* add user code begin 4 */
void start_task(void* pvParameters) {
  taskENTER_CRITICAL();
  // printf("Tick: %d\r\n", xTaskGetTickCount());
  // vTaskDelay(1000);
  // xTaskCreate((TaskFunction_t)led0_task, (const char*)"led0_task", (uint16_t)LED0_STK_SIZE, (void*)NULL, (UBaseType_t)LED0_TASK_PRIO,
  //             (TaskHandle_t*)&LED0Task_Handler);
  // xTaskCreate((TaskFunction_t)led1_task, (const char*)"led1_task", (uint16_t)LED1_STK_SIZE, (void*)NULL, (UBaseType_t)LED1_TASK_PRIO,
  //             (TaskHandle_t*)&LED1Task_Handler);
  xTaskCreate((TaskFunction_t)network_task_function, (const char*)"Network_task", (uint16_t)512, (void*)NULL, (UBaseType_t)2,
              (TaskHandle_t*)&network_handler);
  vTaskDelete(StartTask_Handler);
  taskEXIT_CRITICAL();
}

// void led0_task(void* pvParameters) {
//   while (1) {
//     at32_led_toggle(LED4);
//     // printf("LED4 Toggle: %d\r\n", LED0_count);
//     vTaskDelay(300);
//     LED0_count++;
//   }
// }

// void led1_task(void* pvParameters) {
//   while (1) {
//     at32_led_toggle(LED2);
//     // printf("LED2 Toggle: %d\r\n", LED1_count);
//     vTaskDelay(800);
//     LED1_count++;
//   }
// }

void network_task_function(void* pvParameters) {
    /* init emac function. */
  // printf("EMAC Init Start\r\n");
  while(wk_emac_init() == ERROR) {
    printf("EMAC Init Failed\r\n");
    vTaskDelay(1000);
  }
  printf("EMAC Init Success\r\n");
  tcpip_stack_init();
  udpecho_init();
  while (1) {
    at32_led_toggle(LED3);
    vTaskDelay(500);
  }
}
/* add user code end 4 */
