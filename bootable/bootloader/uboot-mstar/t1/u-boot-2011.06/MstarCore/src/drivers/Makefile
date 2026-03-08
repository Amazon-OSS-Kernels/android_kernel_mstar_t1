#
# (C) Copyright 2000-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#
include $(TOPDIR)/config.mk
include $(TOPDIR)/../sboot/.config

CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
#CONFIG_MSTAR_CHIP_NAME := amber5
#CFLAGS += -DET_DEBUG -DDEBUG

LIB	= $(obj)libdrivers.a

COBJS =

ifeq ($(CONFIG_BOOTVIDEO),y)

# DEMUX
COBJS += ./demux/MsDrvDemux.o

# VIDEO
COBJS += ./video/MsDrvVideo.o
endif

ifeq ($(CONFIG_SD),y)
CFLAGS += -I./sd/inc/common/
CFLAGS += -I./sd/inc/config/
CPPFLAGS += -I./sd/inc/common/
CPPFLAGS += -I./sd/inc/config/

COBJS += ./sd/src/common/drvFCIE_v5.o
COBJS += ./sd/src/common/DrvSd.o
COBJS += ./sd/src/common/DrvSdioIPVerify.o
COBJS += ./sd/src/common/fcie_mmc.o
COBJS += ./sd/src/common/mmc.o
COBJS += ./sd/src/config/$(CONFIG_MSTAR_CHIP_NAME)/HalSdio_platform.o
endif

ifeq ($(CONFIG_MSTAR_FTL),y)
CFLAGS += -I./unfd/inc/common/
CFLAGS += -I./unfd/inc/api/
CFLAGS += -I./unfd/inc/config/
CPPFLAGS += -I./unfd/inc/common/
CPPFLAGS += -I./unfd/inc/api/
CPPFLAGS += -I./unfd/inc/config/

CFLAGS += -I./unfd/ftl/inc/
CPPFLAGS += -I./unfd/ftl/inc/

COBJS += ./unfd/ftl/ftl.o
COBJS += ./unfd/ftl/src/ftl_api.o
COBJS += ./unfd/ftl/src/ftl_hal.o
COBJS += ./unfd/ftl/src/ftl_utl.o
endif

ifeq ($(CONFIG_NAND),y)
CFLAGS += -I./unfd/inc/api/
CFLAGS += -I./unfd/inc/common/
CFLAGS += -I./unfd/inc/config/
CFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/nand/
CPPFLAGS += -I./unfd/inc/api/
CPPFLAGS += -I./unfd/inc/common/
CPPFLAGS += -I./unfd/inc/config/
CPPFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/nand/

COBJS += ../hal/$(CONFIG_MSTAR_CHIP_NAME)/nand/drvNAND_$(CONFIG_MSTAR_CHIP_NAME)_platform.o
COBJS += ./unfd/src/common/drvNAND_hal.o
COBJS += ./unfd/src/common/drvNAND_ReadRetry.o
COBJS += ./unfd/nand.o
COBJS += ./unfd/src/common/drvNAND_utl.o
COBJS += ./unfd/src/common/drvNAND_ip_verify.o
COBJS += ./unfd/src/common/drvNAND_hal_v5.o
COBJS += ./unfd/src/common/drvNAND_ftl.o
COBJS += ./unfd/src/common/drvNAND_task.o
COBJS += ./unfd/src/common/drvNAND_ReadRetry_v5.o
COBJS += ./unfd/src/api/drvNAND_prg.o
endif

ifeq ($(CONFIG_SPI_NAND),y)
CFLAGS += -I./spinand/inc/common/
CFLAGS += -I./spinand/inc/config/
CPPFLAGS += -I./spinand/inc/common/
CPPFLAGS += -I./spinand/inc/config/
COBJS += ./spinand/src/config/$(CONFIG_MSTAR_CHIP_NAME)/drvSPINAND_hal.o
COBJS += ./spinand/src/common/drvSPINAND.o
COBJS += ./spinand/spinand.o
endif

ifeq ($(CONFIG_FRONT_END),y)
COBJS += ./tuner/Tuner.o
COBJS += ./cofdm/COFDM_Demodulator.o
endif

ifeq ($(CONFIG_OAD_IN_MBOOT),y)
COBJS += ./oad/msAPI_oad_biop.o
COBJS += ./oad/msAPI_oad_parser.o
COBJS += ./oad/msAPI_oad_process.o
COBJS += ./oad/msAPI_demux.o
COBJS += ./oad/cus_oad/msAPI_oad_scan.o
COBJS += ./oad/cus_oad/apiRfChannel.o
COBJS += ./oad/msAPI_Memory.o
COBJS += ./oad/oad_main.o
COBJS += ./iic/drvIIC.o
endif

ifeq ($(CONFIG_GMAC),y)
COBJS += ../hal/$(CONFIG_MSTAR_CHIP_NAME)/gmac/mhal_gmac.o
COBJS += ./gmac/mdrv_gmac.o
endif

ifeq ($(CONFIG_INX_NOVA_VB1),y)
COBJS += ./panel/panel_INX_NOVA_vb1.o
endif

ifeq ($(CONFIG_INX_VB1),y)
COBJS += ./panel/panel_INX_vb1.o
endif

ifeq ($(CONFIG_URSA_6M30),y)
COBJS += ./ursa/ursa_6m30.o
endif

ifeq ($(CONFIG_URSA_6M40),y)
COBJS += ./ursa/ursa_6m40.o
endif

ifeq ($(CONFIG_URSA6_VB1),y)
COBJS += ./ursa/ursa_6m38.o
endif

ifeq ($(CONFIG_NOVA_KS2),y)
COBJS += ./ursa/ursa_6m38.o
endif

ifeq ($(CONFIG_URSA7_VB1),y)
COBJS += ./ursa/ursa_7.o
endif

ifeq ($(CONFIG_URSA_8),y)
COBJS += ./ursa/ursa_8.o
endif

ifeq ($(CONFIG_URSA9_VB1),y)
endif

ifeq ($(CONFIG_URSA_10),y)
COBJS += ./ursa/ursa_10.o
endif

ifeq ($(CONFIG_URSA_UNION),y)
COBJS += ./ursa/ursa_common.o
COBJS += ./ursa/ursa_6m38.o
COBJS += ./ursa/ursa_9.o
endif

ifeq ($(CONFIG_USB),y)
CFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/usb/
CPPFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/usb/
ifeq ($(CONFIG_MSTAR_MONACO),y)
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
else ifeq ($(CONFIG_MSTAR_MIAMI),y)
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
else ifeq ($(CONFIG_MSTAR_MUJI),y)
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
else ifeq ($(CONFIG_MSTAR_CELTICS),y)
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
else ifeq ($(CONFIG_MSTAR_MONET),y)
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
else ifeq ($(CONFIG_MSTAR_MESSI),y)
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
else ifeq ($(CONFIG_MSTAR_MUNICH),y)
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
else ifeq ($(CONFIG_MSTAR_MULAN),y)
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
else ifeq ($(CONFIG_MSTAR_KRATOS),y)
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
else ifeq ($(CONFIG_MSTAR_KIWI),y)
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
else ifeq ($(CONFIG_MSTAR_MANHATTAN),y)
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
else ifeq ($(CONFIG_MSTAR_MASERATI),y)
else ifeq ($(CONFIG_MSTAR_KANO),y)
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
else ifeq ($(CONFIG_MSTAR_CURRY),y)
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
else ifeq ($(CONFIG_MSTAR_MASERATI),y)
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
else ifeq ($(CONFIG_MSTAR_MACAN),y)
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
else ifeq ($(CONFIG_MSTAR_KAYLA),y)
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
else ifeq ($(CONFIG_MSTAR_MAXIM),y)
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
else ifeq ($(CONFIG_MSTAR_MUSTANG),y)
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
else ifeq ($(CONFIG_MSTAR_KASTOR),y)
COBJS += ./usb20/drvhostlib.o
COBJS += ./usb20/ubootglue.o
COBJS += ./usb20/usb_lan.o
COBJS += ./usb20/ax88772a.o
COBJS += ./usb20/rtl8150.o
COBJS += ./usb20/pegasus.o
else
COBJS += ./usb/usbdcore.o
COBJS += ./usb/usbtty.o
COBJS += ./usb/drvusbmain.o
COBJS += ./usb/drvhostlib.o
COBJS += ./usb/drvusbmain2.o
COBJS += ./usb/drvhostlib2.o
COBJS += ./usb/drvusbmain3.o
COBJS += ./usb/drvhostlib3.o
COBJS += ./usb/drvusbmain4.o
COBJS += ./usb/drvhostlib4.o
COBJS += ./usb/ehci_usb.o
COBJS += ./usb/usb_lan.o
COBJS += ./usb/ax88772a.o
COBJS += ./usb/rtl8150.o
COBJS += ./usb/pegasus.o
endif
endif

ifeq ($(CONFIG_USB_XHCI),y)
COBJS += ./usb30/xhci-mstar.o
COBJS += ./usb30/xhci.o
COBJS += ./usb30/xhci-dbg.o
COBJS += ./usb30/xhci-mem.o
COBJS += ./usb30/xhci-ring.o
COBJS += ./usb30/xhci-lib.o
COBJS += ./usb30/xhci-parse.o
endif

ifeq ($(CONFIG_HDMI_TX),y)
ifeq ($(CONFIG_HDMITX_MSTAR_ROCKET),y)
COBJS +=  ./hdmitx/mstar/rocket/MsDrvRocket.o
endif
ifeq ($(CONFIG_HDMITX_MSTAR_ROCKET2),y)
COBJS +=  ./hdmitx/mstar/rocket2/MsDrvRocket.o
endif


ifeq ($(CONFIG_HDMITX_MSTAR),y)
COBJS +=  ./hdmitx/mstar/MsDrvHdmi.o
else ifeq ($(CONFIG_HDMITX_NXP_TDA19988),y)
# CONFIG_HDMITX_NXP_TDA19988
COBJS +=  ./hdmitx/sde2/inc/M_supply.o
COBJS +=  ./hdmitx/sde2/comps/tmbsltda9989/src/tmbslTDA9989_Edid.o
COBJS +=  ./hdmitx/sde2/comps/tmbsltda9989/src/tmbslTDA9989_HDCP.o
COBJS +=  ./hdmitx/sde2/comps/tmbsltda9989/src/tmbslTDA9989_InOut.o
COBJS +=  ./hdmitx/sde2/comps/tmbsltda9989/src/tmbslTDA9989_local.o
COBJS +=  ./hdmitx/sde2/comps/tmbsltda9989/src/tmbslTDA9989_Misc.o
COBJS +=  ./hdmitx/sde2/comps/tmbsltda9989/src/tmbslTDA9989_State.o
COBJS +=  ./hdmitx/sde2/comps/tmdlhdmicec/src/tmdlHdmiCEC.o
COBJS +=  ./hdmitx/sde2/comps/tmdlhdmicec/src/tmdlHdmiCEC_local.o
COBJS +=  ./hdmitx/sde2/comps/tmdlhdmitx/cfg/TDA9989/tmdlHdmiTx_cfg.o
COBJS +=  ./hdmitx/sde2/comps/tmdlhdmitx/src/tmdlHdmiTx.o
COBJS +=  ./hdmitx/sde2/comps/tmdlhdmitx/src/tmdlHdmiTx_local.o
COBJS +=  ./hdmitx/sde2/comps/tmdlhdmitx/tst/tmdlHdmiTx_ExampleApplication/src/tmdlHdmiTx_ExampleApplicationArm7_NO_RTOS.o
COBJS +=  ./hdmitx/sde2/comps/tmdlhdmitx/tst/tmdlHdmiTx_ExampleApplication/src/tmdlHdmiTx_IW_RTX.o
endif
endif
COBJS += ./MDrv_Sys.o
COBJS += ./MsDrvCache.o

ifeq ($(CONFIG_CVBS_OUT),y)
COBJS +=  ./ve/MsDrvVE.o
endif

ifeq ($(CONFIG_XC_Init),y)
COBJS +=  ./xc/MsDrvXC.o
endif

ifeq ($(CONFIG_GMAC),y)
CFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/gmac/
CPPFLAGS += -I../hal/$(CONFIG_MSTAR_CHIP_NAME)/gmac/
endif

ifeq ($(CONFIG_UART_RX),y)
COBJS += ./uart/MsDrvUART.o
endif

CFLAGS += -I./iic/
CPPFLAGS += -I./iic/
all:	$(LIB) $(AOBJS)

SRCS	:= $(AOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(AOBJS) $(COBJS))

all:	$(LIB)

$(LIB): $(obj).depend $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
