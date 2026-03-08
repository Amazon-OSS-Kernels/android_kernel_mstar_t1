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

ROOT      = .
TRUNK     = .
PRJ      ?= t3_nos
TARGET   ?= t3_nos

ADD      ?=
PROJECT_DIR = $(TRUNK)/../../../../project/$(PRJ)
LIB_DIR_  = $(PROJECT_DIR)/bsp/lib
INC_DIR_  = $(PROJECT_DIR)/bsp/include


OUT_TRUNK = $(TRUNK)/../../../../../SxLib/XDemux/MediaCodec_T3/interface/kernel/drv_bsp
OUT_LIB_DIR ?= $(OUT_TRUNK)/$(TARGET)/lib
OUT_INC_DIR ?= $(OUT_TRUNK)/$(TARGET)/include


include install.mk

ifeq ($(PRJ),u3_nos)
all: vdec
vdec: init
else
all: vdec $(ADD)
vdec $(ADD): init
endif

ifneq ($(LINK_TYPE),dynamic)
	@make -C $(PROJECT_DIR) --no-print-directory MAKE_TYPE=lint $@ > /dev/null
	@make -C $(PROJECT_DIR) --no-print-directory $@ lint
else
	@make -C $(PROJECT_DIR) --no-print-directory $@
endif

install: init bsp
	@for i in $(INSTALL_INC_FILES); do	\
	    if [ $(INC_DIR_)/$$i -nt $(OUT_INC_DIR)/$$i ]; \
		then ((cmp $(INC_DIR_)/$$i $(OUT_INC_DIR)/$$i) || (cp -f $(INC_DIR_)/$$i $(OUT_INC_DIR);)) \
	    fi 						\
	done;
	@for i in $(INSTALL_LIB_FILES); do	\
	    if [ $(LIB_DIR_)/$$i -nt $(OUT_LIB_DIR)/$$i ]; \
		then cp -f $(LIB_DIR_)/$$i $(OUT_LIB_DIR); \
	    fi 						\
	done;


init:
	@rm -rfd $(PROJECT_DIR)/bsp

clean:
	@make -C $(PROJECT_DIR) --no-print-directory $@
	@rm -rfd $(PROJECT_DIR)/bsp
	@rm -f $(PROJECT_DIR)/lint_test.log

bsp:
	@make -C $(PROJECT_DIR) --no-print-directory $@
