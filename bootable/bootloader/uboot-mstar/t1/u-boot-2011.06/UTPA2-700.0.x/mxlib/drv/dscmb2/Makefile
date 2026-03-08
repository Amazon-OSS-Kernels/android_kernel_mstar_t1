################################################################################
#
# Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
# This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
# Makefile used for building DDI
#
################################################################################

#
#
#
#
#
#

BOX_CHIPS = u4 j2 k1 k2 keltic
TSP3_CHIPS = agate
TSP4_CHIPS = kaiser keltic keres kirin
# **********************************************
# DRV Name
# **********************************************
DRV_NAME = DSCMB2


# **********************************************
# HAL Source Files
# **********************************************
HAL_DIR = \
            dscmb                                                   \


# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/drv_rule.mk

# TODO : not good for driver alling driver, plan to obsolete
ifeq ($(CHIP), $(filter $(CHIP),$(BOX_CHIPS)))
DRV_INCDIR += $(DDIDRV)/tsp2

else ifeq ($(CHIP), $(filter $(CHIP),$(TSP3_CHIPS)))
DRV_INCDIR += $(DDIDRV)/tsp3

else ifeq ($(CHIP), $(filter $(CHIP),$(TSP4_CHIPS)))
DRV_INCDIR += $(DDIDRV)/tsp4

else
DRV_INCDIR += $(DDIDRV)/tsp
endif
