/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/


#ifndef __CUS_CONFIG_H__
#define __CUS_CONFIG_H__

//******** This part is for boot ********
#if(ENABLE_MODULE_ANDROID_BOOT == 1)
#define KL_PARTITION "boot"
#else
#define KL_PARTITION "KL"
#endif
#define TEE_PARTITION "tee"
#define SECKL_PARTITION "secKL"
//***************************************

#if (ENABLE_MODULE_ANDROID_BOOT == 1)
#define UBIPART  "ubi"
/*******************************************************************************
* 1,if CONFIG_PATH is null, you should put "boot0.mp3, boot0.jpg, boot.ini" to
*   /tvconfig; otherwise, you should put these files to /tvconfig/CONFIG_PATH.
*
* 2,if CUSTOMER_PATH is null, you should put "sys.ini, mmap.ini" to /tvconfig
*   otherwise, you should put these files to /tvconfig/CUSTOMER_PATH.
*******************************************************************************/
#define CONFIG   "tvconfig"
#define CUSTOMER "tvconfig"
//#define CONFIG_PATH   ""
#define CONFIG_PATH   "/config"
#define CUSTOMER_PATH  ""
#define CHECK_FLASH_EMPTY "tvconfig"
#define SET_ENV_PATITION "system"
#define SET_ENV_FILE "/etc/set_env"
//#define CUSTOMER_PATH  "/Customer"
#elif(ENABLE_STB_CONFIG_PARTITION==1)	// STB Partition Cfg.
  #define UBIPART               "UBI"
  #define CONFIG                "CON"
  #define CUSTOMER              "CON"
  #define CONFIG_PATH           ""
  #define CUSTOMER_PATH         "customer"
  #define CHECK_FLASH_EMPTY     "CON"
  #define SET_ENV_PATITION      "system"
  #define SET_ENV_FILE          "/etc/set_env"
#else
#define UBIPART  "UBI"
#define CONFIG   "CONFIG"
#define CUSTOMER "customer"
#define CONFIG_PATH   ""
#define CUSTOMER_PATH  ""
#define CHECK_FLASH_EMPTY "customerbackup"
#define SET_ENV_PATITION "CONFIG"
#define SET_ENV_FILE "/set_env"
#endif
#define OAD            "oad"
#endif

