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
# Chips that default using VDEC 2.0
VDEC_EX_CHIPS = j2 a1 a7 a5 a3 amethyst agate eagle edison emerald kaiser nugget einstein einstein3 monaco muji monet manhattan
ifeq ($(CHIP), $(filter $(CHIP),$(VDEC_EX_CHIPS)))
API_NAME = VDEC_V1
else
API_NAME = VDEC
endif


# **********************************************
# DRV Source Files
# **********************************************

VDEC_MVD_ONLY =

ifeq ("$(CHIP)", "$(VDEC_MVD_ONLY)")
DRV_DIR = mvd
else
DRV_DIR = mvd hvd mjpeg
endif

ifneq ($(CHIP), $(filter $(CHIP),$(VDEC_EX_CHIPS)))
all: update_hdr
update_hdr:
	echo " Copy apiVDEC_EX.h to vdec folder "
	cp $(PUBINC)/apiVDEC_EX.h ./
else
all: remove_hdr
remove_hdr:
	if [ -f ../vdec/apiVDEC_EX.h ] ; then \
	rm $(PUBINC)/apiVDEC_EX.h ; \
	echo "*** Remove apiVDEC_EX.h from vdec folder ***"; \
	fi
endif

# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/api_rule.mk

