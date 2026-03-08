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
//==============================================================================
#ifndef MHAL_PQ_ADAPTIVE_H
#define MHAL_PQ_ADAPTIVE_H

#ifdef __cplusplus
extern "C"
{
#endif

    /******************************************************************************/
    /*                     definition                                             */
    /* ****************************************************************************/
#define MSIF_ADAPTIVE_LIB_CODE_H            {'A','D','A','P','T','I','V','E','_'}
#define MSIF_ADAPTIVE_VERSION_H             {'0','0','0','0','0','1'}



#ifdef DRV_SCALER_NR_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//define print for test run procedure
#ifdef CES_DBG
#define PRT_TRP sd_print
#else
#define PRT_TRP printf
#endif

#define MFC_Debug_ENable 0
    //-------------------------------------------------------------------------------------------------
    //  Local Defines
    //-------------------------------------------------------------------------------------------------
#define DRVSCA_DBG(x)       //x
#define DBG_DEFEATHERING    0
#define DBG_DEFLICKERING    0
#define DBG_DEBOUNCING      0
#define DBG_DYNAMIC_DNR     0

    // DeFeathering
#define MDP_CNT     2
#define ENABLE_MDP  0   //Motion decrase progressively enable

#define DEFETHERING_LV1_TH              30000
#define DEFETHERING_LV2_TH              5000
#define DEFETHERING_LV3_TH              2000
#define DEFETHERING_LV1_CNT             25
#define DEFETHERING_LV2_CNT             20
#define DEFETHERING_LV3_CNT             5

#define SST_STATIC_CORE_TH_LV1_VALUE    0x14              //ryan update
#define SST_STATIC_CORE_TH_LV2_VALUE    0x0C              //ryan update
#define SST_STATIC_CORE_TH_LV3_VALUE    0x06              //ryan update
#define SST_STATIC_CORE_TH_LV4_VALUE    0x00              //ryan update
#define SST_MOTION_TH_LV1_VALUE         0x03              //YY for SZ feathering 151201
#define SST_MOTION_TH_LV2_VALUE         0x03              //YY for SZ feathering 151201
#define SST_MOTION_TH_LV3_VALUE         0x02              //YY for SZ feathering 151201
#define SST_MOTION_TH_LV4_VALUE         0x01              //YY for SZ feathering 151201

#define SST_DYNAMIC_CORE_TH_LV1_VALUE   0x0F              //YY for SZ feathering 151201
#define SST_DYNAMIC_CORE_TH_LV2_VALUE   0x18              //YY for SZ feathering 151201
#define SST_DYNAMIC_CORE_TH_LV3_VALUE   0x1F              //YY for SZ feathering 151201
#define SST_DYNAMIC_CORE_TH_LV4_VALUE   0x3F              //YY for SZ feathering 151201

    // DeFlickering
#define DEFLICKERING_TH                 52000
#define DEFLICKERING_CNT                150

    // DeBouncing
#define DEBOUNCING_TH                   35000
#define DEBOUNCING_CNT                  10

    // Dynamic SNR
#define DYNAMIC_SNR_TH                  2000
#define DYNAMIC_SNR_CNT                 30

    // Dynamic DNR
    //#define DYNAMIC_DNR_TH                  6000

#define DNR_TABLEY_0L_Zero_VALUE        0xDD
#define DNR_TABLEY_0H_Zero_VALUE        0xBD
#define DNR_TABLEY_1L_Zero_VALUE        0x79
#define DNR_TABLEY_1H_Zero_VALUE        0x35
#define DNR_TABLEY_2L_Zero_VALUE        0x11
#define DNR_TABLEY_2H_Zero_VALUE        0x00
#define DNR_TABLEY_3L_Zero_VALUE        0x00
#define DNR_TABLEY_3H_Zero_VALUE        0x00

#define DNR_TABLEY_0L_LV2_VALUE         0xCC
#define DNR_TABLEY_0H_LV2_VALUE         0xAC
#define DNR_TABLEY_1L_LV2_VALUE         0x68
#define DNR_TABLEY_1H_LV2_VALUE         0x24
#define DNR_TABLEY_2L_LV2_VALUE         0x00
#define DNR_TABLEY_2H_LV2_VALUE         0x00
#define DNR_TABLEY_3L_LV2_VALUE         0x00
#define DNR_TABLEY_3H_LV2_VALUE         0x00

#define DNR_TABLEY_0L_LV3_VALUE         0x67
#define DNR_TABLEY_0H_LV3_VALUE         0x45
#define DNR_TABLEY_1L_LV3_VALUE         0x33
#define DNR_TABLEY_1H_LV3_VALUE         0x22
#define DNR_TABLEY_2L_LV3_VALUE         0x11
#define DNR_TABLEY_2H_LV3_VALUE         0x00
#define DNR_TABLEY_3L_LV3_VALUE         0x00
#define DNR_TABLEY_3H_LV3_VALUE         0x00

    // Dynamic Film 22
#define DYNAMIC_FILM22_TH               520000

#define PRJ_MCNR                        TRUE

#define REG_LPLL_11_L                   (0x103100 | 0x11<< 1)
#define REG_LPLL_11_H                   (0x103100 | ((0x11<< 1)+1))
#define REG_LPLL_13_L                   (0x103100 | 0x13<< 1)
#define REG_LPLL_13_H                   (0x103100 | ((0x13<< 1)+1))

    //-------------------------------------------------------------------------------------------------
    //  Local Structurs
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

    /******************************************************************************/
    /*                           Global Variables                                 */
    /******************************************************************************/

    //*************************************************************************
    //          Enums
    //*************************************************************************


    //*************************************************************************
    //          Structures
    //*************************************************************************

    typedef struct
    {
        MS_U8 minMvXdir;
        MS_U8 minMvXBin;
        MS_U8 minMvYdir;
        MS_U8 minMvYBin;
    }XC_Me1HisStates;

    typedef struct
    {
        MS_U8 minMvXdir;
        MS_U8 minMvYdir;
        MS_U32 minMvXbin;
        MS_U32 minMvYbin;
        MS_U32 h1XYsum;
        MS_U32 h2XYsum;
        MS_U32 h1XYdiff;
        MS_U32 h2XYdiff;
        MS_BOOL yMvIsMovingOdd;
    }XC_GMvStates;

    typedef struct
    {
        MS_BOOL film32;
        MS_BOOL film22;
        MS_BOOL filmany;
        MS_BOOL film_act;
    }XC_FilmStates;

    typedef struct
    {
        MS_U8 curOpFrame;
        MS_U8 maxOpFrame;

    }XC_OpFrameStates;

    typedef struct
    {
        MS_U16 filmLikePrvPixels;
        MS_U16 filmLikeNxtPixels;
        MS_BOOL vaildStates;
    }XC_McFilmStates;

    typedef struct
    {
        MS_U16 lbCntT;
        MS_U16 lbCntB;
        MS_U16 lbCntL;
        MS_U16 lbCntR;
        MS_U8 lbRatioC;
        MS_U8 lbRatioTB;
        MS_U8 lbRatioLR;
        MS_U8 lbComfirmLevel;
    }XC_LetterBoxState;

    typedef struct
    {
        MS_U16 motion[16];
        MS_U16 unmatch_1[16];
        MS_U16 unmatch_3[16];
        MS_U8 vaildCnt[16];
        MS_BOOL vaild[16];
    }XC_FilmRoStates;

    typedef struct
    {
        MS_U32 lumaBin[32];
    }XC_LumaHistogramStates;

    typedef struct
    {
        MS_BOOL HDSDD0_det_mode;
        MS_BOOL HDSDD1_det_mode;

        MS_U8 HDSDD0_det_mode_shift;
        MS_U8 HDSDD1_det_mode_shift;

        MS_U16 HDSDD0_det_mode_threshold;
        MS_U16 HDSDD1_det_mode_threshold;

        MS_U8 HDSDD_underflow_threshold;
        MS_U8 HDSDD_overflow_threshold;

        MS_BOOL HDSD_detection_letter_box_enable;

    }XC_HpfDefaultRegSetting;

    typedef struct
    {
        MS_U16 pdnrlowLut[4];
        MS_U16 pdnrlow2Lut[4];
        MS_U16 pdnrhighLut[4];
        MS_U16 mcnrLut[8];
    }XC_MCNRLutQmapSetting;

    typedef struct
    {
        MS_U8 pbgY, pbgU, pbgV;
        MS_U32 flatSkinCnt,cplxCnt, prvBgCnt;
        MS_U32 skinRawCnt, cplxRawCnt, dswRawCnt;
        MS_U32 pbgrate;
    }XC_BackgroundState;

    typedef struct
    {
        MS_U16 Pk2DfinalGain,UFSCPk2DfinalGain;
        MS_U16 P3x3pkPosfinalGain,P3x3pkNegfinalGain;
        MS_U16 ERCEdgeG, ERCDetailG2 ,ERCDetailG3;
        MS_U16 NLMGain;
        MS_U16 SPNNGL,SPNNGH;
        MS_U16 LdefinalGain,LdefinalGainH,Prepk3x3finalGain;
        MS_U16 P2mfinalGain;
    }XC_PqWriteReg;


    /********************************************************************************/
    /*                   Function Prototypes                                        */
    /********************************************************************************/
    extern MS_U8 msAPI_Scaler_GetXC_Info(void);
    INTERFACE MS_U8 Hal_PQ_GetCurrentMotionValue(void *pInstance);
    INTERFACE void Hal_PQ_AdaptiveTuning(void *pInstance);
    INTERFACE void Hal_PQ_FilmMode_AnyCandence_Enable(void *pInstance,MS_BOOL bEnable);
    INTERFACE void Hal_PQ_StoreCurrentValue(void *pInstance);
    INTERFACE void Hal_PQ_ReStoreToDefault(void *pInstance);
    INTERFACE void MApi_XC_Adaptive_Film(void);
    INTERFACE MS_BOOL Hal_PQ_SetFakeOutEnable(void *pInstance,MS_BOOL bEnable);
        INTERFACE MS_BOOL Hal_PQ_LoadMCNRLutQmapSetting(void);

#undef INTERFACE
#ifdef __cplusplus
}
#endif

#endif

