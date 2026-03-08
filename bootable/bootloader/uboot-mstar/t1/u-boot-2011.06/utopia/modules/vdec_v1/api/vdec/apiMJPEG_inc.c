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
/// file    apiMJPEG.c
/// @brief  MJPEG API
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
// JPEG
#include "drvMJPEG.h"
#include "apiJPEG.h"
#include "apiMJPEG.h"
// RIU Base
#include "drvMMIO.h"


//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

#define ENABLE_MJPEG_FIRMWARE           1

#define ENABLE_AVSYNC_ID_CHECK          0

#define ENABLE_DOUBLE_FRAMERATE         0

#if (defined(CHIP_T8))
#define DECODER_RENDER_ARCH     1
#else
#define DECODER_RENDER_ARCH     0
#endif
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define JPEGBASEWIDTHMAX    1600
#define JPEGBASEHEIGHTMAX   1200
#define JPEGPROWIDTHMAX     1024
#define JPEGPROHEIGHTMAX    768

#define MJPEG_MAX_SUPPORT_WIDTH     640
#define MJPEG_MAX_SUPPORT_HEIGHT     480


#define DISPLAY_QUEUE_NUM           4



//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
#define _MJPEG_Memcpy(pDstAddr, pSrcAddr, u32Size)               \
    do {                                                        \
        MS_U32 i = 0;                                           \
        volatile MS_U8 *dst = (volatile MS_U8 *)(pDstAddr);     \
        volatile MS_U8 *src = (volatile MS_U8 *)(pSrcAddr);     \
        for (i = 0; i < (u32Size); i++)                         \
        {                                                       \
            dst[i] = src[i];                                    \
        }                                                       \
    } while (0)

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_BOOL bEnableDispCmdQ = TRUE;

//static MS_U32 __u32FwAddr = 0xFFFFFFFF;           // Address to load firmware
static MS_U32 __u32MRCBufAddr = 0;
//static MS_U32 __u32MRCBufSize = 0;
static MS_U32 __u32MWCBufAddr = 0;
#if defined(CHIP_MARIA10) ||  defined(CHIP_MACAW12) || defined(CHIP_EDEN) || defined(CHIP_EULER)
static MS_U32 __u32MWCBufSize = 0;
#endif
static MS_U32 __u32InternalBufAddr = 0;
static MS_U32 __u32InternalBufSize = 0;

static MS_U32 __u32ReadPtr = 0;
static MS_U32 __u32WritePtr = 0;

static MS_U32 __u32FrameCnt = 0;
static MS_U32 __u32FrameTime = 0;
static MS_U32 __u32FrameRate = 600;
static MS_U32 __u32FrameRateBase = 20;

static MS_U16 __u16Width = 0;
static MS_U16 __u16Height = 0;
static MS_U16 __u16Pitch = 0;

static MS_U8 __u8FrameBuffTotalNum = DISPLAY_QUEUE_NUM;
static MS_U32 __u32FrameBuffUnitSize = 0;

static MS_BOOL __bIsFirstFrameRdy = FALSE;

static MS_U8 PreVIdx = 0;
static MS_U16 ReCheckTime = 0;

static MS_BOOL __bIsDecodeDone = FALSE;
static MS_U32 __u32DropFrameCnt = 0;
static MS_U32 u32NextFrameBuffIdx = 0;
static MS_U32 u32EndAddr = 0;

// Speed Control
static MJPEG_SpeedType eCurSpeedType = E_MJPEG_SPEED_DEFAULT;
static MJPEG_DispSpeed eCurDispSpeed = E_MJPEG_DISP_SPEED_1X;
static MS_U32 __u32SpeedCtrlFrameCntBase = 0;

static MS_U8 _u8MJPEG_ApiDbgLevel = E_JPEG_DEBUG_NONE;


MJPEG_FrameInfo infoDecFrame;
static MS_BOOL isDecFrameInfoValid = FALSE;
static MS_BOOL bStepDec = FALSE;
static MS_BOOL bLstFrame = FALSE;

static MS_BOOL __bSkipToPts = FALSE;
static MS_BOOL __bSeekToPts = FALSE;
static MS_U32 __u32TargetPts = 0;
static MS_U32 __u32Skip2PtsBase = 0;

static MJPEG_ErrorCode eMJPEGErrCode;

#if defined(CHIP_MARIA10) ||  defined(CHIP_MACAW12) || defined(CHIP_EDEN) || defined(CHIP_EULER)
static MS_U32 _u32FramePitch = 0;
static MS_U32 _u32FrameHeight = 0;
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

#define MJPEG_DEBUG(format, args...)          do{if(_u8MJPEG_ApiDbgLevel & E_JPEG_DEBUG_MJPEG) printf(format, ##args);}while(0)
#define MJPEG_DEBUG_API_MSG(format, args...)  do{if(_u8MJPEG_ApiDbgLevel & E_JPEG_DEBUG_API) printf(format, ##args);}while(0)
#define MJPEG_DEBUG_API_ERR(format, args...)  do{if(_u8MJPEG_ApiDbgLevel & E_JPEG_DEBUG_ERR) printf(format, ##args);}while(0)


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
void _MApi_MJPEG_SetErrCode(MJPEG_ErrorCode eErrorCode)
{
    MJPEG_DEBUG_API_MSG("_MApi_MJPEG_SetErrCode() with eMJPEGErrCode=0x%x\n", eErrorCode);
    eMJPEGErrCode = eErrorCode;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

/******************************************************************************/
///Called only when video start
///@param pInitParam \b IN initial paramters
///@param pStream \b IN file system realted function pointers
///@return true or false
/******************************************************************************/
MJPEG_Result MApi_MJPEG_Init(MJPEG_INIT_PARAM *pInitParam)//, PJPEG_FILE_FileSystem_t pStream)
{
    JPEG_InitParam jpegInitParam;

    // For register read/write
    extern MS_U32 u32MJPEGRiuBaseAdd;
    MS_U32 u32NonPMBankSize = 0;

    if(FALSE == MDrv_MMIO_GetBASE( &u32MJPEGRiuBaseAdd, &u32NonPMBankSize, MS_MODULE_JPD))
    {
        MJPEG_DEBUG_API_MSG("MDrv_MMIO_GetBASE FAILED !! \n");
        return E_MJPEG_RET_FAIL;
    }

    MJPEG_DEBUG_API_MSG("DRVreg base = 0x%lX, length = %lu\n", u32MJPEGRiuBaseAdd, u32NonPMBankSize);

#if defined(CHIP_MARIA10) ||  defined(CHIP_MACAW12) || defined(CHIP_EDEN) || defined(CHIP_EULER)
    _u32FramePitch = (((pInitParam->u32Width + 7)>>3)<<3);
    _u32FrameHeight = pInitParam->u32Height;
#endif

#if ENABLE_MJPEG_FIRMWARE
    //
    // Init Firmware
    //
#if defined(CHIP_T2)
    // Reset VPU
    MDrv_JPD_RstVPU();
    // Load JPD firmware
//    __u32FwAddr = pInitParam->u32FwAddr;
    MDrv_JPD_LoadCode(pInitParam->u32FwAddr, pInitParam->u32FwBinSize, pInitParam->u32FwBinAddr, pInitParam->eFwSrcType);
    // Set VPU base and endian
    MDrv_JPD_ConfigVPU(pInitParam->u32FwAddr, 1); // 1 for little endian

    // Clear ShareMemory before releasing VPU
    MDrv_JPD_ClearShareMemory();

    // Release VPU
    MDrv_JPD_ReleaseVPU();
#else
    if(E_JPD_OK != MDrv_JPD_InitFw(pInitParam->u32FwAddr, pInitParam->u32FwBinSize, pInitParam->u32FwBinAddr, pInitParam->eFwSrcType))
    {
        MJPEG_DEBUG_API_ERR("MDrv_JPD_InitFw Failed~~~~\n");
    }
//    __u32FwAddr = pInitParam->u32FwAddr;
#endif

    isDecFrameInfoValid = FALSE;
    bStepDec = FALSE;

    MJPEG_DEBUG_API_MSG("clear error code due to call MApi_MJPEG_Init()\n");
    _MApi_MJPEG_SetErrCode(E_MJPEG_ERROR_CODE_NONE);
    // Check if firmware is ready or not
    if (MDrv_JPD_IsFirmwareReady() == FALSE)
        return E_MJPEG_RET_FAIL;

    if((pInitParam->u32MWCBufAddr)%8)
    {
        MJPEG_DEBUG_API_ERR("u32MWCBufAddr need 8 byte alignment~~~~0x%lx\n", pInitParam->u32MWCBufAddr);
    }
    else
    {
        MJPEG_DEBUG_API_ERR("u32MWCBufAddr don't need 8 byte alignment~~~~0x%lx\n", pInitParam->u32MWCBufAddr);
    }

    MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_FRAME_BUFF_START_ADDR, pInitParam->u32MWCBufAddr);
#if defined(CHIP_MARIA10) ||  defined(CHIP_MACAW12) || defined(CHIP_EDEN) || defined(CHIP_EULER)
    __u32FrameBuffUnitSize = _u32FramePitch * _u32FrameHeight * 2;
    printf("_u32FramePitch = %ld\n", _u32FramePitch);
    printf("_u32FrameHeight = %ld\n", _u32FrameHeight);
    printf("__u32FrameBuffUnitSize = 0x%lx\n", __u32FrameBuffUnitSize);
#else
    __u32FrameBuffUnitSize = pInitParam->u32MWCBufSize/__u8FrameBuffTotalNum;
    __u32FrameBuffUnitSize = (__u32FrameBuffUnitSize >> 3) << 3;//8 byte alignment
#endif
    MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_FRAME_BUFF_UNIT_SIZE, __u32FrameBuffUnitSize);
    MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_FRAME_BUFF_TOTAL_NUM, __u8FrameBuffTotalNum);
    #if ENABLE_DOUBLE_FRAMERATE
    printf ("   frame rate base %ld\n", pInitParam->u32FrameRate);
    pInitParam->u32FrameRate /= 2;
    printf ("   1/2 frame rate base %ld\n", pInitParam->u32FrameRate);
    #endif
    MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_FRAMERATE, pInitParam->u32FrameRate);
    MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_FRAMERATE_BASE, pInitParam->u32FrameRateBase);
    MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_CHIP_ID, MDrv_JPD_Get_Chip_ID());

    if(pInitParam->u32FrameRate ==0)
    {
        MJPEG_DEBUG_API_ERR("Error!!! MJPEG Frame rate is 0\n");
        pInitParam->u32FrameRate =30000;
    }
    __u32FrameRate = pInitParam->u32FrameRate;
    __u32FrameRateBase = pInitParam->u32FrameRateBase;
    __u32FrameTime = __u32FrameRateBase * 1000 / __u32FrameRate;
#endif // ENABLE_MJPEG_FIRMWARE

    __u32FrameCnt = 0;
    __u32DropFrameCnt = 0;

    __u32MRCBufAddr = pInitParam->u32MRCBufAddr;
//    __u32MRCBufSize = pInitParam->u32MRCBufSize;
    __u32MWCBufAddr = pInitParam->u32MWCBufAddr;
#if defined(CHIP_MARIA10) ||  defined(CHIP_MACAW12) || defined(CHIP_EDEN) || defined(CHIP_EULER)
    __u32MWCBufSize = __u32FrameBuffUnitSize * __u8FrameBuffTotalNum * 2;
#else
//    __u32MWCBufSize = pInitParam->u32MWCBufSize;
#endif
    __u32InternalBufAddr = pInitParam->u32InternalBufAddr;
    __u32InternalBufSize = pInitParam->u32InternalBufSize;

    jpegInitParam.u32MRCBufAddr = pInitParam->u32MRCBufAddr;
    jpegInitParam.u32MRCBufSize = pInitParam->u32MRCBufSize;
    jpegInitParam.u32MWCBufAddr = pInitParam->u32MWCBufAddr;
#if defined(CHIP_MARIA10) ||  defined(CHIP_MACAW12) || defined(CHIP_EDEN) || defined(CHIP_EULER)
    jpegInitParam.u32MWCBufSize = __u32MWCBufSize;
#else
    jpegInitParam.u32MWCBufSize = pInitParam->u32MWCBufSize;
#endif
    jpegInitParam.u32InternalBufAddr = pInitParam->u32InternalBufAddr;
    jpegInitParam.u32InternalBufSize = pInitParam->u32InternalBufSize;
    jpegInitParam.u32DecByteRead = 0;
    jpegInitParam.bEOF = TRUE;
    jpegInitParam.u8DecodeType = E_JPEG_TYPE_MJPEG;
    jpegInitParam.bInitMem = TRUE;
    jpegInitParam.pFillHdrFunc = NULL;


    // This is used to init memory
    if (MApi_JPEG_Init(&jpegInitParam) == E_JPEG_FAILED)
        return E_MJPEG_RET_FAIL;

    // Make sure it's exited correctly
    MApi_JPEG_Exit();

    // Init Local Variables
    PreVIdx = 0;
    ReCheckTime = 0;
    bLstFrame = FALSE;
    __u32ReadPtr = 0;
    __u32WritePtr =0;

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Reset JPD
/******************************************************************************/
MJPEG_Result MApi_MJPEG_Rst(void)
{
    //__u32FrameCnt = 0;
    __bIsFirstFrameRdy = FALSE;

    MApi_JPEG_Rst();
    MJPEG_DEBUG_API_MSG("clear error code due to call MApi_MJPEG_Rst()\n");
    _MApi_MJPEG_SetErrCode(E_MJPEG_ERROR_CODE_NONE);

    // CHECK : reset firmware???

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Turn off JPD clock
/******************************************************************************/
//extern void MApi_JPEG_DisableAddressConvert(void); // defined in apiJPEG.c
MJPEG_Result MApi_MJPEG_Stop(void)
{

    //__u32FrameCnt = 0;
    __bIsFirstFrameRdy = FALSE;
    isDecFrameInfoValid = FALSE;
    bStepDec = FALSE;

    MDrv_JPD_StopVPU();

    //MApi_JPEG_PowerOff() is called in MApi_JPEG_Exit()
    MApi_JPEG_Exit();

    MApi_JPEG_DisableAddressConvert();

    // CHECK : stop firmware???

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Check if first frame has been decoded
///@return TRUE if first picture is decoded, else return FALSE
/******************************************************************************/
MJPEG_Result MApi_MJPEG_CheckDispInfoRdy(void)
{
    if ((TRUE == __bIsFirstFrameRdy)
    && (E_JPEG_EVENT_DEC_DONE & MApi_JPEG_GetJPDEventFlag()))
    {
        // Should use the width & height of 1st frame
        //__u16Width = MApi_JPEG_GetWidth();
        //__u16Height = MApi_JPEG_GetHeight();

        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_CheckDispInfoRdy : E_MJPEG_RET_SUCCESS\n");
        return E_MJPEG_RET_SUCCESS;
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_CheckDispInfoRdy : E_MJPEG_RET_FAIL\n");
    return E_MJPEG_RET_FAIL;
}

/******************************************************************************/
///Play
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_Play(void)
{
    if (MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_PLAY, 0xFFFFFFFF) == 0)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_Play : E_MJPEG_RET_FAIL\n");
        return E_MJPEG_RET_FAIL;
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_Play : E_MJPEG_RET_SUCCESS\n");
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Pause
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_Pause(void)
{
    if (MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_PAUSE, 0xFFFFFFFF) == 0)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_Play : E_MJPEG_RET_FAIL\n");
        return E_MJPEG_RET_FAIL;
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_Play : E_MJPEG_RET_SUCCESS\n");
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Resume
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_Resume(void)
{
    if (MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_RESUME, 0xFFFFFFFF) == 0)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_Resume : E_MJPEG_RET_FAIL\n");
        return E_MJPEG_RET_FAIL;
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_Resume : E_MJPEG_RET_SUCCESS\n");
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Step Play
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_StepPlay(void)
{
    if (MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_STEP_PLAY, 0xFFFFFFFF) == 0)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_StepPlay : E_MJPEG_RET_FAIL\n");
        return E_MJPEG_RET_FAIL;
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_StepPlay : E_MJPEG_RET_SUCCESS\n");
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Check if step play is done or not
///@return true or false
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsStepPlayDone(void)
{
    if (MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_IS_STEP_PLAY_DONE, 0xFFFFFFFF) == 0)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_IsStepPlayDone : TRUE\n");
        return TRUE;
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_IsStepPlayDone : FALSE\n");
    return FALSE;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_StepDecode(void)
{
    bStepDec = FALSE;
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Check if JPEG decode done
///@return status
/******************************************************************************/
MJPEG_Result __MApi_MJPEG_CheckDecodeDone(void)
{
    MS_U16 cur_vidx = 0;


    if (__bIsFirstFrameRdy)
    {
        JPEG_Event reg_val = MApi_JPEG_GetJPDEventFlag();
        MJPEG_DEBUG_API_MSG("        EventFlag = %d\n", reg_val);

        //For H/W bug, some cases can not exit after decode done. Check Vidx to exit.
        if(PreVIdx != (cur_vidx = MApi_JPEG_GetCurVidx()))
        {
            PreVIdx = cur_vidx;
            ReCheckTime = 0;
        }
        else
        {
            ReCheckTime++;
            if(ReCheckTime >= 1000)
            {
                MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID, u32NextFrameBuffIdx);
                MJPEG_DEBUG_API_MSG("Set frame buffer index %ld as Valid", u32NextFrameBuffIdx);
                __bIsDecodeDone = TRUE;
                __u32DropFrameCnt++;

                MJPEG_DEBUG_API_ERR("Decode timeout!!!!\n");
                reg_val = E_JPEG_EVENT_DEC_ERROR_MASK;
                return E_MJPEG_RET_FAIL;
            }
        }

        if(E_JPEG_EVENT_DEC_DONE & reg_val)
        {
            ReCheckTime = 0;

            MJPEG_DEBUG_API_MSG("        Decode Done -- ");
#if ENABLE_MJPEG_FIRMWARE
            if (__bIsDecodeDone == FALSE)
            {
                if (__u32DropFrameCnt != 0)
                {   // Ask firmware to compensate PTS
                    MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_COMPENSATE_PTS, __u32DropFrameCnt);
                    MJPEG_DEBUG_API_MSG("Compensate PTS for %ld dropped frames", __u32DropFrameCnt);
                }
                __u32DropFrameCnt = 0;

                MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_FRAME_BUFF_IDX_READY, u32NextFrameBuffIdx);
                MJPEG_DEBUG_API_MSG("Frame Buffer Index %ld", u32NextFrameBuffIdx);
                __bIsDecodeDone = TRUE;
            }
#endif
            MJPEG_DEBUG_API_MSG("\n");
            //__u32FrameCnt++;
            //__u32ReadPtr = u32EndAddr;
            MJPEG_DEBUG_API_MSG("Count %ld\n", __u32FrameCnt);
        }
        else
        if(E_JPEG_EVENT_DEC_ERROR_MASK & reg_val)
        {
            MJPEG_DEBUG_API_ERR("   JPD Decode Error\n");

            MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID, u32NextFrameBuffIdx);
            MJPEG_DEBUG_API_MSG("Set frame buffer index %ld as Valid", u32NextFrameBuffIdx);
            __bIsDecodeDone = TRUE;
            __u32DropFrameCnt++;
            return E_MJPEG_RET_DECODE_ERROR;
        }
        else
        {
            MJPEG_DEBUG_API_MSG("   Still decoding previous JPEG ......\n");

            return E_MJPEG_RET_DECODING;
        }
    }
    else
    {
        return E_MJPEG_RET_FAIL;
    }


    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsStepDecodeDone(void)
{
    if(FALSE == bStepDec)
        return FALSE;

    if (E_MJPEG_RET_SUCCESS == __MApi_MJPEG_CheckDecodeDone())
        return TRUE;

    return FALSE;
}

static MS_U16 u16NullCnt;

/******************************************************************************/
///Issue full JPEG decode process
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
//MJPEG_Result MApi_MJPEG_PushQueue(MS_PHYADDR u32StAddr, MS_U32 u32Size, MS_U32 u32TimeStamp)
MJPEG_Result MApi_MJPEG_PushQueue(MJPEG_Packet_Info *pInfo)
{
    //
    // There's no queue
    //
    JPEG_InitParam jpegInitParam;


    if(pInfo->u32Size == 0)
    {
        u16NullCnt++;
        MJPEG_DEBUG_API_MSG("%s: MJPEG NULL PACKET!!!!!!!!!!\n", __FUNCTION__);
        return E_MJPEG_RET_SUCCESS;
    }

    MS_U16 u16DecodeCnt;
    for(u16DecodeCnt=0; u16DecodeCnt<=u16NullCnt; u16DecodeCnt++)
    {
        if (E_MJPEG_RET_DECODING == __MApi_MJPEG_CheckDecodeDone())
        {
            u16NullCnt -=u16DecodeCnt;
            MJPEG_DEBUG_API_MSG("%s: E_MJPEG_RET_DECODING with u16DecodeCnt=%d\n", __FUNCTION__, u16DecodeCnt);
            return E_MJPEG_RET_DECODING;
        }
#if 1
        MJPEG_DEBUG("[MWC][%dx%d.yuyv]addr=0x%lx, size=0x%x\n",
            MApi_JPEG_GetWidth(), MApi_JPEG_GetHeight(), infoDecFrame.u32LumaAddr,
            MApi_JPEG_GetWidth()*MApi_JPEG_GetHeight()*2);
#endif
        __u32FrameCnt++;
        __u32ReadPtr = u32EndAddr;
        MJPEG_DEBUG_API_MSG("%s: Count %ld, __bSkipToPts=%d, __bSeekToPts=%d, __u32Skip2PtsBase=%ld, __u32TargetPts=%ld\n",
            __FUNCTION__, __u32FrameCnt, __bSkipToPts, __bSeekToPts, __u32Skip2PtsBase, __u32TargetPts);


        __u32WritePtr = pInfo->u32StAddr + pInfo->u32Size - 1;


        // Skip/Seek to PTS
        if (__bSkipToPts || __bSeekToPts)
        {
            if (__u32Skip2PtsBase >= __u32TargetPts)
            {
                MJPEG_DEBUG_API_MSG("Skip/Seek to PTS done -- current PTS %ld\n", __u32Skip2PtsBase);
                __bSkipToPts = FALSE;
                __bSeekToPts = FALSE;
                // Need to update firmware PTS base
                MDrv_JPD_ResetPTS(__u32Skip2PtsBase);
            }
            else
            {
                MJPEG_DEBUG_API_MSG("Skip Frame -- PTS %ld\n", __u32Skip2PtsBase);
                __u32Skip2PtsBase += __u32FrameTime;
                __bIsDecodeDone = TRUE;
                u32EndAddr = pInfo->u32StAddr + pInfo->u32Size - 1;
                return E_MJPEG_RET_SUCCESS;
            }
        }

        // Speed Control
        if (__bIsFirstFrameRdy)
        {
            if (eCurSpeedType == E_MJPEG_SPEED_FAST)
            {
                if ((__u32FrameCnt - __u32SpeedCtrlFrameCntBase) % eCurDispSpeed)
                {
                    MJPEG_DEBUG_API_MSG("       Skip frame %ld\n", (__u32FrameCnt - __u32SpeedCtrlFrameCntBase));
                    //__u32FrameCnt++;
                    __bIsDecodeDone = TRUE;
                    u16NullCnt=0;
                    return E_MJPEG_RET_SUCCESS;
                }
            }
            else if (eCurSpeedType == E_MJPEG_SPEED_SLOW)
            {
            }
            else // if (eCurSpeedType == E_MJPEG_SPEED_DEFAULT)
            {
                __u32SpeedCtrlFrameCntBase = __u32FrameCnt;
                MJPEG_DEBUG_API_MSG("   Update Frame Count Base %ld\n", __u32SpeedCtrlFrameCntBase);
            }
        }


        MApi_JPEG_SetMaxDecodeResolution(JPEGBASEWIDTHMAX, JPEGBASEHEIGHTMAX);
        // Init
        jpegInitParam.u32MRCBufAddr = pInfo->u32StAddr + __u32MRCBufAddr;
        // Tricky code: let JPD decode only L buffer, then decode dne
        jpegInitParam.u32MRCBufSize = pInfo->u32Size*2;
        if (jpegInitParam.u32MRCBufSize < JPEG_DEFAULT_EXIF_SIZE)
        {   // Meet size check in MApi_JPEG_Init()
            jpegInitParam.u32MRCBufSize = JPEG_DEFAULT_EXIF_SIZE;
        }

        // Ask firmware if there is any frame buffer available
        u32NextFrameBuffIdx = MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_GET_NEXT_FREE_FRAME_BUFF_IDX, 0xFFFFFFFF);
        MJPEG_DEBUG_API_MSG("                   E_MJPEG_CMD_GET_NEXT_FREE_FRAME_BUFF_IDX %ld\n", u32NextFrameBuffIdx);
        if (u32NextFrameBuffIdx >= 0xFF)// It may be 0xFF, 0xFFFF, 0xFFFFFF, or 0xFFFFFFFF
        {
            MJPEG_DEBUG_API_ERR("   !!! No frame buffer available !!!\n");
            __bIsDecodeDone = TRUE;
            __u32DropFrameCnt++;
            u16NullCnt=0;
            return E_MJPEG_RET_FAIL;
        }
        if (__bIsFirstFrameRdy == FALSE)
        {
            u32NextFrameBuffIdx = 0;
        }
        __bIsDecodeDone = FALSE;
        MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_FRAME_BUFF_IDX, u32NextFrameBuffIdx);
        MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_TIMESTAMP, pInfo->u32TimeStamp);
        MJPEG_DEBUG_API_MSG("   u32TimeStamp %ld", pInfo->u32TimeStamp);
        MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_FRAME_ID_L, pInfo->u32ID_L);
        MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_FRAME_ID_H, pInfo->u32ID_H);
        MJPEG_DEBUG_API_MSG("Frame ID 0x%lx%lx", pInfo->u32ID_H, pInfo->u32ID_L);

        jpegInitParam.u32MWCBufAddr = __u32MWCBufAddr + __u32FrameBuffUnitSize*u32NextFrameBuffIdx;
        MJPEG_DEBUG_API_MSG("   Set Frame buffer addr 0x%lx\n", jpegInitParam.u32MWCBufAddr);
        jpegInitParam.u32MWCBufSize = __u32FrameBuffUnitSize;

        jpegInitParam.u32InternalBufAddr = __u32InternalBufAddr;
        jpegInitParam.u32InternalBufSize = __u32InternalBufSize;
        jpegInitParam.u32DecByteRead = pInfo->u32Size;
        jpegInitParam.bEOF = TRUE; // Set to TRUE to disable read function
        jpegInitParam.u8DecodeType = E_JPEG_TYPE_MAIN; // only set to E_JPEG_TYPE_MJPEG when 1st time
        jpegInitParam.bInitMem = TRUE;
        jpegInitParam.pFillHdrFunc = NULL;

        #if ENABLE_AVSYNC_ID_CHECK
        printf ("ID 0x%08lx%08lx    PTS %ld\n", pInfo->u32ID_H, pInfo->u32ID_L, pInfo->u32TimeStamp);
        #endif

        // Make sure it's exited correctly
        MApi_JPEG_Exit();


        u32EndAddr = pInfo->u32StAddr + pInfo->u32Size - 1;


        if (MApi_JPEG_Init(&jpegInitParam) == E_JPEG_FAILED)
        {
            MJPEG_DEBUG_API_ERR("MApi_JPEG_Init() FAIL\n");
            MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID, u32NextFrameBuffIdx);
            MJPEG_DEBUG_API_MSG("Set frame buffer index %ld as Valid", u32NextFrameBuffIdx);
            __bIsDecodeDone = TRUE;
            __u32DropFrameCnt++;
            u16NullCnt=0;
            return E_MJPEG_RET_FAIL;
        }

        // Get width, height, & pitch of 1st frame
        if (__bIsFirstFrameRdy == FALSE)
        {
            __u16Width = MApi_JPEG_GetWidth();
            MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_WIDTH, __u16Width);
            __u16Height = MApi_JPEG_GetHeight();
            MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_HEIGHT, __u16Height);
            __u16Pitch = MApi_JPEG_GetAlignedPitch();
            MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_PITCH, __u16Pitch);
            MJPEG_DEBUG_API_MSG("       width %d, height %d, pitch %d\n", __u16Width, __u16Height, __u16Pitch);

            if((__u16Width > MJPEG_MAX_SUPPORT_WIDTH) ||
                (__u16Height>MJPEG_MAX_SUPPORT_HEIGHT))
            {
                MJPEG_DEBUG_API_MSG("Set MJPEG error code=E_MJPEG_ERR_CODE_RES \n");
                _MApi_MJPEG_SetErrCode(E_MJPEG_ERR_CODE_RES);
            }
#if defined(CHIP_MARIA10) ||  defined(CHIP_MACAW12) || defined(CHIP_EDEN) || defined(CHIP_EULER)
            if((_u32FramePitch < __u16Width)
            || (_u32FrameHeight < __u16Height))
            {
                _u32FramePitch = (((__u16Width + 7)>>3)<<3);
                _u32FrameHeight = __u16Height;
                __u32FrameBuffUnitSize = _u32FramePitch * _u32FrameHeight * 2;
                MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_FRAME_BUFF_UNIT_SIZE, __u32FrameBuffUnitSize);
                MJPEG_DEBUG_API_MSG("Change _u32FramePitch = %ld\n", _u32FramePitch);
                MJPEG_DEBUG_API_MSG("Change _u32FrameHeight = %ld\n", _u32FrameHeight);
                MJPEG_DEBUG_API_MSG("Change __u32FrameBuffUnitSize = 0x%lx\n", __u32FrameBuffUnitSize);
            }
#endif
        }

        // Fill decode frame info
        infoDecFrame.u32LumaAddr = jpegInitParam.u32MWCBufAddr;
        infoDecFrame.u32ChromaAddr = infoDecFrame.u32LumaAddr;
        infoDecFrame.u32TimeStamp = pInfo->u32TimeStamp;
        infoDecFrame.u32ID_L = pInfo->u32ID_L;
        infoDecFrame.u32ID_H = pInfo->u32ID_H;
        infoDecFrame.u16Pitch = __u16Pitch;
        infoDecFrame.u16Width = __u16Width;
        infoDecFrame.u16Height = __u16Height;
        isDecFrameInfoValid = TRUE;

        // Decode header
        if (MApi_JPEG_DecodeHdr() == E_JPEG_FAILED)
        {
            MJPEG_DEBUG_API_ERR("MApi_JPEG_DecodeHdr() FAIL\n");
            MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID, u32NextFrameBuffIdx);
            MJPEG_DEBUG_API_MSG("Set frame buffer index %ld as Valid", u32NextFrameBuffIdx);
            __bIsDecodeDone = TRUE;
            __u32DropFrameCnt++;
            u16NullCnt=0;

            JPEG_ErrCode jpeg_errcode = MApi_JPEG_GetErrorCode();
            if((E_JPEG_UNSUPPORTED_COLORSPACE == jpeg_errcode)
            || (E_JPEG_UNSUPPORTED_SAMP_FACTORS == jpeg_errcode)
            || (E_JPEG_UNSUPPORTED_HUFF_DECODE == jpeg_errcode))
            {
                MJPEG_DEBUG_API_ERR("Set MJPEG error code=E_MJPEG_ERR_CODE_RES for unsupport!!\n");
                _MApi_MJPEG_SetErrCode(E_MJPEG_ERR_CODE_RES);
            }

            return E_MJPEG_RET_DECODE_HDR_ERROR;
        }

        bStepDec = TRUE;

        // Decode
        if (MApi_JPEG_Decode() == E_JPEG_FAILED)
        {
            MJPEG_DEBUG_API_ERR("MApi_JPEG_Decode() FAIL\n");
            MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID, u32NextFrameBuffIdx);
            MJPEG_DEBUG_API_MSG("Set frame buffer index %ld as Valid", u32NextFrameBuffIdx);
            __bIsDecodeDone = TRUE;
            __u32DropFrameCnt++;
            u16NullCnt=0;
            return E_MJPEG_RET_DECODE_ERROR;
        }


        // Moved Up
        //u32EndAddr = pInfo->u32StAddr + pInfo->u32Size - 1;
        __bIsFirstFrameRdy = TRUE;

        // All done, wait for decoding finish
        if(u16NullCnt ==u16DecodeCnt)
        {
            if(u16NullCnt!=0)
            {
                MJPEG_DEBUG_API_MSG("%s: Handle MJPEG NULL PACKET with u16DecodeCnt=%d\n", __FUNCTION__, u16DecodeCnt);
            }
            u16NullCnt=0;
            return E_MJPEG_RET_SUCCESS;
        }
        else
        {
            MJPEG_DEBUG_API_MSG("%s: Duplicate PACKET with u16DecodeCnt=%d\n", __FUNCTION__, u16DecodeCnt);
        }
    }

    // Only for compile ok, the code should not run to this place.
    MJPEG_DEBUG_API_MSG("%s: Handle MJPEG NULL PACKET with u16DecodeCnt=%d\n", __FUNCTION__, u16DecodeCnt);
    u16NullCnt=0;
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_U32 MApi_MJPEG_GetQueueVacancy(void)
{
    MS_U32 u32tmpcounter;
    #if ENABLE_AVSYNC_ID_CHECK
    MJPEG_FrameInfo info;
    MApi_MJPEG_GetDispFrameInfo(&info);
    #endif


    if (__bIsFirstFrameRdy == FALSE)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_GetQueueVacancy : 1\n");
        return 1;
    }
    else
    {
        if (__MApi_MJPEG_CheckDecodeDone() != E_MJPEG_RET_DECODING)
        {
            if (MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_IS_DISPLAY_QUEUE_FULL, 0xFFFFFFFF) == 1)
            {
                MJPEG_DEBUG_API_MSG("            Display queue is full\n");
                MJPEG_DEBUG_API_MSG("           MApi_MJPEG_GetQueueVacancy : 0\n");
                return 0;
            }

            u32tmpcounter = MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_GET_NEXT_FREE_FRAME_BUFF_IDX, 0xFFFFFFFF);
            if ( u32tmpcounter>= 0xFF)
            {
                MJPEG_DEBUG_API_MSG("E_MJPEG_CMD_GET_NEXT_FREE_FRAME_BUFF_IDX %ld\n", u32tmpcounter);
                return 0;
            }
            MJPEG_DEBUG_API_MSG("           MApi_MJPEG_GetQueueVacancy : 1\n");
            return 1;
        }
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_GetQueueVacancy : 0\n");
    return 0;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_FlushQueue(void)
{
    // Wait current frame decode done
    while (E_MJPEG_RET_DECODING == __MApi_MJPEG_CheckDecodeDone())
        ;

    // Stop firmware display to avoid display queue status overwritten
    MApi_MJPEG_Pause();

    isDecFrameInfoValid = FALSE;
    bStepDec = FALSE;

    if (MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_FLUSH_DISP_QUEUE, 0xFFFFFFFF) == 0)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_FlushQueue : E_MJPEG_RET_FAIL\n");
        return E_MJPEG_RET_FAIL;
    }

    while (MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_IS_FLUSH_DONE, 0xFFFFFFFF) == 0)
    {
        printf ("Wait for flush done\n");
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_FlushQueue : E_MJPEG_RET_SUCCESS\n");
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_U32 MApi_MJPEG_GetESWritePtr(void)
{
    // return end of data
    return __u32WritePtr;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_U32 MApi_MJPEG_GetESReadPtr(void)
{
    // return end of data
    return __u32ReadPtr;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_EnableLastFrameShow(MS_BOOL bEnable)
{
    bLstFrame = bEnable;
    // This function always returns true.
    // Because MJPEG firmware does not reserve any frame.
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsDispFinish(void)
{
    // Check if the last frame decode finish,
    // Call this function to set E_MJPEG_CMD_SET_FRAME_BUFF_IDX_READY
    // when decode done.
//    MJPEG_Result ret;
    if (bLstFrame == 0)
    {
        return FALSE;
    }
//    ret = __MApi_MJPEG_CheckDecodeDone();

    if(E_MJPEG_RET_SUCCESS != __MApi_MJPEG_CheckDecodeDone())
    {
        MJPEG_DEBUG_API_MSG("MApi_MJPEG_IsDispFinish: MApi_MJPEG_CheckDecodeDone Fail!!\n");
    }


    if (MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_IS_DISP_FINISH, 0xFFFFFFFF) == 0)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_IsDispFinish : FALSE\n");
        return FALSE;
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_IsDispFinish : TRUE\n");
    return TRUE;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_SetSpeed(MJPEG_SpeedType eSpeedType, MJPEG_DispSpeed eDispSpeed)
{
    eCurSpeedType = eSpeedType;
    MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_SPEED_TYPE, (MS_U32)eSpeedType);

    eCurDispSpeed = eDispSpeed;
    MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_SPEED, (MS_U32)eDispSpeed);

    MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_STC_BASE, 0xFFFFFFFF);

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_SetFreezeDisp(MS_BOOL bEnable)
{
    MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_FREEZE_DISP, (MS_U32)bEnable);

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_SetBlueScreen(MS_BOOL bOn)
{
    MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_BLUE_SCREEN, (MS_U32)bOn);

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Reset PTS value
///@param u32Value \b IN new PTS value
/******************************************************************************/
MJPEG_Result MApi_MJPEG_ResetPTS(MS_U32 u32PtsBase)
{
    if (MDrv_JPD_ResetPTS(u32PtsBase))
        return E_MJPEG_RET_SUCCESS;

    return E_MJPEG_RET_FAIL;
}

/******************************************************************************/
///Return PTS of current displayed frame
///@return PTS
/******************************************************************************/
MS_U32 MApi_MJPEG_GetPTS(void)
{
    return MDrv_JPD_GetPTS();
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_AVSyncOn(MS_BOOL bOn, MS_U32 u32SyncDelay, MS_U16 u16SyncTolerance)
{
    MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_ENABLE_AVSYNC, (MS_U32)bOn);
    MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_AVSYNC_DELAY, (MS_U32)u32SyncDelay);
    MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_AVSYNC_TOLERENCE, (MS_U32)u16SyncTolerance);

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsAVSyncOn(void)
{
    return MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_IS_AVSYNC_ON, 0xFFFFFFFF);
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsReachSync(void)
{
    return MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_IS_REACH_AVSYNC, 0xFFFFFFFF);
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_BOOL MApi_MJPEG_GetStatus(MJPEG_Status *pStatus)
{
    pStatus->bIdle = MApi_MJPEG_IsIdle();
    //pStatus->bInit
    //pStatus->eStage = E_MJPEG_STAGE_PLAY;

    return FALSE;
}

/******************************************************************************/
///Return JPD error code
///@return JPD error code
/******************************************************************************/
MS_U32 MApi_MJPEG_GetErrCode(void)
{
    // Per XDemux's request, this only returns critical error.
    // Since no error would prevent JPD from decoding next JPEG,
    // it always return 0 here.
    MJPEG_DEBUG_API_MSG("MApi_MJPEG_GetErrCode() with eMJPEGErrCode=0x%x\n", eMJPEGErrCode);
    return eMJPEGErrCode;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_U32 MApi_MJPEG_GetErrCnt(void)
{
    return 0;
}

/******************************************************************************/
///Return count of current displayed frame
///@return frame count
/******************************************************************************/
MS_U32 MApi_MJPEG_GetFrameCnt(void)
{
    return __u32FrameCnt;
}

/******************************************************************************/
///Return video information
///@param pInfo \b OUT video information: width & height
/******************************************************************************/
MJPEG_Result MApi_MJPEG_GetDispInfo(MJPEG_DISP_INFO *pInfo)
{
    // APP wants pitch to setup display
    // This should be used with crop information
    pInfo->u16Width = __u16Pitch;
    pInfo->u16Height = __u16Height;
    pInfo->u16Pitch = __u16Pitch;
    pInfo->u32FrameRate = __u32FrameRate * 1000 / __u32FrameRateBase;

    pInfo->u16CropLeft = (pInfo->u16Width - __u16Width)/2;
    pInfo->u16CropRight = pInfo->u16Width - __u16Width - pInfo->u16CropLeft;
    pInfo->u16CropTop = (pInfo->u16Height - __u16Height)/2;
    pInfo->u16CropBottom = pInfo->u16Height - __u16Height - pInfo->u16CropTop;
    MJPEG_DEBUG_API_MSG("   Crop info right %d, left %d, bottom %d, top %d\n",
        pInfo->u16CropRight, pInfo->u16CropLeft, pInfo->u16CropBottom, pInfo->u16CropTop);

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsPlaying(void)
{
    if (MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_IS_PLAYING, 0xFFFFFFFF) == 0)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_IsPlaying : FALSE\n");
        return FALSE;
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_IsPlaying : TRUE\n");
    return TRUE;
}

/******************************************************************************/
///Return true if idle, including decode done & error
///@return idle or not
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsIdle(void)
{
    JPEG_Event u16Value = MApi_JPEG_GetJPDEventFlag();
    if ((E_JPEG_EVENT_DEC_DONE & u16Value)
    || (E_JPEG_EVENT_DEC_ERROR_MASK & u16Value))
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_IsIdle : TRUE\n");
        return TRUE;
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_IsIdle : FALSE\n");
    return FALSE;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsIFrameFound(void)
{
    return TRUE;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsWithLowDelay(void)
{
    return TRUE;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_GetDecFrameInfo(MJPEG_FrameInfo *pFrmInfo)
{
    if(FALSE == isDecFrameInfoValid)
        return E_MJPEG_RET_FAIL;

    pFrmInfo->u32LumaAddr = infoDecFrame.u32LumaAddr;
    pFrmInfo->u32ChromaAddr = infoDecFrame.u32ChromaAddr;
    pFrmInfo->u32TimeStamp = infoDecFrame.u32TimeStamp;
    pFrmInfo->u32ID_L = infoDecFrame.u32ID_L;
    pFrmInfo->u32ID_H = infoDecFrame.u32ID_H;
    pFrmInfo->u16Pitch = infoDecFrame.u16Pitch;
    pFrmInfo->u16Width = infoDecFrame.u16Width;
    pFrmInfo->u16Height = infoDecFrame.u16Height;
    pFrmInfo->eFrameType = E_MJPEG_FRM_TYPE_I;

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_GetDispFrameInfo(MJPEG_FrameInfo *pFrmInfo)
{
    #if ENABLE_AVSYNC_ID_CHECK
    static MS_U32 u32OldID_L = 0, u32OldID_H = 0, u32OldTimeStamp = 0;
    #endif
    MJPEG_FrameInfo info;
    MDrv_JPD_GetDispFrameInfo((void *)&info);

    pFrmInfo->u32LumaAddr = info.u32LumaAddr;
    pFrmInfo->u32ChromaAddr = info.u32ChromaAddr;
    pFrmInfo->u32TimeStamp = info.u32TimeStamp;
    pFrmInfo->u32ID_L = info.u32ID_L;
    pFrmInfo->u32ID_H = info.u32ID_H;
    pFrmInfo->u16Pitch = __u16Pitch;
    pFrmInfo->u16Width = __u16Width;
    pFrmInfo->u16Height = __u16Height;
    pFrmInfo->eFrameType = E_MJPEG_FRM_TYPE_I;

    #if ENABLE_AVSYNC_ID_CHECK
    if (u32OldID_H != pFrmInfo->u32ID_H || u32OldID_L != pFrmInfo->u32ID_L
        || u32OldTimeStamp != pFrmInfo->u32TimeStamp)
    {
        printf ("ID 0x%08lx%08lx    PTS %ld     addr 0x%lx\n",
            pFrmInfo->u32ID_H, pFrmInfo->u32ID_L, pFrmInfo->u32TimeStamp, pFrmInfo->u32LumaAddr);
        u32OldID_H = pFrmInfo->u32ID_H;
        u32OldID_L = pFrmInfo->u32ID_L;
        u32OldTimeStamp = pFrmInfo->u32TimeStamp;
    }
    #endif

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
void MApi_MJPEG_DbgSetMsgLevel(MJPEG_DbgLevel eDbgLevel)
{
    MS_U8 u8Level;

    switch (eDbgLevel)
    {
    case E_MJPEG_DBG_LEVEL_ERR:
        u8Level = E_JPEG_DEBUG_ERR;
        _u8MJPEG_ApiDbgLevel = E_JPEG_DEBUG_ERR;
        break;
    case E_MJPEG_DBG_LEVEL_INFO:
        u8Level = E_JPEG_DEBUG_ERR;
        _u8MJPEG_ApiDbgLevel = E_JPEG_DEBUG_MJPEG | E_JPEG_DEBUG_ERR;
        break;
    case E_MJPEG_DBG_LEVEL_DBG:
        u8Level = E_JPEG_DEBUG_ALL;
        _u8MJPEG_ApiDbgLevel = E_JPEG_DEBUG_API | E_JPEG_DEBUG_ERR;
        break;
    case E_MJPEG_DBG_LEVEL_TRACE:
        u8Level = E_JPEG_DEBUG_ALL;
        _u8MJPEG_ApiDbgLevel = E_JPEG_DEBUG_API | E_JPEG_DEBUG_ERR;
        break;
    case E_MJPEG_DBG_LEVEL_FW:
        printf ("\n   !!!Firmware debug message is not supported yet!!!\n\n");
        u8Level = E_JPEG_DEBUG_NONE;
        _u8MJPEG_ApiDbgLevel = E_JPEG_DEBUG_NONE;
        break;
    case E_MJPEG_DBG_LEVEL_NONE:
    default:
        u8Level = E_JPEG_DEBUG_NONE;
        _u8MJPEG_ApiDbgLevel = E_JPEG_DEBUG_NONE;
        break;
    }

    MApi_JPEG_SetDbgLevel(u8Level);
    MDrv_MJPEG_SetDbgLevel(u8Level);
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_DbgSetData(MS_PHYADDR u32Addr, MS_U32 u32Data)
{
    u32Addr = u32Addr;
    u32Data = u32Data;
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_DbgGetData(MS_PHYADDR u32Addr, MS_U32* u32Data)
{
    u32Addr = u32Addr;
    *u32Data = 0;

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
#if DECODER_RENDER_ARCH
MJPEG_Result MApi_MJPEG_DispFrame(MS_U32 u32DispIdx)
{
    MDrv_JPD_DispFrame(u32DispIdx);
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_FreeFrame(MS_U32 u32DispIdx)
{
    MDrv_JPD_FreeFrame(u32DispIdx);
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_GetNextDispFrame(MJPEG_FrameInfo *pFrmInfo)
{
    MJPEG_FrameInfo *pSrc = (MJPEG_FrameInfo*)MDrv_JPD_GetNextDispFrame();
    if (pSrc != NULL)
    {
        _MJPEG_Memcpy(pFrmInfo, pSrc, sizeof(MJPEG_FrameInfo));
        pFrmInfo->u16Width = __u16Pitch;
        pFrmInfo->u16Height = __u16Height;
        pFrmInfo->u16Pitch = __u16Pitch;
        pFrmInfo->eFrameType = E_MJPEG_FRM_TYPE_I;
        return E_MJPEG_RET_SUCCESS;
    }
    return E_MJPEG_RET_FAIL;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_U32 MApi_MJPEG_GetNextDispQPtr(void)
{
    return MDrv_JPD_GetNextDispQPtr();
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
#else
MJPEG_Result MApi_MJPEG_DispFrame(MS_U32 u32DispIdx)
{
    return E_MJPEG_RET_FAIL;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_FreeFrame(MS_U32 u32DispIdx)
{
    return E_MJPEG_RET_FAIL;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_GetNextDispFrame(MJPEG_FrameInfo *pFrmInfo)
{
    UNUSED(pFrmInfo);
    return E_MJPEG_RET_FAIL;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_U32 MApi_MJPEG_GetNextDispQPtr(void)
{
    return E_MJPEG_RET_FAIL;
}
#endif
MJPEG_Result MApi_MJPEG_EnableDispCmdQ(MS_BOOL bEnable)
{
    bEnableDispCmdQ = bEnable;

    MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_ENABLE_DISP_CMD_QUEUE, bEnable);

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_PushDispCmdQ(void)
{
    if (bEnableDispCmdQ)
    {
    }

    return E_MJPEG_RET_FAIL;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_U32 MApi_MJPEG_GetDispCmdQVacancy(void)
{
    if (bEnableDispCmdQ)
    {
    }

    return 0;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_SeekToPTS(MS_U32 u32PTS)
{
    __bSeekToPts = TRUE;
    __u32Skip2PtsBase = MApi_MJPEG_GetPTS();
    __u32TargetPts = u32PTS;
    MJPEG_DEBUG_API_MSG("   Seek from PTS %ld to %ld\n", __u32Skip2PtsBase, __u32TargetPts);

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_SkipToPTS(MS_U32 u32PTS)
{
    __bSkipToPts = TRUE;
    __u32Skip2PtsBase = MApi_MJPEG_GetPTS();
    __u32TargetPts = u32PTS;
    MJPEG_DEBUG_API_MSG("   Skip from PTS %ld to %ld\n", __u32Skip2PtsBase, __u32TargetPts);

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description : Enable/Disable FW turbo mode
///@param bEnable \b IN enable or not.
///@return NONE
/******************************************************************************/
void MApi_MJPEG_EnableTurboFWMode(MS_BOOL bEnable)
{
    MDrv_JPD_EnableTurboFWMode(bEnable);
}

