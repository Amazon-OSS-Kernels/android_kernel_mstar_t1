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
API_NAME = DMX


# **********************************************
# DRV Source Files
# **********************************************
# tsp2 is prior than rasp
BOX_CHIPS = u4 j2 k1 k2 kappa
TSP3_CHIPS = agate eiffel monaco miami clippers muji munich manhattan maserati maxim
TSP4_CHIPS = kaiser keltic keres kirin kano kastor

ifeq ($(CHIP), $(filter $(CHIP),$(BOX_CHIPS)))
DRV_DIR = \
            tsp2                                                    \
            rasp                                                    \

else ifeq ($(CHIP),maria10)
DRV_DIR = \
            minitsp                                                \

else
DRV_DIR = \
            tsp                            \

endif

ifeq ($(CHIP),$(filter $(CHIP),$(TSP3_CHIPS)))
DRV_DIR = \
            tsp3                                                \

endif

ifeq ($(CHIP),$(filter $(CHIP),$(TSP4_CHIPS)))
DRV_DIR = tsp4

ifeq ($(RASP_SUPPORT),y)
DRV_DIR += rasp
endif

ifeq ($(OTV_SUPPORT),y)
DRV_DIR += otv
endif

endif

FQ_CHIPS = kappa keltic keres kirin monaco kano maserati maxim kastor
ifeq ($(CHIP),$(filter $(CHIP),$(FQ_CHIPS)))
DRV_DIR += fq
endif

MMFI_CHIPS = t12 j2 a2 a1 k1 kappa a5 a7 a3 k2 a5p amethyst eagle edison macaw12 emerald eden euler einstein nike madison nugget nikon einstein3 napoli monet messi
ifeq ($(CHIP), $(filter $(CHIP),$(MMFI_CHIPS)))
DRV_DIR += mmfi
endif

MMFI2_CHIPS = agate eiffel kaiser keltic keres kirin monaco kano miami clippers muji munich manhattan maserati maxim kastor
ifeq ($(CHIP), $(filter $(CHIP),$(MMFI2_CHIPS)))
DRV_DIR += mmfi2
endif

TSO_CHIPS = agate einstein nike kaiser keltic einstein3 keres kirin napoli
ifeq ($(CHIP), $(filter $(CHIP),$(TSO_CHIPS)))
DRV_DIR += tso
endif

TSO2_CHIPS = monaco miami munich muji manhattan maserati maxim kano kastor
ifeq ($(CHIP), $(filter $(CHIP),$(TSO2_CHIPS)))
DRV_DIR += tso2
endif

TSIO_CHIPS = kastor
ifeq ($(CHIP), $(filter $(CHIP),$(TSIO_CHIPS)))
DRV_DIR += tsio
endif

# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/api_rule.mk

