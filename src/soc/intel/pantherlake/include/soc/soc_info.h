/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef _SOC_PANTHERLAKE_SOC_INFO_H_
#define _SOC_PANTHERLAKE_SOC_INFO_H_

uint8_t get_max_usb20_port(void);
uint8_t get_max_usb30_port(void);
uint8_t get_max_tcss_port(void);
uint8_t get_max_tbt_pcie_port(void);
uint8_t get_max_pcie_port(void);
uint8_t get_max_pcie_clock(void);
uint8_t get_max_uart_port(void);
uint8_t get_max_i2c_port(void);
uint8_t get_max_gspi_port(void);

#endif /* _SOC_PANTHERLAKE_SOC_INFO_H_ */
