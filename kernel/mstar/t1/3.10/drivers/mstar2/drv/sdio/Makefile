#
# Makefile for mstar SD card device drivers using SDIO IP.
#
CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
ifdef CONFIG_MSTAR_PROJECT_NAME
    CONFIG_MSTAR_PROJECT_NAME := $(subst ",,$(CONFIG_MSTAR_PROJECT_NAME))
endif


# general options
EXTRA_CFLAGS    += -I$(srctree)/drivers/mstar2/include
EXTRA_CFLAGS    += -I$(srctree)/drivers/mstar2/drv/sdio
EXTRA_CFLAGS    += -I$(srctree)/drivers/mstar2/drv/sdio/hal/$(CONFIG_MSTAR_CHIP_NAME)

# specific options
EXTRA_CFLAGS    += -DMSOS_TYPE_LINUX

obj-y           += mmc_host_sdio.o
obj-y           += hal/$(CONFIG_MSTAR_CHIP_NAME)/
