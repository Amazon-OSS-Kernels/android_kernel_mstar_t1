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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvSYS.c
/// @brief  System Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsVersion.h"
#include "regCHIP.h"
#include "halCHIP.h"
#include "drvMMIO.h"
#include "drvDMD_VD_MBX.h"
#include "halDMD_VD_MBX.h"
#include "ULog.h"



//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define DMD_VD_MBX_LOCK()      \
    do{                         \
        MS_ASSERT(MsOS_In_Interrupt() == FALSE); \
        ULOGD(TAG_DMD_VD_MBX,"%s lock\n", __FUNCTION__);\
        MsOS_ObtainMutex(_s32_DMD_VD_MBX_Mutex, MSOS_WAIT_FOREVER);\
        }while(0)

#define DMD_VD_MBX_UNLOCK()      \
    do{                         \
        MsOS_ReleaseMutex(_s32_DMD_VD_MBX_Mutex);\
        ULOGD(TAG_DMD_VD_MBX,"%s unlock\n", __FUNCTION__); \
        }while(0)
#else
#define DMD_VD_MBX_LOCK()      \
    do{                         \
        MS_ASSERT(MsOS_In_Interrupt() == FALSE); \
        MsOS_ObtainMutex(_s32_DMD_VD_MBX_Mutex, MSOS_WAIT_FOREVER);\
        }while(0)

#define DMD_VD_MBX_UNLOCK()      \
    do{                         \
        MsOS_ReleaseMutex(_s32_DMD_VD_MBX_Mutex);\
        }while(0)
#endif

#define TAG_DMD_VD_MBX "DMD_VD_MBX"
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if (DMD_VD_MBX_CHIP_VERSION >= DMD_VD_MBX_CHIP_T3)
static MS_S32 _s32_DMD_VD_MBX_Mutex=-1;
#endif
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// System initialzation
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
#if (DMD_VD_MBX_CHIP_VERSION >= DMD_VD_MBX_CHIP_T3)
MS_BOOL MDrv_SYS_DMD_VD_MBX_Init(void)
{
    if (_s32_DMD_VD_MBX_Mutex == -1)
    {
        _s32_DMD_VD_MBX_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex DMD_VD_MBX", MSOS_PROCESS_SHARED);
        MS_ASSERT(_s32_DMD_VD_MBX_Mutex >= 0);
        #ifdef MS_DEBUG
        ULOGD(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_Init %lx\n",_s32_DMD_VD_MBX_Mutex);
        #endif
    }
    else
    {
        ULOGE(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_Init more than twice\n");
        return FALSE;
    }

    if (_s32_DMD_VD_MBX_Mutex >= 0)
    {
        return HAL_SYS_DMD_VD_MBX_Init();
    }
    else
    {
        #ifdef MS_DEBUG
        ULOGE(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_Init Fail\n");
        #endif
        return FALSE;
    }
}

void MDrv_SYS_DMD_VD_MBX_SetType(DMD_VD_MBX_Type eType)
{
    DMD_VD_MBX_LOCK();
    switch (eType)
    {
        case E_DMD_VD_MBX_TYPE_ATV:
            HAL_SYS_DMD_VD_MBX_SetType(BIN_FOR_ATV);
            break;
        case E_DMD_VD_MBX_TYPE_DVBT:
            HAL_SYS_DMD_VD_MBX_SetType(BIN_FOR_DVBT);
            break;
        case E_DMD_VD_MBX_TYPE_DVBC:
            HAL_SYS_DMD_VD_MBX_SetType(BIN_FOR_DVBC);
            break;
        case E_DMD_VD_MBX_TYPE_ATSC:
            HAL_SYS_DMD_VD_MBX_SetType(BIN_FOR_ATSC);
            break;
            
        case E_DMD_VD_MBX_TYPE_DTMB: 
        	  HAL_SYS_DMD_VD_MBX_SetType(BIN_FOR_DTMB);
            break;   
            
        case E_DMD_VD_MBX_TYPE_ISDBT:
            HAL_SYS_DMD_VD_MBX_SetType(BIN_FOR_ISDBT);
            break;
        case E_DMD_VD_MBX_TYPE_DVBS:

            HAL_SYS_DMD_VD_MBX_SetType(BIN_FOR_DVBS);

            break;

        case E_DMD_VD_MBX_TYPE_DVBT2:

            HAL_SYS_DMD_VD_MBX_SetType(BIN_FOR_DVBT2);

            break;

        default:
            #ifdef MS_DEBUG
            ULOGE(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_SetType Fail\n");
            #endif
            break;
    }
    DMD_VD_MBX_UNLOCK();
}

MS_BOOL MDrv_SYS_DMD_VD_MBX_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value)
{
    MS_BOOL bResult = FALSE;
    DMD_VD_MBX_LOCK();
    u32Reg &= 0xFFFF;
    switch (HAL_SYS_DMD_VD_MBX_GetType())
    {
        case BIN_FOR_ATV:
            bResult = HAL_SYS_DMD_VD_MBX_ATV_ReadByte(u32Reg, u8Value);
            break;
        case BIN_FOR_DVBT:
        case BIN_FOR_DVBC:
        case BIN_FOR_DVBS:

		case BIN_FOR_DVBT2:

            bResult = HAL_SYS_DMD_VD_MBX_DVB_ReadByte(u32Reg, u8Value);
            break;
        case BIN_FOR_ATSC:
        case BIN_FOR_DTMB:
        case BIN_FOR_ISDBT:
            *u8Value = HAL_SYS_DMD_VD_MBX_ATSC_ReadByte(u32Reg);
            bResult = TRUE;
            break;
        default:
            #ifdef MS_DEBUG
            ULOGE(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_ReadReg>> Incorrect firmware code!!\n");
            #endif
            *u8Value = 0;
            bResult = FALSE;
            break;
    }
    #ifdef MS_DEBUG
    ULOGD(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_ReadReg %lx %x\n",u32Reg,*u8Value);
    #endif
    DMD_VD_MBX_UNLOCK();
    return bResult;
}

MS_BOOL MDrv_SYS_DMD_VD_MBX_ReadDSPReg(MS_U32 u32Reg, MS_U8 *u8Value)
{
    MS_BOOL bResult;
    DMD_VD_MBX_LOCK();
    u32Reg &= 0xFFFF;
    if (HAL_SYS_DMD_VD_MBX_GetType() & (BIN_FOR_DVBT|BIN_FOR_DVBC|BIN_FOR_DVBT2|BIN_FOR_DVBS))

    {
        bResult = HAL_SYS_DMD_VD_MBX_DVB_ReadDspReg(u32Reg, u8Value);
    }
    else
    {
        #ifdef MS_DEBUG
        ULOGE(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_ReadDSPReg>> Incorrect firmware code!!\n");
        #endif
        bResult = FALSE;
    }
    #ifdef MS_DEBUG
    ULOGD(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_ReadDSPReg %lx %x\n",u32Reg,*u8Value);
    #endif
    DMD_VD_MBX_UNLOCK();
    return bResult;
}

#if (DMD_VD_MBX_CHIP_VERSION >= DMD_VD_MBX_CHIP_T12_T13)
MS_BOOL MDrv_SYS_DMD_VD_MBX_DBG_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value)
{
    MS_BOOL bResult;
    DMD_VD_MBX_LOCK();
    u32Reg &= 0xFFFF;
    if (HAL_SYS_DMD_VD_MBX_GetType() & (BIN_FOR_DVBT|BIN_FOR_DVBC|BIN_FOR_DVBT2|BIN_FOR_DVBS))

    {
        bResult = HAL_SYS_DMD_VD_MBX_DVB_DBG_ReadReg(u32Reg, u8Value);
    }
    else
    {
        #ifdef MS_DEBUG
        ULOGE(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_DBG_ReadReg>> Incorrect firmware code!!\n");
        #endif
        bResult = FALSE;
    }
    #ifdef MS_DEBUG
    ULOGD(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_DBG_ReadReg %lx %x\n",u32Reg,*u8Value);
    #endif
    DMD_VD_MBX_UNLOCK();
    return bResult;
}
#else
MS_BOOL MDrv_SYS_DMD_VD_MBX_DBG_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value)
{
    u32Reg = u32Reg;
    *u8Value = *u8Value;

    return TRUE;
}
#endif

MS_BOOL MDrv_SYS_DMD_VD_MBX_WriteReg(MS_U32 u32Reg, MS_U8 u8Value)
{
    MS_BOOL bResult = FALSE;
    DMD_VD_MBX_LOCK();
    u32Reg &= 0xFFFF;
    #ifdef MS_DEBUG
    ULOGD(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_WriteReg %lx %x\n",u32Reg,u8Value);
    #endif
    switch (HAL_SYS_DMD_VD_MBX_GetType())
    {
        case BIN_FOR_ATV:
            bResult = HAL_SYS_DMD_VD_MBX_ATV_WriteByte(u32Reg, u8Value);
            break;
        case BIN_FOR_DVBT:
        case BIN_FOR_DVBC:
        case BIN_FOR_DVBS:	

		case BIN_FOR_DVBT2:

            bResult = HAL_SYS_DMD_VD_MBX_DVB_WriteByte(u32Reg, u8Value);
            break;
        case BIN_FOR_ATSC:
        case BIN_FOR_DTMB:
        case BIN_FOR_ISDBT:
            HAL_SYS_DMD_VD_MBX_ATSC_WriteByte(u32Reg, u8Value);
            bResult = TRUE;
            break;
        default:
            #ifdef MS_DEBUG
            ULOGE(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_WriteReg>> Incorrect firmware code!!\n");
            #endif
            DMD_VD_MBX_UNLOCK();
            bResult = FALSE;
            break;
    }
    DMD_VD_MBX_UNLOCK();
    return bResult;
}

MS_BOOL MDrv_SYS_DMD_VD_MBX_WriteDSPReg(MS_U32 u32Reg, MS_U8 u8Value)
{
    MS_BOOL bResult;
    DMD_VD_MBX_LOCK();
    u32Reg &= 0xFFFF;
    #ifdef MS_DEBUG
    ULOGD(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_WriteDSPReg %lx %x\n",u32Reg,u8Value);
    #endif
    if (HAL_SYS_DMD_VD_MBX_GetType()& (BIN_FOR_DVBT|BIN_FOR_DVBC|BIN_FOR_DVBT2|BIN_FOR_DVBS))

    {
        bResult = HAL_SYS_DMD_VD_MBX_DVB_WriteDspReg(u32Reg, u8Value);
    }
    else
    {
        #ifdef MS_DEBUG
        ULOGE(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_WriteDSPReg>> Incorrect firmware code!!\n");
        #endif
        bResult = FALSE;
    }
    DMD_VD_MBX_UNLOCK();
    return bResult;
}

#if (DMD_VD_MBX_CHIP_VERSION >= DMD_VD_MBX_CHIP_T12_T13)
MS_BOOL MDrv_SYS_DMD_VD_MBX_DBG_WriteReg(MS_U32 u32Reg, MS_U8 u8Value)
{
    MS_BOOL bResult;
    DMD_VD_MBX_LOCK();
    u32Reg &= 0xFFFF;
    #ifdef MS_DEBUG
    ULOGD(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_DBG_WriteReg %lx %x\n",u32Reg,u8Value);
    #endif
    if (HAL_SYS_DMD_VD_MBX_GetType()& (BIN_FOR_DVBT|BIN_FOR_DVBC|BIN_FOR_DVBT2|BIN_FOR_DVBS))

    {
        bResult = HAL_SYS_DMD_VD_MBX_DVB_DBG_WriteReg(u32Reg, u8Value);
    }
    else
    {
        #ifdef MS_DEBUG
        ULOGE(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_DBG_WriteReg>> Incorrect firmware code!!\n");
        #endif
        bResult = FALSE;
    }
    DMD_VD_MBX_UNLOCK();
    return bResult;
}
#else
MS_BOOL MDrv_SYS_DMD_VD_MBX_DBG_WriteReg(MS_U32 u32Reg, MS_U8 u8Value)
{
    u32Reg = u32Reg;
    u8Value = u8Value;

    return TRUE;
}
#endif
#else
MS_BOOL MDrv_SYS_DMD_VD_MBX_Init(void)
{
    return FALSE;
}
void MDrv_SYS_DMD_VD_MBX_SetType(DMD_VD_MBX_Type eType)
{
    return;
}
MS_BOOL MDrv_SYS_DMD_VD_MBX_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value)
{
    *u8Value = 0;
    return FALSE;
}
MS_BOOL MDrv_SYS_DMD_VD_MBX_ReadDSPReg(MS_U32 u32Reg, MS_U8 *u8Value)
{
    return FALSE;
}
MS_BOOL MDrv_SYS_DMD_VD_MBX_DBG_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value)
{
    return FALSE;
}
MS_BOOL MDrv_SYS_DMD_VD_MBX_WriteReg(MS_U32 u32Reg, MS_U8 u8Value)
{
    return FALSE;
}
MS_BOOL MDrv_SYS_DMD_VD_MBX_WriteDSPReg(MS_U32 u32Reg, MS_U8 u8Value)
{
    return FALSE;
}
MS_BOOL MDrv_SYS_DMD_VD_MBX_DBG_WriteReg(MS_U32 u32Reg, MS_U8 u8Value)
{
    return FALSE;
}
#endif
