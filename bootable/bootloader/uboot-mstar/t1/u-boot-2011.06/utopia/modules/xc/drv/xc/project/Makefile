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

ROOT      = .
TRUNK     = .
PRJ      ?= t2_nos
TARGET   ?= s4le_nos
ADD      ?=
PROJECT_DIR = $(TRUNK)/../../../../project/$(PRJ)
LIB_DIR_  = $(PROJECT_DIR)/bsp/lib
INC_DIR_  = $(PROJECT_DIR)/bsp/include
TARGET_ROOT ?= Chakra2

OUT_TRUNK = $(TRUNK)/../../../../../../DAILEO/Yoga/$(TARGET_ROOT)
OUT_LIB_DIR ?= $(OUT_TRUNK)/core/drv_bsp/$(TARGET)/lib
OUT_INC_DIR ?= $(OUT_TRUNK)/core/drv_bsp/$(TARGET)/include

include install.mk

all: wble cec pnl ace dlc xc ddc2bi ve 
wble cec pnl ace dlc xc ddc2bi ve hdmitx dac mfc: init

ifneq ($(LINK_TYPE),dynamic)
	@make -C $(PROJECT_DIR) --no-print-directory MAKE_TYPE=lint $@ > /dev/null
	@make -C $(PROJECT_DIR) --no-print-directory $@ lint
else
	@make -C $(PROJECT_DIR) --no-print-directory $@
endif

install: init bsp
	@for i in $(INSTALL_INC_FILES); do	\
	    ./diff_cp.sh $(INC_DIR_)/$$i $(OUT_INC_DIR)/$$i;	\
	done;
	@for i in $(INSTALL_LIB_FILES); do	\
	    ./diff_cp.sh $(LIB_DIR_)/$$i $(OUT_LIB_DIR)/$$i;	\
	done;

init:
	@rm -rfd $(PROJECT_DIR)/bsp
	@make -C $(PROJECT_DIR) --no-print-directory clean

clean:
	@make -C $(PROJECT_DIR) --no-print-directory $@
	@rm -rfd $(PROJECT_DIR)/bsp
	@rm -f $(PROJECT_DIR)/lint_test.log

bsp:
	@make -C $(PROJECT_DIR) --no-print-directory $@ > /dev/null
