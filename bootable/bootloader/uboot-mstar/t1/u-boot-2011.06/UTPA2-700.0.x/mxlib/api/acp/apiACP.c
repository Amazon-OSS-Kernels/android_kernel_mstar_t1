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
/// file   apiACP.c
/// @brief  ACP Api
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"

#include "apiACP.h"
#include "drvDAC.h"
#include "drvTVEncoder.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "utopia.h"
#include "utopia_dapi.h"
#include "apiACP_private.h"
#include "apiACP_v2.h"


VE_Result MDrv_VE_SetMV(MS_BOOL bEnble, MS_U32 eType);

VE_Result MDrv_VE_DCS_SetType(MS_BOOL bEnable, MS_U32 eType);
VE_Result MDrv_VE_DCS_SetActivationKey(MS_U8 *pu8ActivationKeyTbl, MS_U8 u8ActivationKeyTblSize);
//-------------------------------------------------------------------------------------------------
//  local data structures
//-------------------------------------------------------------------------------------------------
void* g_pACPInst;

//MV Type
typedef enum
{
    MS_VE_MV_NTSC_START = 0,
    MS_VE_MV_NTSC_TYPE1 = MS_VE_MV_NTSC_START,
    MS_VE_MV_NTSC_TYPE2,
    MS_VE_MV_NTSC_TYPE3,
    MS_VE_MV_NTSC_TEST_N01,
    MS_VE_MV_NTSC_TEST_N02,
    MS_VE_MV_NTSC_TYPE2_TTX,
    MS_VE_MV_NTSC_TEST_N01_TTX,
    MS_VE_MV_PAL_START = 0x10,
    MS_VE_MV_PAL_TYPE1 = MS_VE_MV_PAL_START,
    MS_VE_MV_PAL_TYPE2,
    MS_VE_MV_PAL_TYPE3,
    MS_VE_MV_PAL_TEST_P01,
    MS_VE_MV_PAL_TEST_P02,
    MS_VE_MV_PAL_TYPE2_TTX,
    MS_VE_MV_PAL_TEST_P01_TTX,
} MS_VE_MV_TYPE;


/*
 * This Enum type was defined base on EncoderICTest_Forms Rev1_3_K2_0819a.doc, ROVI CONFIDENTIAL
 * - PAL denotes signal standards PAL-B, -D, -G, -H, -I, -K, -L
 * - NTSC TYPE 1/2/3 include NTSC-M, NTSC-J, PAL-M and NTSC-443
 * - PAL TYPE 1/2/3 include PAL, PAL-N, PAL-Nc and SECAM
 */
typedef enum
{
    MS_ACP_CVBS_NTSC_TYPE1 = 0,         //NTSC, AGC only (Table2)
    MS_ACP_CVBS_NTSC_TYPE2,             //NTSC, AGC + 2-line Colorstripe (Table2)
    MS_ACP_CVBS_NTSC_TYPE3,             //NTSC, AGC + 4-line Colorstripe (Table2)
    MS_ACP_CVBS_NTSC_TEST_N01,          //NTSC, RGB Copy Protect on (N01) (Table2)
    MS_ACP_CVBS_NTSC_TEST_N02,          //NTSC, RGB Copy Protect on (N02) (Table2)
    MS_ACP_CVBS_NTSC_TYPE2_TTX,         //NTSC, AGC + 2-line Colorstripe (Table2) with TTX compatibility
    MS_ACP_CVBS_NTSC_TEST_N01_TTX,      //NTSC, RGB Copy Protect on (N01) (Table2) with TTX compatibility
    MS_ACP_CVBS_PAL_TYPE1 = 0x10,       //PAL, Type1, BPP = 0x40 (Table2)
    MS_ACP_CVBS_PAL_TYPE2,              //PAL, Type2, BPP = 0x60 (Table2)
    MS_ACP_CVBS_PAL_TYPE3,              //PAL, Type3, BPP = 0x50 (Table2)
    MS_ACP_CVBS_PAL_TEST_P01,           //PAL, RGB Copy Protect on (P01) (Table2)
    MS_ACP_CVBS_PAL_TEST_P02,           //PAL, RGB Copy Protect on (P02) (Table2)
    MS_ACP_CVBS_PAL_TYPE2_TTX,          //PAL, Type2, BPP = 0x60 (Table2) with TTX compatibility
    MS_ACP_CVBS_PAL_TEST_P01_TTX,       //PAL, RGB Copy Protect on (P01) (Table2) with TTX compatibility
    MS_ACP_COMPONENT_480I_TYPE2 = 0x20, //YPbPr, NTSC-Type2:700/300 Levels (Table4)
    MS_ACP_COMPONENT_480I_TYPE3,        //YPbPr, NTSC-Type3:714/286 Levels (Table4)
    MS_ACP_COMPONENT_480P_EIA_7701,     //YPbPr, 525P EIA-770.1 (Table6)
    MS_ACP_COMPONENT_480P_EIA_7702,     //YPbPr, 525P EIA-770.2 (Table6)
    MS_ACP_COMPONENT_576I_TEST_P01,     //YPbPr, Test P01 (Table8)
    MS_ACP_COMPONENT_576I_TEST_P02,     //YPbPr, Test P02 (Table8)
    MS_ACP_COMPONENT_576P,              //YPbPr, 625P (Table9)
    MS_ACP_SVIDEO_NTSC_TYPE2 = 0x40,    //S-video, NTSC-Type2 (Table4)
    MS_ACP_SVIDEO_NTSC_TYPE3,           //S-video, NTSC-Type3 (Table4)
    MS_ACP_SVIDEO_PAL_TEST_P01,         //S-video, PAL Test P01 (Table8)
    MS_ACP_SVIDEO_PAL_TEST_P02,         //S-video, PAL Test P02 (Table8)
    MS_ACP_TYPE_MAX,
} MS_ACP_TYPE;

/*
 * This Enum type was defined base on DCS for composite (CVBS) and component outputs
 */
typedef enum
{
    MS_ACP_DCS_TYPE_MIN = 0,
    MS_ACP_DCS_CVBS_NTSC = MS_ACP_DCS_TYPE_MIN,
    MS_ACP_DCS_CVBS_PAL,
    MS_ACP_DCS_TYPE_MAX = MS_ACP_DCS_CVBS_PAL,
    MS_ACP_DCS_TYPE_NUM,
} MS_ACP_DCS_TYPE;

//----------------------------------------------------------------
// MApi_ACP_SetProtection
// enable/disable protection for composite (CVBS) and component outputs
// @bEnabled: TRUE: enabled, FALSE: not enabled
// @bIsYPbPr: TRUE: component, FALSE: CVBS
// @u8Type: protection type
// @return ACP_Result
//----------------------------------------------------------------
ACP_Result MApi_ACP_SetProtection_U2(MS_BOOL bEnable, MS_BOOL bIsYPbPr, MS_U8 u8Type)
{
    MS_S32 ret = 0;
    MS_ACP_TYPE eType = (MS_ACP_TYPE)u8Type;
    MS_VE_MV_TYPE eVeMVtype = MS_VE_MV_NTSC_START;

    if (bIsYPbPr)
    {
        // component protection, not implemented
        printf("component Macrovision is not ready now! \n");
        ret = E_ACP_FAIL;
    }
    else
    {
        // CVBS protection
        eVeMVtype = (MS_VE_MV_TYPE)eType;
        ret = MDrv_VE_SetMV(bEnable, eVeMVtype);
        if (ret == E_VE_OK)
        {
            ret = E_ACP_OK;
        }
        else if (ret == E_VE_NOT_SUPPORT)
        {
            ret = E_ACP_NOT_SUPPORT;
            if (bEnable)
            {
                goto chip_not_support;
            }
        }
        else
        {
            ret = E_ACP_FAIL;
        }
    }

    return (ACP_Result)ret;

chip_not_support:
    // disconnect output signals (CVBS and component) in this case
    printf("chip-not-support error\n");
    MDrv_DAC_Enable(FALSE, TRUE);
    MDrv_DAC_Enable(FALSE, FALSE);
    return (ACP_Result)ret;
}

ACP_Result MApi_ACP_SetProtection(MS_BOOL bEnable, MS_BOOL bIsYPbPr, MS_U8 u8Type)
{
    if (g_pACPInst == NULL)
    {
        if(UtopiaOpen(MODULE_ACP, &g_pACPInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen ACP failed\n");
    		return FALSE;
        }
    }

    stACP_SET_PROTECTION ACPArgs;
    ACPArgs.bEnable = bEnable;
    ACPArgs.bIsYPbPr = bIsYPbPr;
    ACPArgs.u8Type = u8Type;
    ACPArgs.eReturnValue = E_ACP_FAIL;

    if(UtopiaIoctl(g_pACPInst, E_ACP_SET_PROTECTION, (void*)&ACPArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return ACPArgs.eReturnValue;
}

//----------------------------------------------------------------
// MApi_DCS_SetProtection
// enable/disable DCS protection for composite (CVBS)
// @bEnabled: TRUE: enabled, FALSE: not enabled
// @u8Type: protection type
// @return ACP_Result
//----------------------------------------------------------------
ACP_Result MApi_DCS_SetProtection_U2(MS_BOOL bEnable, MS_U8 u8Type)
{
    MS_S32 s32Ret = FALSE;
    MS_ACP_DCS_TYPE eType = (MS_ACP_DCS_TYPE)u8Type;

    s32Ret = MDrv_VE_DCS_SetType(bEnable, eType);
    if(s32Ret > E_VE_OK || s32Ret < E_VE_NOT_SUPPORT)
    {
        return E_ACP_FAIL;
    }

    return (ACP_Result)s32Ret;
}

ACP_Result MApi_DCS_SetProtection(MS_BOOL bEnable, MS_U8 u8Type)
{
    if (g_pACPInst == NULL)
    {
        if(UtopiaOpen(MODULE_ACP, &g_pACPInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen ACP failed\n");
    		return FALSE;
        }
    }

    stACP_DCS_PROTECTION ACPArgs;
    ACPArgs.bEnable = bEnable;
    ACPArgs.u8Type = u8Type;
    ACPArgs.eReturnValue = E_ACP_FAIL;

    if(UtopiaIoctl(g_pACPInst, E_ACP_DCS_PROTECTION, (void*)&ACPArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return ACPArgs.eReturnValue;
}

//----------------------------------------------------------------
// MApi_DCS_SetActivationKey
// DCS protection Activation Key Setting By External
// @pu8ActivationKeyTbl: External Activation Key Table
// @u8ActivationKeyTblSize: External Activation Key Table Size
// @return ACP_Result
//----------------------------------------------------------------
ACP_Result MApi_DCS_SetActivationKey_U2(MS_U8 *pu8ActivationKeyTbl, MS_U8 u8ActivationKeyTblSize)
{
    MS_S32 s32Ret = FALSE;

    s32Ret = MDrv_VE_DCS_SetActivationKey(pu8ActivationKeyTbl, u8ActivationKeyTblSize);
    if(s32Ret > E_ACP_OK || s32Ret < E_ACP_NOT_SUPPORT)
    {
        return E_ACP_FAIL;
    }

    return (ACP_Result)s32Ret;
}

ACP_Result MApi_DCS_SetActivationKey(MS_U8 *pu8ActivationKeyTbl, MS_U8 u8ActivationKeyTblSize)
{
    if (g_pACPInst == NULL)
    {
        if(UtopiaOpen(MODULE_ACP, &g_pACPInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen ACP failed\n");
    		return FALSE;
        }
    }

    stACP_DCS_SETACTIVATIONKEY ACPArgs;
    ACPArgs.pu8ActivationKeyTbl = pu8ActivationKeyTbl;
    ACPArgs.u8ActivationKeyTblSize = u8ActivationKeyTblSize;
    ACPArgs.eReturnValue = E_ACP_FAIL;

    if(UtopiaIoctl(g_pACPInst, E_ACP_DCS_SETACTIVATIONKEY, (void*)&ACPArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return ACPArgs.eReturnValue;
}

