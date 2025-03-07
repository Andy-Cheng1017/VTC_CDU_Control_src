#include "RS485_Region_handler.h"
#include "sensor_task.h"
#include "pump_task.h"
#include "pt100_task.h"
#include "side_card_task.h"

uint32_t SysStat_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root) {
  switch (addr) {
    case 0x0000:
      break;
    case 0x0001:
      break;
    case 0x0002:
      break;
    case 0x0003:
      break;
    case 0x0004:
      break;
    case 0x0005:
      break;
    case 0x0006:
      break;
    case 0x0007:
      break;
    case 0x0008:
      break;
    case 0x0009:
      break;
    case 0x000A:
      break;
    case 0x000B:
      break;
    case 0x000C:
      break;
    case 0x000D:
      break;
    case 0x000E:
      break;
    case 0x000F:
      break;
    case 0x0010:
      break;
    case 0x0011:
      break;
    case 0x0012:
      break;
    case 0x0013:
      break;
    default:
      break;
  }
  return 0;
}

uint32_t DataRead_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root) {
  uint16_t retval = 0;
  if (func == READ_HOLDING_REGISTERS) {
    switch (addr) {
      case 0x0030:
        return (Pt100Stat.pt100_1_temp_m / 10) & 0xFFFF;
      case 0x0031:
        return (Pt100Stat.pt100_2_temp_m / 10) & 0xFFFF;
      case 0x0032:
        return (Pt100Stat.pt100_3_temp_m / 10) & 0xFFFF;
      case 0x0033:
        return (Pt100Stat.pt100_4_temp_m / 10) & 0xFFFF;
      case 0x0034:
        return (SensStat.ntc_1_temp / 10) & 0xFFFF;
      case 0x0035:
        return (SensStat.ntc_2_temp / 10) & 0xFFFF;
      case 0x0036:
        return (SensStat.ntc_3_temp / 10) & 0xFFFF;
      case 0x0037:
        return (SensStat.ntc_4_temp / 10) & 0xFFFF;
      case 0x0038:
        return (SensStat.press_1_val) & 0xFFFF;
      case 0x0039:
        return (SensStat.press_2_val) & 0xFFFF;
      case 0x003A:
        return (SensStat.press_3_val) & 0xFFFF;
      case 0x003B:
        return (SensStat.press_4_val) & 0xFFFF;
      case 0x003C:
        return (SensStat.Flow_val) & 0xFFFF;
      case 0x003D:
        return pump_status.pump_1_FB & 0xFFFF;
      case 0x003E:
        return pump_status.pump_2_FB & 0xFFFF;
      case 0x003F:
        retval = SensStat.voltage_input;
        return (retval >= 0 && retval <= 3000) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x0040:
        retval = SensStat.current_input;
        return (retval >= 0 && retval <= 3000) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x0041:
        retval = (SensStat.power_input >> 16) & 0xFFFF;
        return (retval >= 0 && retval <= 3000) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x0042:
        retval = SensStat.power_input & 0xFFFF;
        return (retval >= 0 && retval <= 3000) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x0043:
        return SensStat.temperature & 0xFFFF;
      case 0x0044:
        return SensStat.humidity & 0xFFFF;
      case 0x0050:
        return (SensCardStat.pt100_1_temp_m / 10) & 0xFFFF;
      case 0x0051:
        return (SensCardStat.pt100_2_temp_m / 10) & 0xFFFF;
      case 0x0052:
        return (SensCardStat.pt100_3_temp_m / 10) & 0xFFFF;
      case 0x0053:
        return (SensCardStat.pt100_4_temp_m / 10) & 0xFFFF;
      case 0x0054:
        return (SensCardStat.press_1_val) & 0xFFFF;
      case 0x0055:
        return (SensCardStat.press_2_val) & 0xFFFF;
      case 0x0056:
        return SensCardStat.leak_sensor & 0xFFFF;
      case 0x0057:
        return SensCardStat.temperature & 0xFFFF;
      case 0x0058:
        return SensCardStat.humidity & 0xFFFF;
      default:
        return ILLIGAL_DATA_ADDR << 16;
    }
  }
  if (func == WRITE_SINGLE_REGISTER || func == WRITE_MULTIPLE_REGISTERS) {
    if (root) {
      switch (addr) {
        case 0x0050:
          return (SensCardStat.pt100_1_temp_m = data * 10) & 0xFFFF;
        case 0x0051:
          return (SensCardStat.pt100_2_temp_m = data * 10) & 0xFFFF;
        case 0x0052:
          return (SensCardStat.pt100_3_temp_m = data * 10) & 0xFFFF;
        case 0x0053:
          return (SensCardStat.pt100_4_temp_m = data * 10) & 0xFFFF;
        case 0x0054:
          return (SensCardStat.press_1_val = data) & 0xFFFF;
        case 0x0055:
          return (SensCardStat.press_2_val = data) & 0xFFFF;
        case 0x0056:
          return (SensCardStat.leak_sensor = data) & 0xFFFF;
        case 0x0057:
          return (SensCardStat.temperature = data) & 0xFFFF;
        case 0x0058:
          return (SensCardStat.humidity = data) & 0xFFFF;
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
        case 0x0062:
          return (pump_control.pump_1_rpm = data) & 0xFFFF;
        case 0x0063:
          return (pump_control.pump_2_rpm = data) & 0xFFFF;
        case 0x0066:
          return (SensCtrl.porpo_1_duty = data) & 0xFFFF;
        case 0x0067:
          return (SensCtrl.porpo_2_duty = data) & 0xFFFF;
        default:
          return ILLIGAL_DATA_ADDR << 16;
      }
    } else {
      return ILLIGAL_DATA_VALUE << 16;
    }
  } else if (func == READ_HOLDING_REGISTERS) {
    switch (addr) {
      case 0x0062:
        return (pump_control.pump_1_rpm & 0xFFFF);
      case 0x0063:
        return (pump_control.pump_2_rpm & 0xFFFF);
      case 0x0066:
        return (SensCtrl.porpo_1_duty & 0xFFFF);
      case 0x0067:
        return (SensCtrl.porpo_2_duty & 0xFFFF);
      default:
        return ILLIGAL_DATA_ADDR << 16;
    }
  } else {
    return ILLIGAL_FUNC << 16;
  }
}

uint32_t FansCardHdle(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len, bool root) {
  if (func == READ_HOLDING_REGISTERS) {
    switch (addr) {
      case 0x0080:
        return FansCardStat.fan1_fb & 0xFFFF;
      case 0x0081:
        return FansCardStat.fan2_fb & 0xFFFF;
      case 0x0082:
        return FansCardStat.fan3_fb & 0xFFFF;
      case 0x0083:
        return FansCardStat.fan4_fb & 0xFFFF;
      case 0x0084:
        return FansCardStat.fan5_fb & 0xFFFF;
      case 0x0085:
        return FansCardStat.fan6_fb & 0xFFFF;
      case 0x0086:
        return FansCardStat.fan7_fb & 0xFFFF;
      case 0x0087:
        return FansCardStat.fan8_fb & 0xFFFF;
      case 0x0088:
        return FansCardStat.fan9_fb & 0xFFFF;
      case 0x0089:
        return FansCardStat.fan10_fb & 0xFFFF;
      case 0x008A:
        return FansCardStat.fan11_fb & 0xFFFF;
      case 0x008B:
        return FansCardStat.fan12_fb & 0xFFFF;
      case 0x008C:
        return FansCardStat.fan13_fb & 0xFFFF;
      case 0x008D:
        return FansCardStat.fan14_fb & 0xFFFF;
      case 0x008E:
        return FansCardStat.fan15_fb & 0xFFFF;
      case 0x008F:
        return FansCardStat.fan16_fb & 0xFFFF;
      case 0x0090:
        return FansCardCtrl.fan_duty[0] & 0xFFFF;
      case 0x0091:
        return FansCardCtrl.fan_duty[1] & 0xFFFF;
      case 0x0092:
        return FansCardCtrl.fan_duty[2] & 0xFFFF;
      case 0x0093:
        return FansCardCtrl.fan_duty[3] & 0xFFFF;
      case 0x0094:
        return FansCardCtrl.fan_duty[4] & 0xFFFF;
      case 0x0095:
        return FansCardCtrl.fan_duty[5] & 0xFFFF;
      case 0x0096:
        return FansCardCtrl.fan_duty[6] & 0xFFFF;
      case 0x0097:
        return FansCardCtrl.fan_duty[7] & 0xFFFF;
      case 0x0098:
        return FansCardCtrl.fan_duty[8] & 0xFFFF;
      case 0x0099:
        return FansCardCtrl.fan_duty[9] & 0xFFFF;
      case 0x009A:
        return FansCardCtrl.fan_duty[10] & 0xFFFF;
      case 0x009B:
        return FansCardCtrl.fan_duty[11] & 0xFFFF;
      case 0x009C:
        return FansCardCtrl.fan_duty[12] & 0xFFFF;
      case 0x009D:
        return FansCardCtrl.fan_duty[13] & 0xFFFF;
      case 0x009E:
        return FansCardCtrl.fan_duty[14] & 0xFFFF;
      case 0x009F:
        return FansCardCtrl.fan_duty[15] & 0xFFFF;
      default:
        return ILLIGAL_DATA_ADDR << 16;
    }
  } else if (func == WRITE_SINGLE_REGISTER || func == WRITE_MULTIPLE_REGISTERS) {
    if (addr >= 0x0080 && addr <= 0x008f) {
      if (root) {
        switch (addr) {
          case 0x0080:
            return (FansCardStat.fan1_fb = data) & 0xFFFF;
          case 0x0081:
            return (FansCardStat.fan2_fb = data) & 0xFFFF;
          case 0x0082:
            return (FansCardStat.fan3_fb = data) & 0xFFFF;
          case 0x0083:
            return (FansCardStat.fan4_fb = data) & 0xFFFF;
          case 0x0084:
            return (FansCardStat.fan5_fb = data) & 0xFFFF;
          case 0x0085:
            return (FansCardStat.fan6_fb = data) & 0xFFFF;
          case 0x0086:
            return (FansCardStat.fan7_fb = data) & 0xFFFF;
          case 0x0087:
            return (FansCardStat.fan8_fb = data) & 0xFFFF;
          case 0x0088:
            return (FansCardStat.fan9_fb = data) & 0xFFFF;
          case 0x0089:
            return (FansCardStat.fan10_fb = data) & 0xFFFF ;
          case 0x008A:
            return (FansCardStat.fan11_fb = data) & 0xFFFF;
          case 0x008B:
            return (FansCardStat.fan12_fb = data) & 0xFFFF;
          case 0x008C:
            return (FansCardStat.fan13_fb = data) & 0xFFFF;
          case 0x008D:
            return (FansCardStat.fan14_fb = data) & 0xFFFF;
          case 0x008E:
            return (FansCardStat.fan15_fb = data) & 0xFFFF;
          case 0x008F:
            return (FansCardStat.fan16_fb = data) & 0xFFFF;
          default:
            return ILLIGAL_DATA_ADDR << 16;
        }
      } else {
        return ILLIGAL_FUNC << 16;
      }
    } else if (addr >= 0x0090 && addr <= 0x009f) {
      if (data >= 0 && data <= 1000) {
        switch (addr) {
          case 0x0090:
            return (FansCardCtrl.fan_duty[0] = data) & 0xFFFF;
          case 0x0091:
            return (FansCardCtrl.fan_duty[1] = data) & 0xFFFF;
          case 0x0092:
            return (FansCardCtrl.fan_duty[2] = data) & 0xFFFF;
          case 0x0093:
            return (FansCardCtrl.fan_duty[3] = data) & 0xFFFF;
          case 0x0094:
            return (FansCardCtrl.fan_duty[4] = data) & 0xFFFF;
          case 0x0095:
            return (FansCardCtrl.fan_duty[5] = data) & 0xFFFF;
          case 0x0096:
            return (FansCardCtrl.fan_duty[6] = data) & 0xFFFF;
          case 0x0097:
            return (FansCardCtrl.fan_duty[7] = data) & 0xFFFF;
          case 0x0098:
            return (FansCardCtrl.fan_duty[8] = data) & 0xFFFF;
          case 0x0099:
            return (FansCardCtrl.fan_duty[9] = data) & 0xFFFF;
          case 0x009A:
            return (FansCardCtrl.fan_duty[10] = data) & 0xFFFF;
          case 0x009B:
            return (FansCardCtrl.fan_duty[11] = data) & 0xFFFF;
          case 0x009C:
            return (FansCardCtrl.fan_duty[12] = data) & 0xFFFF;
          case 0x009D:
            return (FansCardCtrl.fan_duty[13] = data) & 0xFFFF;
          case 0x009E:
            return (FansCardCtrl.fan_duty[14] = data) & 0xFFFF;
          case 0x009F:
            return (FansCardCtrl.fan_duty[15] = data) & 0xFFFF ;
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