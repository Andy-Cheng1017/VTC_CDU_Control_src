#include <string.h>
#include "RS485.h"

#define MAX_HANDLER_COUNT 10

typedef struct {
  uint16_t start_addr;
  uint16_t end_addr;
  rs485_handler_t handler;
} rs485_handler_entry_t;

static rs485_handler_entry_t g_handler_table[MAX_HANDLER_COUNT];
static int g_handler_count = 0;

bool RS485_RegisterHandler(rs485_t *rs485, rs485_handler_t handler) {
  if (g_handler_count >= MAX_HANDLER_COUNT) {
    return false;
  }
  g_handler_table[g_handler_count].start_addr = rs485->RegHdlerStat;
  g_handler_table[g_handler_count].end_addr = rs485->RegHdlerEnd;
  g_handler_table[g_handler_count].handler = handler;
  g_handler_count++;
  return true;
}

#ifndef CRC_MODULE_ENABLED
uint32_t crc8_block_calculate(uint8_t *pbuffer, uint32_t length) {
  // software crc8
}
#endif  // !CRC_MODULE_ENABLED
#ifdef CRC_MODULE_ENABLED
uint32_t crc8_block_calculate(uint8_t *pbuffer, uint32_t length) {
  uint32_t index = 0;

  for (index = 0; index < length; index++) {
    (*(uint8_t *)&CRC->dt) = pbuffer[index];
  }

  return (CRC->dt);
}
#endif  // CRC_MODULE_ENABLED

IRQn_Type Get_USART_IRQn(usart_type *uart) {
  switch ((uint32_t)uart) {
    case (uint32_t)USART1:
      return USART1_IRQn;
    case (uint32_t)USART2:
      return USART2_IRQn;
    case (uint32_t)USART3:
      return USART3_IRQn;
    case (uint32_t)UART4:
      return UART4_IRQn;
    case (uint32_t)UART5:
      return UART5_IRQn;
    case (uint32_t)USART6:
      return USART6_IRQn;
    case (uint32_t)UART7:
      return UART7_IRQn;
    case (uint32_t)UART8:
      return UART8_IRQn;
    default:
      return USART1_IRQn;
  }
}

void RS485_init(rs485_t *rs485) {
  usart_reset(rs485->UART);
  usart_init(rs485->UART, (uint32_t)rs485->BaudRate, rs485->DataBit, rs485->StopBit);

  usart_receiver_enable(rs485->UART, TRUE);
  usart_transmitter_enable(rs485->UART, FALSE);

  usart_interrupt_enable(rs485->UART, USART_IDLE_INT, TRUE);
  usart_interrupt_enable(rs485->UART, USART_RDBF_INT, TRUE);
  nvic_irq_enable(Get_USART_IRQn(rs485->UART), 2, 0);

  usart_parity_selection_config(rs485->UART, USART_PARITY_NONE);
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  usart_enable(rs485->UART, TRUE);

  usart_interrupt_enable(rs485->UART, USART_TDBE_INT, TRUE);

  memset(rs485->RxData, 0, MAX_DATA_BUFFER_SIZE);
  rs485->RxData[0] = 0x7E;
  rs485->RxIdex = 0;
  rs485->RxPkgCpltFlag = FALSE;
  rs485->DecodeIdex = 0;

  memset(rs485->TxData, 0, MAX_DATA_BUFFER_SIZE);
  rs485->TxData[0] = 0x7E;
  rs485->TxIdex = 0;
  rs485->TxPkgCpltFlag = FALSE;
  rs485->EncodeIdex = 0;
}

void RS485_Re_Config(rs485_t *rs485) {
  usart_receiver_enable(rs485->UART, FALSE);
  usart_transmitter_enable(rs485->UART, FALSE);
  RS485_init(rs485);
}

void RS485_Tx_Data_ISR(rs485_t *rs485) {
  if (rs485->EncodeIdex != rs485->TxIdex) {
    if (rs485->TxData[++rs485->TxIdex & MAX_BUF_MASK] == 0X7D) {
      usart_data_transmit(rs485->UART, rs485->TxData[++rs485->TxIdex & MAX_BUF_MASK] ^ 0x20);
    } else {
      usart_data_transmit(rs485->UART, rs485->TxData[rs485->TxIdex]);
    }
  }
}

void RS485_Rx_Data_ISR(rs485_t *rs485) {
  uint16_t data = usart_data_receive(rs485->UART);
  if (data == 0x7E || data == 0x7D) {
    rs485->RxData[++rs485->RxIdex & MAX_BUF_MASK] = 0x7D;
    rs485->RxData[++rs485->RxIdex & MAX_BUF_MASK] = data ^ 0x20;

  } else {
    rs485->RxData[++rs485->RxIdex & MAX_BUF_MASK] = data;
  }
}

void RS485_Rx_Cplt_ISR(rs485_t *rs485) {
  rs485->RxData[++rs485->RxIdex & MAX_BUF_MASK] = 0X7E;
  rs485->RxPkgCpltFlag = TRUE;
}

rs485_error_t RS485_Unpkg(rs485_t *rs485, rs485_func_t *upk_func, uint8_t *upk_data, uint8_t *upk_data_len) {
  int i = 0;
  memset(rs485->RxPkg, 0, MAX_PKG_SIZE);
  if (rs485->DecodeIdex != rs485->RxIdex) {
    while (rs485->RxData[rs485->DecodeIdex] != 0x7E) rs485->RxIdex = (rs485->RxIdex + 1) & MAX_BUF_MASK;

    while (rs485->RxData[rs485->DecodeIdex] == 0x7E) {
      if (rs485->RxData[++rs485->DecodeIdex & MAX_BUF_MASK] == 0x7D) {
        rs485->RxPkg[i++] = rs485->RxData[++rs485->DecodeIdex & MAX_BUF_MASK] ^ 0x20;
      } else {
        rs485->RxPkg[i++] = rs485->RxData[rs485->DecodeIdex];
      }

      if (i >= MAX_PKG_SIZE) return UNPKG_OVER_PACKGE_SIZE;
    }
  } else {
    rs485->RxPkgCpltFlag = FALSE;
    return UNPKG_FINISH;
  }
  if (rs485->RxPkg[0] != rs485->IpAddr) return NOT_MY_ADDR;

  volatile uint32_t crc_value = crc8_block_calculate((uint8_t *)rs485->RxPkg, i - 1);
  uint16_t calculatedCRC = (uint16_t)(crc_value & 0xFFFF);

  uint16_t receivedCRC = (rs485->RxPkg[i - 1] << 8) | rs485->RxPkg[i];

  if (calculatedCRC != receivedCRC) return CRC_ERROR;

  *upk_func = rs485->RxPkg[1];
  for (int j = 0; j < i - 3; j++) {
    upk_data[j] = rs485->RxPkg[j + 2];
  }
  *upk_data_len = i - 3;

  return RS485_OK;
}

rs485_error_t RS485_Chk_Reg_AddrVal(bool handler_found, uint32_t Data_return, rs485_func_t rx_Func, rs485_func_t *tx_Func, uint8_t *tx_Data,
                                    uint8_t *tx_Data_len) {
  if ((Data_return >> 16) == 1) {
    *tx_Func = rx_Func + 0x80;
    tx_Data[0] = 0x01;
    *tx_Data_len = 1;
    return ILLIGAL_FUNC;
  } else if (!handler_found || (Data_return >> 16) == 2) {
    *tx_Func = rx_Func + 0x80;
    tx_Data[0] = 0x02;
    *tx_Data_len = 1;
    return ILLIGAL_DATA_ADDR;
  } else if ((Data_return >> 16) == 3) {
    *tx_Func = rx_Func + 0x80;
    tx_Data[0] = 0x03;
    *tx_Data_len = 1;
    return ILLIGAL_DATA_VALUE;
  } else if ((Data_return >> 16) == 4) {
    *tx_Func = rx_Func + 0x80;
    tx_Data[0] = 0x04;
    *tx_Data_len = 1;
    return SLAVE_DEVICE_FAILURE;
  }
  return RS485_OK;
}

rs485_error_t RS485_Decode(rs485_t *rs485, rs485_func_t rx_Func, uint8_t *rx_Data, uint8_t rx_Data_len, rs485_func_t *tx_Func, uint8_t *tx_Data,
                           uint8_t *tx_Data_len) {
  tx_Data[0] = rx_Data[0];
  tx_Data[1] = rx_Data[1];
  if (rx_Func == READ_HOLDING_REGISTERS) {
    uint16_t Start_addr = rx_Data[0] << 8 | rx_Data[1];
    uint16_t Read_num = rx_Data[2] << 8 | rx_Data[3];

    for (int i = 0; i < (Read_num * 2); i += 2) {
      uint32_t Data_return = 0;

      bool handler_found = false;
      for (int j = 0; j < g_handler_count; j++) {
        if (Start_addr + i >= g_handler_table[j].start_addr && Start_addr + i <= g_handler_table[j].end_addr) {
          Data_return = g_handler_table[j].handler(READ_HOLDING_REGISTERS, Start_addr + i, NULL, NULL);
          handler_found = true;
          break;
        }
      }
      tx_Data[2 + i] = (Data_return >> 8) & 0xFF;
      tx_Data[3 + i] = Data_return & 0xFF;

      rs485_error_t ret = RS485_Chk_Reg_AddrVal(handler_found, Data_return, rx_Func, tx_Func, tx_Data, tx_Data_len);
      if (ret != RS485_OK) return ret;
    }

    *tx_Func = READ_HOLDING_REGISTERS;
    *tx_Data_len = Read_num * 2;
  } else if (rx_Func == WRITE_SINGLE_REGISTER) {
    uint16_t Start_addr = rx_Data[0] << 8 | rx_Data[1];
    uint16_t Data = rx_Data[2] << 8 | rx_Data[3];

    uint32_t Data_return = 0;

    bool handler_found = false;
    for (int j = 0; j < g_handler_count; j++) {
      if (Start_addr >= g_handler_table[j].start_addr && Start_addr <= g_handler_table[j].end_addr) {
        Data_return = g_handler_table[j].handler(READ_HOLDING_REGISTERS, Start_addr, Data, NULL);
        handler_found = true;
        break;
      }
    }

    rs485_error_t ret = RS485_Chk_Reg_AddrVal(handler_found, Data_return, rx_Func, tx_Func, tx_Data, tx_Data_len);
    if (ret != RS485_OK) return ret;

    tx_Data[2] = (Data_return >> 8) & 0xFF;
    tx_Data[3] = Data_return & 0xFF;

    *tx_Func = WRITE_SINGLE_REGISTER;
    *tx_Data_len = 4;
  } else if (rx_Func == WRITE_MULTIPLE_REGISTERS) {
    uint16_t Start_addr = rx_Data[0] << 8 | rx_Data[1];
    uint16_t Num_addr = rx_Data[2] << 8 | rx_Data[3];
    for (int i = 0; i < (Num_addr * 2); i += 2) {
      uint16_t Data = rx_Data[i + 5] << 8 | rx_Data[i + 6];
      uint32_t Data_return = 0;

      bool handler_found = false;
      for (int j = 0; j < g_handler_count; j++) {
        if (Start_addr + i >= g_handler_table[j].start_addr && Start_addr + i <= g_handler_table[j].end_addr) {
          Data_return = g_handler_table[j].handler(READ_HOLDING_REGISTERS, Start_addr + i, Data, NULL);
          handler_found = true;
          break;
        }
      }

      rs485_error_t ret = RS485_Chk_Reg_AddrVal(handler_found, Data_return, rx_Func, tx_Func, tx_Data, tx_Data_len);
      if (ret != RS485_OK) return ret;
    }

    tx_Data[2] = rx_Data[2];
    tx_Data[3] = rx_Data[3];

    *tx_Func = WRITE_MULTIPLE_REGISTERS;
    *tx_Data_len = 4;
  }
  *tx_Func = rx_Func + 0x80;
  tx_Data[0] = 0x01;
  *tx_Data_len = 1;
  return ILLIGAL_FUNC;
}

rs485_error_t RS485_Encode(rs485_t *rs485, rs485_func_t Func, uint16_t stat_addr, uint8_t *Data_or_Num, uint8_t Data_len, uint8_t *tx_Data,
                           uint8_t *tx_Data_len) {
  tx_Data[0] = (stat_addr >> 8) & 0xFF;
  tx_Data[1] = stat_addr & 0xFF;
  if (Func == READ_HOLDING_REGISTERS || Func == WRITE_SINGLE_REGISTER) {
    tx_Data[2] = Data_or_Num[0];
    tx_Data[3] = Data_or_Num[1];
    *tx_Data_len = 4;
    if (Func == READ_HOLDING_REGISTERS && Data_len != 2) {
      return ENCODE_FOR_NUMBER;
    } else if (Func == WRITE_SINGLE_REGISTER && Data_len != 2) {
      return ENCODE_FOR_SINGLE_DATA;
    }
  } else if (Func == WRITE_MULTIPLE_REGISTERS) {
    if (Data_len & 0X01) return ENCODE_FOR_NUMBER;
    tx_Data[2] = 0;
    tx_Data[3] = (Data_len >> 1) & 0xFF;
    tx_Data[4] = Data_len;
    for (int i = 0; i < Data_len; i++) {
      tx_Data[i + 5] = Data_or_Num[i];
    }
    *tx_Data_len = Data_len + 5;
  }
  return RS485_OK;
}

void RS485_Pkg(rs485_t *rs485, uint8_t DstIpAddr, rs485_func_t pkg_func, uint8_t *pkg_data, uint8_t pkg_data_len) {
  uint8_t CRC_frame[pkg_data_len + 4];
  CRC_frame[0] = DstIpAddr;
  CRC_frame[1] = pkg_func;
  for (int i = 0; i < pkg_data_len; i++) {
    CRC_frame[i + 2] = pkg_data[i];
  }
  uint32_t crc_value = crc8_block_calculate(CRC_frame, pkg_data_len + 2);
  CRC_frame[pkg_data_len] = (crc_value >> 8) & 0xFF;
  CRC_frame[pkg_data_len + 1] = crc_value & 0xFF;

  for (int i = 0; i < pkg_data_len + 4; i++) {
    if (CRC_frame[i] == 0x7E || CRC_frame[i] == 0x7D) {
      rs485->TxData[++rs485->EncodeIdex & MAX_BUF_MASK] = 0x7D;
      rs485->TxData[++rs485->EncodeIdex & MAX_BUF_MASK] = CRC_frame[i] ^ 0x20;
    } else {
      rs485->TxData[++rs485->EncodeIdex & MAX_BUF_MASK] = CRC_frame[i];
    }
  }
  rs485->TxData[++rs485->EncodeIdex & MAX_BUF_MASK] = 0x7E;
  RS485_Tx_Data_ISR(rs485);
}
