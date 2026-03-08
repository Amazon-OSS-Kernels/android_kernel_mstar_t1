#
# Makefile for MStar EMAC device drivers.
#
CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
ifdef CONFIG_MSTAR_PROJECT_NAME
    CONFIG_MSTAR_PROJECT_NAME := $(subst ",,$(CONFIG_MSTAR_PROJECT_NAME))
endif

# general options
ccflags-y      +=  -Idrivers/mstar2/include
ccflags-y      +=  -Idrivers/mstar2/drv/pm
ccflags-y      +=  -Idrivers/mstar2/hal/$(CONFIG_MSTAR_CHIP_NAME)/pm
ccflags-y      +=  -Iinclude/linux
ccflags-y      +=  -I$(srctree)/drivers/mstar2/include
ccflags-y      +=  -I$(srctree)/drivers/mstar2/drv/pm
ccflags-y      +=  -I$(srctree)/drivers/mstar2/drv/pm/hal/$(CONFIG_MSTAR_CHIP_NAME)
ccflags-y      +=  -I$(srctree)/include/linux

# specific options
ccflags-y      +=  -DRED_LION

# files
obj-y   := mdrv_pm.o
obj-y   += mdrv_pm_io.o
obj-y   += hal/$(CONFIG_MSTAR_CHIP_NAME)/
