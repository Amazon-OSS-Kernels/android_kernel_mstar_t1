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
#define MHAL_PQ_CUS_C


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"

// Internal Definition
#include "hwreg_utility2.h"
#include "color_reg.h"

#include "drvPQ_Define.h"
#include "Maxim_Main.h"              // table config parameter
#include "Maxim_Sub.h"              // table config parameter
#include "drvPQ_Datatypes.h"
#include "mhal_pq.h"

#include "drvXC_IOPort.h"
#include "apiXC.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifndef UNUSED //to avoid compile warnings...
#define UNUSED(var) (void)((var) = (var))
#endif

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

MS_U16 Hal_PQ_get_420_cup_idx(void *pInstance,MS_420_CUP_TYPE etype, PQ_WIN enWin)
{
    MS_U16 u16ret = 0xFFFF;

#if PQ_ENABLE_PIP
    if(enWin == PQ_SUB_WINDOW)
    {
        switch(etype)
        {
            case MS_420_CUP_OFF:
            u16ret = PQ_IP_420CUP_OFF_Sub;
            break;

            case MS_420_CUP_ON:
            u16ret = PQ_IP_420CUP_ON_Sub;
            break;

            default:
            u16ret = 0xFFFF;
            break;
        }
    }
    else
#else
    UNUSED(enWin);
#endif
    {
        switch(etype)
        {
            case MS_420_CUP_OFF:
            u16ret = PQ_IP_420CUP_OFF_Main;
            break;

            case MS_420_CUP_ON:
            u16ret = PQ_IP_420CUP_ON_Main;
            break;

            default:
            u16ret = 0xFFFF;
            break;
        }
    }
    return u16ret;
}

MS_U16 Hal_PQ_get_422To444_idx(void *pInstance,MS_422To444_TYPE etype, PQ_WIN enWin)
{
    MS_U16 u16ret = 0xFFFF;
#if PQ_ENABLE_PIP
    if(enWin == PQ_SUB_WINDOW)
    {
        switch(etype)
        {
        case MS_422To444_ON:
            u16ret = PQ_IP_422To444_ON_Sub;
            break;
        case MS_422To444_OFF:
            u16ret = PQ_IP_422To444_OFF_Sub;
            break;

        default:
            u16ret = 0xFFFF;
            printf("Use default 422To444 idx: %d\n", u16ret );
            break;
        }
    }
    else
#else
    UNUSED(enWin);
#endif
    {
        switch(etype)
        {
        case MS_422To444_ON:
            u16ret = PQ_IP_422To444_ON_Main;
            break;
        case MS_422To444_OFF:
            u16ret = PQ_IP_422To444_OFF_Main;
            break;

        default:
            u16ret = 0xFFFF;
            printf("Use default 422To444 idx: %d\n", u16ret );
            break;
        }
    }
    return u16ret;
}

MS_U8 Hal_PQ_get_422To444_mode(void *pInstance,MS_BOOL bMemFmt422)
{
    if(bMemFmt422)
    {
        return MS_422To444_ON;
    }
    else
    {
        return MS_422To444_OFF;
    }
}

MS_U16 Hal_PQ_get_madi_idx(void *pInstance,MS_BOOL bMainWin, MS_MADI_TYPE etype)
{
    MS_U16 u16ret = 0xFFFF;

    if(bMainWin == FALSE)
    {
        switch(etype)
        {
        case MS_MADI_24_4R:
            u16ret = PQ_IP_MADi_24_4R_Sub;
            break;
        case MS_MADI_24_2R:
            u16ret = PQ_IP_MADi_24_2R_Sub;
            break;
        case MS_MADI_25_4R:
            u16ret = PQ_IP_MADi_25_4R_Sub;
            break;
        case MS_MADI_25_2R:
            u16ret = PQ_IP_MADi_25_2R_Sub;
            break;
        case MS_MADI_27_4R:
            u16ret = PQ_IP_MADi_27_4R_Sub;
            break;
        case MS_MADI_27_2R:
            u16ret = PQ_IP_MADi_27_2R_Sub;
            break;
        case MS_MADI_P_MODE8:
            u16ret = PQ_IP_MADi_P_MODE8_Sub;
            break;
        case MS_MADI_P_MODE10:
            u16ret = PQ_IP_MADi_P_MODE10_Sub;
            break;
        case MS_MADI_P_MODE_MOT10:
            u16ret = PQ_IP_MADi_P_MODE_MOT10_Sub;
            break;
        case MS_MADI_P_MODE_MOT8:
            u16ret = PQ_IP_MADi_P_MODE_MOT8_Sub;
            break;
        case MS_MADI_P_MODE10_444:
            u16ret = PQ_IP_MADi_P_MODE10_444_Sub;
            break;
        case MS_MADI_P_MODE8_444:
            u16ret = PQ_IP_MADi_P_MODE8_444_Sub;
            break;
#if 0
        case MS_MADi_25_4R_M:
            u16ret = PQ_IP_MADi_25_4R_M_Sub;
            break;
        case MS_MADi_P_MODE10_M_444:
            u16ret = PQ_IP_MADi_P_MODE10_444_M_Sub;
            break;
        case MS_MADi_P_MODE_MOT10_M:
            u16ret = PQ_IP_MADi_P_MODE_MOT10_M_Sub;
            break;
        case MS_MADi_P_MODE_MOT10_M_4K:
            u16ret = PQ_IP_MADi_P_MODE_MOT10_M_4K_Sub;
            break;
#endif
        default:
            u16ret = 0xFFFF;
            //MS_ASSERT(0);
            break;
        }
    }
    else
    {
        switch(etype)
        {
        case MS_MADI_25_4R:
            u16ret = PQ_IP_MADi_25_4R_Main;
            break;
        case MS_MADI_25_4R_MC:
            u16ret = PQ_IP_MADi_25_4R_MC_Main;
            break;
        case MS_MADI_25_6R_MC:
            u16ret = PQ_IP_MADi_25_6R_MC_Main;
            break;
        #if 0
        case MS_MADi_25_8F_8R_MC:                      //Add New MADi mode
            u16ret = PQ_IP_MADi_25_8F_8R_MC_Main;
            break;
        #endif
        case MS_MADI_25_14F_6R_MC:                     //Add New MADi mode
            u16ret = PQ_IP_MADi_25_14F_6R_MC_Main;
            break;
        case MS_MADI_25_16F_6R_MC:                     //Add New MADi mode
            u16ret = PQ_IP_MADi_25_16F_6R_MC_Main;
            break;
        case MS_MADI_25_2R:
            u16ret = PQ_IP_MADi_25_2R_Main;
            break;
        case MS_MADI_27_4R:
            u16ret = PQ_IP_MADi_27_4R_Main;
            break;
        case MS_MADI_27_2R:
            u16ret = PQ_IP_MADi_27_2R_Main;
            break;
        case MS_MADI_P_MODE8:
            u16ret = PQ_IP_MADi_P_MODE8_Main;
            break;
        case MS_MADI_P_MODE10:
            u16ret = PQ_IP_MADi_P_MODE10_Main;
            break;
        case MS_MADI_P_MODE_MOT10:
            u16ret = PQ_IP_MADi_P_MODE_MOT10_Main;
            break;

        case MS_MADI_P_MODE_MC_3Frame:
            u16ret = PQ_IP_MADi_P_MODE_MC_3Frame_Main;
            break;

        case MS_MADI_P_MODE_MC_4Frame:
            u16ret = PQ_IP_MADi_P_MODE_MC_4Frame_Main;
            break;

        case MS_MADI_P_MODE_MC_6Frame_6R:
            u16ret = PQ_IP_MADi_P_MODE_MC_6Frame_6R_Main;
            break;
        case MS_MADI_P_MODE_MC_6Frame:
            u16ret = PQ_IP_MADi_P_MODE_MC_6Frame_Main;
            break;
        #if 1
        case MS_MADI_P_MODE_P_MC_12Frame_8R:
            u16ret = PQ_IP_MADi_P_MODE_P_MC_12Frame_8R_Main;
        break;

        case MS_MADI_P_MODE_N_MC_12Frame_8R:
            u16ret = PQ_IP_MADi_P_MODE_N_MC_12Frame_8R_Main;
        break;
        #endif

        case MS_MADI_P_MODE_MOT8:
            u16ret = PQ_IP_MADi_P_MODE_MOT8_Main;
            break;

        case MS_MADI_P_MODE_MC_12Frame_8R:
            u16ret = PQ_IP_MADi_P_MODE_MC_12Frame_8R_Main;
            break;

        case MS_MADI_24_4R:
            u16ret = PQ_IP_MADi_24_4R_Main;
            break;

        case MS_MADI_24_2R:
            u16ret = PQ_IP_MADi_24_2R_Main;
            break;

        case MS_MADI_26_4R:
            u16ret = PQ_IP_MADi_26_4R_Main;
            break;

        case MS_MADI_26_2R:
            u16ret = PQ_IP_MADi_26_2R_Main;
            break;

        case MS_MADI_24_4R_880:
            u16ret = PQ_IP_MADi_24_4R_880_Main;
            break;

        case MS_MADI_24_2R_880:
            u16ret = PQ_IP_MADi_24_2R_880_Main;
            break;

        case MS_MADI_25_4R_884:
            u16ret = PQ_IP_MADi_25_4R_884_Main;
            break;

        case MS_MADI_25_4R_880:
            u16ret = PQ_IP_MADi_25_4R_880_Main;
            break;

        case MS_MADI_25_2R_884:
            u16ret = PQ_IP_MADi_25_2R_884_Main;
            break;

        case MS_MADI_25_2R_880:
            u16ret = PQ_IP_MADi_24_2R_880_Main;
            break;
        case MS_MADI_25_4R_MC_NW:
            u16ret = PQ_IP_MADi_25_4R_MC_NW_Main;
            break;
        case MS_MADI_25_6R_MC_NW:
            u16ret = PQ_IP_MADi_25_6R_MC_NW_Main;
            break;

        case MS_MADI_25_12F_8R_MC:
            u16ret = PQ_IP_MADi_25_12F_8R_MC_Main;
            break;

        case MS_MADI_25_14F_8R_MC:
            u16ret = PQ_IP_MADi_25_14F_8R_MC_Main;
            break;

        case MS_MADI_25_16F_8R_MC:
            u16ret = PQ_IP_MADi_25_16F_8R_MC_Main;
            break;

        case MS_MADI_P_MODE8_444:
        	u16ret = PQ_IP_MADi_P_MODE8_444_Main;
        	break;
        case MS_MADI_P_MODE10_444:
        	u16ret = PQ_IP_MADi_P_MODE10_444_Main;
        	break;
        case MS_MADI_P_MODE_MOT10_8Frame:
        	u16ret = PQ_IP_MADi_P_MODE_MOT10_8Frame_Main;
        	break;
        case MS_MADI_P_MODE_MOT10_4Frame:
        	u16ret = PQ_IP_MADi_P_MODE_MOT10_4Frame_Main;
        	break;
        case MS_MADI_RFBL_3D:
        	u16ret = PQ_IP_MADi_RFBL_3D_Main;
        	break;

        case MS_MADI_RFBL_P_MODE8_MOT:
            u16ret = PQ_IP_MADi_RFBL_P_MODE8_MOT_Main;
            break;

        case MS_MADI_RFBL_P_MODE10_MOT:
            u16ret = PQ_IP_MADi_RFBL_P_MODE10_MOT_Main;
            break;

        case MS_MADI_P_MODE8_2BYTE_3D:
            u16ret = PQ_IP_MADi_P_MODE8_2BYTE_3D_Main;
            break;

#if 0
        case MS_MADI_P_MODE_MC_8Frame_6R:
            u16ret = PQ_IP_MADi_P_MODE_MC_8Frame_6R_Main;
        break;

        case MS_MADI_P_MODE_MC_16Frame_8R:
            u16ret = PQ_IP_MADi_P_MODE_MC_16Frame_8R_Main;
        break;

        case MS_MADi_25_4R_M_MC:
            u16ret = PQ_IP_MADi_25_4R_M_MC_Main;
        break;

        case MS_MADi_25_16F_8R_M_MC:
            u16ret = PQ_IP_MADi_25_16F_8R_M_MC_Main;
        break;

        case MS_MADi_25_16F_6R_M_MC:
            u16ret = PQ_IP_MADi_25_16F_6R_M_MC_Main;
        break;

        case MS_MADi_P_MODE8_M_444:
            u16ret = PQ_IP_MADi_P_MODE8_M_444_Main;
        break;

        case MS_MADi_P_MODE_MC_M_4F:
            u16ret = PQ_IP_MADi_P_MODE_MC_M_4F_Main;
        break;

        case MS_MADi_P_MODE10_M_444:
            u16ret = PQ_IP_MADi_P_MODE10_M_444_Main;
        break;

        case MS_MADi_P_MODE10_M_444_4K:
            u16ret = PQ_IP_MADi_P_MODE10_M_444_4K_Main;
        break;

        case MS_MADi_P_MODE8_M:
            u16ret = PQ_IP_MADi_P_MODE8_M_Main;
        break;

        case MS_MADi_P_MODE10_M:
            u16ret = PQ_IP_MADi_P_MODE10_M_Main;
        break;

        case MS_MADi_25_4R_M:
            u16ret = PQ_IP_MADi_25_4R_M_Main;
        break;

        case MS_MADi_P_MODE_MOT10_M:
            u16ret = PQ_IP_MADi_P_MODE_MOT10_M_Main;
        break;

        case MS_MADi_P_MODE_MOT8_M:
            u16ret = PQ_IP_MADi_P_MODE_MOT8_M_Main;
        break;

        case MS_MADi_P_MODE_MOT10_M_4K:
            u16ret = PQ_IP_MADi_P_MODE_MOT10_M_4K_Main;
        break;

        case MS_MADi_P_MODE_MC_12Frame_8R_M:
            u16ret = PQ_IP_MADi_P_MODE_MC_12F_8R_M_Main;
        break;

        case MS_MADi_P_MODE_MC_8Frame_8R_M:
            u16ret = PQ_IP_MADi_P_MODE_MC_8F_8R_M_Main;
        break;
        #endif
        case MS_MADi_P_MODE_MOT10_NR_Yonly_M:
            u16ret = PQ_IP_MADi_P_MODE_MOT10_NR_Yonly_Main;
        break;
        case MS_MADi_P_MODE_MOT10_NO_DNR:
            u16ret = PQ_IP_MADi_P_MODE_MOT10_NO_DNR_Main;
        break;
        default:
            u16ret = 0xFFFF;
            //MS_ASSERT(0);
            break;
        }
    }
    return u16ret;
}

MS_U16 Hal_PQ_get_csc_ip_idx(void *pInstance,MS_CSC_IP_TYPE enCSC, PQ_WIN enWin)
{
    MS_U16 u16Ret = 0xFFFF;

#if PQ_ENABLE_PIP
    if(enWin == PQ_SUB_WINDOW)
    {
        switch(enCSC)
        {
            case MS_CSC_IP_CSC:
            u16Ret = PQ_IP_CSC_Sub;
            break;

            default:
            case MS_CSC_IP_VIP_CSC:
            u16Ret = PQ_IP_VIP_CSC_Sub;
            break;
        }
    }
    else
#else
    UNUSED(enWin);
#endif
    {
        switch(enCSC)
        {
            case MS_CSC_IP_CSC:
            u16Ret = PQ_IP_CSC_Main;
            break;

            default:
            case MS_CSC_IP_VIP_CSC:
            u16Ret = PQ_IP_VIP_CSC_Main;
            break;
        }
    }
    return u16Ret;
}

MS_U16 Hal_PQ_get_ip_idx(void *pInstance,MS_PQ_IP_TYPE eType, PQ_WIN enWin)
{
    MS_U16 u16ret = 0xFFFF;

#if PQ_ENABLE_PIP
    if(enWin == PQ_SUB_WINDOW)
    {
        switch(eType)
        {
        case MS_PQ_IP_VD_SAMPLING:
            u16ret = PQ_IP_VD_Sampling_no_comm_Sub;
            break;
        case MS_PQ_IP_HSD_SAMPLING:
        case MS_PQ_IP_ADC_SAMPLING:
        default:
            u16ret = 0xFFFF;
            break;
        }
    }
    else
#else
    UNUSED(enWin);
#endif
    {
        switch(eType)
        {
        case MS_PQ_IP_VD_SAMPLING:
            u16ret = PQ_IP_VD_Sampling_no_comm_Main;
            break;
        case MS_PQ_IP_HSD_SAMPLING:
            u16ret = PQ_IP_HSD_Sampling_Main;
            break;
        case MS_PQ_IP_ADC_SAMPLING:
            u16ret = PQ_IP_ADC_Sampling_Main;
            break;
        default:
            u16ret = 0xFFFF;
            break;
        }
    }
    return u16ret;
}

MS_U16 Hal_PQ_get_hsd_sampling_idx(void *pInstance,MS_HSD_SAMPLING_TYPE eType, PQ_WIN enWin)
{
    MS_U16 u16ret = 0xFFFF;
#if PQ_ENABLE_PIP
    if(enWin == PQ_SUB_WINDOW)
    {
        switch(eType)
        {
        case MS_HSD_SAMPLING_Div_1_000:
        case MS_HSD_SAMPLING_Div_1_125:
        case MS_HSD_SAMPLING_Div_1_250:
        case MS_HSD_SAMPLING_Div_1_375:
        case MS_HSD_SAMPLING_Div_1_500:
        case MS_HSD_SAMPLING_Div_1_625:
        case MS_HSD_SAMPLING_Div_1_750:
        case MS_HSD_SAMPLING_Div_1_875:
        case MS_HSD_SAMPLING_Div_2_000:
        default:
            u16ret = 0xFFFF;
            break;
        }
    }
    else
#else
    UNUSED(enWin);
#endif
    {
        switch(eType)
        {
        case MS_HSD_SAMPLING_Div_1_000:
            u16ret = PQ_IP_HSD_Sampling_Div_1o000_Main;
            break;

        case MS_HSD_SAMPLING_Div_1_125:
            u16ret = PQ_IP_HSD_Sampling_Div_1o125_Main;
            break;

        case MS_HSD_SAMPLING_Div_1_250:
            u16ret = PQ_IP_HSD_Sampling_Div_1o250_Main;
            break;

        case MS_HSD_SAMPLING_Div_1_375:
            u16ret = PQ_IP_HSD_Sampling_Div_1o375_Main;
            break;

        case MS_HSD_SAMPLING_Div_1_500:
            u16ret = PQ_IP_HSD_Sampling_Div_1o500_Main;
            break;

        case MS_HSD_SAMPLING_Div_1_625:
            u16ret = PQ_IP_HSD_Sampling_Div_1o625_Main;
            break;

        case MS_HSD_SAMPLING_Div_1_750:
            u16ret = PQ_IP_HSD_Sampling_Div_1o750_Main;
            break;

        case MS_HSD_SAMPLING_Div_1_875:
            u16ret = PQ_IP_HSD_Sampling_Div_1o875_Main;
            break;

        case MS_HSD_SAMPLING_Div_2_000:
            u16ret = PQ_IP_HSD_Sampling_Div_2o000_Main;
            break;

        default:
            u16ret = 0xFFFF;
            break;
        }
    }
    return u16ret;
}

MS_U16 Hal_PQ_get_adc_sampling_idx(void *pInstance,MS_ADC_SAMPLING_TYPE eType, PQ_WIN enWin)
{
    MS_U16 u16ret = 0xFFFF;
#if PQ_ENABLE_PIP
    if(enWin == PQ_SUB_WINDOW)
    {
        switch(eType)
        {
        case MS_ADC_SAMPLING_X_1:
        case MS_ADC_SAMPLING_X_2:
        case MS_ADC_SAMPLING_X_4:
        default:
            u16ret = 0xFFFF;
            break;
        }
    }
    else
#else
    UNUSED(enWin);
#endif
    {
        switch(eType)
        {
        case MS_ADC_SAMPLING_X_1:
            u16ret = PQ_IP_ADC_Sampling_x1_Main;
            break;

        case MS_ADC_SAMPLING_X_2:
            u16ret = PQ_IP_ADC_Sampling_x2_Main;
            break;
        case MS_ADC_SAMPLING_X_4:
            u16ret = PQ_IP_ADC_Sampling_x4_Main;
            break;
        default:
            u16ret = 0xFFFF;
            break;
        }
    }
    return u16ret;
}
