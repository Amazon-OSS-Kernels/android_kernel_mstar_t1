CC_TVOPTS += -DCHIP_FAMILY_TYPE=CHIP_FAMILY_A1

ifeq ($(OS),NOS)
	PRANA_SRC += \
		$(ROOT)/driver/core/device.c                    \
		$(ROOT)/driver/riu/drvGlobal.c
else
	PRANA_SRC += \
		$(ROOT)/driver/riu/drvGlobal.c
endif

INC_DIR   += \
	-I$(ROOT)/driver

