#include "RS485_Region_handler.h"
#include "sensor_task.h"
#include "pump_task.h"
#include "pt100_task.h"
#include "side_card_task.h"
#include "power_task.h"

uint32_t SysStat_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root) {
  if (func == READ_HOLDING_REGISTERS) {
    switch (addr) {
      case 0X0000 ... 0X004F:
        return 0;
      default:
        return ILLIGAL_DATA_ADDR << 16;
    }
  } else {
    return ILLIGAL_FUNC << 16;
  }
}

uint32_t DataRead_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root) {
  if (func == READ_HOLDING_REGISTERS) {
    switch (addr) {
      case 0x0050:
        return (Pt100Stat.pt100_1_temp_m / 10) & 0xFFFF;
      case 0x0051:
        return (Pt100Stat.pt100_2_temp_m / 10) & 0xFFFF;
      case 0x0052:
        return (Pt100Stat.pt100_3_temp_m / 10) & 0xFFFF;
      case 0x0053:
        return (Pt100Stat.pt100_4_temp_m / 10) & 0xFFFF;
      case 0x0054:
        return (SensStat.ntc_1_temp / 10) & 0xFFFF;
      case 0x0055:
        return (SensStat.ntc_2_temp / 10) & 0xFFFF;
      case 0x0056:
        return (SensStat.ntc_3_temp / 10) & 0xFFFF;
      case 0x0057:
        return (SensStat.ntc_4_temp / 10) & 0xFFFF;
      case 0x0058:
        return (SensStat.press_1_val) & 0xFFFF;
      case 0x0059:
        return (SensStat.press_2_val) & 0xFFFF;
      case 0x005A:
        return (SensStat.press_3_val) & 0xFFFF;
      case 0x005B:
        return (SensStat.press_4_val) & 0xFFFF;
      case 0x005C:
        return (SensStat.Flow_val) & 0xFFFF;
      case 0x005D:
        return pump_status.pump_1_FB & 0xFFFF;
      case 0x005E:
        return pump_status.pump_2_FB & 0xFFFF;
      case 0x005F:
        return SensCardStat.leak_sensor & 0xFFFF;
      case 0x0060:
        return Power_1_Stat.power_state & 0xFFFF;
      case 0x0061:
        return Power_1_Stat.voltage_input & 0xFFFF;
      case 0x0062:
        return Power_1_Stat.current_input & 0xFFFF;
      case 0x0063:
        return Power_1_Stat.power_temp & 0xFFFF;
      case 0x0064:
        return Power_2_Stat.power_state & 0xFFFF;
      case 0x0065:
        return Power_2_Stat.voltage_input & 0xFFFF;
      case 0x0066:
        return Power_2_Stat.current_input & 0xFFFF;
      case 0x0067:
        return Power_2_Stat.power_temp & 0xFFFF;
      case 0x0068:
        return SensStat.temperature & 0xFFFF;
      case 0x0069:
        return SensStat.humidity & 0xFFFF;
      case 0x006A:
        return SensStat.dew_temp & 0xFFFF;
      case 0x006B:
        return FansCardStat.fan1_fb & 0xFFFF;
      case 0x006C:
        return 0;
      case 0x006D:
        return 0;
      case 0x006E:
        return 0;
      case 0x006F:
        return 0;
      default:
        return ILLIGAL_DATA_ADDR << 16;
    }
  } else if (func == WRITE_SINGLE_REGISTER || func == WRITE_MULTIPLE_REGISTERS) {
    if (root) {
      switch (addr) {
        case 0x0050:
          return 0;
        case 0x0051:
          return 0;
        case 0x0052:
          return 0;
        case 0x0053:
          return 0;
        case 0x0054:
          return 0;
        case 0x0055:
          return 0;
        case 0x0056:
          return 0;
        case 0x0057:
          return 0;
        case 0x0058:
          return 0;
        case 0x0059:
          return 0;
        case 0x005A:
          return 0;
        case 0x005B:
          return 0;
        case 0x005C:
          return 0;
        case 0x005D:
          return 0;
        case 0x005E:
          return 0;
        case 0x005F:
          return 0;
        case 0x0060:
          return 0;
        case 0x0061:
          return 0;
        case 0x0062:
          return 0;
        case 0x0063:
          return 0;
        case 0x0064:
          return 0;
        case 0x0065:
          return 0;
        case 0x0066:
          return 0;
        case 0x0067:
          return 0;
        case 0x0068:
          return (SensStat.temperature = data) & 0xFFFF;
        case 0x0069:
          return (SensStat.humidity = data) & 0xFFFF;
        case 0x006A:
          return (SensStat.dew_temp = data) & 0xFFFF;
        case 0x006B:
          return 0;
        case 0x006C:
          return 0;
        case 0x006D:
          return 0;
        case 0x006E:
          return 0;
        case 0x006F:
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

uint32_t SideCar_Sens_DataRead_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root) {
  if (func == READ_HOLDING_REGISTERS) {
    switch (addr) {
      case 0x0070:
        return (SensCardStat.pt100_1_temp_m / 10) & 0xFFFF;
      case 0x0071:
        return (SensCardStat.pt100_2_temp_m / 10) & 0xFFFF;
      case 0x0072:
        return (SensCardStat.pt100_3_temp_m / 10) & 0xFFFF;
      case 0x0073:
        return (SensCardStat.pt100_4_temp_m / 10) & 0xFFFF;
      case 0x0074:
        return (SensCardStat.press_1_val) & 0xFFFF;
      case 0x0075:
        return (SensCardStat.press_2_val) & 0xFFFF;
      case 0x0076:
        return SensCardStat.leak_sensor & 0xFFFF;
      case 0x0077:
        return SensCardStat.temperature & 0xFFFF;
      case 0x0078:
        return SensCardStat.humidity & 0xFFFF;
      case 0x0079:
        return 0;
      case 0x007A:
        return 0;
      case 0x007B:
        return 0;
      case 0x007C:
        return 0;
      case 0x007D:
        return 0;
      case 0x007E:
        return 0;
      case 0x007F:
        return 0;
      default:
        return ILLIGAL_DATA_ADDR << 16;
    }
  } else if (func == WRITE_SINGLE_REGISTER || func == WRITE_MULTIPLE_REGISTERS) {
    if (root) {
      switch (addr) {
        case 0x0070:
          return (SensCardStat.pt100_1_temp_m = data * 10) & 0xFFFF;
        case 0x0071:
          return (SensCardStat.pt100_2_temp_m = data * 10) & 0xFFFF;
        case 0x0072:
          return (SensCardStat.pt100_3_temp_m = data * 10) & 0xFFFF;
        case 0x0073:
          return (SensCardStat.pt100_4_temp_m = data * 10) & 0xFFFF;
        case 0x0074:
          return (SensCardStat.press_1_val = data) & 0xFFFF;
        case 0x0075:
          return (SensCardStat.press_2_val = data) & 0xFFFF;
        case 0x0076:
          return (SensCardStat.leak_sensor = data) & 0xFFFF;
        case 0x0077:
          return (SensCardStat.temperature = data) & 0xFFFF;
        case 0x0078:
          return (SensCardStat.humidity = data) & 0xFFFF;
        case 0x0079:
          return 0;
        case 0x007A:
          return 0;
        case 0x007B:
          return 0;
        case 0x007C:
          return 0;
        case 0x007D:
          return 0;
        case 0x007E:
          return 0;
        case 0x007F:
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

uint32_t DevCtrl_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root) {
  if (func == WRITE_SINGLE_REGISTER || func == WRITE_MULTIPLE_REGISTERS) {
    if (data >= 0 && data <= 1000) {
      switch (addr) {
        case 0x0080:
          return (pump_control.pump_1_rpm = data) & 0xFFFF;
        case 0x0081:
          return (pump_control.pump_2_rpm = data) & 0xFFFF;
        case 0x0082:
          return (SensCtrl.porpo_1_duty = data) & 0xFFFF;
        case 0x0083:
          return (SensCtrl.porpo_2_duty = data) & 0xFFFF;
        case 0x0084:
          return 0;
        case 0x0085:
          return 0;
        case 0x0086:
          return 0;
        case 0x0087:
          return 0;
        case 0x0088:
          return 0;
        case 0x0089:
          return 0;
        case 0x008A:
          return 0;
        case 0x008B:
          return 0;
        case 0x008C:
          return 0;
        case 0x008D:
          return 0;
        case 0x008E:
          return 0;
        case 0x008F:
          return 0;
        default:
          return ILLIGAL_DATA_ADDR << 16;
      }
    } else {
      return ILLIGAL_DATA_VALUE << 16;
    }
  } else if (func == READ_HOLDING_REGISTERS) {
    switch (addr) {
      case 0x0080:
        return (pump_control.pump_1_rpm & 0xFFFF);
      case 0x0081:
        return (pump_control.pump_2_rpm & 0xFFFF);
      case 0x0082:
        return (SensCtrl.porpo_1_duty & 0xFFFF);
      case 0x0083:
        return (SensCtrl.porpo_2_duty & 0xFFFF);
      case 0x0084:
        return 0;
      case 0x0085:
        return 0;
      case 0x0086:
        return 0;
      case 0x0087:
        return 0;
      case 0x0088:
        return 0;
      case 0x0089:
        return 0;
      case 0x008A:
        return 0;
      case 0x008B:
        return 0;
      case 0x008C:
        return 0;
      case 0x008D:
        return 0;
      case 0x008E:
        return 0;
      case 0x008F:
        return 0;
      default:
        return ILLIGAL_DATA_ADDR << 16;
    }
  } else {
    return ILLIGAL_FUNC << 16;
  }
}

uint32_t SideCar_Sens_DevCtrl_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root) {
  if (func == READ_HOLDING_REGISTERS) {
    switch (addr) {
      case 0x0090:
        return SensCardCtrl.pressure_pump & 0xFFFF;
      case 0x0091:
        return 0;
      case 0x0092:
        return 0;
      case 0x0093:
        return 0;
      case 0x0094:
        return 0;
      case 0x0095:
        return 0;
      case 0x0096:
        return 0;
      case 0x0097:
        return 0;
      case 0x0098:
        return 0;
      case 0x0099:
        return 0;
      case 0x009A:
        return 0;
      case 0x009B:
        return 0;
      case 0x009C:
        return 0;
      case 0x009D:
        return 0;
      case 0x009E:
        return 0;
      case 0x009F:
        return 0;
      default:
        return ILLIGAL_DATA_ADDR << 16;
    }
  } else if (func == WRITE_SINGLE_REGISTER || func == WRITE_MULTIPLE_REGISTERS) {
    if (root) {
      switch (addr) {
        case 0x0090:
          return (SensCardCtrl.pressure_pump = data) & 0xFFFF;
        case 0x0091:
          return 0;
        case 0x0092:
          return 0;
        case 0x0093:
          return 0;
        case 0x0094:
          return 0;
        case 0x0095:
          return 0;
        case 0x0096:
          return 0;
        case 0x0097:
          return 0;
        case 0x0098:
          return 0;
        case 0x0099:
          return 0;
        case 0x009A:
          return 0;
        case 0x009B:
          return 0;
        case 0x009C:
          return 0;
        case 0x009D:
          return 0;
        case 0x009E:
          return 0;
        case 0x009F:
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
      case 0x0100:
        return FansCardStat.fan1_fb & 0xFFFF;
      case 0x0101:
        return FansCardStat.fan2_fb & 0xFFFF;
      case 0x0102:
        return FansCardStat.fan3_fb & 0xFFFF;
      case 0x0103:
        return FansCardStat.fan4_fb & 0xFFFF;
      case 0x0104:
        return FansCardStat.fan5_fb & 0xFFFF;
      case 0x0105:
        return FansCardStat.fan6_fb & 0xFFFF;
      case 0x0106:
        return FansCardStat.fan7_fb & 0xFFFF;
      case 0x0107:
        return FansCardStat.fan8_fb & 0xFFFF;
      case 0x0108:
        return FansCardStat.fan9_fb & 0xFFFF;
      case 0x0109:
        return FansCardStat.fan10_fb & 0xFFFF;
      case 0x010A:
        return FansCardStat.fan11_fb & 0xFFFF;
      case 0x010B:
        return FansCardStat.fan12_fb & 0xFFFF;
      case 0x010C:
        return FansCardStat.fan13_fb & 0xFFFF;
      case 0x010D:
        return FansCardStat.fan14_fb & 0xFFFF;
      case 0x010E:
        return FansCardStat.fan15_fb & 0xFFFF;
      case 0x010F:
        return FansCardStat.fan16_fb & 0xFFFF;
      case 0x0110:
        return FansCardCtrl.fan_duty[0] & 0xFFFF;
      case 0x0111:
        return FansCardCtrl.fan_duty[1] & 0xFFFF;
      case 0x0112:
        return FansCardCtrl.fan_duty[2] & 0xFFFF;
      case 0x0113:
        return FansCardCtrl.fan_duty[3] & 0xFFFF;
      case 0x0114:
        return FansCardCtrl.fan_duty[4] & 0xFFFF;
      case 0x0115:
        return FansCardCtrl.fan_duty[5] & 0xFFFF;
      case 0x0116:
        return FansCardCtrl.fan_duty[6] & 0xFFFF;
      case 0x0117:
        return FansCardCtrl.fan_duty[7] & 0xFFFF;
      case 0x0118:
        return FansCardCtrl.fan_duty[8] & 0xFFFF;
      case 0x0119:
        return FansCardCtrl.fan_duty[9] & 0xFFFF;
      case 0x011A:
        return FansCardCtrl.fan_duty[10] & 0xFFFF;
      case 0x011B:
        return FansCardCtrl.fan_duty[11] & 0xFFFF;
      case 0x011C:
        return FansCardCtrl.fan_duty[12] & 0xFFFF;
      case 0x011D:
        return FansCardCtrl.fan_duty[13] & 0xFFFF;
      case 0x011E:
        return FansCardCtrl.fan_duty[14] & 0xFFFF;
      case 0x011F:
        return FansCardCtrl.fan_duty[15] & 0xFFFF;
      default:
        return ILLIGAL_DATA_ADDR << 16;
    }
  } else if (func == WRITE_SINGLE_REGISTER || func == WRITE_MULTIPLE_REGISTERS) {
    if (addr >= 0x0100 && addr <= 0x010F) {
      if (root) {
        switch (addr) {
          case 0x0100:
            return (FansCardStat.fan1_fb = data) & 0xFFFF;
          case 0x0101:
            return (FansCardStat.fan2_fb = data) & 0xFFFF;
          case 0x0102:
            return (FansCardStat.fan3_fb = data) & 0xFFFF;
          case 0x0103:
            return (FansCardStat.fan4_fb = data) & 0xFFFF;
          case 0x0104:
            return (FansCardStat.fan5_fb = data) & 0xFFFF;
          case 0x0105:
            return (FansCardStat.fan6_fb = data) & 0xFFFF;
          case 0x0106:
            return (FansCardStat.fan7_fb = data) & 0xFFFF;
          case 0x0107:
            return (FansCardStat.fan8_fb = data) & 0xFFFF;
          case 0x0108:
            return (FansCardStat.fan9_fb = data) & 0xFFFF;
          case 0x0109:
            return (FansCardStat.fan10_fb = data) & 0xFFFF;
          case 0x010A:
            return (FansCardStat.fan11_fb = data) & 0xFFFF;
          case 0x010B:
            return (FansCardStat.fan12_fb = data) & 0xFFFF;
          case 0x010C:
            return (FansCardStat.fan13_fb = data) & 0xFFFF;
          case 0x010D:
            return (FansCardStat.fan14_fb = data) & 0xFFFF;
          case 0x010E:
            return (FansCardStat.fan15_fb = data) & 0xFFFF;
          case 0x010F:
            return (FansCardStat.fan16_fb = data) & 0xFFFF;
          default:
            return ILLIGAL_DATA_ADDR << 16;
        }
      } else {
        return ILLIGAL_FUNC << 16;
      }
    } else if (addr >= 0x0110 && addr <= 0x011F) {
      if (data >= 0 && data <= 1000) {
        switch (addr) {
          case 0x0110:
            return (FansCardCtrl.fan_duty[0] = data) & 0xFFFF;
          case 0x0111:
            return (FansCardCtrl.fan_duty[1] = data) & 0xFFFF;
          case 0x0112:
            return (FansCardCtrl.fan_duty[2] = data) & 0xFFFF;
          case 0x0113:
            return (FansCardCtrl.fan_duty[3] = data) & 0xFFFF;
          case 0x0114:
            return (FansCardCtrl.fan_duty[4] = data) & 0xFFFF;
          case 0x0115:
            return (FansCardCtrl.fan_duty[5] = data) & 0xFFFF;
          case 0x0116:
            return (FansCardCtrl.fan_duty[6] = data) & 0xFFFF;
          case 0x0117:
            return (FansCardCtrl.fan_duty[7] = data) & 0xFFFF;
          case 0x0118:
            return (FansCardCtrl.fan_duty[8] = data) & 0xFFFF;
          case 0x0119:
            return (FansCardCtrl.fan_duty[9] = data) & 0xFFFF;
          case 0x011A:
            return (FansCardCtrl.fan_duty[10] = data) & 0xFFFF;
          case 0x011B:
            return (FansCardCtrl.fan_duty[11] = data) & 0xFFFF;
          case 0x011C:
            return (FansCardCtrl.fan_duty[12] = data) & 0xFFFF;
          case 0x011D:
            return (FansCardCtrl.fan_duty[13] = data) & 0xFFFF;
          case 0x011E:
            return (FansCardCtrl.fan_duty[14] = data) & 0xFFFF;
          case 0x011F:
            return (FansCardCtrl.fan_duty[15] = data) & 0xFFFF;
          default:
            return ILLIGAL_DATA_ADDR << 16;
        }
      } else {
        return ILLIGAL_FUNC << 16;
      }
    } else {
      return ILLIGAL_DATA_ADDR << 16;
    }
  } else {
    return ILLIGAL_DATA_VALUE << 16;
  }
}