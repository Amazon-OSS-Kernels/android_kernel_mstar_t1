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
/// file    drvDAC.c
/// @brief  DAC Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#define  MDRV_DAC_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvMMIO.h"
#include "drvDAC.h"
#include "halDAC.h"
#include "UFO.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

void MDrv_DAC_SetIOMapBase(MS_VIRT virtNPMBase, MS_VIRT virtPMBase)
{
    Hal_DAC_SetIOMapBase(virtNPMBase, virtPMBase);
}

void MDrv_DAC_Init(void)
{
    Hal_DAC_Power_Saving(FALSE);
    Hal_HDMITx_InitSeq();
}

void MDrv_DAC_Exit(void)
{
    Hal_DAC_Power_Saving(TRUE);
}

void MDrv_DAC_Enable(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    Hal_DAC_Enable(bEnable, bIsYPbPr);
}

void MDrv_DAC_SetClkInv(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetClkInv(bEnable, bIsYPbPr);
}

//DAC MUX select: YPbPr -> E_DAC_TO_HDGEN; CVBS/S-Video -> E_DAC_TO_VE
void MDrv_DAC_SetOutputSource(EN_DAC_OUTPUT_TYPE enOutputType, MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetOutputSource(enOutputType, bIsYPbPr);
}

//by default, YPbPr -> E_DAC_OUTPUT_LEVEL_LOW; CVBS/S-Video -> E_DAC_OUTPUT_LEVEL_HIGH
void MDrv_DAC_SetOutputLevel(EN_DAC_MAX_OUTPUT_LEVEL enLevel, MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetOutputLevel(enLevel, bIsYPbPr);
}

void MDrv_DAC_SetOutputSwapSel(EN_DAC_SWAP_SEL enSwap,MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetOutputSwapSel(enSwap,bIsYPbPr);
}

void MDrv_DAC_SetOutputTiming(EN_OUTPUT_VIDEO_TIMING_TYPE enTiming)
{
    Hal_HDGEN_SetTiming(enTiming);
}

void MDrv_ClkSel_Set(EN_OUTPUT_VIDEO_TIMING_TYPE enTiming, EN_OUTPUT_BIT_TYPE enBits)
{
    //Hal_ClkSel_Set(enTiming, enBits);
}

void MDrv_DAC_OnOffSD(EN_DAC_SD_ENABLE_CTRL enBit)
{
    Hal_DAC_OnOffSD(enBit);
}

void MDrv_DAC_OnOffHD(EN_DAC_HD_ENABLE_CTRL enBit)
{
    Hal_DAC_OnOffHD(enBit);
}

void MDrv_DAC_DumpTable(MS_U8 *pDACTable, MS_U8 u8DACtype)
{
    Hal_DAC_DumpTable(pDACTable, u8DACtype);
}
#ifdef UFO_XC_GETOUTPUTINTELACETIMING
//----------------------------------------------------------------
// MDrv_DAC_GetOutputInterlaceTiming - Get InterlaceTiming
// @return: TRUE is InterlaceTiming
//----------------------------------------------------------------
MS_BOOL MDrv_DAC_GetOutputInterlaceTiming(void)
{
    return Hal_DAC_GetOutputInterlaceTiming();
}
#endif

void MDrv_DAC_SetWSSOnOff(MS_BOOL bEnable)
{
    Hal_HDGEN_SetWSSOnOff(bEnable);
}

void MDrv_DAC_ResetWSSData(void)
{
    Hal_HDGEN_ResetWSSData();
}

MS_BOOL MDrv_DAC_WSS_SetAS(MS_BOOL bEnable, EN_OUTPUT_VIDEO_TIMING_TYPE eVideo_Timing, EN_DAC_VIDEO_VBI_WSS_TYPE eWSS_TYPE)
{
    MS_U32 u32PreWSSData = 0;
    MS_U32 u32WSSData = 0;

    if((eVideo_Timing == RES_720x576I_50Hz) || (eVideo_Timing == RES_720x576P_50Hz))
    {
        if((eWSS_TYPE >= DAC_VIDEO_VBI_WSS_AS_4x3_FULL) && (eWSS_TYPE <= DAC_VIDEO_VBI_WSS_AS_16x9_ANAMORPHIC))
        {
            switch(eWSS_TYPE)
            {
                default:
                case DAC_VIDEO_VBI_WSS_AS_4x3_FULL:
                    u32WSSData = VIDEO_VBI_WSS_AS_4x3_FULL;
                    break;
                case DAC_VIDEO_VBI_WSS_AS_14x9_LETTERBOX_CENTER:
                    u32WSSData = VIDEO_VBI_WSS_AS_14x9_LETTERBOX_CENTER;
                    break;
                case DAC_VIDEO_VBI_WSS_AS_14x9_LETTERBOX_TOP:
                    u32WSSData = VIDEO_VBI_WSS_AS_14x9_LETTERBOX_TOP;
                    break;
                case DAC_VIDEO_VBI_WSS_AS_16x9_LETTERBOX_CENTER:
                    u32WSSData = VIDEO_VBI_WSS_AS_16x9_LETTERBOX_CENTER;
                    break;
                case DAC_VIDEO_VBI_WSS_AS_16x9_LETTERBOX_TOP:
                    u32WSSData = VIDEO_VBI_WSS_AS_16x9_LETTERBOX_TOP;
                    break;
                case DAC_VIDEO_VBI_WSS_AS_ABOVE16x9_LETTERBOX_CENTER:
                    u32WSSData = VIDEO_VBI_WSS_AS_ABOVE16x9_LETTERBOX_CENTER;
                    break;
                case DAC_VIDEO_VBI_WSS_AS_14x9_FULL_CENTER:
                    u32WSSData = VIDEO_VBI_WSS_AS_14x9_FULL_CENTER;
                    break;
                case DAC_VIDEO_VBI_WSS_AS_16x9_ANAMORPHIC:
                    u32WSSData = VIDEO_VBI_WSS_AS_16x9_ANAMORPHIC;
                    break;
            }

            u32PreWSSData = (MS_U32)Hal_HDGEN_Get_WSS_data();

            if(bEnable == TRUE)
            {
                u32WSSData = (u32PreWSSData & 0x3FF0) | ((EN_VIDEO_VBI_WSS_AS_TYPE)u32WSSData);
            }
            else
            {
                u32WSSData = u32PreWSSData & 0x3FF0;
            }

            Hal_HDGEN_Set_WSS_data(TRUE, eVideo_Timing, u32WSSData);
        }
        else
        {
            printf("[MDRV_DAC]The timing %d does not support WSS %d type!\n", eVideo_Timing, eWSS_TYPE);
            return FALSE;
        }
    }
    else if((eVideo_Timing == RES_720x480I_60Hz) || (eVideo_Timing == RES_720x480P_60Hz) || (eVideo_Timing == RES_1280x720P_50Hz)
             || (eVideo_Timing == RES_1280x720P_60Hz) || (eVideo_Timing == RES_1920x1080I_50Hz) || (eVideo_Timing == RES_1920x1080I_60Hz))
    {
        if((eWSS_TYPE >= DAC_VIDEO_VBI_WSS525_A_AS_4x3_NORMAL) && (eWSS_TYPE <= DAC_VIDEO_VBI_WSS525_A_AS_RESERVED))
        {
            switch(eWSS_TYPE)
            {
                default:
                case DAC_VIDEO_VBI_WSS525_A_AS_4x3_NORMAL:
                    u32WSSData = VIDEO_VBI_WSS525_A_AS_4x3_NORMAL;
                    break;
                case DAC_VIDEO_VBI_WSS525_A_AS_4x3_LETTERBOX:
                    u32WSSData = VIDEO_VBI_WSS525_A_AS_4x3_LETTERBOX;
                    break;
                //case DAC_VIDEO_VBI_WSS525_A_AS_16x9_ANAMORPHIC:
                case DAC_VIDEO_VBI_WSS525_A_AS_16x9_NORMAL:
                    u32WSSData = VIDEO_VBI_WSS525_A_AS_16x9_NORMAL;
                    break;
                case DAC_VIDEO_VBI_WSS525_A_AS_RESERVED:
                    u32WSSData = VIDEO_VBI_WSS525_A_AS_RESERVED;
                    break;
            }

            u32PreWSSData = Hal_HDGEN_Get_WSS525_A_data();

            if(bEnable == TRUE)
            {
                u32WSSData = (u32PreWSSData & 0xFFFFC) | ((EN_VIDEO_VBI_WSS525_A_AS_TYPE)u32WSSData);
            }
            else
            {
                u32WSSData = u32PreWSSData & 0xFFFFC;
            }

            Hal_HDGEN_Set_WSS525_A_data(TRUE, eVideo_Timing, u32WSSData);
        }
        else
        {
            printf("[MDRV_DAC]The timing %d does not support WSS %d type!\n", eVideo_Timing, eWSS_TYPE);
            return FALSE;
        }
    }
    else
    {
        printf("[MDRV_DAC]The timing %d does not implement WSS yet!\n", eVideo_Timing);
        return FALSE;
    }

    return TRUE;
}

MS_BOOL MDrv_DAC_WSS_SetCGMS(MS_BOOL bEnable, EN_OUTPUT_VIDEO_TIMING_TYPE eVideo_Timing, EN_DAC_VIDEO_VBI_WSS_TYPE eWSS_TYPE)
{
    MS_U32 u32PreWSSData = 0;
    MS_U32 u32WSSData = 0;

    if((eVideo_Timing == RES_720x576I_50Hz) || (eVideo_Timing == RES_720x576P_50Hz))
    {
        if((eWSS_TYPE >= DAC_VIDEO_VBI_WSS_CGMS_COPYRIGHT) && (eWSS_TYPE <= DAC_VIDEO_VBI_WSS_CGMS_COPY_PROTECTION))
        {
            switch(eWSS_TYPE)
            {
                default:
                case DAC_VIDEO_VBI_WSS_CGMS_COPYRIGHT:
                    u32WSSData = VIDEO_VBI_WSS_CGMS_COPYRIGHT;
                    break;
                case DAC_VIDEO_VBI_WSS_CGMS_COPY_PROTECTION:
                    u32WSSData = VIDEO_VBI_WSS_CGMS_COPY_PROTECTION;
                    break;
            }

            u32PreWSSData = (MS_U32)Hal_HDGEN_Get_WSS_data();

            if(bEnable == TRUE)
            {
                u32WSSData = u32PreWSSData | ((EN_VIDEO_VBI_WSS_CGMS_TYPE) u32WSSData << 12);
            }
            else
            {
                u32WSSData = u32PreWSSData & (~((EN_VIDEO_VBI_WSS_CGMS_TYPE) u32WSSData << 12));
            }

            Hal_HDGEN_Set_WSS_data(TRUE, eVideo_Timing, u32WSSData);
        }
        else
        {
            printf("[MDRV_DAC]The timing %d does not support WSS %d type!\n", eVideo_Timing, eWSS_TYPE);
            return FALSE;
        }
    }
    else if((eVideo_Timing == RES_720x480I_60Hz) || (eVideo_Timing == RES_720x480P_60Hz) || (eVideo_Timing == RES_1280x720P_50Hz)
             || (eVideo_Timing == RES_1280x720P_60Hz) || (eVideo_Timing == RES_1920x1080I_50Hz) || (eVideo_Timing == RES_1920x1080I_60Hz))
    {
        if((eWSS_TYPE >= DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_FREELY) && (eWSS_TYPE <= DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_NEVER))
        {
            switch(eWSS_TYPE)
            {
                default:
                case DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_FREELY:
                    u32WSSData = VIDEO_VBI_WSS525_A_CGMS_COPY_FREELY;
                    break;
                case DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_ONCE:
                    u32WSSData = VIDEO_VBI_WSS525_A_CGMS_COPY_ONCE;
                    break;
                case DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_NO_MORE:
                    u32WSSData = VIDEO_VBI_WSS525_A_CGMS_COPY_NO_MORE;
                    break;
                case DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_NEVER:
                    u32WSSData = VIDEO_VBI_WSS525_A_CGMS_COPY_NEVER;
                    break;
            }

            u32PreWSSData = Hal_HDGEN_Get_WSS525_A_data();

            if(bEnable == TRUE)
            {
                u32WSSData = (u32PreWSSData & 0xFFF3F) | ((EN_VIDEO_VBI_WSS525_A_CGMS_TYPE) u32WSSData << 6);
            }
            else
            {
                u32WSSData = u32PreWSSData & 0xFFF3F;
            }

            Hal_HDGEN_Set_WSS525_A_data(TRUE, eVideo_Timing, u32WSSData);
        }
        else
        {
            printf("[MDRV_DAC]The timing %d does not support WSS %d type!\n", eVideo_Timing, eWSS_TYPE);
            return FALSE;
        }
    }
    else
    {
        printf("[MDRV_DAC]The timing %d does not implement WSS yet!\n", eVideo_Timing);
        return FALSE;
    }

    return TRUE;
}

MS_BOOL MDrv_DAC_WSS_SetAPS(MS_BOOL bEnable, EN_OUTPUT_VIDEO_TIMING_TYPE eVideo_Timing, EN_DAC_VIDEO_VBI_WSS_TYPE eWSS_TYPE)
{
    MS_U32 u32PreWSSData = 0;
    MS_U32 u32WSSData = 0;

    if((eVideo_Timing == RES_720x480I_60Hz) || (eVideo_Timing == RES_720x480P_60Hz) || (eVideo_Timing == RES_1280x720P_50Hz)
        || (eVideo_Timing == RES_1280x720P_60Hz) || (eVideo_Timing == RES_1920x1080I_50Hz) || (eVideo_Timing == RES_1920x1080I_60Hz))
    {
        if((eWSS_TYPE >= DAC_VIDEO_VBI_WSS525_A_APS_NO_APS) && (eWSS_TYPE <= DAC_VIDEO_VBI_WSS525_A_APS_PSP_4_LINE_CS))
        {
            switch(eWSS_TYPE)
            {
                default:
                case DAC_VIDEO_VBI_WSS525_A_APS_NO_APS:
                    u32WSSData = VIDEO_VBI_WSS525_A_APS_NO_APS;
                    break;
                case DAC_VIDEO_VBI_WSS525_A_APS_PSP_CS_OFF:
                    u32WSSData = VIDEO_VBI_WSS525_A_APS_PSP_CS_OFF;
                    break;
                case DAC_VIDEO_VBI_WSS525_A_APS_PSP_2_LINE_CS:
                    u32WSSData = VIDEO_VBI_WSS525_A_APS_PSP_2_LINE_CS;
                    break;
                case DAC_VIDEO_VBI_WSS525_A_APS_PSP_4_LINE_CS:
                    u32WSSData = VIDEO_VBI_WSS525_A_APS_PSP_4_LINE_CS;
                    break;
            }

            u32PreWSSData = Hal_HDGEN_Get_WSS525_A_data();

            if(bEnable == TRUE)
            {
                u32WSSData = (u32PreWSSData & 0xFFCFF) | ((EN_VIDEO_VBI_WSS525_A_APS_TYPE) u32WSSData << 8);
            }
            else
            {
                u32WSSData = u32PreWSSData & 0xFFCFF;
            }

            Hal_HDGEN_Set_WSS525_A_data(TRUE, eVideo_Timing, u32WSSData);
        }
    }
    else
    {
        printf("[MDRV_DAC]The timing %d does not implement WSS yet!\n", eVideo_Timing);
        return FALSE;
    }

    return TRUE;
}

//----------------------------------------------------------------
// DAC - set half output current
// @return none
//----------------------------------------------------------------
void MDrv_DAC_SetIHalfOutput(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetIHalfOutput(bEnable, bIsYPbPr);
}

//----------------------------------------------------------------
// DAC - set quart output current
// @return none
//----------------------------------------------------------------
void MDrv_DAC_SetQuartOutput(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetQuartOutput(bEnable, bIsYPbPr);
}

//----------------------------------------------------------------
// MDrv_DAC_SetDACState - Set DAC init state
// @param: bEnable: TRUE for DAC is initialized, FALSE for not initialized
// @param: bIsYPbPr: TRUE for YPbPr, FALSE for CVBS
// @return none
//----------------------------------------------------------------
void MDrv_DAC_SetDACState(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetDACState(bEnable, bIsYPbPr);
}

//----------------------------------------------------------------
// MDrv_DAC_GetDACState - Get DAC init state
// @param: bIsYPbPr: TRUE for YPbPr, FALSE for CVBS
// @return: TRUE is DAC is initialized
//----------------------------------------------------------------
MS_BOOL MDrv_DAC_GetDACState(MS_BOOL bIsYPbPr)
{
    return Hal_DAC_GetDACState(bIsYPbPr);
}

//----------------------------------------------------------------
// MDrv_DAC_HotPlugDetect - Get DAC HotPlug state
// @param: SelDAC: DAC_DETECT_HD, DAC_DETECT_SD
// @param: DetectType: DAC_DETECT_PLUGIN, DAC_DETECT_PLUGOUT
// @param: bIsSignIn: Report signal is in/out
// @return: TRUE is working successful
//----------------------------------------------------------------
MS_BOOL MDrv_DAC_HotPlugDetect(EN_DAC_DETECT SelDAC, EN_DAC_DETECT_TYPE DetectType, MS_BOOL *State)
{
    return Hal_DAC_HotPlugDetect(SelDAC, DetectType, State);
}
