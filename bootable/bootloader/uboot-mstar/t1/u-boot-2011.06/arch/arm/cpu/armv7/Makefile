#
# (C) Copyright 2000-2003
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

LIB	= $(obj)lib$(CPU).o
LIBS_OUT = $(OUT)/obj/MBOOT_OBJ/lib/mstar
START	:= start.o
ifdef CONFIG_MSTAR_STR_MINISIZE
START += start_str.o
endif
COBJS	:= cpu.o
COBJS  += syslib.o
SOBJS  := cache.o

ifneq ($(CONFIG_ARMV7_NONSEC)$(CONFIG_ARMV7_VIRT),)
	SOBJS    += nonsec_virt.o
	COBJS    += virt-v7.o
endif

SRCS	:= $(START:.o=.S) $(COBJS:.o=.c) $(SOBJS:.o=.S)
OBJS	:= $(addprefix $(obj),$(COBJS))
OBJS	+= $(addprefix $(obj),$(SOBJS))
START	:= $(addprefix $(obj),$(START))

all:	$(obj).depend PREPARE_PRANA2 $(START) $(LIB)

PREPARE_PRANA2:
ifeq ($(CONFIG_MSTAR_EAGLE),y)
	cp $(LIBS_OUT)/libprana_eagle.a $(LIBS_OUT)/temp_libprana_eagle.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/temp_libprana_eagle.a vectors.o
	ar d $(LIBS_OUT)/temp_libprana_eagle.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/temp_libprana_eagle.a head.o
endif
ifeq ($(CONFIG_MSTAR_EIFFEL),y)
	cp $(LIBS_OUT)/libprana_eiffel.a $(LIBS_OUT)/temp_libprana_eiffel.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/temp_libprana_eiffel.a vectors.o
	ar d $(LIBS_OUT)/temp_libprana_eiffel.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/temp_libprana_eiffel.a head.o
endif
ifeq ($(CONFIG_MSTAR_NIKE),y)
	cp $(LIBS_OUT)/libprana_nike.a $(LIBS_OUT)/temp_libprana_nike.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/temp_libprana_nike.a vectors.o
	ar d $(LIBS_OUT)/temp_libprana_nike.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/temp_libprana_nike.a head.o
endif

ifeq ($(CONFIG_MSTAR_MADISON),y)
	cp $(LIBS_OUT)/libprana_madison.a $(LIBS_OUT)/temp_libprana_madison.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/temp_libprana_madison.a vectors.o
	ar d $(LIBS_OUT)/temp_libprana_madison.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/temp_libprana_madison.a head.o
endif

ifeq ($(CONFIG_MSTAR_CLIPPERS),y)
	cp $(LIBS_OUT)/libprana_clippers.a $(LIBS_OUT)/temp_libprana_clippers.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/temp_libprana_clippers.a vectors.o
	ar d $(LIBS_OUT)/temp_libprana_clippers.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/temp_libprana_clippers.a head.o
endif

ifeq ($(CONFIG_MSTAR_MIAMI),y)
	cp $(LIBS_OUT)/libprana_miami.a $(LIBS_OUT)/temp_libprana_miami.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/temp_libprana_miami.a vectors.o
	ar d $(LIBS_OUT)/temp_libprana_miami.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/temp_libprana_miami.a head.o
endif

ifeq ($(CONFIG_MSTAR_NADAL),y)
	cp $(LIBS_OUT)/libprana_nadal.a $(LIBS_OUT)/temp_libprana_nadal.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/temp_libprana_nadal.a vectors.o
	ar d $(LIBS_OUT)/temp_libprana_nadal.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/temp_libprana_nadal.a head.o
endif

ifeq ($(CONFIG_MSTAR_AGATE),y)
	cp $(LIBS_OUT)/libprana_agate.a $(LIBS_OUT)/templibprana_agate.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_agate.a vectors.o
	ar d $(LIBS_OUT)/templibprana_agate.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_agate.a head.o
endif
ifeq ($(CONFIG_MSTAR_EINSTEIN),y)
	cp $(LIBS_OUT)/libprana_einstein.a $(LIBS_OUT)/templibprana_einstein.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_einstein.a vectors.o
	ar d $(LIBS_OUT)/templibprana_einstein.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_einstein.a head.o
endif
ifeq ($(CONFIG_MSTAR_AMBER3),y)
	cp $(LIBS_OUT)/libprana_a3.a $(LIBS_OUT)/templibprana_a3.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_a3.a vectors.o
	ar d $(LIBS_OUT)/templibprana_a3.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_a3.a head.o
endif
ifeq ($(CONFIG_MSTAR_EDISON),y)
	cp $(LIBS_OUT)/libprana_edison.a $(LIBS_OUT)/templibprana_edison.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_edison.a vectors.o
	ar d $(LIBS_OUT)/templibprana_edison.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_edison.a head.o
endif
ifeq ($(CONFIG_MSTAR_KAISER),y)
	cp $(LIBS_OUT)/libprana_kaiser.a $(LIBS_OUT)/templibprana_kaiser.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_kaiser.a vectors.o
	ar d $(LIBS_OUT)/templibprana_kaiser.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_kaiser.a head.o
endif
ifeq ($(CONFIG_MSTAR_KANO),y)
	cp $(LIBS_OUT)/libprana_kano.a $(LIBS_OUT)/templibprana_kano.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_kano.a vectors.o
	ar d $(LIBS_OUT)/templibprana_kano.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_kano.a head.o
endif
ifeq ($(CONFIG_MSTAR_CURRY),y)
	cp $(LIBS_OUT)/libprana_curry.a $(LIBS_OUT)/templibprana_curry.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_curry.a vectors.o
	ar d $(LIBS_OUT)/templibprana_curry.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_curry.a head.o
endif
ifeq ($(CONFIG_MSTAR_KASTOR),y)
	cp $(LIBS_OUT)/libprana_kastor.a $(LIBS_OUT)/templibprana_kastor.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_kastor.a vectors.o
	ar d $(LIBS_OUT)/templibprana_kastor.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_kastor.a head.o
endif
ifeq ($(CONFIG_MSTAR_EINSTEIN3),y)
	cp $(LIBS_OUT)/libprana_einstein3.a $(LIBS_OUT)/templibprana_einstein3.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_einstein3.a vectors.o
	ar d $(LIBS_OUT)/templibprana_einstein3.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_einstein3.a head.o
endif
ifeq ($(CONFIG_MSTAR_NAPOLI),y)
	cp $(LIBS_OUT)/libprana_napoli.a $(LIBS_OUT)/templibprana_napoli.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_napoli.a vectors.o
	ar d $(LIBS_OUT)/templibprana_napoli.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_napoli.a head.o
endif
ifeq ($(CONFIG_MSTAR_MONACO),y)
	cp $(LIBS_OUT)/libprana_monaco.a $(LIBS_OUT)/templibprana_monaco.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_monaco.a vectors.o
	ar d $(LIBS_OUT)/templibprana_monaco.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_monaco.a head.o
endif
ifeq ($(CONFIG_MSTAR_MONET),y)
	cp $(LIBS_OUT)/libprana_monet.a $(LIBS_OUT)/templibprana_monet.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_monet.a vectors.o
	ar d $(LIBS_OUT)/templibprana_monet.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_monet.a head.o
	rm $(LIBS_OUT)/templibprana_monet.a
endif
ifeq ($(CONFIG_MSTAR_MESSI),y)
	cp $(LIBS_OUT)/libprana_messi.a $(LIBS_OUT)/templibprana_messi.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_messi.a vectors.o
	ar d $(LIBS_OUT)/templibprana_messi.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_messi.a head.o
	rm $(LIBS_OUT)/templibprana_messi.a
endif
ifeq ($(CONFIG_MSTAR_MUJI),y)
	cp $(LIBS_OUT)/libprana_muji.a $(LIBS_OUT)/templibprana_muji.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_muji.a vectors.o
	ar d $(LIBS_OUT)/templibprana_muji.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_muji.a head.o
endif

ifeq ($(CONFIG_MSTAR_MANHATTAN),y)
	cp $(LIBS_OUT)/libprana_manhattan.a $(LIBS_OUT)/templibprana_manhattan.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_manhattan.a vectors.o
	ar d $(LIBS_OUT)/templibprana_manhattan.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_manhattan.a head.o
endif

ifeq ($(CONFIG_MSTAR_MASERATI),y)
	@echo "[Jawa] cp libprana_maserati.a"
	cp $(LIBS_OUT)/libprana_maserati.a $(LIBS_OUT)/templibprana_maserati.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_maserati.a vectors.o
	ar d $(LIBS_OUT)/templibprana_maserati.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_maserati.a head.o
	rm $(LIBS_OUT)/templibprana_maserati.a vectors.o
endif

ifeq ($(CONFIG_MSTAR_MAXIM),y)
	cp $(LIBS_OUT)/libprana_maxim.a $(LIBS_OUT)/templibprana_maxim.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_maxim.a vectors.o
	ar d $(LIBS_OUT)/templibprana_maxim.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_maxim.a head.o
endif

ifeq ($(CONFIG_MSTAR_MUNICH),y)
	cp $(LIBS_OUT)/libprana_munich.a $(LIBS_OUT)/templibprana_munich.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_munich.a vectors.o
	ar d $(LIBS_OUT)/templibprana_munich.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_munich.a head.o
endif

ifeq ($(CONFIG_MSTAR_MACAN),y)
	cp $(LIBS_OUT)/libprana_macan.a $(LIBS_OUT)/templibprana_macan.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_macan.a vectors.o
	ar d $(LIBS_OUT)/templibprana_macan.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_macan.a head.o
endif

ifeq ($(CONFIG_MSTAR_CELTICS),y)
	cp $(LIBS_OUT)/libprana_celtics.a $(LIBS_OUT)/templibprana_celtics.a
	@echo "Extracting vectors.o..."
	ar x $(LIBS_OUT)/templibprana_celtics.a vectors.o
	ar d $(LIBS_OUT)/templibprana_celtics.a vectors.o
	@echo "Removing head.o..."
	ar d $(LIBS_OUT)/templibprana_celtics.a head.o
endif
$(LIB):	$(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
