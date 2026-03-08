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
///
/// @file   mdrv_iic.h
/// @brief  IIC Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_IIC_H_
#define _DRV_IIC_H_

#include <asm/types.h>
#include "mdrv_types.h"
#include "mdrv_iic_io.h"
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
#define	IIC_NUM_OF_MAX				(20)

#if 1
#define	IIC_NUM_OF_HW				(0)
#define IIC_NUM_OF_SYS_EEPROM		(1)
#else
#define	IIC_NUM_OF_HW				(1)
#define IIC_NUM_OF_SYS_EEPROM		(2)
#define IIC_NUM_OF_NEC_MICOM		(3)
#define IIC_NUM_OF_AUDIO_AMP		(4)

#define IIC_NUM_OF_RGB_EDID			(8)
#define IIC_NUM_OF_HDMI_A_EDID		(9)
#define IIC_NUM_OF_HDMI_B_EDID		(10)
#define IIC_NUM_OF_HDMI_C_EDID		(11)
#define IIC_NUM_OF_HDMI_D_EDID		(12)
#endif
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#define TRUE 				1
#define FALSE                           0


// Speed Mapping: Base on the Driver loading, maybe you must to use the oscilloscope to measure this Value// Speed Mapping has been refined, and you can specify the IIC speed X (KHz) driectly by X.
#define SWI2C_SPEED_MAPPING_400K          400
#define SWI2C_SPEED_MAPPING_350K          350
#define SWI2C_SPEED_MAPPING_300K          300
#define SWI2C_SPEED_MAPPING_250K          250
#define SWI2C_SPEED_MAPPING_200K          200
#define SWI2C_SPEED_MAPPING_150K          150
#define SWI2C_SPEED_MAPPING_100K          100
#define SWI2C_SPEED_MAPPING_50K           50

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

///Define SWI2C read mode
typedef enum {    
	E_SWI2C_READ_MODE_DIRECT,                 			///< first transmit slave address + reg address and then start receive the data */    
	E_SWI2C_READ_MODE_DIRECTION_CHANGE,		///< slave address + reg address in write mode, direction change to read mode, repeat start slave address in read mode, data from device    
	E_SWI2C_READ_MODE_DIRECTION_CHANGE_STOP_START,  ///< slave address + reg address in write mode + stop, direction change to read mode, repeat start slave address in read mode, data from device   
	E_SWI2C_READ_MODE_MAX
} SWI2C_ReadMode;

///Define SWI2C debug level
typedef enum _SWI2C_DbgLvl{    
	E_SWI2C_DBGLVL_NONE = 0,		/// no debug message    
	E_SWI2C_DBGLVL_WARNING,		/// show warning only    
	E_SWI2C_DBGLVL_ERROR,		/// show error only    
	E_SWI2C_DBGLVL_INFO, 			/// show error & informaiton    
	E_SWI2C_DBGLVL_ALL,			/// show error, information & funciton name
}SWI2C_DbgLvl;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------


#endif // _DRV_IIC_H_

