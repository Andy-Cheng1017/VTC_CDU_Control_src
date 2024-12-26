#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "at32f403a_407_wk_config.h"
#include "wk_emac.h"
#include "network_task.h"


#define LOG_TAG "Network_Task"
#include "elog.h"

TaskHandle_t network_handler = NULL;

extern void tcpip_stack_init(void);
extern void udpecho_init(void);

void network_task_function(void* pvParameters) {
  while (wk_emac_init() == ERROR) {
    printf("EMAC Init Failed\r\n");
    vTaskDelay(1000);
  }
  log_i("EMAC Init Success");
  tcpip_stack_init();
  udpecho_init();
  while (1) {
    vTaskDelay(500);
  }
}
