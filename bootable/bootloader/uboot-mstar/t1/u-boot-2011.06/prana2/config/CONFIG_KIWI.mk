CC_TVOPTS += -msoft-float -DCHIP_FAMILY_TYPE=CHIP_FAMILY_KIWI

PRANA_SRC += \
	$(ROOT)/driver/core/device.c                    \
	$(ROOT)/driver/riu/drvGlobal.c

INC_DIR   += \
	-I$(ROOT)/driver

