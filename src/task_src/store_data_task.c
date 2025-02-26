#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "at32f403a_407_wk_config.h"
#include "store_data_task.h"
#include "sensor_task.h"
#include "pump_task.h"
#include "pt100.h"

#define LOG_TAG "Store_Data_Task"
#include "elog.h"

TaskHandle_t store_data_handler;

data_sec_t data_sec[10] = {0};
data_ten_t data_ten[6] = {0};
data_min_t data_min[60] = {0};

data_sec_t data_sec_avg = {0};
data_ten_t data_ten_avg = {0};
data_min_t data_min_avg = {0};

int sec_idx = 0;
int ten_idx = 0;
int min_idx = 0;

void store_data_task_function(void *pvParameters) {
  while (1) {
    taskENTER_CRITICAL();
    data_sec[sec_idx].Pt100Stat = Pt100Stat;
    data_sec[sec_idx].SensCtrl = SensCtrl;
    data_sec[sec_idx].pump_status = pump_status;
    data_sec[sec_idx].pump_control = pump_control;
    data_sec[sec_idx].FansCardStat = FansCardStat;
    data_sec[sec_idx].FansCardCtrl = FansCardCtrl;
    taskEXIT_CRITICAL();

    if (++sec_idx > 9) sec_idx = 0;

    // 滿10秒後計算平均
    if (sec_idx >= 9) {
      //   Pt100Stat_t avg_sens_stat = {0};
      //   SensCtrl_t avg_sensor_control = {0};
      //   pump_status_type avg_pump_status = {0};
      //   pump_control_type avg_pump_control = {0};
      //   FansCardStat_t avg_fans_status = {0};
      //   FansCardCtrl_t avg_fans_control = {0};

      //   // 計算每秒數據的平均
      //   for (int i = 0; i < 10; i++) {
      //     avg.PT100_temp[j] += data_sec.sec[i].PT100_temp[j];
      //     avg.NTC_temp[j] += data_sec.sec[i].NTC_temp[j];
      //     avg.Presure_val[j] += data_sec.sec[i].Presure_val[j];
      //     avg.Flow_val += data_sec.sec[i].Flow_val;
      //     avg.voltage_input += data_sec.sec[i].voltage_input;
      //     avg.current_input += data_sec.sec[i].current_input;
      //     avg.power_input += data_sec.sec[i].power_input;
      //   }

      //   // 除以10得到平均值
      //   for (int j = 0; j < 4; j++) {
      //     avg.PT100_temp[j] /= 10;
      //     avg.NTC_temp[j] /= 10;
      //     avg.Presure_val[j] /= 10;
      //   }
      //   avg.Flow_val /= 10;
      //   avg.voltage_input /= 10;
      //   avg.current_input /= 10;
      //   avg.power_input /= 10;

      for (int i = 0; i < 10; i++) {
        data_sec_avg.Pt100Stat.pt100_1_temp_m += data_sec[i].Pt100Stat.pt100_1_temp_m;
        data_sec_avg.Pt100Stat.pt100_2_temp_m += data_sec[i].Pt100Stat.pt100_2_temp_m;
        data_sec_avg.Pt100Stat.pt100_3_temp_m += data_sec[i].Pt100Stat.pt100_3_temp_m;
        data_sec_avg.Pt100Stat.pt100_4_temp_m += data_sec[i].Pt100Stat.pt100_4_temp_m;
        // data_sec_avg.Pt100Stat.NTC_1_temp += data_sec[i].Pt100Stat.NTC_1_temp;
        // data_sec_avg.Pt100Stat.NTC_2_temp += data_sec[i].Pt100Stat.NTC_2_temp;
        // data_sec_avg.Pt100Stat.NTC_3_temp += data_sec[i].Pt100Stat.NTC_3_temp;
        // data_sec_avg.Pt100Stat.NTC_4_temp += data_sec[i].Pt100Stat.NTC_4_temp;
        // data_sec_avg.Pt100Stat.Presure_1_val += data_sec[i].Pt100Stat.Presure_1_val;
        // data_sec_avg.Pt100Stat.Presure_2_val += data_sec[i].Pt100Stat.Presure_2_val;
        // data_sec_avg.Pt100Stat.Presure_3_val += data_sec[i].Pt100Stat.Presure_3_val;
        // data_sec_avg.Pt100Stat.Presure_4_val += data_sec[i].Pt100Stat.Presure_4_val;
        // data_sec_avg.Pt100Stat.Flow_val += data_sec[i].Pt100Stat.Flow_val;
        // data_sec_avg.Pt100Stat.voltage_input += data_sec[i].Pt100Stat.voltage_input;
        // data_sec_avg.Pt100Stat.current_input += data_sec[i].Pt100Stat.current_input;
        // data_sec_avg.Pt100Stat.power_input += data_sec[i].Pt100Stat.power_input;
        // data_sec_avg.SensCtrl.porpo_1_duty += data_sec[i].SensCtrl.porpo_1_duty;
        // data_sec_avg.SensCtrl.porpo_2_duty += data_sec[i].SensCtrl.porpo_2_duty;
        // data_sec_avg.pump_status.pump_1_FB += data_sec[i].pump_status.pump_1_FB;
        // data_sec_avg.pump_status.pump_2_FB += data_sec[i].pump_status.pump_2_FB;
        // data_sec_avg.pump_control.pump_1_rpm += data_sec[i].pump_control.pump_1_rpm;
        // data_sec_avg.pump_control.pump_2_rpm += data_sec[i].pump_control.pump_2_rpm;
        // data_sec_avg.FansCardStat.fan1_FB += data_sec[i].FansCardStat.fan1_FB;
        // data_sec_avg.FansCardStat.fan2_FB += data_sec[i].FansCardStat.fan2_FB;
        // data_sec_avg.FansCardStat.fan3_FB += data_sec[i].FansCardStat.fan3_FB;
      }

      if (++ten_idx > 6) ten_idx = 0;

      //   calculate_average(data_sec.Pt100Stat, &data_10sce.Pt100Stat[ten_idx], sizeof(Pt100Stat_t), sizeof(int16_t), ten_idx, 10);

      // 每60秒計算一次分鐘數據
    //   if (ten_idx >= 6) {
    //     sensor_data min_avg = {0};

    //     for (int i = 0; i < 6; i++) {
    //       for (int j = 0; j < 4; j++) {
    //         min_avg.PT100_temp[j] += data_10sce.ten_sce[i].PT100_temp[j];
    //         min_avg.NTC_temp[j] += data_10sce.ten_sce[i].NTC_temp[j];
    //         min_avg.Presure_val[j] += data_10sce.ten_sce[i].Presure_val[j];
    //       }
    //       min_avg.Flow_val += data_10sce.ten_sce[i].Flow_val;
    //       min_avg.voltage_input += data_10sce.ten_sce[i].voltage_input;
    //       min_avg.current_input += data_10sce.ten_sce[i].current_input;
    //       min_avg.power_input += data_10sce.ten_sce[i].power_input;
    //     }

    //     // 除以6得到平均值
    //     for (int j = 0; j < 4; j++) {
    //       min_avg.PT100_temp[j] /= 6;
    //       min_avg.NTC_temp[j] /= 6;
    //       min_avg.Presure_val[j] /= 6;
    //     }
    //     min_avg.Flow_val /= 6;
    //     min_avg.voltage_input /= 6;
    //     min_avg.current_input /= 6;
    //     min_avg.power_input /= 6;

    //     // 儲存到分鐘數據陣列
    //     int min_idx = data_min.current_index;
    //     data_min.min[min_idx] = min_avg;
    //     data_min.current_index = (data_min.current_index + 1) % 60;
    //   }
    }

    vTaskDelay(1000);
  }
}
