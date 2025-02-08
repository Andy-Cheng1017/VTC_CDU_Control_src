/* add user code begin Header */
/**
 **************************************************************************
 * @file     wk_dma.c
 * @brief    work bench config program
 **************************************************************************
 *                       Copyright notice & Disclaimer
 *
 * The software Board Support Package (BSP) that is made available to
 * download from Artery official website is the copyrighted work of Artery.
 * Artery authorizes customers to use, copy, and distribute the BSP
 * software and its related documentation for the purpose of design and
 * development in conjunction with Artery microcontrollers. Use of the
 * software is governed by this copyright notice and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
 * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
 * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
 * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
 * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
 *
 **************************************************************************
 */
/* add user code end Header */

/* Includes ------------------------------------------------------------------*/
#include "wk_dma.h"


/* add user code begin 0 */
uint8_t uart_tx_buf[BUFFER_SIZE] = {0};
/* add user code end 0 */

/**
 * @brief  config dma channel transfer parameter
 * @param  dmax_channely: DMAx_CHANNELy
 * @param  peripheral_base_addr: peripheral address.
 * @param  memory_base_addr: memory address.
 * @param  buffer_size: buffer size.
 * @retval none
 */
void wk_dma_channel_config(dma_channel_type* dmax_channely, uint32_t peripheral_base_addr, uint32_t memory_base_addr, uint16_t buffer_size) {
  /* add user code begin dma_channel_config 0 */

  /* add user code end dma_channel_config 0 */

  dmax_channely->dtcnt = buffer_size;
  dmax_channely->paddr = peripheral_base_addr;
  dmax_channely->maddr = memory_base_addr;

  /* add user code begin dma_channel_config 1 */

  /* add user code end dma_channel_config 1 */
}

/**
 * @brief  init dma1 channel1 for "adc1"
 * @param  none
 * @retval none
 */
void wk_dma1_channel1_init(void) {
  /* add user code begin dma1_channel1 0 */

  /* add user code end dma1_channel1 0 */

  dma_init_type dma_init_struct;

  dma_reset(DMA1_CHANNEL1);
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_LOW;
  dma_init_struct.loop_mode_enable = TRUE;
  dma_init(DMA1_CHANNEL1, &dma_init_struct);

  /* add user code begin dma1_channel1 1 */
  wk_dma_channel_config(DMA1_CHANNEL1, (uint32_t)&ADC1->odt, DMA1_CHANNEL1_MEMORY_BASE_ADDR, DMA1_CHANNEL1_BUFFER_SIZE);
  dma_channel_enable(DMA1_CHANNEL1, TRUE);
  /* add user code end dma1_channel1 1 */
}

/**
 * @brief  init dma1 channel1 for "uart4_tx"
 * @param  none
 * @retval none
 */
void wk_dma1_channel4_init(void) {
  /* add user code begin dma1_channel4 0 */

  /* add user code end dma1_channel4 0 */

  dma_init_type dma_init_struct;

  dma_reset(DMA1_CHANNEL4);
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_LOW;
  dma_init_struct.loop_mode_enable = FALSE;
  dma_init(DMA1_CHANNEL4, &dma_init_struct);

  /* add user code begin dma1_channel4 1 */
  memset(uart_tx_buf, 0, BUFFER_SIZE);
  wk_dma_channel_config(DMA1_CHANNEL4, (uint32_t)&USART1->dt, DMA1_CHANNEL4_MEMORY_BASE_ADDR, DMA1_CHANNEL4_BUFFER_SIZE);
  dma_channel_enable(DMA1_CHANNEL4, TRUE);
  /* add user code end dma1_channel4 1 */
}

/* add user code begin 1 */
void start_dma_transfer(void) {
  if (usart_flag_get(USART1, USART_TDBE_FLAG) != RESET) {
    if (txBuffer.count > 0) {
      uint16_t len = CircularBuffer_ReadBuffer(&txBuffer, uart_tx_buf, sizeof(uart_tx_buf));
      dma_channel_enable(DMA1_CHANNEL4, FALSE);
      dma_data_number_set(DMA1_CHANNEL4, len);
      dma_channel_enable(DMA1_CHANNEL4, TRUE);
    } else {
      return;
    }
  } else {
    usart_interrupt_enable(USART1, USART_TDBE_INT, TRUE);
  }
}
/* add user code end 1 */
