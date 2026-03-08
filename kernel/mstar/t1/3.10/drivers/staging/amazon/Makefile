
obj-$(CONFIG_AMAZON_KLOG_CONSOLE)	+= klog_console.o
obj-$(CONFIG_AMAZON_SIGN_OF_LIFE) += sign_of_life.o
obj-$(CONFIG_AMAZON_SIGN_OF_LIFE_MSTAR) += sign_of_life_mstar.o
obj-$(CONFIG_AMAZON_PLATFORM_BT_RFKILL)	+= platform_bt_rfkill.o

ifeq ($(or $(strip $(CONFIG_AMAZON_METRICS_LOG)), $(strip $(CONFIG_AMAZON_MINERVA_METRICS_LOG))),y)
	obj-y += amazon_logger.o
endif
obj-$(CONFIG_AMAZON_METRICS_LOG_TEST)	+= amazon_logger_test.o
obj-$(CONFIG_AMAZON_SILENT_OTA_BACKLIGHT) += silent_ota_backlight_mstar.o
