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

#ifndef DRV_SCALER_ADAPTIVE_H
#define DRV_SCALER_ADAPTIVE_H

/******************************************************************************/
/*                     Macro                                                  */
/* ****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************/
/*                     definition                                             */
/* ****************************************************************************/

/******************************************************************************/
/*                           Global Variables                                 */
/******************************************************************************/

//*************************************************************************
//          Enums
//*************************************************************************

//*************************************************************************
//          Structures
//*************************************************************************
typedef enum
{
    Level_Low=0,
    Level_MID,
    Level_High,

    MAX_Level
}XC_CTRL_LEVEL;

typedef enum
{
    E_FRC_MODE_NONE = 0,
    E_FRC_MODE_VDEC_DROP,
    //E_FRC_MODE_VDEC_REPEAT,
    E_FRC_MODE_NUM
}E_FRC_MODE;

/********************************************************************************/
/*                   Function Prototypes                                        */
/********************************************************************************/
MS_U8 MApi_XC_GetCurrentMotionValue(void);
void MApi_XC_AdaptiveTuning(void);
void MApi_XC_FilmMode_AnyCandence_Enable(MS_BOOL bEnable);
void MApi_XC_DBK_Control(MS_BOOL bEnable, XC_CTRL_LEVEL eLevel);
void MApi_XC_DMS_Control(MS_BOOL bEnable, XC_CTRL_LEVEL eLevel);
void MApi_XC_SotreCurrentValue(void);
void MApi_XC_ReSotreToDefault(void);
void MDrv_XC_ScalerAdaptiveGetFRCMode(E_FRC_MODE eFRCMode);

#undef INTERFACE
#ifdef __cplusplus
}
#endif


#endif /* DRV_SCALER_ADAPTIVE_H */
