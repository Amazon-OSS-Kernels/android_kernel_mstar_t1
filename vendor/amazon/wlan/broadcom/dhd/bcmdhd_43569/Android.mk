# Copyright Statement:
#

LOCAL_PATH:=$(call my-dir)
LOCAL_PATH_FULL := $(shell pwd)
LOCAL_PATH_BCMDHD := $(call my-dir)

FULL_PATH_1 := $(LOCAL_PATH_FULL)/$(LOCAL_PATH_BCMDHD)

ifeq ($(TARGET_PRODUCT),$(filter $(TARGET_PRODUCT), full_abc123 full_abc123_ble_remote))
LOCAL_DIR:=$(call my-dir)
BUILD_DIR:=$(LOCAL_DIR)/build
TARGETDIR=$(KERNEL_MODULES_OUT)

###################################################################
# setting the path for cross compiler
###################################################################
LINUXVER:=3.10
KERNELDIR:=kernel/mediatek/mt8127d/3.10
CROSSTOOL:=prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/
LINUXDIR:=out/target/product/abc123/obj/KERNEL_OBJ
# PATH:=${CROSSTOOL}:${LINUXDIR}:$PATH
CC:=arm-eabi-gcc
STRIP:=arm-eabi-strip
CROSS_COMPILE:=arm-eabi-

#These are for compiling under FOS
# export TARGET_ARCH=arm
# export CFLAGS=-w
#export LINUX_PORT=1

.PHONY: bcmdhd_module
bcmdhd_module: $(INSTALLED_KERNEL_TARGET) | $(KERNEL_OUT) $(KERNEL_MODULES_OUT) $(ACP)
	$(hide) rm -rf $(ANDROID_PRODUCT_OUT)/system/lib/modules/bcmdhd.ko
	$(MAKE) -C $(KERNEL_OUT) M=$(FULL_PATH_1) ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(KERNEL_CROSS_COMPILE) modules
	$(hide) cp -f $(FULL_PATH_1)/bcmdhd.ko $(KERNEL_MODULES_OUT)
	$(MAKE) -C $(KERNEL_OUT) M=$(FULL_PATH_1) ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(KERNEL_CROSS_COMPILE) clean

kernel-modules: bcmdhd_module

ALL_DEFAULT_INSTALLED_MODULES += bcmdhd_module
endif

ifeq ($(TARGET_USE_MSTAR_HW_T1_PLAT),true)
LOCAL_DIR:=$(call my-dir)
BUILD_DIR:=$(LOCAL_DIR)/build
TARGETDIR=$(KERNEL_MODULES_OUT)

LINUXVER:=3.10
KERNELDIR:=/kernel/mstar/t1/3.10/
CROSSTOOL:=/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/
LINUXDIR:=out/target/product/$(TARGET_DEVICE)/obj/KERNEL_OBJ
# PATH:=${CROSSTOOL}:${LINUXDIR}:$PATH
CC:=aarch64-linux-android-gcc
STRIP:=aarch64-linux-android-strip
CROSS_COMPILE:=aarch64-linux-android-

#These are for compiling under FOS
# export TARGET_ARCH=arm
TARGET_ARCH:=arm64
CFLAGS:=-w
# export CFLAGS=-w
#export LINUX_PORT=1

.PHONY: bcmdhd_module
bcmdhd_module: $(INSTALLED_KERNEL_TARGET) | $(KERNEL_OUT) $(KERNEL_MODULES_OUT) $(ACP)
	$(hide) rm -rf $(ANDROID_PRODUCT_OUT)/system/lib/modules/bcmdhd_43569.ko
	$(MAKE) -C $(KERNEL_OUT) M=$(FULL_PATH_1) ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(KERNEL_CROSS_COMPILE) modules
	$(hide) cp -f $(FULL_PATH_1)/bcmdhd.ko $(KERNEL_MODULES_OUT)/bcmdhd_43569.ko
	$(MAKE) -C $(KERNEL_OUT) M=$(FULL_PATH_1) ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(KERNEL_CROSS_COMPILE) clean

kernel-modules: bcmdhd_module
endif

ifeq ($(TARGET_PRODUCT), margo_r2)
LOCAL_DIR:=$(call my-dir)
BUILD_DIR:=$(LOCAL_DIR)/build
TARGETDIR=$(KERNEL_MODULES_OUT)

LINUXVER:=3.10
KERNELDIR:=/kernel/mstar/t1/3.10/
CROSSTOOL:=/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/
LINUXDIR:=out/target/product/margo/obj/KERNEL_OBJ
# PATH:=${CROSSTOOL}:${LINUXDIR}:$PATH
CC:=aarch64-linux-android-gcc
STRIP:=aarch64-linux-android-strip
CROSS_COMPILE:=aarch64-linux-android-

#These are for compiling under FOS
# export TARGET_ARCH=arm
TARGET_ARCH:=arm64
CFLAGS:=-w
# export CFLAGS=-w
#export LINUX_PORT=1

.PHONY: bcmdhd_module
bcmdhd_module: $(INSTALLED_KERNEL_TARGET) | $(KERNEL_OUT) $(KERNEL_MODULES_OUT) $(ACP)
	$(hide) rm -rf $(ANDROID_PRODUCT_OUT)/system/lib/modules/bcmdhd_43569.ko
	$(MAKE) -C $(KERNEL_OUT) M=$(FULL_PATH_1) ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(KERNEL_CROSS_COMPILE) modules
	$(hide) cp -f $(FULL_PATH_1)/bcmdhd.ko $(KERNEL_MODULES_OUT)/bcmdhd_43569.ko
	$(MAKE) -C $(KERNEL_OUT) M=$(FULL_PATH_1) ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(KERNEL_CROSS_COMPILE) clean

kernel-modules: bcmdhd_module
endif

ifeq ($(TARGET_DEVICE),sbc123)
$(warning targetdir=$(KERNEL_MODULES_OUT))
LOCAL_DIR:=$(call my-dir)
BUILD_DIR:=$(LOCAL_DIR)/build
TARGETDIR=$(KERNEL_MODULES_OUT)

###################################################################
# setting the path for cross compiler
###################################################################
LINUXVER:=3.18
KERNELDIR:=kernel/mediatek/mt8163/3.18
CROSSTOOL:=prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/
LINUXDIR:=out/target/product/sbc123/obj/KERNEL_OBJ
# PATH:=${CROSSTOOL}:${LINUXDIR}:$PATH
CC:=aarch64-linux-android-gcc
STRIP:=aarch64-linux-android-strip
CROSS_COMPILE:=aarch64-linux-android-

#These are for compiling under FOS
TARGET_ARCH=arm64
CFLAGS=-w

.PHONY: bcmdhd_module
bcmdhd_module: $(INSTALLED_KERNEL_TARGET) | $(KERNEL_OUT) $(KERNEL_MODULES_OUT) $(ACP)
	$(hide) rm -rf $(ANDROID_PRODUCT_OUT)/system/lib/modules/bcmdhd.ko
	$(MAKE) -C $(KERNEL_OUT) M=$(FULL_PATH_1) ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(KERNEL_CROSS_COMPILE) modules
	$(hide) cp -f $(FULL_PATH_1)/bcmdhd.ko $(KERNEL_MODULES_OUT)
	$(MAKE) -C $(KERNEL_OUT) M=$(FULL_PATH_1) ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(KERNEL_CROSS_COMPILE) clean

external-modules: bcmdhd_module
endif

ifeq ($(TARGET_DEVICE),abe123)
$(warning targetdir=$(KERNEL_MODULES_OUT))
LOCAL_DIR:=$(call my-dir)
BUILD_DIR:=$(LOCAL_DIR)/build
TARGETDIR=$(KERNEL_MODULES_OUT)

###################################################################
# setting the path for cross compiler
###################################################################
LINUXVER:=3.18
KERNELDIR:=kernel/mediatek/mt8163/3.18
CROSSTOOL:=prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/
LINUXDIR:=out/target/product/abe123/obj/KERNEL_OBJ
# PATH:=${CROSSTOOL}:${LINUXDIR}:$PATH
CC:=aarch64-linux-android-gcc
STRIP:=aarch64-linux-android-strip
CROSS_COMPILE:=aarch64-linux-android-

#These are for compiling under FOS
TARGET_ARCH=arm64
CFLAGS=-w

.PHONY: bcmdhd_module
bcmdhd_module: $(INSTALLED_KERNEL_TARGET) | $(KERNEL_OUT) $(KERNEL_MODULES_OUT) $(ACP)
	$(hide) rm -rf $(ANDROID_PRODUCT_OUT)/system/lib/modules/bcmdhd.ko
	$(MAKE) -C $(KERNEL_OUT) M=$(FULL_PATH_1) ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(KERNEL_CROSS_COMPILE) modules
	$(hide) cp -f $(FULL_PATH_1)/bcmdhd.ko $(KERNEL_MODULES_OUT)
	$(MAKE) -C $(KERNEL_OUT) M=$(FULL_PATH_1) ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(KERNEL_CROSS_COMPILE) clean

external-modules: bcmdhd_module
endif
