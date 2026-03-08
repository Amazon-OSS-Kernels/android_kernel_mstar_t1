################################################################################
#
# Copyright (c) 2006 - 2018 MStar Semiconductor, Inc.
# This program is free software. You can redistribute it and/or modify it under the terms of
# the GNU General Public License as published by the Free Software Foundation;
# either version 2 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with this program;
# if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
# MA 02111-1307, USA.
#
################################################################################
#

# **********************************************
# DRV Name
# **********************************************

# Chips that default using VDEC 2.0
VDEC_EX_CHIPS = j2 a1 a7 a5 a3 amethyst agate eagle edison emerald kaiser eiffel nike madison miami nugget einstein einstein3 napoli monaco clippers muji keres kirin monet manhattan kano kastor curry
ifneq ($(CHIP), $(filter $(CHIP),$(VDEC_EX_CHIPS)))
API_NAME = VDEC_V2
else
API_NAME = VDEC
endif

# **********************************************
# DRV Source Files
# **********************************************
DRV_DIR = \
            hvd_lite                                           \
            mjpeg_lite                                         \
            mvd_lite                                           \

ifeq ($(CHIP), $(filter $(CHIP),$(VDEC_EX_CHIPS)))
all: update_hdr
update_hdr:
	cp $(PUBINC)/apiVDEC.h ./
endif 

# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/api_rule.mk


# **********************************************
# Incremental Build 
# **********************************************
ifeq ($(CONFIG_INCREMENTAL_BUILD),y)
LD_LIBS_INC += -L$(DDI_PATH)  -ldrvIPAUTH -lapiJPEG
endif


# **********************************************
# Clock Resource Management 
# **********************************************
ifeq ($(CONFIG_INCREMENTAL_BUILD),y)
ifeq ($(CONFIG_MSTAR_CLKM),y)
LD_LIBS_INC += -L$(DDI_PATH)  -ldrvCLKM
endif
endif