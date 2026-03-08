LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

INSTALLED_MBOOT_TARGET := $(PRODUCT_OUT)/mboot.bin

#TODO: switch obj folder to out/target/product/abc123/obj/MBOOT_OBJ


MBOOT_OUT := $(abspath $(PRODUCT_OUT))/obj/MBOOT_OBJ
TARGET_MBOOT_CONFIG := $(MBOOT_OUT)/.config
# ACOS_MOD_BEGIN {vendor_mstar}
TARGET_MBOOT_CONFIG_DDR3 := $(MBOOT_OUT)/config.ddr3
TARGET_MBOOT_CONFIG_DDR4 := $(MBOOT_OUT)/config.ddr4
TARGET_BUILT_MBOOT_CID41_DDR3 :=  $(MBOOT_OUT)/cid41_ddr3_sboot.bin
TARGET_BUILT_MBOOT_CID41_DDR4 :=  $(MBOOT_OUT)/cid41_ddr4_sboot.bin
TARGET_BUILT_MBOOT_CID00_DDR4 :=  $(MBOOT_OUT)/cid00_ddr4_sboot.bin
TARGET_BUILT_MBOOT := $(MBOOT_OUT)/mboot.bin
# ACOS_MOD_END {vendor_mstar}
ROOTDIR := $(abspath $(TOP))
TOOLSCHAINPATH := $(ROOTDIR)/prebuilts/gcc/linux-x86/arm/arm_eabi-2011.03/bin/
MAKE += SHELL=/bin/bash
export BOARD_UFBL_PROJECT BOARD_KERNEL_SEPARATED_DT

# ACOS_MOD_BEGIN {vendor_mstar}
$(TARGET_MBOOT_CONFIG_DDR3): $(MBOOT_DIR)/$(MBOOT_DEFCONFIG_DDR3) $(ACP) mbootclean
	@echo "Building ddr3 oldconfig for mboot"
	mkdir -p $(MBOOT_OUT)
	$(ACP) $(MBOOT_DIR)/$(MBOOT_DEFCONFIG_DDR3) $@
	$(ACP) $(MBOOT_DIR)/$(MBOOT_DEFCONFIG_DDR3) $(TARGET_MBOOT_CONFIG)
	yes "" | PATH=$(PATH):$(TOOLSCHAINPATH)  $(MAKE) -j1 -C $(MBOOT_DIR) O=$(MBOOT_OUT) oldconfig

$(TARGET_BUILT_MBOOT_CID41_DDR3): $(TARGET_MBOOT_CONFIG_DDR3)
	@echo "Building cid41 ddr3 mboot from $(MBOOT_DIR)"
	PATH=$(PATH):$(TOOLSCHAINPATH) $(MAKE) UFBL_PROJECT=abc123 -j1 -C $(MBOOT_DIR) PRODUCT_OUT=$(abspath $(PRODUCT_OUT))  O=$(MBOOT_OUT) BOARD_TYPE=CID41_DDR3
	@rm -Rf $(MBOOT_DIR)/include
	@rm $(MBOOT_DIR)/.config
	@rm -rf $(MBOOT_OUT)

$(TARGET_MBOOT_CONFIG_DDR4): $(TARGET_BUILT_MBOOT_CID41_DDR3) $(MBOOT_DIR)/$(MBOOT_DEFCONFIG_DDR4) $(ACP)
	@echo "Building ddr4 oldconfig for mboot"
	mkdir -p $(MBOOT_OUT)
	$(ACP) $(MBOOT_DIR)/$(MBOOT_DEFCONFIG_DDR4) $@
	$(ACP) $(MBOOT_DIR)/$(MBOOT_DEFCONFIG_DDR4) $(TARGET_MBOOT_CONFIG)
	yes "" | PATH=$(PATH):$(TOOLSCHAINPATH)  $(MAKE) -j1 -C $(MBOOT_DIR) O=$(MBOOT_OUT) oldconfig

$(TARGET_BUILT_MBOOT_CID41_DDR4): $(TARGET_MBOOT_CONFIG_DDR4)
	@echo "Building cid41_ddr4 mboot from $(MBOOT_DIR)"
	PATH=$(PATH):$(TOOLSCHAINPATH) $(MAKE) UFBL_PROJECT=abc123 -j1 -C $(MBOOT_DIR) PRODUCT_OUT=$(abspath $(PRODUCT_OUT)) O=$(MBOOT_OUT) BOARD_TYPE=CID41_DDR4
	@rm -Rf $(MBOOT_DIR)/include
	@rm $(MBOOT_DIR)/.config
	@rm -rf $(MBOOT_OUT)

ifeq ($(TARGET_PRODUCT),abc123)
$(TARGET_BUILT_MBOOT): $(TARGET_BUILT_MBOOT_CID41_DDR4)
	@echo "Building cid00_ddr4 mboot from $(MBOOT_DIR)"
	mkdir -p $(MBOOT_OUT)
	$(ACP) $(MBOOT_DIR)/$(MBOOT_DEFCONFIG_DDR4) $(TARGET_MBOOT_CONFIG)
	yes "" | PATH=$(PATH):$(TOOLSCHAINPATH)  $(MAKE) -j1 -C $(MBOOT_DIR) O=$(MBOOT_OUT) oldconfig
	PATH=$(PATH):$(TOOLSCHAINPATH) $(MAKE) UFBL_PROJECT=abc123 -j1 -C $(MBOOT_DIR) PRODUCT_OUT=$(abspath $(PRODUCT_OUT)) O=$(MBOOT_OUT) BOARD_TYPE=CID00_DDR4
	@rm -Rf $(MBOOT_DIR)/include
	@rm $(MBOOT_DIR)/.config
else
ifeq ($(TARGET_PRODUCT),abc123)
$(TARGET_BUILT_MBOOT):  $(MBOOT_DIR)/$(MBOOT_DEFCONFIG_DDR3) $(ACP) mbootclean
	@echo "Building cid00_ddr3 mboot from $(MBOOT_DIR)"
	mkdir -p $(MBOOT_OUT)
	$(ACP) $(MBOOT_DIR)/$(MBOOT_DEFCONFIG_DDR3) $(TARGET_MBOOT_CONFIG)
	yes "" | PATH=$(PATH):$(TOOLSCHAINPATH)  $(MAKE) -j1 -C $(MBOOT_DIR) O=$(MBOOT_OUT) oldconfig
	PATH=$(PATH):$(TOOLSCHAINPATH) $(MAKE) UFBL_PROJECT=abc123 -j1 -C $(MBOOT_DIR) PRODUCT_OUT=$(abspath $(PRODUCT_OUT)) O=$(MBOOT_OUT) BOARD_TYPE=CID00_DDR3
	@rm -Rf $(MBOOT_DIR)/include
	@rm $(MBOOT_DIR)/.config
else
$(TARGET_BUILT_MBOOT):  $(MBOOT_DIR)/$(MBOOT_DEFCONFIG_DDR3) $(ACP) mbootclean
	@echo "Building cid00_ddr3 mboot from $(MBOOT_DIR)"
	mkdir -p $(MBOOT_OUT)
	$(ACP) $(MBOOT_DIR)/$(MBOOT_DEFCONFIG_DDR3) $(TARGET_MBOOT_CONFIG)
	yes "" | PATH=$(PATH):$(TOOLSCHAINPATH)  $(MAKE) -j1 -C $(MBOOT_DIR) O=$(MBOOT_OUT) oldconfig
	PATH=$(PATH):$(TOOLSCHAINPATH) $(MAKE) UFBL_PROJECT=alison -j1 -C $(MBOOT_DIR) PRODUCT_OUT=$(abspath $(PRODUCT_OUT)) O=$(MBOOT_OUT) BOARD_TYPE=CID00_DDR3
	@rm -Rf $(MBOOT_DIR)/include
	@rm $(MBOOT_DIR)/.config
endif
# ACOS_MOD_END {vendor_mstar}
.PHONY: mboot mbootclean

mboot: $(INSTALLED_MBOOT_TARGET)

# ACOS_MOD_BEGIN {vendor_mstar}
ifeq ($(wildcard $(TARGET_PREBUILT_SBOOT)),)
ifeq ($(TARGET_PRODUCT),abc123)
TARGET_PREBUILT_SBOOT := $(MBOOT_OUT)/rom_emmc_boot.bin

else
TARGET_PREBUILT_SBOOT := $(MBOOT_OUT)/out/rom_emmc_boot.bin
endif
endif

INSTALLED_SBOOT_TARGET := $(foreach s, $(TARGET_PREBUILT_SBOOT), \
	$(addprefix $(PRODUCT_OUT)/, $(notdir $(s))))

$(INSTALLED_SBOOT_TARGET): $(INSTALLED_MBOOT_TARGET)

$(INSTALLED_MBOOT_TARGET): $(TARGET_BUILT_MBOOT)
ifeq ($(wildcard $(TARGET_PREBUILT_MBOOT)),)
	@echo Copies the built mboot to $(PRODUCT_OUT)
ifeq ($(TARGET_PRODUCT),abc123)
	$(copy-file-to-target)
else
	$(ACP) $(MBOOT_OUT)/out/mboot.bin $(PRODUCT_OUT)/mboot.bin
endif
else
	@echo Copies the prebuilt mboot to $(PRODUCT_OUT)
	$(ACP) $(TARGET_PREBUILT_MBOOT) $(PRODUCT_OUT)/mboot.bin
endif
	@echo Copies the prebuilt sboot to $(PRODUCT_OUT)
	$(foreach f, $(TARGET_PREBUILT_SBOOT), \
		$(ACP) $(f) $(PRODUCT_OUT))


.PHONY: optee_unsigned armtf_unsigned optee armtf

ifeq ($(TARGET_PRODUCT),abc123)
TEE_BIN_FILE	= $(PRODUCT_OUT)/obj/OPTEE_OS/tee.bin
TEE_MMAP_FILE	= $(PRODUCT_OUT)/obj/OPTEE_OS/mmapDB.ini
TEE_BIN_SIZE	= $(shell stat -c%s $(TEE_BIN_FILE))
TEE_MMAP_SIZE	= $(shell stat -c%s $(TEE_MMAP_FILE))
# Subtract the two security headers plus INI file size
TEE_UNSIGNED_SIZE	= $(shell echo "$(TEE_BIN_SIZE) - 272 - 272 - $(TEE_MMAP_SIZE)" | bc)

ATF_BIN_FILE	= $(PRODUCT_OUT)/obj/ARM_TRUSTED_FIRMWARE/arm-trusted-firmware_31.bin

# Extract the raw OPTEE binary and INI file
optee_unsigned: optee
	test ! -s $(TEE_BIN_FILE) || { \
	mkdir -p $(PRODUCT_OUT)/unsigned; \
	head -c $(TEE_UNSIGNED_SIZE) $(TEE_BIN_FILE) \
		> $(PRODUCT_OUT)/unsigned/tee.unsigned; \
	tail -c $(TEE_MMAP_SIZE) $(TEE_BIN_FILE) \
		> $(PRODUCT_OUT)/unsigned/tee.ini.bin; \
	}

# Extract the raw ARM Trusted Firmware binary
armtf_unsigned: armtf
	test ! -s $(ATF_BIN_FILE) || { \
	mkdir -p $(PRODUCT_OUT)/unsigned; \
	cp $(ATF_BIN_FILE) $(PRODUCT_OUT)/unsigned/bl31.unsigned; \
	}

droidcore: optee_unsigned armtf_unsigned
endif
# ACOS_MOD_END {vendor_mstar}

droidcore: $(INSTALLED_MBOOT_TARGET)

mbootclean:
	@echo "Doing mbootclean"
	@rm -rf $(MBOOT_OUT)
	@rm -f $(TARGET_MBOOT_CONFIG)
	@rm -f $(TARGET_MBOOT_CONFIG_DDR3)
	@rm -f $(TARGET_MBOOT_CONFIG_DDR4)
	@find $(MBOOT_DIR)/.. -type f -name '*.o' -delete
	@rm -rf $(PRODUCT_OUT)/mboot.bin
	@rm -rf $(PRODUCT_OUT)/rom_emmc_boot.bin
