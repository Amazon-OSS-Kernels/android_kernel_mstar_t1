EXTRA_CFLAGS    += -I$(srctree)/drivers/mstar2/include/
EXTRA_CFLAGS    += -I$(srctree)/drivers/mstar2/drv/nand/inc/api/
EXTRA_CFLAGS    += -I$(srctree)/drivers/mstar2/drv/nand/inc/common/
EXTRA_CFLAGS    += -I$(srctree)/drivers/mstar2/drv/nand/inc/config/
EXTRA_CFLAGS    += -I$(srctree)/drivers/mstar2/drv/nand/hal/$(CONFIG_MSTAR_CHIP_NAME)/

obj-y           += drvNAND_utl.o
obj-y           += drvNAND_hal.o
obj-y           += drvNAND_ReadRetry.o
obj-y           += drvNAND_hal_v5.o
obj-y           += drvNAND_ReadRetry_v5.o
obj-y           += drvNAND_ftl.o
obj-y           += drvNAND_task.o
