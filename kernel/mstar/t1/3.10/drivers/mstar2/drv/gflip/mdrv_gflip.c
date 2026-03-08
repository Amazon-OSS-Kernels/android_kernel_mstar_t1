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
/// @file   mdrv_gflip.c
/// @brief  MStar gflip Interface
/// @author MStar Semiconductor Inc.
/// @attention
/// <b><em></em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////

#define _MDRV_GFLIP_C

//=============================================================================
// Include Files
//=============================================================================
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#if defined(CONFIG_MIPS)
#elif defined(CONFIG_ARM)
#include <asm/io.h>
#endif

#include "mdrv_mstypes.h"
#include "mdrv_gflip.h"
#include "mhal_gflip.h"
#include "mhal_gflip_reg.h"
#include "mdrv_utopia2k_str_io.h"

//=============================================================================
// Compile options
//=============================================================================


//=============================================================================
// Local Defines
//=============================================================================
#define MAX_FLIP_ADDR_FIFO 	(0x10)
#define FLIP_INTERRUPT_TIMEOUT   (100)  //In MS
#define MAGIC 0x9527
#define GOP_LOGO 0

#if	( defined (CONFIG_MSTAR_VE_CAPTURE_SUPPORT))
#define VE_CAPTURE_FRAME_INVALID_NUM 0
#endif

#define MAIN_WINDOW         0
#define SUB_WINDOW          1

#define GFLIP_HZ_TIMER 0
#define GFLIP_HR_TIMER 1
#define GFLIP_TIMER_TYPE  GFLIP_HZ_TIMER

#if ( !defined (CONFIG_HIGH_RES_TIMERS) && (GFLIP_TIMER_TYPE==GFLIP_HR_TIMER))
#error "can't using high resolution timer when kernel config not support CONFIG_HIGH_RES_TIMERS"
#endif

#if (GFLIP_TIMER_TYPE == GFLIP_HR_TIMER)
#define GFLIP_TIMER_CHECK_TIME          	2 //Check every 2ms
#else
#define GFLIP_TIMER_CHECK_TIME          	1 //Check every jiffies
#endif

//=============================================================================
// Debug Macros
//=============================================================================
#define GFLIP_DEBUG
#ifdef GFLIP_DEBUG
    #define GFLIP_PRINT(fmt, args...)      pr_debug("[GFlip (Driver)][%05d] " fmt, __LINE__, ## args)
    #define GFLIP_ASSERT(_cnd, _fmt, _args...)                   \
                                    if (!(_cnd)) {              \
                                        GFLIP_PRINT(_fmt, ##_args);  \
                                    }
#else
    #define GFLIP_PRINT(_fmt, _args...)
    #define GFLIP_ASSERT(_cnd, _fmt, _args...)
#endif
#define MS_TO_NS(x) (x * 1E6L)

//=============================================================================
// Local Function Prototypes
//=============================================================================
MS_GWIN_INFO MDrv_GFLIP_GetGwinInfo(MS_U8 u8GopIdx,MS_U8 u8GwinIdx);
MS_BOOL _MDrv_GFLIP_SetFlipInfo(MS_U32 u32GopIdx, MS_U32 u32GwinIdx, MS_PHY64 u32MainAddr, MS_PHY64 u32SubAddr, MS_U32 u32TagId, MS_U32 * u32QEntry, MS_U32 *u32Result);

EXPORT_SYMBOL(_MDrv_GFLIP_SetFlipInfo);
#ifdef	GFLIP_MULTI_FLIP
EXPORT_SYMBOL(_MDrv_GFLIP_SetMultiFlipInfo);
EXPORT_SYMBOL(_MDrv_GFLIP_SetTLBMultiFlipInfo);
#endif
EXPORT_SYMBOL(_MDrv_GFLIP_SetGwinInfo);
EXPORT_SYMBOL(_MDrv_GFLIP_RestoreFromVsyncLimitation);
EXPORT_SYMBOL(_MDrv_GFLIP_GetDWinIntInfo);
#if	( defined (CONFIG_MSTAR_VE_CAPTURE_SUPPORT))
EXPORT_SYMBOL(MDrv_GFLIP_GetVECapCurState);
EXPORT_SYMBOL(MDrv_GFLIP_VECapWaitOnFrame);
#endif
EXPORT_SYMBOL(_MDrv_GFLIP_ClearFlipQueue);
EXPORT_SYMBOL(_MDrv_GFLIP_SetGPIO3DPin);
#if ( defined (CONFIG_MSTAR_NEW_FLIP_FUNCTION_ENABLE))
EXPORT_SYMBOL(MDrv_GFLIP_InitTimer);
EXPORT_SYMBOL(MDrv_GFLIP_Del_Timer);
#endif
//=============================================================================
// Macros
//=============================================================================

//=============================================================================
// Local Variables
//=============================================================================
static volatile GFLIP_INFO _GFlipInfo[MAX_GOP_SUPPORT][MAX_GOP_GWIN][MAX_FLIP_ADDR_FIFO];
static volatile MS_U32 _u32GFlipInfoReadPtr[MAX_GOP_SUPPORT][MAX_GOP_GWIN];
static volatile MS_U32 _u32GFlipInfoWritePtr[MAX_GOP_SUPPORT][MAX_GOP_GWIN];
static volatile MS_BOOL _bGFlipInVsyncLimitation[MAX_GOP_SUPPORT] = { FALSE };
static volatile MS_GWIN_INFO _u32GwinInfo[MAX_GOP_SUPPORT][MAX_GOP_GWIN];

static volatile MS_BOOL _bEnableGOPTLB[MAX_GOP_SUPPORT] = { FALSE };
static volatile MS_U32 _u32GOPTLBaddress[MAX_GOP_SUPPORT];

static GFLIP_DWININT_INFO _GFlipDWinIntInfo = { 0x0 };
static MS_U32 _u32GFlipLatestIntTicket; //The jiffies when Interrupt happen
static DECLARE_WAIT_QUEUE_HEAD(_gflip_waitqueue);
static DECLARE_WAIT_QUEUE_HEAD(_gcapture_waitqueue);
static DECLARE_WAIT_QUEUE_HEAD(_gvsync_gop0_waitqueue);
static DECLARE_WAIT_QUEUE_HEAD(_gvsync_gop1_waitqueue);
static DECLARE_WAIT_QUEUE_HEAD(_gvsync_gop2_waitqueue);
static DECLARE_WAIT_QUEUE_HEAD(_gvsync_gop3_waitqueue);
static DECLARE_WAIT_QUEUE_HEAD(_gvsync_gop4_waitqueue);
static DECLARE_WAIT_QUEUE_HEAD(_gvsync_gop5_waitqueue);

#if	( defined (CONFIG_MSTAR_VE_CAPTURE_SUPPORT))
static MS_BOOL s_bEnable = FALSE;
static MS_U8   s_u8FrameCount = VE_CAPTURE_FRAME_INVALID_NUM;
static MS_U8   s_u8FrameNumUpdated = VE_CAPTURE_FRAME_INVALID_NUM;
static MS_GFLIP_VEC_CONFIG s_stGflipVECConfig = {0, sizeof(MS_GFLIP_VEC_CONFIG), 0, 0, 3, 4, 0, MS_VEC_CONFIG_INIT}; //the VEC config setting
#endif

#if ( defined (CONFIG_MSTAR_NEW_FLIP_FUNCTION_ENABLE))
#if (GFLIP_TIMER_TYPE == GFLIP_HR_TIMER)
static struct hrtimer _stGflip_hrtimer;
#else
static struct timer_list _stGflip_timer;
#endif

#if (GFLIP_TIMER_TYPE == GFLIP_HR_TIMER)
static enum hrtimer_restart MDrv_GFLIP_Timer_Callback( struct hrtimer *timer );
#else
static void MDrv_GFLIP_Timer_Callback(unsigned long value);
#endif

#ifdef	GFLIP_MULTI_FLIP
#if (GFLIP_TIMER_TYPE == GFLIP_HR_TIMER)
enum hrtimer_restart MDrv_GFLIP_MultiGOP_Timer_Callback( struct hrtimer *timer );
#else
static void MDrv_GFLIP_MultiGOP_Timer_Callback(unsigned long value);
#endif
#endif
#endif

#if (MAX_GOP_SUPPORT < 5)
#define GFLIP_REG_BANKS (MAX_GOP_SUPPORT * GFLIP_GOP_BANKOFFSET)
#else
#define GFLIP_REG_BANKS (MAX_GOP_SUPPORT * GFLIP_GOP_BANKOFFSET + 2)
#endif
#define GFLIP_REG16_NUM_PER_BANK 128

typedef struct
{
    unsigned short BankReg[GFLIP_REG_BANKS][GFLIP_REG16_NUM_PER_BANK];
    //unsigned short GWinEnable[MAX_GOP_SUPPORT];
    unsigned short CKG_GopReg[10];
    unsigned short GS_GopReg;
    unsigned short SC_OPBlending[5];
    unsigned short SC_save[3][128];
#ifdef GOP_MIU_REG
    unsigned short MIU_GopReg[3];
#endif
#ifdef GOP_MIU_IN_SC
    unsigned short MIU_SC_GopReg[3];
#endif

#ifdef SUPPORT_GOP_DUALRATE
    unsigned short SC_GopBlending[3];
    unsigned short SC_GopHsyncShift;
#endif

#ifdef SUPPORT_GOP_AFBC
    unsigned short u16AfbcRegClk;

    unsigned short u16AfbcRegEn[GOP_AFBC_CORE_NUMBER];
    unsigned short u16AfbcRegAddrL[GOP_AFBC_CORE_NUMBER];
    unsigned short u16AfbcRegAddrH[GOP_AFBC_CORE_NUMBER];
    unsigned short u16AfbcRegFmt[GOP_AFBC_CORE_NUMBER];
    unsigned short u16AfbcRegWidth[GOP_AFBC_CORE_NUMBER];
    unsigned short u16AfbcRegHeight[GOP_AFBC_CORE_NUMBER];
    unsigned short u16AfbcRegMiu;
    unsigned short u16AfbcTrigger;
#endif

#ifdef NEED_MANUAL_CROP
    unsigned short u16GopManualCrop_PreCalDone[MANUAL_CROP_SETS];
    unsigned short u16GopManualCrop_Hstart[MANUAL_CROP_SETS];
    unsigned short u16GopManualCrop_HEnd[MANUAL_CROP_SETS];
#endif
	unsigned int suspend_check;

}GFLIP_REGS_SAVE_AREA;

static GFLIP_REGS_SAVE_AREA _gflip_regs_save={{{0},{0}}};

//=============================================================================
// Global Variables
//=============================================================================
DEFINE_MUTEX(Semutex_Gflip);
//=============================================================================
// Local Function Prototypes
//=============================================================================
static void _MDrv_GFLIP_ResetFlipInfoPtr(MS_U32 u32GopIdx);

static FP_CFD_GOP_PreProcess _fpGflip_CB_CFD_GOP_PreProcess;
static FP_CFD_GOP_PreSDR _fpGflip_CB_CFD_GOP_PreSDR;
static FP_CFD_GOP_TestMode _fpGflip_CB_CFD_GOP_TestMode;

//=============================================================================
// Local Function
//=============================================================================
//-------------------------------------------------------------------------------------------------
/// Reset Flip Info Reader/Writer ptr.
/// @param  u32GopIdx    \b IN: the idx of ptr's owner(gop)
/// @return void
/// @attention
/// <b>[OBAMA] <em></em></b>
//-------------------------------------------------------------------------------------------------
void _MDrv_GFLIP_ResetFlipInfoPtr(MS_U32 u32GopIdx)
{
    MS_U16 u16Idx;

    for(u16Idx=0; u16Idx<MAX_GOP_GWIN; u16Idx++)
    {
        _u32GFlipInfoReadPtr[u32GopIdx][u16Idx] = 0;
        _u32GFlipInfoWritePtr[u32GopIdx][u16Idx] = 0;
    }
}

//=============================================================================
// GFLIP Driver Function
//=============================================================================
#if ( defined (CONFIG_MSTAR_NEW_FLIP_FUNCTION_ENABLE))
MS_U32 MDrv_GFLIP_InitTimer(void)
{
#if (GFLIP_TIMER_TYPE == GFLIP_HR_TIMER)
    hrtimer_init( &_stGflip_hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
#else
    init_timer( &_stGflip_timer);
#endif
    return 0;
}

MS_U32 MDrv_GFLIP_Del_Timer(void)
{
    mutex_lock(&Semutex_Gflip);
#if (GFLIP_TIMER_TYPE == GFLIP_HR_TIMER)
    hrtimer_cancel( &_stGflip_hrtimer );
#else
    del_timer_sync( &_stGflip_timer);
#endif
    mutex_unlock(&Semutex_Gflip);
    return 0;
}
#endif

//-------------------------------------------------------------------------------------------------
/// GFlip Init(reset flip info ptr, interrupt enable, etc..)
/// @param  u32GopIdx                  \b IN:the idx of gop which need be initialized.
/// @return TRUE: success
/// @attention
/// <b>[OBAMA] <em></em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_GFLIP_Init(MS_U32 u32GopIdx)
{
    //reset flip Info:
    _MDrv_GFLIP_ResetFlipInfoPtr(u32GopIdx);
    //enable interrupt:
    MHal_GFLIP_IntEnable(u32GopIdx,TRUE);

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// GFlip DeInit(interrupt disable)
/// @param  u32GopIdx                  \b IN:the idx of gop which need be de-initialized.
/// @return TRUE: success
/// @attention
/// <b>[OBAMA] <em></em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_GFLIP_DeInit(MS_U32 u32GopIdx)
{
    //disable interrupt:
    mutex_lock(&Semutex_Gflip);
    MHal_GFLIP_IntEnable(u32GopIdx,FALSE);
    mutex_unlock(&Semutex_Gflip);

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Clear Irq
/// @param  void
/// @return TRUE: success
/// @attention
/// <b>[OBAMA] <em></em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_GFLIP_ClearIRQ(void)
{
    MS_U32 u32GopIdx;

    MHal_GFLIP_ClearDWINIRQ(&_GFlipDWinIntInfo);

    if ( waitqueue_active(&_gcapture_waitqueue))
    {
        wake_up(&_gcapture_waitqueue);
    }

    for(u32GopIdx=0; u32GopIdx<MAX_GOP_SUPPORT; u32GopIdx++)
    {
        if(MHal_GFLIP_IsVSyncInt(u32GopIdx) == FALSE)
        {
            continue;
        }

        MHal_GFLIP_IntEnable(u32GopIdx, FALSE);

        if(MHal_GFLIP_IsVSyncInt(u32GopIdx) == TRUE)
        {   //Wow...The Vsync Issue Happened...
            MHal_GFLIP_HandleVsyncLimitation(u32GopIdx); //different chip maybe has different handle.
            _bGFlipInVsyncLimitation[u32GopIdx] = TRUE;
        }

        MHal_GFLIP_IntEnable(u32GopIdx, TRUE);
    }

    //Record current Time Ticket:
    _u32GFlipLatestIntTicket = jiffies;
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Process Irq
/// @param  void
/// @return TRUE: success kickoff flip to gop
/// @return FALSE: no flip kickoff to gop
/// @attention
/// <b>[OBAMA] <em>
///         step1: check src of vsync. interrupt
///         step2: check if any flip request in queue with this Gop
///         step3: check if TAG back
///         step4: set flip to GOP
///         step5: set ReadPtr to next, this entry consumed!
/// </em></b>
//-------------------------------------------------------------------------------------------------
static MS_U32 g_remaplastGwinAddr = 0;
static MS_U32 g_GPIO3DPin = 0;

extern void Chip_Flush_Memory(void);
extern void MDrv_GPIO_Set_High(U8 u8IndexGPIO);
extern void MDrv_GPIO_Set_Low(U8 u8IndexGPIO);

MS_BOOL MDrv_GFLIP_ProcessIRQ(void)
{
    MS_U32 u32GopIdx = 0;
    MS_U8 u8WakeUp = FALSE;
#if ( defined (CONFIG_MSTAR_VE_CAPTURE_SUPPORT))
    static MS_BOOL bVECaptureEna = FALSE, bVEReady = FALSE;
#endif
#if ( defined (CONFIG_MSTAR_3D_LR_REPORT))
    static MS_U32 lasttime=0, thistime = 0;
    static MS_U32 uCount = 0;
#endif

#if (!(defined (CONFIG_MSTAR_NEW_FLIP_FUNCTION_ENABLE)))
    MS_U32 u32GwinIdx=0;
#endif

#if ((defined (CONFIG_MSTAR_VE_CAPTURE_SUPPORT)) || (defined (CONFIG_MSTAR_3D_LR_REPORT)))
    MS_BOOL bOPVsyncInterrupt = FALSE;
#endif

#if ( defined (CONFIG_MSTAR_NEW_FLIP_FUNCTION_ENABLE))

#else
#ifdef GFLIP_GOP_TLB
    MS_PHY64 u32TLBMainAddr=0;
    MS_PHY64 u32TLBSubAddr=0;
    MS_PHY64 u32MainAddress=0;
    MS_PHY64 u32SubAddress=0;
#endif
#endif

    for(u32GopIdx=0; u32GopIdx<MAX_GOP_SUPPORT; u32GopIdx++)
    {
        //step 1: check src of vsync. interrupt
        if(MHal_GFLIP_IsVSyncInt(u32GopIdx) == FALSE)
        {
            continue;
        }
        MDrv_GFLIP_GetVsync(u32GopIdx);
#if ( defined (CONFIG_MSTAR_NEW_FLIP_FUNCTION_ENABLE))
        {
#if ((defined (CONFIG_MSTAR_VE_CAPTURE_SUPPORT)) || (defined (CONFIG_MSTAR_3D_LR_REPORT)))
            if((bOPVsyncInterrupt == FALSE) && (GFLIP_GOP_DST_OP0 == MHal_GFLIP_GetGopDst(u32GopIdx)))
            {
                bOPVsyncInterrupt = TRUE;
            }
#endif
        }
#else //Old flip logic
#if ((defined (CONFIG_MSTAR_VE_CAPTURE_SUPPORT)) || (defined (CONFIG_MSTAR_3D_LR_REPORT)))
        if((bOPVsyncInterrupt == FALSE) && (GFLIP_GOP_DST_OP0 == MHal_GFLIP_GetGopDst(u32GopIdx)))
        {
            bOPVsyncInterrupt = TRUE;
        }
#endif

        for(u32GwinIdx=0; u32GwinIdx<MAX_GOP_GWIN; u32GwinIdx++)
        {
            //step 2:check if any flip request in queue with this Gop:
            if(_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx] == _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]) //no any flip request in queue
            {
                continue;
            }

            //step 3: if get queue, check if TAG back.
            if(MHal_GFLIP_IsTagIDBack(_GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32TagId) == FALSE)
            {
                continue;
            }

            //Has Flip Request, and Tag Returned! we need programming flip address:
            //step 4: if Tag Back: set flip to GOP.
#ifdef GFLIP_GOP_TLB
            if(_bEnableGOPTLB[u32GopIdx] == TRUE)
            {
                u32TLBMainAddr=_u32GOPTLBaddress[u32GopIdx]+(_GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32MainAddr/GOP_TLB_PAGE_SIZE)*TLB_PER_ENTRY_SIZE;
                u32TLBSubAddr=_u32GOPTLBaddress[u32GopIdx]+(_GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32SubAddr/GOP_TLB_PAGE_SIZE)*TLB_PER_ENTRY_SIZE;
                u32TLBMainAddr /= ADDRESSING_8BYTE_UNIT;
                u32TLBSubAddr /= ADDRESSING_8BYTE_UNIT;
                u32MainAddress=_GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32MainAddr % (GOP_TLB_PAGE_SIZE*PER_MIU_TLB_ENTRY_COUNT);
                u32SubAddress=_GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32SubAddr % (GOP_TLB_PAGE_SIZE*PER_MIU_TLB_ENTRY_COUNT);
                MHal_GFLIP_SetTLBFlipToGop(u32GopIdx,u32GwinIdx,u32TLBMainAddr,u32TLBSubAddr,TRUE);
                MHal_GFLIP_SetFlipToGop(u32GopIdx,u32GwinIdx,u32MainAddress,u32SubAddress,TRUE);
            }
            else
#endif
            {
                MHal_GFLIP_SetFlipToGop(
                    u32GopIdx,
                    u32GwinIdx,
                    _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32MainAddr,
                    _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32SubAddr,
                    TRUE);
            }
            //Step 5: set ReadPtr to next, this entry consumed!
            _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx] = (_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]+1)%MAX_FLIP_ADDR_FIFO;

            u8WakeUp = TRUE;

        }
#endif
    }

#if ( defined (CONFIG_MSTAR_3D_LR_REPORT))
    //[3D TV]when AP call the pixelIDAddr IOCTL, below code will draw two pixel on the screen to tell ursa
    //current LR flag of scaler
    if(bOPVsyncInterrupt)
    {
        if(g_remaplastGwinAddr || g_GPIO3DPin)
        {
            MS_U16 result = (MHal_XC_GetCurrentReadBank() & 0x01);
            if(result == 0)
            {
                if(g_remaplastGwinAddr)
                {
                   (*((volatile MS_U8*)(g_remaplastGwinAddr)))     =  0xFF;
                   (*((volatile MS_U8*)(g_remaplastGwinAddr) + 1)) =  0xFF;
                   (*((volatile MS_U8*)(g_remaplastGwinAddr) + 2)) =  0xFF;
                   (*((volatile MS_U8*)(g_remaplastGwinAddr) + 3)) =  0xFF;

                   (*((volatile MS_U8*)(g_remaplastGwinAddr) + 4)) =  0x01;
                   (*((volatile MS_U8*)(g_remaplastGwinAddr) + 5)) =  0x00;
                   (*((volatile MS_U8*)(g_remaplastGwinAddr) + 6)) =  0x00;
                   (*((volatile MS_U8*)(g_remaplastGwinAddr) + 7)) =  0xFF;

                    Chip_Flush_Memory();
                }

                if(g_GPIO3DPin)
                {
                    MDrv_GPIO_Set_High(g_GPIO3DPin);
                }

            }
            else
            {
                if(g_remaplastGwinAddr)
                {
                    (*((volatile MS_U8*)(g_remaplastGwinAddr)))     =  0x01;
                    (*((volatile MS_U8*)(g_remaplastGwinAddr) + 1)) =  0x00;
                    (*((volatile MS_U8*)(g_remaplastGwinAddr) + 2)) =  0x00;
                    (*((volatile MS_U8*)(g_remaplastGwinAddr) + 3)) =  0xFF;

                    (*((volatile MS_U8*)(g_remaplastGwinAddr) + 4)) =  0xFF;
                    (*((volatile MS_U8*)(g_remaplastGwinAddr) + 5)) =  0xFF;
                    (*((volatile MS_U8*)(g_remaplastGwinAddr) + 6)) =  0xFF;
                    (*((volatile MS_U8*)(g_remaplastGwinAddr) + 7)) =  0xFF;
                    Chip_Flush_Memory();
                }

                if(g_GPIO3DPin)
                {
                    MDrv_GPIO_Set_Low(g_GPIO3DPin);
                }
            }
            #if 1
            thistime = jiffies;
            if(thistime - lasttime >22)
            {
		GFLIP_PRINT("\n-[%s][%d]---------timeout:----jiffies=%lu, ucount=%lu\n", __func__, __LINE__, thistime - lasttime, uCount++);
            }

            lasttime = thistime;
            #endif
        }
    }
#endif
#if ( defined (CONFIG_MSTAR_VE_CAPTURE_SUPPORT))
    if((s_stGflipVECConfig.eIsrType == MS_VEC_ISR_GOP_OP) && bOPVsyncInterrupt) //Only GOP OP isr enter here now
    {
        if(s_bEnable != bVECaptureEna)
        {
            //Enable or disable VE capture
            MHal_GFLIP_VECaptureEnable(s_bEnable); //For old version VEC, enable ve in ISR

            bVECaptureEna = s_bEnable;
            bVEReady = FALSE; //Reset HW state
            if(bVECaptureEna)
            {
                s_u8FrameCount = VE_CAPTURE_FRAME_INVALID_NUM; //Valid Frame is 1~3
                s_u8FrameNumUpdated = VE_CAPTURE_FRAME_INVALID_NUM; //Valid Frame is 1~3
            }
        }
        if(bVECaptureEna)
        {
            //Valid Frame is 1~3
            if(!bVEReady)
            {
                bVEReady = MHal_GFLIP_CheckVEReady();
            }
            else
            {
                if(((s_stGflipVECConfig.bInterlace == FALSE) && (s_u8FrameCount >= s_stGflipVECConfig.u8MaxFrameNumber_P)) ||
                   ((s_stGflipVECConfig.bInterlace == TRUE)  && (s_u8FrameCount >= s_stGflipVECConfig.u8MaxFrameNumber_I))
                  )
                {
                    s_u8FrameCount = VE_CAPTURE_FRAME_INVALID_NUM+1;
                }
                else
                {
                    s_u8FrameCount++;
                }
                s_u8FrameNumUpdated = s_u8FrameCount; //Valid Frame is 1~3
            }
        }
    }
#endif

    if (u8WakeUp && waitqueue_active(&_gflip_waitqueue))
    {
        wake_up(&_gflip_waitqueue);
    }

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Restore HW Limitation -- Vsync Limitation.
/// @return TRUE: DDI call success
/// @attention
/// <b>[OBAMA] <em>The Signal of GOP Dst from IP/VE restored and we re-enable GFlip. </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_GFLIP_RestoreFromVsyncLimitation(void)
{
    MS_U32 u32GopIdx;

    for(u32GopIdx=0; u32GopIdx<MAX_GOP_SUPPORT; u32GopIdx++)
    {
        if(TRUE == _bGFlipInVsyncLimitation[u32GopIdx])
        {
            MHal_GFLIP_RestoreFromVsyncLimitation(u32GopIdx);
            _bGFlipInVsyncLimitation[u32GopIdx] = FALSE;
        }
    }

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Get DWin Interrupt Info
/// @param  pGFlipDWinIntInfo       \b OUT: the dwin interrupt info
/// @param  bResetDWinIntInfo       \b IN: reset dwin interrupt infor or not
/// @return TRUE: DDI call success
/// @attention
/// <b>[OBAMA] <em>Get DWin Interrupt Info and reset it if requested. </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_GFLIP_GetDWinIntInfo(GFLIP_DWININT_INFO *pGFlipDWinIntInfo, MS_BOOL bResetDWinIntInfo, MS_U32 u32Timeout)
{
    pGFlipDWinIntInfo->u8DWinIntInfo = _GFlipDWinIntInfo.u8DWinIntInfo;
    if(bResetDWinIntInfo)
    {
        _GFlipDWinIntInfo.u8DWinIntInfo = 0x0;
    }

    if(!pGFlipDWinIntInfo->sDwinIntInfo.bDWinIntPROG && u32Timeout > 0)
    {
        interruptible_sleep_on_timeout(&_gcapture_waitqueue, msecs_to_jiffies(u32Timeout));
    }


    return TRUE;
}

#if ( defined (CONFIG_MSTAR_NEW_FLIP_FUNCTION_ENABLE))
MS_BOOL MDrv_GFLIP_IsGOPACK(MS_U32 u32GopIdx)
{
    return MHal_GFLIP_IsGOPACK(u32GopIdx);
}

//-------------------------------------------------------------------------------------------------
/// Kick off Flip to Gop HW.
/// @param  u32GopIdx                  \b IN: The gop idx who is the flip owner
/// @param  u32GwinIdx                 \b IN: The gwin idx who is the flip owner
/// @param  u32MainAddr                \b IN: The flip address for normal 2D display
/// @param  u32SubAddr                 \b IN: The 2nd fb flip address for GOP 3D display
/// @param  bForceWriteIn              \b IN: if use force write to update register, TRUE=force write
/// @return TRUE: success
/// @return FALSE: fail
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_GFLIP_SetFlipToGop(MS_U32 u32GopIdx, MS_U32 u32GwinIdx, MS_PHY64 u32MainAddr, MS_PHY64 u32SubAddr, MS_BOOL bForceWriteIn)
{
    MS_BOOL bRet= FALSE;
#ifdef GFLIP_GOP_TLB
    MS_PHY64 u32TLBMainAddr=0;
    MS_PHY64 u32TLBSubAddr=0;
    MS_PHY64 u32MainAddress=0;
    MS_PHY64 u32SubAddress=0;
    if(_bEnableGOPTLB[u32GopIdx] == TRUE)
    {
        u32TLBMainAddr=_u32GOPTLBaddress[u32GopIdx]+(_GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32MainAddr/GOP_TLB_PAGE_SIZE)*TLB_PER_ENTRY_SIZE;
        u32TLBSubAddr=_u32GOPTLBaddress[u32GopIdx]+(_GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32SubAddr/GOP_TLB_PAGE_SIZE)*TLB_PER_ENTRY_SIZE;
        u32TLBMainAddr /= ADDRESSING_8BYTE_UNIT;
        u32TLBSubAddr /= ADDRESSING_8BYTE_UNIT;
        u32MainAddress=_GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32MainAddr % (GOP_TLB_PAGE_SIZE*PER_MIU_TLB_ENTRY_COUNT);
        u32SubAddress=_GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32SubAddr % (GOP_TLB_PAGE_SIZE*PER_MIU_TLB_ENTRY_COUNT);
        MHal_GFLIP_SetTLBFlipToGop(u32GopIdx,u32GwinIdx,u32TLBMainAddr,u32TLBSubAddr,TRUE);
        bRet=MHal_GFLIP_SetFlipToGop(u32GopIdx,u32GwinIdx,u32MainAddress,u32SubAddress,TRUE);
    }
    else
#endif
    {
        bRet=MHal_GFLIP_SetFlipToGop(u32GopIdx, u32GwinIdx, u32MainAddr, u32SubAddr, bForceWriteIn);
    }
    return bRet;
}

#if (GFLIP_TIMER_TYPE == GFLIP_HR_TIMER)
static enum hrtimer_restart MDrv_GFLIP_Timer_Callback( struct hrtimer *timer )
#else
static void MDrv_GFLIP_Timer_Callback(unsigned long value)
#endif
{
    MS_U32 u32GopIdx = 0, u32GwinIdx = 0;
    MS_BOOL bWakeUpQue = FALSE, bContinueTimer = FALSE;
#ifdef GFLIP_GOP_TLB
    MS_PHY64 u32TLBMainAddr=0;
    MS_PHY64 u32TLBSubAddr=0;
    MS_PHY64 u32MainAddress=0;
    MS_PHY64 u32SubAddress=0;
#endif

#if (GFLIP_TIMER_TYPE == GFLIP_HZ_TIMER)
    //avoid reentry
    spin_lock_irq(&spinlock_gflip);
#else
    spin_lock(&spinlock_gflip);
#endif

    for(u32GopIdx=0; u32GopIdx<MAX_GOP_SUPPORT; u32GopIdx++)
    {
        for(u32GwinIdx=0; u32GwinIdx<MAX_GOP_GWIN; u32GwinIdx++)
        {
            //step 1:check if any flip request in queue with this gwin
            if(_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx] == _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]) //no any flip request in queue
            {
                continue;
            }

            bContinueTimer= TRUE;//There is new flip to check
            //Step 2: check if old request is done, if true:set ReadPtr to next, this entry consumed!
            if(_GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].bKickOff == TRUE) //kicked off, check hw ack
            {
                if(MHal_GFLIP_IsGOPACK(u32GopIdx))
                {
                    //flip is done
                    bWakeUpQue = TRUE;
                    _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].bKickOff = FALSE;
                    _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx] = (_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]+1)%MAX_FLIP_ADDR_FIFO;
                }
            }
            else //no kick off, try kick off
            {
                //step 3: if get queue, check if TAG back.
                if(MHal_GFLIP_IsTagIDBack(_GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32TagId) == FALSE)
                {
                    continue;
                }

                //Has Flip Request, and Tag Returned! we need programming flip address:
                //step 4: if Tag Back: set flip to GOP.
#ifdef GFLIP_GOP_TLB
                if(_bEnableGOPTLB[u32GopIdx] == TRUE)
                {
                    u32TLBMainAddr=_u32GOPTLBaddress[u32GopIdx]+(_GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32MainAddr/GOP_TLB_PAGE_SIZE)*TLB_PER_ENTRY_SIZE;
                    u32TLBSubAddr=_u32GOPTLBaddress[u32GopIdx]+(_GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32SubAddr/GOP_TLB_PAGE_SIZE)*TLB_PER_ENTRY_SIZE;
                    u32TLBMainAddr /= ADDRESSING_8BYTE_UNIT;
                    u32TLBSubAddr /= ADDRESSING_8BYTE_UNIT;
                    u32MainAddress=_GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32MainAddr % (GOP_TLB_PAGE_SIZE*PER_MIU_TLB_ENTRY_COUNT);
                    u32SubAddress=_GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32SubAddr % (GOP_TLB_PAGE_SIZE*PER_MIU_TLB_ENTRY_COUNT);
                    MHal_GFLIP_SetTLBFlipToGop(u32GopIdx,u32GwinIdx,u32TLBMainAddr,u32TLBSubAddr,TRUE);
                    MHal_GFLIP_SetFlipToGop(u32GopIdx, u32GwinIdx, _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32MainAddr,
                                            _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32SubAddr, FALSE);

                }
                else
#endif
                {
                    MHal_GFLIP_SetFlipToGop(u32GopIdx, u32GwinIdx, _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32MainAddr,
                                            _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32SubAddr, FALSE);
                }
                _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].bKickOff = TRUE;
                _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32KickOffStartTime = jiffies_to_msecs(jiffies);
            }
        }
    }


#if (GFLIP_TIMER_TYPE == GFLIP_HZ_TIMER)
    spin_unlock_irq(&spinlock_gflip);
    if(bContinueTimer)
    {
        _stGflip_timer.data = u32GopIdx;
        _stGflip_timer.function = MDrv_GFLIP_Timer_Callback;
	_stGflip_timer.expires = jiffies + msecs_to_jiffies(GFLIP_TIMER_CHECK_TIME*10);
        mod_timer(&_stGflip_timer, _stGflip_timer.expires);
    }

#else
    spin_unlock(&spinlock_gflip);
#endif

    if (bWakeUpQue && waitqueue_active(&_gflip_waitqueue))
    {
        wake_up(&_gflip_waitqueue);
    }

#if (GFLIP_TIMER_TYPE == GFLIP_HR_TIMER)
    if(bContinueTimer)
    {
        hrtimer_forward_now( timer, ns_to_ktime(MS_TO_NS(GFLIP_TIMER_CHECK_TIME)));
        return HRTIMER_RESTART;
    }
    else
    {
        return HRTIMER_NORESTART;
    }
#endif
}

#ifdef	GFLIP_MULTI_FLIP
#if (GFLIP_TIMER_TYPE == GFLIP_HR_TIMER)
enum hrtimer_restart MDrv_GFLIP_MultiGOP_Timer_Callback( struct hrtimer *timer )
#else
static void MDrv_GFLIP_MultiGOP_Timer_Callback(unsigned long value)
#endif
{
    MS_U32 u32GopIdx = 0, u32GwinIdx = 0;
    MS_BOOL bWakeUpQue = FALSE, bContinueTimer = FALSE;
    MS_U8 i=0;
#ifdef GFLIP_GOP_TLB
    MS_BOOL bTLBenable = FALSE;
    MS_TLB_GFLIP_MULTIINFO* pTLBMultiFlipInfo;
#endif

#if (GFLIP_TIMER_TYPE == GFLIP_HZ_TIMER)
    //avoid reentry
    spin_lock_irq(&spinlock_gflip);
#endif

    for(u32GopIdx=0; u32GopIdx<MAX_GOP_SUPPORT; u32GopIdx++)
    {
        for(u32GwinIdx=0; u32GwinIdx<MAX_GOP_GWIN; u32GwinIdx++)
        {
            //step 1:check if any flip request in queue with this gwin
            if(_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx] == _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]) //no any flip request in queue
            {
                continue;
            }

            bContinueTimer= TRUE;//There is new flip to check
            //Step 2: check if old request is done, if true:set ReadPtr to next, this entry consumed!
            if(_GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].bKickOff == TRUE) //kicked off, check hw ack
            {
                if(MHal_GFLIP_IsGOPACK(u32GopIdx))
                {
                    //flip is done
                    bWakeUpQue = TRUE;
                    _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].bKickOff = FALSE;
                    _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx] = (_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]+1)%MAX_FLIP_ADDR_FIFO;
                }
            }
            else //no kick off, try kick off
            {
                //step 3: if get queue, check if TAG back.
                if(MHal_GFLIP_IsTagIDBack(_GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32TagId) == FALSE)
                {
                    continue;
                }

                //Has Flip Request, and Tag Returned! we need programming flip address:
                //step 4: if Tag Back: set flip to GOP.
#ifdef GFLIP_GOP_TLB
                bTLBenable = FALSE;
                pTLBMultiFlipInfo =(MS_TLB_GFLIP_MULTIINFO* ) _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].pdata;
                for( i=0 ; i < pTLBMultiFlipInfo->u8GopInfoCnt ; i++ )
                {
                    if(pTLBMultiFlipInfo->astTLBGopInfo[i].bTLBEnable == TRUE)
                    {
                       bTLBenable = TRUE;
                    }
                }
                if(bTLBenable == TRUE)
                {
                    MHal_GFLIP_SetTLBMultiFlipToGop(pTLBMultiFlipInfo,FALSE);
                }
                else
#endif
                {
                    MS_GFLIP_MULTIINFO* pMultiFlipInfo =(MS_GFLIP_MULTIINFO* ) _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].pdata;
                    MHal_GFLIP_SetMultiFlipToGop(pMultiFlipInfo,FALSE);
                }
                _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].bKickOff = TRUE;
                _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32KickOffStartTime = jiffies_to_msecs(jiffies);
            }
        }
    }

#if (GFLIP_TIMER_TYPE == GFLIP_HZ_TIMER)
    spin_unlock_irq(&spinlock_gflip);
    if(bContinueTimer)
    {
        _stGflip_timer.data = u32GopIdx;
        _stGflip_timer.function = MDrv_GFLIP_Timer_Callback;
	_stGflip_timer.expires = jiffies + msecs_to_jiffies(GFLIP_TIMER_CHECK_TIME*10);
        mod_timer(&_stGflip_timer, _stGflip_timer.expires);
    }
#endif

    if (bWakeUpQue && waitqueue_active(&_gflip_waitqueue))
    {
        wake_up(&_gflip_waitqueue);
    }

#if (GFLIP_TIMER_TYPE == GFLIP_HR_TIMER)
    if(bContinueTimer)
    {
        hrtimer_forward_now( timer, ns_to_ktime(MS_TO_NS(GFLIP_TIMER_CHECK_TIME)));
        return HRTIMER_RESTART;
    }
    else
    {
        return HRTIMER_NORESTART;
    }
#endif

}
#endif
#endif
//-------------------------------------------------------------------------------------------------
/// Set Flip Info(flip request) to GFlip
/// @param  u32GopIdx       \b IN: the gop which flip kickoff to
/// @param  u32GwinIdx       \b IN: the gwin which flip kickoff to
/// @param  u32Addr             \b IN: the flip address which will be kickoff
/// @param  u32TagId            \b IN: the TagId need be waited before flip
/// @param  u32QEntry           \b IN: the queued flips, if current queued flips >= u32QEntry, won't set this flip request to gop
/// @param  u32QEntry           \b out: the current queued flips in GFLIP
/// @param  u32Result   \b out: kickoff flip info success(TRUE) or fail(FALSE)
/// @return TRUE: DDI call success
/// @attention
/// <b>[OBAMA] <em>Flip when vsync interrupt, and the DDI used for set flip info to GFlip. </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_GFLIP_SetFlipInfo(MS_U32 u32GopIdx, MS_U32 u32GwinIdx, MS_PHY64 u32MainAddr, MS_PHY64 u32SubAddr, MS_U32 u32TagId, MS_U32 * u32QEntry, MS_U32 *u32Result)
{
    MS_U32 u32NextWritePtr;
    MS_U32 u32QCnt = 0;
    MS_U64 cur_jiffies;
    MS_BOOL bRet = TRUE;

    if ((u32GopIdx >= MAX_GOP_SUPPORT) || (u32GwinIdx >= MAX_GOP_GWIN)) //overflow
    {
        *u32Result = FALSE;
        return bRet;
    }

    if(TRUE == _bGFlipInVsyncLimitation[u32GopIdx])
    { //in handling vsync limitation status, just print the error and return true to avoid block app:
	GFLIP_PRINT("\n@@@@Error!!!! [%s][%d]no Signal for GOP Clock Source and GFlip Vsync Interrupt disabled!!!\n", __func__, __LINE__);
        _bGFlipInVsyncLimitation[u32GopIdx] = false;
    }
    mutex_lock(&Semutex_Gflip);
    u32NextWritePtr = (_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]+1) % MAX_FLIP_ADDR_FIFO;
    if(u32NextWritePtr != _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]) //can write
    {
        if(_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] >= _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx])
        {
            u32QCnt = _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] - _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
        }
        else
        {
            u32QCnt = MAX_FLIP_ADDR_FIFO + _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] - _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
        }

        if( u32QCnt >= *u32QEntry )
        {
            *u32QEntry = u32QCnt;
            *u32Result = FALSE;
            bRet = TRUE;
            goto _Release_Mutex;
        }

        _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].u32MainAddr = (u32MainAddr >> GFLIP_ADDR_ALIGN_RSHIFT);
        _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].u32SubAddr = (u32SubAddr >> GFLIP_ADDR_ALIGN_RSHIFT);
        _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].u32TagId= u32TagId;
        _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].bKickOff   = FALSE;
        _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] = u32NextWritePtr;
        _bEnableGOPTLB[u32GopIdx] =  FALSE;
        _u32GOPTLBaddress[u32GopIdx] =  NULL;
#if ( defined (CONFIG_MSTAR_NEW_FLIP_FUNCTION_ENABLE))
#if (GFLIP_TIMER_TYPE == GFLIP_HR_TIMER)
	    if(FALSE == hrtimer_active(&_stGflip_hrtimer)) //no call back timer, then add one
        {
            MDrv_GFLIP_Timer_Callback(&_stGflip_hrtimer);

            _stGflip_hrtimer.function = MDrv_GFLIP_Timer_Callback;
            hrtimer_start( &_stGflip_hrtimer, ns_to_ktime(MS_TO_NS(GFLIP_TIMER_CHECK_TIME)), HRTIMER_MODE_REL );
        }
#else
        mb();
        if(FALSE == timer_pending(&_stGflip_timer)) //no call back timer, then add one
        {
            _stGflip_timer.data = u32GopIdx;
            _stGflip_timer.function = MDrv_GFLIP_Timer_Callback;
		_stGflip_timer.expires = jiffies + msecs_to_jiffies(GFLIP_TIMER_CHECK_TIME*10);
            mod_timer(&_stGflip_timer, _stGflip_timer.expires);
        }
        MDrv_GFLIP_Timer_Callback(0);
#endif
#endif
        //Hold on when New QCnt can't match requested QCnt:
        //Fix issue if we set Flip Info and back which leads to App write to on-show window.
        cur_jiffies = jiffies;
        while(TRUE)
        {
            if(_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] >= _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx])
            {
                u32QCnt = _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] - _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
            }
            else
            {
                u32QCnt = MAX_FLIP_ADDR_FIFO + _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] - _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
            }

            if( u32QCnt < *u32QEntry )
            {
                break;
            }

            if(time_after(jiffies, (msecs_to_jiffies(FLIP_INTERRUPT_TIMEOUT)+cur_jiffies)))
            {
                break;//Time out skip
            }
            interruptible_sleep_on_timeout(&_gflip_waitqueue, msecs_to_jiffies(FLIP_INTERRUPT_TIMEOUT));
        }

        //since do not do the sync. with mutex/spinlock, the return of avaiable queue number maybe not accurate.
        *u32QEntry = u32QCnt + 1;
        *u32Result = TRUE;
        bRet = TRUE;
    }
    else
    {
        *u32QEntry = MAX_FLIP_ADDR_FIFO-1;
        *u32Result = FALSE;
        bRet = TRUE;
    }
_Release_Mutex:
    mutex_unlock(&Semutex_Gflip);
    return bRet;
}
//-------------------------------------------------------------------------------------------------
/// Set Flip Info(flip request) to GFlip
/// @param  u32GopIdx       \b IN: the gop which flip kickoff to
/// @param  u32GwinIdx       \b IN: the gwin which flip kickoff to
/// @param  u32Addr             \b IN: the flip address which will be kickoff
/// @param  u32TagId            \b IN: the TagId need be waited before flip
/// @param  u32QEntry           \b IN: the queued flips, if current queued flips >= u32QEntry, won't set this flip request to gop
/// @param  u32QEntry           \b out: the current queued flips in GFLIP
/// @param  u32Result   \b out: kickoff flip info success(TRUE) or fail(FALSE)
/// @param  u32Addr             \b IN: TLB base address
/// @param  u32Addr             \b IN: TLB enable
/// @return TRUE: DDI call success
/// @attention
/// <b>[OBAMA] <em>Flip when vsync interrupt, and the DDI used for set flip info to GFlip. </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_GFLIP_SetTLBFlipInfo(MS_U32 u32GopIdx, MS_U32 u32GwinIdx, MS_PHY64 u32MainAddr, MS_PHY64 u32SubAddr, MS_U32 u32TagId, MS_U32 * u32QEntry, MS_U32 *u32Result, MS_BOOL bTLBenable, MS_PHY64 u32TLBAddr)
{
    MS_U32 u32NextWritePtr;
    MS_U32 u32QCnt = 0;
    MS_U64 cur_jiffies;
    MS_BOOL bRet = TRUE;

    if ((u32GopIdx >= MAX_GOP_SUPPORT) || (u32GwinIdx >= MAX_GOP_GWIN)) //overflow
    {
        *u32Result = FALSE;
        return bRet;
    }

    if(TRUE == _bGFlipInVsyncLimitation[u32GopIdx])
    { //in handling vsync limitation status, just print the error and return true to avoid block app:
	GFLIP_PRINT("\n@@@@Error!!!![%s][%d] no Signal for GOP Clock Source and GFlip Vsync Interrupt disabled!!!\n", __func__, __LINE__);
        _bGFlipInVsyncLimitation[u32GopIdx] = false;
    }
    mutex_lock(&Semutex_Gflip);
    u32NextWritePtr = (_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]+1) % MAX_FLIP_ADDR_FIFO;
    if(u32NextWritePtr != _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]) //can write
    {
        if(_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] >= _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx])
        {
            u32QCnt = _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] - _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
        }
        else
        {
            u32QCnt = MAX_FLIP_ADDR_FIFO + _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] - _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
        }

        if( u32QCnt >= *u32QEntry )
        {
            *u32QEntry = u32QCnt;
            *u32Result = FALSE;
            bRet = TRUE;
            goto _Release_Mutex;
        }

        _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].u32MainAddr = (u32MainAddr >> GFLIP_ADDR_ALIGN_RSHIFT);
        _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].u32SubAddr = (u32SubAddr >> GFLIP_ADDR_ALIGN_RSHIFT);
        _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].u32TagId= u32TagId;
        _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].bKickOff   = FALSE;
        _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] = u32NextWritePtr;
        _bEnableGOPTLB[u32GopIdx] =  bTLBenable;
        _u32GOPTLBaddress[u32GopIdx] =  u32TLBAddr;
#if ( defined (CONFIG_MSTAR_NEW_FLIP_FUNCTION_ENABLE))
#if (GFLIP_TIMER_TYPE == GFLIP_HR_TIMER)
        enum hrtimer_restart (*timer_cb)(struct hrtimer *);

#if ( defined (GFLIP_MULTI_FLIP))
        timer_cb = MDrv_GFLIP_MultiGOP_Timer_Callback;
#else
        timer_cb = MDrv_GFLIP_Timer_Callback;
#endif
        if(FALSE == hrtimer_active(&_stGflip_hrtimer)) //no call back timer, then add one
        {
            timer_cb(&_stGflip_hrtimer);

            _stGflip_hrtimer.function = timer_cb;
            hrtimer_start( &_stGflip_hrtimer, ns_to_ktime(MS_TO_NS(GFLIP_TIMER_CHECK_TIME)), HRTIMER_MODE_REL );
        }
#else
        mb();
        if(FALSE == timer_pending(&_stGflip_timer)) //no call back timer, then add one
        {
            _stGflip_timer.data = u32GopIdx;
            _stGflip_timer.function = MDrv_GFLIP_Timer_Callback;
		_stGflip_timer.expires = jiffies + msecs_to_jiffies(GFLIP_TIMER_CHECK_TIME*10);
            mod_timer(&_stGflip_timer, _stGflip_timer.expires);
        }
        MDrv_GFLIP_Timer_Callback(0);
#endif
#endif
        //Hold on when New QCnt can't match requested QCnt:
        //Fix issue if we set Flip Info and back which leads to App write to on-show window.
        cur_jiffies = jiffies;
        while(TRUE)
        {
            if(_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] >= _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx])
            {
                u32QCnt = _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] - _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
            }
            else
            {
                u32QCnt = MAX_FLIP_ADDR_FIFO + _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] - _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
            }

            if( u32QCnt < *u32QEntry )
            {
                break;
            }

            if(time_after(jiffies, (msecs_to_jiffies(FLIP_INTERRUPT_TIMEOUT)+cur_jiffies)))
            {
                break;//Time out skip
            }
            interruptible_sleep_on_timeout(&_gflip_waitqueue, msecs_to_jiffies(FLIP_INTERRUPT_TIMEOUT));
        }

        //since do not do the sync. with mutex/spinlock, the return of avaiable queue number maybe not accurate.
        *u32QEntry = u32QCnt + 1;
        *u32Result = TRUE;
        bRet = TRUE;
    }
    else
    {
        *u32QEntry = MAX_FLIP_ADDR_FIFO-1;
        *u32Result = FALSE;
        bRet = TRUE;
    }
_Release_Mutex:
    mutex_unlock(&Semutex_Gflip);
    return bRet;
}

#ifdef	GFLIP_MULTI_FLIP
MS_BOOL _MDrv_GFLIP_SetMultiFlipInfo(MS_GFLIP_MULTIINFO* pMultiFlipInfo)
{
    MS_U32 u32NextWritePtr;
    MS_U32 u32QCnt = 0;
    MS_U64 cur_jiffies;
    MS_BOOL bRet = TRUE;
    MS_U32 u32GopIdx = 0,u32GwinIdx = 0,u32TagId = 0;
    MS_U32 *u32QEntry = NULL,*u32Result = NULL;
    MS_PHY64 u32MainAddr = 0,u32SubAddr = 0;

    u32GopIdx = pMultiFlipInfo->astGopInfo[0].u32GopIdx;
    u32GwinIdx = pMultiFlipInfo->astGopInfo[0].u32GwinIdx;
    u32TagId = pMultiFlipInfo->astGopInfo[0].u32TagId;
    u32QEntry = &(pMultiFlipInfo->astGopInfo[0].u32QEntry);
    u32Result = &(pMultiFlipInfo->astGopInfo[0].u32Result);
    u32MainAddr = pMultiFlipInfo->astGopInfo[0].u32MainAddr;
    u32SubAddr = pMultiFlipInfo->astGopInfo[0].u32SubAddr;

    if ((u32GopIdx >= MAX_GOP_SUPPORT) || (u32GwinIdx >= MAX_GOP_GWIN)) //overflow
    {
        *u32Result = FALSE;
	GFLIP_PRINT("\33[0;36m   %s:%d    \33[m\n", __func__, __LINE__);
        return bRet;
    }

    if(TRUE == _bGFlipInVsyncLimitation[u32GopIdx])
    { //in handling vsync limitation status, just print the error and return true to avoid block app:
	GFLIP_PRINT("\n@@@@Error!!!! [%s:%d], no Signal for GOP Clock Source and GFlip Vsync Interrupt disabled!!!\n", __func__, __LINE__);
        _bGFlipInVsyncLimitation[u32GopIdx] = false;
    }
    mutex_lock(&Semutex_Gflip);
    u32NextWritePtr = (_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]+1) % MAX_FLIP_ADDR_FIFO;
    if(u32NextWritePtr != _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]) //can write
    {
        if(_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] >= _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx])
        {
            u32QCnt = _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] - _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
        }
        else
        {
            u32QCnt = MAX_FLIP_ADDR_FIFO + _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] - _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
        }

        if( u32QCnt >= *u32QEntry )
        {
            *u32QEntry = u32QCnt;
            *u32Result = FALSE;
            bRet = TRUE;
            goto _Release_Mutex;
        }

        _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].u32MainAddr = (u32MainAddr >> GFLIP_ADDR_ALIGN_RSHIFT) ;
        _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].u32SubAddr = (u32SubAddr >> GFLIP_ADDR_ALIGN_RSHIFT);;
        _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].u32TagId= u32TagId;
        _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].bKickOff   = FALSE;
        _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].pdata = (MS_U32* )pMultiFlipInfo;
        _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] = u32NextWritePtr;
        _bEnableGOPTLB[u32GopIdx] =  FALSE;
        _u32GOPTLBaddress[u32GopIdx] =  0;
#if ( defined (CONFIG_MSTAR_NEW_FLIP_FUNCTION_ENABLE))
#if (GFLIP_TIMER_TYPE == GFLIP_HR_TIMER)
        if(FALSE == hrtimer_active(&_stGflip_hrtimer)) //no call back timer, then add one
        {
            MDrv_GFLIP_MultiGOP_Timer_Callback(&_stGflip_hrtimer);
            _stGflip_hrtimer.function = MDrv_GFLIP_MultiGOP_Timer_Callback;
            hrtimer_start( &_stGflip_hrtimer, ns_to_ktime(MS_TO_NS(GFLIP_TIMER_CHECK_TIME)), HRTIMER_MODE_REL );
        }
#else
        mb();
        if(FALSE == timer_pending(&_stGflip_timer)) //no call back timer, then add one
        {
            _stGflip_timer.data = u32GopIdx;
            _stGflip_timer.function = MDrv_GFLIP_MultiGOP_Timer_Callback;
		_stGflip_timer.expires = jiffies + msecs_to_jiffies(GFLIP_TIMER_CHECK_TIME*10);
            mod_timer(&_stGflip_timer, _stGflip_timer.expires);
        }
        MDrv_GFLIP_MultiGOP_Timer_Callback(0);
#endif
#endif
        //Hold on when New QCnt can't match requested QCnt:
        //Fix issue if we set Flip Info and back which leads to App write to on-show window.
        cur_jiffies = jiffies;
        while(TRUE)
        {
            if(_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] >= _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx])
            {
                u32QCnt = _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] - _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
            }
            else
            {
                u32QCnt = MAX_FLIP_ADDR_FIFO + _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] - _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
            }

            if( u32QCnt < *u32QEntry )
            {
                break;
            }

            if(time_after(jiffies, (msecs_to_jiffies(FLIP_INTERRUPT_TIMEOUT)+cur_jiffies)))
            {
                break;//Time out skip
            }
            interruptible_sleep_on_timeout(&_gflip_waitqueue, msecs_to_jiffies(FLIP_INTERRUPT_TIMEOUT));
        }

        //since do not do the sync. with mutex/spinlock, the return of avaiable queue number maybe not accurate.
        *u32QEntry = u32QCnt + 1;
        *u32Result = TRUE;
        bRet = TRUE;
    }
    else
    {
        *u32QEntry = MAX_FLIP_ADDR_FIFO-1;
        *u32Result = FALSE;
        bRet = TRUE;
    }
_Release_Mutex:
    mutex_unlock(&Semutex_Gflip);
    return bRet;
}

MS_BOOL _MDrv_GFLIP_SetTLBMultiFlipInfo(MS_TLB_GFLIP_MULTIINFO* pTLBMultiFlipInfo)
{
    MS_U32 u32NextWritePtr;
    MS_U32 u32QCnt = 0;
    MS_U64 cur_jiffies;
    MS_BOOL bRet = TRUE;
    MS_U32 u32GopIdx = 0,u32GwinIdx = 0,u32TagId = 0;
    MS_U32 *u32QEntry = NULL,*u32Result = NULL;
    MS_PHY64 u32MainAddr = 0,u32SubAddr = 0;
    MS_BOOL bTLBenable = FALSE;
    MS_PHY64 u32TLBAddr = 0;

    u32GopIdx = pTLBMultiFlipInfo->astTLBGopInfo[0].u32GopIdx;
    u32GwinIdx = pTLBMultiFlipInfo->astTLBGopInfo[0].u32GwinIdx;
    u32TagId = pTLBMultiFlipInfo->astTLBGopInfo[0].u32TagId;
    u32QEntry = &(pTLBMultiFlipInfo->astTLBGopInfo[0].u32QEntry);
    u32Result = &(pTLBMultiFlipInfo->astTLBGopInfo[0].u32Result);
    u32MainAddr = pTLBMultiFlipInfo->astTLBGopInfo[0].u32MainAddr;
    u32SubAddr = pTLBMultiFlipInfo->astTLBGopInfo[0].u32SubAddr;
    bTLBenable = pTLBMultiFlipInfo->astTLBGopInfo[0].bTLBEnable;
    u32TLBAddr = pTLBMultiFlipInfo->astTLBGopInfo[0].u32TLBAddr;

    if ((u32GopIdx >= MAX_GOP_SUPPORT) || (u32GwinIdx >= MAX_GOP_GWIN)) //overflow
    {
        *u32Result = FALSE;
	GFLIP_PRINT("\33[0;36m   %s:%d    \33[m\n", __func__, __LINE__);
        return bRet;
    }

    if(TRUE == _bGFlipInVsyncLimitation[u32GopIdx])
    { //in handling vsync limitation status, just print the error and return true to avoid block app:
	GFLIP_PRINT("\n@@@@Error!!!! [%s][%d], no Signal for GOP Clock Source and GFlip Vsync Interrupt disabled!!!\n", __func__, __LINE__);
        _bGFlipInVsyncLimitation[u32GopIdx] = false;
    }
    mutex_lock(&Semutex_Gflip);
    u32NextWritePtr = (_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]+1) % MAX_FLIP_ADDR_FIFO;
    if(u32NextWritePtr != _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]) //can write
    {
        if(_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] >= _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx])
        {
            u32QCnt = _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] - _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
        }
        else
        {
            u32QCnt = MAX_FLIP_ADDR_FIFO + _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] - _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
        }

        if( u32QCnt >= *u32QEntry )
        {
            *u32QEntry = u32QCnt;
            *u32Result = FALSE;
            bRet = TRUE;
            goto _Release_Mutex;
        }

        _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].u32MainAddr = (u32MainAddr >> GFLIP_ADDR_ALIGN_RSHIFT) ;
        _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].u32SubAddr = (u32SubAddr >> GFLIP_ADDR_ALIGN_RSHIFT);;
        _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].u32TagId= u32TagId;
        _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].bKickOff   = FALSE;
        _GFlipInfo[u32GopIdx][u32GwinIdx][_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].pdata = (MS_U32* )pTLBMultiFlipInfo;
        _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] = u32NextWritePtr;
        _bEnableGOPTLB[u32GopIdx] =  bTLBenable;
        _u32GOPTLBaddress[u32GopIdx] =  u32TLBAddr;
#if ( defined (CONFIG_MSTAR_NEW_FLIP_FUNCTION_ENABLE))
#if (GFLIP_TIMER_TYPE == GFLIP_HR_TIMER)
        MDrv_GFLIP_MultiGOP_Timer_Callback(&_stGflip_hrtimer);
        if(FALSE == hrtimer_active(&_stGflip_hrtimer)) //no call back timer, then add one
        {
            _stGflip_hrtimer.function = MDrv_GFLIP_MultiGOP_Timer_Callback;
            hrtimer_start( &_stGflip_hrtimer, ns_to_ktime(MS_TO_NS(GFLIP_TIMER_CHECK_TIME)), HRTIMER_MODE_REL );
        }
#else
        mb();
        if(FALSE == timer_pending(&_stGflip_timer)) //no call back timer, then add one
        {
            _stGflip_timer.data = u32GopIdx;
            _stGflip_timer.function = MDrv_GFLIP_MultiGOP_Timer_Callback;
		_stGflip_timer.expires = jiffies + msecs_to_jiffies(GFLIP_TIMER_CHECK_TIME*10);
            mod_timer(&_stGflip_timer, _stGflip_timer.expires);
        }
        MDrv_GFLIP_MultiGOP_Timer_Callback(0);
#endif
#endif
        //Hold on when New QCnt can't match requested QCnt:
        //Fix issue if we set Flip Info and back which leads to App write to on-show window.
        cur_jiffies = jiffies;
        while(TRUE)
        {
            if(_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] >= _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx])
            {
                u32QCnt = _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] - _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
            }
            else
            {
                u32QCnt = MAX_FLIP_ADDR_FIFO + _u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] - _u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
            }

            if( u32QCnt < *u32QEntry )
            {
                break;
            }

            if(time_after(jiffies, (msecs_to_jiffies(FLIP_INTERRUPT_TIMEOUT)+cur_jiffies)))
            {
                break;//Time out skip
            }
            interruptible_sleep_on_timeout(&_gflip_waitqueue, msecs_to_jiffies(FLIP_INTERRUPT_TIMEOUT));
        }

        //since do not do the sync. with mutex/spinlock, the return of avaiable queue number maybe not accurate.
        *u32QEntry = u32QCnt + 1;
        *u32Result = TRUE;
        bRet = TRUE;
    }
    else
    {
        *u32QEntry = MAX_FLIP_ADDR_FIFO-1;
        *u32Result = FALSE;
        bRet = TRUE;
    }
_Release_Mutex:
    mutex_unlock(&Semutex_Gflip);
    return bRet;
}
#endif
MS_BOOL MDrv_GFLIP_SetPixelIDAddr(MS_U32 u32GopIdx, MS_U32 u32GwinIdx, MS_PHY64 u32Addr, MS_U32 u32TagId, MS_U32 * u32QEntry, MS_U32 *u32Result)
{
    if ((u32GopIdx >= MAX_GOP_SUPPORT) || (u32GwinIdx >= MAX_GOP_GWIN)) //overflow
    {
        *u32Result = FALSE;
        return TRUE;
    }

    *u32Result = TRUE;
    if(u32Addr != 0)
    {
#if defined(CONFIG_MIPS)
        g_remaplastGwinAddr = (MS_U32)__ioremap(u32Addr + 0, 100, _CACHE_UNCACHED);
#elif defined(CONFIG_ARM)
        g_remaplastGwinAddr = (MS_U32) ioremap(u32Addr + 0, 100);
#endif
    }
    else
    {
        g_remaplastGwinAddr = 0;
    }

    return TRUE;
}

MS_BOOL _MDrv_GFLIP_SetGPIO3DPin(MS_PHY64 u32Addr, MS_U32 *u32Result)
{
    *u32Result = TRUE;

    g_GPIO3DPin = u32Addr;

    return TRUE;
}

#if	( defined (CONFIG_MSTAR_VE_CAPTURE_SUPPORT))
//-------------------------------------------------------------------------------------------------
/// Get VE capture state: Enable state and the FrameNumber that has been full captured
/// @param  pbEna                  \b OUT: TRUE: Enable, FALSE: Disable
/// @param  pu8FramCount           \b OUT: Current captured FrameNumber
/// @return TRUE: success
/// @return FALSE: fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_GFLIP_GetVECapCurState(MS_BOOL *pbEna, MS_U8 *pu8FramCount)
{
    *pbEna = s_bEnable;
    *pu8FramCount = s_u8FrameCount;
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Wait on the finish of specified frame: Check if the frame is captured,
/// if Yes, return TRUE, otherwise sleep until the next Vsync ISR
/// @param  pbEna                  \b OUT: TRUE: Enable, FALSE: Disable
/// @param  pu8FramCount           \b IN : Specified frame number to waiting
/// @return TRUE: success
/// @return FALSE: fail
//-------------------------------------------------------------------------------------------------
#define TIMEOUT_SLEEP 60
MS_BOOL MDrv_GFLIP_VECapWaitOnFrame(MS_BOOL *pbEna, MS_U8 *pu8FramNum)
{
    MS_BOOL bret = FALSE;
    *pbEna = s_bEnable;
    if(s_bEnable == TRUE)
    {
        if(((s_stGflipVECConfig.bInterlace == FALSE) && (s_u8FrameNumUpdated == *pu8FramNum)) ||
           ((s_stGflipVECConfig.bInterlace == TRUE ) && (s_u8FrameNumUpdated == (*pu8FramNum<<1)))
          )
        {
            bret = TRUE;//State change, return TRUE
            s_u8FrameNumUpdated = VE_CAPTURE_FRAME_INVALID_NUM; //reset to wait next update frame
	}
        else
        {
            interruptible_sleep_on_timeout(&_gcapture_waitqueue, msecs_to_jiffies(TIMEOUT_SLEEP));
        }
    }
    else
    {
        interruptible_sleep_on_timeout(&_gcapture_waitqueue, msecs_to_jiffies(TIMEOUT_SLEEP));
    }
    return bret;
}

//-------------------------------------------------------------------------------------------------
/// Set VE capture state: Enable or Disable VE capture function
/// @param  pbEna                  \b IN: TRUE: Enable, FALSE: Disable
/// @return TRUE: success
/// @return FALSE: fail
/// @attention
///           the enable or disable operation will be taken in the following vsync interrupt process.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_GFLIP_SetVECapCurState(MS_BOOL *pbEna)
{
	s_bEnable = *pbEna;
    s_u8FrameCount = VE_CAPTURE_FRAME_INVALID_NUM;
    s_u8FrameNumUpdated = VE_CAPTURE_FRAME_INVALID_NUM; //Valid Frame is 1~3 for P mode or 1~4 for interlace mode
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Config the VE capture
/// otherwise sleep until the next Vsync ISR
/// @param  pstGflipVECConfig      \b IN : @ref PMS_GFLIP_VEC_CONFIG
/// @return TRUE: success
/// @return FALSE: fail
//-------------------------------------------------------------------------------------------------
void MDrv_GFLIP_GetVECaptureConfig(PMS_GFLIP_VEC_CONFIG pstGflipVECConfig)
{
    memcpy(pstGflipVECConfig, &s_stGflipVECConfig, sizeof(MS_GFLIP_VEC_CONFIG));
}

void MDrv_GFLIP_SetVECaptureConfig(PMS_GFLIP_VEC_CONFIG pstGflipVECConfig)
{
    pstGflipVECConfig->u8Result = TRUE;
    if(pstGflipVECConfig->eConfigType == MS_VEC_CONFIG_ENABLE)
    {
        s_stGflipVECConfig.eConfigType = MS_VEC_CONFIG_ENABLE;
        s_stGflipVECConfig.bInterlace  = pstGflipVECConfig->bInterlace;
    }
    else if(pstGflipVECConfig->eConfigType == MS_VEC_CONFIG_INIT)
    {
        memcpy(&s_stGflipVECConfig, pstGflipVECConfig, sizeof(MS_GFLIP_VEC_CONFIG));
        s_stGflipVECConfig.u8MaxFrameNumber_I <<= 1; //In kernel it is counted by field for interlace in ISR
        if(s_stGflipVECConfig.eIsrType >= MS_VEC_ISR_MAXNUM)
        {
            s_stGflipVECConfig.eIsrType = MS_VEC_ISR_GOP_OP;
            pstGflipVECConfig->u8Result = FALSE;//Unknown config type
        }
    }
    else
    {
        pstGflipVECConfig->u8Result = FALSE;//Unknown config type
    }
}

//-------------------------------------------------------------------------------------------------
/// Clear Irq
/// @param  void
/// @return TRUE: success
/// @attention
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_GFLIP_VEC_ClearIRQ(void)
{
    if(s_stGflipVECConfig.eIsrType == MS_VEC_ISR_VE)
    {
        //No need to clear for FIQ interrupt, just wake up queue
        if ( waitqueue_active(&_gcapture_waitqueue))
        {
            wake_up(&_gcapture_waitqueue);
        }
        return TRUE;
    }
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Process Irq
/// @param  void
/// @return TRUE: success
/// @attention
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_GFLIP_VEC_ProcessIRQ(void)
{
    static MS_BOOL bVEReady = FALSE;
    if(s_stGflipVECConfig.eIsrType == MS_VEC_ISR_VE)
    {
        if(s_bEnable && (s_u8FrameCount == VE_CAPTURE_FRAME_INVALID_NUM))
        {
            //VEC restart, reset state
            bVEReady = MHal_GFLIP_CheckVEReady();
        }
        if(bVEReady)
        {
            //Valid Frame is 1~3 for P mode or 1~4 for interlace mode
            //But HW is count from 0, so need conversion

            s_u8FrameCount = MHal_GFLIP_GetFrameIdx();
            if(s_u8FrameCount == 0)
            {
                if(s_stGflipVECConfig.bInterlace == FALSE)
                {
                    s_u8FrameCount = s_stGflipVECConfig.u8MaxFrameNumber_P;
                }
                else
                {
                    s_u8FrameCount = s_stGflipVECConfig.u8MaxFrameNumber_I;
                }
            }
            s_u8FrameNumUpdated = s_u8FrameCount;
        }
        return TRUE;
    }
    return FALSE;
}

#endif //CONFIG_MSTAR_VE_CAPTURE_SUPPORT
//-------------------------------------------------------------------------------------------------
/// Get histogram data
/// @param pu16Histogram                \b OUT: the value of histogram
/// @return FALSE :fail
//-------------------------------------------------------------------------------------------------

static MS_S64 s_s64VsyncTs[6] = {0, 0, 0, 0, 0, 0};

MS_S64 MDrv_GFLIP_WaitForVsync(MS_U32 u32GopIdx)
{
    switch(u32GopIdx)
    {
        case 0:
            interruptible_sleep_on_timeout(&_gvsync_gop0_waitqueue, msecs_to_jiffies(FLIP_INTERRUPT_TIMEOUT));
            break;
        case 1:
            interruptible_sleep_on_timeout(&_gvsync_gop1_waitqueue, msecs_to_jiffies(FLIP_INTERRUPT_TIMEOUT));
            break;
        case 2:
            interruptible_sleep_on_timeout(&_gvsync_gop2_waitqueue, msecs_to_jiffies(FLIP_INTERRUPT_TIMEOUT));
            break;
        case 3:
            interruptible_sleep_on_timeout(&_gvsync_gop3_waitqueue, msecs_to_jiffies(FLIP_INTERRUPT_TIMEOUT));
            break;
        case 4:
            interruptible_sleep_on_timeout(&_gvsync_gop4_waitqueue, msecs_to_jiffies(FLIP_INTERRUPT_TIMEOUT));
            break;
        case 5:
            interruptible_sleep_on_timeout(&_gvsync_gop5_waitqueue, msecs_to_jiffies(FLIP_INTERRUPT_TIMEOUT));
            break;
        default :
		GFLIP_PRINT("[%s][%d]u32GopIdx =%td is out of case", __func__, __LINE__, (ptrdiff_t)u32GopIdx);
			return 0;
    }
    return s_s64VsyncTs[u32GopIdx];
}
MS_BOOL MDrv_GFLIP_GetVsync(MS_U32 u32GopIdx)
{
    MS_BOOL bRet = TRUE;

    MS_S64 s64VsyncTS = ktime_to_ns(ktime_get());

    switch(u32GopIdx)
    {
        case 0:
        {
            if ( waitqueue_active(&_gvsync_gop0_waitqueue))
            {
                wake_up(&_gvsync_gop0_waitqueue);
            }
            break;
        }
        case 1:
        {
            if ( waitqueue_active(&_gvsync_gop1_waitqueue))
            {
                wake_up(&_gvsync_gop1_waitqueue);
            }
            break;
        }
        case 2:
        {
            if ( waitqueue_active(&_gvsync_gop2_waitqueue))
            {
                wake_up(&_gvsync_gop2_waitqueue);
            }
            break;
        }
        case 3:
        {
            if ( waitqueue_active(&_gvsync_gop3_waitqueue))
            {
                wake_up(&_gvsync_gop3_waitqueue);
            }
            break;
        }
        case 4:
        {
            if ( waitqueue_active(&_gvsync_gop4_waitqueue))
            {
                wake_up(&_gvsync_gop4_waitqueue);
            }
            break;
        }
        case 5:
        {
            if ( waitqueue_active(&_gvsync_gop5_waitqueue))
            {
                wake_up(&_gvsync_gop5_waitqueue);
            }
            break;
        }
        default :
		GFLIP_PRINT("[%s][%d] u32GopIdx =%td is out of case", __func__, __LINE__, (ptrdiff_t)u32GopIdx);
			return bRet;
    }

    s_s64VsyncTs[u32GopIdx] = s64VsyncTS;

    return bRet;
}


//-------------------------------------------------------------------------------------------
///Clear the FlipQueue by the  gop index and gwin Index
///@param u32GopIndex
///@param u32GwinIndex
///@return FALSE :fail
//---------------------------------------------------------------------------------------------
MS_BOOL _MDrv_GFLIP_ClearFlipQueue(MS_U32 u32GopIdx,MS_U32 u32GwinIdx)
{
    mutex_lock(&Semutex_Gflip);
	_u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]=_u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx] = 0;
	mutex_unlock(&Semutex_Gflip);
	return TRUE;
}
//-------------------------------------------------------------------------------------------
///Set GWIN info
///@param stGwinInfo
///@return FALSE :fail
//---------------------------------------------------------------------------------------------
MS_BOOL _MDrv_GFLIP_SetGwinInfo(MS_GWIN_INFO stGwinInfo)
{

    _u32GwinInfo[stGwinInfo.u8GopIdx][stGwinInfo.u8GwinIdx].u32Addr =  stGwinInfo.u32Addr;
    _u32GwinInfo[stGwinInfo.u8GopIdx][stGwinInfo.u8GwinIdx].u16X =  stGwinInfo.u16X;
    _u32GwinInfo[stGwinInfo.u8GopIdx][stGwinInfo.u8GwinIdx].u16Y =  stGwinInfo.u16Y;
    _u32GwinInfo[stGwinInfo.u8GopIdx][stGwinInfo.u8GwinIdx].u16W =  stGwinInfo.u16W;
    _u32GwinInfo[stGwinInfo.u8GopIdx][stGwinInfo.u8GwinIdx].u16H =  stGwinInfo.u16H;
    _u32GwinInfo[stGwinInfo.u8GopIdx][stGwinInfo.u8GwinIdx].u8GopIdx =  stGwinInfo.u8GopIdx;
    _u32GwinInfo[stGwinInfo.u8GopIdx][stGwinInfo.u8GwinIdx].u8GwinIdx =  stGwinInfo.u8GwinIdx;
    _u32GwinInfo[stGwinInfo.u8GopIdx][stGwinInfo.u8GwinIdx].clrType =  stGwinInfo.clrType;

     return TRUE;
}
//-------------------------------------------------------------------------------------------
///Get GWIN info
///@param u32GopIndex
///@param u32GwinIndex
///@return FALSE :MS_GWIN_INFO
//---------------------------------------------------------------------------------------------

MS_GWIN_INFO MDrv_GFLIP_GetGwinInfo(MS_U8 u8GopIdx,MS_U8 u8GwinIdx)
{
    return _u32GwinInfo[u8GopIdx][u8GwinIdx];
}

void SC_Save_Bank(int bkidx, unsigned short bank[])
{
    int i;
    unsigned short u16Bank = 0;
    u16Bank = SC_REG( 0x00 );
    SC_REG( 0x00 ) = (unsigned short)(bkidx);
    for(i=1;i<128;i++){
        bank[i]=SC_REG(i);
    }
    SC_REG( 0x00 ) = u16Bank;
}
void SC_Restore_Bank(int bkidx, unsigned short bank[])
{
    int i;
    unsigned short u16Bank = 0;
    u16Bank = SC_REG( 0x00 );
    SC_REG( 0x00 ) = (unsigned short)(bkidx);
    for(i=1;i<128;i++){
        SC_REG(i)=bank[i];
    }
    SC_REG( 0x00 ) = u16Bank;
}

int MDrv_GFLIP_Suspend(void)
{
    int i,j;
    int GopIdx, GopBks;

    for(i=0;i<GFLIP_REG_BANKS;i++){
        if (i < 12)
        {
            GopIdx=i/GFLIP_GOP_BANKOFFSET;
            GopBks=i%GFLIP_GOP_BANKOFFSET;
        }
        else if (i == 12)
        {
            //DWIN
            continue;
        }
        else if (i == 13)
        {
            //MIXER
            continue;
        }
        else
        {
            GopIdx=(i-2)/GFLIP_GOP_BANKOFFSET;
            GopBks=(i-2)%GFLIP_GOP_BANKOFFSET;
        }
        for(j=0;j<GFLIP_REG16_NUM_PER_BANK;j++){
            MHal_GFLIP_ReadGopReg(GopIdx,
                GopBks, j, &(_gflip_regs_save.BankReg[i][j]));
        }
    }
	_gflip_regs_save.suspend_check = MAGIC;
    _gflip_regs_save.CKG_GopReg[0]=CKG_REG(GOP_GOPCLK);
    _gflip_regs_save.CKG_GopReg[1]=CKG_REG(GOP_GOP2CLK);
    _gflip_regs_save.CKG_GopReg[2]=CKG_REG(GOP_GOP3CLK);
    _gflip_regs_save.CKG_GopReg[6]=CKG_REG(GOP_SRAMCLK);        /*Palette SRAM*/
    SC_REG(GOP_SC_BANKSEL) = GOP_SC_GOPBLENDING;
    _gflip_regs_save.SC_OPBlending[0] = SC_REG(GOP_SC_GOPBLENDING_L);
    _gflip_regs_save.SC_OPBlending[1] = SC_REG(GOP_SC_GOPBLENDING_H);
#ifdef GOP_SC1_GOPEN
    SC_REG(GOP_SC_BANKSEL) = GOP_SC1_GOPEN;
    _gflip_regs_save.SC_OPBlending[3] = SC_REG(GOP_SC_IP2SC);
    _gflip_regs_save.SC_OPBlending[4] = SC_REG(GOP_SC_GOPBLENDING_H);
#endif
    SC_REG(GOP_SC_BANKSEL) = GOP_SC_OCBANKSEL;
    _gflip_regs_save.SC_OPBlending[2] = SC_REG(GOP_SC_OCMIXER_ALPHA);

    SC_Save_Bank(0x00,_gflip_regs_save.SC_save[0]);
    SC_Save_Bank(0x0f,_gflip_regs_save.SC_save[1]);
    SC_Save_Bank(0x10,_gflip_regs_save.SC_save[2]);

#ifdef GOP_MIU_REG
    _gflip_regs_save.MIU_GopReg[0] = MIU0_REG(GOP_MIU_REG);
    _gflip_regs_save.MIU_GopReg[1] = MIU1_REG(GOP_MIU_REG);
    _gflip_regs_save.MIU_GopReg[2] = MIU2_REG(GOP_MIU_REG);
#endif

#ifdef GOP_MIU_IN_SC
    SC_REG(GOP_SC_BANKSEL) = GOP_SC_MIUBANKSEL;
    _gflip_regs_save.MIU_SC_GopReg[0] = SC_REG(GOP_SC_MIUSEL_HW_SW);
    _gflip_regs_save.MIU_SC_GopReg[1] = SC_REG(GOP_SC_MIUSEL_L);
#ifdef GOP_SC_MIUSEL_H
    _gflip_regs_save.MIU_SC_GopReg[2] = SC_REG(GOP_SC_MIUSEL_H);
#endif //GOP_SC_MIUSEL_H
#endif //GOP_MIU_IN_SC
#ifdef GS_REG_RESTORE_FUNCTION
    _gflip_regs_save.CKG_GopReg[3]=CKG_REG(GOP_SCLCLK);
    _gflip_regs_save.GS_GopReg=GS_REG(REG_GS_VSP_SRAM);
    _gflip_regs_save.CKG_GopReg[4]=CKG_REG(GOP_GOP4CLK);
#ifdef GOP_LB_SRAMCLK
    _gflip_regs_save.CKG_GopReg[5]=CKG_REG(GOP_LB_SRAMCLK);
#endif
#endif


#ifdef SUPPORT_GOP_DUALRATE
    SC_REG(GOP_SC_BANKSEL) = GOP_SC_DUALRATE_BLENDING_BNK;
    _gflip_regs_save.SC_GopBlending[0] = SC_REG(GOP_SC_DUALRATE_BLENDING0);
    _gflip_regs_save.SC_GopBlending[1] = SC_REG(GOP_SC_DUALRATE_BLENDING1);
    _gflip_regs_save.SC_GopBlending[2] = SC_REG(GOP_SC_DUALRATE_BLENDING2);

    SC_REG(GOP_SC_BANKSEL) = GOP_SC_HSYNC_SHIFT_BNK;
    _gflip_regs_save.SC_GopHsyncShift = SC_REG(GOP_SC_HSYNC_SHIFT);
#endif

#ifdef NEED_MANUAL_CROP
    for (i=0; i<MANUAL_CROP_SETS; i++)
    {
        _gflip_regs_save.u16GopManualCrop_PreCalDone[i] = REG_GOP_CROP_PRECALDONE(i);
        _gflip_regs_save.u16GopManualCrop_Hstart[i] = REG_GOP_CROP_ORIHSTART(i);
        _gflip_regs_save.u16GopManualCrop_HEnd[i]= REG_GOP_CROP_ORIHEND(i);
    }
#endif

#ifdef SUPPORT_GOP_AFBC
    _gflip_regs_save.u16AfbcRegClk = CKG_REG(GOP_AFBC_CLK);
    _gflip_regs_save.u16AfbcTrigger = REG_AFBC_TRIGGER;
    for (i=0; i<GOP_AFBC_CORE_NUMBER; i++)
    {
        _gflip_regs_save.u16AfbcRegEn[i] = REG_AFBC_CORE_EN(i);
        _gflip_regs_save.u16AfbcRegAddrL[i] = REG_AFBC_ADDR_L(i);
        _gflip_regs_save.u16AfbcRegAddrH[i] = REG_AFBC_ADDR_H(i);
        _gflip_regs_save.u16AfbcRegFmt[i] = REG_AFBC_FMT(i);
        _gflip_regs_save.u16AfbcRegWidth[i] = REG_AFBC_WIDTH(i);
        _gflip_regs_save.u16AfbcRegHeight[i] = REG_AFBC_HEIGHT(i);
    }
    _gflip_regs_save.u16AfbcRegMiu = REG_AFBC_MIU;
#endif




    return 0;
}

int MDrv_GFLIP_Resume(void)
{
    int i,j;
    int GopIdx, GopBks;
    unsigned short GWinEnable[MAX_GOP_SUPPORT]={0};
    MS_U32 u32GOPBitMask;
    MS_U16 u16tmp;
	MS_BOOL bIsRTCWakeUp = false;
#ifdef GOP_SC_MIU_ARBITOR_HW_BUG_PATCH
	MS_U16 u16mask0 = 0 , u16mask1 = 0;
#ifdef MIU2_ARB_REG
	MS_U16 u16mask2 = 0;
#endif
#endif
    CKG_REG(GOP_GOPCLK)=_gflip_regs_save.CKG_GopReg[0];
    CKG_REG(GOP_GOP2CLK)=_gflip_regs_save.CKG_GopReg[1];
    CKG_REG(GOP_GOP3CLK)=_gflip_regs_save.CKG_GopReg[2];
    SC_REG(GOP_SC_BANKSEL) = GOP_SC_GOPBLENDING;
    SC_REG(GOP_SC_GOPBLENDING_L)=_gflip_regs_save.SC_OPBlending[0];
    SC_REG(GOP_SC_GOPBLENDING_H)=_gflip_regs_save.SC_OPBlending[1];
#ifdef GOP_SC1_GOPEN
    SC_REG(GOP_SC_BANKSEL) = GOP_SC1_GOPEN;
    SC_REG(GOP_SC_IP2SC) = _gflip_regs_save.SC_OPBlending[3];
    SC_REG(GOP_SC_GOPBLENDING_H) = _gflip_regs_save.SC_OPBlending[4];
#endif
    SC_REG(GOP_SC_BANKSEL) = GOP_SC_OCBANKSEL;
    SC_REG(GOP_SC_OCMIXER_ALPHA) =  _gflip_regs_save.SC_OPBlending[2];

    SC_Restore_Bank(0x00,_gflip_regs_save.SC_save[0]);
    SC_Restore_Bank(0x0f,_gflip_regs_save.SC_save[1]);
    SC_Restore_Bank(0x10,_gflip_regs_save.SC_save[2]);

#ifdef GOP_MIU_REG
    MIU0_REG(GOP_MIU_REG) = _gflip_regs_save.MIU_GopReg[0];
    MIU1_REG(GOP_MIU_REG) = _gflip_regs_save.MIU_GopReg[1];
    MIU2_REG(GOP_MIU_REG) = _gflip_regs_save.MIU_GopReg[2];
#endif

#ifdef WAKUP_ANDROID_FROM_KERNEL
	if ((WAKEUP_SOURCE == WKUP_SRC_RTC) || (WAKEUP_SOURCE == WKUP_SRC_RTC2))
		bIsRTCWakeUp = true;
#else
	bIsRTCWakeUp = false;
#endif

	if (bIsRTCWakeUp == true) {
#ifdef GOP_MIU_IN_SC
		SC_REG(GOP_SC_BANKSEL) = GOP_SC_MIUBANKSEL;
		u16tmp = (SC_REG(GOP_SC_MIUSEL_HW_SW) & (~GOP_MIU_CLIENT)) |
			 (_gflip_regs_save.MIU_SC_GopReg[0] & GOP_MIU_CLIENT);
#ifdef GOP_SC_MIU_ARBITOR_HW_BUG_PATCH
	u16mask0 = MIU0_ARB_REG(GOP_ARB);
	u16mask1 = MIU1_ARB_REG(GOP_ARB);
	MIU0_ARB_REG(GOP_ARB) = u16mask0 | GOP_MIU_CLIENT;
	MIU1_ARB_REG(GOP_ARB) = u16mask1 | GOP_MIU_CLIENT;
#ifdef MIU2_ARB_REG
	u16mask2 = MIU2_ARB_REG(GOP_ARB);
	MIU2_ARB_REG(GOP_ARB) = u16mask2 | GOP_MIU_CLIENT;
#endif
#endif
		SC_REG(GOP_SC_MIUSEL_HW_SW) =  u16tmp;
		u16tmp = (SC_REG(GOP_SC_MIUSEL_L) & (~GOP_MIU_CLIENT)) |
			 (_gflip_regs_save.MIU_SC_GopReg[1] & GOP_MIU_CLIENT);
		SC_REG(GOP_SC_MIUSEL_L) =  u16tmp;
#ifdef GOP_SC_MIUSEL_H
		u16tmp = (SC_REG(GOP_SC_MIUSEL_H) & (~GOP_MIU_CLIENT)) |
			 (_gflip_regs_save.MIU_SC_GopReg[2] & GOP_MIU_CLIENT);
		SC_REG(GOP_SC_MIUSEL_H) =  u16tmp;
#ifdef GOP_SC_MIU_ARBITOR_HW_BUG_PATCH
	MIU0_ARB_REG(GOP_ARB) = u16mask0;
	MIU1_ARB_REG(GOP_ARB) = u16mask1;
#ifdef MIU2_ARB_REG
	MIU2_ARB_REG(GOP_ARB) = u16mask2;
#endif
#endif
#endif /* GOP_SC_MIUSEL_H */
#endif /* GOP_MIU_IN_SC */
	} else {
#ifdef GOP_MIU_IN_SC
		SC_REG(GOP_SC_BANKSEL) = GOP_SC_MIUBANKSEL;
		u16tmp = (SC_REG(GOP_SC_MIUSEL_HW_SW) &
			  (~(GOP_MIU_RESUME_CLIENT(GOP_LOGO)))) |
			 (_gflip_regs_save.MIU_SC_GopReg[0] &
			  GOP_MIU_RESUME_CLIENT(GOP_LOGO));
#ifdef GOP_SC_MIU_ARBITOR_HW_BUG_PATCH
	u16mask0 = MIU0_ARB_REG(GOP_ARB);
	u16mask1 = MIU1_ARB_REG(GOP_ARB);
	MIU0_ARB_REG(GOP_ARB) = u16mask0 | GOP_MIU_CLIENT;
	MIU1_ARB_REG(GOP_ARB) = u16mask1 | GOP_MIU_CLIENT;
#ifdef MIU2_ARB_REG
	u16mask2 = MIU2_ARB_REG(GOP_ARB);
	MIU2_ARB_REG(GOP_ARB) = u16mask2 | GOP_MIU_CLIENT;
#endif
#endif
		SC_REG(GOP_SC_MIUSEL_HW_SW) = u16tmp;
		u16tmp = (SC_REG(GOP_SC_MIUSEL_L) &
			  (~(GOP_MIU_RESUME_CLIENT(GOP_LOGO)))) |
			 (_gflip_regs_save.MIU_SC_GopReg[1] &
			  GOP_MIU_RESUME_CLIENT(GOP_LOGO));
		SC_REG(GOP_SC_MIUSEL_L) =  u16tmp;
#ifdef GOP_SC_MIUSEL_H
		u16tmp = (SC_REG(GOP_SC_MIUSEL_H) &
			  (~(GOP_MIU_RESUME_CLIENT(GOP_LOGO)))) |
			 (_gflip_regs_save.MIU_SC_GopReg[2] &
			  GOP_MIU_RESUME_CLIENT(GOP_LOGO));
		SC_REG(GOP_SC_MIUSEL_H) =  u16tmp;
#ifdef GOP_SC_MIU_ARBITOR_HW_BUG_PATCH
	MIU0_ARB_REG(GOP_ARB) = u16mask0;
	MIU1_ARB_REG(GOP_ARB) = u16mask1;
#ifdef MIU2_ARB_REG
	MIU2_ARB_REG(GOP_ARB) = u16mask2;
#endif
#endif
#endif /* GOP_SC_MIUSEL_H */
#endif /* GOP_MIU_IN_SC */
	}

#ifdef GS_REG_RESTORE_FUNCTION
        CKG_REG(GOP_SCLCLK)=_gflip_regs_save.CKG_GopReg[3];
        GS_REG(REG_GS_VSP_SRAM)=_gflip_regs_save.GS_GopReg;
        CKG_REG(GOP_GOP4CLK)=_gflip_regs_save.CKG_GopReg[4];
#ifdef GOP_LB_SRAMCLK
        CKG_REG(GOP_LB_SRAMCLK)=_gflip_regs_save.CKG_GopReg[5];
#endif
#endif

#ifdef SUPPORT_GOP_DUALRATE
    SC_REG(GOP_SC_BANKSEL) = GOP_SC_DUALRATE_BLENDING_BNK;
    SC_REG(GOP_SC_DUALRATE_BLENDING0) = _gflip_regs_save.SC_GopBlending[0];
    SC_REG(GOP_SC_DUALRATE_BLENDING1) = _gflip_regs_save.SC_GopBlending[1];
    SC_REG(GOP_SC_DUALRATE_BLENDING2) = _gflip_regs_save.SC_GopBlending[2];

    SC_REG(GOP_SC_BANKSEL) = GOP_SC_HSYNC_SHIFT_BNK;
    SC_REG(GOP_SC_HSYNC_SHIFT) = _gflip_regs_save.SC_GopHsyncShift;
#endif

    for(i=0;i<GFLIP_REG_BANKS;i++){
        if (i < 12)
        {
            GopIdx=i/GFLIP_GOP_BANKOFFSET;
            GopBks=i%GFLIP_GOP_BANKOFFSET;
        }
        else if (i == 12)
        {
            //DWIN
            continue;
        }
        else if (i == 13)
        {
            //MIXER
            continue;
        }
        else if (i > 13)
        {
            GopIdx=(i-2)/GFLIP_GOP_BANKOFFSET;
            GopBks=(i-2)%GFLIP_GOP_BANKOFFSET;
        }
        if((GopIdx < MAX_GOP_SUPPORT) && (GopBks==0)){//reset gop
            GWinEnable[GopIdx]=_gflip_regs_save.BankReg[i+1][0];
            _gflip_regs_save.BankReg[i+1][0] &= 0xFFFE;
        }

	if (bIsRTCWakeUp == false) {
		if ((i == (GFLIP_REG_PRE_BANK31(GOP_LOGO))) ||
		    (i == (GFLIP_REG_PRE_BANK32(GOP_LOGO))) ||
		    (i == (GFLIP_REG_PRE_BANK33(GOP_LOGO))))
			continue;
	}

	/* Dwin doesn't need resume */
	for (j = 0; j < GFLIP_REG16_NUM_PER_BANK; j++) {
		MHal_GFLIP_WriteGopReg(GopIdx, GopBks, j,
				       (_gflip_regs_save.BankReg[i][j]),
				       0xFFFF);
	}
    }

#ifdef NEED_MANUAL_CROP
    for (i=0; i<MANUAL_CROP_SETS; i++)
    {
        REG_GOP_CROP_PRECALDONE(i) = _gflip_regs_save.u16GopManualCrop_PreCalDone[i];
        REG_GOP_CROP_ORIHSTART(i) = _gflip_regs_save.u16GopManualCrop_Hstart[i];
        REG_GOP_CROP_ORIHEND(i) = _gflip_regs_save.u16GopManualCrop_HEnd[i];
    }
#endif

#ifdef SUPPORT_GOP_AFBC
    CKG_REG(GOP_AFBC_CLK) = _gflip_regs_save.u16AfbcRegClk;
    REG_AFBC_TRIGGER = _gflip_regs_save.u16AfbcTrigger;
    REG_AFBC_MIU = _gflip_regs_save.u16AfbcRegMiu;
    for (i=0; i<GOP_AFBC_CORE_NUMBER; i++)
    {
        REG_AFBC_ADDR_L(i) = _gflip_regs_save.u16AfbcRegAddrL[i];
        REG_AFBC_ADDR_H(i) = _gflip_regs_save.u16AfbcRegAddrH[i];
        REG_AFBC_FMT(i) = _gflip_regs_save.u16AfbcRegFmt[i];
        REG_AFBC_WIDTH(i) = _gflip_regs_save.u16AfbcRegWidth[i];
        REG_AFBC_HEIGHT(i) = _gflip_regs_save.u16AfbcRegHeight[i];

        REG_AFBC_CORE_EN(i) = _gflip_regs_save.u16AfbcRegEn[i];
    }
#endif

	u32GOPBitMask = 0;
	/* restore gwin enable */
	for (i = 0; i < MAX_GOP_SUPPORT; i++) {
		if (bIsRTCWakeUp == false) {
			if (i == GOP_LOGO) {
				u32GOPBitMask |= (1 << i);
				continue;
			}
		}

		MHal_GFLIP_WriteGopReg(i, GFLIP_GOP_BANK_IDX_0,
				REG_GOP4G_CTRL0, GOP_RST_MASK, GOP_RST_MASK);
		mdelay(5);

		if ((i >= 0) && (i <= 3)) {
			MHal_GFLIP_WriteGopReg(i, GFLIP_GOP_BANK_IDX_0,
					REG_GOP4G_CTRL0,
					_gflip_regs_save.BankReg[3*i][0],
					GOP_RST_MASK);
		} else if (i == 4) {
			MHal_GFLIP_WriteGopReg(i, GFLIP_GOP_BANK_IDX_0,
					REG_GOP4G_CTRL0,
					_gflip_regs_save.BankReg[14][0],
					GOP_RST_MASK);
		}
		mdelay(5);
		MHal_GFLIP_WriteGopReg(i, GFLIP_GOP_BANK_IDX_1,
				REG_GOP4G_GWIN_CTRL0(0),
				GWinEnable[i], 0xFFFF);
		u32GOPBitMask |= (1 << i);
	}
	MHal_GFLIP_Fire(u32GOPBitMask);
	return 0;
}

MS_BOOL MDrv_GFLIP_Resume_GOP(MS_U32 u32GopIdx)
{

	int i, j;
	int GopIdx, GopBks;
	unsigned short GWinEnable[MAX_GOP_SUPPORT] = {0};
	MS_U32 u32GOPBitMask;
	MS_U16 u16tmp;
#ifdef GOP_SC_MIU_ARBITOR_HW_BUG_PATCH
	MS_U16 u16mask0 = 0, u16mask1 = 0;
#ifdef MIU2_ARB_REG
	MS_U16 u16mask2 = 0;
#endif
#endif
	if (_gflip_regs_save.suspend_check != MAGIC) {
		return TRUE;
	} else{
		_gflip_regs_save.suspend_check = 0;
	}

#ifdef GOP_MIU_IN_SC
	SC_REG(GOP_SC_BANKSEL) = GOP_SC_MIUBANKSEL;
	u16tmp = (SC_REG(GOP_SC_MIUSEL_HW_SW)&(~(GOP_MIU_RESUME_GOP_CLIENT(u32GopIdx))))|(_gflip_regs_save.MIU_SC_GopReg[0]&(GOP_MIU_RESUME_GOP_CLIENT(u32GopIdx)));
#ifdef GOP_SC_MIU_ARBITOR_HW_BUG_PATCH
	u16mask0 = MIU0_ARB_REG(GOP_ARB);
	u16mask1 = MIU1_ARB_REG(GOP_ARB);
	MIU0_ARB_REG(GOP_ARB) = u16mask0 | GOP_MIU_RESUME_GOP_CLIENT(u32GopIdx);
	MIU1_ARB_REG(GOP_ARB) = u16mask1 | GOP_MIU_RESUME_GOP_CLIENT(u32GopIdx);
#ifdef MIU2_ARB_REG
	u16mask2 = MIU2_ARB_REG(GOP_ARB);
	MIU2_ARB_REG(GOP_ARB) = u16mask2 | GOP_MIU_RESUME_GOP_CLIENT(u32GopIdx);
#endif
#endif
	SC_REG(GOP_SC_MIUSEL_HW_SW) =  u16tmp;
	u16tmp = (SC_REG(GOP_SC_MIUSEL_L)&(~(GOP_MIU_RESUME_GOP_CLIENT(u32GopIdx))))|(_gflip_regs_save.MIU_SC_GopReg[1]&(GOP_MIU_RESUME_GOP_CLIENT(u32GopIdx)));
	SC_REG(GOP_SC_MIUSEL_L) =  u16tmp;
#ifdef GOP_SC_MIUSEL_H
	u16tmp = (SC_REG(GOP_SC_MIUSEL_H)&(~(GOP_MIU_RESUME_GOP_CLIENT(u32GopIdx))))|(_gflip_regs_save.MIU_SC_GopReg[2]&(GOP_MIU_RESUME_GOP_CLIENT(u32GopIdx)));
	SC_REG(GOP_SC_MIUSEL_H) =  u16tmp;
#ifdef GOP_SC_MIU_ARBITOR_HW_BUG_PATCH
	MIU0_ARB_REG(GOP_ARB) = u16mask0;
	MIU1_ARB_REG(GOP_ARB) = u16mask1;
#ifdef MIU2_ARB_REG
	MIU2_ARB_REG(GOP_ARB) = u16mask2;
#endif
#endif
#endif
#endif

	for (i = GFLIP_REG_PRE_BANK31(u32GopIdx) ; i <= GFLIP_REG_PRE_BANK33(u32GopIdx) ; i++) {
		if (i < 12) {
			GopIdx = i/GFLIP_GOP_BANKOFFSET;
			GopBks = i%GFLIP_GOP_BANKOFFSET;
		}
		if ((GopIdx < MAX_GOP_SUPPORT) && (GopBks == 0)) {
			GWinEnable[GopIdx] = _gflip_regs_save.BankReg[i+1][0];
			_gflip_regs_save.BankReg[i+1][0] &= 0xFFFE;
		}

		for (j = 0 ; j < GFLIP_REG16_NUM_PER_BANK ; j++) {
			MHal_GFLIP_WriteGopReg(GopIdx ,
			GopBks , j , (_gflip_regs_save.BankReg[i][j]) , 0xFFFF);
		}
	}

#ifdef NEED_MANUAL_CROP
	for (i = 0 ; i < MANUAL_CROP_SETS ; i++) {
		REG_GOP_CROP_PRECALDONE(i) = _gflip_regs_save.u16GopManualCrop_PreCalDone[i];
		REG_GOP_CROP_ORIHSTART(i) = _gflip_regs_save.u16GopManualCrop_Hstart[i];
		REG_GOP_CROP_ORIHEND(i) = _gflip_regs_save.u16GopManualCrop_HEnd[i];
	}
#endif

#ifdef SUPPORT_GOP_AFBC
	CKG_REG(GOP_AFBC_CLK) = _gflip_regs_save.u16AfbcRegClk;
	REG_AFBC_TRIGGER = _gflip_regs_save.u16AfbcTrigger;
	REG_AFBC_MIU = _gflip_regs_save.u16AfbcRegMiu;
	for (i = 0 ; i < GOP_AFBC_CORE_NUMBER ; i++) {
		REG_AFBC_ADDR_L(i) = _gflip_regs_save.u16AfbcRegAddrL[i];
		REG_AFBC_ADDR_H(i) = _gflip_regs_save.u16AfbcRegAddrH[i];
		REG_AFBC_FMT(i) = _gflip_regs_save.u16AfbcRegFmt[i];
		REG_AFBC_WIDTH(i) = _gflip_regs_save.u16AfbcRegWidth[i];
		REG_AFBC_HEIGHT(i) = _gflip_regs_save.u16AfbcRegHeight[i];

		REG_AFBC_CORE_EN(i) = _gflip_regs_save.u16AfbcRegEn[i];
	}
#endif

	u32GOPBitMask = 0;
	MHal_GFLIP_WriteGopReg(u32GopIdx,
	GFLIP_GOP_BANK_IDX_0 , REG_GOP4G_CTRL0 , GOP_RST_MASK , GOP_RST_MASK);
	mdelay(5);

	if ((u32GopIdx >= 0) && (u32GopIdx <= 3)) {
		MHal_GFLIP_WriteGopReg(u32GopIdx ,
		GFLIP_GOP_BANK_IDX_0 , REG_GOP4G_CTRL0 , _gflip_regs_save.BankReg[3*u32GopIdx][0] , GOP_RST_MASK);
	}

	mdelay(5);
	MHal_GFLIP_WriteGopReg(u32GopIdx ,
	GFLIP_GOP_BANK_IDX_1 , REG_GOP4G_GWIN_CTRL0(0) , GWinEnable[u32GopIdx] , 0xFFFF);
	u32GOPBitMask |= (1 << u32GopIdx);
	MHal_GFLIP_Fire(u32GOPBitMask);
	return TRUE;
}

MS_BOOL MDrv_GFLIP_Register_CB(MS_U8 u8Type, void *fpCB)
{
	if (fpCB == NULL) {
		printk("[%s][%d]  fpCB is NULL.\n", __func__, __LINE__);
		return FALSE;
	}

	if (u8Type == CB_MAPI_CFD_GOP_PREPROCESS) {
		_fpGflip_CB_CFD_GOP_PreProcess = (FP_CFD_GOP_PreProcess)fpCB;
	} else if (u8Type == CB_MAPI_CFD_GOP_PRESDR) {
		_fpGflip_CB_CFD_GOP_PreSDR = (FP_CFD_GOP_PreSDR)fpCB;
	} else if (u8Type == CB_MApi_DebugCFD_GOP_TestMode) {
		_fpGflip_CB_CFD_GOP_TestMode = (FP_CFD_GOP_TestMode)fpCB;
	} else {
		printk("[%s][%d] Not support this type.\n", __func__, __LINE__);
		return FALSE;
	}

	return TRUE;
}

EXPORT_SYMBOL(MDrv_GFLIP_GetGwinInfo);
EXPORT_SYMBOL(MDrv_GFLIP_Register_CB);

