CC_TVOPTS += -DCHIP_FAMILY_TYPE=CHIP_FAMILY_NAPOLI
//CC_TVOPTS += -DMBOOT

PRANA_SRC += \
	$(ROOT)/driver/core/device.c                    \
	$(ROOT)/driver/riu/drvGlobal.c

INC_DIR   += \
	-I$(ROOT)/driver

