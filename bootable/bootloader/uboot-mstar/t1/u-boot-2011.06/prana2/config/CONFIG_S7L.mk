CC_TVOPTS += -DCHIP_FAMILY_TYPE=CHIP_FAMILY_S7L

PRANA_SRC += \
	$(ROOT)/driver/core/device.c                    \
	$(ROOT)/driver/riu/drvGlobal.c

INC_DIR   += \
	-I$(ROOT)/driver

