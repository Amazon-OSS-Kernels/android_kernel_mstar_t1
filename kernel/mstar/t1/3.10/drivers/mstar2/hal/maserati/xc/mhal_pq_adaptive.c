#if 1
/**
 * Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
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
/// file    mhal_pq_adaptive.c
/// @brief  MStar GFLIP DDI HAL LEVEL
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#define _HAL_PQ_ADAPTIVE_C

//=============================================================================
// Include Files
//=============================================================================
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include "mdrv_mstypes.h"
//#include "color_format_ip.h"
#include "mhal_dlc.h"
#include "mdrv_xc_st.h"
#include "mhal_pq_adaptive.h"
#include "color_format_input.h" //for get InputType need to remove

//#include <linux/kernel.h>   /* printk() */

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
extern int Mstar_Timer1_GetMs(void);
extern MS_BOOL KApi_XC_MLoad_Fire(EN_MLOAD_CLIENT_TYPE _client_type, MS_BOOL bImmeidate);

int minmax(int a, int b, int c) // make sure b < c
{
    if (a<=b)
        return b;
    else if (a >= c)
        return c;
    else
        return a;
}

void KApi_XC_MenuLoad(MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask)
{
    MS_U16 u16ReadMask = ~u16Mask;
    MS_U16 u16Original_Data = MApi_GFLIP_XC_R2BYTEMSK(u32Addr,u16ReadMask);
    MS_U16 u16WriteData = u16Data & u16Mask;
    u16WriteData = u16WriteData | u16Original_Data;
    MS_U8 u8IsSCorFSC = u32Addr >> 16;

    if(u8IsSCorFSC == 0x13) //SC bank
    {
        MLoad_W2BYTEMSK(u32Addr,u16WriteData,u16Mask);
    }
    else
    {
        MLoad_UFSC_W2BYTEMSK(u32Addr,u16WriteData,u16Mask);
    }
}

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
//eva experiment need to remove
extern MS_U8 u8InputTypeFromCFI; //from color_format_input.c

static MS_BOOL _bAnyCandenceEnable = TRUE;
static MS_BOOL _bVerticalMovingForDHD = FALSE;
//extern MS_U16 _u16PQSrcType_DBK_Detect[PQ_MAX_WINDOW]; //For Auto_DBK SW driver used
static MS_BOOL _bIsHDSource = true;             //For Check SD mode
static int normalize_factor = 255;
XC_LetterBoxState lbState;
static MS_S32 frameWidth = 0x780;
static MS_S32 frameHeight = 0x438;
static MS_U8 lbConfirmLevel = 0;
static MS_U16 GuardBandWidth = 0x40;
#define MaxWidth 4097
#define MaxHeight 2161

//DBK Variables
#define paraSize 32
#define paraVerSize 16
static MS_S32 WidthIIR = 8,VWidthIIR = 8;

//PQ driver SD HD report W IIR
static MS_U32 u32SdW = 0, u32FhdW = 0;

static XC_MCNRLutQmapSetting m_stMCNRLutQmapSetting = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}};

// Threshold for blockness check
const int hitrate_threshold = 128; // 160/256
const int hitRateDiffThr = 16; // 16/256

//Patch switching control
#define HQV_JAGGY_PATTERN_ENBLE 0
#define SHIBASOKU_ENABLE 1
#define LG_SPECIFIC_PATTERN_ENABLE 0
#define SZ_PATTERN_ENABLE 1
#define SEC_HQV_PATTERN 0
#define KURO_PATTERN_ENABLE 0
#define HOUSE_MD_PATTERN_ENABLE 0

#define MCNR_LUT_LOAD_FROM_QMAP 1
#define PDNR_LUT_LOAD_FROM_QMAP 1

#define FOR_FULL_RANGE 0

//-------------------------------------------------------------------------------------------------

//=============================================================================
// Compile options
//=============================================================================

//=============================================================================
// Debug Macros
//=============================================================================

//=============================================================================
// Macros
//=============================================================================

//=============================================================================
// Local Variables
//=============================================================================
/*********************Eva modify ********************************************/

void SDHD_Report_IIR(MS_U32 *_hpfHdCntReport4, MS_U32 *_hpfSdCntReport4, MS_U32 *hdNactCnt4, MS_U32 *_hpfHdCntReport4_iir, MS_U32 *_hpfSdCntReport4_iir, MS_U32 *hdNactCnt4_iir)
{
    static MS_U32 u32Pre_HdReport = 0;
    static MS_U32 u32Pre_SdReport = 0;
    static MS_U32 u32Pre_hdNactCnt = 0;

    u32Pre_HdReport = (*_hpfHdCntReport4+ u32Pre_HdReport*3) >> 2;
    u32Pre_SdReport = (*_hpfSdCntReport4+ u32Pre_SdReport*3) >> 2;
    u32Pre_hdNactCnt = (*hdNactCnt4+ u32Pre_hdNactCnt*3) >> 2;

    *_hpfHdCntReport4_iir = u32Pre_HdReport;
    *_hpfSdCntReport4_iir = u32Pre_SdReport;
    *hdNactCnt4_iir       = u32Pre_hdNactCnt;
}
void SDHD_Report_NoTimeSharing_DelayNF(MS_U32 *_hpfHdCntReport4, MS_U32 *_hpfSdCntReport4, MS_U32 *hdNactCnt4 )
{
    static MS_U32 pre_hpfHdCntReport4[10] = {0};
    static MS_U32 pre_hpfSdCntReport4[10] = {0};
    static MS_U32 pre_hdNactCnt4[10] = {0};

    MS_U32 now_hpfHdCntReport4 = ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_75_L , 0xFFFF) ) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_74_L,0xFFFF);
    MS_U32 now_hpfSdCntReport4 = ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_77_L , 0xFFFF) ) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_76_L,0xFFFF);
    MS_U32 now_hdNactCnt4 = ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_79_L , 0xFFFF) ) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_78_L,0xFFFF);

    MS_U8 u8Delay_N_Frame;
    if( (( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_07_L,0xFFFF) >>4)&0x1) == 1 )
        u8Delay_N_Frame = minmax( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_07_L , 0x000F), 0 , 10 );
    else
        u8Delay_N_Frame = 4;

    if(u8Delay_N_Frame == 0){
        *_hpfHdCntReport4 =  now_hpfHdCntReport4;
        *_hpfSdCntReport4 =  now_hpfSdCntReport4;
        *hdNactCnt4       =  now_hdNactCnt4;
    }
    else{
        *_hpfHdCntReport4 =  pre_hpfHdCntReport4[(10 - u8Delay_N_Frame)];
        *_hpfSdCntReport4 =  pre_hpfSdCntReport4[(10 - u8Delay_N_Frame)];
        *hdNactCnt4       =  pre_hdNactCnt4[(10 - u8Delay_N_Frame)];
    }

    //printk("HR = ");
    int i;
    for(i = 0 ; i <= 8 ; i++){
        pre_hpfHdCntReport4[i] = pre_hpfHdCntReport4[i+1];
        pre_hpfSdCntReport4[i] = pre_hpfSdCntReport4[i+1];
        pre_hdNactCnt4[i] = pre_hdNactCnt4[i+1];
        //printk("%d ",pre_hpfHdCntReport4[i]);
    }

    pre_hpfHdCntReport4[9] = now_hpfHdCntReport4;
    pre_hpfSdCntReport4[9] = now_hpfSdCntReport4;
    pre_hdNactCnt4[9] = now_hdNactCnt4;

    //printk("%d N: %d\n",pre_hpfHdCntReport4[9],*_hpfHdCntReport4);

}

void SDHD_U11BaseReport_NoTimeSharing_DelayOneF(MS_U32 *_hpfHdCntReport4, MS_U32 *_hpfSdCntReport4, MS_U32 *hdNactCnt4 )
{
    static MS_U32 pre_hpfHdCntReport4 = 0;
    static MS_U32 pre_hpfSdCntReport4 = 0;
    static MS_U32 pre_hdNactCnt4 = 0;

    MS_U32 now_hpfHdCntReport4 = ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_75_L , 0xFFFF) ) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_74_L,0xFFFF);
    MS_U32 now_hpfSdCntReport4 = ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_77_L , 0xFFFF) ) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_76_L,0xFFFF);
    MS_U32 now_hdNactCnt4 = ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_79_L , 0xFFFF) ) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_78_L,0xFFFF);

    *_hpfHdCntReport4 =  pre_hpfHdCntReport4;
    *_hpfSdCntReport4 =  pre_hpfSdCntReport4;
    *hdNactCnt4       =  pre_hdNactCnt4;

    pre_hpfHdCntReport4 = now_hpfHdCntReport4;
    pre_hpfSdCntReport4 = now_hpfSdCntReport4;
    pre_hdNactCnt4 = now_hdNactCnt4;

    /*
    static MS_U32 REG_FSC_BK30_01_L_PRE_VALUE = 0;
    static MS_U32 REG_FSC_BK30_03_L_1_PRE_VALUE = 0;
    static MS_U32 REG_FSC_BK30_02_L_PRE_VALUE = 0;
    static MS_U32 REG_FSC_BK30_03_L_2_PRE_VALUE = 0;
    static MS_U32 REG_FSC_BK30_04_L_PRE_VALUE = 0;
    static MS_U32 REG_REG_FSC_BK30_06_L_1_PRE_VALUE = 0;
    
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_01_L, REG_FSC_BK30_01_L_PRE_VALUE ,0xFFFF);  //HD count
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_03_L, REG_FSC_BK30_03_L_1_PRE_VALUE , 0x00FF);
        
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_02_L, REG_FSC_BK30_02_L_PRE_VALUE ,0xFFFF);  //SD count
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_03_L, REG_FSC_BK30_03_L_2_PRE_VALUE<<8 , 0xFF00);
    
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_04_L, REG_FSC_BK30_04_L_PRE_VALUE ,0xFFFF);  //HD nact count
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_06_L, REG_REG_FSC_BK30_06_L_1_PRE_VALUE , 0x00FF);
    
    REG_FSC_BK30_01_L_PRE_VALUE = MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_74_L);
    REG_FSC_BK30_03_L_1_PRE_VALUE = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_75_L , 0x00FF);
    REG_FSC_BK30_02_L_PRE_VALUE = MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_76_L);
    REG_FSC_BK30_03_L_2_PRE_VALUE = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_77_L , 0x00FF);
    REG_FSC_BK30_04_L_PRE_VALUE = MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_78_L);
    REG_REG_FSC_BK30_06_L_1_PRE_VALUE = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_79_L , 0x00FF); */
}

void SDHD_U11BaseReport_NoTimeSharing( MS_U32 *_hpfHdCntReport4, MS_U32 *_hpfSdCntReport4, MS_U32 *hdNactCnt4)
{

    *_hpfHdCntReport4 =  ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_75_L , 0xFFFF) ) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_74_L,0xFFFF);
    *_hpfSdCntReport4 =  ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_77_L , 0xFFFF) ) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_76_L,0xFFFF);
    *hdNactCnt4       =  ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_79_L , 0xFFFF) ) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_78_L,0xFFFF);

    /*MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_01_L, MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_74_L) ,0xFFFF);  //HD count
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_03_L, MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_75_L , 0x00FF) , 0x00FF);
        
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_02_L, MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_76_L) ,0xFFFF);  //SD count
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_03_L, MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_77_L , 0x00FF)<<8 , 0xFF00);

    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_04_L, MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_78_L) ,0xFFFF);  //HD nact count
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_06_L, MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_79_L , 0x00FF) , 0x00FF);*/
}

#define FOR_FULL_RANGE 0
void MDrv_SC_hpfDefaultRelod()
{
#if FOR_FULL_RANGE
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_72_L, 0x01D8, 0xFFFF); //Reset for Full range in Maserati
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x4000, 0x7000);

            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_71_L, 0x01D8, 0xFFFF);  /*for HD repot*/ //Reset for Full range in Maserati
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x0200, 0x0700); /*for HD repot*/

            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_73_L, 0x0000, 0x003F); /* underflow */ //Reset for Full range in Maserati
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_73_L, 0x3F00, 0x3F00); /* overflow *///Reset for Full range in Maserati

#else  //for limit range settings
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_72_L, 0x0190, 0xFFFF);
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x4000, 0x7000);

            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_71_L, 0x0190, 0xFFFF);  /*for HD repot*/
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x0200, 0x0700); /*for HD repot*/


            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_73_L, 0x000C, 0x003F); /* underflow */
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_73_L, 0x3200, 0x3F00); /* overflow */
#endif

            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x0001 , 0x0001); /* mode */
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x0010, 0x0010); /* mode */
}

#define TIMEID_sdhdFilterBaseReport 0
void SDHD_U11BaseReport( const MS_U8 _timeSharingID, const MS_U8 timingSegment , MS_U32 *_hpfHdCntReport4, MS_U32 *_hpfSdCntReport4, MS_U32 *hdNactCnt4 )
{
    //MS_U32 HDcnt_Act_Cur = 0;
    //MS_U32 SDcnt_Act_Cur = 0;
    //MS_U32 hdNactCntCur;

    if( _timeSharingID == TIMEID_sdhdFilterBaseReport )
    {
        if( timingSegment == 0 )
        {
            //MDrv_SC_hpfDefaultRelod( defaultReg );

            MS_U16 TH1 = MApi_GFLIP_XC_R2BYTE(REG_FSC_BK30_24_L);
            MS_U16 TH2 = MApi_GFLIP_XC_R2BYTE(REG_FSC_BK30_25_L);
            MS_U16 TH3 = MApi_GFLIP_XC_R2BYTE(REG_FSC_BK30_26_L);
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x4411, 0xFFFF);
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_71_L, TH1, 0xFFFF);
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_72_L, TH2, 0xFFFF);
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_73_L, TH3, 0xFFFF);
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_7F_L, 0x0030, 0xFFFF);

        }
        else if( timingSegment > 1 )
        {
            *_hpfHdCntReport4 =  ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_75_L , 0xFFFF) ) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_74_L,0xFFFF);
            *_hpfSdCntReport4 =  ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_77_L , 0xFFFF) ) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_76_L,0xFFFF);
            *hdNactCnt4       =  ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_79_L , 0xFFFF) ) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_78_L,0xFFFF);
            
            /*MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_01_L, MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_74_L) ,0xFFFF);  //HD count
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_03_L, MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_75_L , 0x00FF) , 0x00FF);
                
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_02_L, MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_76_L) ,0xFFFF);  //SD count
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_03_L, MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_77_L , 0x00FF)<<8 , 0xFF00);
            
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_04_L, MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_78_L) ,0xFFFF);  //HD nact count
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_06_L, MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_79_L , 0x00FF) , 0x00FF);*/

            
            //HDcnt_Act_Cur =  (MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_75_L) << 16) + MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_74_L);
            //SDcnt_Act_Cur =  (MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_77_L) << 16) + MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_76_L);
            //hdNactCntCur =   (MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_79_L) << 16) + MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_78_L); 
            
        }
    
    }
    
}

#define TIMEID_dynamicHDfilterBaseReport 1
void MDrv_SC_dynamicHDfilterBaseReport( const MS_U8 _timeSharingID, const MS_U8 timingSegment ,MS_U32 *_hdFilterBaseReport)
{
    //MS_BOOL isStillImage = ( u32MotionValue < 10 ) ? true : false;
    
    if( _timeSharingID == TIMEID_dynamicHDfilterBaseReport )
    {
        if( timingSegment == 0 )
        {
            
            MDrv_SC_hpfDefaultRelod();

            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_71_L, 0x0190, 0xFFFF);
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_70_L, (0x04)<<8, 0x0700);


        }
        else if( timingSegment > 1 )
        {
            *_hdFilterBaseReport =  ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_75_L , 0xFFFF) ) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_74_L,0xFFFF);
            
            /*MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_05_L, MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_74_L) ,0xFFFF);  //HD count
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_06_L, MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_75_L , 0x00FF)<<8 , 0xFF00);*/
            
        }
        

    }
    
}

#define TIMEID_hpfTolerantReport 2
void MDrv_SC_hpfTolerantReport( const MS_U8 _timeSharingID, const MS_U8 timingSegment ,MS_U32 *_hpfHdTolerantReport, MS_U32 *_hpfSdTolerantReport)
{
    
    //MS_U32 HDcnt_Act_Cur = 0;
    //MS_U32 SDcnt_Act_Cur = 0;
    //MS_U32 hdNactCntCur;
    
    if( _timeSharingID == TIMEID_hpfTolerantReport )
    {
        if( timingSegment == 0 )
        {
            
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_72_L, 0x0100, 0xFFFF);
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_70_L, (0x02)<<12, 0x7000);

            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_71_L, 0x0280, 0xFFFF);     /*for HD repot*/
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_70_L, (0x06)<<8, 0x0700);  /*for HD repot*/

            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_73_L, 0x0000 , 0x003F); /* underflow */
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_73_L, 0x3F00, 0x3F00); /* overflow */


        }
        else if( timingSegment > 1 )
        {

            *_hpfHdTolerantReport =  ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_75_L , 0xFFFF) ) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_74_L,0xFFFF);
            *_hpfSdTolerantReport =  ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_77_L , 0xFFFF) ) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_76_L,0xFFFF);
            /*MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_07_L, MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_74_L) ,0xFFFF);  //HD count
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_0A_L, MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_75_L , 0x00FF) , 0x00FF);
            
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_08_L, MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_76_L) ,0xFFFF);  //SD count
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_0A_L, MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_77_L , 0x00FF)<<8 , 0xFF00);*/
            
        }
        
    
    }
    
}

#define TIMEID_FilterBaseReport 3
void MDrv_SC_SDHD_FilterBaseReport( const MS_U8 _timeSharingID, const MS_U8 timingSegment ,MS_U32 *_hpfHdCntReport ,MS_U32 *_hpfSdCntReport)
{   
    MS_U8 u8Luma = 0;
    static MS_U8 u8PrevLuma = 0;
    MS_U16 u16MinPixel = 0,u16MaxPixel = 0, MaxMinPixel = 0;
    static MS_U16 u16PrevMaxPixel = 0;
    static MS_U16 u16PrevMinPixel = 0;
     MS_U32 HDSDD_Det_threshold = 0;
     
    
    if( _timeSharingID == TIMEID_FilterBaseReport )
    {
        if( timingSegment == 0 )
        {
            
            MDrv_SC_hpfDefaultRelod();

            u8Luma=(MS_U32)((MS_U32)u8PrevLuma*3 + msGetAverageLuminous())>>2;
            u8PrevLuma = u8Luma;

            u16MaxPixel = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK1A_0B_L, 0x00FF); //Register move to UFSC in Maserati
            u16MaxPixel = (u16PrevMaxPixel*3 + u16MaxPixel ) >> 2;
            u16PrevMaxPixel = u16MaxPixel;

            u16MinPixel = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK1A_0B_L, 0xFF00)>>8;  //Register move to UFSC in Maserati
            u16MinPixel = (u16PrevMinPixel*3 + u16MinPixel ) >> 2;
            u16PrevMinPixel = u16MinPixel;

            MaxMinPixel = u16MaxPixel - u16MinPixel;

            if(u8Luma>=0xFF)
                u8Luma = 0xFF;

            HDSDD_Det_threshold = ((MaxMinPixel*6)>>2) + ((u8Luma*5)>>2);

            if(HDSDD_Det_threshold >=0xFFFF)
                HDSDD_Det_threshold=0xFFFF;

            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_71_L, (MS_U16)HDSDD_Det_threshold, 0xFFFF);
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_72_L, (MS_U16)(HDSDD_Det_threshold>>1), 0xFFFF);


        }
        else if( timingSegment > 1 )
        {

            *_hpfHdCntReport =  ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_75_L , 0xFFFF) ) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_74_L,0xFFFF);
            *_hpfSdCntReport =  ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_77_L , 0xFFFF) ) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_76_L,0xFFFF);
            /*MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_0B_L, MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_74_L) ,0xFFFF);  //HD count
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_0D_L, MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_75_L , 0x00FF) , 0x00FF);
            
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_0C_L, MApi_GFLIP_XC_R2BYTE(REG_SC_BK02_76_L) ,0xFFFF);  //SD count
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_0D_L, MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_77_L , 0x00FF)<<8 , 0xFF00);*/
            
        }
        

    }
    
}



XC_OpFrameStates MDrv_SC_OpFrame_Status(void)
{
    XC_OpFrameStates opFrameStatus;

    opFrameStatus.curOpFrame = (MS_U8)( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK12_3A_L, 0x00F0) >> 4 );
    opFrameStatus.maxOpFrame = (MS_U8)( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK12_19_L, 0x001F) >> 0 );

    return opFrameStatus;
}

void MDrv_SC_Edison_ME1_SetStatisticsWindow(MS_U32 u16Width, MS_U16 u16Height)
{
    MS_U16 u16MaxHblkSize = 0, u16MaxVblkSize = 0;

    u16MaxHblkSize  = (u16Width/10);  //ME2_H Max
    u16MaxVblkSize = (u16Height/6);  //ME2_V Max

    u16MaxHblkSize  = u16MaxHblkSize - 4;
    u16MaxVblkSize = u16MaxVblkSize - 4;

    MApi_GFLIP_XC_W2BYTEMSK( REG_SC_BK17_5C_L,  u16MaxHblkSize<<8 , 0xFF00 ); /*reg_stat_xblk_end*/
    MApi_GFLIP_XC_W2BYTEMSK( REG_SC_BK17_5D_L,  u16MaxVblkSize<<8 , 0xFF00 ); /*reg_stat_yblk_end*/
}


MS_BOOL MDrv_SC_SourceChange(void)
{
    static MS_BOOL bprevSourceChange = false;

    MS_BOOL sourceChangeInitReg = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_07_L, 0x0001);
    sourceChangeInitReg = sourceChangeInitReg&&(!bprevSourceChange);

    if( sourceChangeInitReg == 1 )
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_07_L, sourceChangeInitReg << 1, 0x0002);
    }
    else
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_07_L, 0x0000, 0x0001);

    bprevSourceChange = sourceChangeInitReg;
    return sourceChangeInitReg;
}

MS_BOOL MDrv_SC_SourceChangeFlag(MS_BOOL SourceChangeLastTime)
{
    static MS_BOOL bsourceChangeFrm1 = false;
    static MS_BOOL bsourceChangeFrm2 = false;
    static MS_BOOL bsourceChangeFrm3 = false;

    MS_BOOL sourceChangeFlag = bsourceChangeFrm1 || bsourceChangeFrm2 || bsourceChangeFrm3 || SourceChangeLastTime;

    bsourceChangeFrm1 = bsourceChangeFrm2;
    bsourceChangeFrm2 = bsourceChangeFrm3;
    bsourceChangeFrm3 = SourceChangeLastTime;


    return sourceChangeFlag;
}

#define TIME_SHARING_DELAY 3
#define TIME_SHARING_MAX_ID 4 
MS_U8 MDrv_SC_TimeSharing( const XC_OpFrameStates opFrameStatusCur,  MS_U8 *timingSegment , MS_BOOL isSourceChangeFlag ,const MS_BOOL isSceneChangeY)
{
    MS_U8 TIME_SHARING_MAX_ID_2 = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_3F_L,0x000F);
    static XC_OpFrameStates opFrameStatusLast;
    static MS_U8 timingId = 0;

    if( opFrameStatusCur.curOpFrame != opFrameStatusLast.curOpFrame )
        timingId++;

    //if( timingId == (TIME_SHARING_DELAY*TIME_SHARING_MAX_ID) || isSourceChangeFlag == 1)
    if( timingId == (TIME_SHARING_DELAY*TIME_SHARING_MAX_ID_2) || isSourceChangeFlag == 1 || isSceneChangeY )
        timingId = 0;

    opFrameStatusLast.curOpFrame = opFrameStatusCur.curOpFrame;

    // return
    *timingSegment = ( timingId % TIME_SHARING_DELAY ) ;
    return ( timingId / TIME_SHARING_DELAY );
}

XC_LumaHistogramStates MDrv_SC_GetLumaHistogram(void)
{
    //all registers move to UFSC BK55 in Maserati
    XC_LumaHistogramStates lumaStatus;
       int i = 0;
    lumaStatus.lumaBin[0] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_30_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_31_L , 0x000F)<<16);
    lumaStatus.lumaBin[1] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_32_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_33_L , 0x000F)<<16);
    lumaStatus.lumaBin[2] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_34_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_35_L , 0x000F)<<16);
    lumaStatus.lumaBin[3] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_36_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_37_L , 0x000F)<<16);
    lumaStatus.lumaBin[4] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_38_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_39_L , 0x000F)<<16);
    lumaStatus.lumaBin[5] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_3A_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_3B_L , 0x000F)<<16);
    lumaStatus.lumaBin[6] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_3C_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_3D_L , 0x000F)<<16);
    lumaStatus.lumaBin[7] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_3E_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_3F_L , 0x000F)<<16);
    lumaStatus.lumaBin[8] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_40_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_41_L , 0x000F)<<16);
    lumaStatus.lumaBin[9] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_42_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_43_L , 0x000F)<<16);
    lumaStatus.lumaBin[10] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_44_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_45_L , 0x000F)<<16);
    lumaStatus.lumaBin[11] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_46_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_47_L , 0x000F)<<16);
    lumaStatus.lumaBin[12] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_48_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_49_L , 0x000F)<<16);
    lumaStatus.lumaBin[13] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_4A_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_4B_L , 0x000F)<<16);
    lumaStatus.lumaBin[14] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_4C_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_4D_L , 0x000F)<<16);
    lumaStatus.lumaBin[15] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_4E_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_4F_L , 0x000F)<<16);
    lumaStatus.lumaBin[16] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_50_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_51_L , 0x000F)<<16);
    lumaStatus.lumaBin[17] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_52_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_53_L , 0x000F)<<16);
    lumaStatus.lumaBin[18] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_54_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_55_L , 0x000F)<<16);
    lumaStatus.lumaBin[19] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_56_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_57_L , 0x000F)<<16);
    lumaStatus.lumaBin[20] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_58_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_59_L , 0x000F)<<16);
    lumaStatus.lumaBin[21] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_5A_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_5B_L , 0x000F)<<16);
    lumaStatus.lumaBin[22] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_5C_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_5D_L , 0x000F)<<16);
    lumaStatus.lumaBin[23] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_5E_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_5F_L , 0x000F)<<16);
    lumaStatus.lumaBin[24] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_60_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_61_L , 0x000F)<<16);
    lumaStatus.lumaBin[25] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_62_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_63_L , 0x000F)<<16);
    lumaStatus.lumaBin[26] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_64_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_65_L , 0x000F)<<16);
    lumaStatus.lumaBin[27] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_66_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_67_L , 0x000F)<<16);
    lumaStatus.lumaBin[28] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_68_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_69_L , 0x000F)<<16);
    lumaStatus.lumaBin[29] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_6A_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_6B_L , 0x000F)<<16);
    lumaStatus.lumaBin[30] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_6C_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_6D_L , 0x000F)<<16);
    lumaStatus.lumaBin[31] =  MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_6E_L , 0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_6F_L , 0x000F)<<16);

    for (i=0; i<32; i++)
    {
        lumaStatus.lumaBin[i]= lumaStatus.lumaBin[i]>>4;
    }
    return lumaStatus;
}

//#define ScenceChange_TH        0x1000
MS_BOOL MDrv_SC_Agate_SceneChangeY( const  XC_LumaHistogramStates lumaHistogram )
{
    MS_U32 ScenceChange_TH = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_0E_L , 0xFFFF);
    //XC_DLC_ApiStatus DLC_Status;
    //MApi_XC_DLC_GetStatus_Ex(&DLC_Status,MAIN_WINDOW);
    static MS_U32 Pre_LumaHistogram[32], Max_HistogramDiff = 0;
    MS_U16 HistogramDiff = 0;
    int i = 0;

    for(i=0;i<32;i++)
    {

        HistogramDiff = abs( lumaHistogram.lumaBin[i] - Pre_LumaHistogram[i] );

        if(HistogramDiff > Max_HistogramDiff)
        {
            Max_HistogramDiff = HistogramDiff;
        }

        Pre_LumaHistogram[i] = lumaHistogram.lumaBin[i];
    
    }

    
    if( Max_HistogramDiff > ScenceChange_TH )
    {
        Max_HistogramDiff = 0;
        return true;
    }
    else
    {
        Max_HistogramDiff = 0;
        return false;
    }
}

void Decrease2DPKwhenSceneChange(const MS_BOOL isSceneChangeY)
{
    MS_U8 u8TarDelayTime = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_0F_L,0x0F00)>>8;
    static MS_U8 u8DelayTime = 0;
    if(isSceneChangeY)
    {
        MS_U8 u8DecreaseRatio = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_0F_L,0x00FF);
        u8DecreaseRatio = min(u8DecreaseRatio,16);
        MS_U16 u16FSC2dpkFinalGain = 0x1B;
        u16FSC2dpkFinalGain = ( u16FSC2dpkFinalGain*u8DecreaseRatio ) >>4;
        
        MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK5F_3F_L,  u16FSC2dpkFinalGain , 0x003F);
        
    }   
    else
    {
        if(u8DelayTime == u8TarDelayTime)
            u8DelayTime = 0;
        
        if(u8DelayTime == 0){
            MS_U16 u16FSC2dpkFinalGainTar = 0x1B;
            MS_U16 u16FSC2dpkFinalGainCur = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK5F_3F_L,0x003F);
            if(u16FSC2dpkFinalGainCur < u16FSC2dpkFinalGainTar)
                u16FSC2dpkFinalGainCur ++;
            
            u16FSC2dpkFinalGainCur = min(u16FSC2dpkFinalGainCur , u16FSC2dpkFinalGainTar);
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK5F_3F_L,  u16FSC2dpkFinalGainCur , 0x003F);
        }
        u8DelayTime ++;
        
    }
}

/*MS_U32 MDrv_SC_read_motion_value1(void)
{
    MS_U32 u32MotionValue = 0;
    MS_U32 u32RegMadi_1C = 0, u32RegMadi_1B = 0, u32RegMadi_1A = 0;

    u32RegMadi_1C = (MS_U32)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK22_0E_L, 0x3F);
    u32RegMadi_1B = (MS_U32)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK22_0D_L, 0xFF00);
    u32RegMadi_1A = (MS_U32)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK22_0D_L, 0x00FF);

    u32RegMadi_1C = u32RegMadi_1C * 0x10000UL;

    u32MotionValue = (u32RegMadi_1C + u32RegMadi_1B + u32RegMadi_1A)  ;

    return u32MotionValue;
}*/

void MDrv_SC_ResetSourceChange(const MS_BOOL sourceChangeForME ) //review MERESET
{

    static MS_U8 u8CurFrameCnt1 = 0;
    static MS_U8 u8CurFrameCnt2 = 0;
    static MS_BOOL meReset = false;
    static MS_BOOL isReset = false;

    if(sourceChangeForME == 1 )
    {
        if(u8CurFrameCnt1 != 60 )
            u8CurFrameCnt1++;
    }
    else
        u8CurFrameCnt1 = 0;

    if( meReset )
    {
        if(u8CurFrameCnt2 != 60 )
            u8CurFrameCnt2++;
    }
    else
        u8CurFrameCnt2 = 0;

    if( (u8CurFrameCnt1 == 30) && (!meReset) )
    {
        meReset = true;
    }

    if( (u8CurFrameCnt2 == 5) && meReset )
    {
        meReset = false;
        isReset = true;
    }
    else
    {
        isReset = false;
    }

    if(isReset)
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_07_L,  0x0000, 0x0002); /* Reset Init Bit */
        u8CurFrameCnt1 = 0;
    }
}

/*********************Eva modify 2 start********************************************/
void MDrv_UFSC_PQ_BNF_status( XC_BackgroundState *backgroundState, MS_BOOL isOpFrameChanged  ) //Left half histogram data
{
    static MS_U16 iir_Ymax = 0;
    static MS_U16 iir_Umax = 0;
    static MS_U16 iir_Vmax = 0;
    static MS_U32 iir_pbgrate = 0;
    static MS_U32 iir_flatSkinCnt = 0;
    static MS_U32 iir_cplxCnt = 0;
    static MS_U32 iir_prvBgCnt = 0;
    static MS_U32 iir_skinRawCnt = 0;
    static MS_U32 iir_cplxRawCnt = 0;
    static MS_U32 iir_dswRawCnt = 0;

    MS_U32 BNF_Yhist[32], BNF_Uhist[32], BNF_Vhist[32];
    static MS_BOOL SetStatus = false;
    MS_U16 i, Ymax, Umax, Vmax;
    MS_U32 Ymax_cnt, Umax_cnt, Vmax_cnt, maxYUVcnt;
    MS_U32 totalPixel, pbgrate;

    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK4D_39_L, 0 , 0x0FFF );
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK4D_3B_L, frameWidth/2-1 , 0x0FFF );
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK4D_3C_L, frameHeight-1 , 0x0FFF );

    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BKCD_39_L, GuardBandWidth , 0x0FFF );
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BKCD_3B_L, GuardBandWidth+ frameWidth/2-1 , 0x0FFF );
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BKCD_3C_L, frameHeight-1 , 0x0FFF );


    if ( isOpFrameChanged )
    {
        if( !SetStatus )
        {
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK4D_51_L, 0x0000 , 0x0004 );
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK4D_51_L, 0x0004 , 0x0004 );

            SetStatus = true;
        }
        else if (( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK4D_51_L,0xFFFF)& 0x0008 ) &&  SetStatus )
        {
            // Y histogram
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK4D_51_L, 0x0000 , 0x0003 );
            for( i = 0 ; i < 32 ; ++i )
            {
                MS_U16 sh = ((i&0x3)<<2);
                            BNF_Yhist[i] = ( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK4D_58_L+ (i<<1) ,0xFFFF) << 3 )
                    + ( (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK4D_78_L + ((i>>2)<<1), (0x7000 >> sh))) >> (12-sh) )
                    + ( MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BKCD_58_L + (i<<1) ,0xFFFF) << 3 )
                    + ( (MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BKCD_78_L + ((i>>2)<<1), (0x7000 >> sh))) >> (12-sh) );
            }

            // U histogram
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK4D_51_L, 0x0001 , 0x0003 );
            for( i = 0 ; i < 32 ; ++i )
            {
                MS_U16 sh = ((i&0x3)<<2);
                            BNF_Uhist[i] = ( MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK4D_58_L + (i<<1) ,0xFFFF) << 3 )
                    + ( (MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK4D_78_L + ((i>>2)<<1), (0x7000 >> sh))) >> (12-sh) )
                    + ( MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BKCD_58_L + (i<<1) ,0xFFFF) << 3 )
                    + ( (MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BKCD_78_L + ((i>>2)<<1), (0x7000 >> sh))) >> (12-sh) );
            }

            // V histogram
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK4D_51_L, 0x0002 , 0x0003 );
            for( i = 0 ; i < 32 ; ++i )
            {
                MS_U16 sh = ((i&0x3)<<2);
                            BNF_Vhist[i] = ( MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK4D_58_L + (i<<1) ,0xFFFF) << 3 )
                    + ( (MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK4D_78_L + ((i>>2)<<1), (0x7000 >> sh))) >> (12-sh) )
                    + ( MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BKCD_58_L + (i<<1) ,0xFFFF) << 3 )
                    + ( (MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BKCD_78_L + ((i>>2)<<1), (0x7000 >> sh))) >> (12-sh) );
            }


            Ymax = 0;
            Umax = 0;
            Vmax = 0;
            Ymax_cnt = 0;
            Umax_cnt = 0;
            Vmax_cnt = 0;

            for ( i = 0; i< 32; i++ )
            {
                if ( BNF_Yhist[i] > Ymax_cnt )
                {
                    Ymax = i << 4;
                    Ymax_cnt = BNF_Yhist[i];
                }

                if ( BNF_Uhist[i] > Umax_cnt )
                {
                    Umax = i << 4;
                    Umax_cnt = BNF_Uhist[i];
                }

                if ( BNF_Vhist[i] > Vmax_cnt )
                {
                    Vmax = i << 4 ;
                    Vmax_cnt = BNF_Vhist[i];
                }

            }

            maxYUVcnt  = min3(Ymax_cnt, Umax_cnt, Vmax_cnt);
            totalPixel = ( frameWidth* frameHeight ) >> 5;
            pbgrate = (totalPixel!=0) ? (maxYUVcnt*5000)/totalPixel : 0;

            Ymax = ((3*iir_Ymax)+ Ymax ) >> 2;
            iir_Ymax = Ymax;

            Umax = ((3*iir_Umax)+ Umax ) >> 2;
            iir_Umax = Umax;

            Vmax = ((3*iir_Vmax )+ Vmax ) >> 2;
            iir_Vmax = Vmax;

            pbgrate = ((iir_pbgrate*3 )+ pbgrate) >> 2;
            iir_pbgrate = pbgrate;

            Ymax = ( Ymax + 8 )>> 4;
            Umax = ( Umax + 8 )>> 4;
            Vmax = ( Vmax + 8 )>> 4;

            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK4D_42_L, Ymax , 0x001F );
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK4D_42_L, Umax << 8 , 0x1F00 );
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK4D_43_L, Vmax , 0x001F );
            MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK4D_45_L, pbgrate , 0xDFFF );

            MS_U32 flatSkinCnt = ( (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK4D_53_L, 0x0007 ) << 16) +  MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK4D_52_L ,0xFFFF)
                        + (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BKCD_53_L, 0x0007 ) << 16) +  MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BKCD_52_L ,0xFFFF) ) >> 1;
            MS_U32 cplxCnt = (( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK4D_55_L, 0x0007 ) << 16) +  MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK4D_54_L ,0xFFFF)
                        + (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BKCD_55_L, 0x0007 ) << 16) +  MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BKCD_54_L ,0xFFFF) ) >> 1;
            MS_U32 prvBgCnt = ((MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK4D_57_L, 0x0007 ) << 16) +  MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK4D_56_L ,0xFFFF)
                        + (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BKCD_57_L, 0x0007 ) << 16) +  MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BKCD_56_L ,0xFFFF) ) >> 1;
            MS_U32 skinRawCnt = ((MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK4D_4B_L, 0x0007 ) << 16) +  MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK4D_4A_L ,0xFFFF)
                        + (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BKCD_4B_L, 0x0007 ) << 16) +  MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BKCD_4A_L ,0xFFFF) ) >> 1;
            MS_U32 cplxRawCnt = ((MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK4D_4D_L, 0x0007 ) << 16) +  MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK4D_4C_L ,0xFFFF)
                        + (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BKCD_4D_L, 0x0007 ) << 16) +  MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BKCD_4C_L ,0xFFFF) ) >> 1;
            MS_U32 dswRawCnt = ((MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK4D_4F_L, 0x0007 ) << 16) +   MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK4D_4E_L ,0xFFFF)
                        + (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BKCD_4F_L, 0x0007 ) << 16) +  MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BKCD_4E_L ,0xFFFF) ) >> 1;


            flatSkinCnt = ((iir_flatSkinCnt*3 )+ flatSkinCnt) >> 2;
            iir_flatSkinCnt = flatSkinCnt;

            cplxCnt = ((iir_cplxCnt*3 )+ cplxCnt) >> 2;
            iir_cplxCnt = cplxCnt;

            prvBgCnt = ((iir_prvBgCnt*3 )+ prvBgCnt) >> 2;
            iir_prvBgCnt = prvBgCnt;

            skinRawCnt = ((iir_skinRawCnt*3 )+ skinRawCnt) >> 2;
            iir_skinRawCnt = skinRawCnt;

            cplxRawCnt = ((iir_cplxRawCnt*3 )+ cplxRawCnt) >> 2;
            iir_cplxRawCnt = cplxRawCnt;

            dswRawCnt = ((iir_dswRawCnt*3 )+ dswRawCnt) >> 2;
            iir_dswRawCnt = dswRawCnt;


            backgroundState->flatSkinCnt = flatSkinCnt;
            backgroundState->cplxCnt = cplxCnt;
            backgroundState->prvBgCnt = prvBgCnt;
            backgroundState->skinRawCnt = skinRawCnt;
            backgroundState->cplxRawCnt = cplxRawCnt;
            backgroundState->dswRawCnt = dswRawCnt;

            backgroundState->pbgY = Ymax;
            backgroundState->pbgU = Umax;
            backgroundState->pbgV = Vmax;
            backgroundState->pbgrate = pbgrate;

            SetStatus = false;
        }
    }

}

void MDrv_SC_GetSDHD_Report_ID0( MS_U32 *_hpfHdCntReport4, MS_U32 *_hpfSdCntReport4 , MS_U32 *hdNactCntRet4 )
{
    MS_U32 HDcnt_Act_Cur = 0;
    MS_U32 SDcnt_Act_Cur = 0;
    MS_U32 hdNactCntCur;
    
    HDcnt_Act_Cur =  ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_03_L,0x00FF) ) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_01_L,0xFFFF);
    SDcnt_Act_Cur =  ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_03_L,0xFF00) ) << 8 ) + MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_02_L,0xFFFF);
    hdNactCntCur =   ( ( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_06_L,0x00FF) ) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_04_L,0xFFFF);

    *_hpfHdCntReport4 = HDcnt_Act_Cur;
    *_hpfSdCntReport4 = SDcnt_Act_Cur;
    *hdNactCntRet4 = hdNactCntCur;
}



void MDrv_UFSC_PQ_LSE( int frNNCnt2, int menuW, int edgePatW, int srEnhStr , int sdW2 , int fHdW2 , XC_PqWriteReg *PqWriteReg)
{
    #define LSE_EDGE_G_LB 8
    // CG pattern
    int nnW = max( 0, frNNCnt2 - 2048 ) >> 3;
    nnW = min( 256, nnW );

    // LSE adaptive edge gain
    int edgeGain = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_38_L, 0x003F);
    int edgeGainLb = LSE_EDGE_G_LB;
    edgeGain = max( edgeGain, edgeGainLb );

    int sdfhdEdgeGain = ( MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_38_L, 0x3F00 ) >> 8 );
    edgeGain = (edgeGain*(256-fHdW2) + sdfhdEdgeGain *fHdW2)>>8;

    int adptGain = edgeGain ;
    adptGain = (adptGain * (256-nnW)) >> 8;
    adptGain = (adptGain * srEnhStr) >> 5;
    PqWriteReg-> ERCEdgeG = adptGain;

    // detail gain
    int reduceW = max(0,256 - nnW);//max( 0, 256 - min( (sdW2>>1), nnW ) );
    {
        int detGain1SD = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_22_L, 0x3F00)>>8;
        int detGain = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_39_L, 0x003F );
        detGain = ( sdW2 * detGain1SD + (256 - sdW2)*detGain )/256;
        detGain = (detGain * reduceW) >> 8;
        detGain = ( detGain * srEnhStr ) >> 5;
        PqWriteReg-> ERCDetailG2 = detGain;

    }
    {
        int detGain2SD = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_23_L, 0x003F);
        int detGain = ( MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_39_L, 0x3F00 ) >> 8 );
        detGain = ( sdW2 * detGain2SD + (256 - sdW2)*detGain )/256;
        detGain = ( detGain * reduceW) >> 8;
        detGain = ( detGain * srEnhStr ) >> 5;
        PqWriteReg-> ERCDetailG3 = detGain;

    }

    short fhd_bld_gain_min = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_2E_L, 0xff00)>>8;
    short hd_bld_gain_min = 0x20;
    short final_gain_min = ( fHdW2*fhd_bld_gain_min + (256-fHdW2)*hd_bld_gain_min )/256;
    final_gain_min = minmax(final_gain_min , 0, 0x1ff);
    MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK75_1A_L, final_gain_min, 0x01FF );

}

void MDrv_UFSC_PQ_NLM( XC_BackgroundState *backgroundState, int menuW, int edgePatW, int srEnhStr , int sdw2 , XC_PqWriteReg *PqWriteReg)
{
    //NLM DS patch gain sd pattern use ds patch but hd pattern not use
    int ds_patch_gain_sd = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_2F_L, 0xff00)>>8;//0x18;
    int ds_patch_gain_final;
    ds_patch_gain_final = (sdw2*ds_patch_gain_sd + (256-sdw2)*0)/256;
    MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK70_2A_L, ds_patch_gain_final <<8, 0x3F00 );

    // coring
    MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK70_31_L, max( edgePatW, menuW ) >> 3, 0x00FF );

    // NLM gain for skin, this should use colour adaptive
    int pbSkinW = max( 0, (int)backgroundState->flatSkinCnt - 1200) >> 2;
    pbSkinW = min( 16, pbSkinW );

    // NLM gain
    int finGain = /*0xA*/MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_30_L, 0x00FF ) - ( (4 * pbSkinW) >> 4 );
    finGain = ( finGain * srEnhStr ) >> 5;
    PqWriteReg-> NLMGain = finGain;


}

void MDrv_UFSC_PQ_PreEnh( int fHdw2, int sdw2, MS_U16 estNoise,MS_U8 estConfirm, int srEnhStr ,XC_PqWriteReg *PqWriteReg)
{
    // BLTI bias gain
    int biasGain = ( 0x10 * fHdw2 ) >> 8;
    if( fHdw2 == 256 )
    {
        biasGain = ( 0x10 * (256-sdw2) + 0x20 * sdw2 ) >> 8;
    }

    MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK4C_52_L, biasGain, 0x007F );

    // LTI ac thrd
    int acThrd = ( 0x04 * (256-sdw2) + 0x1C * sdw2 ) >> 8;
    MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK4C_53_L, acThrd<<8, 0xFF00 );

    // Pre PK3x3
    int finGain = ( MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_30_L, 0x3F00 ) >> 8 ) ;
    finGain = finGain*(256-fHdw2)>>8; //for maserati 2dpk issue
    finGain = finGain* srEnhStr  >> 5;
    finGain = minmax(finGain,0,0x3F);
    PqWriteReg-> Prepk3x3finalGain = finGain;


    // LDE

    int ldeDarkGain = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_31_L, 0x1F00) >> 8 ;
    int ldeDarkGainSD= MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_32_L, 0x001F) ;
    int ldeDarkGainFHD=MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_32_L, 0x1F00)>>8 ;
    int ldeGain = ( ldeDarkGain/*0xA*/ * (256 - fHdw2) + ldeDarkGainFHD/*0x12*/ * fHdw2 ) >> 8;
    if( fHdw2 == 256 )
    {
        ldeGain = ( ldeDarkGainFHD * (256-sdw2) + ldeDarkGainSD/*0x05*/ * sdw2 ) >> 8;
    }

    MS_U8 report_noise = minmax(estNoise,0,15);
    report_noise = (report_noise == 15) ? 16 : report_noise;
    MS_U8 report_confirm = minmax(estConfirm-1,0,7);
    report_confirm = max(0,report_confirm - 2) * 4;
    report_confirm = minmax( report_confirm, 0, 16 );

    int noiseW = estNoise - (16-report_confirm);
    noiseW = 16 - minmax( noiseW, 0, 16 );
    // back to default while noise is large
    ldeGain = ( ldeDarkGain * (16-noiseW) + ldeGain/*0xA*/ * noiseW ) >> 4;
    ldeGain = ( ldeGain * srEnhStr ) >> 5;
    ldeGain = minmax(ldeGain,0,0x1F);
    PqWriteReg-> LdefinalGain = ldeGain;
    

    int ldeGainH =( MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_31_L, 0x001F )/*0xA*/ * noiseW ) >> 4;
    ldeGainH = ( ldeGainH * srEnhStr ) >> 5;
    ldeGainH = minmax(ldeGainH,0,0x1F);
    PqWriteReg-> LdefinalGainH = ldeGainH;

}

void MDrv_UFSC_PQ_PostPK3x3( int menuW, int edgePatW, int fHdW, int sdW, int srEnhStr , XC_PqWriteReg *PqWriteReg ,MS_U16 u16ArtiW)
{
    // ds_minw
    edgePatW = max( edgePatW, menuW );
    fHdW = max( 0, fHdW - edgePatW );
    int dsmin = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_33_L, 0xFF00) >> 8;
    int dsminSD = min( 0x00FF, dsmin + MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_34_L, 0x00FF) );
    int dsminFHD = (dsmin*170 + dsminSD*86) >> 8;
    int dsminW = ( dsmin * (256-fHdW) + /*0x20*/dsminFHD * fHdW ) >> 8;
    if( fHdW == 256 )
    {
        dsminW = ( /*0x20*/dsminFHD * (256-sdW) + /*0x60*/dsminSD * sdW ) >> 8;
    }
    MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK76_42_L, dsminW<<8, 0xFF00 );

    // ds gain
    int dsGain = 0xB/*dsw*/ - ( ((0xB/*dswSD*/)*fHdW ) >> 8 );
    MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK76_42_L, (dsGain&0xFF), 0x00FF );

    int post3x3pkGain_FHD = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_2F_L , 0x00ff);
    int post3x3pkGain_Arti_pos = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1B_L , 0x00ff);
    int post3x3pkGain_Arti_neg = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1B_L , 0xff00)>>8;
    int posGain = (  0x30*(256-fHdW) + post3x3pkGain_FHD*fHdW )>>8;
    posGain = (  posGain*(256-sdW) + 0x18*sdW )>>8;
    posGain = (  posGain*(256-u16ArtiW) + post3x3pkGain_Arti_pos*u16ArtiW )>>8;
    posGain = ( posGain * srEnhStr ) >> 5;
    posGain = min( 0xFF, posGain );
    PqWriteReg-> P3x3pkPosfinalGain = posGain;

    int negGain = (  0x30*(256-fHdW) + post3x3pkGain_FHD*fHdW )>>8;
    negGain = (  negGain*(256-u16ArtiW) + post3x3pkGain_Arti_neg*u16ArtiW )>>8;
    negGain = ( negGain * srEnhStr ) >> 5;
    negGain = min( 0xFF, negGain );
    PqWriteReg-> P3x3pkNegfinalGain = negGain;

}

void MDrv_UFSC_PQ_HASC( int sdW, int srEnhStr )
{
    int lowGain = /*0x28*/( MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_36_L, 0x7F00 ) >> 8 )
                + ( (/*0x38*/MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_37_L, 0x007F ) * sdW) >> 8 );
    lowGain = ( lowGain * srEnhStr ) >> 5;
    lowGain = min( lowGain, 0x007F );
    MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK74_2E_L, lowGain<<8, 0x7F00 );

    int highGain = ( /*0x18*/MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_36_L, 0x007F ) * srEnhStr ) >> 5;
    highGain = min( 0x007F, highGain );
    MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK74_2E_L, highGain, 0x007F );
}

void MDrv_UFSC_PQ_P2M( int menuW, int edgePatW, int srEnhStr , XC_PqWriteReg *PqWriteReg)
{
    int gain = ( /*9*/( MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_37_L, 0x3F00 )>>8 ) * (256 - max(edgePatW, menuW)) ) >> 8;
    gain = (gain * srEnhStr ) >> 5;
    gain = min( 0x3F, gain );
    
    PqWriteReg-> P2mfinalGain = gain;

}

void MDrv_UFSC_PQ_ANN( int frNNCnt2 )
{
    // CG pattern
    int nnW = max( 0, frNNCnt2 - 2048 ) >> 3;
    nnW = min( 256, nnW );
    int artNNW = max( 0, frNNCnt2 - 8192 ) >> 4;
    artNNW = min( 256, artNNW );

    int colorGain = ( 0x10 * (256-nnW) + 0x7 * nnW ) >> 8;
    colorGain = ( colorGain * (256-artNNW) + 0x04 * artNNW ) >> 8;
    int colorOffset = ( artNNW * 0x18 ) >> 8;

    int strictGain = ( 0x20 * (256-nnW) + 0x4 * nnW ) >> 8;
    strictGain = ( strictGain * (256-artNNW) + 0x2 * artNNW ) >> 8;
    int strictOffset = ( 0x17 * nnW ) >> 8;
    strictOffset = ( strictOffset * (256-artNNW) + 0x0 * artNNW ) >> 8;

    int nnGain = ( 0x10 * (256-artNNW) + 0x18 * artNNW ) >> 8;

    MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK72_25_L, ((colorGain<<8)+colorOffset), 0xFF00 );
    MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK72_26_L, ((strictGain<<8)+strictOffset), 0xFF00 );
    MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK72_2D_L, nnGain, 0x001F );

}

void MDrv_UFSC_PQ_DMS_referBNF( XC_BackgroundState *backgroundState )
{

    MS_U16 Ymax_idx = backgroundState->pbgY;
    MS_U16 Umax_idx = backgroundState->pbgU;
    MS_U16 Vmax_idx = backgroundState->pbgV;
    MS_U32 confidence = backgroundState->pbgrate;


    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK26_44_L, Ymax_idx , 0x001F );
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK26_45_L, Umax_idx , 0x001F );
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK26_45_L, Vmax_idx << 8 , 0x1F00 );
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK26_46_L, confidence, 0x3FFF );

}

void MDrv_UFSC_PQ_2DPK(int menuW, MS_U16 sdw, MS_U16 fhdw , MS_U32 srEnhStr ,XC_PqWriteReg *PqWriteReg ,MS_U16 u16ArtiW)
{

    MS_U8 u8Ufsc2DPKfinalGain;
    MS_U8 u8FHDfnlGain = ( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_13_L,0x7F00) >>8);
    MS_U8 u8HDfnlGain = 0 ;
    MS_U8 u8ArtificialfnlGain = ( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1A_L,0x7F00) >> 8);
    MS_U8 u8OsdfnlGain = ( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_16_L,0x7F00) >> 8);

    u8Ufsc2DPKfinalGain = ( fhdw*u8FHDfnlGain + (256-fhdw)*u8HDfnlGain )/256;
    u8Ufsc2DPKfinalGain = ( menuW*u8OsdfnlGain + (256-menuW)*u8Ufsc2DPKfinalGain) >>8;
    u8Ufsc2DPKfinalGain = ( u16ArtiW*u8ArtificialfnlGain + (256-u16ArtiW)*u8Ufsc2DPKfinalGain)>>8;
    u8Ufsc2DPKfinalGain = (u8Ufsc2DPKfinalGain* srEnhStr )>>5;
    PqWriteReg->UFSCPk2DfinalGain = u8Ufsc2DPKfinalGain;


    MS_U8 u8Ufsc2dpkHdGainFhdPtn = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_14_L,0x001F); //0x08;
    MS_U8 u8Ufsc2dpkHdGainHdPtn = (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_14_L,0x1F00)>>8); //0x00;
    MS_U8 u8Ufsc2dpkHdGainSdPtn =  MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_15_L,0x001F) ; //0x10;

    MS_U8 u8Ufsc2dpkHdGain;
    u8Ufsc2dpkHdGain = ( u8Ufsc2dpkHdGainHdPtn*(256-fhdw) + u8Ufsc2dpkHdGainFhdPtn*fhdw )>>8;
    u8Ufsc2dpkHdGain = ( u8Ufsc2dpkHdGain*(256-sdw) + u8Ufsc2dpkHdGainSdPtn*sdw )>>8;
    u8Ufsc2dpkHdGain = minmax(u8Ufsc2dpkHdGain , 0 , 0x1F);

    MS_U8 u8MaxDifference = max( (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1C_L,0x00FF))/2 , 1);
    u8Ufsc2dpkHdGain = minmax( u8Ufsc2dpkHdGain, (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK5F_62_L,0x001F))-u8MaxDifference ,  (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK5F_62_L,0x001F))+u8MaxDifference) ;
        
    MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK5F_62_L, u8Ufsc2dpkHdGain , 0x001F );
}

void MDrv_UFSC_PQ_SPNN( int menuW, int edgePatW, int srEnhStr , int sdW2 , int fHdW2 ,XC_PqWriteReg *PqWriteReg)
{
    MS_U8 u8ReduceW = max(menuW,edgePatW);
    MS_U8 u8SpnnLgain = ( fHdW2*0x06 + (256-fHdW2)*0x08 ) >>8 ;
    u8SpnnLgain = (u8SpnnLgain*srEnhStr) >> 5;
    u8SpnnLgain = u8SpnnLgain*( 256 - u8ReduceW ) >> 8;
    u8SpnnLgain = (u8SpnnLgain*srEnhStr)>>5;
    PqWriteReg-> SPNNGL = u8SpnnLgain;

    MS_U8 u8SpnnHgain = 0x04;
    u8SpnnHgain = u8SpnnHgain*( 256 - u8ReduceW ) >> 8;
    u8SpnnHgain = (u8SpnnHgain*srEnhStr)>>5;
    PqWriteReg-> SPNNGH = u8SpnnHgain;
}

void MDrv_SC_2DPK_PQ(MS_U16 sdw, MS_U16 fhdw , MS_U32 srEnhStr ,XC_PqWriteReg *PqWriteReg ,MS_U16 u16ArtiW)
{
    MS_U8 u8SdGain = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_20_L,0x00FF);
    MS_U8 u8FhdGain = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_20_L,0xFF00)>>8;
    MS_U8 u8HdGain = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_21_L,0x00FF);
    MS_U8 u8SCArtificialfnlGain = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1A_L,0x007F);
    MS_U8 u8HdFhdGainBld;
    MS_U16 u16FinalGainBld;

    u8HdFhdGainBld = ( fhdw*u8FhdGain + (256-fhdw)*u8HdGain )/256;
    u16FinalGainBld = ( sdw*u8SdGain + (256-sdw)*u8HdFhdGainBld )/256;
    u16FinalGainBld = ( u16ArtiW*u8SCArtificialfnlGain + (256-u16ArtiW)*u16FinalGainBld)>>8;
    u16FinalGainBld = ( u16FinalGainBld * srEnhStr )>>5;
    u16FinalGainBld = minmax(u16FinalGainBld,0,127); //7bit gain
    PqWriteReg->Pk2DfinalGain = u16FinalGainBld;

}

void MDrv_UFSC_PQ_BNF(int sdw2) //Eva
{
    short bgw_fnl_gain_sd = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_2D_L, 0x00ff);
    short ori_fnl_gain = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_2D_L, 0xFf00)>>8;
    short bld_fnl_gain = ( sdw2*bgw_fnl_gain_sd + (256-sdw2)*ori_fnl_gain ) /256;
    bld_fnl_gain = minmax( bld_fnl_gain ,0 ,0x1f);

    MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK4D_36_L ,bld_fnl_gain<<8, 0x1f00 );

    //decrease skin protection
    short skinsum_gain_sd = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_2E_L, 0x000f);
    short ori_skinsum_gain = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_2E_L, 0x00f0)>>4;
    short bld_skinsum_gain = ( sdw2*skinsum_gain_sd + (256-sdw2)*ori_skinsum_gain ) /256;
    bld_skinsum_gain = minmax( bld_skinsum_gain ,0 ,0xf);

    MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK4D_2A_L ,bld_skinsum_gain<<12, 0xf000 );

}

void MDrv_SC_UFSC_WRITE_REG_UI_SHARPNESS(XC_PqWriteReg *PqWriteReg , MS_U8 u8UFSCCtrl3 ,MS_U8 ENABLE_SR_DRIVER)
{
#define maserati_2dpk_side_effect_issue
#define UI_LINK_ALL_IP  //for when UI bypass(=0) It looked still has enhancement 

    //UI Sharpness control register
    MS_U8 u8UISharpnessGain = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_11_L,0x00FF);
    // re-mapping 0~63 to 0~100 center: 0x1A : 50
    MS_U8 min_ui = 0;
    MS_U8 max_ui = 63;
    MS_U8 center_ui = 0x1A;
    //u8UISharpnessGain = minmax( u8UISharpnessGain - center_ui , min_ui - center_ui , max_ui - center_ui);

    //re-mapping 0~63 to 0~128 center: 0x1A : 64
    MS_U8 u8Mode = (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_10_L,0xFFFF)>>7)&0x01;

    if(u8Mode == 0){
        if(u8UISharpnessGain < center_ui)
            u8UISharpnessGain = (64*(u8UISharpnessGain - min_ui))/(center_ui-min_ui);
        else
            u8UISharpnessGain = 64 + (64*(u8UISharpnessGain - center_ui)/(max_ui - center_ui));
    }
    else if(u8Mode == 1)
        u8UISharpnessGain = (64-center_ui) + u8UISharpnessGain ;

    u8UISharpnessGain = minmax(u8UISharpnessGain , 0 , 128);

    MS_U8 u8SharpnessFor2DPK;
#ifdef maserati_2dpk_side_effect_issue
    u8SharpnessFor2DPK = minmax( u8UISharpnessGain , 0 , 64 ); //soc 2dpk only can decrease strength for hw issue
#else
    u8SharpnessFor2DPK = u8UISharpnessGain;
#endif

    //2DPK write final gain register
    MS_U16 u16Pk2DFinalGain,u16ErcEdgeG,u16ErcDetailG2,u16ErcDetailG3,u16NlmGain,u16UfscPk2DFinalGain ;
    MS_U32 posGain,negGain;
    MS_U16 u16SpnnLG, u16SpnnHG;
    MS_U16 u16LdefinalGain,u16LdefinalGainH,u16Prepk3x3finalGain,u16P2mfinalGain;

    
    /***** for DS FSC SD pattern too weak issue *****/
    /*MS_BOOL ds_sd_enhance_disable = (MDrv_Read2Byte(REG_FSC_BK30_10_L)>>6)&0x01;
    MS_BOOL ip_ds_en = (MApi_XC_R2BYTE(REG_SC_BK1F_10_L)>>9)&0x1;
    MS_BOOL op_ds_en = (MApi_XC_R2BYTE(REG_SC_BK1F_10_L)>>11)&0x1;
    MS_U16 u16InputHeight = MApi_XC_R2BYTE(REG_SC_BK01_06_L);
    MS_U16 u16InputWidth = MApi_XC_R2BYTE(REG_SC_BK01_07_L);
    MS_U32 u32TargetResolution = 337; //720*480>>10
    MS_U32 u32InputResolution = (u16InputHeight*u16InputWidth)>>10;
    u32InputResolution = minmax(u32InputResolution, u32TargetResolution>>2 ,u32TargetResolution);
    */

    /*if(ip_ds_en == 1 && op_ds_en == 1){
      MS_U16 u16DsSC2DPKsdGain;
      u16DsSC2DPKsdGain = PqWriteReg->Pk2DfinalGain;
      u16DsSC2DPKsdGain = (u16DsSC2DPKsdGain*u32InputResolution)/u32TargetResolution;
      PqWriteReg->Pk2DfinalGain = u16DsSC2DPKsdGain;
    }   */

    //MS_U8 u8MaxDifference = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1C_L,0xFF);
    MS_U8 u8MaxDifference = 0xFF;
    
    if( u8UFSCCtrl3 & ENABLE_SR_DRIVER)
    {
        u16Pk2DFinalGain = PqWriteReg-> Pk2DfinalGain;
        posGain = PqWriteReg-> P3x3pkPosfinalGain;
        negGain = PqWriteReg-> P3x3pkNegfinalGain;
        u16ErcEdgeG = PqWriteReg-> ERCEdgeG;

        u16ErcDetailG2 = PqWriteReg-> ERCDetailG2;
        u16ErcDetailG3 = PqWriteReg-> ERCDetailG3;
        u16NlmGain = PqWriteReg-> NLMGain;
        u16SpnnLG = PqWriteReg-> SPNNGL;
        u16SpnnHG = PqWriteReg-> SPNNGH;
        u16LdefinalGain = PqWriteReg-> LdefinalGain;
        u16LdefinalGainH = PqWriteReg-> LdefinalGainH;
        u16Prepk3x3finalGain = PqWriteReg-> Prepk3x3finalGain;
        u16P2mfinalGain = PqWriteReg-> P2mfinalGain;


        /*if(ip_ds_en == 1 && op_ds_en == 1 && ds_sd_enhance_disable != 1){
            MS_U16 u16DsSC2DPKsdGain;
            u16DsSC2DPKsdGain = u16Pk2DFinalGain;
            u16DsSC2DPKsdGain =  (u16DsSC2DPKsdGain*u32TargetResolution)/u32InputResolution;
            u16Pk2DFinalGain = u16DsSC2DPKsdGain;
        }*/
        
        u16Pk2DFinalGain = (u16Pk2DFinalGain*u8SharpnessFor2DPK)>>6;    
        u16Pk2DFinalGain = minmax( u16Pk2DFinalGain , 0 , 63 );
        
        u16UfscPk2DFinalGain = PqWriteReg->UFSCPk2DfinalGain;
        u16UfscPk2DFinalGain = (u16UfscPk2DFinalGain*u8UISharpnessGain)>>6;
        u16UfscPk2DFinalGain = minmax( u16UfscPk2DFinalGain , 0 , 63 );

        posGain = (posGain*u8UISharpnessGain)>>6;
        posGain = minmax(posGain, 0 , 0xFF);
        negGain = (negGain*u8UISharpnessGain)>>6;
        negGain = minmax(negGain, 0 , 0xFF);
        u16ErcEdgeG = (u16ErcEdgeG*u8UISharpnessGain)>>6;
        u16ErcEdgeG = minmax(u16ErcEdgeG, 0 , 0x003F);

        u16ErcDetailG2 = (u16ErcDetailG2*u8UISharpnessGain)>>6;
        u16ErcDetailG2 = minmax(u16ErcDetailG2, 0 , 0x003F);
        u16ErcDetailG3 = (u16ErcDetailG3*u8UISharpnessGain)>>6;
        u16ErcDetailG3 = minmax(u16ErcDetailG3, 0 , 0x003F);
        u16NlmGain = (u16NlmGain*u8UISharpnessGain)>>6;
        u16NlmGain = minmax(u16NlmGain, 0 , 0x00FF);

        u16SpnnLG = (u16SpnnLG*u8UISharpnessGain)>>6;
        u16SpnnLG = minmax(u16SpnnLG, 0 , 0x001F);
        u16SpnnHG = (u16SpnnHG*u8UISharpnessGain)>>6;
        u16SpnnHG = minmax(u16SpnnHG, 0 , 0x001F);
        
#ifdef UI_LINK_ALL_IP       
        u16LdefinalGain = (u16LdefinalGain * minmax(u8UISharpnessGain,0,64) )>>6;  //only decrease can't enhance IP 
        u16LdefinalGain = minmax(u16LdefinalGain, 0 , 0x1F);
        
        u16LdefinalGainH = (u16LdefinalGainH * minmax(u8UISharpnessGain,0,64) )>>6;  //only decrease can't enhance IP 
        u16LdefinalGainH = minmax(u16LdefinalGainH, 0 , 0x1F);
        
        u16Prepk3x3finalGain = (u16Prepk3x3finalGain * minmax(u8UISharpnessGain,0,64) )>>6;  //only decrease can't enhance IP 
        u16Prepk3x3finalGain = minmax(u16Prepk3x3finalGain, 0 , 0x3F);
        
        u16P2mfinalGain = (u16P2mfinalGain * minmax(u8UISharpnessGain,0,64) )>>6;  //only decrease can't enhance IP
        u16P2mfinalGain = minmax(u16P2mfinalGain, 0 , 0x3F);
#endif 

        //2DPK write final register
        u16Pk2DFinalGain = minmax( u16Pk2DFinalGain, (MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK19_13_L,0x7F00)>>8)-(u8MaxDifference/2) ,  (MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK19_13_L,0x7F00)>>8)+(u8MaxDifference/2)) ;
        u16Pk2DFinalGain = u16Pk2DFinalGain << 8;
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK19_13_L,  u16Pk2DFinalGain , 0x7F00);

        //Post3x3PK write final gain register
        posGain = minmax( posGain, (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK76_44_L,0x00FF))-u8MaxDifference ,  (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK76_44_L,0x00FF))+u8MaxDifference) ;
        negGain = minmax( negGain, (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK76_44_L,0xFF00)>>8)-u8MaxDifference ,  (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK76_44_L,0xFF00)>>8)+u8MaxDifference) ;

        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK76_44_L, posGain, 0x00FF );
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK76_44_L, negGain<<8, 0xFF00 );

        //ERC write register
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK75_58_L, u16ErcEdgeG<<8, 0x3F00 );
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK75_59_L, u16ErcDetailG2<<8, 0x3F00 );
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK75_59_L, u16ErcDetailG3, 0x003F );

        //NLM write register

        u16NlmGain = minmax( u16NlmGain, (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK70_31_L,0xFF00)>>8)-u8MaxDifference ,  (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK70_31_L,0xFF00)>>8)+u8MaxDifference) ;
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK70_31_L, u16NlmGain<<8, 0xFF00 );

        //UFSC 2DPK write register
        
        u16UfscPk2DFinalGain = minmax( u16UfscPk2DFinalGain, (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK19_13_L,0x7F00)>>8)-u8MaxDifference ,  (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK19_13_L,0x7F00)>>8)+u8MaxDifference) ;
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK19_13_L,  u16UfscPk2DFinalGain << 8 , 0x7F00);

        //SPNN write register
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK74_41_L, u16SpnnLG , 0x001F );
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK74_41_L, u16SpnnHG <<8 , 0x1F00 );
        
        //pre_enhance LDE write register
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK4C_25_L, u16LdefinalGain<<8, 0x1F00 );
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK4C_25_L, u16LdefinalGainH, 0x001F );
        
        //pre_enhance PK3x3 write register
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK4C_48_L, u16Prepk3x3finalGain, 0x003F );
        
        //P2M write register
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK72_41_L, u16P2mfinalGain, 0x003F );

    }
    else
    {
        u16Pk2DFinalGain = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_13_L, 0x003F ) ;//0x20;
        u16UfscPk2DFinalGain = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_16_L, 0x003F ) ;
        posGain = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_12_L, 0x00FF ) ;//0x30;
        negGain = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_12_L, 0xFF00 )>>8 ;//0x30;
        
        u16LdefinalGain = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_3A_L, 0x001F ) ;
        u16LdefinalGainH = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_3A_L, 0x1F00 )>>8 ;
        u16Prepk3x3finalGain = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_3B_L, 0x003F ) ;
        u16P2mfinalGain = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_3B_L, 0x3F00 )>>8 ;
        u16NlmGain = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_3C_L, 0x00FF ) ;
        u16ErcEdgeG = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_3C_L, 0x3F00 )>>8 ;
        u16ErcDetailG2 = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_3D_L, 0x003F ) ;
        u16ErcDetailG3 = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_3D_L, 0x3F00 )>>8 ;
        u16SpnnLG = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_09_L, 0x001F ) ;
        u16SpnnHG = MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_09_L, 0x1F00 )>>8 ;
    

        /*if(ip_ds_en == 1 && op_ds_en == 1 && ds_sd_enhance_disable != 1){
            MS_U16 u16DsSC2DPKsdGain;
            u16DsSC2DPKsdGain = u16Pk2DFinalGain;
            u16DsSC2DPKsdGain = (u16DsSC2DPKsdGain*u32TargetResolution)/u32InputResolution;
            u16Pk2DFinalGain = u16DsSC2DPKsdGain;
        }*/
        u16Pk2DFinalGain = u16Pk2DFinalGain*u8SharpnessFor2DPK>>6;
        u16Pk2DFinalGain = minmax( u16Pk2DFinalGain , 0 , 63 );
        
        u16UfscPk2DFinalGain = (u16UfscPk2DFinalGain*u8UISharpnessGain)>>6;
        u16UfscPk2DFinalGain = minmax( u16UfscPk2DFinalGain , 0 , 63 );
        
        posGain = posGain*u8UISharpnessGain>>6;
        posGain = minmax(posGain, 0 , 0xFF);
        negGain = negGain*u8UISharpnessGain>>6;
        negGain = minmax(negGain, 0 , 0xFF);
        
        //these function only decrease ui sharpness
#ifdef UI_LINK_ALL_IP

        u16LdefinalGain =(u16LdefinalGain * minmax(u8UISharpnessGain,0,64) )>>6;  //only decrease can't enhance IP 
        u16LdefinalGainH = (u16LdefinalGainH * minmax(u8UISharpnessGain,0,64) )>>6;  //only decrease can't enhance IP 
        u16Prepk3x3finalGain = (u16Prepk3x3finalGain * minmax(u8UISharpnessGain,0,64) )>>6;  //only decrease can't enhance IP 
        u16P2mfinalGain = (u16P2mfinalGain * minmax(u8UISharpnessGain,0,64) )>>6;  //only decrease can't enhance IP 
        u16NlmGain = (u16NlmGain * minmax(u8UISharpnessGain,0,64) )>>6;  //only decrease can't enhance IP 
        u16ErcEdgeG = (u16ErcEdgeG * minmax(u8UISharpnessGain,0,64) )>>6;  //only decrease can't enhance IP 
        u16ErcDetailG2 = (u16ErcDetailG2 * minmax(u8UISharpnessGain,0,64) )>>6;  //only decrease can't enhance IP 
        u16ErcDetailG3 = (u16ErcDetailG3 * minmax(u8UISharpnessGain,0,64) )>>6;  //only decrease can't enhance IP 
        u16SpnnLG = (u16SpnnLG * minmax(u8UISharpnessGain,0,64) )>>6;  //only decrease can't enhance IP 
        u16SpnnHG = (u16SpnnHG * minmax(u8UISharpnessGain,0,64) )>>6;  //only decrease can't enhance IP 
        
#endif      

        //2DPK write final register
        u16Pk2DFinalGain = u16Pk2DFinalGain << 8;
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK19_13_L,  u16Pk2DFinalGain , 0x7F00);
        
        //UFSC 2DPK write register
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK19_13_L,  u16UfscPk2DFinalGain << 8 , 0x7F00);

        //Post3x3PK write final gain register
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK76_44_L, posGain, 0x00FF );
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK76_44_L, negGain<<8, 0xFF00 );
        
        //pre_enhance LDE write register
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK4C_25_L, u16LdefinalGain<<8, 0x1F00 );
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK4C_25_L, u16LdefinalGainH, 0x001F );
        
        //pre_enhance PK3x3 write register
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK4C_48_L, u16Prepk3x3finalGain, 0x003F );
        
        //P2M write register
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK72_41_L, u16P2mfinalGain, 0x003F );


        //ERC write register
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK75_58_L, u16ErcEdgeG<<8, 0x3F00 );
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK75_59_L, u16ErcDetailG2<<8, 0x3F00 );
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK75_59_L, u16ErcDetailG3, 0x003F );

        //NLM write register
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK70_31_L, u16NlmGain<<8, 0xFF00 );
        
        //SPNN write register
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK74_41_L, u16SpnnLG , 0x001F );
        MApi_GFLIP_XC_W2BYTEMSK( REG_FSC_BK74_41_L, u16SpnnHG <<8 , 0x1F00 );
    }


}



/*********************Eva modify 2 end********************************************/

//===================================== function about film or dms ...etc =============
/******************************************************************************/
///Read VCnt value
///@return MS_U8: VCnt value
/******************************************************************************/
MS_U8 MDrv_SC_read_v_cnt(void)
{
    MS_U8 u8VCnt = 0;
    u8VCnt = (MS_U8)(MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK22_0E_L, 0xC0) >> 6);
    return u8VCnt;
}

XC_HpfDefaultRegSetting MDrv_SC_SaveHpfSetting( const MS_U8 isSourceChange )
{
    static XC_HpfDefaultRegSetting reg;

    if(  isSourceChange == 1 )
    {

        reg.HDSDD0_det_mode = (MS_BOOL)( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_70_L, 0x0001) );
        reg.HDSDD1_det_mode = (MS_BOOL)( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_70_L, 0x0010) >> 4 );

        reg.HDSDD0_det_mode_shift = (MS_U8)( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_70_L, 0x0700) >> 8 );
        reg.HDSDD1_det_mode_shift = (MS_U8)( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_70_L, 0x7000) >> 12 );

        reg.HDSDD0_det_mode_threshold = (MS_U16)( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_71_L, 0xFFFF)  );
        reg.HDSDD1_det_mode_threshold = (MS_U16)( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_72_L, 0xFFFF)  );

        reg.HDSDD_underflow_threshold = (MS_U8)( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_73_L, 0x003F)  );
        reg.HDSDD_overflow_threshold = (MS_U8)( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK02_73_L, 0x3F00) >> 8 );
    }
    return reg;
}

/******************************************************************************/
///Read motion value (F2 motion status)
///@return MS_U8: Motion value
/******************************************************************************/
MS_U32 MDrv_SC_read_motion_value1(void)
{
    MS_U32 u32MotionValue = 0;
    MS_U32 u32RegMadi_1C = 0, u32RegMadi_1B = 0, u32RegMadi_1A = 0;

    u32RegMadi_1C = (MS_U32)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK22_0E_L, 0x3F);
    u32RegMadi_1B = (MS_U32)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK22_0D_L, 0xFF00);
    u32RegMadi_1A = (MS_U32)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK22_0D_L, 0x00FF);

    u32RegMadi_1C = u32RegMadi_1C * 0x10000UL;

    u32MotionValue = (u32RegMadi_1C + u32RegMadi_1B + u32RegMadi_1A)  ;

    // DRVSCA_DBG(PRT_TRP("MotionValue = 0x%x\n", u32MotionValue));  //removed when move to kernel (don't know how to add)

    return u32MotionValue;
}

/******************************************************************************/
///Read motion value (Motion count status)
///@return MS_U8: Motion value
/******************************************************************************/
MS_U32 MDrv_SC_read_motion_value2(void)
{
    MS_U32 u32MotionValue = 0;

    u32MotionValue = (MS_U32)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK0A_1A_L, 0x7F);

    return u32MotionValue;
}

MS_U16 MDrv_SC_read_motion_value4(void)
{
    MS_U16 motionValue = 0;

    motionValue = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK0A_2E_L, 0x7FFF);

    return motionValue;
}

#define Confirm_Gain         5
MS_U16 MDrv_SC_Agate_ME1_UC_InSearchRange_MR_Offset(MS_U16 *me1IsMv0, MS_U16 *me1IsNonMv0, MS_U16 *me1IsNotFound )
{
    MS_U16 MV0_Comfirm = 0,  nonMV0_Comfirm = 0, MV_Contradict = 0;
    static MS_U16 Prev_MV0_Comfirm = 0;
    static MS_U16 Prev_nonMV0_Comfirm = 0;
    static MS_U16 Prev_MV_Contradict = 0;
    MS_U16 MV_Comfirm = 0;
    MS_U16 MR_Offset = 0;

    MV0_Comfirm = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_48_L,0xFFFF);
    nonMV0_Comfirm = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_49_L,0xFFFF);
    MV_Contradict = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_4A_L,0xFFFF);

    MV0_Comfirm = (Prev_MV0_Comfirm * 3 + MV0_Comfirm) >> 2;
    Prev_MV0_Comfirm = MV0_Comfirm;

    nonMV0_Comfirm = (Prev_nonMV0_Comfirm * 3 + nonMV0_Comfirm) >> 2;
    Prev_nonMV0_Comfirm = nonMV0_Comfirm;

    MV_Contradict = (Prev_MV_Contradict * 3 + MV_Contradict) >> 2;
    Prev_MV_Contradict = MV_Contradict;

    MV_Comfirm = MV0_Comfirm + nonMV0_Comfirm;

    if(MV_Comfirm > (MV_Contradict * Confirm_Gain))
        MR_Offset = nonMV0_Comfirm;
    else
        MR_Offset = 0;

    *me1IsMv0 = MV0_Comfirm;
    *me1IsNonMv0 = nonMV0_Comfirm;
    *me1IsNotFound = MV_Contradict;

    return MR_Offset;
}


MS_U16 MDrv_SC_Agate_ME2_UC_InSearchRange_MR_Offset(MS_U16 *me2IsMv0, MS_U16 *me2IsNonMv0, MS_U16 *me2IsNotFound)
{
    MS_U16 MV0_Comfirm = 0,  nonMV0_Comfirm = 0, MV_Contradict = 0;
    static MS_U16 Prev_MV0_Comfirm = 0;
    static MS_U16 Prev_nonMV0_Comfirm = 0;
    static MS_U16 Prev_MV_Contradict = 0;
    MS_U16 MV_Comfirm = 0;
    MS_U16 MR_Offset = 0;

    MV0_Comfirm = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_4B_L,0xFFFF);
    nonMV0_Comfirm = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_4C_L,0xFFFF);
    MV_Contradict = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_4D_L,0xFFFF);

    MV0_Comfirm = (Prev_MV0_Comfirm * 3 + MV0_Comfirm) >> 2;
    Prev_MV0_Comfirm = MV0_Comfirm;

    nonMV0_Comfirm = (Prev_nonMV0_Comfirm * 3 + nonMV0_Comfirm) >> 2;
    Prev_nonMV0_Comfirm = nonMV0_Comfirm;

    MV_Contradict = (Prev_MV_Contradict * 3 + MV_Contradict) >> 2;
    Prev_MV_Contradict = MV_Contradict;

    MV_Comfirm = MV0_Comfirm + nonMV0_Comfirm;

    if(MV_Comfirm > (MV_Contradict * Confirm_Gain))
        MR_Offset = nonMV0_Comfirm;
    else
        MR_Offset = 0;

    *me2IsMv0 = MV0_Comfirm;
    *me2IsNonMv0 = nonMV0_Comfirm;
    *me2IsNotFound = MV_Contradict;

    return MR_Offset;
}

MS_U32 MDrv_SC_A5_ColorPixelSum(void)
{
    MS_U32 colorSum;
    static MS_U32 colorSum_iir = 0;

    colorSum  = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK55_1E_L,0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_1F_L, 0x000F )<<16); //move to UFSC IN Masrati
    colorSum  = colorSum / 8;

    if( colorSum > 255 )
        colorSum = 255;

    colorSum = ((colorSum_iir*3 )+ colorSum ) >> 2;
    colorSum_iir = colorSum;

    return colorSum_iir;
}

MS_U32 MDrv_SC_Agate_ComplexSum(void)
{
    static MS_U32 IsComplex_iir = 0xFFFF;
    MS_U32 IsComplex = 0;

    IsComplex = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_78_L,0xFFFF);

    IsComplex = (IsComplex_iir*3 + IsComplex ) >> 2;
    IsComplex_iir = IsComplex;

    return IsComplex_iir;
}

MS_BOOL MDrv_SC_Agate_SceneChangeMc(const MS_U16 me1IsMv0, const MS_U16 me1IsNonMv0, const MS_U16 me1IsNotFound,
                    const MS_U16 me2IsMv0, const MS_U16 me2IsNonMv0, const MS_U16 me2IsNotFound)
{
    MS_U16 me1Comfirm = 0;
    MS_U16 me2Comfirm = 0;
    static MS_BOOL isNotFound           = false;
    static MS_BOOL isScencChange        = false;

    static MS_U8 notFoundDelayCnt = 0;
    static MS_U8 scencChangeDelayCnt = 0;

    me1Comfirm = me1IsMv0 + me1IsNonMv0;
    me2Comfirm = me2IsMv0 + me2IsNonMv0;

    if((me1IsNotFound > me1Comfirm*5)&&(me2IsNotFound > me2Comfirm*5) &&
        (me1IsNotFound > 0x10) && (me2IsNotFound > 0x10) )/* not found*/
    {
        isNotFound = true;
    }

    if( isNotFound )
    {
        if( notFoundDelayCnt != 60 )
            notFoundDelayCnt++;
    }

    if( notFoundDelayCnt > 10 )
    {
        if((me1IsNotFound*5 < me1Comfirm)&&(me2IsNotFound*5 < me2Comfirm) &&
            (me1Comfirm > 0x10) && (me2Comfirm > 0x10) )/* found */
        {
            isScencChange = true;
        }
        else
        {
            isScencChange = false;
        }

        notFoundDelayCnt = 0;
        isNotFound = false;
    }

    if( isScencChange )
    {
        if( scencChangeDelayCnt != 60 )
            scencChangeDelayCnt++;
    }

    if( scencChangeDelayCnt > 5 )
    {
        scencChangeDelayCnt = 0;
        isScencChange = false;
    }

    return isScencChange;
}

#define OOSRF_Gain          4
#define OOSRF_Power        2
MS_U32 MDrv_SC_McInSearchRangeUseFeather( void )
{
    MS_U32 before_MC_feather = 0, after_MC_feather = 0;
    MS_U32 MC_OOSRF = 0;
    static MS_U32 Prev_before_MC_feather = 0;
    static MS_U32 Prev_after_MC_feather = 0;

    before_MC_feather = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_77_L,0xFFFF) ;//mv0_feather
    after_MC_feather = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_7B_L,0xFFFF);//nonMv0_feather

    before_MC_feather = (Prev_before_MC_feather*3 + before_MC_feather ) >> 2;
    Prev_before_MC_feather = before_MC_feather;

    after_MC_feather = (Prev_after_MC_feather*3 + after_MC_feather ) >> 2;
    Prev_after_MC_feather = after_MC_feather;

    if( before_MC_feather > (after_MC_feather*OOSRF_Gain) >> OOSRF_Power  )//mv0_feather >> nonMv0_feather: MCDI is correct, therefore in search range
        MC_OOSRF = before_MC_feather - ((after_MC_feather*OOSRF_Gain) >> OOSRF_Power) ;
    else
        MC_OOSRF = 0;

    return MC_OOSRF;  // "Big": in seach range "Small": out of seach range
}

MS_U32 MDrv_SC_Agate_TotalFeather(void)
{
    MS_U32 mv0_feather, nonMv0_feather;
    MS_U32 total_feather;
    static MS_U32 total_feather_iir = 255;

    mv0_feather      = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_77_L,0xFFFF);
    nonMv0_feather   = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_7B_L,0xFFFF);

    total_feather = ( mv0_feather + nonMv0_feather ) / 4;

    if( total_feather > 255 )
        total_feather = 255;

    total_feather = ((total_feather_iir*3 )+ total_feather ) >> 2;
    total_feather_iir = total_feather;

    return total_feather_iir;
}

MS_U32 MDrv_SC_TotalFeather_Sum(void)
{
    MS_U32 mv0_feather, nonMv0_feather;
    MS_U32 total_feather;
    static MS_U32 total_feather_iir = 0;

    mv0_feather      = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_77_L,0xFFFF);
    nonMv0_feather   = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_7B_L,0xFFFF);

    total_feather =  mv0_feather + nonMv0_feather;

    total_feather = ((total_feather_iir*3 )+ total_feather ) >> 2;
    total_feather_iir = total_feather;

    return total_feather_iir;
}

MS_U16 MDrv_SC_Agate_ME1_nonMv0Comfirm(void)
{
    MS_U16 nonmv0Comfirm = 0;
    static MS_U16 nonmv0Comfirm_iir = 0;

    nonmv0Comfirm = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_49_L,0xFFFF);

    nonmv0Comfirm = ((nonmv0Comfirm_iir*3 )+ nonmv0Comfirm ) >> 2; //evaneedtoaddback
    nonmv0Comfirm_iir = nonmv0Comfirm;

    return nonmv0Comfirm_iir;
}

MS_U16 MDrv_SC_Agate_ME1_mv0Comfirm(void)
{
    MS_U16 mv0Comfirm = 0;
    static MS_U16 mv0Comfirm_iir = 0;

    mv0Comfirm = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_48_L,0xFFFF);

    mv0Comfirm = ((mv0Comfirm_iir*3 )+ mv0Comfirm ) >> 2; //evaneedtoaddback
    mv0Comfirm_iir = mv0Comfirm;

    return mv0Comfirm_iir;
}

MS_U16 MDrv_SC_Agate_ME1_mvNotFound(void)
{
    MS_U16 mvNotFound = 0;
    static MS_U16 mvNotFound_iir = 0;

    mvNotFound = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_4A_L,0xFFFF);

    mvNotFound = ((mvNotFound_iir*3 )+ mvNotFound ) >> 2; //evaneedtoaddback
    mvNotFound_iir = mvNotFound;

    return mvNotFound_iir;
}

MS_U16 MDrv_SC_Agate_ME2_nonMv0Comfirm(void)
{
    MS_U16 nonmv0Comfirm = 0;
    static MS_U16 nonmv0Comfirm_iir = 0;

    nonmv0Comfirm = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_4C_L,0xFFFF);

    nonmv0Comfirm = ((nonmv0Comfirm_iir*3 )+ nonmv0Comfirm ) >> 2; //evaneedtoaddback
    nonmv0Comfirm_iir = nonmv0Comfirm;

    return nonmv0Comfirm_iir;
}

MS_U16 MDrv_SC_Agate_ME2_mv0Comfirm(void)
{
    MS_U16 mv0Comfirm = 0;
    static MS_U16 mv0Comfirm_iir = 0;

    mv0Comfirm = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_4B_L,0xFFFF);

    mv0Comfirm = ((mv0Comfirm_iir*3 )+ mv0Comfirm ) >> 2; //evaneedtoaddback
    mv0Comfirm_iir = mv0Comfirm;

    return mv0Comfirm_iir;
}

MS_U16 MDrv_SC_Agate_ME2_mvNotFound(void)
{
    MS_U16 mvNotFound = 0;
    static MS_U16 mvNotFound_iir = 0;

    mvNotFound = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_4D_L,0xFFFF);

    mvNotFound = ((mvNotFound_iir*3 )+ mvNotFound ) >> 2; //evaneedtoaddback
    mvNotFound_iir = mvNotFound;

    return mvNotFound_iir;
}

XC_Me1HisStates MDrv_SC_Agate_Hierarchy1( void )
{
    MS_U8 me1_X_histogram[9];
    MS_U8 me1_Y_histogram[9];
    MS_U8 minXdir = 0, minXdirBin = 0;
    MS_U8 minYdir = 0, minYdirBin = 0;
    XC_Me1HisStates H1States;
    int dir = 0;

    me1_X_histogram[0] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_60_L, 0x00FF);
    me1_X_histogram[1] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_60_L, 0xFF00) >> 8;
    me1_X_histogram[2] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_61_L, 0x00FF);
    me1_X_histogram[3] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_61_L, 0xFF00) >> 8;
    me1_X_histogram[4] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_62_L, 0x00FF);
    me1_X_histogram[5] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_62_L, 0xFF00) >> 8;
    me1_X_histogram[6] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_63_L, 0x00FF);
    me1_X_histogram[7] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_63_L, 0xFF00) >> 8;
    me1_X_histogram[8] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_5F_L, 0x00FF);

    me1_Y_histogram[0] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_68_L, 0x00FF);
    me1_Y_histogram[1] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_68_L, 0xFF00) >> 8;
    me1_Y_histogram[2] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_69_L, 0x00FF);
    me1_Y_histogram[3] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_69_L, 0xFF00) >> 8;
    me1_Y_histogram[4] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_6A_L, 0x00FF);
    me1_Y_histogram[5] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_6A_L, 0xFF00) >> 8;
    me1_Y_histogram[6] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_6B_L, 0x00FF);
    me1_Y_histogram[7] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_6B_L, 0xFF00) >> 8;
    me1_Y_histogram[8] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_5F_L, 0xFF00) >> 8;

    minXdir = 0;
    minXdirBin = 0;
    for ( dir = 0; dir <= 8 ; dir ++ )
    {
        if( minXdirBin < me1_X_histogram[dir] )
        {
            minXdir = dir;
            minXdirBin = me1_X_histogram[dir];
        }
    }

    minYdir = 0;
    minYdirBin = 0;
    for ( dir = 0; dir <= 8 ; dir ++ )
    {
        if( minYdirBin < me1_Y_histogram[dir] )
        {
            minYdir = dir;
            minYdirBin = me1_Y_histogram[dir];
        }
    }
    H1States.minMvXdir = minXdir;
    H1States.minMvXBin = minXdirBin;
    H1States.minMvYdir = minYdir;
    H1States.minMvYBin = minYdirBin;

    return H1States;
}

XC_Me1HisStates MDrv_SC_Agate_Hierarchy2( const XC_Me1HisStates H1 )
{
    MS_U8 me1_X_histogram[8];
    MS_U8 me1_Y_histogram[2];
    MS_U8 minXdir = 0, minXdirBin = 0;
    MS_U8 minYdir = 0, minYdirBin = 0;
    XC_Me1HisStates H2States;
    int dir = 0;

    MApi_GFLIP_XC_W2BYTEMSK( REG_SC_BK17_5E_L,  H1.minMvXdir , 0x0007 ); /*reg_gmv_h_hier2_sel*/
    MApi_GFLIP_XC_W2BYTEMSK( REG_SC_BK17_5E_L,  H1.minMvYdir<<4 , 0x0070 ); /*reg_gmv_h_hier2_sel*/

    me1_X_histogram[0] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_64_L, 0x00FF);
    me1_X_histogram[1] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_64_L, 0xFF00) >> 8;
    me1_X_histogram[2] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_65_L, 0x00FF);
    me1_X_histogram[3] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_65_L, 0xFF00) >> 8;
    me1_X_histogram[4] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_66_L, 0x00FF);
    me1_X_histogram[5] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_66_L, 0xFF00) >> 8;
    me1_X_histogram[6] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_67_L, 0x00FF);
    me1_X_histogram[7] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_67_L, 0xFF00) >> 8;

    me1_Y_histogram[0] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_6C_L, 0x00FF);
    me1_Y_histogram[1] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_6C_L, 0xFF00) >> 8;

   /*FOR Vertical move 8 pixels*/
   if((H1.minMvYdir == 7)&&(abs (H1.minMvYBin -(me1_Y_histogram[0] +me1_Y_histogram[1] ))>3))//MV = 7 or 8
    {
        me1_Y_histogram[1] = (MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_6B_L, 0xFF00) >> 8) - MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_6C_L, 0x00FF); // MV+8 =  (H1+7)  - (MV+7)
    }
   else if(H1.minMvYdir == 6 )//MV = 5or 6
    {
        me1_Y_histogram[0]= MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_6B_L, 0x00FF)  - (MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_6C_L, 0xFF00)>>8);  // MV+5= (H1+6) - (MV+6)
    }

    //find max histogram
    minXdir = 0;
    minXdirBin = 0;
    for (dir=0; dir<=7 ; dir++)
    {
        if( minXdirBin < me1_X_histogram[dir] )
        {
            minXdir = dir;
            minXdirBin = me1_X_histogram[dir];
        }
    }

    minYdir = 0;
    minYdirBin = 0;
    for (dir=0; dir<=1 ; dir++)
    {
        if( minYdirBin < me1_Y_histogram[dir] )
        {
            minYdir = dir;
            minYdirBin = me1_Y_histogram[dir];
        }
    }

    if( H1.minMvXdir == 8 ) //MV0
    {
        H2States.minMvXdir = H1.minMvXdir;
        H2States.minMvXBin = H1.minMvXBin;
    }
    else
    {
        H2States.minMvXdir = minXdir;
        H2States.minMvXBin = minXdirBin;
    }

    if( H1.minMvYdir == 8 )
    {
        H2States.minMvYdir = H1.minMvYdir;
        H2States.minMvYBin = H1.minMvYBin;
    }
    else
    {
        H2States.minMvYdir = minYdir;
        H2States.minMvYBin = minYdirBin;
    }

    return H2States;
}

XC_GMvStates MDrv_SC_Agate_GMV( const XC_Me1HisStates H1,  const XC_Me1HisStates H2 )
{
    XC_GMvStates gMv;
    memset(&gMv, 0x0, sizeof(XC_GMvStates));
//  MS_BOOL yIsMovingOdd;
//  MS_BOOL xIsMovingOdd;
//  MS_BOOL isMovingOdd;

    const MS_U8 const_GMV_X_LUT[9][8] =
    {
        {0, 1,  2,  3,  4,  5,  6,  7 },
        {8, 9,  10, 11, 12, 13, 14, 15},
        {16,    17, 18, 19, 20, 21, 22, 23},
        {24,    25, 26, 27, 28, 29, 30, 31},
        {33,    34, 35, 36, 37, 38, 39, 40},
        {41,    42, 43, 44, 45, 46, 47, 48},
        {49,    50, 51, 52, 53, 54, 55, 56},
        {57,    58, 59, 60, 61, 62, 63, 64},
        {32,    32, 32, 32, 32, 32, 32, 32}
    };

    const MS_U8 const_GMV_Y_LUT[9][2] =
    {
        {0,     1},
        {2,     3},
        {4,     5},
        {6,     7},
        {9,     10},
        {11,    12},
        {13,    14},
        {15,    16},
        {8,     8}
    };

    MS_U8 x_h1_idx = H1.minMvXdir;
    MS_U8 x_h2_idx = ( H1.minMvXdir == 8 ) ? 0 : H2.minMvXdir;
    MS_U8 y_h1_idx = H1.minMvYdir;
    MS_U8 y_h2_idx = ( H1.minMvYdir == 8 ) ? 0 : H2.minMvYdir;

    MS_U8 gm_x = const_GMV_X_LUT[x_h1_idx][x_h2_idx];
    MS_U8 gm_y = const_GMV_Y_LUT[y_h1_idx][y_h2_idx];

//  xIsMovingOdd = ( gm_x % 2 ) ? true : false;
//  yIsMovingOdd = ( gm_y % 2 ) ? true : false;
//  isMovingOdd = ( xIsMovingOdd | yIsMovingOdd );

    gMv.minMvXdir   = gm_x;
    //gMv.minMvXbin = H1.minMvXBin;
    gMv.h1XYdiff    = abs(H1.minMvXBin-H1.minMvYBin);
    gMv.h1XYsum     = H1.minMvXBin+H1.minMvYBin;
    gMv.minMvYdir   = gm_y;
    //gMv.minMvYbin = H1.minMvYBin;
    gMv.h2XYdiff    = abs(H2.minMvXBin-H2.minMvYBin);
    gMv.h2XYsum     = H2.minMvXBin+H2.minMvYBin;
    gMv.yMvIsMovingOdd = ( gm_y % 2 ) ? true : false;
#if 1
    //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_08_L,  (gm_x<<8), 0xFF00);
    //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_08_L,  gm_y, 0x00FF);

    //DEBUG_LC_Full6bit(gm_x);
    //DEBUG_LC_Full6bit2(gm_y);

    //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_09_L,  (gMv.h2XYdiff), 0xFFFF);
    //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_09_L,  (gMv.h1XYdiff<<8), 0xFF00);
    //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_09_L,  gMv.h2XYdiff, 0x00FF);
    //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_0A_L,  (gMv.h2XYsum), 0xFFFF);
    //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_0A_L,  (((gMv.h1XYsum)>>2)<<8), 0xFF00);
    //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_0A_L,  ((gMv.h2XYsum)>>2), 0x00FF);
#endif

    return gMv;
}

#define HIS_MOTION_TH 0x0A
MS_BOOL MDrv_SC_Bouncing_Status(const MS_U32 u32MotionValue)
{
    if( u32MotionValue < HIS_MOTION_TH )
    {
        return TRUE;                            //Still pattern
    }
    else
    {
        return FALSE;                           //Moving pattern
    }
}

#define HIS_MOTION_TH2 0x30
MS_BOOL MDrv_SC_Bouncing_Status2(const MS_U32 u32MotionValue)
{
    if( u32MotionValue < HIS_MOTION_TH2 )
    {
        return TRUE;                            //Still pattern
    }
    else
    {
        return FALSE;                           //Moving pattern
    }
}

XC_FilmStates MDrv_SC_Film_Status(void)
{
    XC_FilmStates filmstatus;

    filmstatus.film22 = FALSE;
    filmstatus.film32 = FALSE;
    filmstatus.filmany = FALSE;
    filmstatus.film_act = FALSE;

    if((MS_U16)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0008) == 0x0008)   // film22
    {
        filmstatus.film22 = TRUE;
    }
    if((MS_U16)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010)   // film32
    {
        filmstatus.film32 = TRUE;
    }

    if((MS_U16)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040)   // film any
    {
        filmstatus.filmany = TRUE;
    }

    if(filmstatus.film22 ||filmstatus.film32 ||filmstatus.filmany)
    {
        filmstatus.film_act = TRUE;
    }

    return filmstatus;
}


MS_BOOL MDrv_SC_McFilmLike( const XC_OpFrameStates opFrameStatusCur, MS_BOOL isHd )
{
    static XC_OpFrameStates opFrameStatusLast;
    static XC_McFilmStates mcFilmStates;
    MS_BOOL  isFilm = FALSE;
    static MS_U8 isFilmCnt = 0;
    static MS_U8 continuousError = 0;
    MS_U8 lowerBoundTh = 0;

    if( isHd )
        lowerBoundTh = 0xFF;
    else
      #if SZ_PATTERN_ENABLE
      lowerBoundTh = 0x80;
      #else
            lowerBoundTh = 0x40;
      #endif

    mcFilmStates.filmLikePrvPixels = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_75_L, 0xFFFF);
    mcFilmStates.filmLikeNxtPixels = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_76_L, 0xFFFF);

    if( opFrameStatusCur.curOpFrame != opFrameStatusLast.curOpFrame )
    {
        if(((mcFilmStates.filmLikePrvPixels>(mcFilmStates.filmLikeNxtPixels*5))&&(mcFilmStates.filmLikePrvPixels > lowerBoundTh))
            ||((mcFilmStates.filmLikeNxtPixels>(mcFilmStates.filmLikePrvPixels*5))&&(mcFilmStates.filmLikeNxtPixels > lowerBoundTh)))
        {
            if( isFilmCnt < 255 )
                isFilmCnt++;

            continuousError = 0;
        }
        else
        {
            if( isFilmCnt > 0 )
                isFilmCnt--;

            if( continuousError < 255 )
                continuousError++;
        }

        if( continuousError > 4 )
            isFilmCnt = 0;
    }

    opFrameStatusLast.curOpFrame = opFrameStatusCur.curOpFrame;

    isFilm = ( isFilmCnt > 10 ) ? true : false;

    return isFilm;
}

MS_BOOL MDrv_SC_McFilmLikeHd( const XC_OpFrameStates opFrameStatusCur, MS_BOOL isHd )
{
    static XC_OpFrameStates opFrameStatusLast;
    static XC_McFilmStates mcFilmStates;
    MS_BOOL  isFilm = FALSE;
    static MS_U8 isFilmCnt = 0;
    static MS_U8 continuousError = 0;
    MS_U8 lowerBoundTh = 0;

    if( isHd )
        lowerBoundTh = 0xFF;
    else
        lowerBoundTh = 0x40;

    mcFilmStates.filmLikePrvPixels = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_75_L, 0xFFFF);
    mcFilmStates.filmLikeNxtPixels = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_76_L, 0xFFFF);

    if( opFrameStatusCur.curOpFrame != opFrameStatusLast.curOpFrame )
    {
        if(((mcFilmStates.filmLikePrvPixels>(mcFilmStates.filmLikeNxtPixels*15))&&(mcFilmStates.filmLikePrvPixels > lowerBoundTh ))      /*Sam modify*/
            ||((mcFilmStates.filmLikeNxtPixels>(mcFilmStates.filmLikePrvPixels*15))&&(mcFilmStates.filmLikeNxtPixels > lowerBoundTh )))  /*Sam modify*/
        {
            if( isFilmCnt < 255 )
                isFilmCnt++;

            continuousError = 0;
        }
        else
        {
            if( isFilmCnt > 0 )
                isFilmCnt--;

            if( continuousError < 255 )
                continuousError++;
        }

        if( continuousError > 4 )
            isFilmCnt = 0;
    }

    opFrameStatusLast.curOpFrame = opFrameStatusCur.curOpFrame;

    isFilm = ( isFilmCnt > 10 ) ? true : false;

    return isFilm;
}

#define TOTAL_MV_TIME 10
MS_U32 MDrv_SC_Agate_TotalMvComfirm(void)
{
    MS_U32 mv0 = 0, nonMv0 = 0, notFound = 0;
    MS_U32 total_mv = 0, mvComfirm = 0;
    static MS_U32 mvComfirm_iir = 0;

    mv0      = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_48_L,0xFFFF);
    nonMv0   = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_49_L,0xFFFF);
    notFound   = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_4A_L,0xFFFF);

    total_mv = mv0 + nonMv0;

    if( total_mv > (notFound*TOTAL_MV_TIME))
        mvComfirm =   total_mv - notFound*TOTAL_MV_TIME;
    else
        mvComfirm = 0;

    if( mvComfirm > 255 )
        mvComfirm = 255;

    mvComfirm = ((mvComfirm_iir*3 )+ mvComfirm ) >> 2;
    mvComfirm_iir = mvComfirm;

    return mvComfirm_iir;
}

MS_BOOL MDrv_SC_Agate_ME1Still(void)
{
    MS_U32 mv0 = 0, nonMv0 = 0;
    MS_BOOL isStill = FALSE;

    static MS_U32 mv0_iir = 0;
    static MS_U32 nonMv0_iir = 0;

    mv0      = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_48_L,0xFFFF);
    nonMv0   = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_49_L,0xFFFF);

    if( mv0_iir > (nonMv0_iir*6) )
        isStill = true;
    else
        isStill = false;

    mv0 = ((mv0_iir*3 )+ mv0 ) >> 2;
    mv0_iir = mv0;

    nonMv0 = ((nonMv0_iir*3 )+ nonMv0 ) >> 2;
    nonMv0_iir = nonMv0;

    return isStill;
}

MS_U16 MDrv_SC_Agate_FeatherAndMotion(void)
{
    MS_U16 FAM = 0;
    FAM = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_7F_L,0xFFFF); //feather and motion pixels
    return FAM;
}

MS_BOOL MDrv_SC_Agate_MovingWithoutFeather(void)
{
    MS_U32 mv0_feather = 0, nonMv0_feather = 0;
    MS_BOOL isMovingWithoutFeather = FALSE;

    static MS_U32 mv0_feather_iir = 0;
    static MS_U32 nonMv0_feather_iir = 0;

    mv0_feather      = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_77_L,0xFFFF);
    nonMv0_feather   = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_7B_L,0xFFFF);

    if( mv0_feather_iir > (nonMv0_feather_iir*60) )
        isMovingWithoutFeather = true;
    else
        isMovingWithoutFeather = false;

    mv0_feather = ((mv0_feather_iir*3 )+ mv0_feather ) >> 2;
    mv0_feather_iir = mv0_feather;

    nonMv0_feather = ((nonMv0_feather_iir*3 )+ nonMv0_feather ) >> 2;
    nonMv0_feather_iir = nonMv0_feather;

    return isMovingWithoutFeather;
}

#define Feather_Sum_TH 0x1500
#define Feather_Sum_TH2 0x230
MS_BOOL MDrv_SC_TotalFeather_Sum_Status(const MS_U16 featherAndMotion, const MS_BOOL isSD_source, MS_BOOL *Feather_sum_status2 )
{
    MS_U32 SD2HD_Feather = 0;
    static MS_U32 SD2HD_Feather_iir = 0;
    MS_BOOL isFeatherAndMotion = ( featherAndMotion > 0x10 ) ? true : false;

    MS_BOOL isFeather1 = FALSE, isFeather2 = FALSE;

    static MS_U8 confirm_cnt1 = 0;
    static MS_U8 confirm_cnt2 = 0;

    SD2HD_Feather    = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK23_56_L,0xFFFF);
    SD2HD_Feather = ((SD2HD_Feather_iir*3 )+ SD2HD_Feather ) >> 2;
    SD2HD_Feather_iir = SD2HD_Feather;

    if( (SD2HD_Feather_iir > Feather_Sum_TH2) &&(isSD_source) && (!isFeatherAndMotion) )
        //*Feather_sum_status2 = true;
        isFeather2 = true;
    else
        //*Feather_sum_status2 = false;
        isFeather2 = false;

    if( (SD2HD_Feather_iir > Feather_Sum_TH) &&(isSD_source) && (!isFeatherAndMotion) )
        isFeather1 = true;
    else
        isFeather1 = false;

    if( isFeather2 )
    {
        if( confirm_cnt2 < 120 )
            confirm_cnt2++;
    }
    else
    {
        confirm_cnt2 = 0;
    }

    if( isFeather1 )
    {
        if( confirm_cnt1 < 120 )
            confirm_cnt1++;
    }
    else
    {

        confirm_cnt1 = 0;
    }


    if( confirm_cnt2 > 60 )
        *Feather_sum_status2 = true;
    else
        *Feather_sum_status2 = false;

    return ( confirm_cnt1 > 60 );
}

/******************************************************************************/
///LetterBox Driver
/******************************************************************************/
XC_LetterBoxState MDrv_SC_Letter_Box_Driver(  const MS_BOOL isSceneChange, const MS_BOOL isSourceChange, const MS_BOOL isOpFrameChanged )
{
    //static MS_U8 lbConfirmLevel = 0;
    static MS_U8 triggerCnt = 0;
    static MS_U8 hisPos = 0;
    static MS_U8 hisCnt = 0;
    static MS_U16 au16HisCntT[16] = {0};
    static MS_U16 au16HisCntB[16] = {0};
    static MS_U16 au16HisCntL[16] = {0};
    static MS_U16 au16HisCntR[16] = {0};
    static MS_U16 cntIIRT = 0, cntIIRB = 0, cntIIRL = 0, cntIIRR = 0;
    static MS_U16 prevMinCntT = 0, prevMinCntB = 0, prevMinCntL = 0, prevMinCntR = 0;
    //static XC_LetterBoxState lbState;

    MS_U16 triggerMask = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK0B_15_L, 0xF000) >> 12;
    triggerMask = triggerMask < 3 ? 3 : triggerMask;

    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0B_05_L, 0x7F, 0x1FFF );

    if( isSceneChange || isSourceChange )
    {
        lbConfirmLevel = 0;
        hisCnt = 0;
        memset( au16HisCntT, 0xFF, sizeof(au16HisCntT) );
        memset( au16HisCntB, 0xFF, sizeof(au16HisCntB) );
        memset( au16HisCntL, 0xFF, sizeof(au16HisCntL) );
        memset( au16HisCntR, 0xFF, sizeof(au16HisCntR) );
    }

    if( isOpFrameChanged )
    {
        if( (triggerCnt & triggerMask) == 0 )
        {
            au16HisCntT[hisPos] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK0B_0D_L, 0x03FF );
            au16HisCntB[hisPos] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK0B_0E_L, 0x03FF );
            au16HisCntL[hisPos] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK0B_0F_L, 0x07FF );
            au16HisCntR[hisPos] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK0B_10_L, 0x07FF );

            MS_U16 minCntT = au16HisCntT[0], minCntB = au16HisCntB[0], minCntL = au16HisCntL[0], minCntR = au16HisCntR[0];
            int i;
            for( i = 1 ; i < 16 ; ++i )
            {
                minCntT = au16HisCntT[i] < minCntT ? au16HisCntT[i] : minCntT;
                minCntB = au16HisCntB[i] < minCntB ? au16HisCntB[i] : minCntB;
                minCntL = au16HisCntL[i] < minCntL ? au16HisCntL[i] : minCntL;
                minCntR = au16HisCntR[i] < minCntR ? au16HisCntR[i] : minCntR;
            }

            if( hisPos == 15 )
                hisPos = 0;
            else
                ++hisPos;

            if( hisCnt < 16 )
                ++hisCnt;

            MS_U16 conf = abs( (int)(minCntT + minCntB + minCntL + minCntR) - (int)(prevMinCntT + prevMinCntB + prevMinCntL + prevMinCntR) );
            conf = conf < 256 ? 255 - conf : 0;
            if ( minCntL == 0x01FFF || minCntR == 0x01FFF || minCntT == 0x01FFF || minCntB == 0x01FFF)
                conf = 0;

            lbConfirmLevel = (lbConfirmLevel*6 + conf*2 )>> 3;
            lbState.lbComfirmLevel = conf;
            lbState.lbCntT = cntIIRT = ( cntIIRT * 7 + minCntT ) >> 3;
            lbState.lbCntB = cntIIRB = ( cntIIRB * 7 + minCntB ) >> 3;
            lbState.lbCntL = cntIIRL = ( cntIIRL * 7 + minCntL ) >> 3;
            lbState.lbCntR = cntIIRR = ( cntIIRR * 7 + minCntR ) >> 3;

            prevMinCntT = minCntT;
            prevMinCntB = minCntB;
            prevMinCntL = minCntL;
            prevMinCntR = minCntR;
            /*
            if( isHDSrc )
            {
                int ratio = 0x1FA4000 / ( (int)(1920-cntIIRL-cntIIRR) * (int)(1080-cntIIRT-cntIIRB) );
                lbState.lbRatioC = ratio < 256 ? ratio : 255;

                ratio = 0x1FA4000 / ( (int)(1920) * (int)(1080-cntIIRT-cntIIRB) );
                lbState.lbRatioTB = ratio < 256 ? ratio : 255;

                ratio = 0x1FA4000 / ( (int)(1920-cntIIRL-cntIIRR) * (int)(1080) );
                lbState.lbRatioLR = ratio < 256 ? ratio : 255;
            }
            else
            {
                int ratio = 0x546000 / ( (int)(1920-cntIIRL-cntIIRR) * (int)(1080-cntIIRT-cntIIRB) );
                lbState.lbRatioC = ratio < 256 ? ratio : 255;

                ratio = 0x546000 / ( (int)(1920) * (int)(1080-cntIIRT-cntIIRB) );
                lbState.lbRatioTB = ratio < 256 ? ratio : 255;

                ratio = 0x546000 / ( (int)(1920-cntIIRL-cntIIRR) * (int)(1080) );
                lbState.lbRatioLR = ratio < 256 ? ratio : 255;
            }*/
            int windowWidth  = max((int)(frameWidth-cntIIRL-cntIIRR), 1);
            int windowHeight = max((int)(frameHeight-cntIIRT-cntIIRB), 1);
            int ratio = 0x1FA40000 / ( windowWidth * windowHeight );
            lbState.lbRatioC = ratio < 256 ? ratio : 255;

            ratio = 0x1FA40000 / ( frameWidth * windowHeight );
            lbState.lbRatioTB = ratio < 256 ? ratio : 255;

            ratio = 0x1FA40000 / ( windowWidth * frameHeight );
            lbState.lbRatioLR = ratio < 256 ? ratio : 255;


            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0B_15_L, cntIIRL, 0x01FF);
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0B_16_L, cntIIRR, 0x01FF);

            //PRT_TRP( "Letter Box L : %03d, R : %03d, confirm : %03d, ratio C/TB/LR : %02d.%2d, %02d.%2d, %02d.%2d \n", cntIIRL, cntIIRR, conf,
            //lbState.lbRatioC/16, (lbState.lbRatioC&0xF)*10/16, lbState.lbRatioTB/16, (lbState.lbRatioTB&0xF)*10/16, lbState.lbRatioLR/16, (lbState.lbRatioLR&0xF)*10/16 );
        }
        ++triggerCnt;

        if (lbConfirmLevel > 224 && !(isSceneChange ||isSourceChange) && (lbState.lbCntL > 100) && (lbState.lbCntR > 100) )
        {
                MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x0080, 0x0080);
                MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_7C_L, lbState.lbCntL + 50, 0x1FFF);
                MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_7D_L, frameWidth - lbState.lbCntR - 50, 0x1FFF);
                MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x7000, 0x7000);
        }
        else
        {
                const int boundary = 10;
                MS_U16 widthCur = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK12_0F_L, 0x0FFF);/* cur width */

                MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x0080, 0x0080);
                MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_7C_L, boundary, 0x1FFF); /* set start */
                MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK02_7D_L, widthCur-boundary, 0x1FFF);/* set end */
                MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0000, 0x7000);
        }
    }

    return lbState;
}

//===================================== function used in Mdrv_PQ_DBK() function =================================
MS_BOOL ScenceChangeConfirm(MS_U32 u32StaticCount,MS_BOOL isSceneChangeY)
{
    MS_BOOL bRealSceneChange = 0;
    static MS_U8 u8SceneChangeCounter = 0,u32StaticCountTemp = 0,u8CounterForSC = 0;
    if(u32StaticCountTemp >20 && u32StaticCount ==0)
    {
        u8CounterForSC = 1;
    }

    if(u8CounterForSC ==1)
    {
        u8SceneChangeCounter++;
        if(u8SceneChangeCounter<10)
        {
            if(isSceneChangeY)
            {
                bRealSceneChange = 1;
            }
        }
        else
        {
            u8CounterForSC = 0;
            u8SceneChangeCounter = 0;
        }
    }

    u32StaticCountTemp = u32StaticCount;
    return bRealSceneChange;
}


//===================================== function used in Mdrv_PQ_DBK() function end =============================
MS_U8 readHistogramPart(MS_U8 u8CountForDBK)
{
    MS_U8 u8Data_ack_en;
    MS_U16 u16MarkGain_ack_en;
    MS_U8 u8SuccessAct = 0;
    //MS_U16 u16Blkwei_Err= 0;
    static MS_U8 u8RequestFlag = 0;

    //MS_U16 u16Bin;

    //--------- Blockiness Histogram Request start ---------//
    if (u8CountForDBK == 0 && u8RequestFlag == 0)
    {
    // write 1 to request register
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_0D_L, 1, 0x0001);
    u8RequestFlag = 1;
    //printf("Sequence Request 1 \n");
    }

    //--------- Blockiness Histogram Acknowledge start ---------//
    u8Data_ack_en = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK0C_0D_L, 0x0002);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_08_L, 0x8000, 0x8000);//blk_wei sram io enable
    //u16Blkwei_Err = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_0B_L, 0x0100);//blk_wei_err
    u16MarkGain_ack_en = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_08_L, 0x2000);
    //printf("Sequence blk_wei IO 1 u16Blkwei_Err:%d u8Data_ack_en:%d u16MarkGain_ack_en:%d\n",u16Blkwei_Err,u8Data_ack_en,u16MarkGain_ack_en);

    //--------- Function Process start ---------//
    if(u8Data_ack_en)//&& u16Blkwei_Err == 0)
    {

        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_0D_L, 0x0100, 0x0100);//sram_io_en
        //printf("Sequence sram_io_en  1\n");
        //u16MarkGain_ack_en = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_08_L, 0x2000);
        if(u16MarkGain_ack_en)
        {

#if 0
            // write 1 to sram_io_en register
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_0D_L, 0x0100, 0x0100);
            for(u16Bin = u16Initial_point; u16Bin < u16End_point; u16Bin++)
        {
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_0E_L, (MS_U16)(0x8000 + u16Bin), 0xFFFF);
            // read data from selected address
            au16Histogram[u16Bin] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK0C_0F_L, 0xFFFF);
            }
#endif
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_0D_L, 0, 0x0001);
            u8SuccessAct = 1;
            u8RequestFlag = 0;
            //printf("Sequence Request 0\n");
        }
    }
    else//(!data_ack_en)
    {
        u8SuccessAct = 0;
    }

    return u8SuccessAct;
}


MS_U8 readVHistogramPart(MS_U8 u8PartIndex)
{
    MS_U8 u8Data_ack_en;
    MS_U16 u16MarkGain_ack_en;
    MS_U8 u8SuccessAct = 0;
    //MS_U16 u16Blkwei_Err= 0;
    static MS_U8 u8RequestFlag = 0;

    //--------- Blockiness Histogram Request start ---------//
    if (u8PartIndex == 0 && u8RequestFlag == 0)
    {
        // write 1 to request register
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_7D_L, 1, 0x0001); //reg_bkn_statistic_ack
        u8RequestFlag = 1;
        //printf("V Sequence Request 1 \n");
    }
        //--------- Blockiness Histogram Acknowledge start ---------//
    u8Data_ack_en = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_7D_L, 0x0002); //reg_bkn_width_statistic_ack
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_78_L, 0x8000, 0x8000);//blk_wei sram io enable
    //u16Blkwei_Err = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_7B_L, 0x0100);//blk_wei_err
    u16MarkGain_ack_en = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_78_L, 0x2000);
    //printf("V Sequence blk_wei IO 1 u16Blkwei_Err:%d u8Data_ack_en:%d u16MarkGain_ack_en:%d\n",u16Blkwei_Err,u8Data_ack_en,u16MarkGain_ack_en);
        //--------- Function Process start ---------//
    if(u8Data_ack_en)
    {
        // write 1 to sram_io_en register
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_7D_L, 0x0100, 0x0100); //reg_bkn_sram_io_en
        //printf("V Sequence sram_io_en  1\n");
        if(u16MarkGain_ack_en)
        {
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_7D_L, 0, 0x0001);//reg_bkn_statistic_ack
            u8SuccessAct = 1;
            u8RequestFlag = 0;
            //printf("V Sequence Request 0\n");
        }
    }
    else//(!data_ack_en)
    {
        u8SuccessAct = 0;
    }

    return u8SuccessAct;

}


//---------- function in DBK_Width_Detect() called -----------------------------------------//
MS_S8 DBK_WidthPartHW(MS_U16 *u16WidthDiffHW, MS_U16* u16Avg,MS_BOOL isSceneChangeY,MS_U8 *u8WidthConfidence)
{
    MS_U8 u8Index;
    MS_U32 u32MaxWidthDiff  = 0;
    MS_U32 u32MinWidthDiff  = 0x7FFFFFFF;
    MS_U32 u32Min2WidthDiff = 0x7FFFFFFF;
    MS_U32 u32Min3WidthDiff = 0x7FFFFFFF;
    MS_U32 u32Min4WidthDiff = 0x7FFFFFFF;
    MS_S8 s8MinIdx = -1, s8Min2Idx = -1, s8Min3Idx = -1, s8Min4Idx = -1;// maxIdx = -1;
    static MS_U8 u8TempWidth = 4;
    MS_U16 u16DiffThrd = 2048;
    static MS_S16 s16ConfidenceLevel8 = 0,s16ConfidenceLevel11 = 0,s16ConfidenceLevel16 = 0,s16ConfidenceLevel21 = 0;
    int log_en = 5;
    //log_en = (MS_U32)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_01_L, 0x000F); //easter
    if(isSceneChangeY)
    {
       //printf("H isSceneChangeY:[%d]\n",isSceneChangeY);
       s16ConfidenceLevel8 = 0;
       s16ConfidenceLevel11 = 0;
       s16ConfidenceLevel16 = 0;
       s16ConfidenceLevel21 = 0;
    }
    u16WidthDiffHW[0 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_40_L,0xFFFF) ; // width 4
    u16WidthDiffHW[1 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_41_L,0xFFFF) ; // width 5
    u16WidthDiffHW[2 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_42_L,0xFFFF) ; // width 6
    u16WidthDiffHW[3 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_43_L,0xFFFF) ; // width 7
    u16WidthDiffHW[4 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_44_L,0xFFFF) ; // width 8
    u16WidthDiffHW[5 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_45_L,0xFFFF) ; // width 9
    u16WidthDiffHW[6 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_46_L,0xFFFF) ; // width 10
    u16WidthDiffHW[7 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_47_L,0xFFFF) ; // width 11
    u16WidthDiffHW[8 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_48_L,0xFFFF) ; // width 12
    u16WidthDiffHW[9 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_49_L,0xFFFF) ; // width 13
    u16WidthDiffHW[10] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_4A_L,0xFFFF) ; // width 14
    u16WidthDiffHW[11] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_4B_L,0xFFFF) ; // width 15
    u16WidthDiffHW[12] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_4C_L,0xFFFF) ; // width 16
    u16WidthDiffHW[13] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_4D_L,0xFFFF) ; // width 17
    u16WidthDiffHW[14] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_4E_L,0xFFFF) ; // width 18
    u16WidthDiffHW[15] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_4F_L,0xFFFF) ; // width 19
    u16WidthDiffHW[16] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_50_L,0xFFFF) ; // width 20
    u16WidthDiffHW[17] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_51_L,0xFFFF) ; // width 21
    u16WidthDiffHW[18] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_52_L,0xFFFF) ; // width 22
    u16WidthDiffHW[19] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_53_L,0xFFFF) ; // width 23
    u16WidthDiffHW[20] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_54_L,0xFFFF) ; // width 24
    u16WidthDiffHW[21] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_55_L,0xFFFF) ; // width 25
    u16WidthDiffHW[22] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_56_L,0xFFFF) ; // width 26
    u16WidthDiffHW[23] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_57_L,0xFFFF) ; // width 27
    u16WidthDiffHW[24] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_58_L,0xFFFF) ; // width 28
    u16WidthDiffHW[25] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_59_L,0xFFFF) ; // width 29
    u16WidthDiffHW[26] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_5A_L,0xFFFF) ; // width 30
    u16WidthDiffHW[27] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_5B_L,0xFFFF) ; // width 31
    u16WidthDiffHW[28] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_5C_L,0xFFFF) ; // width 32
    u16WidthDiffHW[29] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_5D_L,0xFFFF) ; // width 33
    u16WidthDiffHW[30] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_5E_L,0xFFFF) ; // width 34
    u16WidthDiffHW[31] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_5F_L,0xFFFF) ; // width 35

    for(u8Index = 1 ; u8Index < paraSize ; u8Index++)
    {
        if (u16WidthDiffHW[u8Index] > u32MaxWidthDiff)
        {
            u32MaxWidthDiff = u16WidthDiffHW[u8Index];
        }
    }

    for(u8Index = 1 ; u8Index < paraSize -1; u8Index+=2)
    {
    if ( (u8Index==3 || u8Index==11 || u8Index==19 || u8Index==27) && (u16WidthDiffHW[u8Index+1] - u16WidthDiffHW[u8Index ] <= u16DiffThrd) )
    {
         u16WidthDiffHW[u8Index+1]= min(u16WidthDiffHW[u8Index ], u16WidthDiffHW[u8Index+1]);
         u16WidthDiffHW[u8Index  ]= u32MaxWidthDiff;
    }
    else if ( (u8Index==7) && ( (u16WidthDiffHW[u8Index  ] - u16WidthDiffHW[u8Index-1 ] <= u16DiffThrd) || (u16WidthDiffHW[u8Index  ] - u16WidthDiffHW[u8Index+1 ] <= u16DiffThrd)) )
    {
         u16WidthDiffHW[u8Index  ]= min3(u16WidthDiffHW[u8Index ], u16WidthDiffHW[u8Index-1], u16WidthDiffHW[u8Index+1]);
         u16WidthDiffHW[u8Index-1]= u32MaxWidthDiff;
         u16WidthDiffHW[u8Index+1]= u32MaxWidthDiff;
    }
    }

    for(u8Index = 1 ; u8Index < paraSize ; u8Index++)
    {
        (*u16Avg) += u16WidthDiffHW[u8Index];
        if (u16WidthDiffHW[u8Index] < u32MinWidthDiff)
        {
            u32Min4WidthDiff = u32Min3WidthDiff;
            s8Min4Idx = s8Min3Idx;
            u32Min3WidthDiff = u32Min2WidthDiff;
            s8Min3Idx = s8Min2Idx;
            u32Min2WidthDiff = u32MinWidthDiff;
            s8Min2Idx = s8MinIdx;
            u32MinWidthDiff = u16WidthDiffHW[u8Index];
            s8MinIdx = u8Index;
        }
        else if (u16WidthDiffHW[u8Index] < u32Min2WidthDiff)
        {
            u32Min4WidthDiff = u32Min3WidthDiff;
            s8Min4Idx = s8Min3Idx;
            u32Min3WidthDiff = u32Min2WidthDiff;
            s8Min3Idx = s8Min2Idx;
            u32Min2WidthDiff = u16WidthDiffHW[u8Index];
            s8Min2Idx = u8Index;
        }
        else if (u16WidthDiffHW[u8Index] < u32Min3WidthDiff)
        {
            u32Min4WidthDiff = u32Min3WidthDiff;
            s8Min4Idx = s8Min3Idx;
            u32Min3WidthDiff = u16WidthDiffHW[u8Index];
            s8Min3Idx = u8Index;
        }
        else if (u16WidthDiffHW[u8Index] < u32Min4WidthDiff)
        {
            u32Min4WidthDiff = u16WidthDiffHW[u8Index];
            s8Min4Idx = u8Index;
        }
    }

    *u16Avg = (*u16Avg) / (paraSize-1);

    s16ConfidenceLevel8 = minmax(s16ConfidenceLevel8,0,16);
    s16ConfidenceLevel11 = minmax(s16ConfidenceLevel11,0,16);
    s16ConfidenceLevel16 = minmax(s16ConfidenceLevel16,0,16);
    s16ConfidenceLevel21 = minmax(s16ConfidenceLevel21,0,16);
    if(s16ConfidenceLevel8>15)
    {
        s16ConfidenceLevel11 = 0;
        s16ConfidenceLevel16 = 0;
        s16ConfidenceLevel21 = 0;
    }
    else if(s16ConfidenceLevel11>15)
    {
        s16ConfidenceLevel8 = 0;
        s16ConfidenceLevel16 = 0;
        s16ConfidenceLevel21 = 0;
    }
    else if(s16ConfidenceLevel16>15)
    {
        s16ConfidenceLevel8 = 0;
        s16ConfidenceLevel11 = 0;
        s16ConfidenceLevel21 = 0;
    }
    else if(s16ConfidenceLevel21>15)
    {
        s16ConfidenceLevel8 = 0;
        s16ConfidenceLevel16 = 0;
        s16ConfidenceLevel11 = 0;
    }

    if(log_en==5)
        {
     //printf("s8MinIdx:%d s8Min2Idx:%d s8Min3Idx:%d s8Min4Idx:%d\n",s8MinIdx,s8Min2Idx,s8Min3Idx,s8Min4Idx);
     //printf("u16ConfidenceLevel8:%d u16ConfidenceLevel11:%d\n",s16ConfidenceLevel8,s16ConfidenceLevel11);
     //printf("u16ConfidenceLevel16:%d u16ConfidenceLevel21:%d\n",s16ConfidenceLevel16,s16ConfidenceLevel21);
        }
    *u8WidthConfidence = max3(s16ConfidenceLevel8,s16ConfidenceLevel11,s16ConfidenceLevel16);
    *u8WidthConfidence = max((*u8WidthConfidence),s16ConfidenceLevel21);
    /*if(u8TempWidth == 4 )//&& s16ConfidenceLevel8>10)
    {
     if((abs(s8MinIdx-u8TempWidth) <2)||(abs(s8Min2Idx-u8TempWidth) <2)||(abs(s8Min3Idx-u8TempWidth) <2)||(abs(s8Min4Idx-u8TempWidth) <2))
     {
         return 4;
    }
}
    else*/
    if(u8TempWidth == 7 && s16ConfidenceLevel11>10)
    {
        if ((((s8MinIdx == 7) || (s8Min2Idx == 7) || (s8Min3Idx == 7) || (s8Min4Idx == 7))|| ((s8MinIdx == 16) || (s8Min2Idx == 16) || (s8Min3Idx == 16) || (s8Min4Idx == 16))
             || ((s8MinIdx == 17) || (s8Min2Idx == 17) || (s8Min3Idx == 17) || (s8Min4Idx == 17)) ) && ((s8MinIdx == 28) || (s8Min2Idx == 28) || (s8Min3Idx == 28) || (s8Min4Idx == 28)))
        {
            s16ConfidenceLevel11++;
             return 7;
        }
        else if((abs(s8MinIdx-u8TempWidth) <2)||(abs(s8Min2Idx-u8TempWidth) <2)||(abs(s8Min3Idx-u8TempWidth) <2)||(abs(s8Min4Idx-u8TempWidth) <2))
        {
            s16ConfidenceLevel11--;
             return 7;
        }
    }
    else if(u8TempWidth == 17 && s16ConfidenceLevel21>10)
    {
        if ((((s8MinIdx == 16 || s8Min2Idx == 16 || s8Min3Idx == 16) || (s8MinIdx == 18 || s8Min2Idx == 18 || s8Min3Idx == 18) ) && (s8MinIdx == 17 || s8Min2Idx == 17 || s8Min3Idx == 17)))
        {
            s16ConfidenceLevel21++;
            u8TempWidth = 17;
            return 17;
        }
        else if((abs(s8MinIdx-u8TempWidth) <4)||(abs(s8Min2Idx-u8TempWidth) <4)||(abs(s8Min3Idx-u8TempWidth) <4)||(abs(s8Min4Idx-u8TempWidth) <4))
        {
            s16ConfidenceLevel21--;
            return 17;
        }
    }

    if ((((s8MinIdx == 7) || (s8Min2Idx == 7) || (s8Min3Idx == 7) || (s8Min4Idx == 7))|| ((s8MinIdx == 16) || (s8Min2Idx == 16) || (s8Min3Idx == 16) || (s8Min4Idx == 16))
         || ((s8MinIdx == 17) || (s8Min2Idx == 17) || (s8Min3Idx == 17) || (s8Min4Idx == 17)) ) && ((s8MinIdx == 28) || (s8Min2Idx == 28) || (s8Min3Idx == 28) || (s8Min4Idx == 28)))
    {
        if(u8TempWidth == 7)
        {
            s16ConfidenceLevel11++;
        }
        else
        {
            s16ConfidenceLevel8--;
            s16ConfidenceLevel16--;
            s16ConfidenceLevel21--;
        }
        u8TempWidth = 7;
        return 7;
    }
    else if ((((s8MinIdx == 16 || s8Min2Idx == 16 || s8Min3Idx == 16) || (s8MinIdx == 18 || s8Min2Idx == 18 || s8Min3Idx == 18) ) && (s8MinIdx == 17 || s8Min2Idx == 17 || s8Min3Idx == 17)))
    {
        if(u8TempWidth == 17)
        {
            s16ConfidenceLevel21++;
        }
        else
        {
            s16ConfidenceLevel8--;
            s16ConfidenceLevel16--;
            s16ConfidenceLevel11--;
        }
        u8TempWidth = 17;
        return 17;
    }

    if (s8MinIdx == 4 || s8Min2Idx == 4 || s8Min3Idx == 4 || s8Min4Idx == 4)
    {
        if(u8TempWidth == 4)
        {
            s16ConfidenceLevel8++;
        }
        else
        {
            s16ConfidenceLevel11--;
            s16ConfidenceLevel16--;
            s16ConfidenceLevel21--;
        }
        u8TempWidth = 4;
        return 4;
    }
    else if (s8MinIdx == 12 || s8Min2Idx == 12 || s8Min3Idx == 12 || s8Min4Idx == 12)
    {
        if(u8TempWidth == 12)
        {
            s16ConfidenceLevel16++;
        }
        else
        {
            s16ConfidenceLevel8--;
            s16ConfidenceLevel11--;
            s16ConfidenceLevel21--;
        }
        u8TempWidth = 12;
        return 12;
    }

    return -1;
}

MS_S8 normFactorWidth(MS_U32 u32AvgBLKN)
{
    MS_S8 s8NormFac = 0;
    if (u32AvgBLKN > 1048576*1024)
    {
         s8NormFac = 25;
    }
    else if (u32AvgBLKN > 1048576*512)
    {
         s8NormFac = 24;
    }
    else if (u32AvgBLKN > 1048576*256)
    {
         s8NormFac = 23;
}
    else if (u32AvgBLKN > 1048576*128)
{
         s8NormFac = 22;
}
    else if (u32AvgBLKN > 1048576*64)
    {
         s8NormFac = 21;
    }
    else if (u32AvgBLKN > 1048576*32)
    {
         s8NormFac = 20;
    }
    else if (u32AvgBLKN > 1048576*16)
    {
         s8NormFac = 19;
    }
    else if (u32AvgBLKN > 1048576*8)
    {
         s8NormFac = 18;
    }
    else if (u32AvgBLKN > 1048576*4)
    {
         s8NormFac = 17;
    }
    else if (u32AvgBLKN > 1048576*2)
    {
         s8NormFac = 16;
    }
    else if (u32AvgBLKN > 1048576)
    {
         s8NormFac = 15;
    }
    else if (u32AvgBLKN > 524288)
    {
         s8NormFac = 14;
    }
    else if (u32AvgBLKN > 262144)
    {
         s8NormFac = 13;
    }
    else if (u32AvgBLKN > 131072)
    {
         s8NormFac = 12;
    }
    else if (u32AvgBLKN > 65536)
    {
         s8NormFac = 11;
    }
    else if (u32AvgBLKN > 32768)
    {
         s8NormFac = 10;
    }
    else if (u32AvgBLKN > 16384)
    {
         s8NormFac = 9;
    }
    else if (u32AvgBLKN > 8192)
    {
         s8NormFac = 8;
    }
    else if (u32AvgBLKN > 4096)
    {
         s8NormFac = 7;
    }
    else if (u32AvgBLKN > 2048)
    {
         s8NormFac = 6;
    }
    else if (u32AvgBLKN > 1024)
    {
         s8NormFac = 5;
    }
    else if (u32AvgBLKN > 512)
    {
         s8NormFac = 4;
    }
    else if (u32AvgBLKN > 256)
    {
         s8NormFac = 3;
    }
    else if (u32AvgBLKN > 128)
    {
         s8NormFac = 2;
    }
    else if (u32AvgBLKN > 64)
    {
         s8NormFac = 1;
    }
    else if (u32AvgBLKN > 32)
    {
         s8NormFac = 0;
    }
    else if (u32AvgBLKN > 16)
    {
         s8NormFac = -1;
    }
    else if (u32AvgBLKN > 8)
    {
         s8NormFac = -2;
    }
    else
    {
         s8NormFac = 0;
    }

    return s8NormFac;
}
    
    
MS_S8 DBK_WidthPartTop(MS_U16 *u16WidthDiffHW, MS_U16* u16MaxMinDiff, MS_U16* u16Avg,MS_BOOL isSceneChangeY,MS_U8* u8WidthConfidence)
{
    MS_U8 u8Bin;
    MS_U32 u32MinValue = 0x7FFFFFFF;
    MS_U16 u16MaxValue = 0;
    MS_S8 s8NormFac;
    MS_S8 s8MinIndex;
    s8MinIndex = DBK_WidthPartHW(u16WidthDiffHW, u16Avg,isSceneChangeY,u8WidthConfidence);
    if (s8MinIndex == -1)
    {
        for (u8Bin =0; u8Bin<paraSize;u8Bin++)
        {
            if (u16WidthDiffHW[u8Bin] > u16MaxValue )
            {
                u16MaxValue = u16WidthDiffHW[u8Bin];
            }

            if (u16WidthDiffHW[u8Bin] < u32MinValue )
            {
            u32MinValue = u16WidthDiffHW[u8Bin];
            }
        }
        *u16MaxMinDiff = u16MaxValue - u32MinValue;
        s8NormFac = normFactorWidth(*u16MaxMinDiff);
        if(s8NormFac >0)
        {
            for (u8Bin=0;u8Bin<paraSize;u8Bin++)
            {
                u16WidthDiffHW[u8Bin] = (u16WidthDiffHW[u8Bin] - u32MinValue) >> s8NormFac;
            }
            *u16Avg = (*u16Avg-u32MinValue) >> s8NormFac;
            *u16MaxMinDiff = (*u16MaxMinDiff) >>s8NormFac;
        }
        else if(s8NormFac ==0)
        {
            for (u8Bin=0;u8Bin<paraSize;u8Bin++)
            {
                u16WidthDiffHW[u8Bin] = (u16WidthDiffHW[u8Bin] - u32MinValue);
            }
            *u16Avg = (*u16Avg-u32MinValue);
            *u16MaxMinDiff = (*u16MaxMinDiff);
        }
        else
        {
            s8NormFac = 0-s8NormFac;
            for (u8Bin=0;u8Bin<paraSize;u8Bin++)
            {
                u16WidthDiffHW[u8Bin] = (u16WidthDiffHW[u8Bin] - u32MinValue) << s8NormFac;
            }
            *u16Avg = (*u16Avg-u32MinValue) <<s8NormFac;
            *u16MaxMinDiff = (*u16MaxMinDiff) <<s8NormFac;
        }
        return -1;
    }
    else
    {
        return s8MinIndex;
    }
}

MS_S8 DBK_ComputeWidthPart(  MS_U16 *u16WidthDiffHW,MS_U8 u8LastminIdex, MS_U16 u16Avg,MS_U8 u8WidthConfidence)
{
    MS_BOOL bWidthCandidate[paraSize];
    MS_U16 u16WTh = u16Avg;
    MS_U8 u8MinIndex=0;
    MS_U8 u8NumW=0;
    MS_U8 u8Bin;
    MS_BOOL bIf8 = 0, bIf11 = 0, bIf21 = 0, bIf32 = 0;

    for (u8Bin=0;u8Bin<paraSize;u8Bin++)
    {
        if (u16WidthDiffHW[u8Bin]<= u16WTh)
        {
            bWidthCandidate[u8Bin] = 1;
        }
        else
        {
            bWidthCandidate[u8Bin] = 0;
        }
    }

    if(bWidthCandidate[u8LastminIdex] ==1 && u8WidthConfidence>10)
    {
        return u8LastminIdex;
    }
    else
    {
        if (bWidthCandidate[4] == 1 ) //8
        {
            if (bWidthCandidate[3] == 1) //7
                bWidthCandidate[3]  = 0;
            if (bWidthCandidate[11] == 1) // 15
                bWidthCandidate[11] = 0;
            if (bWidthCandidate[12] == 1) // 16
                bWidthCandidate[12] = 0;
            if (bWidthCandidate[19] == 1) // 23
                bWidthCandidate[19] = 0;
            if (bWidthCandidate[20] == 1) // 24
                bWidthCandidate[20] = 0;

            bIf8 = 1;
        }
        if (bWidthCandidate[7] == 1 ) //11
        {
            if (bWidthCandidate[6] == 1) //10
                bWidthCandidate[6]  = 0;
            if (bWidthCandidate[8] == 1) //12
                bWidthCandidate[8]  = 0;

            bIf11 = 1;
        }

        if ( ( bWidthCandidate[16] == 1 ) || ( bWidthCandidate[17] == 1 ) ||( bWidthCandidate[18] == 1 ) ) //20 21 22
        {
            bWidthCandidate[16] = 0;
            bWidthCandidate[17] = 1;
            bWidthCandidate[18] = 0;
            bIf21 = 1;
        }

        if ( bWidthCandidate[27] == 1 && bWidthCandidate[28] == 1)
            bIf32 = 1;

        if (bIf11 && bIf21 && bIf32) //11/21/32 all true -> source = 11 ->delete 21
        {
            bWidthCandidate[17] = 0;
        }

        if (bIf8 || bIf11 || bIf21)
        {
            if (bWidthCandidate[27] == 1) // 31
                bWidthCandidate[27] = 0;
            if (bWidthCandidate[28] == 1) // 32
                bWidthCandidate[28] = 0;
        }

    }

    for (u8Bin=0;u8Bin<paraSize;u8Bin++) // count number of frequencies
    {
        if (bWidthCandidate[u8Bin] == 1)
        {
             u8MinIndex = u8Bin;
             u8NumW++;
        }
    }
    if (u8NumW == 1)
    {
        return u8MinIndex;
    }
    else
    {
        return -1;
    }
}

void WidthIIR_H(MS_U8 u8Width) // Width IIR Protection
{
    static MS_U8 u8HysteresisCount = 0;
    static MS_U8 u8LastWidth = 8;
    int log_en = 5;
    //log_en = (MS_U32)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_01_L, 0x000F); //easter
    if(u8HysteresisCount<6)
    {
        if(log_en==5)
        {
            //printf("new width:%d last width:%d final width:%d hysteresisCount:%d\n",u8Width,u8LastWidth,WidthIIR,u8HysteresisCount);
        }

        if(u8Width == u8LastWidth)
        {
            u8HysteresisCount = 0;
            WidthIIR = u8Width;
        }
        else
        {
            u8HysteresisCount++;
            WidthIIR = u8LastWidth;
        }
    }
    else
    {
        WidthIIR = u8Width;
        u8HysteresisCount = 0;
    }
    u8LastWidth = WidthIIR;
}

void WidthToRegister(MS_U8 u8Width) // Write width to register
{
    MS_U8 u8Left1,u8Right1;
    if(u8Width<8)
    {
     u8Width = 8;
     u8Left1 = 8;
     u8Right1 = 9;
    }
    else if (u8Width==8) //test 0808 for performance
    {
     u8Left1 = 8;
     u8Right1 = 8;
    }
    else
    {
     u8Left1 = u8Width - 1; //fix prevent defect
     u8Right1 = (u8Width >=31) ? 31 : u8Width + 1;
    }
    //write interval left && interval right
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_39_L, (MS_U16)((u8Left1<<8)|u8Right1), 0xFFFF);
}



//---------- function in DBK_Width_Detect() called end-----------------------------------------//
MS_U8 DBK_Width_Detect(MS_BOOL isSceneChangeY,MS_U8 *u8HwidthConfidence)//width detection with hardware accelaration
{
    MS_S8 s8MinIndex = -1;     //minIndex+width_offset = candidate width
    //For width calculation
    //int WidthCandidate[paraSize];
    MS_U16 au16WidthDiffHW[paraSize];
    MS_U16 u16MaxMinDiff = 0;
    MS_U16 u16WidthAvg = 0;
    MS_U8 u8Width = 8;
    static MS_U8 u8LastminIdex = 4;
    MS_U8 u8WidthConfidence = 0;
    int log_en = 5;
    //log_en = (MS_U32)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_01_L, 0x000F); //easter
    u16MaxMinDiff = 0;
    u16WidthAvg = 0;

    s8MinIndex = DBK_WidthPartTop( au16WidthDiffHW, &u16MaxMinDiff, &u16WidthAvg,isSceneChangeY,&u8WidthConfidence);
    *u8HwidthConfidence = u8WidthConfidence;
    if (s8MinIndex == -1)
    {
        s8MinIndex = DBK_ComputeWidthPart( au16WidthDiffHW ,u8LastminIdex, u16WidthAvg,u8WidthConfidence);
    }

    if (s8MinIndex >= 0 && s8MinIndex <28)           //if (width == 32) //no such pattern yet, only artificials
    {
        u8Width = s8MinIndex + 4;
    }
    else
    {
        u8Width = 8;//can't find any possible width set to default value
    }

    WidthIIR_H(u8Width);
    WidthToRegister(WidthIIR);
    //FeedBackSD2HD(WidthIIR, 0);

    if(log_en==5)
    {
     //printf("**bforeIIR[%d]********\n",u8Width);
     //printf("*********[%d]*********\n",WidthIIR);
     //printf("**Confiden[%d]*********\n",u8WidthConfidence);
    }

    u8LastminIdex = WidthIIR -4;

    return WidthIIR;
}

MS_U8 StartPositionAverage(MS_U8 u8StartPosition,MS_BOOL isSceneChangeY)
{
    static MS_U8 u8BufferCount = 1;
    MS_U8 u8BufferIndex =0;
    static MS_U8 u8StartPositionBuffer[10] = {0};
    MS_U16 u16StartPositionSum = 0;
    MS_U8 u8StartPositionAvg = 0;
    static MS_U8 u8StartPositionLast = 255;
    static MS_U8 u8StartPositionConfidence = 0;
    MS_U8 u8ConfidenceLevel = 7;
    static MS_U8 u8BufferFull = 0;
    MS_U8 u8BufferLoop = 0;

    if(isSceneChangeY)
    {
        u8StartPositionLast = 255;
        u8BufferFull = 0;
    }

    if(u8StartPositionLast == 255)
    {
        u8StartPositionBuffer[u8BufferCount - 1] = u8StartPosition;
    }
    else
    {
        if(u8StartPositionLast == u8StartPosition)
        {
            u8StartPositionBuffer[u8BufferCount - 1] = u8StartPosition;
            u8StartPositionConfidence++;
            u8StartPositionConfidence = minmax(u8StartPositionConfidence,0,10);
        }
        else
        {
            u8StartPositionConfidence--;
            u8StartPositionConfidence = minmax(u8StartPositionConfidence,0,10);
            if(u8StartPositionConfidence > u8ConfidenceLevel)
            {
                u8StartPositionBuffer[u8BufferCount - 1] = u8StartPositionLast;
            }
            else
            {
                u8StartPositionBuffer[u8BufferCount - 1] = u8StartPosition;
            }
        }
    }
    u8StartPositionLast = u8StartPositionBuffer[u8BufferCount - 1];

    //printf("u8BufferIndex:%d u8StartPosition:%d u8StartPositionLast:%d\n",(u8BufferCount-1),u8StartPosition,u8StartPositionLast);

    if(u8BufferFull)
    {
        u8BufferLoop = 10;
    }
    else
    {
        u8BufferLoop = u8BufferCount;
    }

    for(u8BufferIndex = 0;u8BufferIndex<u8BufferLoop;u8BufferIndex++)
    {
        u16StartPositionSum+= u8StartPositionBuffer[u8BufferIndex];
        //printf("u8StartPositionBuffer[%d]:%d u8BufferCount:%d\n",u8BufferIndex,u8StartPositionBuffer[u8BufferIndex],u8BufferCount);
    }
    u8StartPositionAvg = u16StartPositionSum/u8BufferLoop;
    //printf("u8StartPositionAvg:%d u8StartPositionConfidence:%d \n",u8StartPositionAvg,u8StartPositionConfidence);

    if(u8BufferCount == 10)
    {
        u8BufferCount = 1;
        u8BufferFull = 1;
    }
    else
    {
        u8BufferCount++;
    }

    return u8StartPositionAvg;
}

MS_U8 DBK_FindStartPosition(MS_U8 u8Width,MS_BOOL isSceneChangeY)
{
    MS_U32 u32AccMax[32] = {0};
    MS_U32 u32AccMaxSum[32] = {0};
    MS_U32 u32Max = 0;
    MS_U8 u8StartPosition = 0;
    MS_U8 u8StartPositionAverage = 0;
    MS_U8 u8FindIndex = 0;
    MS_U8 u8InitailPosition = 0;
    //printf("easter befor read\n");
    u32AccMax[0  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_10_L, 0x1FFF);
    u32AccMax[1  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_11_L, 0x1FFF);
    u32AccMax[2  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_12_L, 0x1FFF);
    u32AccMax[3  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_13_L, 0x1FFF);
    u32AccMax[4  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_14_L, 0x1FFF);
    u32AccMax[5  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_15_L, 0x1FFF);
    u32AccMax[6  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_16_L, 0x1FFF);
    u32AccMax[7  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_17_L, 0x1FFF);
    u32AccMax[8  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_18_L, 0x1FFF);
    u32AccMax[9  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_19_L, 0x1FFF);
    u32AccMax[10] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_1A_L, 0x1FFF);
    u32AccMax[11] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_1B_L, 0x1FFF);
    u32AccMax[12] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_1C_L, 0x1FFF);
    u32AccMax[13] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_1D_L, 0x1FFF);
    u32AccMax[14] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_1E_L, 0x1FFF);
    u32AccMax[15] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_1F_L, 0x1FFF);
    u32AccMax[16] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_20_L, 0x1FFF);
    u32AccMax[17] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_21_L, 0x1FFF);
    u32AccMax[18] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_22_L, 0x1FFF);
    u32AccMax[19] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_23_L, 0x1FFF);
    u32AccMax[20] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_24_L, 0x1FFF);
    u32AccMax[21] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_25_L, 0x1FFF);
    u32AccMax[22] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_26_L, 0x1FFF);
    u32AccMax[23] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_27_L, 0x1FFF);
    u32AccMax[24] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_28_L, 0x1FFF);
    u32AccMax[25] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_29_L, 0x1FFF);
    u32AccMax[26] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_2A_L, 0x1FFF);
    u32AccMax[27] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_2B_L, 0x1FFF);
    u32AccMax[28] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_2C_L, 0x1FFF);
    u32AccMax[29] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_2D_L, 0x1FFF);
    u32AccMax[30] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_2E_L, 0x1FFF);
    u32AccMax[31] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_2F_L, 0x1FFF);
    //printf("easter findstart\n");
    u8InitailPosition = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_34_L, 0x00FF);
    if(u8Width == 8)
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK47_33_L, 0x0000, 0xC000); //markline_toler_pts 0->3
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK47_0B_L, 0x000F, 0x00FF); //max statistic thrd/toler_pts
    }
    else if(u8Width == 11)
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK47_33_L, 0x4000, 0xC000); //markline_toler_pts 0->3
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK47_0B_L, 0x000F, 0x00FF); //max statistic thrd/toler_pts
    }
    else
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK47_33_L, 0xC000, 0xC000); //markline_toler_pts 0->3
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK47_0B_L, 0x00C4, 0x00FF); //max statistic thrd/toler_pts
    }

    for(u8FindIndex = 0;u8FindIndex<u8Width;u8FindIndex++)
    {
        if(u8FindIndex ==0)
        {
            u32AccMaxSum[0] = u32AccMax[u8Width - 1] + u32AccMax[0] + u32AccMax[1];
        }
        else if(u8FindIndex == (u8Width - 1))
        {
            u32AccMaxSum[u8Width - 1] = u32AccMax[u8Width - 2] + u32AccMax[u8Width - 1] + u32AccMax[1];
        }
        else
        {
            u32AccMaxSum[u8FindIndex] = u32AccMax[u8FindIndex - 1] + u32AccMax[u8FindIndex] + u32AccMax[u8FindIndex + 1];
        }

        if(u8Width == 8 || u8Width == 11)
        {
            if(u32AccMax[u8FindIndex]>2*u32Max)
            {
                u32Max = u32AccMax[u8FindIndex];
                u8StartPosition = u8InitailPosition + u8FindIndex;
            }
            else if(u32AccMax[u8FindIndex]>u32Max)
            {
                u32Max = u32AccMax[u8FindIndex];
                u8StartPosition = u8InitailPosition + u8FindIndex;
            }
        }
        else
        {
            if(u32AccMaxSum[u8FindIndex]>u32Max)
            {
                u32Max = u32AccMaxSum[u8FindIndex];
                u8StartPosition = u8InitailPosition + u8FindIndex;
            }
        }
        //printf("u32AccMaxSum:%d u32AccMax:%d u32Max:%d\n",u32AccMaxSum[u8FindIndex],u32AccMax[u8FindIndex],u32Max);

    }


    u8StartPositionAverage = StartPositionAverage(u8StartPosition,isSceneChangeY);

    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK47_31_L, u8StartPositionAverage, 0x1FFF);//set start to Max->Mark
    return u8StartPositionAverage;
}


MS_S8 VDBK_WidthPartHW(MS_BOOL isSceneChangeY,MS_U16 *u16WidthDiffHW,MS_U16 *u16MaxMinDiff,MS_U16 *u16WidthAvg,MS_S16 *as16MinDiffIndex
    ,MS_U8 u8Widdth,MS_U8 u8HwidthConfidence,MS_U8 *u8VwidthConfidence)
{
    MS_U32 u32Avg =0;
    MS_U8 u8Index;
    MS_U32 u32MaxWidthDiff  = 0;
    MS_U32 u32MinWidthDiff  = 0x7FFFFFFF;
    MS_U32 u32Min2WidthDiff = 0x7FFFFFFF;
    MS_U32 u32Min3WidthDiff = 0x7FFFFFFF;
    MS_U32 u32Min4WidthDiff = 0x7FFFFFFF;
    MS_S8 s8OutWidth = -1;
    MS_S8 s8MinIdx   = -1, s8min2Idx   = -1, s8min3Idx   = -1, s8min4Idx   = -1;
    MS_S8 s8MinWidth = -1, s8min2Width = -1, s8min3Width = -1, s8min4Width = -1;
    MS_U16 u16DiffThrd = 2048;
    static MS_S8 s8ReferHconfidence = 0,s8Vwidthconfidence = 0;
    //long ScalingRatio = 1<<20;
    // long ScalingRatio = (MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK23_0A_L,0x00FF)<<16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK23_09_L,0xFFFF);
    //const int OrigWidth = 8;
    //long EstWidth = (ScalingRatio*OrigWidth + (1<<19) ) >> 20;
    if(isSceneChangeY)
    {
        s8ReferHconfidence = 0;
        s8Vwidthconfidence = 0;
    }

    u16WidthDiffHW[0 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_60_L,0xFFFF); // width 5
    u16WidthDiffHW[1 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_61_L,0xFFFF); // width 6
    u16WidthDiffHW[2 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_62_L,0xFFFF); // width 7
    u16WidthDiffHW[3 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_63_L,0xFFFF); // width 8
    u16WidthDiffHW[4 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_64_L,0xFFFF); // width 9
    u16WidthDiffHW[5 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_65_L,0xFFFF); // width 10
    u16WidthDiffHW[6 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_66_L,0xFFFF); // width 11
    u16WidthDiffHW[7 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_67_L,0xFFFF); // width 12
    u16WidthDiffHW[8 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_68_L,0xFFFF); // width 13
    u16WidthDiffHW[9 ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_69_L,0xFFFF); // width 14
    u16WidthDiffHW[10] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_6A_L,0xFFFF); // width 15
    u16WidthDiffHW[11] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_6B_L,0xFFFF); // width 16
    u16WidthDiffHW[12] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_6C_L,0xFFFF); // width 17
    u16WidthDiffHW[13] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_6D_L,0xFFFF); // width 18
    u16WidthDiffHW[14] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_6E_L,0xFFFF); // width 19
    u16WidthDiffHW[15] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK60_6F_L,0xFFFF); // width 20
    for(u8Index = 0 ; u8Index < paraVerSize ; u8Index++)
    {
        if (u16WidthDiffHW[u8Index] > u32MaxWidthDiff)
        {
            u32MaxWidthDiff = u16WidthDiffHW[u8Index];
        }
        //printf("V states width :%d u16WidthDiffHW:%d\n",u8Index+5,u16WidthDiffHW[u8Index]);
    }
    for(u8Index = 0 ; u8Index < paraVerSize -1; u8Index+=2)
    {
        if ( (u8Index==2 || u8Index==10 ) && (u16WidthDiffHW[u8Index+1] - u16WidthDiffHW[u8Index ] <= u16DiffThrd) )
        {
            u16WidthDiffHW[u8Index+1]= min(u16WidthDiffHW[u8Index ], u16WidthDiffHW[u8Index+1]);
            u16WidthDiffHW[u8Index  ]= u32MaxWidthDiff;
        }
    }

    for(u8Index = 0 ; u8Index < paraVerSize ; u8Index++)
    {
        u32Avg += u16WidthDiffHW[u8Index];

        if (u16WidthDiffHW[u8Index] < u32MinWidthDiff)
        {
            u32Min4WidthDiff = u32Min3WidthDiff;
            s8min4Idx = s8min3Idx;
            u32Min3WidthDiff = u32Min2WidthDiff;
            s8min3Idx = s8min2Idx;
            u32Min2WidthDiff = u32MinWidthDiff;
            s8min2Idx = s8MinIdx;

            u32MinWidthDiff = u16WidthDiffHW[u8Index];
            s8MinIdx = u8Index;

        }
        else if (u16WidthDiffHW[u8Index] < u32Min2WidthDiff)
        {
            u32Min4WidthDiff = u32Min3WidthDiff;
            s8min4Idx = s8min3Idx;
            u32Min3WidthDiff = u32Min2WidthDiff;
            s8min3Idx = s8min2Idx;

            u32Min2WidthDiff = u16WidthDiffHW[u8Index];
            s8min2Idx = u8Index;

        }
        else if (u16WidthDiffHW[u8Index] < u32Min3WidthDiff)
        {
            u32Min4WidthDiff = u32Min3WidthDiff;
            s8min4Idx = s8min3Idx;

            u32Min3WidthDiff = u16WidthDiffHW[u8Index];
            s8min3Idx = u8Index;

        }
        else if (u16WidthDiffHW[u8Index] < u32Min4WidthDiff)
        {
            u32Min4WidthDiff = u16WidthDiffHW[u8Index];
            s8min4Idx = u8Index;
        }
    }

    *u16WidthAvg = u32Avg / paraVerSize;
    *u16MaxMinDiff = u32MaxWidthDiff - u32MinWidthDiff;

    if ( s8MinIdx  != -1)
    {
        s8MinWidth  = s8MinIdx +5;
    }
    if ( s8min2Idx != -1)
    {
        s8min2Width = s8min2Idx+5;
    }
    if ( s8min3Idx != -1)
    {
        s8min3Width = s8min3Idx+5;
    }
    if ( s8min4Idx != -1)
    {
        s8min4Width = s8min4Idx+5;
    }

    as16MinDiffIndex[0] = s8MinIdx;
    as16MinDiffIndex[1] = s8min2Idx;
    as16MinDiffIndex[2] = s8min3Idx;
    as16MinDiffIndex[3] = s8min4Idx;
    //PRT_TRP(" --min4WidthDiff = %d, min4Idx=%d, min4Width =%d \n", min4WidthDiff, min4Idx, min4Width);
    //PRT_TRP(" --min3WidthDiff = %d, min3Idx=%d, min3Width =%d \n", min3WidthDiff, min3Idx, min3Width);
    //PRT_TRP(" --min2WidthDiff = %d, min2Idx=%d, min2Width =%d \n", min2WidthDiff, min2Idx, min2Width);
    //PRT_TRP(" --minWidthDiff  = %d, minIdx =%d, minWidth  =%d \n", minWidthDiff , minIdx , minWidth );
    //PRT_TRP(" --maxWidthDiff  = %d, maxIdx =%d \n", maxWidthDiff , maxIdx );
    //PRT_TRP(" --avg  = %d \n", avg );
    //PRT_TRP(" --MaxMinDiff  = %d \n", MaxMinDiff );
    //printf("V states s8MinWidth:%d ,s8min2Width:%d , s8min3Width:%d ,s8min4Width:%d\n",s8MinWidth ,s8min2Width , s8min3Width,s8min4Width);
    //printf("V states u8HwidthConfidence:%d\n",u8HwidthConfidence);
    if (u8HwidthConfidence > 10)
    {
        if(u8Widdth == 8)
        {
            if((s8MinWidth == 8 ||s8min2Width == 8 || s8min3Width == 8 || s8min4Width == 8)
            ||(s8MinWidth == 16 || s8min2Width == 16 || s8min3Width == 16 || s8min4Width == 16))
            {
                s8OutWidth = 8;
                s8ReferHconfidence++;
                s8Vwidthconfidence--;
            }
        }
        else if(u8Widdth == 11)
        {
            if((s8MinWidth == 8 ||s8min2Width == 8 || s8min3Width == 8 || s8min4Width == 8)
            ||(s8MinWidth == 16 || s8min2Width == 16 || s8min3Width == 16 || s8min4Width == 16))
            {
                s8OutWidth = 8;
                s8ReferHconfidence++;
                s8Vwidthconfidence--;
            }
            else if(s8MinWidth == 11 ||s8min2Width == 11|| s8min3Width == 11|| s8min4Width == 11)
            {
                s8OutWidth = 11;
                s8ReferHconfidence++;
                s8Vwidthconfidence--;
            }
        }
        else if (u8Widdth == 16)
        {
            if(s8MinWidth == 16 || s8min2Width == 16 || s8min3Width == 16 || s8min4Width == 16)
            {
                s8OutWidth = 16;
                s8ReferHconfidence++;
                s8Vwidthconfidence--;
            }
            else if((s8MinWidth == 17 ||s8min2Width == 17|| s8min3Width == 17|| s8min4Width == 17)
                &&(s8MinWidth == 18 ||s8min2Width == 18|| s8min3Width == 18|| s8min4Width == 18))
            {
                s8OutWidth = 18;
                s8ReferHconfidence++;
                s8Vwidthconfidence--;
            }
        }
        else if (u8Widdth == 21)
        {
            if((s8MinWidth == 17 ||s8min2Width == 17|| s8min3Width == 17|| s8min4Width == 17)
                &&(s8MinWidth == 18 ||s8min2Width == 18|| s8min3Width == 18|| s8min4Width == 18))
            {
                s8OutWidth = 18;
                s8ReferHconfidence++;
                s8Vwidthconfidence--;
            }
        }
    }
    else if ((s8MinWidth == 8 ||s8min2Width == 8 || s8min3Width == 8 || s8min4Width == 8)
        &&(s8MinWidth == 16 || s8min2Width == 16 || s8min3Width == 16 || s8min4Width == 16))
    {
    s8OutWidth = 8;
    s8ReferHconfidence--;
    s8Vwidthconfidence++;
    }
    else
    {
        s8OutWidth = -1;
        s8Vwidthconfidence--;
        s8ReferHconfidence--;
    }
    s8ReferHconfidence = minmax(s8ReferHconfidence,0,16);
    s8Vwidthconfidence = minmax(s8Vwidthconfidence,0,16);
    if(s8Vwidthconfidence >15)
    {
        s8ReferHconfidence = 0;
    }

    if(s8ReferHconfidence >15)
    {
        s8Vwidthconfidence = 0;
    }

    *u8VwidthConfidence = max(s8ReferHconfidence,s8Vwidthconfidence);

    return s8OutWidth;
}

MS_S8 VDBK_ComputeWidthPart(MS_BOOL isSceneChangeY,MS_U16 *u16WidthDiffHW,MS_U8 u8LastminWidth
    ,MS_U16 u16MaxMinDiff,MS_U16 u16WidthAvg,MS_U8 u8VWidthConfidence,MS_S16 *as16MinDiffIndex)
{
    MS_U8 u8WidthCandidate[paraSize] ={0};
    MS_U16 u16minWidthDiff = 0,u16min2WidthDiff = 0,u16min3WidthDiff = 0,u16min4WidthDiff = 0;
    MS_S8 s8minWidth = 0,s8min2Width = 0,s8min3Width = 0,s8min4Width = 0;
    //MS_U8 u8SNRmin = 0,u8SNR2min = 0,u8SNR3min = 0,u8SNR4min = 0;
    MS_S16 s16minIdx = -1,s16min2Idx = -1,s16min3Idx = -1,s16min4Idx = -1,s16OutWidth =  -1;

    //if(isSceneChangeY)
   //{
        //u8VNconfidence = 0;
   //}

    s16minIdx = as16MinDiffIndex[0];
    s16min2Idx = as16MinDiffIndex[1];
    s16min3Idx = as16MinDiffIndex[2];
    s16min4Idx = as16MinDiffIndex[3];

    if(s16minIdx != -1)
    {
        s8minWidth = s16minIdx + 5;
        u16minWidthDiff = u16WidthDiffHW[s16minIdx];
    }
    if(s16min2Idx != -1)
    {
        s8min2Width = s16min2Idx + 5;
        u16min2WidthDiff = u16WidthDiffHW[s16min2Idx];
    }
    if(s16min3Idx != -1)
    {
        s8min3Width = s16min3Idx + 5;
        u16min3WidthDiff = u16WidthDiffHW[s16min3Idx];
    }
    if(s16min4Idx != -1)
    {
        s8min4Width = s16min4Idx + 5;
        u16min4WidthDiff = u16WidthDiffHW[s16min4Idx];
    }
    //printf("Revise s16minIdx:%d,s16min2Idx:%d,s16min3Idx:%d,s16min4Idx:%d\n",s8minWidth,s8min2Width,s8min3Width,s8min4Width);
    //printf("Revise u16minWidthDiff:%d ,u16min2WidthDiff:%d ,u16min3WidthDiff:%d ,u16min4WidthDiff:%d\n",u16minWidthDiff,u16min2WidthDiff,u16min3WidthDiff,u16min4WidthDiff);
    //printf("Revise u16MaxMinDiff:%d\n",u16MaxMinDiff);
#if 0
    if(u16WidthAvg > 0)
{
        if(u16minWidthDiff > u16WidthAvg)
    {
            u8SNRmin = 0;
    }
    else
    {
            u8SNRmin = (u16WidthAvg -u16minWidthDiff)*100/u16WidthAvg;
    }

        if(u16min2WidthDiff > u16WidthAvg)
    {
            u8SNR2min = 0;
        }
        else
        {
            u8SNR2min = (u16WidthAvg -u16min2WidthDiff)*100/u16WidthAvg;
    }

        if(u16min3WidthDiff > u16WidthAvg)
        {
            u8SNR3min = 0;
}
        else
{
            u8SNR3min = (u16WidthAvg -u16min3WidthDiff)*100/u16WidthAvg;
}

        if(u16min4WidthDiff > u16WidthAvg)
    {
            u8SNR4min = 0;
        }
        else
        {
            u8SNR4min = (u16WidthAvg -u16min4WidthDiff)*100/u16WidthAvg;
        }
    }
#endif
    //printf("V states H maxmindiff :%d\n",u16MaxMinDiff);
    //printf("V states u16minWidthDiff SNR:%d width :%d\n",u8SNRmin,s8minWidth);
    //printf("V states u16min2WidthDiff SNR:%d width :%d\n",u8SNR2min,s8min2Width);
    //printf("V states u16min3WidthDiff SNR:%d width :%d\n",u8SNR3min,s8min3Width);
    //printf("V states u16min4WidthDiff SNR:%d width :%d\n",u8SNR4min,s8min4Width);
    if(s16minIdx >=0 && s16min2Idx>=0 && s16min3Idx>=0 && s16min4Idx>=0)
    {
        u8WidthCandidate[s16minIdx] = 1;
        if (u16MaxMinDiff > 0)
        {
            if ( (u16min2WidthDiff-u16minWidthDiff)*256/u16MaxMinDiff <48)
            {
                u8WidthCandidate[s16min2Idx] = 1;
            }

            if ( (u16min3WidthDiff-u16minWidthDiff)*256/u16MaxMinDiff <48)
            {
                u8WidthCandidate[s16min3Idx] = 1;
            }

            if ( (u16min4WidthDiff-u16minWidthDiff)*256/u16MaxMinDiff <48)
            {
                u8WidthCandidate[s16min4Idx] = 1;
            }
        }
        //printf("V states PRE-u8WidthCandidate: 1:%d 2:%d 3:%d 4:%d\n",u8WidthCandidate[s16minIdx],u8WidthCandidate[s16min2Idx],u8WidthCandidate[s16min3Idx],u8WidthCandidate[s16min4Idx]);
        if(u8WidthCandidate[u8LastminWidth - 5] == 1 && u8VWidthConfidence >10)
        {
            return u8LastminWidth;
        }
        else
        {
            if(u8WidthCandidate[s16min2Idx] == 1)
            {
                if ( abs(s8min2Width - s8minWidth) == 1 )
                {
                    if (s16min2Idx < s16minIdx)
                    {
                        u8WidthCandidate[ s16min2Idx] = 0;
                    }
                    else
                    {
                        u8WidthCandidate[ s16minIdx] = 0;
                    }
                }
            }

            if(u8WidthCandidate[s16min3Idx] == 1)
            {
                if ( abs(s8min3Width - s8minWidth) == 1 )
                {
                    if (s16min3Idx < s16minIdx)
                    {
                        u8WidthCandidate[ s16min3Idx] = 0;
                    }
                    else
                    {
                        u8WidthCandidate[ s16minIdx] = 0;
                    }
                }
            }

            if(u8WidthCandidate[s16min4Idx] == 1)
            {
                if ( abs(s8min4Width - s8minWidth) == 1 )
                {
                    if (s16min4Idx < s16minIdx)
                    {
                        u8WidthCandidate[ s16min4Idx] = 0;
                    }
                    else
                    {
                        u8WidthCandidate[ s16minIdx] = 0;
                    }
                }
            }


            if ( abs(s8min3Width - s8min2Width) == 1 )
            {
                if (s16min3Idx < s16min2Idx)
                {
                    u8WidthCandidate[ s16min3Idx] = 0;
                }
                else
                {
                    u8WidthCandidate[ s16min2Idx] = 0;
                }
            }

            if ( abs(s8min4Width - s8min2Width) == 1 )
            {
                if (s16min4Idx < s16min2Idx)
                {
                    u8WidthCandidate[ s16min4Idx] = 0;
                }
                else
                {
                    u8WidthCandidate[ s16min2Idx] = 0;
                }
            }

            if ( abs(s8min4Width - s8min3Width) == 1)
            {
                if (s16min4Idx < s16min3Idx)
                {
                    u8WidthCandidate[ s16min4Idx] = 0;
                }
                else
                {
                    u8WidthCandidate[ s16min3Idx] = 0;
                }
            }

            if (u8WidthCandidate[ s16min4Idx] > 0)
            {
                if ( s8min4Width == s8min3Width*2 || s8min4Width == s8min2Width*2 || s8min4Width == s8minWidth*2 )
                {
                    u8WidthCandidate[ s16min4Idx] = 0;
                }

                if ( s8min3Width == s8min4Width*2 || s8min3Width == s8min2Width*2 || s8min3Width == s8minWidth*2 )
                {
                    u8WidthCandidate[ s16min3Idx] = 0;
                }

                if ( s8min2Width == s8min4Width*2 || s8min2Width == s8min3Width*2 || s8min2Width == s8minWidth*2 )
                {
                    u8WidthCandidate[ s16min2Idx] = 0;
                }

                if ( s8minWidth == s8min4Width*2 || s8minWidth == s8min3Width*2 || s8minWidth == s8min2Width*2 )
                {
                    u8WidthCandidate[ s16minIdx] = 0;
                }
            }
            //printf("V states u8WidthCandidate: 1:%d 2:%d 3:%d 4:%d\n",u8WidthCandidate[s16minIdx],u8WidthCandidate[s16min2Idx],u8WidthCandidate[s16min3Idx],u8WidthCandidate[s16min4Idx]);
            if (u8WidthCandidate[s16minIdx] + u8WidthCandidate[s16min2Idx] + u8WidthCandidate[s16min3Idx] + u8WidthCandidate[s16min4Idx] == 1)
            {
                s16OutWidth = u8WidthCandidate[s16minIdx]* s8minWidth + u8WidthCandidate[s16min2Idx]* s8min2Width + u8WidthCandidate[s16min3Idx]* s8min3Width + u8WidthCandidate[s16min4Idx]* s8min4Width;
            }
            else
            {
                s16OutWidth = -1;
            }
        }
    }

    return s16OutWidth;
}

void WidthIIR_V(MS_U8 u8Width) // Width IIR Protection
{
    static MS_U8 u8HysteresisCount = 0;
    static MS_U8 u8LastWidth = 8;
    int log_en = 5;
    //log_en = (MS_U32)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_01_L, 0x000F); //easter
    if(u8HysteresisCount<6)
    {
        if(log_en==5)
        {
        //printf("V new width:%d last width:%d final width:%d hysteresisCount:%d\n",u8Width,u8LastWidth,WidthIIR,u8HysteresisCount);
        }

        if(u8Width == u8LastWidth)
        {
            u8HysteresisCount = 0;
            VWidthIIR = u8Width;
        }
        else
        {
            u8HysteresisCount++;
            VWidthIIR = u8LastWidth;
        }
    }
    else
    {
        VWidthIIR = u8Width;
        u8HysteresisCount = 0;
    }
    u8LastWidth = VWidthIIR;
}

MS_U8 VDBK_Width_Detect(MS_BOOL isSceneChangeY,MS_U8 u8Widdth,MS_U8 u8HwidthConfidence,MS_U8 *u8VConfidence2Strength)//width detection with hardware accelaration
{
    MS_S8 s8MinWidth = -1;     //minIndex+width_offset = candidate width
    //For width calculation
    //int WidthCandidate[paraSize];
    MS_U16 au16WidthDiffHW[paraSize];
    MS_S16 as16MinDiffIndex[4] = {0};
    MS_U16 u16MaxMinDiff = 0;
    MS_U16 u16WidthAvg = 0;
    MS_U8 u8Width = 8;
    static MS_U8 u8LastminWidth = 8;
    MS_U8 u8VWidthConfidence = 0;
   // int log_en = 5;
    //log_en = (MS_U32)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_01_L, 0x000F); //easter
    u16MaxMinDiff = 0;
    u16WidthAvg = 0;

    s8MinWidth = VDBK_WidthPartHW(isSceneChangeY,au16WidthDiffHW,&u16MaxMinDiff,&u16WidthAvg,as16MinDiffIndex,u8Widdth,u8HwidthConfidence,&u8VWidthConfidence);

    if (s8MinWidth == -1)
    {
        s8MinWidth = VDBK_ComputeWidthPart(isSceneChangeY,au16WidthDiffHW,u8LastminWidth,u16MaxMinDiff,u16WidthAvg,u8VWidthConfidence,as16MinDiffIndex);
    }

    if (s8MinWidth >=5 && s8MinWidth<= 20)            //if (width == 20) //no such pattern yet, only artificials
    {
        u8Width = s8MinWidth;// + 4;
    }
    else
    {
        u8Width = 5;//can't find any possible width set to default value
    }

    *u8VConfidence2Strength = u8VWidthConfidence;
    WidthIIR_V(u8Width);
    //WidthToRegister(WidthIIR_V);
    //FeedBackSD2HD(WidthIIR, 0);

    u8LastminWidth = VWidthIIR;


     //printf("**V bforeIIR[%d]********\n",u8Width);
     //printf("*********V [%d]*********\n",VWidthIIR);
     //printf("**V Confiden[%d]*********\n",u8WidthConfidence);


    return VWidthIIR;
}


MS_U8 VStartPositionAverage(MS_U8 u8StartPosition,MS_BOOL isSceneChangeY)
        {
    static MS_U8 u8BufferCount = 1;
    MS_U8 u8BufferIndex =0;
    static MS_U8 u8StartPositionBuffer[10] = {0};
    MS_U16 u16StartPositionSum = 0;
    MS_U8 u8StartPositionAvg = 0;
    static MS_U8 u8StartPositionLast = 255;
    static MS_U8 u8StartPositionConfidence = 0;
    MS_U8 u8ConfidenceLevel = 7;
    static MS_U8 u8BufferFull = 0;
    MS_U8 u8BufferLoop = 0;

    if(isSceneChangeY)
            {
        u8StartPositionLast = 255;
        u8BufferFull = 0;
    }

    if(u8StartPositionLast == 255)
                {
        u8StartPositionBuffer[u8BufferCount - 1] = u8StartPosition;
                }
    else
    {
        if(u8StartPositionLast == u8StartPosition)
                {
            u8StartPositionBuffer[u8BufferCount - 1] = u8StartPosition;
            u8StartPositionConfidence++;
            u8StartPositionConfidence = minmax(u8StartPositionConfidence,0,10);
                }
        else
                {
            u8StartPositionConfidence--;
            u8StartPositionConfidence = minmax(u8StartPositionConfidence,0,10);
            if(u8StartPositionConfidence > u8ConfidenceLevel)
                {
                u8StartPositionBuffer[u8BufferCount - 1] = u8StartPositionLast;
                }
            else
                {
                u8StartPositionBuffer[u8BufferCount - 1] = u8StartPosition;
                }
            }
        }
    u8StartPositionLast = u8StartPositionBuffer[u8BufferCount - 1];

    //printf("u8BufferIndex:%d u8StartPosition:%d u8StartPositionLast:%d\n",(u8BufferCount-1),u8StartPosition,u8StartPositionLast);

    if(u8BufferFull)
    {
        u8BufferLoop = 10;
        }
        else
    {
        u8BufferLoop = u8BufferCount;
    }

    for(u8BufferIndex = 0;u8BufferIndex<u8BufferLoop;u8BufferIndex++)
    {
        u16StartPositionSum+= u8StartPositionBuffer[u8BufferIndex];
        //printf("u8StartPositionBuffer[%d]:%d u8BufferCount:%d\n",u8BufferIndex,u8StartPositionBuffer[u8BufferIndex],u8BufferCount);
    }
    u8StartPositionAvg = u16StartPositionSum/u8BufferLoop;
    //printf("u8StartPositionAvg:%d u8StartPositionConfidence:%d \n",u8StartPositionAvg,u8StartPositionConfidence);

    if(u8BufferCount == 10)
    {
        u8BufferCount = 1;
        u8BufferFull = 1;
    }
    else
    {
        u8BufferCount++;
    }
    return u8StartPositionAvg;
}

MS_U8 VDBK_FindStartPosition(MS_U8 u8VWidth,MS_BOOL isSceneChangeY)
{
    MS_U32 u32AccMax[32] = {0};
    MS_U32 u32AccMaxSum[32] = {0};
    MS_U32 u32Max = 0;
    MS_U8 u8StartPosition = 0;
    MS_U8 u8StartPositionAverage = 0;
    MS_U8 u8FindIndex = 0;
    MS_U8 u8InitailPosition = 0;
    //printf("easter befor read\n");
    u32AccMax[0  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_40_L, 0x1FFF);
    u32AccMax[1  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_41_L, 0x1FFF);
    u32AccMax[2  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_42_L, 0x1FFF);
    u32AccMax[3  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_43_L, 0x1FFF);
    u32AccMax[4  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_44_L, 0x1FFF);
    u32AccMax[5  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_45_L, 0x1FFF);
    u32AccMax[6  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_46_L, 0x1FFF);
    u32AccMax[7  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_47_L, 0x1FFF);
    u32AccMax[8  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_48_L, 0x1FFF);
    u32AccMax[9  ] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_49_L, 0x1FFF);
    u32AccMax[10] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_4A_L, 0x1FFF);
    u32AccMax[11] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_4B_L, 0x1FFF);
    u32AccMax[12] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_4C_L, 0x1FFF);
    u32AccMax[13] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_4D_L, 0x1FFF);
    u32AccMax[14] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_4E_L, 0x1FFF);
    u32AccMax[15] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_4F_L, 0x1FFF);
    u32AccMax[16] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_50_L, 0x1FFF);
    u32AccMax[17] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_51_L, 0x1FFF);
    u32AccMax[18] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_52_L, 0x1FFF);
    u32AccMax[19] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_53_L, 0x1FFF);
    u32AccMax[20] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_54_L, 0x1FFF);
    u32AccMax[21] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_55_L, 0x1FFF);
    u32AccMax[22] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_56_L, 0x1FFF);
    u32AccMax[23] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_57_L, 0x1FFF);
    u32AccMax[24] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_58_L, 0x1FFF);
    u32AccMax[25] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_59_L, 0x1FFF);
    u32AccMax[26] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_5A_L, 0x1FFF);
    u32AccMax[27] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_5B_L, 0x1FFF);
    u32AccMax[28] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_5C_L, 0x1FFF);
    u32AccMax[29] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_5D_L, 0x1FFF);
    u32AccMax[30] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_5E_L, 0x1FFF);
    u32AccMax[31] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_5F_L, 0x1FFF);
    //printf("easter findstart\n");
    u8InitailPosition = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_64_L, 0x00FF);

    for(u8FindIndex = 0;u8FindIndex<u8VWidth;u8FindIndex++)
    {
        if(u8FindIndex ==0)
        {
            u32AccMaxSum[0] = u32AccMax[u8VWidth - 1] + u32AccMax[0] + u32AccMax[1];
        }
        else if(u8FindIndex == (u8VWidth - 1))
        {
            u32AccMaxSum[u8VWidth - 1] = u32AccMax[u8VWidth - 2] + u32AccMax[u8VWidth - 1] + u32AccMax[1];
        }
        else
        {
            u32AccMaxSum[u8FindIndex] = u32AccMax[u8FindIndex - 1] + u32AccMax[u8FindIndex] + u32AccMax[u8FindIndex + 1];
        }

        if(u8VWidth == 8 || u8VWidth == 11)
        {
            if(u32AccMax[u8FindIndex]>2*u32Max)
        {
                u32Max = u32AccMax[u8FindIndex];
                u8StartPosition = u8InitailPosition + u8FindIndex;
        }
            else if(u32AccMax[u8FindIndex]>u32Max)
        {
                u32Max = u32AccMax[u8FindIndex];
                u8StartPosition = u8InitailPosition + u8FindIndex;
        }
    }
        else
        {
            if(u32AccMaxSum[u8FindIndex]>u32Max)
            {
                u32Max = u32AccMaxSum[u8FindIndex];
                u8StartPosition = u8InitailPosition + u8FindIndex;
            }
            }
        //printf("u32AccMaxSum:%d u32AccMax:%d u32Max:%d\n",u32AccMaxSum[u8FindIndex],u32AccMax[u8FindIndex],u32Max);

        }


    u8StartPositionAverage = VStartPositionAverage(u8StartPosition,isSceneChangeY);

    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK47_61_L, u8StartPositionAverage, 0x1FFF);//set start to Max->Mark
    return u8StartPositionAverage;
}

void UIcontrolMpegNR(MS_U8 u8DetailGain_hw,MS_U8 *u8DetailGain_UI_hw)
{
    MS_U8 u8UIgain = 0x80;
    u8UIgain = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_0C_L,0x00FF);
    (*u8DetailGain_UI_hw) = minmax(((u8DetailGain_hw*u8UIgain)>>7),0x2,0xF);
}

MS_U8 SetAdaptiveParameter(MS_U8 u8Widdth, MS_BOOL isSD_source)
{
    MS_U32 u32PureBlockSumM = 0;
    MS_U32 u32PureBlockSumL = 0;
    MS_U32 u32DetailSumM = 0;
    MS_U32 u32DetailSumL = 0;
    MS_U32 u32PureBlockSumTotal = 0;
    MS_U32 u32DetailSumTotal = 0;
    MS_U8 u8PostGain = 8;
    MS_U8 u8VoidGain = 0;
    MS_U8 u8DBKGain_hw=0;
    MS_U8 u8DBK_coase_TH = 2;
    MS_U8 u8DetailGain_hw = 0;
    MS_U8 u8STD_Threshold_hw=0;
    MS_U32 BlockyPercentage = 0.0;
    static MS_U32 BlockyPercentageIIR = 0.0;
    static MS_BOOL b8Init = FALSE;
    MS_U8 u8DetailGain_UI_hw = 0;

    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_38_L, 0x0002, 0x00C2);//coring enable
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_3D_L, 0x0004, 0x00FF);//coring thrd
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_40_L, 0x0002, 0x000E);//coase step
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x0900, 0x0F30);//ref left&right
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_08_L, 0x2000, 0x2000);//weight fun enable
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_30_L, 0x0008, 0x0008);//cont3t
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_41_L, 0x0000, 0x0008);//disable real HD
    u32PureBlockSumM = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_39_L, 0xFFFF);
    u32PureBlockSumL = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_3A_L, 0xFFFF);
    u32DetailSumM = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_3B_L, 0xFFFF);
    u32DetailSumL = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_3C_L, 0xFFFF);
    
    u32PureBlockSumTotal = (u32PureBlockSumM<<16) + u32PureBlockSumL;
    u32DetailSumTotal = (u32DetailSumM<<16) + u32DetailSumL;
    BlockyPercentage = ((MS_U64)(256*u32PureBlockSumTotal)/(MS_U64)(u32PureBlockSumTotal+u32DetailSumTotal));


    if(FALSE == b8Init)
    {
        BlockyPercentageIIR = BlockyPercentage;
        b8Init = TRUE;
    }
    BlockyPercentageIIR = (6*BlockyPercentageIIR + 2*BlockyPercentage)>>3;

    if(u8Widdth == 8)
    {
        if(BlockyPercentageIIR > 47)
        {
            u8STD_Threshold_hw = (MS_U8)(((211*BlockyPercentageIIR+128)>>8) - 8);
            u8DetailGain_hw = (MS_U8)(((49*BlockyPercentageIIR+128)>>8) -6);
            u8DBKGain_hw = (MS_U8)(((42*BlockyPercentageIIR+128)>>8) +1);
            u8DBK_coase_TH = (MS_U8)(((13*BlockyPercentageIIR+128)>>8));
        }
        else if((BlockyPercentageIIR > 38)&&(isSD_source)&&(SZ_PATTERN_ENABLE==1)) //for BOOK1 chapter#5 child face noise pattern
        {
             u8STD_Threshold_hw = (MS_U8)(((211*BlockyPercentageIIR+128)>>8) - 8);
            u8DetailGain_hw = (MS_U8)(((80*BlockyPercentageIIR+128)>>8) -6);
            u8DBKGain_hw = (MS_U8)(((42*BlockyPercentageIIR+128)>>8) +1);
            u8DBK_coase_TH = (MS_U8)(((13*BlockyPercentageIIR+128)>>8));
        }
        else
        {
            u8STD_Threshold_hw = 0x3F;
            u8DetailGain_hw = 0x02;
            u8DBKGain_hw = 0x08;
            u8DBK_coase_TH = 0x02;
        }
    }
    else if(u8Widdth == 11)
    {
        u8STD_Threshold_hw = 0x8F;
        u8DetailGain_hw = 0x0A;
        u8DBKGain_hw = 0x0F;
        u8DBK_coase_TH = 0x04;
    }
    else
    {
        u8STD_Threshold_hw = 0xBF ;
        u8DBKGain_hw = 0x0F;
        u8DBK_coase_TH = 0x0A;
        u8DetailGain_hw = 0x0F;
        u8PostGain = 0x04;
        u8VoidGain = 0x08;
    }
    u8STD_Threshold_hw = minmax(u8STD_Threshold_hw,0x3F,0xFF);
    u8DetailGain_hw = minmax(u8DetailGain_hw,0x02,0x0F);
    u8DBKGain_hw = minmax(u8DBKGain_hw,0x08,0x0F);
    u8DBK_coase_TH = minmax(u8DBK_coase_TH,0x02,0x0A);

    UIcontrolMpegNR(u8DetailGain_hw,&u8DetailGain_UI_hw);

    //printf("Set u8DetailGain_UI_hw:%d u8DetailGain_hw:%d\n",u8DetailGain_UI_hw,u8DetailGain_hw);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_11_L, u8DBKGain_hw, 0x000F); //DBK Gain
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_10_L, u8STD_Threshold_hw<<8, 0xFF00); //DBK STD threshold
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_40_L, (u8DBK_coase_TH<<8), 0xFF00); //DBK coase threshold
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_42_L, (u8DetailGain_UI_hw<<12), 0xF000); //blockness gain
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK47_30_L, ((u8VoidGain<<12)+(u8PostGain<<8)), 0xFF00); //Post VS Void gain

    if(u8Widdth == 21 ||u8Widdth == 16)//Scaling up SD2HD
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_41_L, 0x0001, 0x0001); //hd mode enable
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_71_L, 0x00C0, 0x00C0); //switch to Y 7tap filter
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_02_L, 0x0006, 0x0006); //switch to C 7tap filter
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_72_L, 0x0000, 0x0006); //smooth step 0
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x0A86, 0x0F87); //blockness extend enable/switch pulse filter
    }
    else
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_41_L, 0x0000, 0x0001); //hd mode disable
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_71_L, 0x0040, 0x00C0); //switch to Y 5tap filter
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_72_L, 0x0002, 0x0006); //smooth step 1
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_02_L, 0x0002, 0x0006); //switch to C 3tap filter
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x0905, 0x0F87); //blockness extend disable/switch pulse filter
    }

    //printf("Set Function u8STD_Threshold_hw:%d u8DetailGain_hw:%d\n",u8STD_Threshold_hw,u8DetailGain_hw);
    //printf("Set Function u8DBKGain_hw:%d u8DBK_coase_TH:%d\n",u8DBKGain_hw,u8DBK_coase_TH);
    BlockyPercentageIIR = BlockyPercentageIIR;
    return u8STD_Threshold_hw;
}

void VSetAdaptiveParameter(MS_U8 u8VWiddth,MS_U8 u8HDBKStrength,MS_U8 u8VConfidence2Strength)
{
    MS_U32 u32PureBlockSumM = 0;
    MS_U32 u32PureBlockSumL = 0;
    MS_U32 u32DetailSumM = 0;
    MS_U32 u32DetailSumL = 0;
    MS_U32 u32PureBlockSumTotal = 0;
    MS_U32 u32DetailSumTotal = 0;
    MS_U8 u8PostGain = 0xF;
    MS_U8 u8VoidGain = 0x8;
    MS_S8 s8VDBKGain=6;
    MS_U8 u82DfilterDiffth = 0x3F;
    static MS_S8 s8VDBKGainTemp = 6;
    //double BlockyPercentage = 0.0;
    //static double BlockyPercentageIIR = 0.0;
    MS_U32 BlockyPercentage = 0.0;
    static MS_U32 BlockyPercentageIIR = 0.0;
    static MS_BOOL b8Init = FALSE;
    MS_BOOL bifPIP_en = (MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK20_10_L, 0x0003) == 0x0003);
    MS_BOOL blineBuffer = (MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK20_10_L, 0x0010) == 0x0010);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_78_L, 0x2000, 0x2000);//weight fun enable
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_20_L, 0x0006, 0x0006);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_20_L, 0x1000, 0x1000);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_21_L, 0x0147, 0x0377);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_22_L, 0x0011, 0x0033);

    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_23_L, 0x8000, 0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_24_L, 0x4088, 0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_25_L, 0x0182, 0x07FF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_26_L, 0x008F, 0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_27_L, 0x0888, 0x0FFF);
    //Statistics
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_28_L, 0x8000, 0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_29_L, 0x4088, 0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_2A_L, 0x0182, 0x07FF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_2B_L, 0x008F, 0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_2C_L, 0x0888, 0xFFFF);

    u32PureBlockSumM = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_69_L, 0xFFFF);
    u32PureBlockSumL = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_6A_L, 0xFFFF);
    u32DetailSumM = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_6B_L, 0xFFFF);
    u32DetailSumL = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_6C_L, 0xFFFF);
    u32PureBlockSumTotal = (u32PureBlockSumM<<16) + u32PureBlockSumL;
    u32DetailSumTotal = (u32DetailSumM<<16) + u32DetailSumL;
    BlockyPercentage = ((MS_U64)(256*u32PureBlockSumTotal)/(MS_U64)(u32PureBlockSumTotal+u32DetailSumTotal));
    if(FALSE == b8Init)
    {
        BlockyPercentageIIR = BlockyPercentage;
        b8Init = TRUE;
    }
    BlockyPercentageIIR = (6*BlockyPercentageIIR + 2*BlockyPercentage + 4)>>3;
    //printf("Set u32PureBlockSumM :%d,u32PureBlockSumL :%d,u32DetailSumM :%d,u32DetailSumL :%d\n",u32PureBlockSumM,u32PureBlockSumL,u32DetailSumM,u32DetailSumL);
    //printf("Set BlockyPercentage:%d,u32DetailSumTotal:%d,u32PureBlockSumTotal:%d\n",BlockyPercentage,u32DetailSumTotal,u32PureBlockSumTotal);
    //printf("V Set u8VWiddth:%d u8HDBKStrength:%d,u8VConfidence2Strength:%d BlockyPercentageIIR:%f\n",u8VWiddth,u8HDBKStrength,u8VConfidence2Strength,BlockyPercentageIIR);

    if ((bifPIP_en) || (u8HDBKStrength <= 0x3F))
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_20_L, 0x0000, 0x0001); //VDBK disable for PIP
    }
    else
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_20_L, 0x0001, 0x0001); //VDBK enable
    }

    if(u8VConfidence2Strength > 10)
    {
        if(u8VWiddth == 8)
        {
            s8VDBKGain = (BlockyPercentageIIR*64+128)>>8;//Casdade mode BlockyPercentageIIR*64 - 8;
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_20_L, 0x1000, 0x3000); //v filter 3tap
        }
        else if(u8VWiddth == 11)
        {
            s8VDBKGain = ((BlockyPercentageIIR*256+128)>>8) -44;
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_20_L, 0x1000, 0x3000); //v filter 3tap
        }
        else
        {
            s8VDBKGain = 0x0F;
            if(1 ==blineBuffer)
            {
                MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_20_L, 0x3000, 0x3000); //v filter 5tap
            }
            else
            {
                MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_20_L, 0x1000, 0x3000); //v filter 3tap
            }
        }
        u82DfilterDiffth = (MS_U8)((BlockyPercentageIIR*512+128)>>8);
        u82DfilterDiffth = minmax(u82DfilterDiffth,0x3F,0xFF);
        s8VDBKGain = minmax(s8VDBKGain,0x08,0x0F);
        s8VDBKGainTemp = s8VDBKGain;
    }
    else
    {
        s8VDBKGain = (s8VDBKGainTemp*(10-u8VConfidence2Strength) +6*u8VConfidence2Strength)/10;
    }

    u82DfilterDiffth = minmax(u82DfilterDiffth,0x3F,0xFF);
    s8VDBKGain = minmax(s8VDBKGain,0x08,0x0F);
    BlockyPercentageIIR = BlockyPercentageIIR;
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_18_L, u82DfilterDiffth<<8, 0xFF00); // 2D filter diff th
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK60_22_L, s8VDBKGain<<8, 0x0F00); //v total gain
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK47_60_L, ((u8VoidGain<<12)+(u8PostGain<<8)), 0xFF00); //Post VS Void gain
    //printf("set Function V u8VConfidence:%d s8VDBKGain:%d\n",u8VConfidence2Strength,s8VDBKGain);
    //printf("Set Function u8DBKGain_hw:%d Blocky:%d\n",s8VDBKGain,BlockyPercentageIIR);
}

void Mdrv_PQ_DBK(MS_BOOL isSceneChangeY,MS_U16 u16MotionValue,MS_U16 u16Width,MS_U16 u16height, const MS_BOOL isSD_source)
    {
     //DBK Variables
    MS_BOOL bSceneChange = 0;
    static MS_BOOL bDBK_initial = 0;
    MS_U8 u8ReadDBKHistAck = 0,u8VReadDBKHistAck = 0;
    static MS_U8 u8CountForSample = 0;
    static MS_U8 u8CountForDBK = 0;
    static MS_U8 u8Widdth = 8,u8VWidth = 8;
    static MS_U32 u32StaticValueTemp=0,u32StaticCount=0;
    //int log_en = 5;
    //log_en = (MS_U32)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_01_L, 0x000F); //easter
    //MS_U8 u8StartPosition = 0,u8VStartPosition = 0;
    //MS_U8 u8SramError = 0;
    MS_U8 u8DebugMode = 0;
    static MS_U8 u8HwidthConfidence = 0;
    static MS_U8 u8VConfidence2Strength = 0;
    MS_U8 u8HDBKStrength = 0;
    //u8SramError = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_0B_L, 0x0100)>>8;
    u8DebugMode = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_37_L, 0x0007 ) |MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK47_67_L, 0x0007 );
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK47_08_L, u16Width, 0x1FFF);//set H maxline_addr_end of readout
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK47_78_L, u16height, 0x1FFF);//set V maxline_addr_end of readout
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK47_30_L, 0x009B, 0x00FF); //HW enable LPF/Extend enable
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK47_60_L, 0x008B, 0x00FF); //HW enable LPF/Extend enable


    if(u16MotionValue ==0 && u32StaticValueTemp==0)
    {
        u32StaticCount++;
    }
    else
    {
        u32StaticCount = 0;
    }
    u32StaticValueTemp = u16MotionValue;

    bSceneChange = ScenceChangeConfirm(u32StaticCount,isSceneChangeY);

    if (bSceneChange)
    {
        u8CountForDBK = 0;
    }
    //printf("SC u8CountForDBK:%d\n",u8CountForDBK);
    //printf("SC u32StaticCount:%d ,isSceneChangeY:%d\n",u32StaticCount,isSceneChangeY);
    //printf("SC bRealSceneChange:%d\n",bSceneChange);

    if (u8CountForDBK == 0) // Read Histogram
    {
        if(u8DebugMode == 0)
        {
            u8ReadDBKHistAck = readHistogramPart(u8CountForDBK);
            u8VReadDBKHistAck = readVHistogramPart(u8CountForDBK);
            //printf("easter request histogram\n");
        }
        bDBK_initial = u8ReadDBKHistAck |u8VReadDBKHistAck;
        if((u8ReadDBKHistAck==1)&& (u32StaticCount < 15))
        {
            u8Widdth = DBK_Width_Detect(bSceneChange,&u8HwidthConfidence);
            if(u8Widdth > 2 && u8Widdth<32)
            {
                MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK47_32_L, u8Widdth, 0x003F); //Mark width input
                MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK47_34_L, u8Widdth, 0x00FF); //Mark initial input
                DBK_FindStartPosition(u8Widdth,bSceneChange);
                //printf("H u8Widdth:%d u8StartPosition:%d\n",u8Widdth,u8StartPosition);
            }
        }

        if((u8VReadDBKHistAck==1)&& (u32StaticCount < 15))
        {
            u8VWidth = VDBK_Width_Detect(bSceneChange,u8Widdth,u8HwidthConfidence,&u8VConfidence2Strength);
            if(u8VWidth > 2 && u8VWidth <21)
            {
                MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK47_62_L, u8VWidth, 0x003F); //Mark width input
                MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK47_64_L, u8VWidth, 0x00FF); //Mark initial input
                VDBK_FindStartPosition(u8VWidth,bSceneChange);
                //printf("V u8VWidth:%d u8VStartPosition:%d\n",u8VWidth,u8VStartPosition);
            }
        }

        if (bDBK_initial)
        {
            u8CountForDBK++;
        }
    }
    else if  (u8CountForDBK == 1)
    {
        if ( bDBK_initial == 1)
        {
            u8HDBKStrength = SetAdaptiveParameter(u8Widdth, isSD_source);
            VSetAdaptiveParameter(u8VWidth,u8HDBKStrength,u8VConfidence2Strength);
            //printf("u8HDBKStrength:%d\n",u8HDBKStrength);
        }
        u8CountForDBK++;
        u8CountForSample++;

    }

    if(u8CountForDBK >= 2)
    {
        u8CountForDBK = 0;
    }

}

//------------- MV0 comfirm for such as like "Piano Bar" motionless pattern ----------------------//
#define KFCnonHistoryMotionDivideMode_MaxValue 0x02
#define KFCnonHistoryMotionDivideMode_MinValue 0x01
void MDrv_SC_KFC_EODiW_Motionless(void)
{
    MS_U32 mv0Comfirm = 0,  nonMV0Comfirm = 0, mvNotFound = 0, luma = 0;
    static MS_U32 prevMV0Comfirm = 0;
    static MS_U32 prevNonMV0Comfirm = 0;
    static MS_U32 prevMvNotFound = 0;
    static MS_U32 prevLuma = 0;
    static MS_U32 prevComfirmValue = 0;
    MS_U32 comfirmValue = 0;
    static MS_U32 prevComfirmValue2 = 0;
    MS_U32 comfirmValue2 =0;
    static MS_U32 KFCnonHistoryMotionDivideMode = KFCnonHistoryMotionDivideMode_MaxValue;

    mv0Comfirm          = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK09_78_L,0xFFFF);
    nonMV0Comfirm     = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK09_79_L,0xFFFF);
    mvNotFound          = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK09_7A_L,0xFFFF);
    luma            = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK55_1A_L,0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_1B_L, 0x000F )<<16); //move to UFSC in Maserati

    mv0Comfirm = (prevMV0Comfirm * 3 + mv0Comfirm) >> 2;
    prevMV0Comfirm = mv0Comfirm;

    nonMV0Comfirm = (prevNonMV0Comfirm * 3 + nonMV0Comfirm) >> 2;
    prevNonMV0Comfirm = nonMV0Comfirm;

    mvNotFound = (prevMvNotFound * 3 + mvNotFound) >> 2;
    prevMvNotFound = mvNotFound;

    luma = (prevLuma * 3 + luma) >> 2;
    prevLuma = luma;
    //  ==================================================  //
    /* mv0 check !! */
    if(mv0Comfirm > (mvNotFound * 20) )
        comfirmValue = mv0Comfirm - mvNotFound * 20;
    else
        comfirmValue = 0;

    comfirmValue = (prevComfirmValue * 3 + comfirmValue) >> 2;
    prevComfirmValue = comfirmValue;

    //  ==================================================  //
    /* Small moving check !! */
    if(mv0Comfirm > (nonMV0Comfirm * 1) )
        comfirmValue2 = mv0Comfirm - nonMV0Comfirm * 1;
    else
        comfirmValue2 = 0;

    comfirmValue2 = (prevComfirmValue2 * 3 + comfirmValue2) >> 2;
    prevComfirmValue2 = comfirmValue2;

    //  ==================================================  //
    if( (comfirmValue > 100) && (comfirmValue2 > 100) && (luma < 0x2000) )
    {
        //KFCnonHistoryMotionDivideMode = 0x00;
        if(KFCnonHistoryMotionDivideMode > KFCnonHistoryMotionDivideMode_MinValue)
            KFCnonHistoryMotionDivideMode--;
        else
            KFCnonHistoryMotionDivideMode = KFCnonHistoryMotionDivideMode_MinValue;
    }
    else
    {
        if(KFCnonHistoryMotionDivideMode < KFCnonHistoryMotionDivideMode_MaxValue)
            KFCnonHistoryMotionDivideMode++;
        else
            KFCnonHistoryMotionDivideMode = KFCnonHistoryMotionDivideMode_MaxValue;
    }
    /* Main window KFC non-history motion divide mode */
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK22_28_L, KFCnonHistoryMotionDivideMode << 4, 0x0030);
}

MS_BOOL MDrv_SC_HQV_JaggedTestPattern( const XC_GMvStates gMv )
{
    MS_BOOL isMvX0 = (gMv.minMvXdir == 32) ? true : false;
    MS_BOOL isOddY = ( gMv.minMvYdir % 2 ) ? true : false;
    MS_BOOL maxBinNonFocus = ( gMv.h1XYsum < 0x10 ) ? true : false;

    if( isMvX0 && isOddY && maxBinNonFocus )
    {
        return  true;
        //printf("MDrv_SC_HQV_JaggedTestPattern == TRUE\n\n");
    }
    else
    {
        return false;
        //printf("MDrv_SC_HQV_JaggedTestPattern == FALSE\n\n");
    }
    //printf("isMvX0 == %d, isOddY == %d, maxBinNonFocus == %d\n\n", isMvX0, isOddY, maxBinNonFocus);
}

#define REG_MCDI_EODIW_SHIFT_MAX 3
#define REG_MCDI_EODIW_SHIFT_MIN 1
#define REG_MCDI_EODIW_AB_MAX_GAIN_MAX 12
#define REG_MCDI_EODIW_AB_MAX_GAIN_MIN 4
#define REG_MCDI_EODIW_MIN_SAD_GAIN_MAX 6
#define REG_MCDI_EODIW_MIN_SAD_GAIN_MIN 1
void MDrv_SC_Agate_SubMvPanScan( const XC_GMvStates gMv, const MS_BOOL isSceneChange, const MS_BOOL isSdHqvJaggedTest, const MS_U16 motion )
{
    MS_BOOL bisSubMvPanScan = FALSE;
    static MS_U8 u8iir_cnt = 0;
    MS_BOOL bisMv0 = ( ( gMv.minMvXdir == 32 ) && ( gMv.minMvYdir == 8 ) ) ? true : false;
    static MS_U8 u8isMv0Cnt = 0;
    static MS_U8 u8isSubMvPanScanCnt = 0;

    static MS_U16 u16reg_mcdi_eodiw_shift = REG_MCDI_EODIW_SHIFT_MAX;
    static MS_U16 u16reg_mcdi_eodiw_ab_max_gain = REG_MCDI_EODIW_AB_MAX_GAIN_MIN;
    static MS_U16 u16reg_mcdi_eodiw_min_sad_gain = REG_MCDI_EODIW_MIN_SAD_GAIN_MAX;


    if( bisMv0 && ( motion < 0x100 ) )
    {
        if( u8isMv0Cnt < 20 )
            u8isMv0Cnt++;
    }
    else
    {
        if( u8isMv0Cnt > 0 )
            u8isMv0Cnt--;
    }

    if( ( gMv.h1XYsum > 10 ) && ( gMv.h2XYsum > 10 )
        &&( gMv.h1XYdiff < 5 ) && ( gMv.h2XYdiff < 5 ) && gMv.yMvIsMovingOdd )
    {
        bisSubMvPanScan = true;
    }
    else
    {
        bisSubMvPanScan = false;
    }


    if( bisSubMvPanScan )
    {
        if( u8isSubMvPanScanCnt < 20 )
            u8isSubMvPanScanCnt++;
    }
    else
    {
        if( u8isSubMvPanScanCnt > 0 )
            u8isSubMvPanScanCnt--;
    }


    //PRT_TRP(" isSubMvPanScan(%d)\n", isSubMvPanScan);
    //PRT_TRP(" isSubMvPanScan(%d)\n", isSubMvPanScan);

    if( ( u8isSubMvPanScanCnt > 5 ) || isSdHqvJaggedTest )
    {
        u8iir_cnt = 240;
    }
    else
    {
        if( u8iir_cnt != 0 )
            u8iir_cnt--;
    }

    if( isSceneChange ||  ( u8isMv0Cnt > 10 ) ) /* for pattern stable, if only check GMV-Mv0 have problem */
        u8iir_cnt = 0;

    if( u8iir_cnt == 0 )
    {
        if( u16reg_mcdi_eodiw_shift < REG_MCDI_EODIW_SHIFT_MAX )
            u16reg_mcdi_eodiw_shift++;

        if( u16reg_mcdi_eodiw_ab_max_gain > REG_MCDI_EODIW_AB_MAX_GAIN_MIN )
            u16reg_mcdi_eodiw_ab_max_gain--;

        if( u16reg_mcdi_eodiw_min_sad_gain < REG_MCDI_EODIW_MIN_SAD_GAIN_MAX )
            u16reg_mcdi_eodiw_min_sad_gain++;
    }
    else/* do sub pixel */
    {
        if( u16reg_mcdi_eodiw_shift > REG_MCDI_EODIW_SHIFT_MIN )
            u16reg_mcdi_eodiw_shift--;

        if( u16reg_mcdi_eodiw_ab_max_gain < REG_MCDI_EODIW_AB_MAX_GAIN_MAX )
            u16reg_mcdi_eodiw_ab_max_gain++;

        if( u16reg_mcdi_eodiw_min_sad_gain > REG_MCDI_EODIW_MIN_SAD_GAIN_MIN )
            u16reg_mcdi_eodiw_min_sad_gain--;
    }

    MApi_GFLIP_XC_W2BYTEMSK( REG_SC_BK22_29_L, u16reg_mcdi_eodiw_shift  , 0x000F );
    MApi_GFLIP_XC_W2BYTEMSK( REG_SC_BK22_29_L, (u16reg_mcdi_eodiw_ab_max_gain<<12) , 0xF000 );

     //Dummy to control write value, add by Bella 20150708
        if( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK23_5D_L,0x0F00)!=0x0000 )
        {
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK22_2A_L, MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK23_5D_L,0xFFFF), 0x0F00);
         }
        else
         {
        MApi_GFLIP_XC_W2BYTEMSK( REG_SC_BK22_2A_L, (u16reg_mcdi_eodiw_min_sad_gain<<8) , 0x0F00 );
         }
}

void MDrv_SC_EODi_Armored( const XC_GMvStates gMv , const MS_U16 mv0Comfirm , const MS_U16 nonMv0Comfirm )
{
    MS_BOOL is_still = FALSE;
    MS_BOOL isMv0 = ( ( gMv.minMvXdir == 32 ) && ( gMv.minMvYdir == 8 ) ) ? true : false;
    static MS_U8 confirm_cnt = 0;
    static MS_BOOL reg_eodi_pwsam_en = false;

    if( isMv0 && (mv0Comfirm > 0x15) && (nonMv0Comfirm < 8))
        is_still = true;
    else
        is_still = false;

    if( is_still )
    {
        confirm_cnt = confirm_cnt + 5;

        if(confirm_cnt > 20)
            confirm_cnt = 20;
    }
    else
    {
        if(confirm_cnt > 0)
            confirm_cnt--;
        else
            confirm_cnt = 0;
    }

    if( confirm_cnt >= 10)
    {
        reg_eodi_pwsam_en = true;
    }
    else
    {
        reg_eodi_pwsam_en = false;
    }
    MApi_GFLIP_XC_W2BYTEMSK( REG_SC_BK21_3D_L, (MS_U16)( reg_eodi_pwsam_en << 3) , 0x0008 );
}

/*Added for VG846 Sparse Pattern to avoid forcing MCDi function */
MS_BOOL MDrv_SC_ukoSabihS_SparsePattern( const MS_U16 mvNotFound, const MS_U16 mvNotFoundME2, const MS_U16 mv0Comfirm, const MS_U16 nonMv0Comfirm, const MS_U16 mv0ComfirmME2, const MS_U16 nonMv0ComfirmME2, const MS_BOOL isHDSrc )
{

    MS_BOOL  isSparse = 0;

    int notFound = (int)mvNotFound;
    int notFoundME2 = (int)mvNotFoundME2;
    int MVConfirm = (int) mv0Comfirm;
    int MVConfirmME2 = (int) mv0ComfirmME2;
    int nonmv0 = (int) nonMv0Comfirm;
    int nonmv0ME2 = (int) nonMv0ComfirmME2;
    int notFoundTH = 0;
    int notFoundME2TH = 0;

    /*if(isHDSrc)
    {
        isSparse = ( notFound > 0x200 ) && ( notFoundME2 > 0xF00 ) ? true : false;
    }
    else
    {
        isSparse = ( notFound > 0x40 ) && ( notFoundME2 > 0x800 ) ? true : false;
    }*/

    if(isHDSrc)
    {
         notFoundTH = (0x200* normalize_factor)/256;
     notFoundME2TH = (0xF00* normalize_factor)/256;
    }
    else
    {
       notFoundTH = (0x40* normalize_factor)/256;
       notFoundME2TH = (0x800* normalize_factor)/256;
    }
    isSparse = ( notFound > notFoundTH ) && ( notFoundME2 > notFoundME2TH ) ? true : false;

    if (isSparse && (notFound >(MVConfirm+nonmv0))&& (notFoundME2 >(MVConfirmME2+nonmv0ME2)))
    {
            return true;
    }
    else
    {
            return false;
     }

}

MS_BOOL MDrv_SC_ukoSabihS_HBurstMoving( const MS_U32 _hpfSdCntReport )
{
    MS_U32 mv0_feather = 0;
    MS_BOOL isHBurst_act = FALSE;
    static MS_U8 u8HBurstConfirmCnt = 0;

    mv0_feather = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_77_L,0xFFFF);

    
    isHBurst_act = ( ((mv0_feather) > (_hpfSdCntReport*100)) && ( _hpfSdCntReport < 0x10 ) ) ? true : false;
    if(isHBurst_act)
    {
        if(u8HBurstConfirmCnt <= 0x20)
        {
            u8HBurstConfirmCnt++;
        }
    }
    else
    {
        u8HBurstConfirmCnt = 0;
    }

    if(u8HBurstConfirmCnt > 10)
    {
        return true;
    }
    else
    {
        return false;
    }
}

MS_BOOL MDrv_SC_ukoSabihS_HBurst( const MS_U32 _hpfSdCntReport, const MS_U32 uComplex )
{
    MS_BOOL isHBurst = FALSE;

    isHBurst = ( ((uComplex) > (_hpfSdCntReport*100)) && ( _hpfSdCntReport < 0x10 ) ) ? true : false;

    return isHBurst;
}


#define S_UPPERBOUND_TH 15
#define S_MATCH_TH 10
MS_BOOL MDrv_SC_Agate_ukoSabihS( const XC_ApiStatus *stXCStatus, const XC_GMvStates gMv ,const MS_U16 nonMv0Comfirm, const XC_FilmStates filmstatus, const MS_BOOL isMcFilm,
                MS_BOOL *isUkosabihsMotion, MS_BOOL *isUkosabihsMc, MS_BOOL *isUkosabihsForFilm, const MS_U16 mv0Comfirm, MS_BOOL *isUkosabihsMv0, const MS_BOOL isHDSrc )
{
    MS_BOOL is_ukoSabihS = FALSE, is_ukoSabihSmv0 = FALSE;
    MS_BOOL isMv0 = (( gMv.minMvXdir == 32 ) && ( gMv.minMvYdir == 8 )) ? true : false;
    MS_BOOL isFilm = ( filmstatus.film_act || isMcFilm ) ? true : false;
    //printk("Fim: %d %d\n",filmstatus.film_act,isMcFilm);
    MS_BOOL isWithoutMv0Comfirm = ( mv0Comfirm < 0x20 ) ? true : false;
    MS_BOOL isCVBSinput,isTVinput;
    MS_U8 XYDiffTH= 0,SpeedUp_TH = 0,Confirm_TH = 0;
    int h2XYsumTH_HD = 0, h2XYsumTH_SD = 0,nonMv0ComfirmTH = 0, mv0ComfirmTH=0;

    static MS_U8 confirm_cnt = 0;
    static MS_U8 confirm_cnt_mv0 = 0;
    static MS_U8 even_cnt = 0;

    static MS_U8 confirm_cnt_film = 0;

    //MS_BOOL is_ukoSabihS_isHBurst = ( isHBurstMoving && ( gMv.h2XYdiff > 0x45 )) ? true : false;

    //changed bcz move driver to kernel
    //isCVBSinput = ((stXCStatus->bInterlace) && (stXCStatus->enInputSourceType >= INPUT_SOURCE_CVBS) && (stXCStatus->enInputSourceType <= INPUT_SOURCE_CVBS_MAX));
    //isTVinput = ((stXCStatus->bInterlace) && (stXCStatus->enInputSourceType == INPUT_SOURCE_TV));
    
    isCVBSinput = ((stXCStatus->bInterlace) && (u8InputTypeFromCFI == E_CFD_INPUT_SOURCE_CVBS) );
    isTVinput = ((stXCStatus->bInterlace) && (u8InputTypeFromCFI == E_CFD_INPUT_SOURCE_TV));


    if(isHDSrc)
    {
        SpeedUp_TH = 200;
    }
    else
    {
        SpeedUp_TH = 40;
    }

    if(isCVBSinput || isTVinput)
    {
        XYDiffTH = 0x01;
    Confirm_TH =  0x80;
    }
    else
    {
        XYDiffTH = 0x06;
    Confirm_TH =0x40 ;
    }

    SpeedUp_TH = 200;
    nonMv0ComfirmTH = 0x40;
    mv0ComfirmTH = 0x200;
    h2XYsumTH_HD = 0x32; //2016.04.27 think the value is error. original h2XYsumTH_HD = 0x16, h2XYsumTH_SD = 0x32 
    h2XYsumTH_SD = 0x16;

    nonMv0ComfirmTH = (nonMv0ComfirmTH * normalize_factor) / 256;
    mv0ComfirmTH = (mv0ComfirmTH * normalize_factor) / 256;
    h2XYsumTH_HD = (h2XYsumTH_HD * normalize_factor) / 256;
    h2XYsumTH_SD = (h2XYsumTH_SD * normalize_factor) / 256;
    SpeedUp_TH = (SpeedUp_TH * normalize_factor) / 256;
    XYDiffTH = (XYDiffTH *normalize_factor) / 256;
    Confirm_TH = (Confirm_TH *normalize_factor) / 256;

    //printk("MV: %d %d %d %d %d %d %d %d %d %d\n",gMv.h2XYsum,h2XYsumTH_HD,h2XYsumTH_SD,gMv.h2XYdiff,XYDiffTH,isMv0,nonMv0Comfirm,Confirm_TH,isWithoutMv0Comfirm,isHDSrc);

    if(isCVBSinput||isTVinput)
    {
        is_ukoSabihS = false;
    }
    else if((gMv.h2XYsum > h2XYsumTH_HD)&& (gMv.h2XYdiff <= XYDiffTH)&& (!isMv0) && (nonMv0Comfirm> Confirm_TH ) && isWithoutMv0Comfirm && (isHDSrc) )
    {
        is_ukoSabihS = true;
    }
    else if((gMv.h2XYsum > h2XYsumTH_SD)&& (gMv.h2XYdiff <= XYDiffTH)&& (!isMv0) && (nonMv0Comfirm> Confirm_TH ) && isWithoutMv0Comfirm )
   {
        is_ukoSabihS = true;
    }
    else
    {
        is_ukoSabihS = false;
    }

    if( (!gMv.yMvIsMovingOdd) )
    {
        even_cnt++;

        if(even_cnt > S_UPPERBOUND_TH)
            even_cnt = S_UPPERBOUND_TH;
    }
    else
    {
        even_cnt = 0;
    }

    if( is_ukoSabihS )
    {
        if(gMv.h2XYsum > SpeedUp_TH )    //for Speed up//
        {
            confirm_cnt = confirm_cnt + 2;
        }
        else
        {
            confirm_cnt++;
        }

        if(confirm_cnt > S_UPPERBOUND_TH){
            confirm_cnt = S_UPPERBOUND_TH;
        }
    }
    else
    {
        if(gMv.h2XYsum > SpeedUp_TH )    //for Speed up//
        {
            if( confirm_cnt > 0 ){
                confirm_cnt--;
            }
        }
        else{
            confirm_cnt = 0;
        }
    }

    if (isMcFilm)
    {
        confirm_cnt = 0;
    }

    if(( confirm_cnt >= S_MATCH_TH)&&( even_cnt >= S_MATCH_TH) )
    {
        *isUkosabihsMc = true;
        //MApi_GFLIP_XC_W2BYTEMSK( REG_SC_BK22_7A_L, 0x1000 , 0x1000 );
    }
    else
    {
        *isUkosabihsMc = false;
        //MApi_GFLIP_XC_W2BYTEMSK( REG_SC_BK22_7A_L, 0x0000 , 0x1000 );
    }

    if( confirm_cnt >= S_MATCH_TH)
    {
        *isUkosabihsMotion = true;
    }
    else
    {
        *isUkosabihsMotion = false;
    }

    if( confirm_cnt >= S_MATCH_TH)
    {
        *isUkosabihsForFilm = true;
    }
    else
    {
        *isUkosabihsForFilm = false;
    }
    if( *isUkosabihsForFilm )
    {
        confirm_cnt_film++;

        if(confirm_cnt_film > S_UPPERBOUND_TH)
            confirm_cnt_film = S_UPPERBOUND_TH;
    }
    else
    {
        confirm_cnt_film = 0;
    }
    if( confirm_cnt_film >= S_MATCH_TH)
    {
        MApi_GFLIP_XC_W2BYTEMSK( REG_SC_BK30_09_L, 0x0001 , 0x0001 );
        //MApi_GFLIP_XC_W2BYTEMSK( REG_SC_BK30_41_L, 0x7000 , 0x7000 );
    }
    else
    {
        MApi_GFLIP_XC_W2BYTEMSK( REG_SC_BK30_09_L, 0x0000 , 0x0001 );
        //MApi_GFLIP_XC_W2BYTEMSK( REG_SC_BK30_41_L, 0x0000 , 0x7000 );
    }

    if( isFilm )
    {
        //confirm_cnt = 0;
        *isUkosabihsMc = false;
        *isUkosabihsMotion = false;
        even_cnt = 0;
    }
    //==============================================================================================================

    if((gMv.h2XYsum > 0x3)&& (gMv.h2XYdiff <= 1) && (isMv0) && (mv0Comfirm>mv0ComfirmTH))
    {
        is_ukoSabihSmv0 = true;
    }
    else
    {
        is_ukoSabihSmv0 = false;
    }

    if( is_ukoSabihSmv0 )
    {
        confirm_cnt_mv0++;

        if(confirm_cnt_mv0 > S_UPPERBOUND_TH )
            confirm_cnt_mv0 = S_UPPERBOUND_TH;
    }
    else
    {
        confirm_cnt_mv0 = 0;
    }

    if( confirm_cnt_mv0 >= S_MATCH_TH )
    {
        *isUkosabihsMv0 = true;
    }
    else
    {
        *isUkosabihsMv0 = false;
    }
    //==============================================================================================================
    //printk("isUhsMn = %d\n",*isUkosabihsMotion);
    return ( *isUkosabihsMc || *isUkosabihsMotion );
}

MS_BOOL MDrv_SC_sweep_ukoSabihS( const XC_GMvStates gMv,
                const MS_U16 nonMv0Comfirm, const MS_U16 mv0Comfirm,
                const XC_FilmStates filmstatus, const MS_BOOL isMcFilm,
                const MS_U32 uComplex,
                MS_BOOL *isUkosabihsMotionSweep )
{
    MS_BOOL is_ukoSabihS_sweep = FALSE;
    MS_BOOL isMv0 = (( gMv.minMvXdir == 32 ) && ( gMv.minMvYdir == 8 )) ? true : false;
    MS_BOOL isFilm = ( filmstatus.film22 || filmstatus.film22 || isMcFilm ) ? true : false;
    MS_BOOL isUkosabihsMcSweep = FALSE;

    static MS_U8 confirm_cnt = 0;
    static MS_U8 even_cnt = 0;

    if((gMv.h2XYsum > 0x9)&& (gMv.h2XYdiff <= 5) && (!isMv0) && (nonMv0Comfirm>0x300) && (uComplex>0x7300) && (mv0Comfirm <= 10) )
        is_ukoSabihS_sweep = true;
    else
        is_ukoSabihS_sweep = false;

    if( (!gMv.yMvIsMovingOdd) )
    {
        even_cnt++;

        if(even_cnt > 10)
            even_cnt = 10;
    }
    else
    {
        even_cnt = 0;
    }

    if( is_ukoSabihS_sweep )
    {
        confirm_cnt++;

        if(confirm_cnt > 10)
            confirm_cnt = 10;
    }
    else
    {
        confirm_cnt = 0;
    }

    if( isFilm )
    {
        confirm_cnt = 0;
        even_cnt = 0;
    }

    if(( confirm_cnt == 10)&&( even_cnt == 10) )
        isUkosabihsMcSweep = true;
    else
        isUkosabihsMcSweep = false;

    if( confirm_cnt == 10)
        *isUkosabihsMotionSweep = true;
    else
        *isUkosabihsMotionSweep = false;

    return isUkosabihsMcSweep;
}

MS_BOOL MDrv_SC_ukoSabihS_ArtifactPattern( const MS_U32 _hpfHdTolerantReport,const MS_BOOL isHDSrc )
{
    MS_BOOL isArtifactPattern_act = 0;
    static MS_U8 u8ArtifactPatternConfirmCnt = 0;

    if(isHDSrc)
    {
        isArtifactPattern_act = ( _hpfHdTolerantReport > 0x18000 ) ? true : false;
    }
    else
    {
        isArtifactPattern_act = ( _hpfHdTolerantReport > 0x4000 ) ? true : false;
    }

    if(isArtifactPattern_act)
    {
        if(u8ArtifactPatternConfirmCnt < 20)
        {
            u8ArtifactPatternConfirmCnt++;
        }
    }
    else
    {
        if(u8ArtifactPatternConfirmCnt > 0)
        {
            u8ArtifactPatternConfirmCnt--;
        }
    }

    if(u8ArtifactPatternConfirmCnt > 10)
    {
        return true;
    }
    else
    {
        return false;
    }

    //return isHBurstMoving;
}

MS_BOOL MDrv_SC_ukoSabihS_noLumaCplx( void )
{
    MS_U8 reg_main_max_pixel_sat = 0;
    MS_U8 reg_main_min_pixel_sat = 0;

    MS_U16 reg_feather_and_motion_pixel = 0;

    static MS_U8 confirmCnt = 0;

    reg_main_max_pixel_sat = (MS_U8)( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK1A_61_L,0x00FF)  ); //Move to UFSC in Maserati
    reg_main_min_pixel_sat = (MS_U8)(( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK1A_61_L,0xFF00)  ) >> 8); //Move to UFSC in Maserati
    reg_feather_and_motion_pixel = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_78_L,0xFFFF);

    if( ( reg_main_min_pixel_sat == 0 ) &&
        ( reg_main_max_pixel_sat == 0 ) &&
        ( reg_feather_and_motion_pixel > 0x5000 ) )
    {
        if( confirmCnt < 30 )
            confirmCnt++;
    }
    else
        confirmCnt = 0;

    return ( confirmCnt > 20 );
}

MS_BOOL MDrv_SC_McFilmUkoSabihS( const XC_OpFrameStates opFrameStatusCur, MS_BOOL isHd, MS_U32 _hpfHdCntReport )
{
    static XC_OpFrameStates opFrameStatusLast;
    static XC_McFilmStates mcFilmStates;
    MS_BOOL  isFilm;
    static MS_U8 isFilmCnt = 0;
    static MS_U8 continuousError = 0;
    MS_U8 lowerBoundTh = 0;
    
    if( isHd )
    {
        if (_hpfHdCntReport >= 50000)
            lowerBoundTh = 0xFF;//255
        else if (_hpfHdCntReport >= 32000)
            lowerBoundTh = 128+ 127*(_hpfHdCntReport-32000)/18000;
        else if (_hpfHdCntReport >= 6400)
            lowerBoundTh = 64+ 16*(_hpfHdCntReport-6400)/6400;
        else //if (_hpfHdCntReport <= 6400)
            lowerBoundTh = _hpfHdCntReport/100;
    }
    else
        lowerBoundTh = 0x80;

    mcFilmStates.filmLikePrvPixels = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_75_L, 0xFFFF);
    mcFilmStates.filmLikeNxtPixels = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_76_L, 0xFFFF);

    MS_U32 filmLikePrvPixelsAdj = max(mcFilmStates.filmLikePrvPixels - lowerBoundTh*2, 0);
    MS_U32 filmLikeNxtPixelsAdj = max(mcFilmStates.filmLikeNxtPixels - lowerBoundTh*2, 0);
    //DEBUG_LC_Full6bit3(max(mcFilmStates.filmLikePrvPixels, mcFilmStates.filmLikeNxtPixels )/16);
    //DEBUG_LC_Full6bit4(min(mcFilmStates.filmLikePrvPixels, mcFilmStates.filmLikeNxtPixels )/16);
    //DEBUG_LC_Full6bit5(lowerBoundTh);
    if( opFrameStatusCur.curOpFrame != opFrameStatusLast.curOpFrame )
    {
        if( (filmLikePrvPixelsAdj > 0) && (filmLikeNxtPixelsAdj > 0) &&
            ( (filmLikePrvPixelsAdj>filmLikeNxtPixelsAdj*5 && filmLikePrvPixelsAdj > lowerBoundTh)
            ||(filmLikeNxtPixelsAdj>filmLikePrvPixelsAdj*5 && filmLikeNxtPixelsAdj > lowerBoundTh) ) )
        {
            if( isFilmCnt < 63 ){
                isFilmCnt++;
            }

            continuousError = 0;
        }
        else if(((mcFilmStates.filmLikePrvPixels>(mcFilmStates.filmLikeNxtPixels*5))&&(mcFilmStates.filmLikePrvPixels > lowerBoundTh))
            ||((mcFilmStates.filmLikeNxtPixels>(mcFilmStates.filmLikePrvPixels*5))&&(mcFilmStates.filmLikeNxtPixels > lowerBoundTh)))
        {
            if( isFilmCnt < 63 ){
                isFilmCnt++;
            }

            continuousError = 0;
        }
        else
        {
            if( isFilmCnt > 0 ){
                isFilmCnt--;
            }

            if( continuousError < 63 ){
                continuousError++;
            }
        }

        if( continuousError > 4 ){
            isFilmCnt = 0;
        }
    }

    opFrameStatusLast.curOpFrame = opFrameStatusCur.curOpFrame;
    isFilm = ( isFilmCnt > 10 ) ? true : false;
    //DEBUG_LC_Full6bit2(isFilmCnt);

    return isFilm;
}

MS_BOOL MDrv_SC_McOutOfSearchRange( const MS_U16 mvNotFound, const MS_U16 mv0Comfirm, const MS_U16 nonMv0Comfirm, const MS_U16 isFeatherAndMotion, const XC_GMvStates gMv )
{

    MS_BOOL isBigGmvDiff = (( abs(32 - gMv.minMvXdir) > 15 ) || ( abs(8 - gMv.minMvYdir) > 4 ) ) ? true : false;
    MS_BOOL  isOutOfSearchRange = 0;
    static MS_U8 isCnt = 0;

    int notFound = (int)mvNotFound;
    int mv0 = (int) mv0Comfirm;
    int nonmv0 = (int) nonMv0Comfirm;

    /* where are nonMv0Comfirm ??? >_< */
    isOutOfSearchRange = ( (notFound > ((mv0+nonmv0)*8)) && ( (notFound) > 0x80 ) ) ? true : false;

    if( isOutOfSearchRange && (isFeatherAndMotion > 5) && isBigGmvDiff )
    {
        if( isCnt < 255 )
            isCnt++;
    }
    else
    {
        if( isCnt > 0 )
            isCnt--;
    }

    return ( isCnt > 15 ) ? true : false;
}

MS_BOOL MDrv_SC_McFilmLikeForSlowMotion( const MS_U32 motion, const XC_GMvStates gMv  )
{
    MS_BOOL isMv0 = ((gMv.minMvXdir == 32)&&(gMv.minMvYdir == 8)) ? true : false;
    MS_BOOL maxBinFocus = ( gMv.h1XYsum > 0x40 ) ? true : false;
    MS_BOOL isStill = ( motion < 0x100 ) ? true : false;

    static XC_McFilmStates mcFilmStates;
    MS_BOOL  isFilm = FALSE;
    static MS_U8 isFilmCnt = 0;
    static MS_U8 continuousError = 0;

    mcFilmStates.filmLikePrvPixels = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_75_L, 0xFFFF);
    mcFilmStates.filmLikeNxtPixels = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_76_L, 0xFFFF);

    if( (mcFilmStates.filmLikePrvPixels>(mcFilmStates.filmLikeNxtPixels*1) )
        || (mcFilmStates.filmLikeNxtPixels>(mcFilmStates.filmLikePrvPixels*1) ) )
    {
        if( isFilmCnt < 255 )
            isFilmCnt++;

        continuousError = 0;
    }
    else
    {
        if( isFilmCnt > 0 )
            isFilmCnt--;

        if( continuousError < 255 )
            continuousError++;
    }

    if( continuousError > 10 )
        isFilmCnt = 0;

    //isFilm = ( isFilmCnt > 10 ) ? true : false;
    isFilm = ( isFilmCnt > 5 ) ? true : false;

    return ( isFilm && isMv0 && maxBinFocus && isStill );
}

#define MF_Gain            6
MS_BOOL MDrv_SC_ZoomMoving( const XC_GMvStates gMv , const MS_U16 me1IsNonMv0, const MS_U16 me1IsNotFound, const MS_U16 featherAndMotion, const MS_U16 motion )
{

    //MS_BOOL isMvY0 = (gMv.minMvYdir == 8) ? true : false;
    MS_BOOL isMvY0 = (( abs(32 - gMv.minMvXdir) < 4 )&&(gMv.minMvYdir == 8)) ? true : false;
    //MS_BOOL maxBin = true;//( gMv.h1XYsum < 0x30 && gMv.h1XYsum > 0x04 ) ? true : false;
    MS_BOOL isMoving = ( motion > 0x200 ) ? true : false;

    MS_BOOL isSmallMoving = FALSE;
    static MS_U16 prev_featherAndMotion = 0;
    MS_U16 cur_featherAndMotion = 0;
    MS_BOOL withoutFeather = false;
    static MS_U8 confirm_cnt = 0;

    cur_featherAndMotion = featherAndMotion;

    //denyMoving = ( me1IsNonMv0 + me1IsNotFound );

    cur_featherAndMotion = (prev_featherAndMotion*3 + cur_featherAndMotion ) >> 2;
    prev_featherAndMotion = cur_featherAndMotion;

    withoutFeather = ( cur_featherAndMotion < 8 ) ? true : false;

    if( (( me1IsNotFound > (me1IsNonMv0*MF_Gain)) && ( me1IsNotFound > 0x30 )) && withoutFeather && isMvY0 && isMoving )
        isSmallMoving = true;
    else
        isSmallMoving = false;

    if( isSmallMoving )
    {
        if( confirm_cnt < 40 )
            confirm_cnt++;
    }
    else
    {
        if( confirm_cnt > 0 )
            confirm_cnt--;
    }


    return ( confirm_cnt > 30 );
}

#define L139_COLORSUM 0x0A /*CVBS*/
MS_BOOL MDrv_SC_Agate_L139( const MS_U32 mvComfirm, const MS_U32 colorSum, const MS_U32 uComplex, const MS_BOOL meStill, const  XC_Me1HisStates H2 /*, const MS_BOOL ukoSabihS_en*/ )
{
    MS_BOOL me1IsMv0 = 0;
    MS_BOOL isL139 = 0;

    if((H2.minMvXdir == 8) && (H2.minMvYdir == 8) &&(H2.minMvXBin > 13) && (H2.minMvYBin > 13))
    {
        me1IsMv0 = true;
    }
    else
    {
        me1IsMv0 = false;
    }

    if( (mvComfirm > 95) && (colorSum < L139_COLORSUM)&&(uComplex > 3456) && meStill && me1IsMv0 )
    {
        isL139 = true;
    }
    else
    {
        isL139 = false;
    }
    //printf("isL139== %d \n\n",isL139);
    return isL139;
}

MS_BOOL MDrv_SC_Agate_L153( const MS_BOOL movingWithoutFeather, const MS_U32 colorSum, const MS_U32 uComplex, const  XC_Me1HisStates H2 )
{
    MS_BOOL notXmoving = FALSE;
    MS_BOOL YmovingNotFocus = FALSE;
    MS_BOOL CplxRange = FALSE;
    MS_BOOL withoutColor = FALSE;
    MS_BOOL L153 = FALSE;

    if( colorSum < 5 )
        withoutColor = true;
    else
        withoutColor = false;

    if((H2.minMvXdir == 8) && (H2.minMvXBin > 10))
        notXmoving = true;
    else
        notXmoving = false;

    if( H2.minMvYBin < 5)
        YmovingNotFocus = true;
    else
        YmovingNotFocus = false;

    if(( uComplex > 0x200 ) && ( uComplex < 0x300 ))
        CplxRange = true;
    else
        CplxRange = false;

    if(notXmoving && YmovingNotFocus && movingWithoutFeather && withoutColor && CplxRange)
    {
        L153 = true;
        //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_36_L, 0x0000, 0x000F);
    }
    else
    {
        L153 = false;
        //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_36_L, 0x0008, 0x000F);
    }
    //printf("L153 == %d \n\n",L153);
    return L153;
}

MS_BOOL MDrv_SC_Agate_L107( const MS_U32 mvComfirm, const MS_U32 uComplex, const XC_GMvStates gMv, const MS_U32 u32MotionValue, const MS_U32 total_feather, const MS_U8 inputSource )
{
    MS_BOOL mvComfirmId = FALSE, colorId = FALSE, cplxId = FALSE, motionId = FALSE, featherId = FALSE;
    MS_BOOL notFoundMvId = FALSE, foundMvId = FALSE, foundNonMv0Id = FALSE;
    MS_BOOL gMvId = FALSE;
    MS_BOOL L107 = FALSE;

    MS_U32 notFoundMv = 0;
    static MS_U32 notFoundMv_iir = 0;
    MS_U32 foundMv0 = 0;
    static MS_U32 foundMv0_iir = 0;
    MS_U32 foundNonMv0 = 0;
    static MS_U32 foundNonMv0_iir = 0;
    MS_U32 colorSum = 0;
    static MS_U32 colorSum_iir = 0;

    foundMv0 = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_48_L,0xFFFF);
    foundMv0 = ((foundMv0_iir*3 )+ foundMv0 ) >> 2;
    foundMv0_iir = foundMv0;

    foundNonMv0 = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_49_L,0xFFFF);
    foundNonMv0 = ((foundNonMv0_iir*3 )+ foundNonMv0 ) >> 2;
    foundNonMv0_iir = foundNonMv0;

    notFoundMv = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_4A_L,0xFFFF);
    notFoundMv = ((notFoundMv_iir*3 )+ notFoundMv ) >> 2;
    notFoundMv_iir = notFoundMv;

    colorSum = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK55_1E_L,0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_1F_L, 0x000F )<<16); //move to UFSC in Maserati
    colorSum = ((colorSum_iir*3 )+ colorSum ) >> 2;
    colorSum_iir = colorSum;

    motionId = (u32MotionValue > 0x2C00) ? true : false;
    mvComfirmId = ( mvComfirm < 16 ) ? true : false;
    if( inputSource == 1 ) /*CVBS*/
        colorId = ( (colorSum > 0x2300) && (colorSum < 0x2B00) ) ? true : false;
    else if( inputSource == 2 )
        colorId = ( (colorSum > 0x1700) && (colorSum < 0x1B00) ) ? true : false;
    else
        colorId = false;

    cplxId = ( (uComplex > 0x200) && (uComplex < 0x600) ) ? true : false;
    featherId = ( (total_feather < 0x40) && (total_feather > 0x20) ) ? true : false;

    foundMvId =( (foundMv0_iir < 0x90) && (foundMv0_iir > 0x50) ) ? true : false;
    foundNonMv0Id =( (foundNonMv0_iir < 0x20) && (foundNonMv0_iir > 0x01) ) ? true : false;
    notFoundMvId = ( notFoundMv_iir < 0x2F ) ? true : false;

    if( ( gMv.h1XYsum > 10) && ( gMv.h2XYsum > 10)&&( gMv.h1XYdiff < 5)
        && ( gMv.h2XYdiff < 5) &&( gMv.minMvXdir == 0x20) && (gMv.minMvYdir == 8))
    {
        gMvId = true;
    }
    else
    {
        gMvId = false;
    }

    if( mvComfirmId && colorId && cplxId && gMvId && motionId && featherId
        &&foundMvId && foundNonMv0Id && notFoundMvId )
    {
        L107 = true;
    }
    else
    {
        L107 = false;
    }
    //printf("isL107 == %d\n\n",L107);
    return L107;
}

#define VerticalMovingUdRatio_MaxValue 0x0F
#define VerticalMovingUdRatio_MinValue 0x07
void MDrv_SC_KFC_VerticalMovingUdRatio_Ctrl(const MS_U32 inSearchUseFeather, MS_BOOL isfilmLike )
{
    static MS_U16 u16VerticalMovingUdRatioValue = VerticalMovingUdRatio_MaxValue;

    if( inSearchUseFeather > 0x300 )
    {
        if ( u16VerticalMovingUdRatioValue > VerticalMovingUdRatio_MinValue)
            u16VerticalMovingUdRatioValue--;
        else
            u16VerticalMovingUdRatioValue = VerticalMovingUdRatio_MinValue;
    }
    else
    {
        if ( u16VerticalMovingUdRatioValue < VerticalMovingUdRatio_MaxValue)
            u16VerticalMovingUdRatioValue++;
        else
            u16VerticalMovingUdRatioValue = VerticalMovingUdRatio_MaxValue;
    }

    if ( isfilmLike )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_28_L, 0, 0x000F);
    else
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_28_L, (MS_U16)u16VerticalMovingUdRatioValue, 0x000F);

}

#define KFC_8bitMotionGain_MaxValue 0x08
#define KFC_8bitMotionGain_MinValue 0x04 //0 have feather issue.
void MDrv_SC_KFC_8bitMotionGain_Ctrl(const MS_BOOL isZoomIn, const MS_BOOL isFilmLike, const MS_BOOL McSmallMoving )
{
    static MS_U16 u16KFC8bitMotionGainValue = KFC_8bitMotionGain_MaxValue;

    if( isFilmLike || McSmallMoving )
    {
        if ( u16KFC8bitMotionGainValue < 0x0F/*Max*/ )
            u16KFC8bitMotionGainValue++;
        else
            u16KFC8bitMotionGainValue = 0x0F;
    }
    else if( isZoomIn  )
    {
        //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK22_7A_L, 0x1000, 0x1000);
        if ( u16KFC8bitMotionGainValue > KFC_8bitMotionGain_MinValue)
            u16KFC8bitMotionGainValue--;
        else
            u16KFC8bitMotionGainValue = KFC_8bitMotionGain_MinValue;
    }
    else
    {
        //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK22_7A_L, 0x0000, 0x1000);
        if ( u16KFC8bitMotionGainValue < KFC_8bitMotionGain_MaxValue)
            u16KFC8bitMotionGainValue++;
        else
            u16KFC8bitMotionGainValue = KFC_8bitMotionGain_MaxValue;
    }

    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_3B_L, (MS_U16)u16KFC8bitMotionGainValue<<4, 0x00F0);
}

#define FeatherSstGain_Max 0x0F
#define FeatherSstGain_Min  0x02
void MDrv_SC_featherSstGain_Ctrl( const MS_BOOL  isMcFilmSlowMotion )
{
    static MS_U8 reg_featherSstGain =  FeatherSstGain_Min;

    if( isMcFilmSlowMotion )
    {
        if( reg_featherSstGain < FeatherSstGain_Max )
            reg_featherSstGain++;
    }
    else
    {
        if( reg_featherSstGain > FeatherSstGain_Min )
            reg_featherSstGain--;
    }

    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_29_L, (MS_U16)(reg_featherSstGain << 12 ), 0xF000);
}

#define FeatherSstOffset_Max 0x01 /* default */
#define FeatherSstOffset_Min  0x00
void MDrv_SC_featherSstOffset_Ctrl( const XC_OpFrameStates opFrameStatusCur, const MS_BOOL  isMcFilmSlowMotion, const MS_BOOL favorStill )
{
    static MS_U8 reg_featherSstOffset =  FeatherSstOffset_Max;
    static XC_OpFrameStates opFrameStatusLast;
    static MS_U8 frameCnt = 0;

    if( opFrameStatusCur.curOpFrame != opFrameStatusLast.curOpFrame )
    {
        if( frameCnt < 16 )
            frameCnt++;
        else
            frameCnt = 0;
    }

    if( isMcFilmSlowMotion )
    {
        if( reg_featherSstOffset > FeatherSstOffset_Min )
            reg_featherSstOffset--;
    }
    else if( favorStill )
    {
        if( frameCnt == 15 )
        {
            if( reg_featherSstOffset < 0x0F/* max of offset value */ )
                reg_featherSstOffset++;
        }
    }
    else
    {
        if( reg_featherSstOffset < FeatherSstOffset_Max )
            reg_featherSstOffset++;
        else if( reg_featherSstOffset > FeatherSstOffset_Max )
            reg_featherSstOffset--;
    }

    //PRT_TRP("==reg_featherSstOffset=%x\n",reg_featherSstOffset);

    opFrameStatusLast.curOpFrame = opFrameStatusCur.curOpFrame;

        //Dummy to control write value, add by Bella 20150708
        if((MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK23_5C_L,0x0F00))!=0x0000)
        {
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_29_L, MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK23_5C_L,0xFFFF), 0x0F00);
         }
        else
         {
           MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_29_L, (MS_U16)(reg_featherSstOffset << 8 ), 0x0F00);
         }
}

void MDrv_SC_MotionLimitGain_Ctrl( const MS_BOOL isL153 )
{
    if( isL153 )
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_36_L, 0x0000, 0x000F);
    }
    else
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_36_L, 0x0008, 0x000F);
    }
}

void MDrv_SC_UCDi_Weight_Boundary_Ctrl( const XC_Me1HisStates H1)
{
    MS_BOOL isHorizontalscroll =  ( H1.minMvXdir != 8 ) ? true : false;
    if( isHorizontalscroll )
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK22_77_L, 0x1807, 0xFFFF);
    }
    else
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK22_77_L, 0x0000, 0xFFFF);
    }
}

void MDrv_SC_MCDi_En_Ctrl( const MS_BOOL disable )
{
    if( disable )
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_02_L, 0x0000, 0x0080); // disable MCDi
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_04_L, 0x0000, 0x0002); // disable MCDi_KFC
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK22_10_L, 0x0080, 0x0080); // ensable MADi_DFK
        //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK22_18_L, 0x0080, 0x0080); // Enable MADi_SST
    }
    else
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_02_L, 0x0080, 0x0080);// enable MCDi
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_04_L, 0x0002, 0x0002); // disable MCDi_KFC
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK22_10_L, 0x0000, 0x0080); // disable MADi_DFK
        //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK22_18_L, 0x0000, 0x0080); // disable MADi_SST
    }
}

void MDrv_SC_Force_Motion_Ctrl( const MS_BOOL isL139, const MS_BOOL isUkosabihsMotion )
{
    MS_U16 reg_force_motion;
    MS_U32 mv0_feather, nonMv0_feather;
    MS_U32 total_feather;

    mv0_feather      = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_77_L,0xFFFF);
    nonMv0_feather   = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_7B_L,0xFFFF);

    total_feather =  mv0_feather + nonMv0_feather;

    if( (isUkosabihsMotion)&&(total_feather< 0x1200))
    {
        reg_force_motion = 0x008F; /* full motion */
    }
    else if( isL139 )
    {
        reg_force_motion = 0x0080; /* full still */
    }
    else
    {
        reg_force_motion = 0x0000;
    }

    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK22_78_L, reg_force_motion, 0x00FF);
}

void MDrv_SC_Force_Mcdi_Ctrl( const MS_BOOL isUkosabihsMc)
{
    MS_U16 reg_force_mcdi = 0;

    if( isUkosabihsMc )
        reg_force_mcdi = 0x8080;
    else
        reg_force_mcdi = 0x0000;

    MApi_GFLIP_XC_W2BYTEMSK( REG_SC_BK2A_2F_L, reg_force_mcdi , 0xFFFF );
}

void MDrv_SC_KFC_EODiW_Ctrl( const MS_BOOL isUkosabihsMc )
{
    MS_U16 reg = 0;

    if( isUkosabihsMc )
        reg = 0x0000; /* disable kfcEodiW  */
    else
        reg = 0x00C0; /* enable kfcEodiW  */

    MApi_GFLIP_XC_W2BYTEMSK( REG_SC_BK22_28_L, reg, 0x00C0 );
}

void MDrv_SC_Mcdi_EODiW_Ctrl( const MS_BOOL isUkosabihsMc, const MS_BOOL isL107 )
{
    MS_U16 reg = 0;

    if( isUkosabihsMc || isL107 )
    {
        reg = 0x0000; /* disable mcEodiW */
        //printf("MDrv_SC_Mcdi_EODiW_Ctrl ==0\n\n");
    }
    else
    {
        reg = 0x0080; /* enable mcEodiW */
        //printf("MDrv_SC_Mcdi_EODiW_Ctrl ==80\n\n");
    }

    MApi_GFLIP_XC_W2BYTEMSK( REG_SC_BK22_28_L, reg, 0x0080 );
}

#define reg_mcdi_feather_gain_MaxValue 0x02
#define reg_mcdi_feather_gain_MinValue 0x00
void MDrv_SC_KFC_Ctrl( const MS_BOOL isAbihsot )
{
    static MS_U16 u16regmcdifeathergain = reg_mcdi_feather_gain_MaxValue;

    if( isAbihsot )
    {
        if(u16regmcdifeathergain > reg_mcdi_feather_gain_MinValue)
            u16regmcdifeathergain--;
        else
            u16regmcdifeathergain = reg_mcdi_feather_gain_MinValue;
    }
    else
    {
        if(u16regmcdifeathergain < reg_mcdi_feather_gain_MaxValue)
            u16regmcdifeathergain++;
        else
            u16regmcdifeathergain = reg_mcdi_feather_gain_MaxValue;
    }

        //Dummy to control write value, add by Bella 20150708
        if((MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK23_5D_L,0x000F))!=0x0000)
        {
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_2D_L, MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK23_5D_L,0xFFFF), 0x0F00);
         }
        else
         {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_2D_L, (MS_U16)u16regmcdifeathergain << 8, 0x0F00);
         }
}

#define UCDi_OP1_HisWeight_MaxValue 0x0E    // history weight = 6
#define UCDi_OP1_HisWeight_MinValue 0x04    // history weight = 2
void MDrv_SC_UCDi_OP1_HisWeight_Ctrl(const MS_BOOL Bouncing_status,const MS_BOOL Feather_sum_status,const XC_FilmStates filmstatus, const MS_U8 colorSum, const MS_U32 FeatherSum, const MS_BOOL isHDSrc)
{
    static MS_U32 reg_uc_his_weight = UCDi_OP1_HisWeight_MinValue;
    static MS_U32 mv0_feather_prv = 0x0000;
    MS_U32 mv0_feather;
    MS_U8 u8MotionValue;

    u8MotionValue = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK0A_1A_L,0x007F);

    mv0_feather = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_77_L,0xFFFF);
    //Add iir filter
    mv0_feather = (mv0_feather_prv*3 + mv0_feather)/4;
    mv0_feather_prv = mv0_feather;

    if((u8MotionValue<0x0A) && (SZ_PATTERN_ENABLE ==1))//Add for AV patterns, titles are shaking issue
    {
        reg_uc_his_weight = UCDi_OP1_HisWeight_MinValue;
        //printf("u8MotionValue = %d, SZ_PATTERN_ENABLE = %d\n\n",u8MotionValue, SZ_PATTERN_ENABLE);
    }
    else if(((FeatherSum > 0x1200)&&(mv0_feather>0x1100)&&(!isHDSrc))&& (SZ_PATTERN_ENABLE ==1)) //Add feathersum for TSB#3 capter4 white noise
    {
        reg_uc_his_weight = UCDi_OP1_HisWeight_MinValue;
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_50_L, 0x0000, 0x0002);
        //printf("FeatherSum = %d, mv0_feather = %d\n\n",(int)FeatherSum, (int)mv0_feather);
    }
    else if( Bouncing_status || Feather_sum_status )
    {
        reg_uc_his_weight = UCDi_OP1_HisWeight_MinValue;
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_50_L, 0x0002, 0x0002);
        //printf("Bouncing_status = %d, Feather_sum_status = %d\n\n",Bouncing_status, Feather_sum_status);
    }
#if SEC_HQV_PATTERN
    else if ( checkHQVMovingBarPatch( colorSum ) ) // special patch for SEC : HQV 2.0 moving white bar, need to reduce history weight
    {
        if( reg_uc_his_weight > 0x8 )
            reg_uc_his_weight--;
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_50_L, 0x0002, 0x0002);
        //printf("reg_uc_his_weight = %d\n\n",(int)reg_uc_his_weight);
    }
#endif
    else
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_50_L, 0x0002, 0x0002);
        if( reg_uc_his_weight < UCDi_OP1_HisWeight_MaxValue )
            reg_uc_his_weight++;
        else
            reg_uc_his_weight = UCDi_OP1_HisWeight_MaxValue;
        //printf("else reg_uc_his_weight= %d\n\n",(int)reg_uc_his_weight);
    }

    if(filmstatus.film_act)
    {
        reg_uc_his_weight = UCDi_OP1_HisWeight_MaxValue;
    }

        //Dummy to control write value, add by Bella 20150708
        if((MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK23_5C_L,0x000E))!=0x0000)
        {
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_02_L, MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK23_5C_L,0xFFFF), 0x000E);
        }
        else
        {
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_02_L, reg_uc_his_weight, 0x000E);
        }

}

#define SPF_OP1_HisWeight_MaxValue 0x06    // history weight = 6
#define SPF_OP1_HisWeight_MinValue 0x02    // history weight = 2
void MDrv_SC_SPF_OP1_HisWeight_Ctrl(const MS_BOOL Bouncing_status)
{
    static MS_U32 reg_uc_his_weight = SPF_OP1_HisWeight_MinValue;

    if( Bouncing_status  )
    {
        reg_uc_his_weight = SPF_OP1_HisWeight_MinValue;
#ifdef DBG_HISW8T_LC
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_46_L, 0x0007, 0x000F );
#endif
    }
    else
    {
        if( reg_uc_his_weight < SPF_OP1_HisWeight_MaxValue )
            reg_uc_his_weight++;
        else
            reg_uc_his_weight = SPF_OP1_HisWeight_MaxValue;
#ifdef DBG_HISW8T_LC
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_46_L, 0x0000, 0x000F );
#endif
    }

    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK09_5C_L, reg_uc_his_weight, 0x0007);

}

void MDrv_SC_SST_Ctrl( const MS_BOOL disable, const MS_U32 FeatherSum, const MS_BOOL isHDSrc )
{
    static MS_BOOL reg_sst_en = true;
    MS_U32 mv0_feather;
    mv0_feather = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_77_L,0xFFFF);
    if( disable ||((FeatherSum > 0x1200)&&(mv0_feather>0x1100)&&(!isHDSrc)&&(SZ_PATTERN_ENABLE ==1))) //Add feathersum for TSB#3 capter4 white noise
    {
        reg_sst_en = false;
    }
    else
    {
        reg_sst_en = true;
    }

    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_04_L, (MS_U16)(reg_sst_en << 8), 0x0100);

}

/******************************************************************************/
///Noise Estimation Driver
/******************************************************************************/
MS_U8 MDrv_SC_GetNoiseLevel( MS_U8 *estNoise, const XC_LetterBoxState *letterBoxState, const XC_ApiStatus *stXCStatus, const MS_BOOL bInterlace, const MS_BOOL isOpFrameChanged )
{
    const int defaultnoise = 35;
    static int noiseIIR = 35;
    static int noisehistory[5] = {0,0,0,0,0};
    static MS_BOOL long_noisehistory[11] = {0,0,0,0,0,0,0,0,0,0,0};
    static int safenoise = 0;
    static int resetcount = 0;
    MS_U8 reg_noise_confirm_level = 2;
    static MS_BOOL is_ne_set = false;

    int avgpixeldiff = defaultnoise;
    int NE_histogram[25];
    int MV0count = 0;

    if( isOpFrameChanged )
    {
        MS_U16 height = bInterlace ? stXCStatus->ScaledCropWin.height/2 : stXCStatus->ScaledCropWin.height;
        MS_U16 width = stXCStatus->ScaledCropWin.width;

        MS_U16 hSt = (letterBoxState->lbCntL + 49 )/ 10;
        MS_U16 hEnd = ( max( width - letterBoxState->lbCntR - 50, 0 ) ) / 10;

        MS_U16 vSt = (letterBoxState->lbCntT + 29 )/ 6;
        MS_U16 vEnd = ( max( height - letterBoxState->lbCntB - 30, 0 ) ) / 6;

        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK39_0E_L, (hEnd << 8) | hSt ,0xFFFF);
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK39_0F_L, (vEnd << 8) | vSt ,0xFFFF);

        MS_U16 total_block_num=max((vEnd+1)/4-(vSt+3)/4,0)*max((hEnd+1)/4-(hSt+3)/4,0);
        
        if( !is_ne_set )
        {
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK39_08_L, 0x0001, 0x0001);
            is_ne_set = true;

            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK39_0A_L, 0x0000, 0x0100);
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK39_0D_L, 0x0001, 0x0001); // active window en
        }
        else if (MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_08_L, 0x0080)!=0)//ready
        {
            NE_histogram[0]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_10_L,0x07FF);
            NE_histogram[1]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_11_L,0x07FF);
            NE_histogram[2]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_12_L,0x07FF);
            NE_histogram[3]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_13_L,0x07FF);
            NE_histogram[4]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_14_L,0x07FF);
            NE_histogram[5]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_15_L,0x07FF);
            NE_histogram[6]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_16_L,0x07FF);
            NE_histogram[7]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_17_L,0x07FF);
            NE_histogram[8]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_18_L,0x07FF);
            NE_histogram[9]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_19_L,0x07FF);
            NE_histogram[10]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_1A_L,0x07FF);
            NE_histogram[11]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_1B_L,0x07FF);
            NE_histogram[12]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_1C_L,0x07FF);
            NE_histogram[13]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_1D_L,0x07FF);
            NE_histogram[14]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_1E_L,0x07FF);
            NE_histogram[15]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_1F_L,0x07FF);
            NE_histogram[16]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_20_L,0x07FF);
            NE_histogram[17]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_21_L,0x07FF);
            NE_histogram[18]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_22_L,0x07FF);
            NE_histogram[19]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_23_L,0x07FF);
            NE_histogram[20]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_24_L,0x07FF);
            NE_histogram[21]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_25_L,0x07FF);
            NE_histogram[22]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_26_L,0x07FF);
            NE_histogram[23]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_27_L,0x07FF);
            NE_histogram[24]=MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK39_28_L,0x07FF);

            int i;
            for (i = 4; i > 0; i-- )
                noisehistory[i]=noisehistory[i-1];
            for (i = 10; i > 0; i-- )
                long_noisehistory[i]=long_noisehistory[i-1];
            for (i=0;i<25;i++)
                MV0count+=NE_histogram[i];
        
            if (MV0count<total_block_num/12 || total_block_num<24)
            {
                if (safenoise>0)
                {
                    avgpixeldiff=safenoise;
                    resetcount++;
                    reg_noise_confirm_level=8;
                }
                else
                {
                    avgpixeldiff=noiseIIR;
                    reg_noise_confirm_level=0;
                }
                noisehistory[0]=0;
                long_noisehistory[0]=0;
            }
            else
            {
                int maxhis=1;
                int maxval=0;
                for (i=0;i<24;i++)
                {
                    int curva;
                    if (i==0)
                        curva=2*NE_histogram[i]+NE_histogram[i+1];
                    else if (i==23)
                        curva=2*NE_histogram[i]+NE_histogram[i-1];
                    else
                        curva=NE_histogram[i-1]+2*NE_histogram[i]+NE_histogram[i+1];

                    if (curva>maxval)
                    {
                        maxval=curva;
                        maxhis=i;
                    }
                }
                int thre1=4;
                int thre2=13;
                if (maxhis>12)
                {
                    thre1=3;
                    thre2=10;
                }
                if ((maxval>total_block_num/10 && (maxval-NE_histogram[maxhis])*10/MV0count>=thre1 && NE_histogram[maxhis] * 100 / MV0count>thre2))// || MV0count<total_block_num/24)
                {
                    noisehistory[0]=10*maxhis+5;
                    long_noisehistory[0]=1;
                    int nonzerocount=0;
                    int historytotal=0;
                    int firstnoise=0;
                    int long_nonzerocount=0;
                    for ( i=0;i<5;i++)
                    {
                        historytotal=historytotal+noisehistory[i];
                        if (noisehistory[i]>0)
                        {
                            nonzerocount++;
                            firstnoise=noisehistory[i];
                        }
                    }
                    for ( i=0;i<11;i++)
                        long_nonzerocount+=long_noisehistory[i];
                    if (nonzerocount>=3 && (maxhis<=3 || long_nonzerocount>=7))
                    {
                        int removefirstavg=(historytotal-firstnoise)/(nonzerocount-1);
                        if ((removefirstavg+20)<firstnoise)
                            safenoise=removefirstavg;
                        else
                            safenoise=historytotal/nonzerocount;
                        resetcount=0;
                        avgpixeldiff=noisehistory[0];
                        reg_noise_confirm_level=8;
                    }
                    else if (safenoise>0)
                    {
                        avgpixeldiff=safenoise;
                        resetcount++;
                        reg_noise_confirm_level=8;
                    }
                    else
                    {
                        avgpixeldiff=noisehistory[0];
                        reg_noise_confirm_level=1;
                    }
                }
                else
                {
                    noisehistory[0]=0;
                    long_noisehistory[0]=0;
                    if (safenoise>0)
                    {
                        avgpixeldiff=safenoise;
                        resetcount++;
                        reg_noise_confirm_level=8;
                    }
                    else
                    {
                        avgpixeldiff=10*maxhis+5;
                        reg_noise_confirm_level=1;
                    }
                }
            }
            if (resetcount>8)
            {
                safenoise=0;
                resetcount=0;
            }
            int tmpcount=0;
            int prenoise=0;
            for ( i = 0; i < 5; i++ )
            {
                if (noisehistory[i]<250 && noisehistory[i]>0)
                {
                    tmpcount++;
                    prenoise=prenoise+noisehistory[i];
                }
            }
            if (tmpcount>0)
                prenoise=prenoise/tmpcount;
            int reg_ne_mv0cost_th =2*prenoise/10;
            reg_ne_mv0cost_th = reg_ne_mv0cost_th > 3 ? reg_ne_mv0cost_th : 3;
            if (tmpcount==0)
                reg_ne_mv0cost_th =60;
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK39_0A_L, reg_ne_mv0cost_th, 0x003F);
            
            
            if (reg_noise_confirm_level==8)
                noiseIIR = (noiseIIR*3+avgpixeldiff) >> 2;

            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK39_08_L, 0x0001, 0x0001);
            is_ne_set = true;
        }
    }

    *estNoise = (MS_U8)( noiseIIR <= 255 ? noiseIIR : 255 );

    return reg_noise_confirm_level;
}

/******************************************************************************/
///UCNR Driver
/******************************************************************************/
MS_BOOL Hal_PQ_LoadMCNRLutQmapSetting()
{
    //UCNR LUT
#if MCNR_LUT_LOAD_FROM_QMAP
    m_stMCNRLutQmapSetting.mcnrLut[0] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_08_L,0xFFFF);
    m_stMCNRLutQmapSetting.mcnrLut[1] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_09_L,0xFFFF);
    m_stMCNRLutQmapSetting.mcnrLut[2] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_0A_L,0xFFFF);
    m_stMCNRLutQmapSetting.mcnrLut[3] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_0B_L,0xFFFF);
    m_stMCNRLutQmapSetting.mcnrLut[4] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_0C_L,0xFFFF);
    m_stMCNRLutQmapSetting.mcnrLut[5] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_0D_L,0xFFFF);
    m_stMCNRLutQmapSetting.mcnrLut[6] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_0E_L,0xFFFF);
    m_stMCNRLutQmapSetting.mcnrLut[7] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_0F_L,0xFFFF);
#else
    m_stMCNRLutQmapSetting.mcnrLut[0] = 0xFFEE;
    m_stMCNRLutQmapSetting.mcnrLut[1] = 0xDDCC;
    m_stMCNRLutQmapSetting.mcnrLut[2] = 0xBBAA;
    m_stMCNRLutQmapSetting.mcnrLut[3] = 0x9988;
    m_stMCNRLutQmapSetting.mcnrLut[4] = 0x7766;
    m_stMCNRLutQmapSetting.mcnrLut[5] = 0x5544;
    m_stMCNRLutQmapSetting.mcnrLut[6] = 0x3322;
    m_stMCNRLutQmapSetting.mcnrLut[7] = 0x1100;
#endif

    //PDNR LUT
#if PDNR_LUT_LOAD_FROM_QMAP
    m_stMCNRLutQmapSetting.pdnrlowLut[0] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_1C_L,0xFFFF);
    m_stMCNRLutQmapSetting.pdnrlowLut[1] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_1D_L,0xFFFF);
    m_stMCNRLutQmapSetting.pdnrlowLut[2] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_1E_L,0xFFFF);
    m_stMCNRLutQmapSetting.pdnrlowLut[3] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_1F_L,0xFFFF);
    m_stMCNRLutQmapSetting.pdnrlow2Lut[0] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_20_L,0xFFFF);
    m_stMCNRLutQmapSetting.pdnrlow2Lut[1] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_21_L,0xFFFF);
    m_stMCNRLutQmapSetting.pdnrlow2Lut[2] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_22_L,0xFFFF);
    m_stMCNRLutQmapSetting.pdnrlow2Lut[3] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_23_L,0xFFFF);
    m_stMCNRLutQmapSetting.pdnrhighLut[0] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_24_L,0xFFFF);
    m_stMCNRLutQmapSetting.pdnrhighLut[1] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_25_L,0xFFFF);
    m_stMCNRLutQmapSetting.pdnrhighLut[2] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_26_L,0xFFFF);
    m_stMCNRLutQmapSetting.pdnrhighLut[3] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_27_L,0xFFFF);
#else
    m_stMCNRLutQmapSetting.pdnrlowLut[0] = 0x8888;
    m_stMCNRLutQmapSetting.pdnrlowLut[1] = 0x8888;
    m_stMCNRLutQmapSetting.pdnrlowLut[2] = 0x7654;
    m_stMCNRLutQmapSetting.pdnrlowLut[3] = 0x3210;
    m_stMCNRLutQmapSetting.pdnrlow2Lut[0] = 0x2100;
    m_stMCNRLutQmapSetting.pdnrlow2Lut[1] = 0x0000;
    m_stMCNRLutQmapSetting.pdnrlow2Lut[2] = 0x0000;
    m_stMCNRLutQmapSetting.pdnrlow2Lut[3] = 0x0000;
    if(_bIsHDSource)
    {
        m_stMCNRLutQmapSetting.pdnrhighLut[0] = 0x8888;
        m_stMCNRLutQmapSetting.pdnrhighLut[1] = 0x8888;
        m_stMCNRLutQmapSetting.pdnrhighLut[2] = 0x8887;
        m_stMCNRLutQmapSetting.pdnrhighLut[3] = 0x6420;
    }
    else
    {
        m_stMCNRLutQmapSetting.pdnrhighLut[0] = 0x8876;
        m_stMCNRLutQmapSetting.pdnrhighLut[1] = 0x5432;
        m_stMCNRLutQmapSetting.pdnrhighLut[2] = 0x1000;
        m_stMCNRLutQmapSetting.pdnrhighLut[3] = 0x0000;
    }
#endif

    return true;
}

/******************************************************************************/
///Read ME value (F2 ME status)
///@return MS_U16: Motion value
/******************************************************************************/
MS_U16 MDrv_SC_read_avgMcnrMovingWeightMin(void)
{
    MS_U16 cur;
    static MS_U16 prv = 0;

    cur = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK09_68_L,0xFFFF);
    cur = ((prv*3 )+ cur ) >> 2;
    prv = cur;

    return prv;
}

MS_U16 MDrv_SC_read_avgMcnrMovingWeight(void)
{
    MS_U16 cur;
    static MS_U16 prv = 0;

    cur = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK09_6A_L,0xFFFF);
    cur = ((prv*3 )+ cur ) >> 2;
    prv = cur;

    return prv;
}

MS_U16 MDrv_SC_read_avgMv0MovingWeight(void)
{
    MS_U16 cur;
    static MS_U16 prv = 0;

    cur = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK09_69_L,0xFFFF);
    cur = ((prv*3 )+ cur ) >> 2;
    prv = cur;

    return prv;
}

MS_U16 MDrv_SC_read_me2SadDiff(void)
{
    MS_U16 cur;
    static MS_U16 prv = 0;

    cur = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK09_6C_L,0xFFFF);
    cur = ((prv*3 )+ cur ) >> 2;
    prv = cur;

    return prv;
}

MS_U8 MDrv_SC_McSmallMoving( const MS_U16 me13x3MwMin, const MS_U16 mv0Cost )
{
    static MS_U8 smallMovingCnt = 0;

    if( ( mv0Cost>>8 ) > me13x3MwMin )
    {
        if( smallMovingCnt < 255 )
            smallMovingCnt++;
    }
    else
    {
        if( smallMovingCnt > 0 )
            smallMovingCnt--;
    }

    return smallMovingCnt;
}

void DEBUG_LC_40( const MS_U8 a )
{
    if( (a > 0x00) && (a < 0x04) )
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0001 ,0xFFFF);
    }
    else if( (a > 0x03) && (a < 0x10) )
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0011 ,0xFFFF);
    }
    else if( (a > 0x0F) && (a < 0x40) )
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0111 ,0xFFFF);
    }
    else if( (a > 0x3F) /*&& (a < 0x100)*/ )
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x1111 ,0xFFFF);
    }
    else
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0000 ,0xFFFF);
    }
}

MS_U8 MDrv_SC_Motion4StillCnt( const MS_U16 motion4 )
{
    static MS_U8 motionCnt = 0;

    if( motion4 == 0 )
    {
     //motionCnt = motionCnt < 12 ? motionCnt += 3 : 15);

     if(motionCnt < 12)
     {
      motionCnt += 3;
     }
     else
     {
      motionCnt =15;
     }
    }
       else
       {
     //motionCnt = motionCnt > 3 ? motionCnt -= 3 : 0);
     if(motionCnt > 3 )
     {
       motionCnt -= 3;
    }
    else
    {
      motionCnt =0;
      }
    }

    return motionCnt;
}

void MDrv_SC_GMV_Stability( MS_U16 *gmvDiff, MS_U8 *gmvAvgX, MS_U8 *gmvAvgY, const XC_GMvStates *GMV, const MS_BOOL isOpFrameChanged )
{
    static XC_GMvStates gmvHist[8];
    static MS_U8 histPos = 0;
    static MS_U16 mvDiff = 0;
    static MS_U16 mvAvgX = 0, mvAvgY = 0;

    if( isOpFrameChanged )
    {
        gmvHist[histPos] = *GMV;
        histPos = histPos < 7 ? histPos+1 : 0;

        mvDiff = 0;
        mvAvgX = mvAvgY = 0;
        int i;
        for( i = 0 ; i < 8 ; ++i )
        {
            if( i != 0 )
                mvDiff += abs( gmvHist[i].minMvXdir - gmvHist[i-1].minMvXdir ) + abs( gmvHist[i].minMvYdir - gmvHist[i-1].minMvYdir );
            mvAvgY += gmvHist[i].minMvYdir;
            mvAvgX += gmvHist[i].minMvXdir;
        }
        mvAvgY = ( mvAvgY + 4 ) >> 3;
        mvAvgX = ( mvAvgX + 4 ) >> 3;
    }

    *gmvDiff = mvDiff;
    *gmvAvgX = mvAvgX;
    *gmvAvgY = mvAvgY;
}

MS_U8 MDrv_SC_NR_HouseMD_Patch(
    /*const MS_U8 gmvAvgX, const MS_U8 gmvAvgY, const int blkConfidence,*/ const MS_U16 me1NotFoundBlk, const XC_LumaHistogramStates *lumaHistogramStates,
    const MS_BOOL isSceneChange
)
{
    static MS_U8 u8LowLumaVerMovingCnt = 0;

    MS_U32 darkSum = 0, lowLumaSum = 0, avgLuma = 0, totalSum = 0;
    int i = 0;
    for( i = 0 ; i < 32 ; ++i )
    {
        if( i < 4 ) darkSum += lumaHistogramStates->lumaBin[i];
        if( i < 8 ) lowLumaSum += lumaHistogramStates->lumaBin[i];
        totalSum += lumaHistogramStates->lumaBin[i];
        avgLuma += lumaHistogramStates->lumaBin[i] * i;
    }
    if( totalSum != 0 )
    {
        avgLuma /= totalSum;
        darkSum = darkSum * 256 / totalSum;
        lowLumaSum = lowLumaSum * 256 / totalSum;
    }
    else
    {
        avgLuma = 0;
        darkSum = 0;
        lowLumaSum = 0;
    }

    MS_U16 u16TopCnt = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK0B_0D_L, 0x03FF );
    MS_U16 u16BottomCnt = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK0B_0E_L, 0x03FF );

    //PRT_TRP( "Luma dark, low, avg: %d, %d, %d\n", (int)darkSum, (int)lowLumaSum, (int)avgLuma );

    if(
        ( u8LowLumaVerMovingCnt <= 8 ? (darkSum > 170) : (darkSum > 160)  )
        && lowLumaSum > 230
        && avgLuma < 4
        /*&& blkConfidence > 70*/
        && me1NotFoundBlk > 0x300
        && ( u8LowLumaVerMovingCnt <= 8 ? (u16TopCnt > 0xFF || u16BottomCnt > 0xFF) : (u16TopCnt > 0xF || u16BottomCnt > 0xF) )
    )
    {
        u8LowLumaVerMovingCnt = u8LowLumaVerMovingCnt < 64 ? u8LowLumaVerMovingCnt+1 : 64;
    }
    else
    {
        u8LowLumaVerMovingCnt = u8LowLumaVerMovingCnt > 0 ? u8LowLumaVerMovingCnt-1 : 0;
    }

    if( isSceneChange )
    {
        u8LowLumaVerMovingCnt = 0;
    }

    //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_44_L, isVerMoving ? 0x0020 : 0x0010, 0x00F0 );

    return u8LowLumaVerMovingCnt;
}

MS_U16 MDrv_SC_NR_LUT_Aux( MS_U16 nr_lut, MS_U8 sc)
{
    sc = sc < 16 ? sc : 16;

    MS_U8 h0 = (nr_lut >>  0) & 0xF;
    MS_U8 h1 = (nr_lut >>  4) & 0xF;
    MS_U8 h2 = (nr_lut >>  8) & 0xF;
    MS_U8 h3 = (nr_lut >> 12) & 0xF;

    XC_ApiStatus DrvStatus;
    MApi_XC_GetStatus(&DrvStatus, MAIN_WINDOW);
    if ((!DrvStatus.bInterlace) && (DrvStatus.u16InputVFreq < 300)) //reduce gain value if 24p
    {
        h0 = ((h0 * sc)/3) >> 4;
        h1 = ((h1 * sc)/3) >> 4;
        h2 = ((h2 * sc)/3) >> 4;
        h3 = ((h3 * sc)/3) >> 4;
    }
    else
    {
        h0 = (h0 * sc) >> 4;
        h1 = (h1 * sc) >> 4;
        h2 = (h2 * sc) >> 4;
        h3 = (h3 * sc) >> 4;
    }

    return (h3 << 12) | (h2 << 8) | (h1 << 4) | (h0 << 0);
}

void MDrv_SC_Set_MCNR_Gain( MS_U8 gain, MS_BOOL isSourceChange )
{
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_08_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.mcnrLut[0], gain ) ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_09_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.mcnrLut[1], gain ) ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_0A_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.mcnrLut[2], gain ) ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_0B_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.mcnrLut[3], gain ) ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_0C_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.mcnrLut[4], gain ) ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_0D_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.mcnrLut[5], gain ) ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_0E_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.mcnrLut[6], gain ) ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_0F_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.mcnrLut[7], gain ) ,0xFFFF);
}

void MDrv_SC_Set_PDNR_Gain( MS_U8 gain, MS_BOOL isSourceChange )
{
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_1C_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.pdnrlowLut[0], gain ) ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_1D_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.pdnrlowLut[1], gain ) ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_1E_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.pdnrlowLut[2], gain ) ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_1F_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.pdnrlowLut[3], gain ) ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_20_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.pdnrlow2Lut[0], gain ) ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_21_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.pdnrlow2Lut[1], gain ) ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_22_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.pdnrlow2Lut[2], gain ) ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_23_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.pdnrlow2Lut[3], gain ) ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_24_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.pdnrhighLut[0], gain ) ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_25_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.pdnrhighLut[1], gain ) ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_26_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.pdnrhighLut[2], gain ) ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_27_L, MDrv_SC_NR_LUT_Aux( m_stMCNRLutQmapSetting.pdnrhighLut[3], gain ) ,0xFFFF);
}

void MDrv_SC_Set_PostDINR_Gain( MS_U8 gain, MS_BOOL isSourceChange )
{
    static MS_U16 lut[4];
    if( isSourceChange )
    {
        lut[0] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK61_08_L,0xFFFF);
        lut[1] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK61_09_L,0xFFFF);
        lut[2] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK61_0A_L,0xFFFF);
        lut[3] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK61_0B_L,0xFFFF);
    }
    else
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK61_08_L, MDrv_SC_NR_LUT_Aux( lut[0], gain ) ,0xFFFF);
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK61_09_L, MDrv_SC_NR_LUT_Aux( lut[1], gain ) ,0xFFFF);
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK61_0A_L, MDrv_SC_NR_LUT_Aux( lut[2], gain ) ,0xFFFF);
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK61_0B_L, MDrv_SC_NR_LUT_Aux( lut[3], gain ) ,0xFFFF);
    }
}

MS_U16 MDrv_SC_Luma_PK_LUT_Aux( const MS_U8 table[], MS_U8 sc)
{
    sc = sc < 16 ? sc : 16;

    MS_U8 h0 = ( 8 * (16-sc) + table[0] * sc + 8) >> 4;
    MS_U8 h1 = ( 8 * (16-sc) + table[1] * sc + 8) >> 4;
    MS_U8 h2 = ( 8 * (16-sc) + table[2] * sc + 8) >> 4;
    MS_U8 h3 = ( 8 * (16-sc) + table[3] * sc + 8) >> 4;

    return (h0 << 12) | (h1 << 8) | (h2 << 4) | (h3 << 0);
}

void MDrv_SC_Set_Luma_PK_Gain( MS_U8 gain )
{
    const MS_U8 tableL[4] = { 2, 2, 8, 8 };
    const MS_U8 tableH[4] = { 10, 8, 8, 6 };

    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK19_55_L, 0x21 ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK19_56_L, MDrv_SC_Luma_PK_LUT_Aux( tableL, gain ) ,0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK19_57_L, MDrv_SC_Luma_PK_LUT_Aux( tableH, gain ) ,0xFFFF);
}



#define NR_DIRECT_OVERWRITE_EN 0
void MDrv_SC_NR_Driver_E28
(
    const MS_U16 u16MotionValue4,
/*  const int blkConfidence,*/
    //const MS_U32 hpfHdTolerantReport,
    const XC_LumaHistogramStates *lumaHistogramStates,
    const XC_FilmStates *filmstatus,
    //const XC_LetterBoxState *letterBoxState,
    //const XC_ApiStatus *stXCStatus,
    const XC_GMvStates *GMV,
    const MS_BOOL isUkosabihsMotion,
    const MS_BOOL isSceneChangeExt,
    const MS_BOOL bInterlace,
    const MS_BOOL isHDSrc,
    const MS_BOOL isSourceChange,
    const MS_BOOL isOpFrameChanged,
    const MS_U8 *estNoise,
    const MS_U8 NE_confirm
)
{
    // Update MCNR/PDNR LUT from Qmap
    MS_BOOL bMCNR_LUT_Reload = (MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_0C_L, 0x0400)>>8);
    if(bMCNR_LUT_Reload)
    {
        if(Hal_PQ_LoadMCNRLutQmapSetting())
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_0C_L, 0x0000, 0x0400);
    }

    // debug en
    MS_BOOL dbgEn = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_38_L, 0x0001 ) & (MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_39_L, 0x0001 ));
    if( dbgEn ) MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_3A_L, 0x0003, 0x0003 );

    static MS_BOOL prevIsSceneChangeExt = 0;
    MS_BOOL isSceneChange = 0;
    if( (!prevIsSceneChangeExt) && isSceneChangeExt )
        isSceneChange = 1;

    prevIsSceneChangeExt = isSceneChangeExt;

    static MS_U8 mcnrMotionCnt = 0;
    static MS_U8 noiseLevel = 0;
    //static MS_U8 smvFrameCnt = 0;
    static MS_U8 panScanCnt = 0;

    /* Get UCNR Status */
    MS_U16 me1MovingWeight3x3Min = MDrv_SC_read_avgMcnrMovingWeightMin();
    //MS_U16 me1MovingWeight3x3Max = MDrv_SC_read_avgMcnrMovingWeightMax();
    MS_U16 me1MovingWeight = MDrv_SC_read_avgMcnrMovingWeight();
    MS_U16 me1Mv0Cost = MDrv_SC_read_avgMv0MovingWeight();
    MS_U16 me2SadDiff = MDrv_SC_read_me2SadDiff();

    MS_U16 mcnrWPixCnt = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK09_70_L,0xFFFF);
    //MS_U16 mcnr2WPixCnt = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK09_71_L,0xFFFF);
    MS_U16 pdnrWPixCnt = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK09_72_L,0xFFFF);

    MS_U8 smallMovingW = MDrv_SC_McSmallMoving( me1MovingWeight3x3Min, me1Mv0Cost );
    if (dbgEn)
    DEBUG_LC_40( smallMovingW );

    MS_U8 motion4StillCnt = MDrv_SC_Motion4StillCnt( u16MotionValue4 ); // 0 ~ 15
    //MS_U8 stillW = MDrv_SC_Me1Still( me1MovingWeight3x3Max, me1Mv0Cost );

    /*1D motion status*/
    MS_U16 mv0pixels  = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK09_78_L,0xFFFF);
    MS_U16 nonMv0Pixels = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK09_79_L,0xFFFF);
    MS_U16 mvNotFoundPixels = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK09_7A_L,0xFFFF);

    /*2D motion status*/
    MS_U16 me1Mv0Blk = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_48_L,0xFFFF);
    MS_U16 me1NonMv0Blk = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_49_L,0xFFFF);
    MS_U16 me1NotFoundBlk = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_4A_L,0xFFFF);
    MS_U16 me2Mv0Blk = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_4B_L,0xFFFF);
    MS_U16 me2NonMv0Blk = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_4C_L,0xFFFF);
    MS_U16 me2NotFoundBlk = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK17_4D_L,0xFFFF);

    static MS_BOOL lastEN = 0;


    if( isSceneChange || isSourceChange )
        noiseLevel = 0;
    else
    {
        //NE_confirm = MDrv_SC_GetNoiseLevel( &estNoise, letterBoxState, stXCStatus, bInterlace, isOpFrameChanged );
        if (NE_confirm==8)
            lastEN=1;
        else if (NE_confirm==0 || NE_confirm==1)
            lastEN=0;
        if( NE_confirm == 8 || (NE_confirm==2 && lastEN==1))
        noiseLevel = *estNoise;
        else
        noiseLevel = 0;
    }
    //printf("noise level : %d, %d\n", noiseLevel, NE_confirm);
    //printf("noise opframe change : %d\n", (int)isOpFrameChanged);

    // NE adaptive debug en
    MS_BOOL neAdptDbgEn = /*(MApi_XC_R2BYTE(REG_SC_BK2A_51_L) == 0x0100) && */(!isUkosabihsMotion);

    // NE adaptive dnr4w_max
    MS_U8 dnr4Max = noiseLevel > 40 ? (noiseLevel-40)/4 + 0xC : 0xC;
    dnr4Max = dnr4Max < 0xF ? dnr4Max : 0xF;
    dnr4Max = neAdptDbgEn ? dnr4Max : 0xC;
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_4D_L, dnr4Max << 12, 0xF000 );

    // NE adaptive mcnr noise level
    MS_U8 nlMax = noiseLevel > 40 ? (noiseLevel-40)/4 + 8 : 8;
    nlMax = nlMax < 0xF ? nlMax : 0xF;
    nlMax = neAdptDbgEn ? nlMax : 8;
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_18_L, nlMax << 8, 0x0F00 );

    MS_U16 gmvDiff = 0;
    MS_U8 gmvAvgX = 0, gmvAvgY = 0;
    MDrv_SC_GMV_Stability( &gmvDiff, &gmvAvgX, &gmvAvgY, GMV, isOpFrameChanged );
    //printf( "GMV diff, avgX, avgY: %d, %d %d\n", gmvDiff, gmvAvgX, gmvAvgY );

    //DEBUG_LC_41( motion4StillCnt );
    //DEBUG_LC_43( noiseLevel );
    //printf( "noise level : %d\n", noiseLevel );

    // force film-like/m2s weight to max if source is progressive/film
    if( (!bInterlace) || filmstatus->film_act )
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_6E_L, 0xE000, 0xFF00);
        if( dbgEn ) MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_44_L, 0x5000, 0xF000 );
    }
    else
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_6E_L, 0x0000, 0xFF00);
        if( dbgEn ) MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_44_L, 0x3000, 0xF000 );
    }

    MS_U16 mcnrWPixCntThL, pdnrWPixCntThL, mcnrWPixCntThH, pdnrWPixCntThH;
//  MS_U16 smvTh;
    MS_U16 panScanThL, panScanThH;
    MS_U16 almostStillTh;
    if( isHDSrc )
    {
        mcnrWPixCntThL = 0xF0;
        pdnrWPixCntThL = 0xF0;
        mcnrWPixCntThH = 0x3800;
        pdnrWPixCntThH = 0x3800;
        panScanThH = 0x3000;
        panScanThL = 0x2000;
        almostStillTh = 0x200;
#if 0
        if( noiseLevel > 0x40 )
            smvTh = 0xFF;
        else
            smvTh = 0x00;
#endif
    }
    else
    {
        mcnrWPixCntThL = 0xF0;
        pdnrWPixCntThL = 0xF0;
        mcnrWPixCntThH = 0x980; //0x580;
        pdnrWPixCntThH = 0xB80; //0xA00;
        panScanThH = 0x800;
        panScanThL = 0x500;
        almostStillTh = 0x30;
#if 0
        if( noiseLevel > 0x40 )
            smvTh = 0xFF;
        else
            smvTh = 0x00;
#endif
    }

    static MS_BOOL isStill = true, isStillDelay = true, isStillNoSceneChane = true, isStillDelayNoSceneChane = true;
    if( (mcnrWPixCnt < mcnrWPixCntThL && pdnrWPixCnt < pdnrWPixCntThL && motion4StillCnt > 8 )
        || ( mcnrWPixCnt < mcnrWPixCntThH && pdnrWPixCnt < pdnrWPixCntThH && motion4StillCnt > 12 )
        )
    {
        mcnrMotionCnt = 0;
        isStill = isStillDelay = true;
        isStillNoSceneChane = isStillDelayNoSceneChane = isSceneChange ? false : true;
    }
    else
    {
        if( mcnrMotionCnt > 16 )
        {
            isStill = isStillNoSceneChane = false;

            if( mcnrMotionCnt > 32 )
                isStillDelay = isStillDelayNoSceneChane = false;
            else
            {
                isStillDelay = true;
                isStillDelayNoSceneChane = ( isSceneChange || !isStillDelayNoSceneChane ) ? false : true;
                ++mcnrMotionCnt;
            }
        }
        else
        {
            isStill = isStillDelay = true;
            isStillNoSceneChane = ( isSceneChange || !isStillNoSceneChane ) ? false : true;
            isStillDelayNoSceneChane = ( isSceneChange || !isStillDelayNoSceneChane ) ? false : true;
            ++mcnrMotionCnt;
        }
    }

    // reduce pdnr weight by ME1 smv at small moving or almost still scenes
//  MS_BOOL isAlmostStill;
    if( me1MovingWeight < almostStillTh )
    {
//      isAlmostStill = true;
        if( dbgEn ) MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_44_L, 0x0010, 0x00F0 );
    }
    else
    {
//      isAlmostStill = false;
        if( dbgEn ) MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_44_L, 0x0020, 0x00F0 );
    }

    // panScan
    static MS_BOOL isPanScan = false;
    if( me1MovingWeight > panScanThH )
    {
        if( panScanCnt < 16 )
            ++panScanCnt;
    }
    else
    {
        if( !(isPanScan && me1MovingWeight > panScanThL) )
        {
            if( panScanCnt > 0 )
                --panScanCnt;
        }
    }

    if( panScanCnt > 6 )
    {
        isPanScan = true;
        if( dbgEn ) MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_44_L, 0x0100, 0x0F00 );
    }
    else
    {
        isPanScan = false;
        if( dbgEn ) MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_44_L, 0x0200, 0x0F00 );
    }

    // low texture moving
    static MS_U8 lowTexMovingCnt = 0; // 0~32
    MS_BOOL isMe1Moving = false, isMe2Moving = false, is1DLowTexMoving = false;
    if( me1Mv0Blk < 0x20 && me1NonMv0Blk < 0x40 && me1NotFoundBlk > 0x3D0 && me1MovingWeight > 0x500/*0x1300*/ )
    {
        isMe1Moving = true;
    }

    if( me2Mv0Blk < 0x800 && me2NonMv0Blk > 0x700 && me2NotFoundBlk > 0xD00 )
    {
        isMe2Moving = true;
    }

    if( mv0pixels < 0x2FF && nonMv0Pixels < 0xF00 && mvNotFoundPixels < 0x160 )
    {
        is1DLowTexMoving = true;
    }

    if( isMe1Moving && isMe2Moving && is1DLowTexMoving )
    {
        if( lowTexMovingCnt < 32 )
            ++lowTexMovingCnt;
    }
    else if( lowTexMovingCnt > 0 )
    {
        --lowTexMovingCnt;
    }

    MS_BOOL isLowTexMoving = false;
    if( lowTexMovingCnt > 8 )
    {
        // enable keep detail
        isLowTexMoving = true;
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_18_L, 0x1000, 0xF000);
    }
    else
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_18_L, 0xF000, 0xF000);
    }

    static MS_U8 pdnrGain = 16;
    static MS_U8 me1NRStrength = 0xF;

    // isUkosabihsMotion then reduce PDNR table
    if( isUkosabihsMotion && isPanScan && !isLowTexMoving )
    {
        // maximum PDNR gain 12
        pdnrGain = pdnrGain > 12 ? pdnrGain-1 : (pdnrGain < 12 ? pdnrGain + 1 : pdnrGain );
        // reduce ME1_moving_w_nr_strength to 3
        me1NRStrength = me1NRStrength > 3 ? me1NRStrength - 1 : me1NRStrength;
    }
    // isLowTexMoving or isUkosabihsMotion but me1 moving weight is small then turn off PDNR
    else if( ( isUkosabihsMotion && !isPanScan ) || isLowTexMoving )
    {
        // turn off PDNR
            pdnrGain = pdnrGain > 6 ? pdnrGain - 6 : 0;
            me1NRStrength = me1NRStrength < 0xE ? me1NRStrength + 2 : 0xF;
    }
    else
    {
            pdnrGain = pdnrGain < 10 ? pdnrGain + 6 : 16;
            me1NRStrength = me1NRStrength < 0xB ? me1NRStrength + 4 : 0xF;
    }

    static MS_U8 maxSnrStrength = 32;
    // isUkosabihsMotion || isPanScan then turn off SNR
    if( isUkosabihsMotion || isPanScan )
        maxSnrStrength = maxSnrStrength > 0 ? maxSnrStrength-1 : 0;
    else
        maxSnrStrength = maxSnrStrength < 32 ? maxSnrStrength+1 : 32;

    // max SNR Weight
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_69_L, maxSnrStrength << 8, 0x3F00);

#if HOUSE_MD_PATTERN_ENABLE
    // House MD patch
    MS_U8 u8LowLumaVerMovingCnt = MDrv_SC_NR_HouseMD_Patch( /*gmvAvgX, gmvAvgY, blkConfidence, */me1NotFoundBlk, lumaHistogramStates, isSceneChange );
#else
    static MS_U8 u8LowLumaVerMovingCnt = 0;
#endif


    // force 2D for stable vertical moving
    static MS_U8 force2DCnt = 0;
    static MS_U8 force1Dcnt = 0;
    //if( isUkosabihsMotion && gmvDiff < 2 && gmvAvgY != 8 && abs( gmvAvgY - 8 ) < 7)
    //printk("GMV X, Y, diff, isPanScan: %d %d %d %d\n", gmvAvgX, gmvAvgY, gmvDiff, isPanScan );
    if( isUkosabihsMotion && gmvDiff < 2 &&
        ( (gmvAvgY != 8 && abs( gmvAvgY - 8 ) < 7) || ( abs( gmvAvgX - 32 ) < 16 && abs( gmvAvgX - 32 ) > 9 ) )
    )
    {
        if( force2DCnt < 32 )
            ++force2DCnt;
    }
    else if(  gmvDiff < 5 &&
        (gmvAvgY != 8 && abs( gmvAvgY - 8 ) < 7) && isPanScan && ( abs( gmvAvgX - 32 ) < 3)  )
    {
        if( force2DCnt < 32 )
            ++force2DCnt;
    }
    else
    {
        if( force2DCnt > 0 )
            --force2DCnt;
    }

    // force 1D if small moving horizontal burst ( me2 not stable )
    if( !isUkosabihsMotion /*&& hpfHdTolerantReport > 0xC000*/ && me1Mv0Cost > 0x1900 && me2SadDiff < 0x100 )
    {
        if( force1Dcnt < 32 )
            ++force1Dcnt;
    }
    else
    {
        if( force1Dcnt > 0 )
            --force1Dcnt;
    }

    if( !isUkosabihsMotion && u8LowLumaVerMovingCnt > 8 )
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_6D_L, 0x4100, 0x7F00 );
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_07_L, 0x0000, 0x0008 );
    }
    else
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_07_L, 0x0008, 0x0008 );

    if( force2DCnt > 16 )
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_6D_L, 0x4000, 0x7F00 );
    }
    else if( force1Dcnt > 16 )
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_6D_L, 0x6000, 0x7F00 );
    }
    else if( force2DCnt < 8 || force1Dcnt < 8 )
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_6D_L, 0x0000, 0x7F00 );
    }
    }

    // still and vertical pan scan
    static MS_U8 mcnrGain = 16;
    if( isUkosabihsMotion )
    {
        if( gmvDiff < 2 && gmvAvgY != 8 && abs( gmvAvgY - 8 ) < 7 && abs( gmvAvgX - 32 ) < 16 )
        {
            // stable vertical moving
            mcnrGain = mcnrGain > 8 ? mcnrGain  - 1 : ( mcnrGain < 8 ? mcnrGain + 1 : 8 );
        }
        else if( gmvAvgY != 8 && abs( gmvAvgY - 8 ) < 7 && abs( gmvAvgX - 32 ) < 16 )
        {
            mcnrGain = mcnrGain > 4 ? mcnrGain  - 1 : ( mcnrGain < 4 ? mcnrGain + 1 : 4 );
        }
        else if( abs( gmvAvgY - 8 ) >= 7 || abs( gmvAvgX - 32 ) >= 16 )
        {
            // high mv vertical moving
            mcnrGain = mcnrGain > 0 ? mcnrGain  - 1 : 0;
        }
        else
        {
            mcnrGain = mcnrGain < 16 ? mcnrGain + 1 : 16;
        }
    }
    else if( isPanScan )
    {
        // not isUkosabihsMotion
        if( gmvAvgY != 8 && abs( gmvAvgY - 8 ) < 7 )
        {
            if( force2DCnt > 16 )
            {
                mcnrGain = mcnrGain < 16 ? mcnrGain + 1 : 16;
            }
            else
            {
                mcnrGain = mcnrGain > 4 ? mcnrGain  - 1 : ( mcnrGain < 4 ? mcnrGain + 1 : 4 );
            }
        }
        else if( abs( gmvAvgY - 8 ) >= 7 )
        {
            // high mv vertical moving
            mcnrGain = mcnrGain > 0 ? mcnrGain  - 1 : 0;
        }
        else
        {
            mcnrGain = mcnrGain < 16 ? mcnrGain + 1 : 16;
        }
    }
    else if( (abs( gmvAvgX - 32 ) >= 16) && (force2DCnt < 16) )
    {
        // motion is out of 1D search range
        // MCNR creates contours
        mcnrGain = mcnrGain > 8 ? mcnrGain  - 1 : mcnrGain;
        pdnrGain = pdnrGain < 20 ? pdnrGain + 2 : pdnrGain;
    }
    else
    {
        //mcnrGain = mcnrGain < 16 ? mcnrGain + 1 : 16;
        //mcnrGain = isSceneChange ? 16 : ( mcnrGain < 16 ? mcnrGain + 1 : 16 );
         mcnrGain = isSceneChange ? 16 : ( mcnrGain < 12 ? mcnrGain + 4 :16 );
    }

        //20160126 marked by Bella due to still pattern in this condition has too much noise
    // direct overwrite
    if (NR_DIRECT_OVERWRITE_EN)
    {   
        if( isStillNoSceneChane /*isStill*/ || ( isStillDelayNoSceneChane/*isStillDelay*/ && gmvAvgY != 8) )
        {
            mcnrGain = 3;
        }
    }
    // MCNR gain
    MDrv_SC_Set_MCNR_Gain( mcnrGain, isSourceChange );

    // direct overwite
    if (NR_DIRECT_OVERWRITE_EN)
    {
        if( isStill || isSceneChange )
        {
            pdnrGain = 0;
        }
    }
    // PDNR gain
    MDrv_SC_Set_PDNR_Gain( pdnrGain, isSourceChange ); // For UCNR delay Issue: 1078573, solution: open this line and mark"MDrv_SC_Set_PDNR_Gain( ( isStill || isSceneChange ) ? 0 : pdnrGain,isSourceChange );" by Jay Huang
    //MDrv_SC_Set_PDNR_Gain( ( isStill || isSceneChange ) ? 0 : pdnrGain,isSourceChange );
    // ME1_moving_w_nr_strength
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_66_L, me1NRStrength << 12, 0xF000);


    // For BWS1&UCNR both ON, image sticking @scene change (turn on UCNR dither @scene change 12 times.)
    // 2014.9.16 Ryan
    static MS_U16 SceneCnt=0;
    if( ((MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK08_01_L,0x0001)) == 0x0000) && (isSceneChangeExt) ) //Due to compress issue, when BWS enable and scenechange, dither enable
    {
        SceneCnt = 12; //
    }
    else
    {
        if(SceneCnt>0)
        {
            SceneCnt = SceneCnt - 1;
        }
        else
        {
            SceneCnt = 0;
        }
    }

    if(SceneCnt>0)
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_5C_L, 0x003C, 0x007F);
    }
    else
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK2A_5C_L, 0x007F, 0x007F);
    }


    static MS_U8 postDINRGain = 16;
    if( isUkosabihsMotion || isPanScan || is1DLowTexMoving )
    {
        postDINRGain = postDINRGain > 4 ? postDINRGain  - 4 : 0;
    }
    else
    {
        postDINRGain = postDINRGain < 16 ? postDINRGain + 1 : 16;
    }

    // PostDI NR gain
    MDrv_SC_Set_PostDINR_Gain( postDINRGain, isSourceChange );

    //printf( "Gain : %d, %d, %d \n", mcnrGain, pdnrGain, postDINRGain );

#if 0
    // luma peaking
    if( isPanScan && gmvAvgX != 32 && gmvAvgY == 8 && !isUkosabihsMotion )
    {
        MDrv_SC_Set_Luma_PK_Gain( panScanCnt );
    }
    else
    {
        MDrv_SC_Set_Luma_PK_Gain( 0 );
    }
#endif

    if( dbgEn )
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_45_L, (isMe1Moving ? 0x0002 : 0x0001), 0x000F );
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_45_L, (isMe2Moving ? 0x0020 : 0x0010), 0x00F0 );
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_45_L, (is1DLowTexMoving ? 0x0200 : 0x0100), 0x0F00 );
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_45_L, (isLowTexMoving ? 0x2000 : 0x1000), 0xF000 );

        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_44_L, (pdnrGain == 0) ? 0x0001 : 0x0002, 0x000F );

        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_46_L, isUkosabihsMotion ? 0x0020 : 0x0010, 0x00F0 );
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_46_L, force1Dcnt > 16 ? 0x0200 : 0x0100, 0x0F00 );
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_46_L, mcnrGain == 16 ? 0x2000 : 0x1000, 0xF000 );
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_47_L, (isSceneChange ? 0x0002 : 0x0001), 0x000F );
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_47_L, (isStillNoSceneChane ? 0x0020 : 0x0010), 0x00F0 );

        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_47_L, ( ( (NE_confirm == 8) || (NE_confirm==2 && lastEN==1) ) ? 0x2000 : 0x1000), 0xF000 );

        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_48_L, (noiseLevel > 10 ? 0x0002 : 0x0), 0x000F );
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_48_L, (noiseLevel > 20 ? 0x0020 : 0x0), 0x00F0 );
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_48_L, (noiseLevel > 30 ? 0x0200 : 0x0), 0x0F00 );
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_48_L, (noiseLevel > 40 ? 0x2000 : 0x0), 0xF000 );

        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_49_L, (noiseLevel > 50 ? 0x0002 : 0x0), 0x000F );
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_49_L, (noiseLevel > 60 ? 0x0020 : 0x0), 0x00F0 );
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_49_L, (noiseLevel > 70 ? 0x0200 : 0x0), 0x0F00 );
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_49_L, (noiseLevel > 80 ? 0x2000 : 0x0), 0xF000 );

        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_50_L, (noiseLevel >  90 ? 0x0002 : 0x0), 0x000F );
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_51_L, (noiseLevel > 100 ? 0x0020 : 0x0), 0x00F0 );
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_52_L, (noiseLevel > 110 ? 0x0200 : 0x0), 0x0F00 );
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_53_L, (noiseLevel > 120 ? 0x2000 : 0x0), 0xF000 );
    }

    // adaptive coring
    //MDrv_Adaptive_Coring( noiseLevel, isSourceChange, isSceneChange );
}

MS_BOOL MDrv_SC_Agate_ME_Reset( const MS_U32 u32MotionValue2, const MS_BOOL isSceneChangeMc, const MS_BOOL isSceneChangeY, const MS_BOOL isSourceChange  )//review MERESET
{
    static MS_U8 u8CurFrameCnt1 = 0;
    static MS_U8 u8CurFrameCnt2 = 0;
    static MS_BOOL meReset = false;
    MS_BOOL meResetFlag = false;

    if (u32MotionValue2 == 0 )
    {
        if(u8CurFrameCnt1 != 60 )
            u8CurFrameCnt1++;
    }
    else
    {
        u8CurFrameCnt1 = 0;
    }

    if ( meReset )
    {
        if(u8CurFrameCnt2 != 60 )
            u8CurFrameCnt2++;
    }
    else
    {
        u8CurFrameCnt2 = 0;
    }

    if( (u8CurFrameCnt1 == 30) && (!meReset) )
    {
        meReset = true;
    }

    if( (u8CurFrameCnt2 == 5) && meReset )
    {
        meReset = false;
        meResetFlag = true;
    }
    else
    {
        meResetFlag = false;
    }

    if( meReset || isSceneChangeMc || isSceneChangeY || isSourceChange)
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK17_79_L,  0x0011, 0x0011);
    }
    else
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK17_79_L,  0x0000, 0x0011);
    }
    return meResetFlag;
}

void DEBUG_LC_42( const MS_BOOL isSceneChangeMc, const MS_BOOL isSceneChangeY )
{
    if( isSceneChangeMc )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_42_L, 0x0003, 0x0007);
    else
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_42_L, 0x0000, 0x0007);

    if( isSceneChangeY )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_42_L, 0x0040, 0x0070);
    else
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_42_L, 0x0000, 0x0070);
}


MS_U8 LumaAGCLUT(const MS_U8 LumaCheck )
{
    MS_U8 lumaAgcGain = 0;

    if( LumaCheck > 120 )
        lumaAgcGain = 6;
    else if( ( LumaCheck > 114 ) &&  ( LumaCheck <= 120 ))
        lumaAgcGain = 7;
    else if( ( LumaCheck > 104 ) &&  ( LumaCheck <= 114 ))
        lumaAgcGain = 8;
    else if( ( LumaCheck > 94 ) &&  ( LumaCheck <= 104 ))
        lumaAgcGain = 8;
    else if( ( LumaCheck > 83 ) &&  ( LumaCheck <= 94 ))
        lumaAgcGain = 9;
    else if( ( LumaCheck > 73 ) &&  ( LumaCheck <= 83 ))
        lumaAgcGain = 10;
    else if( ( LumaCheck > 62 ) &&  ( LumaCheck <= 73 ))
        lumaAgcGain = 11;
    else if( ( LumaCheck > 51 ) &&  ( LumaCheck <= 62 ))
        lumaAgcGain = 13;
    else if( ( LumaCheck > 40 ) &&  ( LumaCheck <= 51 ))
        lumaAgcGain = 15;
    else if( ( LumaCheck > 26 ) &&  ( LumaCheck <= 40 ))
        lumaAgcGain = 17;
    else
        lumaAgcGain = 15;

    return lumaAgcGain;
}

#define HD_ACT_TH 10
#define HDSDD_SPEED 1
#define HDSDD_DIV 0
#define HD_SPEED 2
#define SD_SPEED 4
void MDrv_SC_Edision_stillFeatherDetection(const MS_U32 u32MotionValue, const MS_BOOL isUkosabihsMotion,
                       const MS_U32 _hpfHdCntReport, const MS_BOOL isHBurstMoving, const MS_BOOL isHBurst, const MS_BOOL isUkosabihsMv0,
                       const MS_BOOL isMcFilmHd/*, const MS_BOOL isSceneChange*/, const MS_U32 _hpfSdCntReport )
{
    MS_U32 exactFeatherCnt = 0;
    static MS_U32 pre_exactFeatherCnt = 0;

    MS_U32 HD_ConfirmCnt_Reg = 0;
    static MS_U32 HD_ConfirmCnt_Reg_post = 0;
    static MS_U16 HD_ConfirmCnt = 0;

    MS_U8 u8MinPixel = 0;
    static MS_U8 u8PrevMinPixel = 0;

    MS_U8 u8MaxPixel = 0;
    static MS_U8 u8PrevMaxPixel = 0;

    MS_U32 Luma = 0;
    static MS_U32 LumaPrev = 0;
    MS_U8 LumaCheck = 0;

    MS_U16 Cplx = 0;
    static MS_U16 CplxPrev = 0;
    MS_U8 CplxCheck = 0;

    MS_U8 lumaAgcGain = 0;
    MS_BOOL cplxLumaPatch = false;

    MS_U8  featherRatio = 0;

//  MS_BOOL XXX;

    MS_BOOL isMovingImage = ( u32MotionValue!=0 ) ? true : false;

    MS_BOOL isWithoutHsignal = ( _hpfSdCntReport < 0x10 ) ? true : false;

//  XXX = isSceneChange;
    exactFeatherCnt = (MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK23_6D_L,0xFFFF) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK23_6C_L,0xFFFF);
    exactFeatherCnt = ( pre_exactFeatherCnt*3 + exactFeatherCnt ) >> 2;
    pre_exactFeatherCnt = exactFeatherCnt;

    u8MaxPixel = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK1A_0B_L, 0x00FF);//move to UFSC in Maserati
    u8MaxPixel = (u8PrevMaxPixel*3 + u8MaxPixel ) >> 2;
    u8PrevMaxPixel = u8MaxPixel;

    u8MinPixel = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK1A_0B_L, 0xFF00)>>8; //move to UFSC in Maserati
    u8MinPixel = (u8PrevMinPixel*3 + u8MinPixel ) >> 2;
    u8PrevMinPixel = u8MinPixel;

    Luma = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK55_1A_L,0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_1B_L, 0x000F )<<16);//move to UFSC in Maserati
    Luma = (LumaPrev*3 + Luma ) >> 2;
    LumaPrev = Luma;

    Cplx = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK2A_78_L, 0xFFFF);
    Cplx = (CplxPrev*3 + Cplx ) >> 2;
    CplxPrev = Cplx;

    {
        LumaCheck = Luma >> 8;
        CplxCheck = Cplx >> 8;
        lumaAgcGain = LumaAGCLUT( LumaCheck );
        cplxLumaPatch = ( CplxCheck > ( LumaCheck * lumaAgcGain ) >> 3 ) ? true : false;
    }

    if( ( isUkosabihsMv0 && !isMovingImage) || isMcFilmHd )
        featherRatio = 5;
    else if( !isUkosabihsMotion && isMovingImage )
        featherRatio = 2;
    else
        featherRatio = 4;

    if((exactFeatherCnt > 0x20)&&( exactFeatherCnt > (_hpfHdCntReport*featherRatio)))
    {
        if(HD_ConfirmCnt > (0xFF - SD_SPEED))
            HD_ConfirmCnt = 0xFF;
        else
            HD_ConfirmCnt = HD_ConfirmCnt + SD_SPEED;
    }
    else
    {
        if(HD_ConfirmCnt < 1)
            HD_ConfirmCnt = 0;
        else
            HD_ConfirmCnt = HD_ConfirmCnt - 1;
    }

    if( HD_ConfirmCnt > 0x80 )
        HD_ConfirmCnt_Reg = HD_ConfirmCnt - 0x80;
    else
        HD_ConfirmCnt_Reg = 0;

    HD_ConfirmCnt_Reg = (HD_ConfirmCnt_Reg*HDSDD_SPEED) >> HDSDD_DIV;

    if( HD_ConfirmCnt_Reg > 0x3F )
        HD_ConfirmCnt_Reg = 0x3F;

    if( cplxLumaPatch )
    {
        HD_ConfirmCnt_Reg_post = 0;
    }
    else if( isWithoutHsignal )
    {
        HD_ConfirmCnt_Reg_post = 0;
    }
    else if( (isHBurst || isHBurstMoving || isUkosabihsMotion ) && HD_ConfirmCnt_Reg != 0 )
    {
        if( HD_ConfirmCnt_Reg_post < 8 )
            HD_ConfirmCnt_Reg_post = 0;
        else
            HD_ConfirmCnt_Reg_post = HD_ConfirmCnt_Reg_post - 8;
    }
    else if( u8MaxPixel < 25 && HD_ConfirmCnt_Reg != 0 )
    {
        if( HD_ConfirmCnt_Reg_post < 8 )
            HD_ConfirmCnt_Reg_post = 0;
        else
            HD_ConfirmCnt_Reg_post = HD_ConfirmCnt_Reg_post - 8;
    }
    else
        HD_ConfirmCnt_Reg_post = HD_ConfirmCnt_Reg;

    //121 weight

    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK23_54_L, (MS_U16)HD_ConfirmCnt_Reg_post, 0x001F);

}

void DMS_SDHDModeCheck( MS_U8 u8SD2HDValue) //Register Settings to hardware
{
    if( (u8SD2HDValue>=60) && (frameWidth>0x700)  ) //SD to HD mode
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK26_10_L, 0x4000, 0x4000); // HD_enable
    else
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK26_10_L, 0x0000, 0x4000);
}

/******************************************************************************/
///CCS Driver
/******************************************************************************/

void MDrv_SC_Edison_CCS_driver( const MS_BOOL ukoSabihS_en )
{
    // horizontal moving => lower ccs weight
    static MS_U32 u32CCSvideoCnt = 0;
    MS_BOOL isMvPanScan = FALSE;

    //if((gMv.h1XYsum >10) && (gMv.h2XYsum > 10) && ( gMv.h1XYdiff < 5) && (gMv.h2XYdiff < 5))
    if( ukoSabihS_en )
    {
        isMvPanScan = true;
    }
    else
    {
        isMvPanScan = false;
    }

    if(isMvPanScan)
    {
        u32CCSvideoCnt++;

        if( u32CCSvideoCnt >= 20 )
        {
            u32CCSvideoCnt = 20;
        }
    }
    else
    {
        if( u32CCSvideoCnt == 0 )
        {
            u32CCSvideoCnt = 0;
        }
        else
        {
            u32CCSvideoCnt--;
        }
    }

    if( u32CCSvideoCnt <= 2 )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_53_L, 0x000F, 0x000F); //ccs user weight
    else if( u32CCSvideoCnt == 3 )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_53_L, 0x000D, 0x000F); //ccs user weight
    else if( u32CCSvideoCnt == 4 )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_53_L, 0x000B, 0x000F); //ccs user weight
    else if( u32CCSvideoCnt == 5 )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_53_L, 0x0009, 0x000F); //ccs user weight
    else if( u32CCSvideoCnt == 6 )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_53_L, 0x0007, 0x000F); //ccs user weight
    else if( u32CCSvideoCnt == 7 )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_53_L, 0x0005, 0x000F); //ccs user weight
    else if( u32CCSvideoCnt == 8 )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_53_L, 0x0003, 0x000F); //ccs user weight
    else if( u32CCSvideoCnt == 9 )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_53_L, 0x0001, 0x000F); //ccs user weight
    else if( u32CCSvideoCnt >= 10 )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_53_L, 0x0000, 0x000F); //ccs user weight
}

void MDrv_SC_Edison_DHD_driver( MS_U32 uComplex, MS_U16 nonMv0Comfirm, MS_BOOL me1Still, MS_U16 mv0Comfirm, XC_GMvStates gMV, MS_U32 _hpfHdCntReport )
{
    // dhd stadium patch: lower dhd strength
    static MS_U32 u32filmCnt = 0;
    int check_width = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK12_0F_L, 0xFFFF);

    MS_U32 exactFeatherCnt = 0;
    static MS_U32 pre_exactFeatherCnt = 0;
    MS_BOOL isMv0 = ( ( gMV.minMvXdir == 32 ) && ( gMV.minMvYdir == 8 ) ) ? true : false;

    exactFeatherCnt = (MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK23_6D_L,0xFFFF) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK23_6C_L,0xFFFF);
    exactFeatherCnt = ( pre_exactFeatherCnt*3 + exactFeatherCnt ) >> 2;
    pre_exactFeatherCnt = exactFeatherCnt;

    MS_U32 colorSum = 0;
    static MS_U32 colorSum_iir = 0;

    colorSum  = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK55_1E_L,0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_1F_L, 0x000F )<<16); //move to UFSC in Maserati

    colorSum = ((colorSum_iir*3 )+ colorSum ) >> 2;
    colorSum_iir = colorSum;

    if((uComplex>0x680) && (nonMv0Comfirm > 0x70) && (me1Still == false) && (check_width > 0x700) && ((/*exactFeatherCnt > 0x30 &&*/colorSum > 0x1500 && colorSum < 0x2500 )|| mv0Comfirm < 0x50 ) ) //exactFeatherCnt > 0x30 for stadium & mv0Comfirm < 0x50 for building of "KURO" disc
    {
        u32filmCnt++;

        if( u32filmCnt >= 50 )
        {
            u32filmCnt = 50;
        }
    }
    else if((gMV.h2XYsum > 0x30)&& (gMV.h2XYdiff <= 0x06)&& (!isMv0) && (nonMv0Comfirm>0x40) && mv0Comfirm < 0x20 )//shibasoku pan
    {

        u32filmCnt++;

        if( u32filmCnt >= 50 )
        {
            u32filmCnt = 50;
        }
    }
    else if ( _hpfHdCntReport > 0xFFFF )
    {

        u32filmCnt++;

        if( u32filmCnt >= 50 )
        {
            u32filmCnt = 50;
        }
    }
    else
    {
        if( u32filmCnt == 0 )
        {
            u32filmCnt = 0;
        }
        else
        {
            u32filmCnt--;
        }
    }

    if( u32filmCnt <= 2 )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0F00, 0x0F00); //dhd user weight
    else if( u32filmCnt == 3 )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0C00, 0x0F00); //dhd user weight
    else if( u32filmCnt == 4 )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0B00, 0x0F00); //dhd user weight
    else if( u32filmCnt == 5 )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0900, 0x0F00); //dhd user weight
    else if( u32filmCnt == 6 )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0700, 0x0F00); //dhd user weight
    else if( u32filmCnt == 7 )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0500, 0x0F00); //dhd user weight
    else if( u32filmCnt == 8 )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0300, 0x0F00); //dhd user weight
    else if( u32filmCnt == 9 )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0100, 0x0F00); //dhd user weight
    else if( u32filmCnt >= 10 )
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0000, 0x0F00); //dhd user weight

}

void MDrv_SC_Edison_DHD_Weather_driver( MS_U32 uComplex, MS_U16 nonMv0Comfirm, MS_BOOL me1Still, MS_U16 mv0Comfirm )
{
    // dhd weather patch: enhance dhd strength
    static MS_U32 u32filmCnt = 0;
    int check_width = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK12_0F_L, 0xFFFF);

    MS_U32 exactFeatherCnt = 0;
    static MS_U32 pre_exactFeatherCnt = 0;

    exactFeatherCnt = (MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK23_6D_L,0xFFFF) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK23_6C_L,0xFFFF);
    exactFeatherCnt = ( pre_exactFeatherCnt*3 + exactFeatherCnt ) >> 2;
    pre_exactFeatherCnt = exactFeatherCnt;


    MS_U32 colorSum = 0;
    static MS_U32 colorSum_iir = 0;

    colorSum  = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK55_1E_L,0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_1F_L, 0x000F )<<16); //move to UFSC in Maserati

    colorSum = ((colorSum_iir*3 )+ colorSum ) >> 2;
    colorSum_iir = colorSum;

    if((uComplex>0x800) && (mv0Comfirm > 0x140) && (nonMv0Comfirm < 0x40) && (me1Still == true) && (check_width>0x700) && (exactFeatherCnt < 0x10) && (colorSum > 0x700) )
    {
        u32filmCnt++;

        if( u32filmCnt >= 24 )
        {
            u32filmCnt = 24;
        }
    }
    else
    {
        if( u32filmCnt > 0 )
            u32filmCnt = u32filmCnt - 2;
    }

    if( check_width>0x700 )
    {

        if( u32filmCnt <= 2 )
        {
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0x0040, 0x00FF); //dhd c edge diff(hor).
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0xA000, 0xFF00); //dhd y edge diff(hor).
        }
        else if( u32filmCnt == 3 )
        {
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0x0054, 0x00FF); //dhd c edge diff(hor).
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0xAB00, 0xFF00); //dhd y edge diff(hor).
        }
        else if( u32filmCnt == 4 )
        {
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0x0068, 0x00FF); //dhd c edge diff(hor).
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0xB600, 0xFF00); //dhd y edge diff(hor).
        }
        else if( u32filmCnt == 5 )
        {
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0x007C, 0x00FF); //dhd c edge diff(hor).
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0xC100, 0xFF00); //dhd y edge diff(hor).
        }
        else if( u32filmCnt == 6 )
        {
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0x0090, 0x00FF); //dhd c edge diff(hor).
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0xCC00, 0xFF00); //dhd y edge diff(hor).
        }
        else if( u32filmCnt == 7 )
        {
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0x00A4, 0x00FF); //dhd c edge diff(hor).
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0xD700, 0xFF00); //dhd y edge diff(hor).
        }
        else if( u32filmCnt == 8 )
        {
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0x00B8, 0x00FF); //dhd c edge diff(hor).
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0xE200, 0xFF00); //dhd y edge diff(hor).
        }
        else if( u32filmCnt == 9 )
        {
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0x00CC, 0x00FF); //dhd c edge diff(hor).
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0xED00, 0xFF00); //dhd y edge diff(hor).
        }
        else if( u32filmCnt >= 10 )
        {
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0x00E0, 0x00FF); //dhd c edge diff(hor).
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0xF800, 0xFF00); //dhd y edge diff(hor).
        }

    }

}

void MDrv_SC_Edison_DHD_GSLSM_driver( MS_U32 uComplex, MS_U16 nonMv0Comfirm, MS_BOOL me1Still, MS_U16 mv0Comfirm )
{
    // dhd patch for global still & local moving region( dot-crawl on dirty background ): enhance dhd strength
    static MS_U32 u32filmCnt = 0;
    int check_width = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK12_0F_L, 0xFFFF);

    MS_U32 exactFeatherCnt = 0;
    static MS_U32 pre_exactFeatherCnt = 0;

    exactFeatherCnt = (MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK23_6D_L,0xFFFF) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK23_6C_L,0xFFFF);
    exactFeatherCnt = ( pre_exactFeatherCnt*3 + exactFeatherCnt ) >> 2;
    pre_exactFeatherCnt = exactFeatherCnt;

    MS_U32 colorSum = 0;
    static MS_U32 colorSum_iir = 0;

    colorSum  = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK55_1E_L,0xFFFF)+(MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK55_1F_L, 0x000F )<<16); //move to UFSC inMaserati

    colorSum = ((colorSum_iir*3 )+ colorSum ) >> 2;
    colorSum_iir = colorSum;

    if((uComplex>0x800) && (mv0Comfirm >= nonMv0Comfirm*3) && (me1Still == true) && (check_width>0x700) && ((exactFeatherCnt > 0x40) || (exactFeatherCnt < 0x10 && colorSum > 0xB00) ) ) //exactFeatherCnt < 0x10 && colorSum > 0xB00 for weather
    {
        u32filmCnt++;

        if( u32filmCnt >= 24 )
        {
            u32filmCnt = 24;
        }
    }
    else
    {
        if( u32filmCnt > 0 )
            u32filmCnt = u32filmCnt - 2;
    }

    if( check_width>0x700 )
    {
        if( u32filmCnt <= 2 )
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_22_L, 0x0024, 0x00FF); //dhd y(n), y(n-4) diff.
        else if( u32filmCnt == 3 )
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_22_L, 0x0026, 0x00FF); //dhd y(n), y(n-4) diff.
        else if( u32filmCnt == 4 )
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_22_L, 0x0029, 0x00FF); //dhd y(n), y(n-4) diff.
        else if( u32filmCnt == 5 )
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_22_L, 0x002B, 0x00FF); //dhd y(n), y(n-4) diff.
        else if( u32filmCnt == 6 )
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_22_L, 0x002E, 0x00FF); //dhd y(n), y(n-4) diff.
        else if( u32filmCnt == 7 )
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_22_L, 0x0030, 0x00FF); //dhd y(n), y(n-4) diff.
        else if( u32filmCnt == 8 )
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_22_L, 0x0033, 0x00FF); //dhd y(n), y(n-4) diff.
        else if( u32filmCnt == 9 )
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_22_L, 0x0035, 0x00FF); //dhd y(n), y(n-4) diff.
        else if( u32filmCnt >= 10 )
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK05_22_L, 0x0038, 0x00FF); //dhd y(n), y(n-4) diff.
    }

}

//
//To adjust the CCS_MR HisWeight Ctrl
//
#define CCS_MR_HisWeight_MaxValue 0x1F
#define CCS_MR_HisWeight_MinValue  0x18
void MDrv_SC_DHD_CCS_MR_HisWeight_Ctrl(const XC_FilmStates filmstatus)
{
    static MS_U8 Reg_CCS_MR_Weight = CCS_MR_HisWeight_MinValue;

    if( (filmstatus.film32) |(filmstatus.filmany))
    {
        Reg_CCS_MR_Weight = CCS_MR_HisWeight_MaxValue;
    }
    else
    {
        if( Reg_CCS_MR_Weight > CCS_MR_HisWeight_MinValue )
            Reg_CCS_MR_Weight--;
        else
            Reg_CCS_MR_Weight = CCS_MR_HisWeight_MinValue;
    }
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK06_77_L, Reg_CCS_MR_Weight<<8, 0x1F00);
}

//-------- 3D source format detection begin ---------//
#define sensio_thrd 3
#define format_up_thrd 500
#define format_low_thrd 100
int MDrv_SC_3DorderDetect( void)
{
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_01_L, 0x0000, 0x0010 );  // turn on 3d format detection

    int PA_count = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK3A_20_L,0xFFFF);
    int LA_count = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK3A_21_L,0xFFFF);
    int CB_count = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK3A_22_L,0xFFFF);
    int sensio_l_even = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK3A_23_L,0xFFFF);
    int sensio_l_odd  = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK3A_24_L,0xFFFF);
    int sensio_r_even = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK3A_25_L,0xFFFF);
    int sensio_r_odd  = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK3A_26_L,0xFFFF);

    int sensio_flag = 0;
    int format = 0;  // 0:2D, 1:PA, 2:LA, 3:CB, 4:Sensio

        int sensio_flag_l_ratio =0;
        int sensio_flag_r_ratio =0;
//  int sensio_L_is_EVEN= 0;  // =1 EVEN;  =0 ODD
//      int sensio_R_is_EVEN= 0;

        if ((sensio_l_even> sensio_l_odd) && (sensio_l_odd!=0))
        {
            sensio_flag_l_ratio = sensio_l_even/sensio_l_odd;
//      sensio_L_is_EVEN =1;
        }
        else if ((sensio_l_odd> sensio_l_even) && (sensio_l_even!=0))
            sensio_flag_l_ratio = sensio_l_odd/sensio_l_even;
        else
            sensio_flag_l_ratio =-1;

        if ((sensio_r_even> sensio_r_odd) && (sensio_r_odd!=0))
        {
            sensio_flag_r_ratio = sensio_r_even/sensio_r_odd;
//          sensio_R_is_EVEN =1;
        }
        else if ((sensio_r_odd> sensio_r_even) && (sensio_r_even!=0))
            sensio_flag_r_ratio = sensio_r_odd/sensio_r_even;
        else
            sensio_flag_r_ratio =-1;

        if (sensio_flag_l_ratio> sensio_thrd && sensio_flag_r_ratio> sensio_thrd)
            sensio_flag =1;
        else
            sensio_flag =0;


        // 0:2D, 1:PA, 2:LA, 3:CB, 4:Sensio
        if(sensio_flag)
        {
            format = 4;
            //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_50_L, 0x0007, 0x0007 );  // debug light
        }
        else
        {
                if  ( ( LA_count < CB_count ) && ( PA_count < CB_count ) )
                {
                    format = 3;
                    //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x0004, 0x0007 );  // debug light
                }
                else if ( ( LA_count < format_low_thrd ) && (PA_count > format_up_thrd) )
                {
                    format = 1;
                    //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x0001, 0x0007 );  // debug light
                }
                else if ( ( PA_count < format_low_thrd ) && (LA_count > format_up_thrd) )
                {
                    format = 2;
                    //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x0002, 0x0007 );  // debug light
                }
                else
                {
                    format = 0;
                    //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x0000, 0x0007 );  // debug light
                }

        }

    //printf( "3D format is : %d\n", format );
    //printf( "PA_count : %d\n", PA_count );
    //printf( "LA_count : %d\n", LA_count );
    //printf( "CB_count : %d\n", CB_count );
    //printf( "sensio_l_even : %d\n", sensio_l_even );
    //printf( "sensio_l_odd  : %d\n", sensio_l_odd );
    //printf( "sensio_r_even : %d\n", sensio_r_even );
    //printf( "sensio_r_odd  : %d\n", sensio_r_odd );
    return format;

}

int heightEstimate(int FrameWidth)
{
    if (FrameWidth < 720)
        return FrameWidth*3/4;
    else if (FrameWidth == 720)
        return 480;
    else if ((FrameWidth>720) && (FrameWidth<1440))
        return FrameWidth*9/16;
    else if (FrameWidth == 1440)
        return 900;
    else if ((FrameWidth>1440) && (FrameWidth<1920))
        return FrameWidth*9/16;
    else if (FrameWidth==1920)
        return 1080;
    else if (FrameWidth>=3840)
        return 2160;
    else //if (frameWidth>1920)
        return FrameWidth*9/16;
}

//       only support progressive input soure       //
//      every 4 horizontal pixel made one decision  //
//      histogram data has been shifted 1 bit       //
#define TBLR_low_thrd  16
#define TBLR_high_thrd 20
int MDrv_SC_3DorderDetect_TBLR(void)
{

  MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_01_L, 0x0000, 0x0010 );  // turn on 3d format detection
    int FrameWidth  = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK12_0F_L, 0xFFFF);
    int FrameHeight = heightEstimate(FrameWidth);
    int totalPix = FrameWidth*FrameHeight/4;

    //printf("FrameWidth = %d, FrameHeight= %d \n", FrameWidth, FrameHeight );

    MS_BOOL TBLR_MODE_flag = true ;
    static int TBLR_MODE_CNT = 0 ;
    static MS_BOOL isHistSet = false;

    int histbin = 0;
    int hist0[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int hist1[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int hist2[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int hist3[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    int sum0 = 0;
    int sum1 = 0;
    int sum2 = 0;
    int sum3 = 0;

    int Psum0 = 0;
    int Psum1 = 0;
    int Psum2 = 0;
    int Psum3 = 0;

    int diff_02= 0;
    int diff_13= 0;
    int diff_03= 0;
    int diff_12= 0;
    int shift = 0;

    int  sum_Diff_02 = 0;
    int  sum_Diff_13 = 0;
    int  sum_Diff_03 = 0;
    int  sum_Diff_12 = 0;
    int  sum_Diff_total = 0;

    static int  sum_Diff_TB02 = 0;
    static int  sum_Diff_TB13 = 0;
    static int  sum_Diff_TB03 = 0;
    static int  sum_Diff_TB12 = 0;
    static int  sum_Diff_LR02 = 0;
    static int  sum_Diff_LR13 = 0;
    static int  sum_Diff_LR03 = 0;
    static int  sum_Diff_LR12 = 0;
    static int  curStatus = 0;


    if((TBLR_MODE_CNT%2)==0)
        TBLR_MODE_flag= true;
    else if((TBLR_MODE_CNT%2)==1)
        TBLR_MODE_flag= false;

    if (TBLR_MODE_flag)  // TB
    {
            //printf("----- TB histogram------\n" );
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_40_L, 0x0000,                    0x01FFF );         // hist win0 x_start
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_41_L, 0x0000,                0x01FFF );         // hist win0 y_start
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_42_L, FrameWidth      -0x0001,   0x01FFF );         // hist win0 x_end
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_43_L, FrameHeight/4 -0x0001,   0x01FFF );           // hist win0 y_end

            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_44_L, 0x0000 ,               0x01FFF );         // hist win1 x_start
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_45_L, FrameHeight/4  ,       0x01FFF );         // hist win1 y_start
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_46_L, FrameWidth      -0x0001,   0x01FFF );         // hist win1 x_end
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_47_L, FrameHeight/2 -0x0001,     0x01FFF );         // hist win1 y_end


            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_48_L, 0x0000,                    0x01FFF );         // hist win2 x_start
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_49_L, FrameHeight/2 ,        0x01FFF );         // hist win2 y_start
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_4A_L, FrameWidth        -0x0001, 0x01FFF );         // hist win2 x_end
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_4B_L, FrameHeight*3/4 -0x0001, 0x01FFF );           // hist win2 y_end

            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_4C_L, 0x0000,                    0x01FFF );         // hist win3 x_start
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_4D_L, FrameHeight*3/4 ,      0x01FFF );         // hist win3 y_start
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_4E_L, FrameWidth  -0x0001,   0x01FFF );         // hist win3 x_end
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_4F_L, FrameHeight -0x0001,   0x01FFF );         // hist win3 y_end

    }
    else
    {
          //printf("----- LR histogram------\n" );
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_40_L, 0x0000, 0x01FFF );                        // hist win0 x_start
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_41_L, 0x0000, 0x01FFF );                        // hist win0 y_start
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_42_L, FrameWidth/4  -0x0001,    0x01FFF );      // hist win0 x_end
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_43_L, FrameHeight       -0x0001,    0x01FFF );      // hist win0 y_end

            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_44_L, FrameWidth/4,                 0x01FFF );      // hist win1 x_start
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_45_L, 0x0000  ,                 0x01FFF );      // hist win1 y_start
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_46_L, FrameWidth/2  -0x0001,    0x01FFF );      // hist win1 x_end
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_47_L, FrameHeight       -0x0001 ,   0x01FFF );      // hist win1 y_end

            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_48_L, FrameWidth/2,             0x01FFF );      // hist win2 x_start
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_49_L, 0x0000,                       0x01FFF );      // hist win2 y_start
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_4A_L, FrameWidth*3/4    -0x0001,    0x01FFF );      // hist win2 x_end
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_4B_L, FrameHeight       -0x0001,    0x01FFF );      // hist win2 y_end

            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_4C_L, FrameWidth*3/4,               0x01FFF );      // hist win3 x_start
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_4D_L, 0x0000,                       0x01FFF );      // hist win3 y_start
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_4E_L, FrameWidth  -0x0001,      0x01FFF );      // hist win3 x_end
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_4F_L, FrameHeight -0x0001,      0x01FFF );      // hist win3 y_end

    }

    if( !isHistSet )
    {
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_39_L, 0x0001, 0x0001);
        isHistSet = true;
        //printf( "set trigger bit\n" );
    }
    MS_BOOL histReady = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK3A_39_L, 0x0080) >> 7;  // if histogram is ready


  if( histReady )
  {
    for ( histbin = 0; histbin< 32 ; histbin++ )
        {
            // every 4 pixel in horizontal
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_50_L, histbin, 0x001F );        // select hist bin

            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_50_L, 0x0000,    0x0300 );          // select hist window0
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_51_L, 0x0001,    0x0001 );          // toggle histogram read
            hist0[histbin] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK3A_52_L,0xFFFF);

            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_50_L, 0x0100,    0x0300 );          // select hist window1
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_51_L, 0x0001,  0x0001 );        // toggle histogram read
            hist1[histbin] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK3A_52_L,0xFFFF);

            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_50_L, 0x0200,  0x0300 );        // select hist window2
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_51_L, 0x0001,  0x0001 );        // toggle histogram read
            hist2[histbin] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK3A_52_L,0xFFFF);

            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_50_L, 0x0300,  0x0300 );        // select hist window3
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK3A_51_L, 0x0001,  0x0001 );        // toggle histogram read
            hist3[histbin] = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK3A_52_L,0xFFFF);

            sum0 = sum0 + hist0[histbin]*histbin;
            sum1 = sum1 + hist1[histbin]*histbin;
            sum2 = sum2 + hist2[histbin]*histbin;
            sum3 = sum3 + hist3[histbin]*histbin;


            Psum0 = Psum0 + hist0[histbin];
            Psum1 = Psum1 + hist1[histbin];
            Psum2 = Psum2 + hist2[histbin];
            Psum3 = Psum3 + hist3[histbin];

            //printf( "hist[%d] = %d  %d  %d  %d \n", histbin, hist0[histbin], hist1[histbin], hist2[histbin], hist3[histbin]);

        }

    //printf( "hist total pixel = %d  %d  %d  %d \n", Psum0, Psum1,Psum2, Psum3 );


    sum0 = 4* sum0 / totalPix;
        sum1 = 4* sum1 / totalPix;
        sum2 = 4* sum2 / totalPix;
        sum3 = 4* sum3 / totalPix;

        shift = ((sum0+ sum1) - (sum2 +  sum3))/2;

        if ((sum0 - sum2)*(sum1 - sum3) <= 0)
            shift =0;

        if( abs(shift) >2)
            shift =0;

        //printf("sum0= %d, sum1=  %d, sum2=  %d, sum3=  %d, shift= %d \n", sum0, sum1, sum2, sum3, shift );

        //sum all the diff
        sum_Diff_02= 0;
        sum_Diff_13= 0;
        sum_Diff_03= 0;
        sum_Diff_12= 0;

        for ( histbin = 0; histbin< 32 ; histbin++)
        {

              diff_02= abs(hist0[histbin] - hist2[histbin-shift]);
                diff_13= abs(hist1[histbin] - hist3[histbin-shift]);
                diff_03= abs(hist0[histbin] - hist3[histbin-shift]);
                diff_12= abs(hist1[histbin] - hist2[histbin-shift]);

                if (TBLR_MODE_flag )
                {
                    sum_Diff_02 = sum_Diff_02 + diff_02;
                    sum_Diff_13 = sum_Diff_13 + diff_13;
                    sum_Diff_03 = sum_Diff_03 + diff_03;
                    sum_Diff_12 = sum_Diff_12 + diff_12;

                //  printf(" sum_diff_TB : %d,  %d, %d, %d, , %d, %d, %d, %d \n", sum_Diff_02, sum_Diff_13, sum_Diff_03, sum_Diff_12 , diff_02, diff_13, diff_03, diff_12);
                }
                else
                {
                    sum_Diff_02 = sum_Diff_02 + diff_02;
                    sum_Diff_13 = sum_Diff_13 + diff_13;
                    sum_Diff_03 = sum_Diff_03 + diff_03;
                    sum_Diff_12 = sum_Diff_12 + diff_12;

                //  printf(" sum_diff_LR : %d,  %d, %d, %d, , %d, %d ,%d ,%d \n", sum_Diff_02, sum_Diff_13, sum_Diff_03, sum_Diff_12,  diff_02, diff_13, diff_03, diff_12);
                }
        }

        sum_Diff_total = max( sum_Diff_02+ sum_Diff_13+ sum_Diff_03+ sum_Diff_12, 1);

        //normalization
        if (TBLR_MODE_flag )
        {
            sum_Diff_TB02= sum_Diff_02*100/sum_Diff_total;
            sum_Diff_TB13= sum_Diff_13*100/sum_Diff_total;
            sum_Diff_TB03= sum_Diff_03*100/sum_Diff_total;
            sum_Diff_TB12= sum_Diff_12*100/sum_Diff_total;
        }
        else
        {
            sum_Diff_LR02= sum_Diff_02*100/sum_Diff_total;
            sum_Diff_LR13= sum_Diff_13*100/sum_Diff_total;
            sum_Diff_LR03= sum_Diff_03*100/sum_Diff_total;
            sum_Diff_LR12= sum_Diff_12*100/sum_Diff_total;
        }


        if(TBLR_MODE_CNT%2)
        {

            //  printf("normalized TBdiff : %d %d %d %d \n", sum_Diff_TB02, sum_Diff_TB13, sum_Diff_TB03, sum_Diff_TB12 );
            //  printf("normalized LRdiff : %d %d %d %d \n", sum_Diff_LR02, sum_Diff_LR13, sum_Diff_LR03, sum_Diff_LR12 );

                if( max(sum_Diff_LR02, sum_Diff_LR13) <  max(sum_Diff_LR03, sum_Diff_LR12) &&  min (sum_Diff_LR02, sum_Diff_LR13) < TBLR_low_thrd && max(sum_Diff_TB02, sum_Diff_TB13) > TBLR_high_thrd)
                {
                    curStatus = 2;//LR
                    //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x0020, 0x0070 );  // debug light green
                }
                else if( max(sum_Diff_TB02, sum_Diff_TB13)<  max(sum_Diff_TB03, sum_Diff_TB12) && min (sum_Diff_TB02, sum_Diff_TB13) < TBLR_low_thrd && max(sum_Diff_LR02, sum_Diff_LR13) > TBLR_high_thrd)
                {
                    curStatus = 1;//TB
                    //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x0010, 0x0070 );  // debug light red
                }
                else
                {
                    curStatus = 0;//Video
                    //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x0000, 0x0070 );  // debug light off
                }

                //printf("3d format is : %d \n", curStatus);

        }

        isHistSet = false;
        TBLR_MODE_CNT ++;
    }

        if (TBLR_MODE_CNT > 300)
        {
                TBLR_MODE_CNT = 0;
        }
        //printf("TBLR_MODE_CNT = %d\n", TBLR_MODE_CNT );

    return curStatus;
}

int MDrv_SC_3DfmtDetect(void)
{
    static int format3D = 0;            // 0:2D, 1:TB, 2:LR(SS), 3:PA, 4:LA, 5:CB, 6:Sensio
    int isCBPALASensio =  MDrv_SC_3DorderDetect(); // 0:2D, 1:PA, 2:LA, 3:CB, 4:Sensio
    int isTBLR =  MDrv_SC_3DorderDetect_TBLR();                 //0:2D, 1:TB, 2:LR(SS)

    if( isCBPALASensio )
        format3D = isCBPALASensio +2;
    else
        format3D = isTBLR;

        //printf("3D format is = %d\n", format3D );

    return format3D;

}
//--------- 3D source format detection end  ---------//

/******************************************************************************/
///DeFeathering
///@param u32MotionValue \     IN: Motion value
/******************************************************************************/
void MDrv_SC_de_feathering(MS_U32 u32MotionValue)
{
    static MS_U32 u32DeFeatherCntLv1 = 0;
    static MS_U32 u32DeFeatherCntLv2 = 0;
    static MS_U32 u32DeFeatherCntLv3 = 0;
    MS_U8 u8SST_Static_Core_TH = 0;

    MS_U8 u8SST_Dynamic_Core_TH =0; //YY for SZ feathering 151201
    MS_U8 u8SST_Motion_TH =0;       //YY for SZ feathering 151201

    MS_U32 reg_defethering_lv1_cnt = 0;
    MS_U32 reg_defethering_lv2_cnt = 0;
    MS_U32 reg_mdp_cnt = 0;

    reg_defethering_lv1_cnt = 0x0A;
    reg_defethering_lv2_cnt = 0x08;
    reg_mdp_cnt = 0x01;

    // motion level count
    if(u32MotionValue >= DEFETHERING_LV1_TH)
    {
        if(u32DeFeatherCntLv1 < reg_defethering_lv1_cnt)
            u32DeFeatherCntLv1++;
    }
    else
    {
        if(u32DeFeatherCntLv1 >= reg_mdp_cnt)
            u32DeFeatherCntLv1 = u32DeFeatherCntLv1 - reg_mdp_cnt;
    }

    if(u32MotionValue >= DEFETHERING_LV2_TH)
    {
        if(u32DeFeatherCntLv2 < reg_defethering_lv2_cnt)
            u32DeFeatherCntLv2++;
    }
    else
    {
        if(u32DeFeatherCntLv2 >= reg_mdp_cnt)
            u32DeFeatherCntLv2 = u32DeFeatherCntLv2 - reg_mdp_cnt;
    }

    if(u32MotionValue >= DEFETHERING_LV3_TH)
    {
        if(u32DeFeatherCntLv3 < DEFETHERING_LV3_CNT)
            u32DeFeatherCntLv3++;
    }
    else
    {
        if(u32DeFeatherCntLv3 >= reg_mdp_cnt)
            u32DeFeatherCntLv3 = u32DeFeatherCntLv3 - reg_mdp_cnt;
    }

    //DeFeathering begin
    if(u32DeFeatherCntLv1 >= reg_defethering_lv1_cnt)
    {
        u8SST_Static_Core_TH    = SST_STATIC_CORE_TH_LV1_VALUE;
        u8SST_Motion_TH         = SST_MOTION_TH_LV1_VALUE;        //YY for SZ feathering 151201
        u8SST_Dynamic_Core_TH   = SST_DYNAMIC_CORE_TH_LV1_VALUE;  //YY for SZ feathering 151201
    }
    else if(u32DeFeatherCntLv2 >= reg_defethering_lv2_cnt)
    {
        u8SST_Static_Core_TH    = SST_STATIC_CORE_TH_LV2_VALUE;
        u8SST_Motion_TH         = SST_MOTION_TH_LV2_VALUE;        //YY for SZ feathering 151201
        u8SST_Dynamic_Core_TH   = SST_DYNAMIC_CORE_TH_LV2_VALUE;  //YY for SZ feathering 151201
    }
    else if(u32DeFeatherCntLv3 >= DEFETHERING_LV3_CNT)
    {
        u8SST_Static_Core_TH    = SST_STATIC_CORE_TH_LV3_VALUE;
        u8SST_Motion_TH         = SST_MOTION_TH_LV3_VALUE;        //YY for SZ feathering 151201
        u8SST_Dynamic_Core_TH   = SST_DYNAMIC_CORE_TH_LV3_VALUE;  //YY for SZ feathering 151201
    }
    else
    {
        u8SST_Static_Core_TH    = SST_STATIC_CORE_TH_LV4_VALUE;
        u8SST_Motion_TH         = SST_MOTION_TH_LV4_VALUE;        //YY for SZ feathering 151201
        u8SST_Dynamic_Core_TH   = SST_DYNAMIC_CORE_TH_LV4_VALUE;  //YY for SZ feathering 151201
    }

    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK22_1A_L, (MS_U16)u8SST_Static_Core_TH, 0xFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK22_18_L, (MS_U16)u8SST_Motion_TH, 0x000F);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK22_19_L, (MS_U16)u8SST_Dynamic_Core_TH, 0x003F);
}

/******************************************************************************/
///DeFlickering
///@param u32MotionValue \     IN: Motion value
/******************************************************************************/
void MDrv_SC_de_flickering(MS_U32 u32MotionValue)
{
    static MS_S32 s32DeFlickerCnt = 0;
    MS_U32 reg_m_feat_smooth_hle_th = 0, reg_m_feat_smooth_shrink = 0;

    reg_m_feat_smooth_hle_th = (MS_U32)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK22_1E_L, 0xF000); // Feat Smooth HLE TH
    reg_m_feat_smooth_shrink = (MS_U32)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK22_1E_L, 0x8F);

    if(u32MotionValue >= DEFLICKERING_TH)
    {
        if(s32DeFlickerCnt < 65535)
            s32DeFlickerCnt++;
    }
    else
    {
        s32DeFlickerCnt = 0;
    }

    if(s32DeFlickerCnt >= DEFLICKERING_CNT)
    {
        reg_m_feat_smooth_hle_th += 0x0300;
        reg_m_feat_smooth_shrink += 0x10;
    }
    else
    {
        reg_m_feat_smooth_hle_th += 0x0700;
        reg_m_feat_smooth_shrink += 0x30;
    }

    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK22_1E_L, reg_m_feat_smooth_shrink, 0xFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK22_1E_L, reg_m_feat_smooth_hle_th, 0xFF00);
}

/******************************************************************************/
///DeBouncing
///@param u32MotionValue \     IN: Motion value
/******************************************************************************/
#define DEBOUNCING_GAIN 1 //0
void MDrv_SC_de_bouncing(MS_U32 u32MotionValue)
{
    static MS_S32 s32DeBouncingCnt = 0;
    MS_U32 u32RegHisWtF2 = 0;
    MS_U32 u32regdebouncingth = 0;
    int reg_debouncing_cnt = 0;

    u32regdebouncingth = 0x0A;
    reg_debouncing_cnt = 0x03;

    u32RegHisWtF2 = (MS_U32)MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK22_0A_L, 0xF8); // history ratio weighting

    if(u32MotionValue <= u32regdebouncingth * DEBOUNCING_GAIN)
    {
        s32DeBouncingCnt = 0;
    }
    else
    {
        if(s32DeBouncingCnt < 65535)
            s32DeBouncingCnt++;
    }

    if(s32DeBouncingCnt >= reg_debouncing_cnt)
    {
        u32RegHisWtF2 += 0x06; // history = 6 moving
    }
    else
    {
        u32RegHisWtF2 += 0x03; // history = 3 still
    }
    MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK22_0A_L, u32RegHisWtF2, 0xFF);
}

#define FPLL_PHASE_VAR_LIMIT    (0x10)          // If phase error variation exceed this, means un-stable
#define FPLL_PRD_THRESHOLD      (0x100)         // The PRD must be smaller than this value to indicate almost finished PRD lock
#define FPLL_PRD_VAR_LIMIT      (0x10)          // If PRD variation exceed this, means un-stable
static MS_BOOL _Hal_PQ_FrameLockCheck (void)
{
    MS_U16 u16NewPhase,u16NewPrd;
    static MS_U16 _u16OrigPhase,_u16OrigPrd;

    u16NewPrd = MApi_GFLIP_XC_R2BYTEMSK(REG_LPLL_13_L, 0xFFFF);
    u16NewPhase = MApi_GFLIP_XC_R2BYTEMSK(REG_LPLL_11_L, 0xFFFF);

    if((u16NewPrd < FPLL_PRD_THRESHOLD) && (abs(_u16OrigPrd - u16NewPrd) < FPLL_PRD_VAR_LIMIT)
        && (abs(_u16OrigPhase - u16NewPhase) < FPLL_PHASE_VAR_LIMIT))
    {
        _u16OrigPhase = u16NewPhase;
        _u16OrigPrd = u16NewPrd;
        return TRUE;
    }
    else
    {
        _u16OrigPhase = u16NewPhase;
        _u16OrigPrd = u16NewPrd;
        return FALSE;
    }
}


MS_BOOL MApi_XC_GetStatus(XC_ApiStatus *stXCStatus , MS_BOOL k)
{
    stXCStatus->stCapWin.height = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK01_06_L , 0x1FFF);
    stXCStatus->stCapWin.width  = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK01_07_L , 0x3FFF);
    
    stXCStatus->stCropWin.height = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK01_06_L , 0x1FFF); //unknow register wait David
    stXCStatus->stCropWin.width  = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK01_07_L , 0x3FFF); //unknow register wait David
    
    stXCStatus->ScaledCropWin.height = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK01_06_L , 0x1FFF); //unknow register wait David
    stXCStatus->ScaledCropWin.width  = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK01_07_L , 0x3FFF); //unknow register wait David
    
    stXCStatus->bInterlace = MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK01_1E_L , 0x0800) >> 11; 
    
    if(stXCStatus->bInterlace == 1) //i mode
        stXCStatus->u16InputVFreq = 24000000 * 10 / ( ((MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK01_1F_L , 0x1FFF)) -1)*(MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK01_20_L , 0x3FFF))); 
    else //p mode
        stXCStatus->u16InputVFreq = 12000000 * 10 / ( ((MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK01_1F_L , 0x1FFF)) -1)*(MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK01_20_L , 0x3FFF))); 
    
    stXCStatus->u16H_SizeAfterPreScaling = stXCStatus->stCapWin.width; //bcz maserati didn't have pre down scaling so = stCapWin.width
    
    return 1;
}






//===================================== function about film or dms ...etc end =============================
static MS_BOOL bFakeOutCusEnable = TRUE;

#define ENABLE_SCALER_DEFEATHERING           0x01   //BK22_7C_L
#define ENABLE_SCALER_DEFLICKERING           0x02
#define ENABLE_SCALER_DEBOUNCING             0x04
#define ENABLE_SCALER_D3D_DRIVER             0x08

#define ENABLE_SCALER_AUTO_DBK_DRIVER        0x01   //BK22_7C_H[0]
#define ENABLE_SCALER_MCDI_DRIVER            0x02   //BK22_7C_H[1]
#define ENABLE_SCALER_MCNR_DRIVER            0x04   //BK22_7C_H[2]
#define ENABLE_SCALER_SDHD_DETECT_DRIVER     0x08   //BK22_7C_H[3]

#define ENABLE_SCALER_CCS_DRIVER             0x20   //BK22_7C_H[5]
#define ENABLE_SCALER_DHD_DRIVER             0x40   //BK22_7C_H[6]
#define ENABLE_SCALER_AUTO_DBK_NMR_DRIVER    0x80   //BK22_7C_H[7]
#define ENABLE_SR_DRIVER    0x01  //BK2C30_3F_H[0]
void MHal_PQ_Adaptive_Tuning(void)
{
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_28_L , 0x1234 , 0xFFFF);

    static MS_U32 _hpfSdTolerantReport;  //MDrv_SC_hpfTolerantReport //need to add  
    static MS_U32 _hpfHdTolerantReport;  //MDrv_SC_hpfTolerantReport
    static MS_U32 _hpfHdCntReport;       //MDrv_SC_SDHD_FilterBaseReport   
    static MS_U32 _hpfSdCntReport;       //MDrv_SC_SDHD_FilterBaseReport
    static MS_U32 _hdFilterBaseReport;   //MDrv_SC_dynamicHDfilterBaseReport
    static MS_U32 hdNactCnt4 = 0;            //SDHD_U11BaseReport
    static MS_U32 _hpfHdCntReport4 = 0;      //SDHD_U11BaseReport
    static MS_U32 _hpfSdCntReport4 = 0;      //SDHD_U11BaseReport
    static MS_U32 hdNactCnt4_iir = 0;            //SDHD_U11BaseReport
    static MS_U32 _hpfHdCntReport4_iir = 0;      //SDHD_U11BaseReport
    static MS_U32 _hpfSdCntReport4_iir = 0;      //SDHD_U11BaseReport
    
    MS_U8 estNoise = 0 ;
    MS_U8 NE_confirm = 0;
    
    XC_OpFrameStates opFrameStatus;
    static XC_OpFrameStates prevOpFrameStatus;  //move to up
    MS_BOOL isOpFrameChanged;
    MS_BOOL isSceneChangeY = 0;
    MS_BOOL isSourceChange;
    MS_BOOL isSourceChangeFlag = false;
    MS_BOOL isSourceChangeForME = false;    
    MS_U8 u8UFSCCtrl3 =  MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_3F_L, 0xFFFF)>>8 ;
    
    MS_BOOL ukoSabihS_en = 0;
    MS_BOOL isUkosabihsMcSweep = false, isUkosabihsMotionSweep = false;
    MS_BOOL isUkosabihsForFilm = 0;
    //static MS_U8 u8PreviousVcnt = 0;  //need to remove if It's for check is v-sync comimg 
    //MS_U8 u8CurrentVcnt;  //need to remove if It's for check is v-sync comimg 
    MS_U8 ColorSum;
    MS_U32 u32MotionValue,u32MotionValue2,TotalFeather,TotalMvComfirm;
    MS_U16 u16MotionValue4;
    MS_U32 uComplex,FeatherSum,u32MotionValue_Sum;

    XC_ApiStatus stXCStatus;  //can't find defination later add
    memset(&stXCStatus, 0x00, sizeof(stXCStatus));

    MS_BOOL me1Still;
    MS_BOOL movingWithoutFeather;
    MS_BOOL isMcOutOfSearchRange;
    MS_U16 nonMv0Comfirm,mv0Comfirm,mvNotFound,nonMv0ComfirmME2,mv0ComfirmME2,mvNotFoundME2;
    MS_U16 isFeatherAndMotion = false;

    MS_U16 me1IsMv0, me1IsNonMv0, me1IsNotFound;
    MS_U16 me2IsMv0, me2IsNonMv0, me2IsNotFound;
    MS_BOOL isSceneChangeMc, isSceneChange = 0;
    MS_BOOL isAbihsot = false; // Toshixa 
    MS_BOOL isHBurstMoving = false, isHBurst = false;
    static MS_BOOL isMcFilm = false;
    static MS_BOOL isMcFilmHd = false;
    static MS_BOOL isMcFilmukoSabihS = false;

    MS_BOOL isMcFilmSlowMotion = true;

    MS_BOOL isL139 = FALSE, isL107 = FALSE, isL153 =FALSE, isUkosabihsMotion = FALSE, isUkosabihsMc = FALSE, isUkosabihsMv0 = FALSE;// isSWoman = FALSE;
    MS_BOOL Bouncing_status,bBouncing_Status2,Feather_sum_status,Feather_sum_status2;
    XC_FilmStates filmstatus;

    XC_Me1HisStates ME1HisStates_1,ME1HisStates_2;
    XC_GMvStates GMV;

    MS_BOOL isZoomMoving = false;
    MS_BOOL isBigFeather = false;
    MS_BOOL isSdHqvJaggedTest = false;
    static XC_HpfDefaultRegSetting hpfDefaultReg;

    MS_U32 inSearchUseFeather;
    MS_BOOL isArtifactPattern = false;
    MS_BOOL isSparsePattern = false;

    MS_BOOL isNoLumaCplx = false;

    //DBK Variables
    //****** Check VCnt******//
    
    /*u8CurrentVcnt = MDrv_SC_read_v_cnt();
    if (u8PreviousVcnt == u8CurrentVcnt)
    {
        return;
    }
    else
    {
        u8PreviousVcnt = u8CurrentVcnt;
    }*/ //need to remove if It's for check is v-sync comimg 

    //need to add
    if(MApi_XC_GetStatus(&stXCStatus, MAIN_WINDOW))  
    {
        //MDrv_SC_Edison_ME1_SetStatisticsWindow(stXCStatus.stCropWin.width, stXCStatus.stCropWin.height);
        if(stXCStatus.bInterlace)
        {
            MDrv_SC_Edison_ME1_SetStatisticsWindow(stXCStatus.ScaledCropWin.width, (stXCStatus.ScaledCropWin.height)/2);
        }
        else
        {
            MDrv_SC_Edison_ME1_SetStatisticsWindow(stXCStatus.ScaledCropWin.width, stXCStatus.ScaledCropWin.height);
        }
    }
    frameWidth  = minmax(stXCStatus.ScaledCropWin.width,  1, MaxWidth );
    frameHeight = minmax(stXCStatus.ScaledCropWin.height, 1, MaxHeight);
    
    //****** Get input resolution info******//
    if( stXCStatus.u16H_SizeAfterPreScaling /*need to add*/ >= 1280 )
    {
        _bIsHDSource= true;
    }
    else
    {
        _bIsHDSource= false;
    }
    
    //****** source change******//  
    isSourceChange = MDrv_SC_SourceChange(); //Trace the rising signal for source change
    isSourceChangeForME = (MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_07_L, 0x0002)>>1) ;//hold for 35frames   
    isSourceChangeFlag = MDrv_SC_SourceChangeFlag(isSourceChange);      //hold for 4 frames
    
    //****** save HPF original setting******/
    hpfDefaultReg = MDrv_SC_SaveHpfSetting( isSourceChangeFlag );
    
    //******Get adaptive function control******//
    MS_U8 u8Ctrl  = (MS_U8)(MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK22_7C_L, 0xFF) );
    MS_U8 u8Ctrl2 = (MS_U8)( MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK22_7C_L, 0xFF00) >> 8 );
    
    //****** Get motion value******//
    u32MotionValue = MDrv_SC_read_motion_value1();
    u32MotionValue2 = MDrv_SC_read_motion_value2();
    u16MotionValue4 = MDrv_SC_read_motion_value4(); /* Edison new arrive ( this is no-history motion */

    MDrv_SC_Agate_ME1_UC_InSearchRange_MR_Offset( &me1IsMv0, &me1IsNonMv0, &me1IsNotFound );
    MDrv_SC_Agate_ME2_UC_InSearchRange_MR_Offset( &me2IsMv0, &me2IsNonMv0, &me2IsNotFound );

    u32MotionValue_Sum = u32MotionValue2;
    XC_LumaHistogramStates lumaHistogramStates;
    lumaHistogramStates = MDrv_SC_GetLumaHistogram();
    ColorSum = MDrv_SC_A5_ColorPixelSum(); // #pixels of specified color window
    uComplex = MDrv_SC_Agate_ComplexSum();
    
    //******scene change******// 
    isSceneChangeMc = MDrv_SC_Agate_SceneChangeMc( me1IsMv0, me1IsNonMv0, me1IsNotFound, me2IsMv0, me2IsNonMv0, me2IsNotFound );
    isSceneChangeY = MDrv_SC_Agate_SceneChangeY( lumaHistogramStates );
    isSceneChange = ( isSceneChangeMc | isSceneChangeY );
    
    //****** Feather ******//
    inSearchUseFeather = MDrv_SC_McInSearchRangeUseFeather();
    TotalFeather = MDrv_SC_Agate_TotalFeather(); //clamp at 255
    FeatherSum = MDrv_SC_TotalFeather_Sum(); //no clamp
    
    //****** MV ******//
    nonMv0Comfirm = MDrv_SC_Agate_ME1_nonMv0Comfirm();
    mv0Comfirm = MDrv_SC_Agate_ME1_mv0Comfirm();
    mvNotFound = MDrv_SC_Agate_ME1_mvNotFound();
    nonMv0ComfirmME2 = MDrv_SC_Agate_ME2_nonMv0Comfirm();
    mv0ComfirmME2 = MDrv_SC_Agate_ME2_mv0Comfirm();
    mvNotFoundME2 = MDrv_SC_Agate_ME2_mvNotFound();
    
    ME1HisStates_1 = MDrv_SC_Agate_Hierarchy1();
    ME1HisStates_2 = MDrv_SC_Agate_Hierarchy2(ME1HisStates_1);
    GMV = MDrv_SC_Agate_GMV( ME1HisStates_1, ME1HisStates_2 );
    
    Bouncing_status = MDrv_SC_Bouncing_Status(u32MotionValue_Sum);
    bBouncing_Status2 = MDrv_SC_Bouncing_Status2(u16MotionValue4);
    
    //Frame Organization: Film and OPFrame
    opFrameStatus = MDrv_SC_OpFrame_Status();
    isOpFrameChanged = ( opFrameStatus.curOpFrame != prevOpFrameStatus.curOpFrame );

    filmstatus = MDrv_SC_Film_Status();
    isMcFilm    = MDrv_SC_McFilmLike( opFrameStatus, _bIsHDSource );
    isMcFilmHd  = MDrv_SC_McFilmLikeHd( opFrameStatus, _bIsHDSource );
    
    //******DI status******//
    TotalMvComfirm = MDrv_SC_Agate_TotalMvComfirm();
    me1Still = MDrv_SC_Agate_ME1Still();
    isFeatherAndMotion = MDrv_SC_Agate_FeatherAndMotion();
    movingWithoutFeather = MDrv_SC_Agate_MovingWithoutFeather();
    Feather_sum_status= MDrv_SC_TotalFeather_Sum_Status( isFeatherAndMotion, ( !_bIsHDSource ), &Feather_sum_status2 );
    
    //****** hdsd report status ******//
    MS_U8 _timeSharingID;
    static MS_U8 timingSegment = 0;
    {
        SDHD_Report_NoTimeSharing_DelayNF(&_hpfHdCntReport4,&_hpfSdCntReport4,&hdNactCnt4);
        SDHD_Report_IIR(&_hpfHdCntReport4,&_hpfSdCntReport4,&hdNactCnt4,&_hpfHdCntReport4_iir,&_hpfSdCntReport4_iir,&hdNactCnt4_iir);
        _hpfHdTolerantReport = _hpfHdCntReport4_iir;
        _hpfSdTolerantReport = _hpfSdCntReport4_iir;
        _hdFilterBaseReport  = _hpfHdCntReport4_iir;
        _hpfHdCntReport = _hpfHdCntReport4_iir;
        _hpfSdCntReport = _hpfSdCntReport4_iir;
        
    }
    /*if( (( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1D_L,0xFFFF) >>12)&0x1) != 0 )
    {
        _timeSharingID = MDrv_SC_TimeSharing( opFrameStatus, &timingSegment, isSourceChangeFlag ,isSceneChangeY);
        
        SDHD_U11BaseReport(_timeSharingID,timingSegment,&_hpfHdCntReport4,&_hpfSdCntReport4,&hdNactCnt4); //time 0 
    
        MDrv_SC_hpfTolerantReport(_timeSharingID,timingSegment,&_hpfHdTolerantReport,&_hpfSdTolerantReport); //time 2
        MDrv_SC_dynamicHDfilterBaseReport(_timeSharingID,timingSegment,&_hdFilterBaseReport); //time 1
        MDrv_SC_SDHD_FilterBaseReport(_timeSharingID,timingSegment,&_hpfHdCntReport,&_hpfSdCntReport); //time 3     
    }
    else{
        if( (( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_07_L,0xFFFF) >>8)&0x1) != 1 )
            SDHD_U11BaseReport_NoTimeSharing(&_hpfHdCntReport4,&_hpfSdCntReport4,&hdNactCnt4);
        else{
            SDHD_Report_NoTimeSharing_DelayNF(&_hpfHdCntReport4,&_hpfSdCntReport4,&hdNactCnt4);
        }

        SDHD_Report_IIR(&_hpfHdCntReport4,&_hpfSdCntReport4,&hdNactCnt4);
        
        _hpfHdTolerantReport = _hpfHdCntReport4;
        _hpfSdTolerantReport = _hpfSdCntReport4;
        _hdFilterBaseReport  = _hpfHdCntReport4;
        _hpfHdCntReport = _hpfHdCntReport4;
        _hpfSdCntReport = _hpfSdCntReport4;
        //SDHD_U11BaseReport_NoTimeSharing_DelayOneF(&timeCnt);
    }   */

#if 1               
    //****** for different picture size case ******//
    XC_ApiStatus DrvStatus;  //need to add later bcz don't know XC_ApiStatus 
    memset(&DrvStatus, 0x0, sizeof(XC_ApiStatus));
    MApi_XC_GetStatus(&DrvStatus, MAIN_WINDOW);
    if((DrvStatus.stCropWin.width)==0)
    {
        (DrvStatus.stCropWin.width)=1;
    }

     if((DrvStatus.stCropWin.height)==0)
    {
        (DrvStatus.stCropWin.height)=1;
    }

    if((DrvStatus.stCapWin.width)==0)
    {
        (DrvStatus.stCapWin.width)=1;
    }

    if((DrvStatus.stCapWin.height)==0)
    {
        (DrvStatus.stCapWin.height)=1;
    }

    normalize_factor  = ((256 * (DrvStatus.stCropWin.width) * (DrvStatus.stCropWin.height))/( (DrvStatus.stCapWin.width) * (DrvStatus.stCapWin.height)));
    
    //****** Letter Box LR******//
    XC_LetterBoxState letterBoxState = MDrv_SC_Letter_Box_Driver(  isSceneChange, isSourceChangeFlag, isOpFrameChanged );

    //******DBK status******//
    if (u8Ctrl2 & ENABLE_SCALER_AUTO_DBK_DRIVER)  //Edison_DBK_driver 0x01
    {
        if(MApi_XC_GetStatus(&stXCStatus, MAIN_WINDOW) == FALSE) //need to add later bcz don't know stXCStatus
        {
                printf("MApi_XC_GetStatus failed because of InitData wrong, please update header file and compile again \n"/*,_hdsddFilterBaseReport*/);
        }
        Mdrv_PQ_DBK(isSceneChangeY,u16MotionValue4,stXCStatus.stCropWin.width, stXCStatus.stCropWin.height, ( !_bIsHDSource )); //need to add later bcz don't know stXCStatus
    }

    if(u8Ctrl2 & ENABLE_SCALER_MCDI_DRIVER) //Edison_UCDi_driver 0x02
    {

        MDrv_SC_KFC_EODiW_Motionless(); /*2012-07-26 Author:sam.hung*/
#if HQV_JAGGY_PATTERN_ENBLE
        if(!_bIsHDSource)   //SD case
        isSdHqvJaggedTest = MDrv_SC_HQV_JaggedTestPattern( GMV );
        //printf("HQV_JAGGY_PATTERN_ENBLE == %d\n\n",HQV_JAGGY_PATTERN_ENBLE);
#endif
        MDrv_SC_Agate_SubMvPanScan( GMV, isSceneChangeMc, isSdHqvJaggedTest, u16MotionValue4 );
        MDrv_SC_EODi_Armored( GMV, mv0Comfirm , nonMv0Comfirm);

    
        //isMcFilmSlowMotion = _bIsHDSource ? MDrv_SC_McFilmLikeForSlowMotion( u32MotionValue2, GMV ) : false;

        isSparsePattern = MDrv_SC_ukoSabihS_SparsePattern( mvNotFound, mvNotFoundME2, mv0Comfirm, nonMv0Comfirm, mv0ComfirmME2, nonMv0ComfirmME2, _bIsHDSource); /*FOR VG846 sparse content pattern*/


        
#if SHIBASOKU_ENABLE

        isHBurstMoving = MDrv_SC_ukoSabihS_HBurstMoving( _hpfSdTolerantReport/*_hpfSdCntReport*/ );
        isHBurst = MDrv_SC_ukoSabihS_HBurst( _hpfSdTolerantReport/*_hpfSdCntReport*/, uComplex );       
        ukoSabihS_en = MDrv_SC_Agate_ukoSabihS(&stXCStatus, GMV,nonMv0Comfirm, filmstatus, isMcFilmukoSabihS, &isUkosabihsMotion, &isUkosabihsMc, &isUkosabihsForFilm, mv0Comfirm, &isUkosabihsMv0, _bIsHDSource);    //need to add later bcz don't know stXCStatus
        isUkosabihsMcSweep = MDrv_SC_sweep_ukoSabihS(  GMV, nonMv0Comfirm, mv0Comfirm, filmstatus, isMcFilm, uComplex, &isUkosabihsMotionSweep );       
        isUkosabihsMc = ( isUkosabihsMc & (!isBigFeather) );  /*isBigFeather for Ukosabihs scence change */
        isBigFeather = (isFeatherAndMotion > 0x100)? true   :   false;      
        isArtifactPattern = MDrv_SC_ukoSabihS_ArtifactPattern( /*_hpfHdTolerantReport*/ hdNactCnt4,_bIsHDSource); //need to tune report later 
        isNoLumaCplx =  MDrv_SC_ukoSabihS_noLumaCplx(); //=>no chroma
        isMcFilmukoSabihS   = MDrv_SC_McFilmUkoSabihS( opFrameStatus, _bIsHDSource, _hpfHdCntReport );      
#else
        isBigFeather = isBigFeather;
        isMcFilmukoSabihS = isMcFilmukoSabihS;
        isUkosabihsForFilm = isUkosabihsForFilm;
#endif

    
        isMcOutOfSearchRange = MDrv_SC_McOutOfSearchRange( mvNotFound, mv0Comfirm, nonMv0Comfirm, isFeatherAndMotion, GMV );

        if((_bIsHDSource)&& KURO_PATTERN_ENABLE == 1)   //HD case
        {
            isMcFilmSlowMotion  = MDrv_SC_McFilmLikeForSlowMotion( u32MotionValue2, GMV ); /*fot Kuro increase SST */
            //printf("McFilmLikeForSlowMotion ==1\n\n");
        }
        else
        {
        isMcFilmSlowMotion = false;
            //printf("McFilmLikeForSlowMotion ==0\n\n");
        }

    //isZoomMoving = _bIsHDSource ? false : MDrv_SC_ZoomMoving( GMV, me1IsNonMv0, me1IsNotFound, isFeatherAndMotion, u16MotionValue4 );
        if(!_bIsHDSource)   //SD case for S Tree and Flower
        {
            isZoomMoving = MDrv_SC_ZoomMoving( GMV, me1IsNonMv0, me1IsNotFound, isFeatherAndMotion, u16MotionValue4 );
        }
        else
        {
            isZoomMoving = false;
        }

#if LG_SPECIFIC_PATTERN_ENABLE
        if(!_bIsHDSource)   //SD case
        {
            isL139 = MDrv_SC_Agate_L139( TotalMvComfirm, ColorSum, uComplex, me1Still, ME1HisStates_2 );
            isL153 = MDrv_SC_Agate_L153( movingWithoutFeather, ColorSum, uComplex, ME1HisStates_2 );
            isL107 = MDrv_SC_Agate_L107( TotalMvComfirm, uComplex, GMV, u32MotionValue, TotalFeather, 2/*HDMI only*/ );
            //isSWoman = MDrv_SC_SBlueShirt(nonMv0Comfirm, mv0Comfirm, mvNotFound, mvNotFoundME2, uComplex, ME1HisStates_2, GMV );
            //isWeakMcNrFilm = filmstatus.film_act;
        isAbihsot = false;
        }
        else    //HD case
        {
            isL139 = false;
            isL153 = false;
            isL107 = false;
            //isSWoman = false;
            //isWeakMcNrFilm = false;
            isAbihsot = false;
        }
#else
        movingWithoutFeather = movingWithoutFeather;
        TotalMvComfirm = TotalMvComfirm;
        TotalFeather = TotalFeather;
#endif
    
    
    //Double Control
        MDrv_SC_KFC_VerticalMovingUdRatio_Ctrl( inSearchUseFeather, isMcFilm );
        MDrv_SC_KFC_8bitMotionGain_Ctrl( ( (isZoomMoving) & (!isSceneChangeMc)) , isMcFilm, isMcFilmSlowMotion );

        MDrv_SC_featherSstGain_Ctrl( isMcFilmSlowMotion ); /* for kuro propeller.. */
        MDrv_SC_featherSstOffset_Ctrl( opFrameStatus, isMcFilmSlowMotion, ( (isZoomMoving) & (!isSceneChangeMc) )); /* for kuro propeller.. */

        MDrv_SC_MotionLimitGain_Ctrl( isL153 );
        MDrv_SC_UCDi_Weight_Boundary_Ctrl( ME1HisStates_1 ); /*patch for Boundary flicker issue when vertical scroll*/
        MDrv_SC_MCDi_En_Ctrl( ( isNoLumaCplx | isMcFilm | isMcOutOfSearchRange | isArtifactPattern | bBouncing_Status2 | isHBurstMoving | isSceneChangeMc | isMcFilmSlowMotion ) );

        MDrv_SC_Force_Motion_Ctrl( isL139, ( isUkosabihsMotion | isUkosabihsMotionSweep ) );
        
        MDrv_SC_Force_Mcdi_Ctrl(isUkosabihsMc);
        MDrv_SC_KFC_EODiW_Ctrl( ( isUkosabihsMc | isUkosabihsMcSweep ) );
        MDrv_SC_Mcdi_EODiW_Ctrl( ( isUkosabihsMc | isUkosabihsMcSweep ), isL107 );
        MDrv_SC_KFC_Ctrl( isAbihsot );
        MDrv_SC_UCDi_OP1_HisWeight_Ctrl(Bouncing_status,Feather_sum_status,filmstatus, ColorSum, FeatherSum, _bIsHDSource);
        MDrv_SC_SPF_OP1_HisWeight_Ctrl(Bouncing_status);
        MDrv_SC_SST_Ctrl(( isNoLumaCplx | isMcFilm | isMcOutOfSearchRange | isArtifactPattern | bBouncing_Status2 | isHBurstMoving | isSceneChangeMc | isMcFilmSlowMotion ), FeatherSum, _bIsHDSource);

    }
    /*isMEReset =*/ MDrv_SC_Agate_ME_Reset(u32MotionValue2,isSceneChangeMc,isSceneChangeY, isSourceChangeForME);

    if(u8Ctrl2 & ENABLE_SCALER_MCNR_DRIVER)  //Edison_UCNR_driver 0x04
    {
        NE_confirm = MDrv_SC_GetNoiseLevel( &estNoise, &letterBoxState, &stXCStatus, stXCStatus.bInterlace, isOpFrameChanged );  //need to add 
        MDrv_SC_NR_Driver_E28( u16MotionValue4, /*blkConfidence,*/ /*_hpfHdTolerantReport,*/ &lumaHistogramStates, &filmstatus, &GMV, isUkosabihsMotion, isSceneChange, stXCStatus.bInterlace, _bIsHDSource, isSourceChangeFlag, isOpFrameChanged,&estNoise, NE_confirm); //need to add
#ifdef DBG_SCCHG_LC
    DEBUG_LC_42( isSceneChangeY, isSceneChangeMc );
#endif
    }
    
    MS_U16 u16DbgMode = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_07_L,0x000F);
    if(u16DbgMode == 1)
    {
        printk("VS0: %d %d %d %d \n",ukoSabihS_en,isUkosabihsMcSweep,isUkosabihsMotionSweep,isUkosabihsForFilm);
        printk("VS1: %d %d \n",ColorSum,u32MotionValue);
        printk("VS2: %d %d %d %d \n",u32MotionValue2,TotalFeather,TotalMvComfirm,u16MotionValue4);
        printk("VS3: %d %d %d %d \n",uComplex,FeatherSum,u32MotionValue_Sum,NE_confirm);
    }
    else if(u16DbgMode == 2)
    {
        printk("VS4: %d %d %d %d \n",me1Still,movingWithoutFeather,isMcOutOfSearchRange,estNoise);
        printk("VS5: %d %d %d %d \n",nonMv0Comfirm,mv0Comfirm,mvNotFound,nonMv0ComfirmME2);
        printk("VS6: %d %d %d %d \n",mv0ComfirmME2,mvNotFoundME2,isFeatherAndMotion,me1IsMv0);
        printk("VS7: %d %d %d %d \n",me1IsNonMv0,me1IsNotFound,me2IsMv0,me2IsNonMv0);
    }
    else if(u16DbgMode == 3)
    {
        printk("VS8: %d %d %d %d \n",me2IsNotFound,isSceneChangeMc,isSceneChangeY,isSceneChange);
        printk("VS9: %d %d %d %d \n",isAbihsot,isHBurstMoving,isHBurst,isMcFilm);
        printk("VSA: %d %d %d %d \n",isMcFilmHd,isMcFilmukoSabihS,isMcFilmSlowMotion,isL139);
        printk("VSB: %d %d %d %d \n",isL107,isL153,isUkosabihsMotion,isUkosabihsMc);
    }
    else if(u16DbgMode == 4)
    {
        printk("VSC: %d %d %d %d \n",isUkosabihsMv0,Bouncing_status,bBouncing_Status2,Feather_sum_status);
        printk("VSD: %d %d %d %d \n",Feather_sum_status2,isOpFrameChanged,hdNactCnt4,_hpfHdCntReport4);
        printk("VSE: %d %d %d %d \n",_hpfSdCntReport4,isZoomMoving,isBigFeather,isSdHqvJaggedTest);
        printk("VSF: %d %d %d %d \n",isSourceChange,isSourceChangeForME,isSourceChangeFlag,inSearchUseFeather);
        
    }
    else if(u16DbgMode == 5)
    {
        printk("VSG: %d %d %d \n",isArtifactPattern,isSparsePattern,isNoLumaCplx);
        printk("VSH: %d %d %d %d \n",stXCStatus.stCropWin.width,stXCStatus.stCropWin.height,stXCStatus.ScaledCropWin.width,stXCStatus.ScaledCropWin.height);
        printk("VSI: %d %d %d %d %d\n",stXCStatus.stCapWin.width,stXCStatus.stCapWin.height,stXCStatus.bInterlace,stXCStatus.u16InputVFreq,stXCStatus.u16H_SizeAfterPreScaling);
        printk("VSJ: %d %d %d %d \n",filmstatus.film32,filmstatus.film22,filmstatus.filmany,filmstatus.film_act);
    }
    else if(u16DbgMode == 6)
    {
        printk("VSK: %d %d %d %d \n",ME1HisStates_1.minMvXdir,ME1HisStates_1.minMvYdir,ME1HisStates_1.minMvXBin,ME1HisStates_1.minMvYBin);
        printk("VSL: %d %d %d %d \n",ME1HisStates_2.minMvXdir,ME1HisStates_2.minMvYdir,ME1HisStates_2.minMvXBin,ME1HisStates_2.minMvYBin);
        printk("VSM: %d %d %d %d \n",GMV.minMvXdir,GMV.minMvYdir,GMV.minMvXbin,GMV.minMvYbin);
        printk("VSN: %d %d %d %d %d\n",GMV.h1XYsum,GMV.h2XYsum,GMV.h1XYdiff,GMV.h2XYdiff,GMV.yMvIsMovingOdd);
    }
    else if(u16DbgMode == 7)
    {
        printk("VSO: %d %d %d %d \n",lbState.lbCntT,lbState.lbCntB,lbState.lbCntL,lbState.lbCntR);
        printk("VSP: %d %d %d %d \n",lbState.lbRatioC,lbState.lbRatioTB,lbState.lbRatioLR,lbState.lbComfirmLevel);

    }
    
    

    
    if(u8Ctrl2 & ENABLE_SCALER_SDHD_DETECT_DRIVER)  //Edison_SDHD_driver 0x08
    {
        MDrv_SC_Edision_stillFeatherDetection( u32MotionValue, isUkosabihsMotion, _hdFilterBaseReport, isHBurstMoving, isHBurst, isUkosabihsMv0, isMcFilmHd/*, isSceneChange*/, _hpfSdTolerantReport );

        static int DMS_SD2HD_Cnt = 0;
        // DMS SDtoHD mode
        MS_BOOL GMV_StillLike = (_bIsHDSource) && (GMV.minMvXdir >= 31) && (GMV.minMvXdir <= 33) && (GMV.minMvYdir >= 7) && (GMV.minMvYdir <= 9) ;
        MS_BOOL SD2HDCntLike1  = ( (_hpfHdCntReport>>7) > 2) && ( (_hpfHdCntReport>>7) < 8);
        MS_BOOL SD2HDCntLike2  = ( (_hpfSdCntReport>>3) > (_hpfHdCntReport*6) );
        // MS_BOOL DBKLike = (FrameBLKEdgIIR > 31) && (FrameBLKEdgIIR < 80);//easter 20150805
        //DEBUG_LC_Full6bit3(minmax((_hpfHdCntReport>>7), 0, 63) );
        //DEBUG_LC_Full6bit4(minmax((FrameBLKEdgIIR>>1), 0, 63) );
#if 0
        if ( GMV_StillLike )
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x7000, 0x7000);
        else
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0000, 0x7000);

        if ( SD2HDCntLike1 )
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0700, 0x0700);
        else
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0000, 0x0700);

        if ( SD2HDCntLike2 )
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0070, 0x0070);
        else
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0000, 0x0070);

        if ( DBKLike )
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0007, 0x0007);
        else
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0000, 0x0007);
#endif
        if (isSceneChange || isSourceChangeFlag)
        {
            DMS_SD2HD_Cnt = 0;
        }
        else if ( GMV_StillLike && SD2HDCntLike1 && SD2HDCntLike2)// && )DBKLike )easter 20150805
        {
            DMS_SD2HD_Cnt += 4;
        }
        else
        {
            if (!GMV_StillLike)
                DMS_SD2HD_Cnt -= 1;
            if (!SD2HDCntLike1)
                DMS_SD2HD_Cnt -= 1;
            if (!SD2HDCntLike2)
                DMS_SD2HD_Cnt -= 1;
            //if (!DBKLike) easter 20150805
            //    DMS_SD2HD_Cnt -= 1;
        }
            DMS_SD2HD_Cnt = minmax(DMS_SD2HD_Cnt, 0, 63);
            DMS_SDHDModeCheck(DMS_SD2HD_Cnt);
    }
    
    
    if(u8Ctrl2 & ENABLE_SCALER_CCS_DRIVER)  //Edison_CCS_driver 0x20
    {
        //MDrv_SC_Edison_CCS_driver(ukoSabihS_en);
        MDrv_SC_Edison_CCS_driver(ukoSabihS_en|isSparsePattern); //Add isSparsePattern for TG59 Variable Dot pattern(Pure Color pattern)
    }

    if(u8Ctrl2 & ENABLE_SCALER_DHD_DRIVER)  //Edison_DHD_driver 0x40
    {
//      u8SDHDDReport = false;
        MDrv_SC_Edison_DHD_driver(uComplex, nonMv0Comfirm, me1Still, mv0Comfirm, GMV, _hpfHdCntReport);
        MDrv_SC_Edison_DHD_Weather_driver(uComplex, nonMv0Comfirm, me1Still, mv0Comfirm );
        MDrv_SC_Edison_DHD_GSLSM_driver(uComplex, nonMv0Comfirm, me1Still, mv0Comfirm );
        MDrv_SC_DHD_CCS_MR_HisWeight_Ctrl(filmstatus);
        //MDrv_SC_Edison_DHD_Stadium_driver(isMcFilmHd,filmstatus);
    }

    if(u8Ctrl & ENABLE_SCALER_D3D_DRIVER)  //Edison_Detect_3D_Source_driver
    {
         MDrv_SC_3DfmtDetect(); //2012.7.24 mark
    }

    if (u8Ctrl & ENABLE_SCALER_DEFEATHERING)
    {
        MDrv_SC_de_feathering(u32MotionValue);  //2015.04.27 ricky.chen change back
        //MDrv_SC_de_feathering(u16MotionValue4);
    }

    if (u8Ctrl & ENABLE_SCALER_DEFLICKERING)
    {
        MDrv_SC_de_flickering(u32MotionValue);
    }

    if (u8Ctrl & ENABLE_SCALER_DEBOUNCING)
    {
        MDrv_SC_de_bouncing(u32MotionValue2);
    }

    if( (MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK0A_10_L, BIT(14)) == BIT(14))
            || (MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK0A_10_L, BIT(15)) == BIT(15)) )
    {
        if(!_Hal_PQ_FrameLockCheck()) // Make not frame LOCKED then turn off FilmMode
        {
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, BIT(1), BIT(1)); //hardware film mode DISABLE
            MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0A_21_L, BIT(1)|BIT(2), BIT(1) | BIT(2)); //software film mode DISABLE
        }
        else    // Make sure frame LOCKED then turn on FilmMode
        {
            if(bFakeOutCusEnable)
            {
                MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, 0, BIT(1)); //hardware film mode ENABLE
                MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0A_21_L, 0, BIT(1) | BIT(2)); //software film mode ENABLE
            }
            else
            {
                MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, BIT(1), BIT(1)); //hardware film mode DISABLE
                MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK0A_21_L, BIT(1)|BIT(2), BIT(1) | BIT(2)); //software film mode DISABLE
            }
        }
    }
#if 0   
#endif
    
    
#endif
    
    
    unsigned int kr_PiuTime=0;
    kr_PiuTime = Mstar_Timer1_GetMs();
   //printk(KERN_ALERT "[start time1][%u]\n", kr_PiuTime);
   
   
    
    
    /*XC_OpFrameStates opFrameStatus;
    MS_BOOL isSourceChange; //move to up
    MS_BOOL isSourceChangeFlag = false;
    MS_BOOL isSourceChangeForME = false; 
    XC_LumaHistogramStates lumaHistogramStates; 
    MS_BOOL isSceneChangeY = 0; */
    
    //lumaHistogramStates = MDrv_SC_GetLumaHistogram(); //move to up
    //isSceneChangeY = MDrv_SC_Agate_SceneChangeY( lumaHistogramStates ); //move to up
    //Decrease2DPKwhenSceneChange(isSceneChangeY);
    

    //opFrameStatus = MDrv_SC_OpFrame_Status();//move to up
    
    //****** source change****** move to up //
    /*isSourceChange = MDrv_SC_SourceChange(); //Trace the rising signal for source change
    isSourceChangeForME = (MApi_GFLIP_XC_R2BYTEMSK(REG_SC_BK30_07_L, 0x0002)>>1) ;//hold for 35frames   
    isSourceChangeFlag = MDrv_SC_SourceChangeFlag(isSourceChange);      //hold for 4 frames*/

    /*MS_U8 _timeSharingID;
    static MS_U8 timingSegment = 0;
    static MS_U8 timeCnt = 0;
    if( (( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1D_L,0xFFFF) >>12)&0x1) != 1 )
    {
        _timeSharingID = MDrv_SC_TimeSharing( opFrameStatus, &timingSegment, isSourceChangeFlag ,isSceneChangeY);
        
        SDHD_U11BaseReport(_timeSharingID,timingSegment); //time 0 
        MDrv_SC_hpfTolerantReport(_timeSharingID,timingSegment); //time 2
        //-------- Get motion value-----------//
        //MS_U32 u32MotionValue = MDrv_SC_read_motion_value1();
        MDrv_SC_dynamicHDfilterBaseReport(_timeSharingID,timingSegment); //time 1
        MDrv_SC_SDHD_FilterBaseReport(_timeSharingID,timingSegment); //time 3   
        
        timeCnt = 0;
    }
    else{
        SDHD_U11BaseReport_NoTimeSharing(&timeCnt);
        //SDHD_U11BaseReport_NoTimeSharing_DelayOneF(&timeCnt);
    }*/
    
    /*************************************** pq adaptive *************************************/
    //Noise estimation REG_FSC_BK30_2A_L
    //MS_U8 estNoise = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_2A_L,0x00FF);
    //MS_U8 NE_confirm = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_2A_L,0xFF00)>>8;
    
    MS_U8 FSC_estNoise=0;
    MS_U8 FSC_estConfirm=0;

    //NE transfer from SOC GetNoiseLevel
    FSC_estNoise = minmax(estNoise/10, 0, 15);
    FSC_estConfirm = minmax( NE_confirm, 1, 8);
    
    //MDrv_SC_GetSDHD_Report_ID0( &_hpfHdCntReport4, &_hpfSdCntReport4, &hdNactCnt4  );
    
    XC_PqWriteReg PqWriteReg;
    memset(&PqWriteReg,0x0,sizeof(XC_PqWriteReg));  
    
    //MS_U8 u8UFSCCtrl3 = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_3F_L, 0x0100)>>8; //move to up
    
    //frNNCnt2 calculation
    static int dsStdCnt = 0 ,frNNCnt2 = 0;
    static int frDsSum = 0, frFetSum = 0;
    {
        int dsStdCntTmp = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK2C_79_L,0xFFFF) + MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BKAC_79_L,0xFFFF);
        int frNNCnt2Tmp = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK72_33_L,0xFFFF) + MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BKF2_33_L,0xFFFF); // not changed with settings
        int frFetSumTmp = ( (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK72_7D_L,0xFFFF) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK72_7C_L,0xFFFF)
                    + (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BKF2_7D_L,0xFFFF) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BKF2_7C_L,0xFFFF) ) >> 1;
        int frDsSumTmp = ( (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK72_7B_L,0xFFFF) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK72_7A_L,0xFFFF)
                    + (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BKF2_7B_L,0xFFFF) << 16) + MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BKF2_7A_L,0xFFFF) ) >> 1;         
        
        dsStdCnt = ( dsStdCntTmp + dsStdCnt*3 ) >> 2;
        frNNCnt2 = ( frNNCnt2Tmp + frNNCnt2*3 ) >> 2;
        frFetSum = ( frFetSumTmp + frFetSum*3 ) >> 2;
        frDsSum = ( frDsSumTmp + frDsSum*3 ) >> 2;
    }
    
    /*static XC_OpFrameStates prevOpFrameStatus;  //move to up
    MS_BOOL isOpFrameChanged;
    isOpFrameChanged = ( opFrameStatus.curOpFrame != prevOpFrameStatus.curOpFrame );*/ 
    
    static XC_BackgroundState backgroundState;
    MDrv_UFSC_PQ_BNF_status( &backgroundState, isOpFrameChanged );
    
    // OSD info
    MS_BOOL isOSDActive = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK132_4A_L,0xFFFF)>>11;
    static MS_U8 osdW = 0;
    if( isOSDActive )
    {
        osdW = osdW < 253 ? osdW + 2 : 255;
    }
    else
    {
        osdW = osdW > 16 ? osdW - 16 : 0;
    }
    
    //menuW calculation
    int menuW, edgePatW; // for menu and dragon ball like edge pattern
    {
        // menu pattern
        menuW = max( 0, (int)backgroundState.cplxRawCnt - ((int)backgroundState.dswRawCnt * 3) );
        menuW = min( 256, menuW );
        menuW = 256 - menuW;

        int cplxW = max( 0 , (int)backgroundState.cplxRawCnt - 4096 ) >> 4;
        menuW = max( 0, menuW - cplxW );

        int fetDsW = max( 0, frFetSum - frDsSum ) >> 2;
        menuW = max( 0, menuW - fetDsW );

        int fetW = max( 0, frFetSum - 512 ) >> 2;
        menuW = max( 0, menuW - fetW );

        // OSD
        menuW = max( menuW, osdW );
    }
    {
        // edge pattern : dragon ball, AUO_0011,12
        edgePatW = (max( 0, (int)backgroundState.cplxRawCnt - ( ((int)backgroundState.dswRawCnt * 19) >> 3 )))>>2;

        edgePatW = min( 256, edgePatW );
        edgePatW = 256 - edgePatW;

        int dsFetW = max( 0, frDsSum - frFetSum );
        dsFetW = min( 256, dsFetW );
        int stdW = max( 0, dsStdCnt - 1024 ) >> 2;
        dsFetW = max( 0, dsFetW - stdW );
        
        //printk("EG: %d %d %d %d %d %d\n",backgroundState.cplxRawCnt,backgroundState.dswRawCnt,edgePatW,frDsSum,frFetSum,dsStdCnt);
        edgePatW = max( 0, edgePatW - dsFetW );

    }
    
    static MS_U32 u32SdW = 0, u32FhdW = 0;
    MS_U8 u8iirPre;
    u8iirPre = (MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_10_L,0x1F00))>>8;
    u8iirPre = minmax(u8iirPre , 0 , 32);
    
    //fhd weight calculation
    MS_U16 u16FhdReportOffset = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_17_L,0xFFFF);
    int fHdW2,fHdW3;
    {
        int shiftbitnum = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_27_L,0xFF00)>>8;
        int offset = u16FhdReportOffset<<shiftbitnum;
        int offset2 = u16FhdReportOffset<<(shiftbitnum+1);
        fHdW2 = max( _hpfHdCntReport4 - offset , 0);
        fHdW2 = minmax( fHdW2 >> shiftbitnum , 0 , 256);;

        fHdW3 = max( _hpfHdCntReport4 - offset2 , 0);
        fHdW3 = minmax( fHdW3 >> shiftbitnum , 0 , 256);;


        int hdcnt_shift = _hpfHdCntReport4>>8;
        int sdcnt_shift = _hpfSdCntReport4>>8;
        int bld_alpha;
        bld_alpha = max( sdcnt_shift - ((160*hdcnt_shift)>>6) ,0);
        bld_alpha = minmax( bld_alpha>>3 , 0 , 64 );

        int weight_bld = ( bld_alpha*fHdW3 + (64-bld_alpha)*fHdW2 )>>6;
        weight_bld = 256-weight_bld;
        fHdW2 = weight_bld;
        
        

    }
    
    //SD weight
    int sdW2;
    {
        int shiftbitnum = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_27_L,0xFFFF)&0x00FF;
        int offset = 256<<shiftbitnum;
        sdW2 = max( _hpfHdCntReport4 - offset , 0);
        sdW2 = minmax( sdW2 >> shiftbitnum , 0 , 256);
        sdW2 = 256 - sdW2;
        
    }
    
    if( (( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1D_L,0xFFFF) >>13)&0x1) != 1 )
    {
        if( isSceneChangeY )
        {
            u32FhdW = fHdW2;
            u32SdW = sdW2;
        }

    }

    fHdW2 = ( fHdW2*(32-u8iirPre) + u32FhdW*u8iirPre +16) /32;
    sdW2 = ( sdW2*(32-u8iirPre) + u32SdW*u8iirPre +16) /32; //iir
    
    //DBG mode
    short mode_sel = MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_10_L,0xFFFF)&0x000f;
    if(mode_sel == 1) //SD
    {
        sdW2 = 256;
        fHdW2 = 256;
    }
    else if(mode_sel == 2) //FHD
    {
        sdW2 = 0;
        fHdW2 = 256;
    }
    else if(mode_sel == 3) // HD
    {
        sdW2 = 0;
        fHdW2 = 0;
    }
    else if(mode_sel == 4)
    {
        edgePatW = 0;
    }
    else if(mode_sel == 5)
    {
        menuW = 0;
    }
    else if(mode_sel == 6)
    {
        menuW = 0;
        edgePatW = 0;
    }
    
    MS_U16 u16ArtiW = 0;
    short reduce_osd_w = max( edgePatW, menuW );
    
    //----------------DBG-PRINT----------------
    MS_U16 hdreport = minmax( _hpfHdCntReport4 >> 4 , 0 , 0xffff);
    MS_U16 sdreport = minmax( _hpfSdCntReport4 >> 4 , 0 , 0xffff);
    if( (( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1D_L,0xFFFF) >>11)&0x1) == 1 ){
        fHdW2 =  MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1E_L,0x01FF);
        sdW2 =  MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1F_L,0x01FF);
    }   
    
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_28_L , hdreport , 0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_29_L , menuW , 0xFFFF);
    //MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_2A_L , sdreport , 0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_2B_L , sdW2 , 0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_2C_L , fHdW2 , 0xFFFF);
    MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_18_L , edgePatW , 0xFFFF);
    
    //iir
    u32SdW = sdW2;
    u32FhdW = fHdW2;

    // SR enhancement strength
    int srEnhStr = (MApi_GFLIP_XC_R2BYTEMSK( REG_FSC_BK30_3E_L,0xFFFF)& 0x003F ); // 1.5
    
    if (u8UFSCCtrl3 & ENABLE_SR_DRIVER)
    {
        
        if( (( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1D_L,0xFFFF) >>0)&0x1) != 1 )
            MDrv_UFSC_PQ_LSE( frNNCnt2, menuW, edgePatW, srEnhStr ,sdW2 , fHdW2 ,&PqWriteReg);
        if( (( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1D_L,0xFFFF) >>1)&0x1) != 1 )
            MDrv_UFSC_PQ_NLM( &backgroundState, menuW, edgePatW, srEnhStr, sdW2 ,&PqWriteReg);
        if( (( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1D_L,0xFFFF) >>2)&0x1) != 1 )
            MDrv_UFSC_PQ_PreEnh( fHdW2, sdW2, FSC_estNoise, FSC_estConfirm, srEnhStr ,&PqWriteReg);
        if( (( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1D_L,0xFFFF) >>3)&0x1) != 1 )
            MDrv_UFSC_PQ_PostPK3x3( menuW, edgePatW, fHdW2, sdW2, srEnhStr ,&PqWriteReg,u16ArtiW);
        if( (( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1D_L,0xFFFF) >>4)&0x1) != 1 )
            MDrv_UFSC_PQ_HASC( sdW2, srEnhStr );
        if( (( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1D_L,0xFFFF) >>5)&0x1) != 1 )
            MDrv_UFSC_PQ_P2M( menuW, edgePatW, srEnhStr ,&PqWriteReg);
        //MDrv_UFSC_PQ_ANN( frNNCnt2 );
        if( (( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1D_L,0xFFFF) >>6)&0x1) != 1 )
            MDrv_UFSC_PQ_DMS_referBNF( &backgroundState );


        if( (( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1D_L,0xFFFF) >>7)&0x1) != 1 )
            MDrv_UFSC_PQ_2DPK(reduce_osd_w ,sdW2,fHdW2,srEnhStr,&PqWriteReg,u16ArtiW);
        if( (( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1D_L,0xFFFF) >>8)&0x1) != 1 )
            MDrv_UFSC_PQ_SPNN( menuW, edgePatW, srEnhStr ,sdW2 , fHdW2 ,&PqWriteReg);
        if( (( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1D_L,0xFFFF) >>9)&0x1) != 1 )
            MDrv_SC_2DPK_PQ(sdW2,fHdW2,srEnhStr,&PqWriteReg,u16ArtiW);
        if( (( MApi_GFLIP_XC_R2BYTEMSK(REG_FSC_BK30_1D_L,0xFFFF) >>10)&0x1) != 1 )
            MDrv_UFSC_PQ_BNF(sdW2);

        //MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_58_L, 0x2, 0x000F );
    }
    //else{
    //  MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK30_58_L, 0x1, 0x000F );
    //}
        
        
    MDrv_SC_UFSC_WRITE_REG_UI_SHARPNESS(&PqWriteReg , u8UFSCCtrl3 , ENABLE_SR_DRIVER);    
    
    prevOpFrameStatus = opFrameStatus;
    
    MDrv_SC_ResetSourceChange( isSourceChangeForME );
    
    //printk("END\n");
    
    //kr_PiuTime = Mstar_Timer1_GetMs();
    //printk(KERN_ALERT "[finish time][%u]\n", kr_PiuTime);
}

void MHal_PQ_Adaptive_Top(void)
{
    MHal_PQ_Adaptive_Tuning();
    
    //menuLoad Test
    /*MApi_GFLIP_XC_W2BYTEMSK(REG_FSC_BK30_06_L, 0x0043 ,0xFFF0);
    KApi_XC_MLoad_WriteCmd_NonXC(E_CLIENT_MAIN_HDR,0x1430, 0x0004,0x0042 ,0x0070);
    KApi_XC_MLoad_WriteCmd(E_CLIENT_MAIN_HDR,REG_SC_BK30_01_L, 0x4300 ,0xFF00);
    MLoad_UFSC_W2BYTEMSK(REG_FSC_BK30_04_L, 0x0042 ,0xFFF0);*/
    //KApi_XC_MLoad_Fire(E_CLIENT_MAIN_HDR, TRUE); 

}

void MHal_Run_PQ_Adaptive(void)
{
    
    MS_U16 u16IRQValue1 = MApi_GFLIP_XC_R2BYTE(REG_SC_BK00_10_L);
    if(u16IRQValue1 & BIT(4))
    {
        MHal_PQ_Adaptive_Top();
    // clear ISR
        MApi_GFLIP_XC_W2BYTEMSK(REG_SC_BK00_12_L, BIT(4), BIT(4));
    }
    
    //MHal_PQ_Adaptive_Top();
    
    /*static struct timespec NowTS;
    static struct timespec previousTS;

    getnstimeofday(&NowTS);

    long diff = NowTS.tv_sec * 1000000 + NowTS.tv_nsec/1000 - previousTS.tv_sec * 1000000 - previousTS.tv_nsec/1000;
    printk("t_diff = %d ms\n",diff);
    previousTS = NowTS;*/
    
    
    
}

#endif
