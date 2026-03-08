################################################################################
#
# Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
# This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
# Makefile used for building DDI
#
################################################################################
#
#
#
#
#
#

ifeq ($(CHIP), nike)

MXLIB =
MXLIB += $(ROOTLIB)/api/ace
MXLIB += $(ROOTLIB)/api/audio
MXLIB += $(ROOTLIB)/api/cec
MXLIB += $(ROOTLIB)/api/dlc
MXLIB += $(ROOTLIB)/api/dmx
MXLIB += $(ROOTLIB)/api/gfx
MXLIB += $(ROOTLIB)/api/gop
MXLIB += $(ROOTLIB)/api/gpd
MXLIB += $(ROOTLIB)/api/mhl
MXLIB += $(ROOTLIB)/api/njpeg_ex
MXLIB += $(ROOTLIB)/api/pnl
MXLIB += $(ROOTLIB)/api/swi2c
MXLIB += $(ROOTLIB)/api/vdec_ex
MXLIB += $(ROOTLIB)/api/xc

MXLIB += $(ROOTLIB)/drv/aesdma
MXLIB += $(ROOTLIB)/drv/audio
MXLIB += $(ROOTLIB)/drv/audsp
MXLIB += $(ROOTLIB)/drv/avd
MXLIB += $(ROOTLIB)/drv/cpu
MXLIB += $(ROOTLIB)/drv/ddc2bi
MXLIB += $(ROOTLIB)/drv/demod
MXLIB += $(ROOTLIB)/drv/dscmb
MXLIB += $(ROOTLIB)/drv/flash/serial
MXLIB += $(ROOTLIB)/drv/gpio
MXLIB += $(ROOTLIB)/drv/hwi2c
MXLIB += $(ROOTLIB)/drv/ipauth
MXLIB += $(ROOTLIB)/drv/ir
MXLIB += $(ROOTLIB)/drv/mspi
MXLIB += $(ROOTLIB)/drv/mvop
MXLIB += $(ROOTLIB)/drv/pcmcia
MXLIB += $(ROOTLIB)/drv/pm
MXLIB += $(ROOTLIB)/drv/pwm
MXLIB += $(ROOTLIB)/drv/pws
MXLIB += $(ROOTLIB)/drv/rtc
MXLIB += $(ROOTLIB)/drv/sar
MXLIB += $(ROOTLIB)/drv/tsp
MXLIB += $(ROOTLIB)/drv/urdma
MXLIB += $(ROOTLIB)/drv/vbi
MXLIB += $(ROOTLIB)/drv/ve
MXLIB += $(ROOTLIB)/drv/vif
MXLIB += $(ROOTLIB)/drv/wdt

ifeq ($(MCU_TYPE), arm_ca7)
MXLIB += $(ROOTLIB)/drv/dip
endif

ifeq ($(OS_TYPE), linux)

ifneq ($(PLATFORM), android)
MXLIB += $(ROOTLIB)/api/hsl
MXLIB += $(ROOTLIB)/msfs/$(OS_TYPE)
endif

MXLIB += $(ROOTLIB)/drv/sc
MXLIB += $(ROOTLIB)/api/mfe_ex
MXLIB += $(ROOTLIB)/drv/mfe_ex

else
MXLIB += $(ROOTLIB)/drv/usbhost/source/usb_hid_p1
MXLIB += $(ROOTLIB)/drv/usbhost/source2/usb_hid_p2
MXLIB += $(ROOTLIB)/drv/usbhost/source3/usb_hid_p3
MXLIB += $(ROOTLIB)/drv/usbhost/source/usb_host_p1
MXLIB += $(ROOTLIB)/drv/usbhost/source2/usb_host_p2
MXLIB += $(ROOTLIB)/drv/usbhost/source3/usb_host_p3

MXLIB += $(ROOTLIB)/drv/miu
MXLIB += $(ROOTLIB)/drv/irq
MXLIB += $(ROOTLIB)/drv/sys
MXLIB += $(ROOTLIB)/drv/uart
MXLIB += $(ROOTLIB)/drv/bdma
MXLIB += $(ROOTLIB)/drv/sem
MXLIB += $(ROOTLIB)/drv/mmio
MXLIB += $(ROOTLIB)/drv/mbx

endif
endif
