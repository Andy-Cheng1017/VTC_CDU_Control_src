/* add user code begin Header */
/**
 **************************************************************************
 * @file     at32f403a_407_int.c
 * @brief    main interrupt service routines.
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

/* includes ------------------------------------------------------------------*/
#include "at32f403a_407_int.h"
#include "wk_dma.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "LCD_task.h"
#include "upper_task.h"

#define LOG_TAG "407_Int"
#include "elog.h"
/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */

/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */

/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

/* add user code end 0 */

/* external variables ---------------------------------------------------------*/
/* add user code begin external variables */

/* add user code end external variables */

/**
 * @brief  this function handles nmi exception.
 * @param  none
 * @retval none
 */
void NMI_Handler(void) {
  /* add user code begin NonMaskableInt_IRQ 0 */

  /* add user code end NonMaskableInt_IRQ 0 */

  /* add user code begin NonMaskableInt_IRQ 1 */

  /* add user code end NonMaskableInt_IRQ 1 */
}

/**
 * @brief  this function handles hard fault exception.
 * @param  none
 * @retval none
 */
void HardFault_Handler(void) {
  /* add user code begin HardFault_IRQ 0 */
  log_e("HardFault_Handler");
  /* add user code end HardFault_IRQ 0 */
  /* go to infinite loop when hard fault exception occurs */
  while (1) {
    /* add user code begin W1_HardFault_IRQ 0 */

    /* add user code end W1_HardFault_IRQ 0 */
  }
}

/**
 * @brief  this function handles memory manage exception.
 * @param  none
 * @retval none
 */
void MemManage_Handler(void) {
  /* add user code begin MemoryManagement_IRQ 0 */
  log_e("MemManage_Handler");
  /* add user code end MemoryManagement_IRQ 0 */
  /* go to infinite loop when memory manage exception occurs */
  while (1) {
    /* add user code begin W1_MemoryManagement_IRQ 0 */

    /* add user code end W1_MemoryManagement_IRQ 0 */
  }
}

/**
 * @brief  this function handles bus fault exception.
 * @param  none
 * @retval none
 */
void BusFault_Handler(void) {
  /* add user code begin BusFault_IRQ 0 */
  log_e("BusFault_Handler");
  /* add user code end BusFault_IRQ 0 */
  /* go to infinite loop when bus fault exception occurs */
  while (1) {
    /* add user code begin W1_BusFault_IRQ 0 */

    /* add user code end W1_BusFault_IRQ 0 */
  }
}

/**
 * @brief  this function handles usage fault exception.
 * @param  none
 * @retval none
 */
void UsageFault_Handler(void) {
  /* add user code begin UsageFault_IRQ 0 */
  log_e("UsageFault_Handler");
  /* add user code end UsageFault_IRQ 0 */
  /* go to infinite loop when usage fault exception occurs */
  while (1) {
    /* add user code begin W1_UsageFault_IRQ 0 */

    /* add user code end W1_UsageFault_IRQ 0 */
  }
}

/**
 * @brief  this function handles svcall exception.
 * @param  none
 * @retval none
 */
// void SVC_Handler(void)
// {
//   /* add user code begin SVCall_IRQ 0 */

//   /* add user code end SVCall_IRQ 0 */
//   /* add user code begin SVCall_IRQ 1 */

//   /* add user code end SVCall_IRQ 1 */
// }

/**
 * @brief  this function handles debug monitor exception.
 * @param  none
 * @retval none
 */
void DebugMon_Handler(void) {
  /* add user code begin DebugMonitor_IRQ 0 */

  /* add user code end DebugMonitor_IRQ 0 */
  /* add user code begin DebugMonitor_IRQ 1 */

  /* add user code end DebugMonitor_IRQ 1 */
}

/**
 * @brief  this function handles pendsv_handler exception.
 * @param  none
 * @retval none
 */
// void PendSV_Handler(void)
// {
//   /* add user code begin PendSV_IRQ 0 */

//   /* add user code end PendSV_IRQ 0 */
//   /* add user code begin PendSV_IRQ 1 */

//   /* add user code end PendSV_IRQ 1 */
// }

/**
 * @brief  this function handles systick handler.
 * @param  none
 * @retval none
 */
// void SysTick_Handler(void)
// {
//   /* add user code begin SysTick_IRQ 0 */

//   /* add user code end SysTick_IRQ 0 */

//   /* add user code begin SysTick_IRQ 1 */

//   /* add user code end SysTick_IRQ 1 */
// }


void USART1_IRQHandler(void) {
  if (usart_interrupt_flag_get(USART1, USART_TDBE_FLAG) != RESET) {
    usart_interrupt_enable(USART1, USART_TDBE_INT, FALSE);
    start_dma_transfer();
    // usart_flag_clear(USART1, USART_TDBE_FLAG);
  }
}

/**
 * @brief  this function handles USART2 handler.
 * @param  none
 * @retval none
 */
void USART2_IRQHandler(void) {
  /* add user code begin USART2_IRQ 0 */

  /* add user code end USART2_IRQ 0 */
  /* add user code begin USART2_IRQ 1 */

  /* add user code end USART2_IRQ 1 */
}

/**
 * @brief  this function handles UART4 handler.
 * @param  none
 * @retval none
 */
void UART4_IRQHandler(void) {
  if (usart_interrupt_flag_get(RS485_UPPER.UART, USART_RDBF_FLAG) != RESET) {
    usart_flag_clear(RS485_UPPER.UART, USART_RDBF_FLAG);
    RS485_Rx_Data_ISR(&RS485_UPPER);

  } else if (usart_interrupt_flag_get(RS485_UPPER.UART, USART_IDLEF_FLAG) != RESET) {
    usart_flag_clear(RS485_UPPER.UART, USART_IDLEF_FLAG);
    RS485_Rx_Cplt_ISR(&RS485_UPPER);

  } else if (usart_interrupt_flag_get(RS485_UPPER.UART, USART_TDBE_FLAG) != RESET) {
    usart_flag_clear(RS485_UPPER.UART, USART_TDBE_FLAG);
    usart_interrupt_enable(RS485_UPPER.UART, USART_TDBE_INT, FALSE);
    RS485_UPPER.tx_idex--;
    RS485_Tx_Data_ISR(&RS485_UPPER);
  }
}

void UART8_IRQHandler(void) {
  if (usart_interrupt_flag_get(RS485_LCD.UART, USART_RDBF_FLAG) != RESET) {
    usart_flag_clear(RS485_LCD.UART, USART_RDBF_FLAG);
    RS485_Rx_Data_ISR(&RS485_LCD);

  } else if (usart_interrupt_flag_get(RS485_LCD.UART, USART_IDLEF_FLAG) != RESET) {
    usart_flag_clear(RS485_LCD.UART, USART_IDLEF_FLAG);
    RS485_Rx_Cplt_ISR(&RS485_LCD);

  } else if (usart_interrupt_flag_get(RS485_LCD.UART, USART_TDBE_FLAG) != RESET) {
    usart_flag_clear(RS485_LCD.UART, USART_TDBE_FLAG);
    usart_interrupt_enable(RS485_LCD.UART, USART_TDBE_INT, FALSE);
    RS485_LCD.tx_idex--;
    RS485_Tx_Data_ISR(&RS485_LCD);
  }
}
