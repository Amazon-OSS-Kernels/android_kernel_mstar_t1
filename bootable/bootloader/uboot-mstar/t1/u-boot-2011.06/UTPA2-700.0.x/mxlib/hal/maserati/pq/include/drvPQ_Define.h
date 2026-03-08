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

#ifndef _DRVPQ_DEFINE_H_
#define _DRVPQ_DEFINE_H_
#include "drvPQ_Define_cus.h"
#if (defined (ANDROID))
#include <pthread.h>
#endif

#define PQ_ENABLE_DEBUG     0
#define PQ_ENABLE_CHECK     0

#define ENABLE_PQ_MLOAD         1
#define ENABLE_PQ_EX     1

#define PQ_ENABLE_VD_SAMPLING   1
#define PQ_ENABLE_PIP           1
#define PQ_ENABLE_3D_VIDEO      1
#define PQ_ENABLE_IOCTL         1

#define PQ_SKIPRULE_ENABLE      1

#define PQ_XRULE_DB_ENABLE      0
#define ESTIMATE_AVAILABLE_RATE 85
#define PQ_BW_H264_ENABLE 0
#define PQ_BW_MM_ENABLE 0
#define PQ_BW_RMVB_ENABLE 0
#define PQ_BW_G3D_ENABLE 0
#define PQ_BW_PIP_ENABLE 0 //PIP case, bw needs strong setting
#define PQ_BW_MVC4kx1k_ENABLE 0
#define PQ_BW_HDMI4kx2k_ENABLE  1
#define PQ_BW_DUALDECODE_ENABLE 0
#define PQ_BW_4K2KPIP_SUPPORT   1

#define PQ_MADI_88X_MODE        1
#define PQ_UC_CTL               1
#define PQ_MADI_DFK             1
#define PQ_VIP_CTL              1
#define PQ_VIP_OFF_MINUS16      0
#define PQ_VIP_RGBCMY_CTL       0
#define PQ_QM_MWE_CLONE_VER2    1
#define PQ_QM_3D_CLONE_ENABLE   1

#define PQ_ENABLE_MULTI_LEVEL_AUTO_NR 1

#define PQ_EN_DMS_SW_CTRL   TRUE
#define PQ_ENABLE_RFBL      TRUE//FALSE
#define PQ_EN_UCNR_OFF      TRUE
#define PQ_ENABLE_FORCE_MADI    ENABLE
#define PQ_NEW_HSD_SAMPLING_TYPE TRUE
#define PQ_EN_UCNR_3D_MADI  TRUE

#define PQ_4K2K_P2P_H_OFFSET_LIMITIOM       0
#define PQ_FRCM_CBCR_SWAP_BY_SW             1
#define PQ_SUPPORT_HDMI_4K2K_P2P            1
#define PQ_SUPPORT_DVI_4K2K                 1
#define PQ_DISABLE_BYPASS_MODE2_BY_SW
#define PQ_4K2K_PHOTO                       1
#define PQ_SUPPORT_SUB_POSTCCS              0
#define PQ_SUPPORT_SUB_DHD                  0

#define U8 MS_U8
#define code

extern MS_S32 _PQ_Mutex ;

#if defined (MSOS_TYPE_LINUX)
#if (defined (ANDROID))
pthread_mutex_t _PQ_MLoad_Mutex;
#else
extern pthread_mutex_t _PQ_MLoad_Mutex;
#endif
#endif

#if PQ_ENABLE_DEBUG
extern MS_BOOL _u16DbgSwitch;
#define PQ_DBG(x)  \
do{                \
    if (_u16DbgSwitch){ \
        x;         \
    }              \
}while(0)

#define PQ_DUMP_DBG(x) x
#define PQ_DUMP_FILTER_DBG(x) x
#else
#define PQ_DBG(x) //x
#define PQ_DUMP_DBG(x)          //x
#define PQ_DUMP_FILTER_DBG(x)   //x
#endif

#define PQ_MAP_REG(reg) (((reg)>>8)&0xFF), ((reg)&0xFF)
#define REG_ADDR_SIZE   2
#define REG_MASK_SIZE   1

#define PQ_IP_NULL          0xFF

#define PQTBL_EX            0x10
#define PQTBL_NORMAL        0x11

#define E_XRULE_HSD         0
#define E_XRULE_VSD         1
#define E_XRULE_HSP         2
#define E_XRULE_VSP         3
#define E_XRULE_CSC         4

#if PQ_XRULE_DB_ENABLE
#define E_XRULE_DB_NTSC     5
#define E_XRULE_DB_PAL      6
#define E_XRULE_NUM         7
#else
#define E_XRULE_NUM         5
#endif


#define _END_OF_TBL_        0xFFFF
#define _END_OF_BW_TBL_     0xFFFFFF
#define MaseratiLinearRGB

#define DS_PQ_MAX_NUM       200
typedef struct
{
    MS_U16 u16Addr;
    MS_U16 u16Value;
    MS_U16 u16Mask;
} stDS_PQ_REG;

typedef enum {
    PQ_TABTYPE_GENERAL,
    PQ_TABTYPE_COMB,
    PQ_TABTYPE_SCALER,
    PQ_TABTYPE_SRAM1,
    PQ_TABTYPE_SRAM2,
    PQ_TABTYPE_SRAM3,
    PQ_TABTYPE_SRAM4,
    PQ_TABTYPE_C_SRAM1,
    PQ_TABTYPE_C_SRAM2,
    PQ_TABTYPE_C_SRAM3,
    PQ_TABTYPE_C_SRAM4,
    PQ_TABTYPE_SRAM_COLOR_INDEX,
    PQ_TABTYPE_SRAM_COLOR_GAIN_SNR,
    PQ_TABTYPE_SRAM_COLOR_GAIN_DNR,
    PQ_TABTYPE_VIP_IHC_CRD_SRAM,
    PQ_TABTYPE_VIP_ICC_CRD_SRAM,
    PQ_TABTYPE_XVYCC_DE_GAMMA_SRAM,
    PQ_TABTYPE_XVYCC_GAMMA_SRAM,
    PQ_TABTYPE_SWDRIVER,
    PQ_TABTYPE_LinearRGB_DE_GAMMA_SRAM,
    PQ_TABTYPE_LinearRGB_GAMMA_SRAM,

    PQ_TABTYPE_UFSC_GENERAL = 50,
    PQ_TABTYPE_UFSC_COMB = 51,
    PQ_TABTYPE_UFSC_SCALER = 52,
    PQ_TABTYPE_UFSC_SRAM1 = 53,
    PQ_TABTYPE_UFSC_SRAM2 = 54,
    PQ_TABTYPE_UFSC_SRAM3 = 55,
    PQ_TABTYPE_UFSC_SRAM4 = 56,
    PQ_TABTYPE_UFSC_C_SRAM1 = 57,
    PQ_TABTYPE_UFSC_C_SRAM2 = 58,
    PQ_TABTYPE_UFSC_C_SRAM3 = 59,
    PQ_TABTYPE_UFSC_C_SRAM4 = 60,
    PQ_TABTYPE_UFSC_SRAM_COLOR_INDEX = 61,
    PQ_TABTYPE_UFSC_SRAM_COLOR_GAIN_SNR = 62,
    PQ_TABTYPE_UFSC_SRAM_COLOR_GAIN_DNR = 63,
    PQ_TABTYPE_UFSC_VIP_IHC_CRD_SRAM = 64,
    PQ_TABTYPE_UFSC_VIP_ICC_CRD_SRAM = 65,
    PQ_TABTYPE_UFSC_XVYCC_DE_GAMMA_SRAM = 66,
    PQ_TABTYPE_UFSC_XVYCC_GAMMA_SRAM = 67,
    PQ_TABTYPE_UFSC_SWDRIVER = 68,
    PQ_TABTYPE_UFSC_100SERIES = 69,
} EN_PQ_TABTYPE;


typedef enum
{
    PQ_MD_720x480_60I,      // 00
    PQ_MD_720x480_60P,      // 01
    PQ_MD_720x576_50I,      // 02
    PQ_MD_720x576_50P,      // 03
    PQ_MD_1280x720_50P,     // 04
    PQ_MD_1280x720_60P,     // 05
    PQ_MD_1920x1080_50I,    // 06
    PQ_MD_1920x1080_60I,    // 07
    PQ_MD_1920x1080_24P,    // 08
    PQ_MD_1920x1080_25P,    // 09
    PQ_MD_1920x1080_30P,    // 10
    PQ_MD_1920x1080_50P,    // 11
    PQ_MD_1920x1080_60P,    // 12
    PQ_MD_Num,
}PQ_MODE_INDEX;


typedef struct
{
    MS_U8 *pIPCommTable;
    MS_U8 *pIPTable;
    MS_U8 u8TabNums;
    MS_U8 u8TabType;
} EN_IPTAB_INFO;

typedef struct
{
    MS_U8 *pIPTable;
    MS_U8 u8TabNums;
    MS_U8 u8TabType;
    MS_U8 u8TabIdx;
} EN_IP_Info;

enum
{
    PQ_FUNC_DUMP_REG,
    PQ_FUNC_CHK_REG,
};

#define PQ_MUX_DEBUG    0

#define BK_UFSC_SCALER_BASE             0x140000

#define SCALER_REGISTER_SPREAD 1

#if(SCALER_REGISTER_SPREAD)
    #define BK_SCALER_BASE              0x130000

    // no need to store bank for spread reg
    #define SC_BK_STORE_NOMUTEX

    #define SC_BK_RESTORE_NOMUTEX

    #if(PQ_MUX_DEBUG)
    // no need to store bank for spread reg
    #define SC_BK_STORE_MUTEX     \
            if(!MsOS_ObtainMutex(_PQ_Mutex, MSOS_WAIT_FOREVER))                     \
            {                                                                        \
                printf("==========================\n");                              \
                printf("[%s][%s][%06d] Mutex taking timeout\n",__FILE__,__FUNCTION__,__LINE__);    \
            }

    // restore bank
    #define SC_BK_RESTORE_MUTEX   \
            MsOS_ReleaseMutex(_PQ_Mutex);
    #else
    #define SC_BK_STORE_MUTEX     \
            if(!MsOS_ObtainMutex(_PQ_Mutex, MSOS_WAIT_FOREVER))                     \
            {                                                                        \
            }

    // restore bank
    #define SC_BK_RESTORE_MUTEX   \
            MsOS_ReleaseMutex(_PQ_Mutex);
    #endif

    // switch bank
    #define SC_BK_SWITCH(_x_)

    #define SC_BK_CURRENT   (u8CurBank)
#else
    #define BK_SCALER_BASE              0x102F00

    #define SC_BK_STORE_NOMUTEX     \
            MS_U8 u8Bank;      \
            u8Bank = MApi_XC_ReadByte(BK_SCALER_BASE)

    // restore bank
    #define SC_BK_RESTORE_NOMUTEX   MApi_XC_WriteByte(BK_SCALER_BASE, u8Bank);

    #define SC_MUTEX_ENTRY     \
            if(!MsOS_ObtainMutex(_PQ_Mutex, MSOS_WAIT_FOREVER))                     \
            {                                                                        \
            }        \

    #define SC_MUTEX_RETURN   \
            MsOS_ReleaseMutex(_PQ_Mutex);

#if(PQ_MUX_DEBUG)
// store bank
    #define SC_BK_STORE_MUTEX     \
        MS_U8 u8Bank;      \
        if(!MsOS_ObtainMutex(_PQ_Mutex, MSOS_WAIT_FOREVER))                     \
        {                                                                        \
            printf("==========================\n");                              \
            printf("[%s][%s][%06d] Mutex taking timeout\n",__FILE__,__FUNCTION__,__LINE__);    \
        }        \
        u8Bank = MApi_XC_ReadByte(BK_SCALER_BASE)

// restore bank
    #define SC_BK_RESTORE_MUTEX   \
        MApi_XC_WriteByte(BK_SCALER_BASE, u8Bank); \
        MsOS_ReleaseMutex(_PQ_Mutex);
#else
    #define SC_BK_STORE_MUTEX     \
        MS_U8 u8Bank;      \
        if(!MsOS_ObtainMutex(_PQ_Mutex, MSOS_WAIT_FOREVER))                     \
        {                                                                        \
        }        \
        u8Bank = MApi_XC_ReadByte(BK_SCALER_BASE)

// restore bank
    #define SC_BK_RESTORE_MUTEX   \
        MApi_XC_WriteByte(BK_SCALER_BASE, u8Bank); \
        MsOS_ReleaseMutex(_PQ_Mutex);
#endif

// switch bank
#define SC_BK_SWITCH(_x_)\
        MApi_XC_WriteByte(BK_SCALER_BASE, _x_)

#define SC_BK_CURRENT   \
        MApi_XC_ReadByte(BK_SCALER_BASE)
#endif




// store bank
#define COMB_BK_STORE     \
        MS_U8 u8Bank;      \
        u8Bank = MApi_XC_ReadByte(COMB_REG_BASE)

// restore bank
#define COMB_BK_RESTORE   \
        MApi_XC_WriteByte(COMB_REG_BASE, u8Bank)

// switch bank
#define COMB_BK_SWITCH(_x_)\
        MApi_XC_WriteByte(COMB_REG_BASE, _x_)

#define COMB_BK_CURRENT   \
        MApi_XC_ReadByte(COMB_REG_BASE)

#define PQ_IP_COMM  0xfe
#define PQ_IP_ALL   0xff


#endif /* _DRVPQ_DEFINE_H_ */

