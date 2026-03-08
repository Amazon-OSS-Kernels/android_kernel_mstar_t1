#//========================================================
#// Makefile for MStar DVFS Mechanism
#//========================================================

CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
ifdef CONFIG_MSTAR_PROJECT_NAME
CONFIG_MSTAR_PROJECT_NAME := $(subst ",,$(CONFIG_MSTAR_PROJECT_NAME))
endif

EXTRA_CFLAGS += -I$(srctree)/drivers/mstar2/include/
EXTRA_CFLAGS += -I$(srctree)/drivers/mstar2/drv/dvfs/include/
EXTRA_CFLAGS += -I$(srctree)/drivers/mstar2/drv/dvfs/hal/$(CONFIG_MSTAR_CHIP_NAME)/
#EXTRA_CFLAGS += -Idrivers/mstar/dvfs/hal/$(CONFIG_MSTAR_CHIP_NAME)/include/
EXTRA_CFLAGS += -I$(srctree)/include/linux/

obj-$(CONFIG_MSTAR_DVFS) := mdrv_dvfs.o
obj-$(CONFIG_MSTAR_DVFS) += hal/$(CONFIG_MSTAR_CHIP_NAME)/
ifdef CONFIG_MSTAR_DVFS_KERNEL_IIC
obj-$(CONFIG_MSTAR_IIC) += hal/$(CONFIG_MSTAR_CHIP_NAME)/
endif
