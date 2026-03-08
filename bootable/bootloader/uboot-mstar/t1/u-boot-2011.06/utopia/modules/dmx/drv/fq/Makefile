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
DRV_NAME = FQ


# **********************************************
# HAL Source Files
# **********************************************
HAL_DIR = \
            fq                                                     \

TSP4_CHIPS = kano kastor
ifeq ($(CHIP), $(filter $(CHIP),$(TSP4_CHIPS)))
HAL_DIR += tsp
endif

# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/drv_rule.mk

# **********************************************
# Customization
# **********************************************
ifeq ($(CHIP), $(filter $(CHIP),$(TSP4_CHIPS)))
CC_DEFS += -DFQ_HW_BOX_SERIES
endif

