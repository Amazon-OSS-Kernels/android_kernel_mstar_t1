#
# (C) COPYRIGHT 2014-2016 MStar Semiconductor, Inc. All rights reserved.
#
# This program is free software and is provided to you under the terms of the
# GNU General Public License version 2 as published by the Free Software
# Foundation, and any use by you of this program is subject to the terms
# of such GNU licence.
#
# A copy of the licence is included with the program, and can also be obtained
# from Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA  02110-1301, USA.
#
#

# Note: the config and flags should synchronize to:
# -bldsys/sconstruct
# -bldsys/profile/*.py
# -base/sconscript

# project profile
PROFILE_DIR = drivers/mstar2/gpu/mali-midgard/r14p0-01rel0/bldsys/profiles
PROFILE_NAME = $(shell echo $(CONFIG_MSTAR_CHIP_NAME))-$(shell echo $(CONFIG_MALI_PROJECT_PLATFORM))
include $(PROFILE_DIR)/$(PROFILE_NAME).mak

# flags
MSTAR_CFLAGS =
ifneq ($(os),android)
MSTAR_CFLAGS += -DCONFIG_KDS
endif
ifneq ($(CONFIG_UMP),)
MSTAR_CFLAGS += -DCONFIG_UMP
endif

# MStar-specific config
MSTAR_PLATFORM_NAME = $(shell echo $(mstar_platform))
MSTAR_USE_FIXED_DEVID ?= 1
MSTAR_UMM_EXPORT ?= 1
MSTAR_SKIP_JOBS ?= 1

ifeq ($(os),android)
MSTAR_USE_FIXED_DEVID = 0
endif

# MStar-specific flags
MSTAR_CFLAGS += -DMSTAR
MSTAR_CFLAGS += -D$(strip $(shell echo $(mstar_platform) | tr a-z A-Z))
MSTAR_CFLAGS += -D$(strip $(shell echo $(project) | tr a-z A-Z))
MSTAR_CFLAGS += -DMALI_MAX_FREQ=$(shell echo $(mali_max_freq))
MSTAR_CFLAGS += -DMALI_MIN_FREQ=$(shell echo $(mali_min_freq))
MSTAR_CFLAGS += -DMALI_BOOST_FREQ=$(shell echo $(mali_boost_freq))
MSTAR_CFLAGS += -DMALI_IRQ=$(shell echo $(mali_irq))
MSTAR_CFLAGS += -DMALI_REG_OFFSET=$(shell echo $(mali_reg_offset))
MSTAR_CFLAGS += -DMSTAR_RIU_ENABLED
MSTAR_CFLAGS += -DMSTAR_PM_CALLBACKS
MSTAR_CFLAGS += -DMSTAR_UMM_EXPORT=$(MSTAR_UMM_EXPORT)
MSTAR_CFLAGS += -DMSTAR_DISABLE_CONFIG_OF
MSTAR_CFLAGS += -DMSTAR_DISABLE_CONFIG_NEED_SG_DMA_LENGTH
MSTAR_CFLAGS += -DMSTAR_GPU_UTILIZATION
MSTAR_CFLAGS += -DMSTAR_MEMORY_USAGE
MSTAR_CFLAGS += -DMSTAR_DISABLE_SHADER_CORES
MSTAR_CFLAGS += -DMSTAR_AVOID_CTX_STARVATION_IN_JS_R14
MSTAR_CFLAGS += -DCONFIG_MALI_MIDGARD_DVFS_FREQ_ADJUSTABLE=$(shell echo $(mali_dvfs_freq_adjustable))
MSTAR_CFLAGS += -DMSTAR_SKIP_JOBS=$(MSTAR_SKIP_JOBS)
MSTAR_CFLAGS += -Idrivers/mstar2/gpu/mali-midgard/r14p0-01rel0/include
ifeq ($(MSTAR_USE_FIXED_DEVID),1)
MSTAR_CFLAGS += -DMSTAR_USE_FIXED_DEVID=$(MSTAR_USE_FIXED_DEVID)
endif
