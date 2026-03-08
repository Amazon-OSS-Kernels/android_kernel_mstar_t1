/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   mdrv_ldm_common.h
/// @brief  MStar local dimming Interface header file
/// @author MStar Semiconductor Inc.
/// @attention
/// <b><em></em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_LD_COMMON_H
#define _MDRV_LD_COMMON_H


#if defined(__cplusplus)
extern "C" {
#endif

//=============================================================================
// Defines & Macros
//=============================================================================


//=============================================================================
// Debug Macros
//=============================================================================


#define CHECK_POINTER_I(ptr)   \
    if((NULL == ptr) || ((void*)((size_t)-1) == ptr))  \
    {   \
        printk("error! %s:%d, parametre error Pointer is null \n", __FUNCTION__, __LINE__); \
        return -1;    \
    }

#define CHECK_POINTER_V(ptr)   \
    if((NULL == ptr) || ((void*)((size_t)-1) == ptr))  \
    {   \
        printk("error! %s:%d, parametre error Pointer is null \n", __FUNCTION__, __LINE__); \
        return;    \
    }

#define LD_ERROR(msg...)\
    do{\
        if(enDbgLevel&E_DEBUG_LEVEL_ERROR) \
        { \
            printk("\033[0;31m[LDM ERROR] %s:%d: \033[0m",__FUNCTION__,__LINE__);\
            printk(msg);\
        } \
    }while(0)
    
#define LD_INFO(msg...)\
    do{\
        if(enDbgLevel&E_DEBUG_LEVEL_INFO) \
        { \
            printk("\033[0;33m[LDM INFO] %s:%d: \033[0m",__FUNCTION__,__LINE__);\
            printk(msg);\
        } \
    }while(0)


#define LD_DEBUG(msg...)\
    do{\
        if(enDbgLevel&E_DEBUG_LEVEL_DEBUG) \
        { \
            printk("\033[0;34m[LDM DEBUG] %s:%d: \033[0m",__FUNCTION__,__LINE__);\
            printk(msg);\
        } \
    }while(0)


#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))

//=============================================================================
// Type and Structure Declaration
//=============================================================================
typedef enum
{
    E_LD_LED_EDGE_TB_TYPE = 0, // deprecated
    E_LD_LED_EDGE_LR_TYPE = 1, // deprecated
    E_LD_LED_DIRECT_TYPE  = 2,
    E_LD_LED_LOCAL_TYPE   = 3,  // new mode supporting 32x32 LED
    E_LD_LED_TYPE_NUM,
    E_LD_LED_TYPE_MAX = E_LD_LED_TYPE_NUM,
} EN_LD_TYPE;

/*
typedef enum
{
    EN_DEBUG_LEVEL_ERROR=0x01,
    EN_DEBUG_LEVEL_INFO=0x02,
    EN_DEBUG_LEVEL_DEBUG=0x04,
    EN_DEBUG_LEVEL_ALL=0x07,
    EN_DEBUG_LEVEL_MAX   
}EN_DEBUG_LEVEL;
*/



//=============================================================================
// Variable
//=============================================================================


//=============================================================================
// Function
//=============================================================================


#if defined(__cplusplus)
}
#endif


#endif //_MDRV_LD_COMMON_H

