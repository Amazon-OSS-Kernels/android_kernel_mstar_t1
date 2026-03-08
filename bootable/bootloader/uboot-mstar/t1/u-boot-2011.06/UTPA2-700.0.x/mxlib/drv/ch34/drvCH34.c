/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
/**
 *
 *
 */


///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvTEMP.c
/// @brief  TEMP Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvCH34.h"
#include "drvCH34_v2.h"
#include "drvCH34_private.h"
#include "drvMMIO.h"

//#include "drvCH34_Tbl.h"

// Internal Definition
#include "MsOS.h"
#include "regCHIP.h"
#include "regCH34.h"
#include "halCH34.h"
#include "utopia.h"

//#include "halCH34_Tbl.h"


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
#define CH34_TABLE_OFFSET                  4UL
#define CH34_TABLE_MAX_DEPTH               2000UL

static MS_BOOL _bInit;
void* pu32CH34Inst = NULL;

CH34_Drv_Result MDrv_Ch34_Init_U2(void)
{
    MS_VIRT virtCH34RiuBaseAddr;
    MS_U32 u32NonPMBankSize;
	// get MMIO base
	if(MDrv_MMIO_GetBASE( &virtCH34RiuBaseAddr, &u32NonPMBankSize, MS_MODULE_GE ) != TRUE)
	{
		printf("GetBase failed\n");
		return En_Drv_CH34_FAIL;
	}

    if(!_bInit)
    {
        // Driver Initialization
        HAL_CH34_Init(virtCH34RiuBaseAddr);
        _bInit = TRUE;
    }
    return En_Drv_CH34_OK;
}

CH34_Drv_Result MDrv_Ch34_Init(void)
{
    if (pu32CH34Inst == NULL)
    {
        if(UtopiaOpen(MODULE_CH34, &pu32CH34Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen pu32CH34Inst failed\n");
    		return En_Drv_CH34_FAIL;
        }
    }
	else
	{
        printf("CH34 has been initialized\n");
        return En_Drv_CH34_OK;
	}

	CH34_INIT_INFO CH34Args;
	if(UtopiaIoctl(pu32CH34Inst, MAPI_CMD_CH34_INIT, (void*)&CH34Args) != UTOPIA_STATUS_SUCCESS)
	{
		printf("Obtain CH34 engine fail\n");
		return En_Drv_CH34_FAIL;
	}
	else
	{
		return CH34Args.bReturnValue;
	}
}

CH34_Drv_Result MDrv_Ch34_TurnOnOff_U2(MS_BOOL bOn)
{
	HAL_CH34_EnableDAC(bOn);
    return En_Drv_CH34_OK;
}

CH34_Drv_Result MDrv_Ch34_TurnOnOff(MS_BOOL bOn)
{
    if (pu32CH34Inst == NULL)
    {
		printf("CH34 not initialize!\n");
        return En_Drv_CH34_FAIL;

    }

	CH34_TURN_ONOFF_INFO CH34Args;
	CH34Args.bOn = bOn;
	if(UtopiaIoctl(pu32CH34Inst, MAPI_CMD_CH34_TURNONOFF, (void*)&CH34Args) != UTOPIA_STATUS_SUCCESS)
	{
		printf("Obtain CH34 engine fail\n");
		return En_Drv_CH34_FAIL;
	}
	else
	{
		return CH34Args.bReturnValue;
	}
}

static void _MappingEnum(E_CH34_CHANNEL channel, E_CH34_VIDEO_SYS videosys, E_CH34_AUDIO_SYS audiosys,
			E_HAL_CH34_CHANNEL *halchannel, E_HAL_CH34_VIDEO_SYS *halvideosys, E_HAL_CH34_AUDIO_SYS *halaudiosys)
{
	switch(channel)
	{
		case E_CH34_CH3:
			*halchannel = E_HAL_CH34_CH3;
			break;
		case E_CH34_CH4:
		default:
			*halchannel = E_HAL_CH34_CH4;
			break;
	}
	switch(videosys)
	{
		case E_CH34_VIDEO_SYS_NTSC:
			*halvideosys = E_HAL_CH34_VIDEO_SYS_NTSC;
			break;
		case E_CH34_VIDEO_SYS_PAL:
		default:
			*halvideosys = E_HAL_CH34_VIDEO_SYS_PAL;
			break;
	}
	switch(audiosys)
	{
		case E_CH34_AUDIO_SYS_MONO:
			*halaudiosys = E_HAL_CH34_AUDIO_SYS_MONO;
			break;
		case E_CH34_AUDIO_SYS_BTSC:
			*halaudiosys = E_HAL_CH34_AUDIO_SYS_BTSC;
			break;
                  case E_CH34_AUDIO_SYS_BTSC_SAP:
			*halaudiosys = E_HAL_CH34_AUDIO_SYS_BTSC_SAP;
			break;
		case E_CH34_AUDIO_SYS_A2:
		default:
			*halaudiosys = E_HAL_CH34_AUDIO_SYS_A2;
			break;
	}
}

CH34_Drv_Result MDrv_Ch34_SetMode_U2(E_CH34_CHANNEL channel, E_CH34_VIDEO_SYS videosys, E_CH34_AUDIO_SYS audiosys)
{
	MS_U8 i = 0;
	E_HAL_CH34_CHANNEL eChannel;
	E_HAL_CH34_VIDEO_SYS eVideoSys;
	E_HAL_CH34_AUDIO_SYS eAudiosys;
	_MappingEnum(channel,videosys,audiosys,&eChannel,&eVideoSys,&eAudiosys);
	for(i = 0 ; i < E_HAL_CH34_TBL_MAX ; i++)
	{
		MS_U8* pTable = NULL;
		if(HAL_CH34_GetTable(eChannel,eVideoSys,eAudiosys,(E_HAL_CH34_TABLETYPE)i,&pTable) == FALSE)
		{
			printf("[%s] Get table fail\n",__FUNCTION__);
			return En_Drv_CH34_INVALID_PARAM;
		}
		if(NULL != pTable)
		{
	    	MDrv_Ch34_WriteCH34Table_U2(pTable);
		}
	}
	return En_Drv_CH34_OK;
}

CH34_Drv_Result MDrv_Ch34_SetMode(E_CH34_CHANNEL channel, E_CH34_VIDEO_SYS videosys, E_CH34_AUDIO_SYS audiosys)
{
    if (pu32CH34Inst == NULL)
    {
		printf("CH34 not initialize!\n");
        return En_Drv_CH34_FAIL;

    }

	CH34_SET_MODE_INFO CH34Args;
	CH34Args.channel = channel;
	CH34Args.videosys = videosys;
	CH34Args.audiosys = audiosys;
	if(UtopiaIoctl(pu32CH34Inst, MAPI_CMD_CH34_SETMODE, (void*)&CH34Args) != UTOPIA_STATUS_SUCCESS)
	{
		printf("Obtain CH34 engine fail\n");
		return En_Drv_CH34_FAIL;
	}
	else
	{
		return CH34Args.bReturnValue;
	}
}

CH34_Drv_Result MDrv_Ch34_WriteCH34Table_U2(MS_U8* pTable)
{
    MS_U32 u32Index; // register addr+offset
    MS_U16 u16Dummy;
    if(!_bInit)
    {
		printf("CH34 not initialize!\n");
        return En_Drv_CH34_FAIL;
    }

    u16Dummy = CH34_TABLE_MAX_DEPTH;
    do
    {
        u32Index = ((pTable[0] << 16) + (pTable[1] << 8) + pTable[2]);
		u32Index &= 0xFFFFFFUL;
        if (u32Index == 0xFFFFFFUL) // check end of table
            break;

		CH34_WriteReg(u32Index,pTable[3]);

        pTable += CH34_TABLE_OFFSET; //next row of table

    }while (--u16Dummy > 0);

    return En_Drv_CH34_OK;
}

CH34_Drv_Result MDrv_Ch34_WriteCH34Table(MS_U8* pTable)
{
    if (pu32CH34Inst == NULL)
    {
		printf("CH34 not initialize!\n");
        return En_Drv_CH34_FAIL;

    }

	CH34_WRITE_TABLE_INFO CH34Args;
	CH34Args.pTable = pTable;
	if(UtopiaIoctl(pu32CH34Inst, MAPI_CMD_CH34_WRITETABLE, (void*)&CH34Args) != UTOPIA_STATUS_SUCCESS)
	{
		printf("Obtain CH34 engine fail\n");
		return En_Drv_CH34_FAIL;
	}
	else
	{
		return CH34Args.bReturnValue;
	}

}

