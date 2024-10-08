# SPDX-License-Identifier: GPL-2.0-only
bootblock-y += gpio.c

romstage-y += gpio.c
romstage-y += memory.c

ramstage-y += gpio.c
ramstage-y += ramstage.c
ramstage-y += variant.c

$(call add_vbt_to_cbfs, vbt-kinox_HDMI.bin, data-kinox_HDMI.vbt)
