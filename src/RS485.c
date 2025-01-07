#include "RS485.h"

#ifndef CRC_MODULE_ENABLED
uint32_t crc8_block_calculate(uint8_t *pbuffer, uint32_t length) {}
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

void RS485_init(rs485_t RS485, usart_type *UART, baud_rate_t BaudRate, usart_data_bit_num_type DataBit, usart_stop_bit_num_type StopBit, uint16_t IpAddr) {
  usart_reset(UART);
  usart_init(UART, (uint32_t)BaudRate, USART_DATA_8BITS, USART_STOP_1_BIT);

  usart_receiver_enable(UART, TRUE);
  usart_transmitter_enable(UART, FALSE);

  usart_interrupt_enable(UART, USART_IDLE_INT, TRUE);
  usart_interrupt_enable(UART, USART_RDBF_INT, TRUE);
  nvic_irq_enable(Get_USART_IRQn(UART), 2, 0);

  usart_parity_selection_config(UART, USART_PARITY_NONE);
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  usart_enable(UART, TRUE);

  usart_interrupt_enable(UART, USART_TDBE_INT, TRUE);

  RS485.UART = UART;
  RS485.BaudRate = BaudRate;
  RS485.StopBit = USART_STOP_1_BIT;
  RS485.DataBit = USART_DATA_8BITS;
  RS485.IpAddr = IpAddr;

  memset(RS485.RxData, 0, MAX_DATA_BUFFER_SIZE);
  RS485.RxData[0] = 0x7E;
  RS485.RxIdex = 1;
  RS485.RxPkgCpltFlag = FALSE;
  RS485.DecodeIdex = 0;

  memset(RS485.TxData, 0, MAX_DATA_BUFFER_SIZE);
  RS485.TxData[0] = 0x7E;
  RS485.TxIdex = 1;
  RS485.TxPkgCpltFlag = FALSE;
  RS485.EncodeIdex = 0;
}

void RS485_Re_Config(rs485_t RS485, baud_rate_t BaudRate, usart_data_bit_num_type DataBit, usart_stop_bit_num_type StopBit, uint16_t IpAddr) {
  usart_receiver_enable(RS485.UART, FALSE);
  usart_transmitter_enable(RS485.UART, FALSE);
  RS485_init(RS485, RS485.UART, BaudRate, DataBit, StopBit, RS485.IpAddr);
}

void RS485_Tx_Data_ISR(rs485_t RS485) {
  if (RS485.EncodeIdex != RS485.TxIdex) {
    usart_data_transmit(RS485.UART, RS485.TxData[RS485.EncodeIdex++]);
  }
  if (RS485.TxIdex >= MAX_DATA_BUFFER_SIZE) {
    RS485.TxIdex = 0;
  }
}

void RS485_Rx_Data_ISR(rs485_t RS485) {
  uint16_t data = usart_data_receive(RS485.UART);
  if (data == 0x7E || data == 0x7D) {
    RS485.RxData[RS485.RxIdex++] = 0x7D;
    RS485.RxData[RS485.RxIdex++] = data ^ 0x20;

  } else {
    RS485.RxData[RS485.RxIdex++] = data;
  }
  if (RS485.RxIdex >= MAX_DATA_BUFFER_SIZE) RS485.RxIdex = 0;
}

void RS485_Rx_Cplt_ISR(rs485_t RS485) {
  RS485.RxData[RS485.RxIdex++] = 0X7E;
  RS485.RxPkgCpltFlag = TRUE;
}

uint8_t RS485_Unpkg(rs485_t RS485, rs485_func_t result_func, uint8_t *result_data, uint8_t result_data_len) {
  int i = 0;
  memset(RS485.RxPkg, 0, MAX_PKG_SIZE);
  if (RS485.DecodeIdex != RS485.RxIdex) {
    while (RS485.RxData[RS485.DecodeIdex++] == 0x7E) {
      if (RS485.DecodeIdex >= MAX_DATA_BUFFER_SIZE) RS485.DecodeIdex = 0;
    }

    while (RS485.RxData[RS485.DecodeIdex] != 0x7E) {
      if (RS485.RxData[RS485.DecodeIdex++] == 0x7D) {
        RS485.RxPkg[i++] = RS485.RxData[RS485.DecodeIdex++] ^ 0x20;
      } else {
        RS485.RxPkg[i++] = RS485.RxData[RS485.DecodeIdex];
      }

      if (i >= MAX_PKG_SIZE) return UNPKG_OVER_PACKGE_SIZE;
      if (RS485.DecodeIdex >= MAX_DATA_BUFFER_SIZE) RS485.DecodeIdex = 0;
    }
  } else {
    RS485.RxPkgCpltFlag = FALSE;
    return UNPKG_FINISH;
  }
  if (RS485.RxPkg[0] != RS485.IpAddr) return NOT_MY_ADDR;

  volatile uint32_t crc_value = crc8_block_calculate((uint8_t *)RS485.RxPkg, i - 1);
  uint16_t calculatedCRC = (uint16_t)(crc_value & 0xFFFF);

  uint16_t receivedCRC = (RS485.RxPkg[i - 1] << 8) | RS485.RxPkg[i];

  if (calculatedCRC != receivedCRC) return CRC_ERROR;

  result_func = RS485.RxPkg[1];
  for (int j = 0; j < i - 3; j++) {
	result_data[j] = RS485.RxPkg[j + 2];
  }
  result_data_len = i - 3;

  return RS485_OK;
}

void RS485_Decode(rs485_t RS485, rs485_func_t func, uint8_t *data, int len) {}