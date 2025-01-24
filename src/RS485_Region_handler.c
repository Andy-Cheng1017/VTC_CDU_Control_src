#include "RS485_Region_handler.h"

uint32_t SysStat_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len) {
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

uint32_t DataRead_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len) {
  uint16_t retval = 0;
  if (func == READ_HOLDING_REGISTERS) {
    switch (addr) {
      case 0x0030:
        return Pt100Stat.pt100_1_temp_m / 10;
      case 0x0031:
        return Pt100Stat.pt100_2_temp_m / 10;
      case 0x0032:
        return Pt100Stat.pt100_3_temp_m / 10;
      case 0x0033:
        return Pt100Stat.pt100_4_temp_m / 10;
      case 0x0034:
        retval = SensStat.NTC_1_temp;
        return (retval >= -500 && retval <= 1500) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x0035:
        retval = SensStat.NTC_2_temp;
        return (retval >= -500 && retval <= 1500) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x0036:
        retval = SensStat.NTC_3_temp;
        return (retval >= -500 && retval <= 1500) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x0037:
        retval = SensStat.NTC_4_temp;
        return (retval >= -500 && retval <= 1500) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x0038:
        retval = SensStat.Presure_1_val;
        return (retval >= -500 && retval <= 500) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x0039:
        retval = SensStat.Presure_2_val;
        return (retval >= -500 && retval <= 500) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x003A:
        retval = SensStat.Presure_3_val;
        return (retval >= -500 && retval <= 500) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x003B:
        retval = SensStat.Presure_4_val;
        return (retval >= -500 && retval <= 500) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x003C:
        retval = SensStat.Flow_val;
        return (retval >= -10000 && retval <= 10000) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x003E:
        retval = SensStat.voltage_input;
        return (retval >= 0 && retval <= 3000) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x003F:
        retval = SensStat.current_input;
        return (retval >= 0 && retval <= 3000) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x0040:
        retval = (SensStat.power_input >> 16) & 0xFFFF;
        return (retval >= 0 && retval <= 3000) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x0041:
        retval = SensStat.power_input & 0xFFFF;
        return (retval >= 0 && retval <= 3000) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x0047:
        retval = pump_status.pump_1_FB;
        return (retval >= 0 && retval <= 10000) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      case 0x0048:
        retval = pump_status.pump_2_FB;
        return (retval >= 0 && retval <= 10000) ? (uint32_t)retval : SLAVE_DEVICE_FAILURE << 16;
      default:
        return ILLIGAL_DATA_ADDR << 16;
    }
  } else {
    return ILLIGAL_FUNC << 16;
  }
}

uint32_t DevCtrl_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len) {
  if (func == WRITE_SINGLE_REGISTER || func == WRITE_MULTIPLE_REGISTERS) {
    switch (addr) {
      case 0x0062:
        return (data >= 0 && data <= 10000) ? pump_control.pump_1_rpm = data : ILLIGAL_DATA_VALUE;
      case 0x0063:
        return (data >= 0 && data <= 10000) ? pump_control.pump_2_rpm = data : ILLIGAL_DATA_VALUE;
      case 0x0066:
        return (data >= 0 && data <= 1000) ? SensCtrl.porpo_1_pwm = data : ILLIGAL_DATA_VALUE;
      case 0x0067:
        return (data >= 0 && data <= 1000) ? SensCtrl.porpo_2_pwm = data : ILLIGAL_DATA_VALUE;
      default:
        return ILLIGAL_DATA_ADDR;
    }
  } else if (func == READ_HOLDING_REGISTERS) {
    switch (addr) {
      case 0x0062:
        return pump_control.pump_1_rpm;
      case 0x0063:
        return pump_control.pump_2_rpm;
      case 0x0066:
        return SensCtrl.porpo_1_pwm;
      case 0x0067:
        return SensCtrl.porpo_2_pwm;
      default:
        return ILLIGAL_DATA_ADDR;
    }
  } else {
    return ILLIGAL_FUNC;
  }
}

uint32_t EthConfig_Handler(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len) {
  switch (addr) {
    case 0x0080:
      break;
    case 0x0081:
      break;
    case 0x0082:
      break;
    case 0x0083:
      break;
    case 0x0084:
      break;
    case 0x0085:
      break;
    case 0x0086:
      break;
    case 0x0087:
      break;
    case 0x0088:
      break;
    case 0x0089:
      break;
    case 0x008A:
      break;
    case 0x008B:
      break;
    case 0x008C:
      break;
    case 0x008D:
      break;
    case 0x008E:
      break;
    case 0x008F:
      break;
    case 0x0090:
      break;
    case 0x0091:
      break;
    case 0x0092:
      break;
    case 0x0093:
      break;
    default:
      break;
  }
  return 0;
}

uint32_t SensCardHdle(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len) {
  switch (addr) {
    case 0x0010:
      break;
    case 0x0011:
      break;
    case 0x0012:
      break;
    case 0x0013:
      break;
    case 0x0014:
      break;
    case 0x0015:
      break;
    case 0x0016:
      break;
    case 0x0017:
      break;
    case 0x0018:
      break;
    case 0x0019:
      break;
    case 0x001A:
      break;
    case 0x001B:
      break;
    case 0x001C:
      break;
    case 0x001D:
      break;
    case 0x001E:
      break;
    case 0x001F:
      break;
    default:
      break;
  }
  return 0;
}

uint32_t FansCardHdle(RsFunc_t func, uint16_t addr, uint16_t data, uint8_t len) {
  switch (addr) {
    case 0x0020:
      break;
    case 0x0021:
      break;
    case 0x0022:
      break;
    case 0x0023:
      break;
    case 0x0024:
      break;
    case 0x0025:
      break;
    case 0x0026:
      break;
    case 0x0027:
      break;
    case 0x0028:
      break;
    case 0x0029:
      break;
    case 0x002A:
      break;
    case 0x002B:
      break;
    case 0x002C:
      break;
    case 0x002D:
      break;
    case 0x002E:
      break;
    case 0x002F:
      break;
    default:
      break;
  }
  return 0;
}