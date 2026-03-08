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
#ifndef __MLOG_H__
#define __MLOG_H__

#ifdef __cplusplus
extern "C" {
#endif
typedef enum
{
    EN_LOG_PRIORITY_UNKNOWN = 0,
    EN_LOG_PRIORITY_DEFAULT,    /* only for SetMinPriority() */
    EN_LOG_PRIORITY_VERBOSE,
    EN_LOG_PRIORITY_INFO,
    EN_LOG_PRIORITY_WARN,
    EN_LOG_PRIORITY_DEBUG,
    EN_LOG_PRIORITY_ERROR,
    EN_LOG_PRIORITY_FATAL,
    EN_LOG_PRIORITY_SILENT,
} EN_LOG_PRIORITY;

void filter_log(EN_LOG_PRIORITY pri,const char* TAG,const char* format, ...);

#define MLOGV(tag,fmt,...)      filter_log(EN_LOG_PRIORITY_VERBOSE, tag, fmt, ##__VA_ARGS__)
#define MLOGI(tag,fmt,...)      filter_log(EN_LOG_PRIORITY_INFO, tag, fmt, ##__VA_ARGS__)
#define MLOGW(tag,fmt,...)      filter_log(EN_LOG_PRIORITY_WARN, tag, fmt, ##__VA_ARGS__)
#define MLOGD(tag,fmt,...)      filter_log(EN_LOG_PRIORITY_DEBUG, tag, fmt, ##__VA_ARGS__)
#define MLOGE(tag,fmt,...)      filter_log(EN_LOG_PRIORITY_ERROR, tag, fmt, ##__VA_ARGS__)
#define MLOGF(tag,fmt,...)      filter_log(EN_LOG_PRIORITY_FATAL, tag, fmt, ##__VA_ARGS__)
#define TARGET_USES_LOGD        1    // Support Client–Server model
//-------------------------------------------------------------------------------------------------
/// show TAG
/// @param  NONE
/// @return  NONE
//-------------------------------------------------------------------------------------------------
void mlog_showTags(void);
//-------------------------------------------------------------------------------------------------
/// add TAG
/// @param  TAG
/// @return  NONE
//-------------------------------------------------------------------------------------------------
void mlog_addTAG(char* TAG);
//-------------------------------------------------------------------------------------------------
/// remove TAG
/// @param  TAG
/// @return  NONE
//-------------------------------------------------------------------------------------------------
void mlog_removeTAG(char* TAG);
//-------------------------------------------------------------------------------------------------
/// get Priority
/// @param  NONE
/// @return  EN_LOG_PRIORITY
//-------------------------------------------------------------------------------------------------
EN_LOG_PRIORITY mlog_getPriority(void);
//-------------------------------------------------------------------------------------------------
/// set Priority
/// @param  pri                             \b set up priority
/// @return  NONE
//-------------------------------------------------------------------------------------------------
void mlog_setPriority(EN_LOG_PRIORITY pri);


#ifdef __cplusplus
}
#endif

#endif
