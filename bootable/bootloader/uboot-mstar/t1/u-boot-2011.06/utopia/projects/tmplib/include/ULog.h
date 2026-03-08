/**
* Copyright (c) 2006 – 2018 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of
* the GNU General Public License as published by the Free Software Foundation;
* either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program;
* if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
* MA 02111-1307, USA.
*/

//******************************************************************************
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifndef __ULOG_H__
#define __ULOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#if (defined CONFIG_UTOPIA_TEE || defined CONFIG_MBOOT || defined CONFIG_NOS)
//#define ULOGI(tag,fmt,...)      printf(tag ": " fmt, ##__VA_ARGS__)
//#define ULOGW(tag,fmt,...)      printf(tag ": " fmt, ##__VA_ARGS__)
//#define ULOGD(tag,fmt,...)      printf(tag ": " fmt, ##__VA_ARGS__)
//#define ULOGE(tag,fmt,...)      printf(tag ": " fmt, ##__VA_ARGS__)
//#define ULOGF(tag,fmt,...)      printf(tag ": " fmt, ##__VA_ARGS__)

#define ULOGI(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGW(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGD(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGE(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGF(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#elif (defined MSOS_TYPE_LINUX_KERNEL)
    
#define ULOGI(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGW(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGD(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGE(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGF(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)    

#elif (defined CONFIG_MLOG)
#include "MLog.h"
//#define ULOGI(tag,fmt,...)      MLOGI(tag, fmt, ##__VA_ARGS__)
//#define ULOGW(tag,fmt,...)      MLOGW(tag, fmt, ##__VA_ARGS__)
//#define ULOGD(tag,fmt,...)      MLOGD(tag, fmt, ##__VA_ARGS__)
//#define ULOGE(tag,fmt,...)      MLOGE(tag, fmt, ##__VA_ARGS__)
//#define ULOGF(tag,fmt,...)      MLOGF(tag, fmt, ##__VA_ARGS__)

#define ULOGI(tag,fmt,...)      MLOGI("Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGW(tag,fmt,...)      MLOGW("Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGD(tag,fmt,...)      MLOGD("Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGE(tag,fmt,...)      MLOGE("Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGF(tag,fmt,...)      MLOGF("Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#elif (defined CONFIG_ANDROID)
#include <cutils/log.h>
#ifdef ALOG
//#define ULOGI(tag,fmt,...)      ALOG(LOG_INFO, tag, fmt, ##__VA_ARGS__)
//#define ULOGW(tag,fmt,...)      ALOG(LOG_WARN, tag, fmt, ##__VA_ARGS__)
//#define ULOGD(tag,fmt,...)      ALOG(LOG_DEBUG, tag, fmt, ##__VA_ARGS__)
//#define ULOGE(tag,fmt,...)      ALOG(LOG_ERROR, tag, fmt, ##__VA_ARGS__)
//#define ULOGF(tag,fmt,...)      ALOG(LOG_FATAL, tag, fmt, ##__VA_ARGS__)

#define ULOGI(tag,fmt,...)      ALOG(LOG_INFO, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGW(tag,fmt,...)      ALOG(LOG_WARN, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGD(tag,fmt,...)      ALOG(LOG_DEBUG, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGE(tag,fmt,...)      ALOG(LOG_ERROR, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGF(tag,fmt,...)      ALOG(LOG_FATAL, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#else
//#define ULOGI(tag,fmt,...)      LOG(LOG_INFO, tag, fmt, ##__VA_ARGS__)
//#define ULOGW(tag,fmt,...)      LOG(LOG_WARN, tag, fmt, ##__VA_ARGS__)
//#define ULOGD(tag,fmt,...)      LOG(LOG_DEBUG, tag, fmt, ##__VA_ARGS__)
//#define ULOGE(tag,fmt,...)      LOG(LOG_ERROR, tag, fmt, ##__VA_ARGS__)
//#define ULOGF(tag,fmt,...)      LOG(LOG_FATAL, tag, fmt, ##__VA_ARGS__)

#define ULOGI(tag,fmt,...)      LOG(LOG_INFO, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGW(tag,fmt,...)      LOG(LOG_WARN, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGD(tag,fmt,...)      LOG(LOG_DEBUG, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGE(tag,fmt,...)      LOG(LOG_ERROR, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#define ULOGF(tag,fmt,...)      LOG(LOG_FATAL, "Utopia", "[" tag "]" fmt, ##__VA_ARGS__)
#endif
#else
//#define ULOGI(tag,fmt,...)      printf(tag ": " fmt, ##__VA_ARGS__)
//#define ULOGW(tag,fmt,...)      printf(tag ": " fmt, ##__VA_ARGS__)
//#define ULOGD(tag,fmt,...)      printf(tag ": " fmt, ##__VA_ARGS__)
//#define ULOGE(tag,fmt,...)      printf(tag ": " fmt, ##__VA_ARGS__)
//#define ULOGF(tag,fmt,...)      printf(tag ": " fmt, ##__VA_ARGS__)

#define ULOGI(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGW(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGD(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGE(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#define ULOGF(tag,fmt,...)      printf("[Utopia][" tag "]: " fmt, ##__VA_ARGS__)
#endif

#ifdef __cplusplus
}
#endif

#endif
