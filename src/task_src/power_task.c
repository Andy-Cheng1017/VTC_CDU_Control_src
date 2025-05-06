#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "power_task.h"
#include "i2c_application.h"

#define LOG_TAG "Power_Task"
#include "elog.h"

TaskHandle_t power_handler;

#define I2C_TIMEOUT 1000
uint8_t rx_buf = 0;
i2c_handle_type hi2cx = {
    .i2cx = I2C2,
};

PowerStat_t Power_Stat[2] = {
    [0] =
        {
            .pwr_stat_addr = 0x21,  // 0010 0001
            .pwr_out_addr = 0x91,   // 1001 0001
            .power_status = 0,
            .voltage_input = 0,
            .current_input = 0,
            .power_temp = 0,
        },
    [1] =
        {
            .pwr_stat_addr = 0x22,  // 0010 0010
            .pwr_out_addr = 0x92,   // 1001 0010
            .power_status = 0,
            .voltage_input = 0,
            .current_input = 0,
            .power_temp = 0,
        },
};

typedef enum {
  voltage,
  current,
  power_temp,
} pwr_out_read_type;

i2c_status_type pwr_out_read(pwr_out_read_type type, int i) {
  i2c_status_type i2c_status;

  uint8_t read_type = type;
  if ((i2c_status = i2c_master_transmit(&hi2cx, Power_Stat[i].pwr_out_addr, &read_type, 1, I2C_TIMEOUT)) != I2C_OK) {
    return i2c_status;
  }
  vTaskDelay(10);
  if ((i2c_status = i2c_master_receive(&hi2cx, Power_Stat[i].pwr_out_addr, &rx_buf, 1, I2C_TIMEOUT)) != I2C_OK) {
    return i2c_status;
  }
  return i2c_status;
}

void power_task_function(void* pvParameters) {
  log_i("Power Task Running");

  i2c_status_type i2c_status;

  while (1) {
    for (int i = 0; i < 2; i++) {
      if ((i2c_status = i2c_master_receive(&hi2cx, Power_Stat[i].pwr_stat_addr, &rx_buf, 1, I2C_TIMEOUT)) != I2C_OK) {
        log_e("I2C receive failed, error code: %d", i2c_status);
      } else {
        Power_Stat[i].power_status = rx_buf;
      }

      if ((i2c_status = pwr_out_read(voltage, i)) != I2C_OK) {
        log_e("I2C receive failed, error code: %d", i2c_status);
      } else {
        Power_Stat[i].voltage_input = (uint16_t)((float)rx_buf * 2.54f);
      }

      if ((i2c_status = pwr_out_read(current, i)) != I2C_OK) {
        log_e("I2C receive failed, error code: %d", i2c_status);
      } else {
        Power_Stat[i].current_input = (uint16_t)((float)rx_buf * 1.17f);
      }
      if ((i2c_status = pwr_out_read(power_temp, i)) != I2C_OK) {
        log_e("I2C receive failed, error code: %d", i2c_status);
      } else {
        Power_Stat[i].power_temp = (uint16_t)((float)rx_buf * 3.91f);
      }
    }
  }
  vTaskDelete(NULL);
}