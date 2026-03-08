#
# Makefile for MStar PM HAL drivers.
#

# general options
ccflags-y      +=  -I$(srctree)/drivers/mstar2/include
ccflags-y      +=  -I$(srctree)/drivers/mstar2/drv/pm
ccflags-y      +=  -I$(srctree)/drivers/mstar2/drv/pm/hal/$(CONFIG_MSTAR_CHIP_NAME)
ccflags-y      +=  -I$(objtree)/include/linux

# specific options
ccflags-y      +=  -DRED_LION
# files

# files
obj-y             += mhal_pm.o
