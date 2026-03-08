#
# Makefile for MStar GFLIP HAL drivers.
#

# general options
#EXTRA_CFLAGS      +=  -Idrivers/mstar/common
EXTRA_CFLAGS      +=  -I$(srctree)/drivers/mstar2/include/
EXTRA_CFLAGS      +=  -I$(srctree)/drivers/mstar2/gflip/drv/
EXTRA_CFLAGS      +=  -I$(srctree)/drivers/mstar2/gflip/drv/$(CONFIG_MSTAR_CHIP_NAME)/

EXTRA_CFLAGS      +=  -DRED_LION

# specific options
EXTRA_CFLAGS      +=

# files
obj-y             += mhal_gflip.o 
