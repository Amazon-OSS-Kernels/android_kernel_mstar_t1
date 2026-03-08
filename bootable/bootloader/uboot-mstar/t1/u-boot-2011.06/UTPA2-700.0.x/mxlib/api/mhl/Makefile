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

# **********************************************
# DRV Name
# **********************************************
API_NAME = MHL

MSTAR_MHL_CHIP = y
EXTERNAL_MHL_CHIP = n

# **********************************************
# DRV Source Files
# **********************************************
# tsp2 is prior than rasp

DRV_DIR = \
            mhl                                                    \


# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/api_rule.mk

API_INCDIR += $(ROOTLIB)/drv/mhl

ifeq ($(EXTERNAL_MHL_CHIP),y)
	CC_OPTS += -DEXTERNAL_MHL
	ifeq ($(MSTAR_MHL_CHIP),y)
		CC_OPTS += -DMSTAR_MHL
		API_INCDIR += $(DDIHAL)/$(CHIP)/mhl/external/mstar
	else
		API_INCDIR += $(DDIHAL)/$(CHIP)/mhl/external/silicon
	endif
else
	API_INCDIR += $(DDIHAL)/$(CHIP)/mhl/internal
endif