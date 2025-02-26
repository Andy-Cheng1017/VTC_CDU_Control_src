#include "RS485_Region_handler.h"

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
        return (SensCardStat.pt100_1_temp_m / 10) & 0xFFFF;
      case 0x0035:
        return (SensCardStat.pt100_2_temp_m / 10) & 0xFFFF;
      case 0x0036:
        return (SensCardStat.pt100_3_temp_m / 10) & 0xFFFF;
      case 0x0037:
        return (SensCardStat.pt100_4_temp_m / 10) & 0xFFFF;
      case 0x0038:
        return (SensStat.ntc_1_temp / 10) & 0xFFFF;
      case 0x0039:
        return (SensStat.ntc_2_temp / 10) & 0xFFFF;
      case 0x003A:
        return (SensStat.ntc_3_temp / 10) & 0xFFFF;
      case 0x003B:
        return (SensStat.ntc_4_temp / 10) & 0xFFFF;
      case 0x003C:
        return (SensStat.press_1_val) & 0xFFFF;
      case 0x003D:
        return (SensStat.press_2_val) & 0xFFFF;
      case 0x003E:
        return (SensStat.press_3_val) & 0xFFFF;
      case 0x003F:
        return (SensStat.press_4_val) & 0xFFFF;
      case 0x0040:
        retval = SensStat.Flow_val;
        return (retval >= -10000 && retval <= 10000) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x0041:
        return pump_status.pump_1_FB & 0xFFFF;
      case 0x0042:
        return pump_status.pump_2_FB & 0xFFFF;
      case 0x0043:
        return SensCardStat.leak_sensor & 0xFFFF;
      case 0x0045:
        retval = SensStat.voltage_input;
        return (retval >= 0 && retval <= 3000) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x0046:
        retval = SensStat.current_input;
        return (retval >= 0 && retval <= 3000) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x0047:
        retval = (SensStat.power_input >> 16) & 0xFFFF;
        return (retval >= 0 && retval <= 3000) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x0048:
        retval = SensStat.power_input & 0xFFFF;
        return (retval >= 0 && retval <= 3000) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x0049:
        return SensStat.temperature & 0xFFFF;
      case 0x004A:
        return SensStat.humidity & 0xFFFF;
      case 0x004B:
        return SensCardStat.temperature & 0xFFFF;
      case 0x004C:
        return SensCardStat.humidity & 0xFFFF;
      default:
        return ILLIGAL_DATA_ADDR << 16;
    }
  }
  if (func == WRITE_SINGLE_REGISTER || func == WRITE_MULTIPLE_REGISTERS) {
    if (root) {
      switch (addr) {
        case 0x0034:
          return SensCardStat.pt100_1_temp_m = data;
        case 0x0035:
          return SensCardStat.pt100_2_temp_m = data;
        case 0x0036:
          return SensCardStat.pt100_3_temp_m = data;
        case 0x0037:
          return SensCardStat.pt100_4_temp_m = data;
        case 0x0043:
          return SensCardStat.leak_sensor = data;
        case 0x004B:
          return SensCardStat.temperature = data;
        case 0x004C:
          return SensCardStat.humidity = data;
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
          return pump_control.pump_1_rpm = data;
        case 0x0063:
          return pump_control.pump_2_rpm = data;
        case 0x0066:
          return SensCtrl.porpo_1_duty = data;
        case 0x0067:
          return SensCtrl.porpo_2_duty = data;
        default:
          return ILLIGAL_DATA_ADDR << 16;
      }
    } else {
      return ILLIGAL_DATA_VALUE << 16;
    }
  } else if (func == READ_HOLDING_REGISTERS) {
    switch (addr) {
      case 0x0062:
        return pump_control.pump_1_rpm;
      case 0x0063:
        return pump_control.pump_2_rpm;
      case 0x0066:
        return SensCtrl.porpo_1_duty;
      case 0x0067:
        return SensCtrl.porpo_2_duty;
      default:
        return ILLIGAL_DATA_ADDR << 16;
    }
  } else {
    return ILLIGAL_FUNC << 16;
  }
}

// uint32_t EthConfig_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len) {
//   switch (addr) {
//     case 0x0080:
//       break;
//     case 0x0081:
//       break;
//     case 0x0082:
//       break;
//     case 0x0083:
//       break;
//     case 0x0084:
//       break;
//     case 0x0085:
//       break;
//     case 0x0086:
//       break;
//     case 0x0087:
//       break;
//     case 0x0088:
//       break;
//     case 0x0089:
//       break;
//     case 0x008A:
//       break;
//     case 0x008B:
//       break;
//     case 0x008C:
//       break;
//     case 0x008D:
//       break;
//     case 0x008E:
//       break;
//     case 0x008F:
//       break;
//     case 0x0090:
//       break;
//     case 0x0091:
//       break;
//     case 0x0092:
//       break;
//     case 0x0093:
//       break;
//     default:
//       break;
//   }
//   return 0;
// }

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
        return FansCardCtrl.fan1_duty & 0xFFFF;
      case 0x0091:
        return FansCardCtrl.fan2_duty & 0xFFFF;
      case 0x0092:
        return FansCardCtrl.fan3_duty & 0xFFFF;
      case 0x0093:
        return FansCardCtrl.fan4_duty & 0xFFFF;
      case 0x0094:
        return FansCardCtrl.fan5_duty & 0xFFFF;
      case 0x0095:
        return FansCardCtrl.fan6_duty & 0xFFFF;
      case 0x0096:
        return FansCardCtrl.fan7_duty & 0xFFFF;
      case 0x0097:
        return FansCardCtrl.fan8_duty & 0xFFFF;
      case 0x0098:
        return FansCardCtrl.fan9_duty & 0xFFFF;
      case 0x0099:
        return FansCardCtrl.fan10_duty & 0xFFFF;
      case 0x009A:
        return FansCardCtrl.fan11_duty & 0xFFFF;
      case 0x009B:
        return FansCardCtrl.fan12_duty & 0xFFFF;
      case 0x009C:
        return FansCardCtrl.fan13_duty & 0xFFFF;
      case 0x009D:
        return FansCardCtrl.fan14_duty & 0xFFFF;
      case 0x009E:
        return FansCardCtrl.fan15_duty & 0xFFFF;
      case 0x009F:
        return FansCardCtrl.fan16_duty & 0xFFFF;
      default:
        return ILLIGAL_DATA_ADDR << 16;
    }
  } else if (func == WRITE_SINGLE_REGISTER || func == WRITE_MULTIPLE_REGISTERS) {
    if (data >= 0 && data <= 1000) {
      if (addr >= 0x0080 && addr <= 0x008f) {
        if (root) {
          switch (addr) {
            case 0x0080:
              return FansCardStat.fan1_fb = data;
            case 0x0081:
              return FansCardStat.fan2_fb = data;
            case 0x0082:
              return FansCardStat.fan3_fb = data;
            case 0x0083:
              return FansCardStat.fan4_fb = data;
            case 0x0084:
              return FansCardStat.fan5_fb = data;
            case 0x0085:
              return FansCardStat.fan6_fb = data;
            case 0x0086:
              return FansCardStat.fan7_fb = data;
            case 0x0087:
              return FansCardStat.fan8_fb = data;
            case 0x0088:
              return FansCardStat.fan9_fb = data;
            case 0x0089:
              return FansCardStat.fan10_fb = data;
            case 0x008A:
              return FansCardStat.fan11_fb = data;
            case 0x008B:
              return FansCardStat.fan12_fb = data;
            case 0x008C:
              return FansCardStat.fan13_fb = data;
            case 0x008D:
              return FansCardStat.fan14_fb = data;
            case 0x008E:
              return FansCardStat.fan15_fb = data;
            case 0x008F:
              return FansCardStat.fan16_fb = data;
            default:
              return ILLIGAL_DATA_ADDR << 16;
          }
        } else {
          return ILLIGAL_FUNC << 16;
        }
      } else if (addr >= 0x0090 && addr <= 0x009f) {
        switch (addr) {
          case 0x0090:
            return FansCardCtrl.fan1_duty = data;
          case 0x0091:
            return FansCardCtrl.fan2_duty = data;
          case 0x0092:
            return FansCardCtrl.fan3_duty = data;
          case 0x0093:
            return FansCardCtrl.fan4_duty = data;
          case 0x0094:
            return FansCardCtrl.fan5_duty = data;
          case 0x0095:
            return FansCardCtrl.fan6_duty = data;
          case 0x0096:
            return FansCardCtrl.fan7_duty = data;
          case 0x0097:
            return FansCardCtrl.fan8_duty = data;
          case 0x0098:
            return FansCardCtrl.fan9_duty = data;
          case 0x0099:
            return FansCardCtrl.fan10_duty = data;
          case 0x009A:
            return FansCardCtrl.fan11_duty = data;
          case 0x009B:
            return FansCardCtrl.fan12_duty = data;
          case 0x009C:
            return FansCardCtrl.fan13_duty = data;
          case 0x009D:
            return FansCardCtrl.fan14_duty = data;
          case 0x009E:
            return FansCardCtrl.fan15_duty = data;
          case 0x009F:
            return FansCardCtrl.fan16_duty = data;
          default:
            return ILLIGAL_DATA_ADDR << 16;
        }
      } else {
        return ILLIGAL_DATA_ADDR << 16;
      }
    } else {
      return ILLIGAL_DATA_VALUE << 16;
    }
  } else {
    return ILLIGAL_FUNC << 16;
  }
}