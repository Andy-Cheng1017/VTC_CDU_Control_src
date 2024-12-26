/* add user code begin Header */
/**
 **************************************************************************
 * @file     wk_emac.c
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
// #include "at32f403a_407_board.h"
#include "lwip/dhcp.h"
#include "wk_emac.h"
#include "task.h"


/* add user code begin 0 */

/* add user code end 0 */

#define LAN8720
#define PHY_ADDRESS (1)             /*!< relative to at32 board */
#define PHY_CONTROL_REG (0x0)       /*!< basic mode control register */
#define PHY_STATUS_REG (0x1)        /*!< basic mode status register */
#define PHY_SPECIFIED_CS_REG (0x1F) /*!< specified configuration and status register */
/* phy control register */
#define PHY_AUTO_NEGOTIATION_BIT (0x1000) /*!< enable auto negotiation */
#define PHY_LOOPBACK_BIT (0x4000)         /*!< enable loopback */
#define PHY_RESET_BIT (0x8000)            /*!< reset phy */
/* phy status register */
#define PHY_LINKED_STATUS_BIT (0x0004) /*!< link status */
#define PHY_NEGO_COMPLETE_BIT (0x0020) /*!< auto negotiation complete */

#define PHY_FULL_DUPLEX_100MBPS_BIT (0x0018) /*!< full duplex 100 mbps */
#define PHY_HALF_DUPLEX_100MBPS_BIT (0x0008) /*!< half duplex 100 mbps */
#define PHY_FULL_DUPLEX_10MBPS_BIT (0x0014)  /*!< full duplex 10 mbps */
#define PHY_HALF_DUPLEX_10MBPS_BIT (0x0004)  /*!< half duplex 10 mbps */
#define PHY_DUPLEX_MBPS_MSK (0x001C)
#define PHY_DUPLEX_MODE (0x0100) /*!< full duplex mode */
#define PHY_SPEED_MODE (0x2000)  /*!< 10 mbps */

emac_control_config_type mac_control_para;

void static reset_phy(void) {
  gpio_init_type gpio_init_struct = {0};

  gpio_default_para_init(&gpio_init_struct);

  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pins = GPIO_PINS_15;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOB, &gpio_init_struct);

  gpio_bits_reset(GPIOB, GPIO_PINS_15);
  vTaskDelay(2);
  gpio_bits_set(GPIOB, GPIO_PINS_15);
  vTaskDelay(1);
}

/**
 * @brief  set mac speed related parameters
 * @param  nego: auto negotiation on or off.
 *         this parameter can be one of the following values:
 *         - EMAC_AUTO_NEGOTIATION_OFF
 *         - EMAC_AUTO_NEGOTIATION_ON.
 * @param  mode: half-duplex or full-duplex.
 *         this parameter can be one of the following values:
 *         - EMAC_HALF_DUPLEX
 *         - EMAC_FULL_DUPLEX.
 * @param  speed: 10 mbps or 100 mbps
 *         this parameter can be one of the following values:
 *         - EMAC_SPEED_10MBPS
 *         - EMAC_SPEED_100MBPS.
 * @retval none
 */
error_status emac_speed_config(emac_auto_negotiation_type nego, emac_duplex_type mode, emac_speed_type speed) {
  uint16_t data = 0;
  uint32_t timeout = 0;
  if (nego == EMAC_AUTO_NEGOTIATION_ON) {
    do {
      timeout++;
      if (emac_phy_register_read(PHY_ADDRESS, PHY_STATUS_REG, &data) == ERROR) {
        return ERROR;
      }
    } while (!(data & PHY_LINKED_STATUS_BIT) && (timeout < PHY_TIMEOUT));

    if (timeout == PHY_TIMEOUT) {
      return ERROR;
    }

    timeout = 0;

    if (emac_phy_register_write(PHY_ADDRESS, PHY_CONTROL_REG, PHY_AUTO_NEGOTIATION_BIT) == ERROR) {
      return ERROR;
    }

    do {
      timeout++;
      if (emac_phy_register_read(PHY_ADDRESS, PHY_STATUS_REG, &data) == ERROR) {
        return ERROR;
      }
    } while (!(data & PHY_NEGO_COMPLETE_BIT) && (timeout < PHY_TIMEOUT));

    if (timeout == PHY_TIMEOUT) {
      return ERROR;
    }

    if (emac_phy_register_read(PHY_ADDRESS, PHY_SPECIFIED_CS_REG, &data) == ERROR) {
      return ERROR;
    }

    if ((data & PHY_DUPLEX_MBPS_MSK) == PHY_FULL_DUPLEX_100MBPS_BIT) {
      emac_duplex_mode_set(EMAC_FULL_DUPLEX);
      emac_fast_speed_set(EMAC_SPEED_100MBPS);
    }

    if ((data & PHY_DUPLEX_MBPS_MSK) == PHY_HALF_DUPLEX_100MBPS_BIT) {
      emac_duplex_mode_set(EMAC_HALF_DUPLEX);
      emac_fast_speed_set(EMAC_SPEED_100MBPS);
    }

    if ((data & PHY_DUPLEX_MBPS_MSK) == PHY_FULL_DUPLEX_10MBPS_BIT) {
      emac_duplex_mode_set(EMAC_FULL_DUPLEX);
      emac_fast_speed_set(EMAC_SPEED_100MBPS);
    }

    if ((data & PHY_DUPLEX_MBPS_MSK) == PHY_FULL_DUPLEX_10MBPS_BIT) {
      emac_duplex_mode_set(EMAC_HALF_DUPLEX);
      emac_fast_speed_set(EMAC_SPEED_100MBPS);
    }
  } else {
    if (emac_phy_register_write(PHY_ADDRESS, PHY_CONTROL_REG, (uint16_t)((mode << 8) | (speed << 13))) == ERROR) {
      return ERROR;
    }
    if (speed == EMAC_SPEED_100MBPS) {
      emac_fast_speed_set(EMAC_SPEED_100MBPS);
    } else {
      emac_fast_speed_set(EMAC_SPEED_10MBPS);
    }
    if (mode == EMAC_FULL_DUPLEX) {
      emac_duplex_mode_set(EMAC_FULL_DUPLEX);
    } else {
      emac_duplex_mode_set(EMAC_HALF_DUPLEX);
    }
  }
  return SUCCESS;
}

/**
 * @brief  init emac phy function.
 * @param  emac_auto_negotiation_type
 * @param  emac_duplex_type
 * @param  emac_speed_type
 * @retval   error_status
 */
error_status wk_emac_phy_init(emac_auto_negotiation_type nego) {
  /* add user code begin emac_phy_init 0 */

  /* add user code end emac_phy_init 0 */

  uint32_t delay, timeout = 0;
  uint16_t data = 0;

  /* add user code begin emac_phy_init 1 */

  /* add user code end emac_phy_init 1*/

  /*PHY Config*/
  if (emac_phy_register_write(PHY_ADDRESS, PHY_CONTROL_REG, PHY_RESET_BIT) == ERROR)  // PHY_ADDRESS, PHY_CONTROL_REG, PHY_RESET_BIT
  {
    return ERROR;
  }

  for (delay = 0; delay < 0x000FFFFF; delay++);

  do {
    timeout++;
    if (emac_phy_register_read(PHY_ADDRESS, PHY_CONTROL_REG, &data) == ERROR) {
      return ERROR;
    }
  } while ((data & PHY_RESET_BIT) && (timeout < PHY_TIMEOUT));

  for (delay = 0; delay < 0x00FFFFF; delay++);

  if (timeout == PHY_TIMEOUT) {
    return ERROR;
  }
  // emac_speed_config(nego, mode, speed);
  /* add user code begin emac_phy_init 2 */

  /* add user code end emac_phy_init 2 */
  return SUCCESS;
}

/**
 * @brief  init emac function.
 * @param  none
 * @retval   error_status
 */
error_status wk_emac_init(void) {
  /* add user code begin emac_init 0 */

  /* add user code end emac_init 0 */

  gpio_init_type gpio_init_struct;
  emac_dma_config_type dma_control_para;
  static uint8_t macaddr[6];
  /* add user code begin emac_init 1 */

  /* add user code end emac_init 1*/

  /* configure the MDC pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_1;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOC, &gpio_init_struct);

  /* configure the RMII_REF_CLK pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_1;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* configure the MDIO pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_2;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* configure the RMII_TX_EN pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_11;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOB, &gpio_init_struct);

  /* configure the RMII_TXD0 pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_12;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOB, &gpio_init_struct);

  /* configure the RMII_TXD1 pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_13;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOB, &gpio_init_struct);

  /* configure the RMII_CRS_DV pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOD, &gpio_init_struct);

  /* configure the RMII_RXD0 pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_9;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOD, &gpio_init_struct);

  /* configure the RMII_RXD1 pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_10;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOD, &gpio_init_struct);

  /* GPIO PIN remap */
  gpio_pin_remap_config(EMAC_GMUX_01, TRUE);

  /* config RMII */
  gpio_pin_remap_config(MII_RMII_SEL_GMUX, TRUE);

  /* reset phy */
  reset_phy();
  /* reset emac ahb bus */
  emac_reset();

  /* software reset emac dma */
  emac_dma_software_reset_set();

  while (emac_dma_software_reset_get() == SET);

  emac_clock_range_set();

  emac_control_para_init(&mac_control_para);
  mac_control_para.auto_nego = EMAC_AUTO_NEGOTIATION_ON;
  // mac_control_para.duplex_mode = EMAC_FULL_DUPLEX;
  // mac_control_para.fast_ethernet_speed = EMAC_SPEED_100MBPS;
  // mac_control_para.ipv4_checksum_offload = TRUE;

  emac_control_config(&mac_control_para);

  /*phy init*/
  if (wk_emac_phy_init(mac_control_para.auto_nego) != SUCCESS) {
    return ERROR;
  }

  emac_dma_para_init(&dma_control_para);

  dma_control_para.rsf_enable = TRUE;
  dma_control_para.tsf_enable = TRUE;
  dma_control_para.osf_enable = TRUE;
  dma_control_para.aab_enable = TRUE;
  dma_control_para.usp_enable = TRUE;
  dma_control_para.fb_enable = TRUE;
  dma_control_para.flush_rx_disable = TRUE;
  dma_control_para.rx_dma_pal = EMAC_DMA_PBL_32;
  dma_control_para.tx_dma_pal = EMAC_DMA_PBL_32;
  dma_control_para.priority_ratio = EMAC_DMA_2_RX_1_TX;

  emac_dma_config(&dma_control_para);

  macaddr[0] = 0x00;
  macaddr[1] = 0x00;
  macaddr[2] = 0x44;
  macaddr[3] = 0x45;
  macaddr[4] = 0x56;
  macaddr[5] = 0x01;
  emac_local_address_set(macaddr);

  /* add user code begin emac_init 2 */

  /* add user code end emac_init 2 */

  /* add user code begin emac_init 3 */

  /* add user code end emac_init 3 */

  return SUCCESS;
}

/* add user code begin 1 */

/**
  * @brief  updates the link states
  * @param  none
  * @retval link state 0: disconnect, 1: connection
  */
uint16_t link_update(void)
{
  uint16_t link_data, link_state;
  if(emac_phy_register_read(PHY_ADDRESS, PHY_STATUS_REG, &link_data) == ERROR)
  {
    return ERROR;
  }
  
  link_state = (link_data & PHY_LINKED_STATUS_BIT)>>2;
  return link_state;
}

/**
  * @brief  this function sets the netif link status.
  * @param  netif: the network interface
  * @retval none
  */  
void ethernetif_set_link(void const *argument)
{
  uint16_t regvalue = 0;
  struct netif *netif = (struct netif *)argument;
  
  for(;;)
  {
    /* Read PHY_BSR*/
    regvalue = link_update();
    
    // if(regvalue > 0)
    // {
    //   at32_led_on(LED2);
    // }
    // else
    // {
    //   at32_led_off(LED2);
    // }
    /* Check whether the netif link down and the PHY link is up */
    if(!netif_is_link_up(netif) && (regvalue))
    {
      /* network cable is connected */ 
      netif_set_link_up(netif);        
    }
    else if(netif_is_link_up(netif) && (!regvalue))
    {
      /* network cable is dis-connected */
      netif_set_link_down(netif);
    }
    
    /* Suspend thread for 200 ms */
    vTaskDelay(200);
  }
}

/**
  * @brief  this function notify user about link status changement.
  * @param  netif: the network interface
  * @retval none
  */
void ethernetif_notify_conn_changed(struct netif *netif)
{
  /* note : this is function could be implemented in user file 
            when the callback is needed,
  */

  if (netif_is_link_up(netif)) {
    netif_set_up(netif);

#if LWIP_DHCP
    /*  creates a new dhcp client for this interface on the first call.
    note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
    the predefined regular intervals after starting the client.
    you can peek in the netif->dhcp struct for the actual dhcp status.*/
    dhcp_start(netif);
#endif
  }
  else
    netif_set_down(netif);
}

/**
 * @brief  link callback function, this function is called on change of link status
 *         to update low level driver configuration.
 * @param  netif: the network interface
 * @retval none
 */
void ethernetif_update_config(struct netif *netif) {
  if (netif_is_link_up(netif)) {
    printf("Link is up\n");
    // at32_led_on(LED2);
    emac_speed_config(mac_control_para.auto_nego, mac_control_para.duplex_mode, mac_control_para.fast_ethernet_speed);

    vTaskDelay(300);
    /* enable mac and dma transmission and reception */
    emac_start();
  } else {
    /* disable mac and dma transmission and reception */
    // at32_led_off(LED2);
    printf("Link is down\n");
    emac_stop();
  }

  ethernetif_notify_conn_changed(netif);
}
/* add user code end 1 */
