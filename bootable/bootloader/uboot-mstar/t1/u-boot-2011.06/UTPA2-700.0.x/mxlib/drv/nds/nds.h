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
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    nds.h
/// @brief  NDS internal header file
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __NDS_H__
#define __NDS_H__


//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define NDS_ECM_SAME_PID_DROP_EN        1

#define E_NDS_FLT_EMM                   0x0001
#define E_NDS_FLT_ECM                   0x0002

#define NDS_DEBUG_ENABLE                1

#if 1 // NDS_DEBUG_ENABLE
#define NDS_DBG(_fmt, _args...)         printf(_fmt, ##_args)
#define NDS_FUNC(_fmt, _args...)        { } //printf(_fmt, ##_args)
#else
#define NDS_DBG(_fmt, _args...)         { }
#define NDS_FUNC(_fmt, _args...)        { }
#endif

#ifdef MS_DEBUG
#define NDS_ERR(_fmt, _args...)         printf(_fmt, ##_args)
#define NDS_ASSERT(_cnd, _ret,                                      \
                   _fmt, _args...)                                  \
                                        if (!(_cnd)) {              \
                                          printf("[%s][%d]",__FUNCTION__,__LINE__);\
                                            printf(_fmt, ##_args);  \
                                            while(1);               \
                                        }
#else
#define NDS_ERR(_fmt, _args...)         { }
#define NDS_ASSERT(_cnd, _ret,                                      \
                   _fmt, _args...)                                  \
                                        if (!(_cnd)) {              \
                                          printf("[%s][%d]",__FUNCTION__,__LINE__);\
                                            printf(_fmt, ##_args);  \
                                            return _ret;            \
                                        } // CAUTION: It should be before GE_ENTRY
#endif


#define NDS_DELAY(_msec)                { MsOS_DelayTask(_msec); }


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

// ICAM2 CA/UART
NDS_Result  NDS_CAM_Init(void);
NDS_Result  NDS_CAM_Exit(void);

NDS_Result  NDS_FLT_Init(NDS_FLT_Param *param);
NDS_Result  NDS_FLT_Exit(void);

NDS_Result  NDS_SC_Init(NDS_SC_Param *param);
NDS_Result  NDS_SC_Exit(void);
NDS_Result  NDS_SC_Open(void);
NDS_Result  NDS_SC_Close(void);
NDS_Result  NDS_SC_Exit(void);

NDS_Result  NDS_NSK_Init(MS_U32 nds_id);
NDS_Result  NDS_NSK_Exit(MS_U32 nds_id);

NDS_Result  NDS_RASP_Init(NDS_RASP_Param *param);


void        NDS_DBG_PrintData(MS_U8 *pu8Data, MS_U16 u16DataLen);


#endif // __NDS_H__

