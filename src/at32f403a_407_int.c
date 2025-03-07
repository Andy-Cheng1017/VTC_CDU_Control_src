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

#define LOG_TAG "407_Int"
#include "elog.h"

#include "FG_RPM.h"

extern FgParam_t Pump1_Fg;
extern FgParam_t Pump2_Fg;
extern FgParam_t Flow_Fg;

void NMI_Handler(void) {}

void HardFault_Handler(void) {
  log_e("HardFault_Handler");
  while (1) {
  }
}

void MemManage_Handler(void) {
  log_e("MemManage_Handler");

  while (1) {
  }
}

void BusFault_Handler(void) {
  log_e("BusFault_Handler");

  while (1) {
  }
}

void UsageFault_Handler(void) {
  log_e("UsageFault_Handler");

  while (1) {
  }
}

void DebugMon_Handler(void) {}

void TMR1_BRK_TMR9_IRQHandler(void) {
  if (tmr_flag_get(TMR9, TMR_OVF_FLAG)) {
    tmr_flag_clear(TMR9, TMR_OVF_FLAG);
    FgTimerIntHandler(&Pump1_Fg, TMR9);
    FgTimerIntHandler(&Pump2_Fg, TMR9);
    FgTimerIntHandler(&Flow_Fg, TMR9);
  }
}

void TMR1_OVF_TMR10_IRQHandler(void) {
  if (tmr_flag_get(TMR10, TMR_OVF_FLAG)) {
    tmr_flag_clear(TMR10, TMR_OVF_FLAG);
    FgTimerIntHandler(&Pump1_Fg, TMR10);
    FgTimerIntHandler(&Pump2_Fg, TMR10);
    FgTimerIntHandler(&Flow_Fg, TMR10);
  }
}

void TMR1_TRG_HALL_TMR11_IRQHandler(void) {
  if (tmr_flag_get(TMR11, TMR_OVF_FLAG)) {
    tmr_flag_clear(TMR11, TMR_OVF_FLAG);
    FgTimerIntHandler(&Pump1_Fg, TMR11);
    FgTimerIntHandler(&Pump2_Fg, TMR11);
    FgTimerIntHandler(&Flow_Fg, TMR11);
  }
}
