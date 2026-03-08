/*
 * Customer code to add GPIO control during WLAN start/stop
 *
 * Copyright (C) 1999-2016, Broadcom Corporation
 * 
 *      Unless you and Broadcom execute a separate written software license
 * agreement governing use of this software, this software is licensed to you
 * under the terms of the GNU General Public License version 2 (the "GPL"),
 * available at http://www.broadcom.com/licenses/GPLv2.php, with the
 * following added to such license:
 * 
 *      As a special exception, the copyright holders of this software give you
 * permission to link this software with independent modules, and to copy and
 * distribute the resulting executable under terms of your choice, provided that
 * you also meet, for each linked independent module, the terms and conditions of
 * the license of that module.  An independent module is a module which is not
 * derived from this software.  The special exception does not apply to any
 * modifications of the software.
 * 
 *      Notwithstanding the above, under no circumstances may you combine this
 * software in any way with any other Broadcom software provided under a license
 * other than the GPL, without Broadcom's express prior written consent.
 *
 *
 * <<Broadcom-WL-IPTag/Open:>>
 *
 * $Id: dhd_custom_gpio.c 586390 2015-09-15 10:34:19Z $
 */

#include <typedefs.h>
#include <linuxver.h>
#include <osl.h>
#include <bcmutils.h>
#include <dngl_stats.h>
#include <dhd.h>
#include <dhd_linux.h>

#include <wlioctl.h>
#if defined(WL_WIRELESS_EXT)
#include <wl_iw.h>
#endif

#define WL_ERROR(x) printf x
#define WL_TRACE(x)


#if defined(OOB_INTR_ONLY) || defined(BCMSPI_ANDROID)

#if defined(BCMLXSDMMC)
extern int sdioh_mmc_irq(int irq);
#endif /* (BCMLXSDMMC)  */

#if defined(PLATFORM_MPS)
#include <mach/gpio.h>
#endif

/* Customer specific Host GPIO defintion  */
static int dhd_oob_gpio_num = -1;

module_param(dhd_oob_gpio_num, int, 0644);
MODULE_PARM_DESC(dhd_oob_gpio_num, "DHD oob gpio number");

/* This function will return:
 *  1) return :  Host gpio interrupt number per customer platform
 *  2) irq_flags_ptr : Type of Host interrupt as Level or Edge
 *
 *  NOTE :
 *  Customer should check his platform definitions
 *  and his Host Interrupt spec
 *  to figure out the proper setting for his platform.
 *  Broadcom provides just reference settings as example.
 *
 */
int dhd_customer_oob_irq_map(void *adapter, unsigned long *irq_flags_ptr)
{
	int  host_oob_irq = 0;

#if defined(CUSTOM_OOB_GPIO_NUM)
	if (dhd_oob_gpio_num < 0) {
		dhd_oob_gpio_num = CUSTOM_OOB_GPIO_NUM;
	}
#endif /* CUSTOMER_OOB_GPIO_NUM */

	if (dhd_oob_gpio_num < 0) {
		WL_ERROR(("%s: ERROR customer specific Host GPIO is NOT defined \n",
		__FUNCTION__));
		return (dhd_oob_gpio_num);
	}

	WL_ERROR(("%s: customer specific Host GPIO number is (%d)\n",
	         __FUNCTION__, dhd_oob_gpio_num));

#if defined(PLATFORM_MPS)
	gpio_request(dhd_oob_gpio_num, "oob irq");
	host_oob_irq = gpio_to_irq(dhd_oob_gpio_num);
	gpio_direction_input(dhd_oob_gpio_num);
#endif 

	return (host_oob_irq);
}
#endif /* defined(OOB_INTR_ONLY) || defined(BCMSPI_ANDROID) */

/* Customer function to control hw specific wlan gpios */
int
dhd_customer_gpio_wlan_ctrl(void *adapter, int onoff)
{
	int err = 0;

	return err;
}

#ifdef GET_CUSTOM_MAC_ENABLE
/* Function to get custom MAC address */
int
dhd_custom_get_mac_address(void *adapter, unsigned char *buf)
{
	int ret = 0;

	WL_TRACE(("%s Enter\n", __FUNCTION__));
	if (!buf)
		return -EINVAL;

	/* Customer access to MAC address stored outside of DHD driver */
#if (0 || 0) && (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 35))
	ret = wifi_platform_get_mac_addr(adapter, buf);
#endif

#ifdef EXAMPLE_GET_MAC
	/* EXAMPLE code */
	{
		struct ether_addr ea_example = {{0x00, 0x11, 0x22, 0x33, 0x44, 0xFF}};
		bcopy((char *)&ea_example, buf, sizeof(struct ether_addr));
	}
#endif /* EXAMPLE_GET_MAC */

	return ret;
}
#endif /* GET_CUSTOM_MAC_ENABLE */

#if !defined(WL_WIRELESS_EXT)
struct cntry_locales_custom {
	char iso_abbrev[WLC_CNTRY_BUF_SZ];	/* ISO 3166-1 country abbreviation */
	char custom_locale[WLC_CNTRY_BUF_SZ];	/* Custom firmware locale */
	int32 custom_locale_rev;		/* Custom local revisin default -1 */
};
#endif /* WL_WIRELESS_EXT */

#ifndef LAB126_SUPPORT_COUNTRY_CODE
/* Customized Locale table : OPTIONAL feature */
const struct cntry_locales_custom translate_custom_table[] = {
/* Table should be filled out based on custom platform regulatory requirement */
#ifdef EXAMPLE_TABLE
	{"",   "XY", 4},  /* Universal if Country code is unknown or empty */
	{"US", "US", 69}, /* input ISO "US" to : US regrev 69 */
	{"CA", "US", 69}, /* input ISO "CA" to : US regrev 69 */
	{"EU", "EU", 5},  /* European union countries to : EU regrev 05 */
	{"AT", "EU", 5},
	{"BE", "EU", 5},
	{"BG", "EU", 5},
	{"CY", "EU", 5},
	{"CZ", "EU", 5},
	{"DK", "EU", 5},
	{"EE", "EU", 5},
	{"FI", "EU", 5},
	{"FR", "EU", 5},
	{"DE", "EU", 5},
	{"GR", "EU", 5},
	{"HU", "EU", 5},
	{"IE", "EU", 5},
	{"IT", "EU", 5},
	{"LV", "EU", 5},
	{"LI", "EU", 5},
	{"LT", "EU", 5},
	{"LU", "EU", 5},
	{"MT", "EU", 5},
	{"NL", "EU", 5},
	{"PL", "EU", 5},
	{"PT", "EU", 5},
	{"RO", "EU", 5},
	{"SK", "EU", 5},
	{"SI", "EU", 5},
	{"ES", "EU", 5},
	{"SE", "EU", 5},
	{"GB", "EU", 5},
	{"KR", "XY", 3},
	{"AU", "XY", 3},
	{"CN", "XY", 3}, /* input ISO "CN" to : XY regrev 03 */
	{"TW", "XY", 3},
	{"AR", "XY", 3},
	{"MX", "XY", 3},
	{"IL", "IL", 0},
	{"CH", "CH", 0},
	{"TR", "TR", 0},
	{"NO", "NO", 0},
#endif /* EXMAPLE_TABLE */
};
#endif

#ifdef LAB126_SUPPORT_COUNTRY_CODE
#if defined(LAB126_TARGET_SN) /* Update Country Code for FW 120.12 */
#define DEFAULT_WWS_CCODE	"XT"
#define DEFAULT_WWS_REV		70
const struct cntry_locales_custom translate_custom_table[] = {
	{"",   "XT", 70},  /* Universal if Country code is unknown or empty */
	{"WS", "XT", 70},  /* Pass WS if no support for country is not present*/
	{"US", "US", 906}, /* input ISO "US" to : US regrev 906 */
	{"CA", "US", 906}, /* input ISO "CA" to : US regrev 906 */
	{"JP", "JP", 102},
	{"IN", "IN", 36},
	{"EU", "DE", 119}, 
	{"BE", "DE", 119},
	{"BG", "DE", 119},
	{"CY", "DE", 119},
	{"CZ", "DE", 119},
	{"DK", "DE", 119},
	{"EE", "DE", 119},
	{"FI", "DE", 119},
	{"FR", "DE", 119},
	{"DE", "DE", 119},
	{"GR", "DE", 119},
	{"HU", "DE", 119},
	{"IE", "DE", 119},
	{"IT", "DE", 119},
	{"LV", "DE", 119},
	{"LI", "DE", 119},
	{"LT", "DE", 119},
	{"LU", "DE", 119},
	{"MT", "DE", 119},
	{"NL", "DE", 119},
	{"PL", "DE", 119},
	{"PT", "DE", 119},
	{"RO", "DE", 119},
	{"SK", "DE", 119},
	{"SI", "DE", 119},
	{"ES", "DE", 119},
	{"SE", "DE", 119},
        {"GB", "DE", 119},
	{"AT", "DE", 119}
};
#elif defined(LAB126_TARGET_RK) /* Update Country Code for FW 120.18.6 */
#define DEFAULT_WWS_CCODE	"XZ"
#define DEFAULT_WWS_REV		979
const struct cntry_locales_custom translate_custom_table[] = {
	{"",   "XZ", 979},  /* Universal if Country code is unknown or empty */
	{"WS", "XZ", 979},  /* Pass WS if no support for country is not present*/
	{"US", "US", 879}, /* input ISO "US" to : US regrev 906 */
	{"CA", "US", 879}, /* input ISO "CA" to : US regrev 906 */
	{"JP", "JP", 120},
	{"IN", "IN", 991},
	{"EU", "DE", 120},
	{"BE", "DE", 120},
	{"BG", "DE", 120},
	{"CY", "DE", 120},
	{"CZ", "DE", 120},
	{"DK", "DE", 120},
	{"EE", "DE", 120},
	{"FI", "DE", 120},
	{"FR", "DE", 120},
	{"DE", "DE", 120},
	{"GR", "DE", 120},
	{"HU", "DE", 120},
	{"IE", "DE", 120},
	{"IT", "DE", 120},
	{"LV", "DE", 120},
	{"LI", "DE", 120},
	{"LT", "DE", 120},
	{"LU", "DE", 120},
	{"MT", "DE", 120},
	{"NL", "DE", 120},
	{"PL", "DE", 120},
	{"PT", "DE", 120},
	{"RO", "DE", 120},
	{"SK", "DE", 120},
	{"SI", "DE", 120},
	{"ES", "DE", 120},
	{"SE", "DE", 120},
	{"AU", "AU", 931},
	{"NZ", "AU", 931},
	{"MX", "MX", 55},
        {"GB", "DE", 120},
	{"AT", "DE", 120}
};
#else
#define DEFAULT_WWS_CCODE	"XZ"
#define DEFAULT_WWS_REV		991
const struct cntry_locales_custom translate_custom_table[] = {
	{"",   "XZ", 991},  /* Universal if Country code is unknown or empty */
	{"US", "US", 936}, /* input ISO "US" to : US regrev 941 */
	{"CA", "US", 936}, /* input ISO "CA" to : US regrev 941 */
	{"JP", "JP", 966},
	{"AT", "AT", 999},
	{"IN", "IN", 996},
	{"EU", "DE", 990},  /* European union countries to : EU regrev 05 */
	{"BE", "DE", 990},
	{"BG", "DE", 990},
	{"CY", "DE", 990},
	{"CZ", "DE", 990},
	{"DK", "DE", 990},
	{"EE", "DE", 990},
	{"FI", "DE", 990},
	{"FR", "DE", 990},
	{"DE", "DE", 990},
	{"GR", "DE", 990},
	{"HU", "DE", 990},
	{"IE", "DE", 990},
	{"IT", "DE", 990},
	{"LV", "DE", 990},
	{"LI", "DE", 990},
	{"LT", "DE", 990},
	{"LU", "DE", 990},
	{"MT", "DE", 990},
	{"NL", "DE", 990},
	{"PL", "DE", 990},
	{"PT", "DE", 990},
	{"RO", "DE", 990},
	{"SK", "DE", 990},
	{"SI", "DE", 990},
	{"ES", "DE", 990},
	{"SE", "DE", 990},
	{"GB", "GB", 997},
	{"WS", "XZ", 991} /*Pass WS if no support for country is not present*/
};
#endif /* defined(LAB126_TARGET_SN) || defined(LAB126_TARGET_RK) */
#endif /*LAB126_SUPPORT_COUNTRY_CODE*/

/* Customized Locale convertor
*  input : ISO 3166-1 country abbreviation
*  output: customized cspec
*/
#ifdef CUSTOM_COUNTRY_CODE
void get_customized_country_code(void *adapter, char *country_iso_code,
  wl_country_t *cspec, u32 flags)
#else
void get_customized_country_code(void *adapter, char *country_iso_code, wl_country_t *cspec)
#endif /* CUSTOM_COUNTRY_CODE */
{
#if defined(OEM_ANDROID)
#if 0 && (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 39))

	struct cntry_locales_custom *cloc_ptr;

	if (!cspec)
		return;
#ifdef CUSTOM_COUNTRY_CODE
	cloc_ptr = wifi_platform_get_country_code(adapter, country_iso_code,
	           flags);
#else
	cloc_ptr = wifi_platform_get_country_code(adapter, country_iso_code);
#endif /* CUSTOM_COUNTRY_CODE */
	if (cloc_ptr) {
		strlcpy(cspec->ccode, cloc_ptr->custom_locale, WLC_CNTRY_BUF_SZ);
		cspec->rev = cloc_ptr->custom_locale_rev;
	}
	return;
#else
	int size, i;

	size = ARRAYSIZE(translate_custom_table);

	if (cspec == 0)
		 return;

	if (size == 0)
		 return;

	for (i = 0; i < size; i++) {
		if (strcmp(country_iso_code, translate_custom_table[i].iso_abbrev) == 0) {
			memcpy(cspec->ccode,
				translate_custom_table[i].custom_locale, WLC_CNTRY_BUF_SZ);
			cspec->rev = translate_custom_table[i].custom_locale_rev;
			return;
		}
	}

	/* if no country code matched return first universal code from translate_custom_table */
	memcpy(cspec->ccode, translate_custom_table[0].custom_locale, WLC_CNTRY_BUF_SZ);
	cspec->rev = translate_custom_table[0].custom_locale_rev;
	return;
#endif
#endif /* OEM_ANDROID */
}
