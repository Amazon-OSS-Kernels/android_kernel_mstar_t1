EXTRA_CFLAGS += -I$(srctree)/drivers/mstar2/include
EXTRA_CFLAGS += -I$(srctree)/drivers/mstar2/drv/dvfs/include

obj-$(CONFIG_MSTAR_DVFS) += mhal_dvfs.o
ifdef CONFIG_MSTAR_DVFS_KERNEL_IIC
obj-$(CONFIG_MSTAR_IIC) += mhal_dvfs_power.o
endif
