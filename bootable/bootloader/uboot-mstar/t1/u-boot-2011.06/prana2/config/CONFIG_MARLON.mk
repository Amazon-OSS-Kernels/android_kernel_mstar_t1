CC_TVOPTS += -DCHIP_FAMILY_TYPE=CHIP_FAMILY_MARLON

# PM ?= 0, 1
PM ?= 0

ifeq ($(PM),1)
CC_TVOPTS += -DCONFIG_PM_ENABLED
endif

PRANA_SRC += \
	$(ROOT)/driver/core/device.c                    \
	$(ROOT)/driver/riu/drvGlobal.c

INC_DIR   += \
	-I$(ROOT)/driver
