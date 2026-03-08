#
# Makefile for MStar GFLIP device drivers.
#
MODULE_NAME := mstar_fb
CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))
ifdef CONFIG_MSTAR_PROJECT_NAME
    CONFIG_MSTAR_PROJECT_NAME := $(subst ",,$(CONFIG_MSTAR_PROJECT_NAME))
endif


# general options
EXTRA_CFLAGS      +=  -I$(srctree)/drivers/mstar2/include
EXTRA_CFLAGS      +=  -I$(srctree)/drivers/mstar2/drv/graphic
EXTRA_CFLAGS      +=  -I$(srctree)/drivers/mstar2/drv/graphic/hal/$(CONFIG_MSTAR_CHIP_NAME)

# files

obj-$(CONFIG_MSTAR_GRAPHIC) += mstar_fb.o

mstar_fb-objs += mdrv_graphic_io.o \
		mdrv_graphic.o \
		hal/$(CONFIG_MSTAR_CHIP_NAME)/mhal_graphic.o

