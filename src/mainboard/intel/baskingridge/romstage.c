/* SPDX-License-Identifier: GPL-2.0-only */

#include <stdint.h>
#include <stddef.h>
#include <arch/romstage.h>
#include <cpu/intel/haswell/haswell.h>
#include <northbridge/intel/haswell/haswell.h>
#include <northbridge/intel/haswell/raminit.h>
#include <southbridge/intel/lynxpoint/pch.h>
#include <southbridge/intel/common/gpio.h>

void mainboard_config_rcba(void)
{
	/*
	 *             GFX    INTA -> PIRQA (MSI)
	 * D28IP_P1IP  WLAN   INTA -> PIRQB
	 * D28IP_P4IP  ETH0   INTB -> PIRQC
	 * D29IP_E1P   EHCI1  INTA -> PIRQD
	 * D26IP_E2P   EHCI2  INTA -> PIRQE
	 * D31IP_SIP   SATA   INTA -> PIRQF (MSI)
	 * D31IP_SMIP  SMBUS  INTB -> PIRQG
	 * D31IP_TTIP  THRT   INTC -> PIRQH
	 * D27IP_ZIP   HDA    INTA -> PIRQG (MSI)
	 */

	/* Device interrupt pin register (board specific) */
	RCBA32(D31IP) = (INTC << D31IP_TTIP) | (NOINT << D31IP_SIP2) |
			(INTB << D31IP_SMIP) | (INTA << D31IP_SIP);
	RCBA32(D30IP) = (NOINT << D30IP_PIP);
	RCBA32(D29IP) = (INTA << D29IP_E1P);
	RCBA32(D28IP) = (INTA << D28IP_P1IP) | (INTC << D28IP_P3IP) |
			(INTB << D28IP_P4IP);
	RCBA32(D27IP) = (INTA << D27IP_ZIP);
	RCBA32(D26IP) = (INTA << D26IP_E2P);
	RCBA32(D25IP) = (NOINT << D25IP_LIP);
	RCBA32(D22IP) = (NOINT << D22IP_MEI1IP);

	/* Device interrupt route registers */
	RCBA32(D31IR) = DIR_ROUTE(PIRQF, PIRQG, PIRQH, PIRQA);
	RCBA32(D29IR) = DIR_ROUTE(PIRQD, PIRQE, PIRQF, PIRQG);
	RCBA32(D28IR) = DIR_ROUTE(PIRQB, PIRQC, PIRQD, PIRQE);
	RCBA32(D27IR) = DIR_ROUTE(PIRQG, PIRQH, PIRQA, PIRQB);
	RCBA32(D26IR) = DIR_ROUTE(PIRQE, PIRQF, PIRQG, PIRQH);
	RCBA32(D25IR) = DIR_ROUTE(PIRQA, PIRQB, PIRQC, PIRQD);
	RCBA32(D22IR) = DIR_ROUTE(PIRQA, PIRQB, PIRQC, PIRQD);
}

void mainboard_romstage_entry(void)
{
	struct pei_data pei_data = {
		.pei_version = PEI_VERSION,
		.mchbar = (uintptr_t)DEFAULT_MCHBAR,
		.dmibar = (uintptr_t)DEFAULT_DMIBAR,
		.epbar = DEFAULT_EPBAR,
		.pciexbar = CONFIG_MMCONF_BASE_ADDRESS,
		.smbusbar = SMBUS_IO_BASE,
		.hpet_address = HPET_ADDR,
		.rcba = (uintptr_t)DEFAULT_RCBA,
		.pmbase = DEFAULT_PMBASE,
		.gpiobase = DEFAULT_GPIOBASE,
		.temp_mmio_base = 0xfed08000,
		.system_type = 0, // 0 Mobile, 1 Desktop/Server
		.tseg_size = CONFIG_SMM_TSEG_SIZE,
		.spd_addresses = { 0xa0, 0xa2, 0xa4, 0xa6 },
		.ec_present = 0,
		// 0 = leave channel enabled
		// 1 = disable dimm 0 on channel
		// 2 = disable dimm 1 on channel
		// 3 = disable dimm 0+1 on channel
		.dimm_channel0_disabled = 0,
		.dimm_channel1_disabled = 0,
		.max_ddr3_freq = 1600,
		.usb2_ports = {
			/* Length, Enable, OCn#, Location */
			{ 0x0040, 1, 0, /* P0: Back USB3 port  (OC0) */
			  USB_PORT_BACK_PANEL },
			{ 0x0040, 1, 0, /* P1: Back USB3 port  (OC0) */
			  USB_PORT_BACK_PANEL },
			{ 0x0040, 1, 1, /* P2: Flex Port on bottom (OC1) */
			  USB_PORT_FLEX },
			{ 0x0040, 1, USB_OC_PIN_SKIP, /* P3: Dock connector */
			  USB_PORT_DOCK },
			{ 0x0040, 1, USB_OC_PIN_SKIP, /* P4: Mini PCIE  */
			  USB_PORT_MINI_PCIE },
			{ 0x0040, 1, 1, /* P5: USB eSATA header (OC1) */
			  USB_PORT_FLEX },
			{ 0x0040, 1, 3, /* P6: Front Header J8H2 (OC3) */
			  USB_PORT_FRONT_PANEL },
			{ 0x0040, 1, 3, /* P7: Front Header J8H2 (OC3) */
			  USB_PORT_FRONT_PANEL },
			{ 0x0040, 1, 4, /* P8: USB/LAN Jack (OC4) */
			  USB_PORT_FRONT_PANEL },
			{ 0x0040, 1, 4, /* P9: USB/LAN Jack (OC4) */
			  USB_PORT_FRONT_PANEL },
			{ 0x0040, 1, 5, /* P10: Front Header J7H3 (OC5) */
			  USB_PORT_FRONT_PANEL },
			{ 0x0040, 1, 5, /* P11: Front Header J7H3 (OC5) */
			  USB_PORT_FRONT_PANEL },
			{ 0x0040, 1, 6, /* P12: USB/DP Jack (OC6) */
			  USB_PORT_FRONT_PANEL },
			{ 0x0040, 1, 6, /* P13: USB/DP Jack (OC6) */
			  USB_PORT_FRONT_PANEL },
		},
		.usb3_ports = {
			/* Enable, OCn# */
			{ 1, 0 }, /* P1; */
			{ 1, 0 }, /* P2; */
			{ 1, 0 }, /* P3; */
			{ 1, 0 }, /* P4; */
			{ 1, 0 }, /* P6; */
			{ 1, 0 }, /* P6; */
		},
	};

	struct romstage_params romstage_params = {
		.pei_data = &pei_data,
	};

	/* Call into the real romstage main with this board's attributes. */
	romstage_common(&romstage_params);
}
