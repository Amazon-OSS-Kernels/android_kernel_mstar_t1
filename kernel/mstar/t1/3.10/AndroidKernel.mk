#
# Copyright (C) 2009-2011 The Android-x86 Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#

# This module is primarily intended for building kernels from source for
# non-Amazon devices, e.g. Nexus devices, for the purposes of major rebases
# of FireOS features. Vendors typically provide their own kernel makefiles.
# Platform integrators can adopt this makefile if desired, but it is
# not a FireOS integration requirement.

ifneq ($(KERNEL_DEFCONFIG),)

ROOTDIR := $(abspath $(TOP))
KERNEL_DIR ?= $(ROOTDIR)/$(KERNEL_PATH)
KERNEL_IMAGE_NAME ?= Image
KERNEL_DTB_NAME ?= maxim_an.dtb

KERNEL_OUT := $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ
KERNEL_OUT_ABS := $(abspath $(KERNEL_OUT))
TARGET_PREBUILT_KERNEL := $(KERNEL_OUT)/arch/$(TARGET_KERNEL_ARCH)/boot/$(KERNEL_IMAGE_NAME)
TARGET_DTIMAGE := $(KERNEL_OUT)/arch/$(TARGET_KERNEL_ARCH)/boot/dts/$(KERNEL_DTB_NAME)
KERNEL_DEFCONFIG_FILE := $(KERNEL_DIR)/arch/$(TARGET_KERNEL_ARCH)/configs/$(KERNEL_DEFCONFIG)
TARGET_KERNEL_HAS_MODULE := $(shell grep -q "CONFIG_MODULES=y" $(KERNEL_DEFCONFIG_FILE) && echo true)

TARGET_KERNEL_CONFIG := $(KERNEL_OUT)/.config
KERNEL_HEADERS_INSTALL := $(KERNEL_OUT)/usr
KERNEL_MODULES_OUT := $(TARGET_OUT)/lib/modules

INSTALLED_DTIMAGE_TARGET := $(PRODUCT_OUT)/dtb.bin

ifeq ($(KERNEL_CROSS_COMPILE),)
ifeq ($(TARGET_KERNEL_ARCH),arm)
KERNEL_CROSS_COMPILE := arm-eabi-
else
KERNEL_CROSS_COMPILE := $(abspath $(TARGET_TOOLS_PREFIX))
endif
endif

ifeq ($(TARGET_BUILD_VARIANT), user)
TARGET_KERNEL_STRIP_CMD:="$(KERNEL_CROSS_COMPILE)strip --strip-unneeded"
endif

ifneq ($(USE_CCACHE),)
KERNEL_CROSS_COMPILE := "$(ROOTDIR)/prebuilts/misc/$(CCACHE_HOST_TAG)/ccache/ccache $(KERNEL_CROSS_COMPILE)"
endif

KERNEL_MAKEFLAGS := -C $(KERNEL_DIR) O=$(KERNEL_OUT_ABS) ARCH=$(TARGET_KERNEL_ARCH) CROSS_COMPILE=$(KERNEL_CROSS_COMPILE) CONFIG_DEBUG_SECTION_MISMATCH=y TARGET_KERNEL_STRIP_CMD=$(TARGET_KERNEL_STRIP_CMD)
ifneq ($(strip $(SHOW_COMMANDS)),)
KERNEL_MAKEFLAGS += V=1
endif

define mv-modules
mdpath=`find $(KERNEL_MODULES_OUT) -type f -name modules.dep`;\
if [ "$$mdpath" != "" ];then\
mpath=`dirname $$mdpath`;\
ko=`find $$mpath/kernel -type f -name *.ko`;\
for i in $$ko; do mv $$i $(KERNEL_MODULES_OUT)/; done;\
fi
endef

define clean-module-folder
mdpath=`find $(KERNEL_MODULES_OUT) -type f -name modules.dep`;\
if [ "$$mdpath" != "" ];then\
mpath=`dirname $$mdpath`; rm -rf $$mpath;\
fi
endef

$(KERNEL_OUT):
	$(hide) mkdir -p $@

$(KERNEL_MODULES_OUT):
	$(hide) mkdir -p $@

.PHONY: kernel kernel-defconfig kernel-menuconfig kernel-modules clean-kernel
.PHONY: $(TARGET_PREBUILT_KERNEL) $(KERNEL_HEADERS_INSTALL)


#kernel-menuconfig: | $(KERNEL_OUT)
#	$(hide) $(MAKE) $(KERNEL_MAKEFLAGS) menuconfig

#kernel-savedefconfig: | $(KERNEL_OUT) $(ACP)
#	$(hide) $(MAKE) $(KERNEL_MAKEFLAGS) savedefconfig
#	$(hide) $(ACP) $(KERNEL_OUT)/defconfig $(KERNEL_DIR)/arch/$(TARGET_KERNEL_ARCH)/configs/$(KERNEL_DEFCONFIG)

ifdef COMMON_FEATURE_TRAPZ
TRAPZ_HEADER := $(KERNEL_OUT)/include/generated/trapz_generated_kernel.h
ifneq ($(TARGET_PRODUCT),"aosp_hammerhead")
USE_TRAPZ := true
endif
else
TRAPZ_HEADER :=
endif
$(TARGET_KERNEL_CONFIG) kernel-defconfig: $(KERNEL_OUT) $(TRAPZ_HEADER)
	$(hide) $(MAKE) $(KERNEL_MAKEFLAGS) $(KERNEL_DEFCONFIG)

$(KERNEL_HEADERS_INSTALL): $(TARGET_KERNEL_CONFIG) | $(KERNEL_OUT)
#	-$(hide) $(MAKE) -k $(KERNEL_MAKEFLAGS) headers_install

$(TARGET_PREBUILT_KERNEL): $(TARGET_KERNEL_CONFIG) $(KERNEL_HEADERS_INSTALL) | $(KERNEL_OUT)
	$(hide) $(MAKE) $(KERNEL_MAKEFLAGS) $(KERNEL_EXTRA_BUILD_OPTIONS)
ifeq ($(TARGET_KERNEL_HAS_MODULE),true)
	$(hide) $(MAKE) $(KERNEL_MAKEFLAGS) modules
	$(hide) $(MAKE) $(KERNEL_MAKEFLAGS) INSTALL_MOD_PATH=$(ROOTDIR)/$(KERNEL_MODULES_OUT) modules_install

kernel-modules: $(TARGET_PREBUILT_KERNEL) | $(KERNEL_MODULES_OUT)
	$(mv-modules)
	$(clean-module-folder)

systemimage: kernel-modules

_INSTALLED_RAMDISK_TARGET := $(PRODUCT_OUT)/ramdisk.img
INSTALLED_FB_KO := $(PRODUCT_OUT)/root/recovery_fb/mstar_fb.ko
BUILT_FB_KO := $(PRODUCT_OUT)/system/lib/modules/mstar_fb.ko


$(_INSTALLED_RAMDISK_TARGET): $(INSTALLED_FB_KO)
$(INSTALLED_FB_KO): kernel-modules
	mv $(BUILT_FB_KO) $(INSTALLED_FB_KO)


endif # TARGET_KERNEL_HAS_MODULE

ifneq ($(strip $(TARGET_NO_KERNEL)),true)
INSTALLED_KERNEL_TARGET ?= $(PRODUCT_OUT)/kernel
endif

$(INSTALLED_KERNEL_TARGET): $(TARGET_PREBUILT_KERNEL) | $(ACP)
	$(copy-file-to-target)

$(TARGET_DTIMAGE): $(TARGET_PREBUILT_KERNEL)

$(INSTALLED_DTIMAGE_TARGET): $(TARGET_DTIMAGE) | $(ACP)
	$(copy-file-to-target)

ALL_DEFAULT_INSTALLED_MODULES += $(INSTALLED_DTIMAGE_TARGET)
ALL_MODULES.$(LOCAL_MODULE).INSTALLED += $(INSTALLED_DTIMAGE_TARGET)

droidcore: $(INSTALLED_DTIMAGE_TARGET)

clean-kernel:
	$(hide) rm -rf $(KERNEL_OUT)
	$(hide) rm -rf $(KERNEL_MODULES_OUT)

endif # KERNEL_DEFCONFIG
