#include "RS485_Region_handler.h"
#include "sensor_task.h"
#include "pump_task.h"
#include "pt100_task.h"
#include "side_card_task.h"
#include "power_task.h"
#include "main_task.h"
typedef enum {
  AUTOMATIC_MODE = SYSPARASET_REG_START,
  TEMPERATURE_SET_POINT,
  FLOW_SET_POINT,
  DIFFERENTIAL_PRESSURE_SET_POINT,
  MIN_PUMP_SPEED,
  PUMP_STOP_DELAY,
  PRESSURE_ALARM_MODE,
  PRESSURE_ALARM_DELAY,
  OUTLET_FLUID_LOW_PRESSURE_ALARM,
  OUTLET_FLUID_HIGH_PRESSURE_ALARM,
  INLET_FLUID_LOW_PRESSURE_ALARM,
  INLET_FLUID_HIGH_PRESSURE_ALARM,
  SIDECAR_RETURN_FLUID_LOW_PRESSURE_ALARM,
  SIDECAR_RETURN_FLUID_HIGH_PRESSURE_ALARM,
  FLOW_ALARM_MODE,
  LOW_FLOW_ALARM,
  LOW_FLOW_DELAY_PERIOD,
  OVER_FLOW_ALARM,
  OVER_FLOW_DELAY_PERIOD,
  TEMP_ALARM_MODE,
  TEMP_ALARM_DELAY_PERIOD,
  LOW_OUTLET_FLUID_TEMP_ALARM,
  HIGH_OUTLET_FLUID_TEMP_ALARM,
  HIGH_AMBIENT_TEMP_ALARM,
  LEAK_DETECTION_MODE,
  CDU_DETECTING_DELAY_PERIOD,
  SERVER_DETECTING_DELAY_PERIOD,
  SIDECAR_DETECTING_DELAY_PERIOD,
  CDU_DEW_POINT_CALIBRATION,
  CDU_TEMP_COMPENSATION,
  CDU_FLUID_INLET_TEMP_CALIBRATION,
  CDU_FLUID_OUTLET_TEMP_CALIBRATION,
  PT100_ABNORMAL_TEMPERATURE_LOW,
  PT100_ABNORMAL_TEMPERATURE_HIGH,
  NTC_ABNORMAL_TEMPERATURE_LOW,
  NTC_ABNORMAL_TEMPERATURE_HIGH,
  ABNORMAL_PRESSURE_LOW,
  ABNORMAL_PRESSURE_HIGH,
} SysParaSetCase_t;

typedef enum {
  SYSTEM_ALARM_STATUS = SYSPARADISP_REG_START,
  SYSTEM_UPTIME_HIGH,
  SYSTEM_UPTIME_LOW,
  PUMP1_RUNTIME_HIGH,
  PUMP1_RUNTIME_LOW,
  PUMP2_RUNTIME_HIGH,
  PUMP2_RUNTIME_LOW,
  PERIPHERAL_STATUS,
} SysParaDispCase_t;

typedef enum {
  PT100_1_TEMPERATURE = DATAREAD_REG_START,
  PT100_2_TEMPERATURE,
  PT100_3_TEMPERATURE,
  PT100_4_TEMPERATURE,
  NTC_1_TEMPERATURE,
  NTC_2_TEMPERATURE,
  NTC_3_TEMPERATURE,
  NTC_4_TEMPERATURE,
  PRESSURE_1_VALUE,
  PRESSURE_2_VALUE,
  PRESSURE_3_VALUE,
  PRESSURE_4_VALUE,
  FLOW_VALUE,
  PUMP_1_FEEDBACK,
  PUMP_2_FEEDBACK,
  LEAK_SENSOR,
  POWER_1_STATUS,
  POWER_1_VOLTAGE_INPUT,
  POWER_1_CURRENT_INPUT,
  POWER_1_TEMPERATURE,
  POWER_2_STATUS,
  POWER_2_VOLTAGE_INPUT,
  POWER_2_CURRENT_INPUT,
  POWER_2_TEMPERATURE,
  TEMPERATURE,
  HUMIDITY,
  DEW_TEMPERATURE,
  FAN_1_FEEDBACK_TEMP,
  DEVICE_CONNECTED,
  RESERVED_1,
  RESERVED_2,
  RESERVED_3,
} DataReadCase_t;

typedef enum {
  SIDECAR_PT100_1_TEMPERATURE = SENS_CARD_DATAREAD_REG_START,
  SIDECAR_PT100_2_TEMPERATURE,
  SIDECAR_PT100_3_TEMPERATURE,
  SIDECAR_PT100_4_TEMPERATURE,
  SIDECAR_PRESSURE_1_VALUE,
  SIDECAR_PRESSURE_2_VALUE,
  SIDECAR_LEAK_SENSOR,
  SIDECAR_TEMPERATURE,
  SIDECAR_HUMIDITY,
  SIDECAR_RESERVED_1,
  SIDECAR_RESERVED_2,
  SIDECAR_RESERVED_3,
  SIDECAR_RESERVED_4,
  SIDECAR_RESERVED_5,
  SIDECAR_RESERVED_6,
  SIDECAR_RESERVED_7,
} SideCarSensReadCase_t;

typedef enum {
  PUMP_1_RPM = DEVCTRL_REG_START,
  PUMP_2_RPM,
  PROPORTIONAL_VALVE_1_DUTY,
  PROPORTIONAL_VALVE_2_DUTY,
  RESERVED_CTRL_1,
  RESERVED_CTRL_2,
  RESERVED_CTRL_3,
  RESERVED_CTRL_4,
  RESERVED_CTRL_5,
  RESERVED_CTRL_6,
  RESERVED_CTRL_7,
  RESERVED_CTRL_8,
  RESERVED_CTRL_9,
  RESERVED_CTRL_10,
  RESERVED_CTRL_11,
  RESERVED_CTRL_12,
} DevCtrlCase_t;

typedef enum {
  SIDECAR_PRESSURE_PUMP = SENS_CARD_DEVCTRL_REG_START,
  SIDECAR_RESERVED_CTRL_1,
  SIDECAR_RESERVED_CTRL_2,
  SIDECAR_RESERVED_CTRL_3,
  SIDECAR_RESERVED_CTRL_4,
  SIDECAR_RESERVED_CTRL_5,
  SIDECAR_RESERVED_CTRL_6,
  SIDECAR_RESERVED_CTRL_7,
  SIDECAR_RESERVED_CTRL_8,
  SIDECAR_RESERVED_CTRL_9,
  SIDECAR_RESERVED_CTRL_10,
  SIDECAR_RESERVED_CTRL_11,
  SIDECAR_RESERVED_CTRL_12,
  SIDECAR_RESERVED_CTRL_13,
  SIDECAR_RESERVED_CTRL_14,
  SIDECAR_RESERVED_CTRL_15,
} SideCarDevCtrlCase_t;

typedef enum {
  FAN_1_FEEDBACK = FANS_CARD_REG_START,
  FAN_2_FEEDBACK,
  FAN_3_FEEDBACK,
  FAN_4_FEEDBACK,
  FAN_5_FEEDBACK,
  FAN_6_FEEDBACK,
  FAN_7_FEEDBACK,
  FAN_8_FEEDBACK,
  FAN_9_FEEDBACK,
  FAN_10_FEEDBACK,
  FAN_11_FEEDBACK,
  FAN_12_FEEDBACK,
  FAN_13_FEEDBACK,
  FAN_14_FEEDBACK,
  FAN_15_FEEDBACK,
  FAN_16_FEEDBACK,
  FAN_1_DUTY,
  FAN_2_DUTY,
  FAN_3_DUTY,
  FAN_4_DUTY,
  FAN_5_DUTY,
  FAN_6_DUTY,
  FAN_7_DUTY,
  FAN_8_DUTY,
  FAN_9_DUTY,
  FAN_10_DUTY,
  FAN_11_DUTY,
  FAN_12_DUTY,
  FAN_13_DUTY,
  FAN_14_DUTY,
  FAN_15_DUTY,
  FAN_16_DUTY,
} FansCardCase_t;

uint32_t SysInfom_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root) {
  if (func == READ_HOLDING_REGISTERS) {
    switch (addr) {
      default:
        return 0;
    }
  } else {
    return ILLIGAL_FUNC << 16;
  }
}

uint32_t SysParaSet_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root) {
  if (func == READ_HOLDING_REGISTERS) {
    switch (addr) {
      case AUTOMATIC_MODE:
        return SysParaSet.ctrl_mode & 0xFFFF;
      case TEMPERATURE_SET_POINT:
        return SysParaSet.temp_set & 0xFFFF;
      case FLOW_SET_POINT:
        return SysParaSet.flow_set & 0xFFFF;
      case DIFFERENTIAL_PRESSURE_SET_POINT:
        return SysParaSet.press_set & 0xFFFF;
      case MIN_PUMP_SPEED:
        return SysParaSet.pump_min_duty & 0xFFFF;
      case PUMP_STOP_DELAY:
        return SysParaSet.pump_stop_delay & 0xFFFF;
      case PRESSURE_ALARM_MODE:
        return SysParaSet.press_warn.act & 0xFFFF;
      case PRESSURE_ALARM_DELAY:
        return SysParaSet.press_warn.delay & 0xFFFF;
      case OUTLET_FLUID_LOW_PRESSURE_ALARM:
        return SysParaSet.press_warn.outlet_low & 0xFFFF;
      case OUTLET_FLUID_HIGH_PRESSURE_ALARM:
        return SysParaSet.press_warn.outlet_high & 0xFFFF;
      case INLET_FLUID_LOW_PRESSURE_ALARM:
        return SysParaSet.press_warn.inlet_low & 0xFFFF;
      case INLET_FLUID_HIGH_PRESSURE_ALARM:
        return SysParaSet.press_warn.inlet_high & 0xFFFF;
      case SIDECAR_RETURN_FLUID_LOW_PRESSURE_ALARM:
        return SysParaSet.press_warn.return_low & 0xFFFF;
      case SIDECAR_RETURN_FLUID_HIGH_PRESSURE_ALARM:
        return SysParaSet.press_warn.return_high & 0xFFFF;
      case FLOW_ALARM_MODE:
        return SysParaSet.flow_warn.act & 0xFFFF;
      case LOW_FLOW_ALARM:
        return SysParaSet.flow_warn.low_val & 0xFFFF;
      case LOW_FLOW_DELAY_PERIOD:
        return SysParaSet.flow_warn.low_delay & 0xFFFF;
      case OVER_FLOW_ALARM:
        return SysParaSet.flow_warn.high_val & 0xFFFF;
      case OVER_FLOW_DELAY_PERIOD:
        return SysParaSet.flow_warn.high_delay & 0xFFFF;
      case TEMP_ALARM_MODE:
        return SysParaSet.temp_warn.act & 0xFFFF;
      case TEMP_ALARM_DELAY_PERIOD:
        return SysParaSet.temp_warn.delay & 0xFFFF;
      case LOW_OUTLET_FLUID_TEMP_ALARM:
        return SysParaSet.temp_warn.outlet_low & 0xFFFF;
      case HIGH_OUTLET_FLUID_TEMP_ALARM:
        return SysParaSet.temp_warn.outlet_high & 0xFFFF;
      case HIGH_AMBIENT_TEMP_ALARM:
        return SysParaSet.temp_warn.ambient_high & 0xFFFF;
      case LEAK_DETECTION_MODE:
        return SysParaSet.leak_warn.act & 0xFFFF;
      case CDU_DETECTING_DELAY_PERIOD:
        return SysParaSet.leak_warn.CUD_delay & 0xFFFF;
      case SERVER_DETECTING_DELAY_PERIOD:
        return SysParaSet.leak_warn.server_delay & 0xFFFF;
      case SIDECAR_DETECTING_DELAY_PERIOD:
        return SysParaSet.leak_warn.sidecar_delay & 0xFFFF;
      case CDU_DEW_POINT_CALIBRATION:
        return SysParaSet.dew_temp_calib & 0xFFFF;
      case CDU_TEMP_COMPENSATION:
        return SysParaSet.ambient_temp_calib & 0xFFFF;
      case CDU_FLUID_INLET_TEMP_CALIBRATION:
        return SysParaSet.inlet_temp_calib & 0xFFFF;
      case CDU_FLUID_OUTLET_TEMP_CALIBRATION:
        return SysParaSet.outlet_temp_calib & 0xFFFF;
      case PT100_ABNORMAL_TEMPERATURE_LOW:
        return (SysParaSet.pt100_abnl_temp_low_m / 100) & 0xFFFF;
      case PT100_ABNORMAL_TEMPERATURE_HIGH:
        return (SysParaSet.pt100_abnl_temp_high_m / 100) & 0xFFFF;
      case NTC_ABNORMAL_TEMPERATURE_LOW:
        return (SysParaSet.ntc_abnl_temp_low_m / 100) & 0xFFFF;
      case NTC_ABNORMAL_TEMPERATURE_HIGH:
        return (SysParaSet.ntc_abnl_temp_high_m / 100) & 0xFFFF;
      case ABNORMAL_PRESSURE_LOW:
        return SysParaSet.press_abnl_val_low_kpa & 0xFFFF;
      case ABNORMAL_PRESSURE_HIGH:
        return SysParaSet.press_abnl_val_high_kpa & 0xFFFF;
      default:
        return 0;
    }
  } else if (func == WRITE_SINGLE_REGISTER || func == WRITE_MULTIPLE_REGISTERS) {
    switch (addr) {
      case AUTOMATIC_MODE:
        return (SysParaSet.ctrl_mode = data) & 0xFFFF;
      case TEMPERATURE_SET_POINT:
        return (SysParaSet.temp_set = data) & 0xFFFF;
      case FLOW_SET_POINT:
        return (SysParaSet.flow_set = data) & 0xFFFF;
      case DIFFERENTIAL_PRESSURE_SET_POINT:
        return (SysParaSet.press_set = data) & 0xFFFF;
      case MIN_PUMP_SPEED:
        return (SysParaSet.pump_min_duty = data) & 0xFFFF;
      case PUMP_STOP_DELAY:
        return (SysParaSet.pump_stop_delay = data) & 0xFFFF;
      case PRESSURE_ALARM_MODE:
        return (SysParaSet.press_warn.act = data) & 0xFFFF;
      case PRESSURE_ALARM_DELAY:
        return (SysParaSet.press_warn.delay = data) & 0xFFFF;
      case OUTLET_FLUID_LOW_PRESSURE_ALARM:
        return (SysParaSet.press_warn.outlet_low = data) & 0xFFFF;
      case OUTLET_FLUID_HIGH_PRESSURE_ALARM:
        return (SysParaSet.press_warn.outlet_high = data) & 0xFFFF;
      case INLET_FLUID_LOW_PRESSURE_ALARM:
        return (SysParaSet.press_warn.inlet_low = data) & 0xFFFF;
      case INLET_FLUID_HIGH_PRESSURE_ALARM:
        return (SysParaSet.press_warn.inlet_high = data) & 0xFFFF;
      case SIDECAR_RETURN_FLUID_LOW_PRESSURE_ALARM:
        return (SysParaSet.press_warn.return_low = data) & 0xFFFF;
      case SIDECAR_RETURN_FLUID_HIGH_PRESSURE_ALARM:
        return (SysParaSet.press_warn.return_high = data) & 0xFFFF;
      case FLOW_ALARM_MODE:
        return (SysParaSet.flow_warn.act = data) & 0xFFFF;
      case LOW_FLOW_ALARM:
        return (SysParaSet.flow_warn.low_val = data) & 0xFFFF;
      case LOW_FLOW_DELAY_PERIOD:
        return (SysParaSet.flow_warn.low_delay = data) & 0xFFFF;
      case OVER_FLOW_ALARM:
        return (SysParaSet.flow_warn.high_val = data) & 0xFFFF;
      case OVER_FLOW_DELAY_PERIOD:
        return (SysParaSet.flow_warn.high_delay = data) & 0xFFFF;
      case TEMP_ALARM_MODE:
        return (SysParaSet.temp_warn.act = data) & 0xFFFF;
      case TEMP_ALARM_DELAY_PERIOD:
        return (SysParaSet.temp_warn.delay = data) & 0xFFFF;
      case LOW_OUTLET_FLUID_TEMP_ALARM:
        return (SysParaSet.temp_warn.outlet_low = data) & 0xFFFF;
      case HIGH_OUTLET_FLUID_TEMP_ALARM:
        return (SysParaSet.temp_warn.outlet_high = data) & 0xFFFF;
      case HIGH_AMBIENT_TEMP_ALARM:
        return (SysParaSet.temp_warn.ambient_high = data) & 0xFFFF;
      case LEAK_DETECTION_MODE:
        return (SysParaSet.leak_warn.act = data) & 0xFFFF;
      case CDU_DETECTING_DELAY_PERIOD:
        return (SysParaSet.leak_warn.CUD_delay = data) & 0xFFFF;
      case SERVER_DETECTING_DELAY_PERIOD:
        return (SysParaSet.leak_warn.server_delay = data) & 0xFFFF;
      case SIDECAR_DETECTING_DELAY_PERIOD:
        return (SysParaSet.leak_warn.sidecar_delay = data) & 0xFFFF;
      case CDU_DEW_POINT_CALIBRATION:
        return (SysParaSet.dew_temp_calib = data) & 0xFFFF;
      case CDU_TEMP_COMPENSATION:
        return (SysParaSet.ambient_temp_calib = data) & 0xFFFF;
      case CDU_FLUID_INLET_TEMP_CALIBRATION:
        return (SysParaSet.inlet_temp_calib = data) & 0xFFFF;
      case CDU_FLUID_OUTLET_TEMP_CALIBRATION:
        return (SysParaSet.outlet_temp_calib = data) & 0xFFFF;
      case PT100_ABNORMAL_TEMPERATURE_LOW:
        return (SysParaSet.pt100_abnl_temp_low_m = data * 100) & 0xFFFF;
      case PT100_ABNORMAL_TEMPERATURE_HIGH:
        return (SysParaSet.pt100_abnl_temp_high_m = data * 100) & 0xFFFF;
      case NTC_ABNORMAL_TEMPERATURE_LOW:
        return (SysParaSet.ntc_abnl_temp_low_m = data * 100) & 0xFFFF;
      case NTC_ABNORMAL_TEMPERATURE_HIGH:
        return (SysParaSet.ntc_abnl_temp_high_m = data * 100) & 0xFFFF;
      case ABNORMAL_PRESSURE_LOW:
        return (SysParaSet.press_abnl_val_low_kpa = data) & 0xFFFF;
      case ABNORMAL_PRESSURE_HIGH:
        return (SysParaSet.press_abnl_val_high_kpa = data) & 0xFFFF;
      default:
        return 0;
    }
  } else {
    return ILLIGAL_FUNC << 16;
  }
}

uint32_t SysParaDisp_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root) {
  if (func == READ_HOLDING_REGISTERS) {
    switch (addr) {
      case SYSTEM_ALARM_STATUS:
        return SysParaDisp.warn_stat & 0xFFFF;
      case SYSTEM_UPTIME_HIGH:
        return SysParaDisp.sys_startup_time_high & 0xFFFF;
      case SYSTEM_UPTIME_LOW:
        return SysParaDisp.sys_startup_time_low & 0xFFFF;
      case PUMP1_RUNTIME_HIGH:
        return SysParaDisp.pump_1_startup_time_high & 0xFFFF;
      case PUMP1_RUNTIME_LOW:
        return SysParaDisp.pump_1_startup_time_low & 0xFFFF;
      case PUMP2_RUNTIME_HIGH:
        return SysParaDisp.pump_2_startup_time_high & 0xFFFF;
      case PUMP2_RUNTIME_LOW:
        return SysParaDisp.pump_2_startup_time_low & 0xFFFF;
      case PERIPHERAL_STATUS:
        return SysParaDisp.periphery_component_exist & 0xFFFF;
      default:
        return 0;
    }
  } else {
    return ILLIGAL_FUNC << 16;
  }
}

uint32_t DataRead_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root) {
  if (func == READ_HOLDING_REGISTERS) {
    switch (addr) {
      case PT100_1_TEMPERATURE:
        return (Pt100Stat.pt100_1_temp_m / 100) & 0xFFFF;
      case PT100_2_TEMPERATURE:
        return (Pt100Stat.pt100_2_temp_m / 100) & 0xFFFF;
      case PT100_3_TEMPERATURE:
        return (Pt100Stat.pt100_3_temp_m / 100) & 0xFFFF;
      case PT100_4_TEMPERATURE:
        return (Pt100Stat.pt100_4_temp_m / 100) & 0xFFFF;
      case NTC_1_TEMPERATURE:
        return (SensStat.ntc_1_temp_m / 100) & 0xFFFF;
      case NTC_2_TEMPERATURE:
        return (SensStat.ntc_2_temp_m / 100) & 0xFFFF;
      case NTC_3_TEMPERATURE:
        return (SensStat.ntc_3_temp_m / 100) & 0xFFFF;
      case NTC_4_TEMPERATURE:
        return (SensStat.ntc_4_temp_m / 100) & 0xFFFF;
      case PRESSURE_1_VALUE:
        return (SensStat.press_1_val_kpa) & 0xFFFF;
      case PRESSURE_2_VALUE:
        return (SensStat.press_2_val_kpa) & 0xFFFF;
      case PRESSURE_3_VALUE:
        return (SensStat.press_3_val_kpa) & 0xFFFF;
      case PRESSURE_4_VALUE:
        return (SensStat.press_4_val_kpa) & 0xFFFF;
      case FLOW_VALUE:
        return (SensStat.Flow_val) & 0xFFFF;
      case PUMP_1_FEEDBACK:
        return pump_status.pump_1_FB & 0xFFFF;
      case PUMP_2_FEEDBACK:
        return pump_status.pump_2_FB & 0xFFFF;
      case LEAK_SENSOR:
        return SensStat.leak_sensor & 0xFFFF;
      case POWER_1_STATUS:
        return Power_1_Stat.power_status & 0xFFFF;
      case POWER_1_VOLTAGE_INPUT:
        return Power_1_Stat.voltage_input & 0xFFFF;
      case POWER_1_CURRENT_INPUT:
        return Power_1_Stat.current_input & 0xFFFF;
      case POWER_1_TEMPERATURE:
        return Power_1_Stat.power_temp & 0xFFFF;
      case POWER_2_STATUS:
        return Power_2_Stat.power_status & 0xFFFF;
      case POWER_2_VOLTAGE_INPUT:
        return Power_2_Stat.voltage_input & 0xFFFF;
      case POWER_2_CURRENT_INPUT:
        return Power_2_Stat.current_input & 0xFFFF;
      case POWER_2_TEMPERATURE:
        return Power_2_Stat.power_temp & 0xFFFF;
      case TEMPERATURE:
        return SensStat.temperature & 0xFFFF;
      case HUMIDITY:
        return SensStat.humidity & 0xFFFF;
      case DEW_TEMPERATURE:
        return SensStat.dew_temp & 0xFFFF;
      case FAN_1_FEEDBACK_TEMP:
        return FansCardStat.fan1_fb & 0xFFFF;
      case DEVICE_CONNECTED:
        return SensStat.device_connected & 0xFFFF;
      case RESERVED_1:
        return 0;
      case RESERVED_2:
        return 0;
      case RESERVED_3:
        return 0;
      default:
        return 0;
    }
  } else if (func == WRITE_SINGLE_REGISTER || func == WRITE_MULTIPLE_REGISTERS) {
    if (root) {
      switch (addr) {
        case PT100_1_TEMPERATURE:
          return 0;
        case PT100_2_TEMPERATURE:
          return 0;
        case PT100_3_TEMPERATURE:
          return 0;
        case PT100_4_TEMPERATURE:
          return 0;
        case NTC_1_TEMPERATURE:
          return 0;
        case NTC_2_TEMPERATURE:
          return 0;
        case NTC_3_TEMPERATURE:
          return 0;
        case NTC_4_TEMPERATURE:
          return 0;
        case PRESSURE_1_VALUE:
          return 0;
        case PRESSURE_2_VALUE:
          return 0;
        case PRESSURE_3_VALUE:
          return 0;
        case PRESSURE_4_VALUE:
          return 0;
        case FLOW_VALUE:
          return 0;
        case PUMP_1_FEEDBACK:
          return 0;
        case PUMP_2_FEEDBACK:
          return 0;
        case LEAK_SENSOR:
          return 0;
        case POWER_1_STATUS:
          return 0;
        case POWER_1_VOLTAGE_INPUT:
          return 0;
        case POWER_1_CURRENT_INPUT:
          return 0;
        case POWER_1_TEMPERATURE:
          return 0;
        case POWER_2_STATUS:
          return 0;
        case POWER_2_VOLTAGE_INPUT:
          return 0;
        case POWER_2_CURRENT_INPUT:
          return 0;
        case POWER_2_TEMPERATURE:
          return 0;
        case TEMPERATURE:
          return (SensStat.temperature = data) & 0xFFFF;
        case HUMIDITY:
          return (SensStat.humidity = data) & 0xFFFF;
        case DEW_TEMPERATURE:
          return (SensStat.dew_temp = data) & 0xFFFF;
        case FAN_1_FEEDBACK_TEMP:
          return 0;
        case DEVICE_CONNECTED:
          return 0;
        case RESERVED_1:
          return 0;
        case RESERVED_2:
          return 0;
        case RESERVED_3:
          return 0;
        default:
          return 0;
      }
    } else {
      return ILLIGAL_FUNC << 16;
    }
  } else {
    return ILLIGAL_FUNC << 16;
  }
}

uint32_t SideCar_Sens_DataRead_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root) {
  if (func == READ_HOLDING_REGISTERS) {
    switch (addr) {
      case SIDECAR_PT100_1_TEMPERATURE:
        return (SensCardStat.pt100_1_temp_m / 100) & 0xFFFF;
      case SIDECAR_PT100_2_TEMPERATURE:
        return (SensCardStat.pt100_2_temp_m / 100) & 0xFFFF;
      case SIDECAR_PT100_3_TEMPERATURE:
        return (SensCardStat.pt100_3_temp_m / 100) & 0xFFFF;
      case SIDECAR_PT100_4_TEMPERATURE:
        return (SensCardStat.pt100_4_temp_m / 100) & 0xFFFF;
      case SIDECAR_PRESSURE_1_VALUE:
        return (SensCardStat.press_1_val_kpa) & 0xFFFF;
      case SIDECAR_PRESSURE_2_VALUE:
        return (SensCardStat.press_2_val_kpa) & 0xFFFF;
      case SIDECAR_LEAK_SENSOR:
        return SensCardStat.leak_sensor & 0xFFFF;
      case SIDECAR_TEMPERATURE:
        return SensCardStat.temperature & 0xFFFF;
      case SIDECAR_HUMIDITY:
        return SensCardStat.humidity & 0xFFFF;
      case SIDECAR_RESERVED_1:
        return 0;
      case SIDECAR_RESERVED_2:
        return 0;
      case SIDECAR_RESERVED_3:
        return 0;
      case SIDECAR_RESERVED_4:
        return 0;
      case SIDECAR_RESERVED_5:
        return 0;
      case SIDECAR_RESERVED_6:
        return 0;
      case SIDECAR_RESERVED_7:
        return 0;
      default:
        return 0;
    }
  } else if (func == WRITE_SINGLE_REGISTER || func == WRITE_MULTIPLE_REGISTERS) {
    if (root) {
      switch (addr) {
        case SIDECAR_PT100_1_TEMPERATURE:
          return (SensCardStat.pt100_1_temp_m = data * 10) & 0xFFFF;
        case SIDECAR_PT100_2_TEMPERATURE:
          return (SensCardStat.pt100_2_temp_m = data * 10) & 0xFFFF;
        case SIDECAR_PT100_3_TEMPERATURE:
          return (SensCardStat.pt100_3_temp_m = data * 10) & 0xFFFF;
        case SIDECAR_PT100_4_TEMPERATURE:
          return (SensCardStat.pt100_4_temp_m = data * 10) & 0xFFFF;
        case SIDECAR_PRESSURE_1_VALUE:
          return (SensCardStat.press_1_val_kpa = data) & 0xFFFF;
        case SIDECAR_PRESSURE_2_VALUE:
          return (SensCardStat.press_2_val_kpa = data) & 0xFFFF;
        case SIDECAR_LEAK_SENSOR:
          return (SensCardStat.leak_sensor = data) & 0xFFFF;
        case SIDECAR_TEMPERATURE:
          return (SensCardStat.temperature = data) & 0xFFFF;
        case SIDECAR_HUMIDITY:
          return (SensCardStat.humidity = data) & 0xFFFF;
        case SIDECAR_RESERVED_1:
          return 0;
        case SIDECAR_RESERVED_2:
          return 0;
        case SIDECAR_RESERVED_3:
          return 0;
        case SIDECAR_RESERVED_4:
          return 0;
        case SIDECAR_RESERVED_5:
          return 0;
        case SIDECAR_RESERVED_6:
          return 0;
        case SIDECAR_RESERVED_7:
          return 0;
        default:
          return 0;
      }
    } else {
      return ILLIGAL_FUNC << 16;
    }
  } else {
    return ILLIGAL_FUNC << 16;
  }
}

uint32_t DevCtrl_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root) {
  if (func == WRITE_SINGLE_REGISTER || func == WRITE_MULTIPLE_REGISTERS) {
    if (data >= 0 && data <= 1000) {
      switch (addr) {
        case PUMP_1_RPM:
          return (pump_control.pump_1_rpm = data) & 0xFFFF;
        case PUMP_2_RPM:
          return (pump_control.pump_2_rpm = data) & 0xFFFF;
        case PROPORTIONAL_VALVE_1_DUTY:
          return (SensCtrl.porpo_1_duty = data) & 0xFFFF;
        case PROPORTIONAL_VALVE_2_DUTY:
          return (SensCtrl.porpo_2_duty = data) & 0xFFFF;
        case RESERVED_CTRL_1:
          return 0;
        case RESERVED_CTRL_2:
          return 0;
        case RESERVED_CTRL_3:
          return 0;
        case RESERVED_CTRL_4:
          return 0;
        case RESERVED_CTRL_5:
          return 0;
        case RESERVED_CTRL_6:
          return 0;
        case RESERVED_CTRL_7:
          return 0;
        case RESERVED_CTRL_8:
          return 0;
        case RESERVED_CTRL_9:
          return 0;
        case RESERVED_CTRL_10:
          return 0;
        case RESERVED_CTRL_11:
          return 0;
        case RESERVED_CTRL_12:
          return 0;
        default:
          return 0;
      }
    } else {
      return ILLIGAL_DATA_VALUE << 16;
    }
  } else if (func == READ_HOLDING_REGISTERS) {
    switch (addr) {
      case PUMP_1_RPM:
        return (pump_control.pump_1_rpm & 0xFFFF);
      case PUMP_2_RPM:
        return (pump_control.pump_2_rpm & 0xFFFF);
      case PROPORTIONAL_VALVE_1_DUTY:
        return (SensCtrl.porpo_1_duty & 0xFFFF);
      case PROPORTIONAL_VALVE_2_DUTY:
        return (SensCtrl.porpo_2_duty & 0xFFFF);
      case RESERVED_CTRL_1:
        return 0;
      case RESERVED_CTRL_2:
        return 0;
      case RESERVED_CTRL_3:
        return 0;
      case RESERVED_CTRL_4:
        return 0;
      case RESERVED_CTRL_5:
        return 0;
      case RESERVED_CTRL_6:
        return 0;
      case RESERVED_CTRL_7:
        return 0;
      case RESERVED_CTRL_8:
        return 0;
      case RESERVED_CTRL_9:
        return 0;
      case RESERVED_CTRL_10:
        return 0;
      case RESERVED_CTRL_11:
        return 0;
      case RESERVED_CTRL_12:
        return 0;
      default:
        return 0;
    }
  } else {
    return ILLIGAL_FUNC << 16;
  }
}

uint32_t SideCar_Sens_DevCtrl_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root) {
  if (func == READ_HOLDING_REGISTERS) {
    switch (addr) {
      case SIDECAR_PRESSURE_PUMP:
        return SensCardCtrl.pressure_pump & 0xFFFF;
      case SIDECAR_RESERVED_CTRL_1:
        return 0;
      case SIDECAR_RESERVED_CTRL_2:
        return 0;
      case SIDECAR_RESERVED_CTRL_3:
        return 0;
      case SIDECAR_RESERVED_CTRL_4:
        return 0;
      case SIDECAR_RESERVED_CTRL_5:
        return 0;
      case SIDECAR_RESERVED_CTRL_6:
        return 0;
      case SIDECAR_RESERVED_CTRL_7:
        return 0;
      case SIDECAR_RESERVED_CTRL_8:
        return 0;
      case SIDECAR_RESERVED_CTRL_9:
        return 0;
      case SIDECAR_RESERVED_CTRL_10:
        return 0;
      case SIDECAR_RESERVED_CTRL_11:
        return 0;
      case SIDECAR_RESERVED_CTRL_12:
        return 0;
      case SIDECAR_RESERVED_CTRL_13:
        return 0;
      case SIDECAR_RESERVED_CTRL_14:
        return 0;
      case SIDECAR_RESERVED_CTRL_15:
        return 0;
      default:
        return 0;
    }
  } else if (func == WRITE_SINGLE_REGISTER || func == WRITE_MULTIPLE_REGISTERS) {
    if (root) {
      switch (addr) {
        case SIDECAR_PRESSURE_PUMP:
          return (SensCardCtrl.pressure_pump = data) & 0xFFFF;
        case SIDECAR_RESERVED_CTRL_1:
          return 0;
        case SIDECAR_RESERVED_CTRL_2:
          return 0;
        case SIDECAR_RESERVED_CTRL_3:
          return 0;
        case SIDECAR_RESERVED_CTRL_4:
          return 0;
        case SIDECAR_RESERVED_CTRL_5:
          return 0;
        case SIDECAR_RESERVED_CTRL_6:
          return 0;
        case SIDECAR_RESERVED_CTRL_7:
          return 0;
        case SIDECAR_RESERVED_CTRL_8:
          return 0;
        case SIDECAR_RESERVED_CTRL_9:
          return 0;
        case SIDECAR_RESERVED_CTRL_10:
          return 0;
        case SIDECAR_RESERVED_CTRL_11:
          return 0;
        case SIDECAR_RESERVED_CTRL_12:
          return 0;
        case SIDECAR_RESERVED_CTRL_13:
          return 0;
        case SIDECAR_RESERVED_CTRL_14:
          return 0;
        case SIDECAR_RESERVED_CTRL_15:
          return 0;
        default:
          return ILLIGAL_DATA_ADDR << 16;
      }
    } else {
      return ILLIGAL_FUNC << 16;
    }
  } else {
    return ILLIGAL_FUNC << 16;
  }
}

uint32_t FansCardHdle(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root) {
  if (func == READ_HOLDING_REGISTERS) {
    switch (addr) {
      case FAN_1_FEEDBACK:
        return FansCardStat.fan1_fb & 0xFFFF;
      case FAN_2_FEEDBACK:
        return FansCardStat.fan2_fb & 0xFFFF;
      case FAN_3_FEEDBACK:
        return FansCardStat.fan3_fb & 0xFFFF;
      case FAN_4_FEEDBACK:
        return FansCardStat.fan4_fb & 0xFFFF;
      case FAN_5_FEEDBACK:
        return FansCardStat.fan5_fb & 0xFFFF;
      case FAN_6_FEEDBACK:
        return FansCardStat.fan6_fb & 0xFFFF;
      case FAN_7_FEEDBACK:
        return FansCardStat.fan7_fb & 0xFFFF;
      case FAN_8_FEEDBACK:
        return FansCardStat.fan8_fb & 0xFFFF;
      case FAN_9_FEEDBACK:
        return FansCardStat.fan9_fb & 0xFFFF;
      case FAN_10_FEEDBACK:
        return FansCardStat.fan10_fb & 0xFFFF;
      case FAN_11_FEEDBACK:
        return FansCardStat.fan11_fb & 0xFFFF;
      case FAN_12_FEEDBACK:
        return FansCardStat.fan12_fb & 0xFFFF;
      case FAN_13_FEEDBACK:
        return FansCardStat.fan13_fb & 0xFFFF;
      case FAN_14_FEEDBACK:
        return FansCardStat.fan14_fb & 0xFFFF;
      case FAN_15_FEEDBACK:
        return FansCardStat.fan15_fb & 0xFFFF;
      case FAN_16_FEEDBACK:
        return FansCardStat.fan16_fb & 0xFFFF;
      case FAN_1_DUTY:
        return FansCardCtrl.fan_duty[0] & 0xFFFF;
      case FAN_2_DUTY:
        return FansCardCtrl.fan_duty[1] & 0xFFFF;
      case FAN_3_DUTY:
        return FansCardCtrl.fan_duty[2] & 0xFFFF;
      case FAN_4_DUTY:
        return FansCardCtrl.fan_duty[3] & 0xFFFF;
      case FAN_5_DUTY:
        return FansCardCtrl.fan_duty[4] & 0xFFFF;
      case FAN_6_DUTY:
        return FansCardCtrl.fan_duty[5] & 0xFFFF;
      case FAN_7_DUTY:
        return FansCardCtrl.fan_duty[6] & 0xFFFF;
      case FAN_8_DUTY:
        return FansCardCtrl.fan_duty[7] & 0xFFFF;
      case FAN_9_DUTY:
        return FansCardCtrl.fan_duty[8] & 0xFFFF;
      case FAN_10_DUTY:
        return FansCardCtrl.fan_duty[9] & 0xFFFF;
      case FAN_11_DUTY:
        return FansCardCtrl.fan_duty[10] & 0xFFFF;
      case FAN_12_DUTY:
        return FansCardCtrl.fan_duty[11] & 0xFFFF;
      case FAN_13_DUTY:
        return FansCardCtrl.fan_duty[12] & 0xFFFF;
      case FAN_14_DUTY:
        return FansCardCtrl.fan_duty[13] & 0xFFFF;
      case FAN_15_DUTY:
        return FansCardCtrl.fan_duty[14] & 0xFFFF;
      case FAN_16_DUTY:
        return FansCardCtrl.fan_duty[15] & 0xFFFF;
      default:
        return 0;
    }
  } else if (func == WRITE_SINGLE_REGISTER || func == WRITE_MULTIPLE_REGISTERS) {
    if (addr >= FAN_1_FEEDBACK && addr <= FAN_16_FEEDBACK) {
      if (root) {
        switch (addr) {
          case FAN_1_FEEDBACK:
            return (FansCardStat.fan1_fb = data) & 0xFFFF;
          case FAN_2_FEEDBACK:
            return (FansCardStat.fan2_fb = data) & 0xFFFF;
          case FAN_3_FEEDBACK:
            return (FansCardStat.fan3_fb = data) & 0xFFFF;
          case FAN_4_FEEDBACK:
            return (FansCardStat.fan4_fb = data) & 0xFFFF;
          case FAN_5_FEEDBACK:
            return (FansCardStat.fan5_fb = data) & 0xFFFF;
          case FAN_6_FEEDBACK:
            return (FansCardStat.fan6_fb = data) & 0xFFFF;
          case FAN_7_FEEDBACK:
            return (FansCardStat.fan7_fb = data) & 0xFFFF;
          case FAN_8_FEEDBACK:
            return (FansCardStat.fan8_fb = data) & 0xFFFF;
          case FAN_9_FEEDBACK:
            return (FansCardStat.fan9_fb = data) & 0xFFFF;
          case FAN_10_FEEDBACK:
            return (FansCardStat.fan10_fb = data) & 0xFFFF;
          case FAN_11_FEEDBACK:
            return (FansCardStat.fan11_fb = data) & 0xFFFF;
          case FAN_12_FEEDBACK:
            return (FansCardStat.fan12_fb = data) & 0xFFFF;
          case FAN_13_FEEDBACK:
            return (FansCardStat.fan13_fb = data) & 0xFFFF;
          case FAN_14_FEEDBACK:
            return (FansCardStat.fan14_fb = data) & 0xFFFF;
          case FAN_15_FEEDBACK:
            return (FansCardStat.fan15_fb = data) & 0xFFFF;
          case FAN_16_FEEDBACK:
            return (FansCardStat.fan16_fb = data) & 0xFFFF;
          default:
            return ILLIGAL_DATA_ADDR << 16;
        }
      } else {
        return ILLIGAL_FUNC << 16;
      }
    } else if (addr >= FAN_1_DUTY && addr <= FAN_16_DUTY) {
      if (data >= 0 && data <= 1000) {
        switch (addr) {
          case FAN_1_DUTY:
            return (FansCardCtrl.fan_duty[0] = data) & 0xFFFF;
          case FAN_2_DUTY:
            return (FansCardCtrl.fan_duty[1] = data) & 0xFFFF;
          case FAN_3_DUTY:
            return (FansCardCtrl.fan_duty[2] = data) & 0xFFFF;
          case FAN_4_DUTY:
            return (FansCardCtrl.fan_duty[3] = data) & 0xFFFF;
          case FAN_5_DUTY:
            return (FansCardCtrl.fan_duty[4] = data) & 0xFFFF;
          case FAN_6_DUTY:
            return (FansCardCtrl.fan_duty[5] = data) & 0xFFFF;
          case FAN_7_DUTY:
            return (FansCardCtrl.fan_duty[6] = data) & 0xFFFF;
          case FAN_8_DUTY:
            return (FansCardCtrl.fan_duty[7] = data) & 0xFFFF;
          case FAN_9_DUTY:
            return (FansCardCtrl.fan_duty[8] = data) & 0xFFFF;
          case FAN_10_DUTY:
            return (FansCardCtrl.fan_duty[9] = data) & 0xFFFF;
          case FAN_11_DUTY:
            return (FansCardCtrl.fan_duty[10] = data) & 0xFFFF;
          case FAN_12_DUTY:
            return (FansCardCtrl.fan_duty[11] = data) & 0xFFFF;
          case FAN_13_DUTY:
            return (FansCardCtrl.fan_duty[12] = data) & 0xFFFF;
          case FAN_14_DUTY:
            return (FansCardCtrl.fan_duty[13] = data) & 0xFFFF;
          case FAN_15_DUTY:
            return (FansCardCtrl.fan_duty[14] = data) & 0xFFFF;
          case FAN_16_DUTY:
            return (FansCardCtrl.fan_duty[15] = data) & 0xFFFF;
          default:
            return ILLIGAL_DATA_ADDR << 16;
        }
      } else {
        return ILLIGAL_DATA_VALUE << 16;
      }
    } else {
      return ILLIGAL_DATA_ADDR << 16;
    }
  } else {
    return ILLIGAL_FUNC << 16;
  }
}