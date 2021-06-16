/* SPDX-License-Identifier: GPL-2.0-only */

#include <acpi/acpi.h>
#include <arch/ioapic.h>
#include <northbridge/amd/nb_common.h>

unsigned long acpi_fill_madt(unsigned long current)
{
	/* create all subtables for processors */
	current = acpi_create_madt_lapics_with_nmis(current);

	/* Write SB800 IOAPIC, only one */
	current += acpi_create_madt_ioapic((acpi_madt_ioapic_t *)current, CONFIG_MAX_CPUS,
					   IO_APIC_ADDR, 0);

	/* TODO: Remove the hardcode */
	current += acpi_create_madt_ioapic((acpi_madt_ioapic_t *)current, CONFIG_MAX_CPUS + 1,
					   IO_APIC2_ADDR, 24);

	current += acpi_create_madt_irqoverride((acpi_madt_irqoverride_t *)
						current, 0, 0, 2, 0);
	current += acpi_create_madt_irqoverride((acpi_madt_irqoverride_t *)
						current, 0, 9, 9, 0xF);
	/* 0: mean bus 0--->ISA */
	/* 0: PIC 0 */
	/* 2: APIC 2 */
	/* 5 mean: 0101 --> Edge-triggered, Active high */

	return current;
}
