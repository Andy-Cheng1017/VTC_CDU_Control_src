#ifndef __RS485_H_
#define __RS485_H_
#include <stdint.h>
#include <stdbool.h>
#include "at32f403a_407.h"
// #include "RS485_Region_handler.h"

#define MAX_DATA_BUFFER_SIZE 64
#define MAX_BUF_MASK (MAX_DATA_BUFFER_SIZE - 1)
#define MAX_PKG_SIZE 16

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
  ILLIGAL_FUNC,
  ILLIGAL_DATA_ADDR,
  ILLIGAL_DATA_VALUE,
  SLAVE_DEVICE_FAILURE,
  ENCODE_FOR_NUMBER,
  ENCODE_FOR_SINGLE_DATA,
} rs485_error_t;

typedef enum{
  MASTER,
  SLAVE,
}rs485_mode_t;

typedef struct {
  usart_type *UART;
  rs485_mode_t Mode;
  baud_rate_t BaudRate;
  usart_stop_bit_num_type StopBit;
  usart_data_bit_num_type DataBit;
  uint8_t IpAddr;

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

  uint8_t RegHdlerStat;
  uint8_t RegHdlerEnd;

} rs485_t;

typedef uint32_t (*rs485_handler_t)(rs485_func_t func, uint16_t addr, uint16_t data, uint8_t len);

bool RS485_RegisterHandler(rs485_t *rs485, rs485_handler_t handler);

void RS485_init(rs485_t *rs485);
void RS485_Re_Config(rs485_t *rs485);

void RS485_Rx_Cplt_ISR(rs485_t *rs485);

void RS485_Tx_Data_ISR(rs485_t *rs485);
void RS485_Rx_Data_ISR(rs485_t *rs485);

rs485_error_t RS485_Unpkg(rs485_t *rs485, rs485_func_t *upk_func, uint8_t *upk_data, uint8_t *upk_data_len);

rs485_error_t RS485_Decode(rs485_t *rs485, rs485_func_t rx_Func, uint8_t *rx_Data, uint8_t rx_Data_len, rs485_func_t *tx_Func, uint8_t *tx_Data,
                           uint8_t *tx_Data_len);
rs485_error_t RS485_Encode(rs485_t *rs485, rs485_func_t Func, uint16_t stat_addr, uint8_t *Data_Num, uint8_t Data_len, uint8_t *tx_Data,
                           uint8_t *tx_Data_len);

void RS485_Pkg(rs485_t *rs485, uint8_t DstIpAddr, rs485_func_t pkg_func, uint8_t *pkg_data, uint8_t pkg_data_len);

uint32_t crc8_block_calculate(uint8_t *pbuffer, uint32_t length);

#endif
