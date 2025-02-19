/* add user code begin Header */
/**
 **************************************************************************
 * @file     wk_tmr.c
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
#include "wk_tmr.h"

/* add user code begin 0 */

/* add user code end 0 */

/**
 * @brief  init tmr1 function.
 * @param  none
 * @retval none
 */
void wk_tmr1_init(void) {
  /* add user code begin tmr1_init 0 */

  /* add user code end tmr1_init 0 */

  /* add user code begin tmr1_init 1 */

  /* add user code end tmr1_init 1 */

  /* configure counter settings */
  tmr_base_init(TMR1, 59999, 199);
  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR1, TMR_CLOCK_DIV1);
  tmr_repetition_counter_set(TMR1, 0x0);
  tmr_period_buffer_enable(TMR1, FALSE);

  /* configure primary mode settings */
  tmr_sub_sync_mode_set(TMR1, FALSE);
  tmr_primary_mode_select(TMR1, TMR_PRIMARY_SEL_OVERFLOW);

  // tmr_interrupt_enable(TMR1, TMR_OVF_INT, TRUE);
  tmr_counter_enable(TMR1, TRUE);

  /* add user code begin tmr1_init 2 */

  /* add user code end tmr1_init 2 */
}

/**
 * @brief  init tmr3 function.
 * @param  none
 * @retval none
 */
void wk_tmr3_init(void) {
  /* add user code begin tmr3_init 0 */

  /* add user code end tmr3_init 0 */

  /* add user code begin tmr3_init 1 */

  /* add user code end tmr3_init 1 */

  /* configure counter settings */
  tmr_base_init(TMR3, 59999, 199);
  tmr_cnt_dir_set(TMR3, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR3, TMR_CLOCK_DIV1);
  tmr_period_buffer_enable(TMR3, FALSE);

  /* configure primary mode settings */
  tmr_sub_sync_mode_set(TMR3, FALSE);
  tmr_primary_mode_select(TMR3, TMR_PRIMARY_SEL_RESET);

  /* configure overflow event */
  tmr_overflow_event_disable(TMR3, TRUE);

  tmr_counter_enable(TMR3, TRUE);

  /* add user code begin tmr3_init 2 */

  /* add user code end tmr3_init 2 */
}

/**
 * @brief  init tmr4 function.
 * @param  none
 * @retval none
 */
void wk_tmr4_init(void) {
  /* add user code begin tmr4_init 0 */

  /* add user code end tmr4_init 0 */

  gpio_init_type gpio_init_struct;
  tmr_output_config_type tmr_output_struct;

  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin tmr4_init 1 */

  /* add user code end tmr4_init 1 */

  /* configure the CH3 pin */
  gpio_init_struct.gpio_pins = GPIO_PINS_14;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init(GPIOD, &gpio_init_struct);

  /* configure the CH4 pin */
  gpio_init_struct.gpio_pins = GPIO_PINS_15;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init(GPIOD, &gpio_init_struct);

  /* GPIO PIN remap */
  gpio_pin_remap_config(TMR4_GMUX_0001, TRUE);

  /* configure counter settings */
  tmr_base_init(TMR4, 59999, 9);
  tmr_cnt_dir_set(TMR4, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR4, TMR_CLOCK_DIV1);
  tmr_period_buffer_enable(TMR4, FALSE);

  /* configure primary mode settings */
  tmr_sub_sync_mode_set(TMR4, FALSE);
  tmr_primary_mode_select(TMR4, TMR_PRIMARY_SEL_RESET);

  /* configure overflow event */
  tmr_overflow_event_disable(TMR4, TRUE);

  /* configure channel 3 output settings */
  tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_output_struct.oc_output_state = TRUE;
  tmr_output_struct.occ_output_state = FALSE;
  tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_output_struct.occ_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_output_struct.oc_idle_state = FALSE;
  tmr_output_struct.occ_idle_state = FALSE;
  tmr_output_channel_config(TMR4, TMR_SELECT_CHANNEL_3, &tmr_output_struct);
  tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_3, 0);
  tmr_output_channel_buffer_enable(TMR4, TMR_SELECT_CHANNEL_3, FALSE);

  tmr_output_channel_immediately_set(TMR4, TMR_SELECT_CHANNEL_3, FALSE);

  /* configure channel 4 output settings */
  tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_output_struct.oc_output_state = TRUE;
  tmr_output_struct.occ_output_state = FALSE;
  tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_output_struct.occ_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_output_struct.oc_idle_state = FALSE;
  tmr_output_struct.occ_idle_state = FALSE;
  tmr_output_channel_config(TMR4, TMR_SELECT_CHANNEL_4, &tmr_output_struct);
  tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_4, 0);
  tmr_output_channel_buffer_enable(TMR4, TMR_SELECT_CHANNEL_4, FALSE);

  tmr_output_channel_immediately_set(TMR4, TMR_SELECT_CHANNEL_4, FALSE);

  tmr_counter_enable(TMR4, TRUE);

  /* add user code begin tmr4_init 2 */
  tmr_output_enable(TMR4, TRUE);
  /* add user code end tmr4_init 2 */
}

/**
 * @brief  init tmr9 function.
 * @param  none
 * @retval none
 */
void wk_tmr9_init(void) {
  /* add user code begin tmr9_init 0 */

  /* add user code end tmr9_init 0 */

  /* add user code begin tmr9_init 1 */

  /* add user code end tmr9_init 1 */

  /* configure counter settings */
  tmr_base_init(TMR9, 59999, 399);
  tmr_cnt_dir_set(TMR9, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR9, TMR_CLOCK_DIV1);
  tmr_period_buffer_enable(TMR9, FALSE);

  tmr_counter_enable(TMR9, TRUE);

  tmr_interrupt_enable(TMR9, TMR_OVF_INT, TRUE);
}
/* add user code begin 1 */

/* add user code end 1 */
