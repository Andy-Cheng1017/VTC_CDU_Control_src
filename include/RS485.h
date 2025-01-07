#ifndef __RS485_H_
#define __RS485_H_
#include <stdint.h>
#include <stdbool.h>
#include "at32f403a_407.h"

#define MAX_DATA_BUFFER_SIZE 64
#define MAX_PKG_SIZE 12

typedef enum {
  BR_4800 = 4800,
  BR_9600 = 9600,
  BR_19200 = 19200,
  BR_38400 = 38400,
  BR_57600 = 57600,
  BR_115200 = 115200,
  BR_230400 = 230400,
  BR_460800 = 460800,
  BR_921600 = 921600,
} baud_rate_t;

typedef enum {
  READ_COILS = 0x01,
  READ_DISCRETE_INPUTS = 0x02,
  READ_HOLDING_REGISTERS = 0x03,
  READ_INPUT_REGISTERS = 0x04,
  WRITE_SINGLE_COIL = 0x05,
  WRITE_SINGLE_REGISTER = 0x06,
  WRITE_MULTIPLE_COILS = 0x0F,
  WRITE_MULTIPLE_REGISTERS = 0x10,
} rs485_func_t;

typedef enum {
  RS485_OK = 0,
  UNPKG_FINISH,
  NOT_MY_ADDR,
  UNPKG_OVER_PACKGE_SIZE,
  CRC_ERROR,
} rs485_error_t;

typedef struct {
  usart_type *UART;

  baud_rate_t BaudRate;
  usart_stop_bit_num_type StopBit;
  usart_data_bit_num_type DataBit;
  uint16_t IpAddr;

  uint8_t RxData[MAX_DATA_BUFFER_SIZE];
  uint8_t RxIdex;
  bool RxPkgCpltFlag;
  uint8_t RxPkg[MAX_PKG_SIZE];
  uint8_t DecodeIdex;

  uint8_t TxData[MAX_DATA_BUFFER_SIZE];
  uint8_t TxIdex;
  bool TxPkgCpltFlag;
  uint8_t TxPkg[MAX_PKG_SIZE];
  uint8_t EncodeIdex;

//   uint8_t region;
//   rs485_region_handler_t handler;
} rs485_t;

typedef void (*rs485_region_handler_t)(uint8_t func, uint16_t addr, uint8_t *buf, int len);

void RS485_init(rs485_t RS485, usart_type *UART, baud_rate_t BaudRate, usart_data_bit_num_type DataBit, usart_stop_bit_num_type StopBit, uint16_t IpAddr);
void RS485_Re_Config(rs485_t RS485, baud_rate_t BaudRate, usart_data_bit_num_type DataBit, usart_stop_bit_num_type StopBit, uint16_t IpAddr);

void RS485_Rx_Data_ISR(rs485_t RS485);
void RS485_Rx_Cplt_ISR(rs485_t RS485);

void RS485_Tx_Data_ISR(rs485_t RS485);

uint8_t RS485_Unpkg(rs485_t RS485, rs485_func_t result_func, uint8_t *result_data, uint8_t result_data_len);
void RS485_Decode(rs485_t RS485, rs485_func_t func, uint8_t *data, int len);

uint8_t RS485_Pkg(rs485_t RS485, uint8_t *data, uint8_t len);
void RS485_Encode(rs485_t RS485, uint8_t *data, uint8_t len);

uint32_t crc8_block_calculate(uint8_t *pbuffer, uint32_t length);

#endif
