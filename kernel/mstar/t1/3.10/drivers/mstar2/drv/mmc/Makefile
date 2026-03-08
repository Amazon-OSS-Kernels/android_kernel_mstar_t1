#
# Makefile for MStar eMMC host drivers.
#
CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))

# general options
EXTRA_CFLAGS    += -I$(srctree)/drivers/mstar2/drv/mmc/inc/api/
EXTRA_CFLAGS    += -I$(srctree)/drivers/mstar2/drv/mmc/inc/common/
EXTRA_CFLAGS    += -I$(srctree)/drivers/mstar2/drv/mmc/inc/config/
EXTRA_CFLAGS    += -I$(srctree)/drivers/mstar2/drv/mmc/hal/$(CONFIG_MSTAR_CHIP_NAME)/
EXTRA_LDFLAGS   += -T$(srctree)/drivers/mstar2/drv/mmc/mmc.lds
ifndef CONFIG_ARM_LPAE
EXTRA_CFLAGS    += -Werror
endif

# specific options


# files
obj-y           += src/api/
obj-y           += src/common/

ifeq ($(CONFIG_MSTAR_CHIP_NAME),$(filter $(CONFIG_MSTAR_CHIP_NAME),einstein kaiser madison napoli))
obj-y           += src/common/eMMC_hal.o
obj-y           += mstar_mci.o
else
obj-y           += src/common/eMMC_hal_v5.o
obj-y           += mstar_mci_v5.o
endif

obj-y           += src/common/eMMC_ip_verify.o
obj-y           += src/common/eMMC_utl.o
obj-y           += hal/$(CONFIG_MSTAR_CHIP_NAME)/

