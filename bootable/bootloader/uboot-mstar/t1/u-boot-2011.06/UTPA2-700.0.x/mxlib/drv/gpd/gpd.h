/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//
//
//
//
//
//
//
//
//
//******************************************************************************
#ifndef _GPD_H
#define _GPD_H

//#define __DEBUG__
//#define U02
#define PA
#define FPGA
#define DUMP_LOG
#define DUMP_LOG2
//#define G3D
//#define TOTAL_TIME
//#define HW_TIME
//#define HUFF_REG_TIME


#if 1

#define DRAMBASE         0x00000000  //physical address

#else

//#define DRAMBASE         0x80000000   //cache
#define DRAMBASE        0xA0000000      //non-cache

#endif


#define ARGB8888   0
#define YUV422     1  //YUYV order from lsb to msb
#define RGB565     2
#define YVYU422    3  //YVYU order from lsb to msb
#define GRAY       4
#define GRAY_ALPHA 5


#define MMIO8(addr) (*((volatile MS_U8*)(MS_VIRT)(addr)))
#define MMIO32(addr) (*((volatile MS_U32*)(MS_VIRT)(addr)))

#ifndef TRUE
#define TRUE		1
#define FALSE		0
#endif

typedef enum
{
    E_GPD_DEBUG_LEVEL_NONE = 0x0,
    E_GPD_DEBUG_LEVEL_ERROR = 0x1,
    E_GPD_DEBUG_LEVEL_INFO = 0x2,
    E_GPD_DEBUG_LEVEL_TIME = 0x4,
    E_GPD_DEBUG_LEVEL_ALL = 0xf,
} gpd_debug_level;

extern gpd_debug_level eEnableDbgMsg;

#define ENABLE_GPD_MESSAGE    TRUE
#define ENABLE_GPD_PATCH      FALSE

#if (defined CONFIG_MLOG)
#include "ULog.h"

#define console_printf(format, args...)         do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) ULOGI("DRV_GPD", format, ##args);}while(0)
#define GPD_DRV_ERROR(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) ULOGE("DRV_GPD", format, ##args);}while(0)
#define GPD_DRV_TIME(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_TIME) ULOGI("DRV_GPD", format, ##args);}while(0)

#else

#ifdef ANDROID
#define LOG_TAG "API_GPD"
#include <cutils/log.h>
#endif

#if ENABLE_GPD_MESSAGE

#ifndef ANDROID
#ifdef MSOS_TYPE_LINUX_KERNEL
#define console_printf(format, args...)   do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) printk(format, ##args);}while(0)
#define GPD_DRV_ERROR(format, args...)   do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) printk(format, ##args);}while(0)
#define GPD_DRV_TIME(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_TIME) printk(format, ##args);}while(0)
#else
#define console_printf(format, args...)   do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) printf(format, ##args);}while(0)
#define GPD_DRV_ERROR(format, args...)   do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) printf(format, ##args);}while(0)
#define GPD_DRV_TIME(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_TIME) printf(format, ##args);}while(0)
#endif
#else
#ifndef LOGD
#define console_printf(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) ALOGI(format, ##args);}while(0)
#define GPD_DRV_ERROR(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) ALOGI(format, ##args);}while(0)
#define GPD_DRV_TIME(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_TIME) ALOGI(format, ##args);}while(0)
#else
#define console_printf(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) LOGI(format, ##args);}while(0)
#define GPD_DRV_ERROR(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) LOGI(format, ##args);}while(0)
#define GPD_DRV_TIME(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_TIME) LOGI(format, ##args);}while(0)
#endif
#endif

#else

#define console_printf(...)

#endif


#endif

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

//MS_U32 console_printf(const char* fmt, ...);




#endif
