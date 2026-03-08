/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
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

#ifndef __DRV_TAS5707_H__
#define __DRV_TAS5707_H__

#include <MsTypes.h>

typedef MS_U8      BOOLEAN;

// ***************************************************************************
// ************************* START OF REGISTER TYPES *************************
// ***************************************************************************
//----------------------------------------------------------------------------
// Control register bit field structure definitions
//----------------------------------------------------------------------------


//-------------------------------------------------------------------------
// Register maps enum
//-------------------------------------------------------------------------

typedef struct {

	U8		        ucAddr;
	U16		        ucNumber;
	U8	            sDatas[20];
} TAS5707_REGMAP, *TAS5707_PREGMAP;

#define BOOL        BOOLEAN

/*#ifdef DRV_TAS5707_C
#define INTERFACE
#else
#define INTERFACE extern
#endif*/
#ifdef DRV_TAS5707_C
    #define  TAS5707_EXT
#else
    #define  TAS5707_EXT  extern
#endif
//INTERFACE void drvTAS5707_Subwoof(void);
//INTERFACE void drvTAS5707_SubwoofValue(U8 Subwoof);
//INTERFACE void drvTAS5707_Shutdown(void);
TAS5707_EXT void drvTAS5707_SW_Init(void);
TAS5707_EXT BOOL drvTAS5707_Is_PowerOn(void);
//INTERFACE BOOLEAN Drv_TAS5707_READ_DATAS(U8 ucSubAddr, U8 ucCnt, U8* pBufs );
//INTERFACE BOOLEAN Drv_TAS5707_WRITE_DATAS(U8 ucSubAddr, U8 ucCnt, U8* pBufs );
//INTERFACE void drvTAS5707_SourceVolSel(INPUT_SOURCE_TYPE_t enInputSourceType);
//INTERFACE void drvTAS5707_SoftMute(BOOLEAN bMute);
//INTERFACE void drvTAS5707_AllChannelShutDown(BOOLEAN bEnable);
//INTERFACE void drvTAS5707_RegSwichChannel(BOOL bUseLeftCH);

#endif // __DRV_TAS5707_H__
