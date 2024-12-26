/* add user code begin Header */
/**
  **************************************************************************
  * @file     wk_rtc.c
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
#include "wk_rtc.h"
#include "rtc.h"

/* add user code begin 0 */

/* add user code end 0 */

/**
  * @brief  init rtc function.
  * @param  none
  * @retval none
  */
void wk_rtc_init(void)
{
  /* add user code begin rtc_init 0 */
  
  /* add user code end rtc_init 0 */

  calendar_type time_struct;

  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin rtc_init 1 */

  /* add user code end rtc_init 1 */

  /* configure the TAMPER pin */
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_pins = MCU_RTC_TAMPER_PIN;
  gpio_init(MCU_RTC_TAMPER_GPIO_PORT, &gpio_init_struct);

  pwc_battery_powered_domain_access(TRUE);
  
  crm_rtc_clock_select(CRM_RTC_CLOCK_LEXT);
  crm_rtc_clock_enable(TRUE);
  rtc_wait_update_finish();
  rtc_wait_config_finish();
  rtc_divider_set(32767);
  rtc_wait_config_finish();

  time_struct.year  = 2025;
  time_struct.month = 1;
  time_struct.date  = 1;
  time_struct.hour  = 0;
  time_struct.min   = 0;
  time_struct.sec   = 0;
  rtc_time_set(&time_struct);

  bpr_rtc_output_select(BPR_RTC_OUTPUT_NONE);

  /* configure tamper */
  bpr_tamper_pin_active_level_set(BPR_TAMPER_PIN_ACTIVE_LOW);
  /* add user code begin rtc_init tamper */

  /* add user code end rtc_init tamper */

  bpr_tamper_pin_enable(TRUE);

  /* add user code begin rtc_init 2 */

  /* add user code end rtc_init 2 */
}

/* add user code begin 1 */

/* add user code end 1 */
