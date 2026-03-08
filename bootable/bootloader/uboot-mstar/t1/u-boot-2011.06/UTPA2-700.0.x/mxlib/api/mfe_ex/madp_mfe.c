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

#ifndef __UBOOT__
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#endif

#include "MFE_chip.h"
#if defined(_MFE_UTOPIA_)
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "drvMMIO.h"
#include "drvMIU.h"
#endif
#include "mfe_type.h"
#include "mfe_common.h"
#include "madp_ms_dprintf.h"

#include "mdrv_mfe_io.h"
#include "drv_mfe_st.h"
#include "madp_mfe.h"
#include "apiVOIPMFE.h"
#if defined(_SUPPORT_JPE_)
#include "Exif.h"
#include "jcmarker.h"
#endif

#if defined(__MFE_G2__)
#include "riumap.h"
#endif

#include "madp_mfe_int.h"

#include "apiMFE_priv.h"



#define CEILING_ALIGN(value, align) (((MS_U32)(value)+((align)-1UL)) & ~((align)-1UL))

#if defined(_MFE_UTOPIA_)
extern void MDrv_MFE_ISR(InterruptNum eIntNum);
#endif

#define ALIGN_TO_256(f)           (((f) + 255UL) & ~255UL)
#define mfe_fd 0UL

PVR_Info* appPVR_Info;

static MS_BOOL MAdp_MFE_Change_State(MFE_STATE* mfe_state,MFE_STATE in_state)
{

    *mfe_state = in_state;

    return TRUE;
}

MS_U8 *my_malloc(MS_VIRT* ppVAStart,MS_U32 size,const MS_S8* msg)
{
    MS_U8 *ret = (MS_U8 *)*ppVAStart;
    ms_dprintf(ADP_L2, "my_malloc[%s]: VA_START: %p, SIZE: 0x%x\n", msg, ret, (unsigned int)size);

#ifdef MEM_CHECK
            ms_dprintf(ADP_L2,"my_malloc:Check mem set.\n");
            memset(ret, 0xFF, size);
            sleep(100000);
#endif

    *ppVAStart += size;
    return ret;
}


static MS_BOOL MAdp_MFE_Proc_nonThread(MEMMAP_CUR_t input_frame)
{

    MS_U32 nDispOrder=0;    //     Driver is responsible for frame buffer re-ordering
    MS_VIRT Vaddr;
    MS_S32 Vsize=0;
    BITSFRAME_INFO bitsinfo;
#if defined(_SUPPORT_JPE_)
    MS_S32 fdc_size = 0;
#endif
    memset(&bitsinfo,0x0,sizeof(BITSFRAME_INFO));
#ifdef _MFE_M1_
    //get last frame DropFlag.
    mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_DROPFLAG,  (MS_VIRT)(&appPVR_Info->nDropFlag));

    //get current frame SkipFlag
    mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_SKIPFLAG,  (MS_VIRT)(&appPVR_Info->nSkipFlag));
#endif
    //if(appPVR_Info->nDropFlag==0)
    mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_DISPORDER, (MS_VIRT)&nDispOrder);
#if defined(_MFE_UTOPIA_)
    if(appPVR_Info->enableISR) {
        mfe_open_int();
        MsOS_EnableInterrupt((InterruptNum)E_INT_IRQ_MFE);
    }
#endif
    mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_ENC_ONEFRAME, (MS_VIRT)&input_frame);

    if(appPVR_Info->nSkipFlag==1) {
        ms_dprintf(ADP_L3,"SkipFrame\n");
#ifndef _GenSkipHeader_
        return TRUE;
#endif
    }
    Vsize = 0;
#if defined(_SUPPORT_JPE_)
    if(appPVR_Info->nCodecType == JPEG_ENC)  {//jpeg
        //FDC_t fdc;
        MS_S32 q_table[128];
        mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_QTABLE, (MS_VIRT)&q_table);
//        set_fdc(appPVR_Info->nDispWidth,appPVR_Info->nDispHeight);
        get_fdc(appPVR_Info->BitstreamOutBuffer.miuPointer,&fdc_size,appPVR_Info->nDispWidth,appPVR_Info->nDispHeight,
            &q_table[0],&q_table[64],appPVR_Info->exif_enable);

        ms_dprintf(ADP_L3,"header size = 0x%x\n",fdc_size);
        //memcpy(appPVR_Info->BitstreamOutBuffer.miuPointer,&fdc,fdc_size);
        Vsize = fdc_size;
    }
#endif
#ifdef _SW_BUF_MODE_
    if(appPVR_Info->m_bFrameMode == 0) {
         ms_dprintf(ADP_L3,"fake rowdone\n");
        mfe_fake_row_mode(input_frame,appPVR_Info->nBufWidth,appPVR_Info->nBufHeight);
    }
#endif
//    MsOS_DelayTask(20); //delay 2 ms
    //get bits

    while(1) {
#ifdef _GenSkipHeader_
        if(appPVR_Info->nSkipFlag==0)
#endif

        mfe_wait_int(appPVR_Info->enableISR);

        mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_GETBITS, (MS_VIRT)(&bitsinfo));
        MS_ASSERT(bitsinfo.size >= 0);
        if(appPVR_Info->enableSecurityMode){
            Vsize = bitsinfo.size;
            goto NONCOPY;
        }
        if (appPVR_Info->reserved0) // cached memory
            Vaddr  = (MS_VIRT) MAdp_MPool_PA2KSEG0(bitsinfo.miuAddress+appPVR_Info->MfeApiInfo.miu_offset);
        else // non-cached memory
            Vaddr  = (MS_VIRT) MAdp_MPool_PA2KSEG1(bitsinfo.miuAddress+appPVR_Info->MfeApiInfo.miu_offset);

        ms_dprintf(ADP_L3, "bitsinfo.miuAddress = 0x%x, Vaddr = 0x"MPRI_VIRT", size = 0x%x\n", (unsigned int)bitsinfo.miuAddress, (ptrdiff_t)Vaddr, (unsigned int)bitsinfo.size);
        if((appPVR_Info->BitstreamOutBufferSize-Vsize) < bitsinfo.size) {
            ms_dprintf(ADP_L0,"BitstreamOutBufferSize is too small\n");
            return FALSE;
        }
        if (appPVR_Info->reserved0) { // cached memory
           MsOS_Dcache_Flush(Vaddr, CEILING_ALIGN(bitsinfo.size, 16UL));
        }
        memcpy(appPVR_Info->BitstreamOutBuffer.miuPointer +Vsize, (MS_U8 *)Vaddr, bitsinfo.size);
        Vsize += bitsinfo.size;

        ms_dprintf(ADP_L3, "memcpy 0x%x, total 0x%x\n", (unsigned int)bitsinfo.size, (unsigned int)Vsize);
        if(bitsinfo.status == BITS_INFO_FRAMEDONE) {//frame done
            if(appPVR_Info->enableCABAC) {
                //CABAC stuffing byte
                if(bitsinfo.stuffing_bytes!=0) {
                    MS_S32 i;
                    MS_U8 *obufp = appPVR_Info->BitstreamOutBuffer.miuPointer + Vsize;

                    ms_dprintf(ADP_L3, "CABAC stuffing byte 0x%x\n", (unsigned int)bitsinfo.stuffing_bytes);
                    for(i=0;i<bitsinfo.stuffing_bytes/3;i++) {
                        *obufp++ = 0x0;
                        *obufp++ = 0x0;
                        *obufp++ = 0x3;
                    }
                    for(i=0;i<bitsinfo.stuffing_bytes%3;i++) {
                        *obufp++ = 0x0;
                    }

                }
                Vsize += bitsinfo.stuffing_bytes;
            }
            ms_dprintf(ADP_L3,"frame done\n");
            break;
        }
        else if(bitsinfo.status == BITS_INFO_BUFFULL) {
            ms_dprintf(ADP_L3,"frame full\n");
            mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_CLEAR_BITSBUF, (MS_VIRT)1);
        }
        else if (bitsinfo.status == BITS_INFO_IMG_BUFFULL){
            ms_dprintf(ADP_L0,"input buf full\n");
            return FALSE;
        } else {
            ms_dprintf(ADP_L0,"[MFE]unknown state\n");
            return FALSE;
        }
    }

NONCOPY:
    mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_CLEAR_BITSBUF, (MS_VIRT)1);
    appPVR_Info->BitstreamOutBuffer.size = Vsize;
    if(Vsize)
        appPVR_Info->g_StreamCallBackFunc(appPVR_Info->g_pCustomCtx, (MS_S32)bitsinfo.voptype,
            appPVR_Info->BitstreamOutBuffer.miuPointer, Vsize, 0x3);

    appPVR_Info->g_CleanCallBackFunc(1, (MS_VIRT)&input_frame);

    ms_dprintf(ADP_L2, "frame size = 0x%x\n", (unsigned int)appPVR_Info->BitstreamOutBuffer.size);

#if (DEBUG_LEVEL & ADP_L3)
    mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_DROPFLAG,  (MS_VIRT)(&appPVR_Info->nDropFlag));
    if(appPVR_Info->nDropFlag)
        ms_dprintf(ADP_L3,"Dropframe.");
#endif
    return TRUE;
}

MS_BOOL MFE_SetRIU_Base(void)
{

#ifdef __MFE_G2__
    MS_VIRT u32RiuBaseAdd=0;
    MsIoMapInit();
    u32RiuBaseAdd = MsIoMapGetRiuBase(RIUBASE_MFE0);

    ms_dprintf(ADP_L2,"MFE Init u32RiuBaseAdd = 0x%X\n", (MS_U32)u32RiuBaseAdd);
#elif defined(WIN32)
    MS_VIRT u32RiuBaseAdd=0xa80;
    MS_PHY u32NonPMBankSize=0;
#else
    MS_VIRT u32RiuBaseAdd=0;
    MS_PHY u32NonPMBankSize=0;

    if( !MDrv_MMIO_GetBASE( &u32RiuBaseAdd, &u32NonPMBankSize, MS_MODULE_HW)){
        ms_dprintf(ADP_L0,"MFE Init Err: MMIO_GetBASE failure\n");
        return FALSE;
    }

    ms_dprintf(ADP_L2, "MFE Init u32RiuBaseAdd = 0x%tx, u32NonPMBankSize = 0x%tx\n",
        (ptrdiff_t)u32RiuBaseAdd, (ptrdiff_t)u32NonPMBankSize);
    //MDrv_MFE_SetOSRegBase(u32RiuBaseAdd);
#endif
     mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_SET_RIU_BASE, u32RiuBaseAdd);

    return TRUE;

}


MS_BOOL MApi_MFE_CompressOnePicture_ex(MEMMAP_CUR_t YUVPlane, MS_BOOL bForceIframe)
{
    return _MApi_MFE_CompressOnePicture(appPVR_Info, YUVPlane, bForceIframe);
}

MS_BOOL MApi_MFE_Encode(void)
{
    return _MApi_MFE_Encode(appPVR_Info);
}

MS_BOOL MApi_MFE_DeInitialize(void)
{

    // old api didnot support multi instance, so always power off mfe when deInit.
    POWERON_t power_Info;
    power_Info.is_off = 1;
    power_Info.clock_level = MFE_CLK_FAST;
    mfe_ioctl((MS_VIRT)NULL, MFE_IOC_POWEROFF, (MS_VIRT)&power_Info);
    return _MApi_MFE_DeInit(appPVR_Info);
}


MFE_STATE MApi_MFE_GetState(void)
{
    return appPVR_Info->mfe_state;
}
MS_BOOL MApi_MFE_GetVOL_ex(void *header_info)
{
    return _MApi_MFE_GetVOL(appPVR_Info,header_info);
}

MS_BOOL MApi_MFE_Initialize_ex(PVR_Info* in_Info,StreamCallback out_cb,CleanCallback
mfe_CleanBufStatus, void *ctx)
{

    MS_BOOL ret;
    ret = _MApi_MFE_Init(in_Info);
    if(ret == FALSE)
        return FALSE;
    appPVR_Info->g_pCustomCtx = ctx;
    appPVR_Info->g_StreamCallBackFunc = out_cb;
    appPVR_Info->g_CleanCallBackFunc = mfe_CleanBufStatus;

    return ret;

}


MS_BOOL MApi_MFE_SetConfig(MFE_ENC_CONF conf_type, void *value)
{

    MS_S32 set_value;
    if(appPVR_Info->mfe_state != MFE_Idle) {
        ms_dprintf(ADP_L0,"state check error.[%d]\n",appPVR_Info->mfe_state);
        return FALSE;
    }

    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Busy);

    switch (conf_type) {
        case MFE_ENC_SETCONF_FRAME_TYPE:
            set_value = (*((MS_S32*) value));
            ms_dprintf(ADP_L1, "MApi_MFE_SetConfig %d\n", (int)set_value);
            if(set_value == I_VOP)
                mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_SET_FORCEDI, (MS_VIRT)NULL);
            break;
        case MFE_ENC_SETCONF_CHANGE_FRAME_RATE:
            set_value = (*((MS_S32*) value));
            if(set_value == 0) {
                ms_dprintf(ADP_L0,"framerate cannot be 0.\n");
                MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
                return FALSE;
            }
            appPVR_Info->FrameRatex100 = set_value;
            mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_RESET_FRAMERATE,  (MS_VIRT)(appPVR_Info));
            break;
        case MFE_ENC_SETCONF_CHANGE_BIT_RATE:
            set_value = (*((MS_S32*) value));
            if(set_value == 0) {
                ms_dprintf(ADP_L0,"bitrate cannot be 0.\n");
                MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
                return FALSE;
            }
            appPVR_Info->nBitrate =  set_value;
            mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_RESET_BITRATE,  (MS_VIRT)(appPVR_Info));
            break;
        case MFE_ENC_SETCONF_CHANGE_BIT_RATE_AND_FRAME_RATE:
            {
                PVR_Info* in_Info = (PVR_Info*)value;
                appPVR_Info->nBitrate =  in_Info->nBitrate;
                appPVR_Info->FrameRatex100 =  in_Info->FrameRatex100;
                if(_MApi_MFE_SetBitrateFramerate(appPVR_Info) == FALSE)
                    return FALSE;
#if 0
                if(in_Info->nBitrate== 0 ) {
                    ms_dprintf(ADP_L0,"bitrate cannot be 0.\n");
                    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
                    return FALSE;
                }
                if(in_Info->FrameRatex100 == 0) {
                    ms_dprintf(ADP_L0,"framerate cannot be 0.\n");
                    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
                    return FALSE;
                }
                appPVR_Info->nBitrate =  in_Info->nBitrate;
                appPVR_Info->FrameRatex100 =  in_Info->FrameRatex100;
                mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_RESET_BITRATE_AND_FRAMERATE,  (MS_VIRT)(appPVR_Info));
#endif
            }
            break;
        case MFE_ENC_SETCONF_FRAME_TAG:
            appPVR_Info->out_frametag_top = (*((MS_S32*) value));
            break;
        case MFE_ENC_SETCONF_RESET_SPSPPS:
            mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_RESET_SPSPPS, (MS_VIRT)NULL);
            break;
        case MFE_ENC_SETCONF_OUT_BSP_BUFFER:
        {
            MEMMAP_t* outbuf = (MEMMAP_t *)value;
            if(outbuf->miuPointer == NULL || outbuf->size == 0) {
                ms_dprintf(ADP_L0,"outbuf->miuPointer == NULL || outbuf->size == 0\n");
                MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
                return FALSE;
            }
            appPVR_Info->BitstreamOutBuffer.miuPointer = outbuf->miuPointer;
            appPVR_Info->BitstreamOutBuffer.miuAddress = outbuf->miuAddress;
            appPVR_Info->BitstreamOutBuffer.size = 0;
            appPVR_Info->BitstreamOutBufferSize = outbuf->size;
            break;
        }
        case MFE_ENC_SETCONF_JPE_QUALITY:
            set_value = (*((MS_S32*) value));
            if(set_value <0 || set_value > 100) {
                ms_dprintf(ADP_L0, "quality does not support %d\n", (int)set_value);
                MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
                return FALSE;
            }
            appPVR_Info->quality = (MS_S32)set_value;
            mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_SET_QUALITY,  (MS_VIRT)(set_value));
            break;
        case MFE_ENC_SETCONF_SET_COLORFORMAT:
#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_)
            set_value = (*((MS_S32*) value));
            appPVR_Info->bColorFormat =  (MFE_COLOR_FORMAT_INFO)set_value;
            mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_SET_COLOR_FORMAT,  (MS_VIRT)(appPVR_Info));
            break;
#else
            ms_dprintf(ADP_L0,"Not support MFE_ENC_SETCONF_SET_COLORFORMAT on this chip\n");
            MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
            return FALSE;
#endif
        default:
            ms_dprintf(ADP_L0,"conf_type error\n");
            MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
            return FALSE;
    }

    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);

    return TRUE;
}

MS_BOOL MApi_MFE_GetConfig(MFE_ENC_CONF conf_type, void *value)
{

    if(appPVR_Info->mfe_state != MFE_Idle) {
        ms_dprintf(ADP_L0,"state check error.[%d]\n",appPVR_Info->mfe_state);
        return FALSE;
    }

    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Busy);

    switch (conf_type) {
        case MFE_ENC_GETCONF_FRAME_TAG:
            (*((MS_U32*) value)) = appPVR_Info->out_frametag_top;
            break;
        case MFE_ENC_GETCONF_OUT_BSP_BUFFER:
        {
            //memcpy(value,&appPVR_Info->BitstreamOutBuffer,sizeof(MEMMAP_t));
            _MApi_MFE_GetOBuf(appPVR_Info,(MEMMAP_t*)value);
            ms_dprintf(ADP_L2, "appPVR_Info->BitstreamOutBuffer.size = 0x%x\n", (unsigned int)appPVR_Info->BitstreamOutBuffer.size);
            break;
        }
        case MFE_ENC_GETCONF_JPE_QUALITY:
            (*((MS_S32*) value)) = appPVR_Info->quality;
            break;
        case MFE_ENC_GETCONF_DROP:
            mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_DROPFLAG,  (MS_VIRT)(&appPVR_Info->nDropFlag));
            *((MS_U32 *)value) = appPVR_Info->nDropFlag;
            break;
        default:
            ms_dprintf(ADP_L0,"conf_type error\n");
            MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
            return FALSE;
    }

    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);

    return TRUE;
}

MS_BOOL MApi_MFE_GetHWCap(MFE_HwCapV0 *pDrv, MS_U32 *len)
{
    return _MApi_MFE_GetHWCap(appPVR_Info, pDrv, len);
}

MS_BOOL _MApi_MFE_Init(PVR_Info* mfe_Info)
{
    ms_dprintf(ADP_L1,"_MApi_MFE_Init\n");
    MS_U32* temp;
    MS_U32 nFrameSize;
    MS_U32 MallocToDrvSize ;
    MS_U32 MallocToDrvFrame;
    appPVR_Info = mfe_Info;

    // block re-init
    if (appPVR_Info->mfe_state != MFE_Invalid && appPVR_Info->mfe_state != MFE_DeInit) {
        ms_dprintf(ADP_L0, "Already initialized. Re-initialization is not allowed.\n");
        return FALSE;
    }

    //parameter check
    if((appPVR_Info->nCodecType != MPEG4_ENC) && (appPVR_Info->nCodecType != H263_ENC)
        && (appPVR_Info->nCodecType != H264_ENC) && (appPVR_Info->nCodecType != JPEG_ENC )){
        ms_dprintf(ADP_L0,"nCodecType error\n");
        return FALSE;
    }

    //video parameter
    if(appPVR_Info->nCodecType != JPEG_ENC){
        if(appPVR_Info->FrameRatex100/100 <= 0) {
                ms_dprintf(ADP_L0, "FrameRatex100 cannot be %d\n", (int)appPVR_Info->FrameRatex100);
                return FALSE;
        }
        if(appPVR_Info->nBbetweenP != 0) {
                ms_dprintf(ADP_L0, "nBbetweenP cannot be %d\n", (int)appPVR_Info->nBbetweenP);
                return FALSE;
        }
        //if(appPVR_Info->nBitrate <= 0) {
        //    ms_dprintf(ADP_L0,"nBitrate cant be %d\n",appPVR_Info->nBitrate);
        //    return FALSE;
        //}
        if(appPVR_Info->width==0 ||appPVR_Info->height == 0) {
            ms_dprintf(ADP_L0,"appPVR_Info->width==0 ||appPVR_Info->height == 0\n");
            return FALSE;
        }
#ifdef MFE_SUPPORT_1080P
        if(appPVR_Info->height > 1088) {
            ms_dprintf(ADP_L0,"appPVR_Info->height > 1088\n");
            return FALSE;
        }
        if(appPVR_Info->width > 1920) {
            ms_dprintf(ADP_L0,"appPVR_Info->width > 1920\n");
            return FALSE;
        }
#else
        if(appPVR_Info->height > 720) {
            ms_dprintf(ADP_L0,"appPVR_Info->height > 720\n");
            return FALSE;
        }
        if(appPVR_Info->width > 1280) {
            ms_dprintf(ADP_L0,"appPVR_Info->width > 1280\n");
            return FALSE;
        }
#endif
        if(appPVR_Info->nCodecType == MPEG4_ENC){
            if(appPVR_Info->TimeIncreamentRes==0 || appPVR_Info->VopTimeIncreament == 0)
                return FALSE;
        }
        if(appPVR_Info->m_bFrameMode != 1) {
            ms_dprintf(ADP_L0,"appPVR_Info->m_bFrameMode != 1\n");
             return FALSE;
        }
    }
    else{ //jpeg
        if((appPVR_Info->quality < 1) || (appPVR_Info->quality > 100)) {
            ms_dprintf(ADP_L0, "quality cannot be %d\n", (int)appPVR_Info->quality);
            return FALSE;
        }
        if(appPVR_Info->height > 2592) {
            ms_dprintf(ADP_L0, "height cannot be %d\n", (int)appPVR_Info->height);
            return FALSE;
        }
        if(appPVR_Info->width > 2960) {
            ms_dprintf(ADP_L0, "width cannot be %d\n", (int)appPVR_Info->width);
            return FALSE;
        }
    }

    if(!appPVR_Info->enableSecurityMode && appPVR_Info->BitstreamOutBuffer.miuPointer == NULL) {
        ms_dprintf(ADP_L0,"appPVR_Info->BitstreamOutBuffer.miuPointer == NULL\n");
        return FALSE;
    }
    if(appPVR_Info->enableSecurityMode) {
        ms_dprintf(ADP_L1,"enableSecurityMode\n");
    }
    if((appPVR_Info->MfeAdvInfo.input_imi_en!=0) && (appPVR_Info->MfeAdvInfo.low_bandwidth_en!=0)) {
        ms_dprintf(ADP_L0, "MfeAdvInfo.input_imi_en = %u, MfeAdvInfo.low_bandwidth_en = %u\n",
            (unsigned int)appPVR_Info->MfeAdvInfo.input_imi_en, (unsigned int)appPVR_Info->MfeAdvInfo.low_bandwidth_en);
        return FALSE; //cannot enable at the same time.
    }
    if(appPVR_Info->bInterlace != PROGRESSIVE) {
        if(appPVR_Info->nBufHeight % 32 != 0){
            ms_dprintf(ADP_L0,"Height must %% 32 for interlace mode\n");
            return FALSE;
        }
    }
#if defined(_MFE_EDISON_) //eagle only support MSTTILE
    if(appPVR_Info->bColorFormat == YUVTILE) {
        ms_dprintf(ADP_L4,"Only support MSTTILE, force to MSTTILE.\n");
        appPVR_Info->bColorFormat = MSTTILE;
    }
#endif
    ms_dprintf(ADP_L2,"video parameter check done\n");
    //set RIU BASE
    if(MFE_SetRIU_Base() == FALSE) {
        ms_dprintf(ADP_L0,"MFE_SetRIU_Base() == FALSE\n");
        return FALSE;
    }

    appPVR_Info->nDropFlag = 0;

    appPVR_Info->nOutBuffer = OBUF_NUM;
    //appPVR_Info->OutBufferSize = OBUF_SIZE;
    appPVR_Info->OutBufferSize = appPVR_Info->BitstreamOutBufferSize;

    if((appPVR_Info->nBufHeight*appPVR_Info->nBufWidth) >= (1280*720))
        MallocToDrvFrame = MAX_BUFPOOL_COUNT -1; // Only 1 ref.
    else
        MallocToDrvFrame = MAX_BUFPOOL_COUNT;

    if(appPVR_Info->nCodecType == H264_ENC) {
        MS_U32 out_buf_size = appPVR_Info->OutBufferSize*appPVR_Info->nOutBuffer;
        if(appPVR_Info->bColorFormat == YUVTILE) {
            nFrameSize = appPVR_Info->nBufHeight*appPVR_Info->nBufWidth*3/2;
            if(appPVR_Info->bInterlace != PROGRESSIVE)
                nFrameSize = (appPVR_Info->nBufHeight>>1)*appPVR_Info->nBufWidth*3/2;
        } else { // MSTTILE or YUV422
            MS_S32 DstSizeY = CEILING_ALIGN(appPVR_Info->nBufHeight,32)*appPVR_Info->nBufWidth;
            MS_S32 DstSizeUV = CEILING_ALIGN(appPVR_Info->nBufHeight,64)*appPVR_Info->nBufWidth/2;
            nFrameSize = DstSizeY + DstSizeUV;
            if(appPVR_Info->bInterlace != PROGRESSIVE) {
                MS_S32 DstSizeY = CEILING_ALIGN(appPVR_Info->nBufHeight/2,32)*appPVR_Info->nBufWidth;
                MS_S32 DstSizeUV = CEILING_ALIGN(appPVR_Info->nBufHeight/2,64)*appPVR_Info->nBufWidth/2;
                nFrameSize = DstSizeY + DstSizeUV;
            }
        }
#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_)
            MallocToDrvSize = nFrameSize*MallocToDrvFrame
                + out_buf_size
                + (appPVR_Info->nBufWidth>>4)*128 + 10*1024;
#else
            MallocToDrvSize = nFrameSize*MallocToDrvFrame
                + out_buf_size
                + (appPVR_Info->nBufWidth>>4)*64 + 10*1024;
#endif
    }
    else if(appPVR_Info->nCodecType == H263_ENC || appPVR_Info->nCodecType == MPEG4_ENC) {
        nFrameSize = appPVR_Info->nBufHeight*appPVR_Info->nBufWidth*3/2;
        MallocToDrvSize = nFrameSize*MallocToDrvFrame+appPVR_Info->OutBufferSize*appPVR_Info->nOutBuffer +
             (appPVR_Info->nBufWidth>>4)*64 +
             appPVR_Info->nBufHeight*appPVR_Info->nBufWidth/256*16 + 10*1024;
    }
    else {
        appPVR_Info->MfeApiInfo.mfe_phy_buf_size = appPVR_Info->MfeApiInfo.mfe_phy_buf_size -
            (ALIGN_TO_256(appPVR_Info->MfeApiInfo.mfe_phy_buf_addr) -appPVR_Info->MfeApiInfo.mfe_phy_buf_addr);
        appPVR_Info->MfeApiInfo.mfe_phy_buf_addr = ALIGN_TO_256(appPVR_Info->MfeApiInfo.mfe_phy_buf_addr);

        appPVR_Info->nOutBuffer = OBUF_NUM;
        appPVR_Info->OutBufferSize = MallocToDrvSize = appPVR_Info->MfeApiInfo.mfe_phy_buf_size;
    }

    if(appPVR_Info->enableSecurityMode) {
        MallocToDrvSize = MallocToDrvSize - (appPVR_Info->OutBufferSize*appPVR_Info->nOutBuffer);
    }
    ms_dprintf(ADP_L2, "mfe_phy_buf_size = 0x%x, MallocToDrvSize = 0x%x\n",
        (unsigned int)appPVR_Info->MfeApiInfo.mfe_phy_buf_size, (unsigned int)MallocToDrvSize);
    if(appPVR_Info->MfeApiInfo.mfe_phy_buf_size < MallocToDrvSize) {
        ms_dprintf(ADP_L0,"appPVR_Info->MfeApiInfo.mfe_phy_buf_size < MallocToDrvSize\n");
        return FALSE;
    }
    // malloc memory for DRV level
    temp = (MS_U32*)my_malloc(&appPVR_Info->MfeApiInfo.mfe_vir_buf_ptr, MallocToDrvSize, (const MS_S8*)("pretend to allocate for MFE driver"));

    //pretend to allocate for MFE
    appPVR_Info->BitsBuffStart = appPVR_Info->MfeApiInfo.mfe_phy_buf_addr;
    appPVR_Info->BitsBuffSize = MallocToDrvSize;
    appPVR_Info->BitsBuffStart -= (appPVR_Info->BitsBuffStart & appPVR_Info->MfeApiInfo.miu_offset);

    appPVR_Info->MfeApiInfo.Encode_stop = 0;
    appPVR_Info->mfe_drvHandle = (void*)MfeDrvMemMalloc(sizeof(MFE_CONFIG), (const MS_S8*)"mfe_drvHandle");
    if ( mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_INIT, (MS_VIRT)(appPVR_Info)) != 0)
        return FALSE;

    if(!appPVR_Info->enableSecurityMode)
        mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_SET_MIU_OFFSET, (MS_VIRT)appPVR_Info->MfeApiInfo.miu_offset);

    if(appPVR_Info->enableISR) {
        ms_dprintf(ADP_L1,"MFE open interrupt\n");
        mfe_open_int();
    }
#if defined(_SUPPORT_JPE_)
    if(appPVR_Info->nCodecType == JPEG_ENC) {
        init_fdc();
        set_fdc(appPVR_Info->nDispWidth,appPVR_Info->nDispHeight);
    }
#endif

    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Init);

    return TRUE;

}
MS_BOOL _MApi_MFE_Encode(PVR_Info* mfe_Info)
{
    appPVR_Info = mfe_Info;
    ms_dprintf(ADP_L1,"_MApi_MFE_Encode\n");
    if(appPVR_Info->mfe_state == MFE_Init)
        MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Busy);
    else {
        ms_dprintf(ADP_L0,"state check error.[%d]\n",appPVR_Info->mfe_state);
        return FALSE;
    }
    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);

    return TRUE;
}
MS_BOOL _MApi_MFE_GetVOL(PVR_Info* mfe_Info, void *header_info)
{
    ms_dprintf(ADP_L1,"_MApi_MFE_GetVOL\n");
    appPVR_Info = mfe_Info;
    if(appPVR_Info->mfe_state != MFE_Idle) {
        ms_dprintf(ADP_L0,"state check error.[%d]\n",appPVR_Info->mfe_state);
        return FALSE;
    }


    if(header_info == NULL) {
        ms_dprintf(ADP_L0,"header_info == NULL\n");
        return FALSE;
    }


    //MPEG4
    if(appPVR_Info->nCodecType == MPEG4_ENC) {
        VOL_INFO_t *vol_info = header_info;
        vol_info->pHeader = appPVR_Info->BitstreamOutBuffer.miuPointer;
        mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_VOL, (MS_VIRT)vol_info);
    }
    else if(appPVR_Info->nCodecType == H264_ENC) { // H264
        SPSPPS_INFO_t *spspps_info = header_info;
        spspps_info->pHeaderSPS = appPVR_Info->BitstreamOutBuffer.miuPointer;
        mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_VOL, (MS_VIRT)spspps_info);
        appPVR_Info->BitstreamOutBuffer.size = spspps_info->PPSLen + spspps_info->SPSLen;
    }
    else {
        ms_dprintf(ADP_L0, "non-support codec type %d\n", (int)appPVR_Info->nCodecType);
        return FALSE;
    }

    return TRUE;
}
MS_BOOL _MApi_MFE_CompressOnePicture(PVR_Info* mfe_Info, MEMMAP_CUR_t YUVPlane, MS_BOOL bForceIframe)
{

    MS_BOOL ret;
    appPVR_Info = mfe_Info;
    if(appPVR_Info->mfe_state != MFE_Idle) {
        ms_dprintf(ADP_L0,"state check error.[%d]\n",appPVR_Info->mfe_state);
        return FALSE;
    }

    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Busy);

    ms_dprintf(ADP_L2, "input Y0: VA %p, PA 0x%x\n", YUVPlane.Cur_Y0.miuPointer, (unsigned int)YUVPlane.Cur_Y0.miuAddress);
    ms_dprintf(ADP_L2, "input C0: VA %p, PA 0x%x\n", YUVPlane.Cur_C0.miuPointer, (unsigned int)YUVPlane.Cur_C0.miuAddress);

    //check input alignment
    if(appPVR_Info->bColorFormat == YUVTILE || appPVR_Info->bColorFormat == MSTTILE) {
        if((YUVPlane.Cur_Y0.miuAddress & 0xFF) !=0)
            return FALSE;
        if((YUVPlane.Cur_C0.miuAddress & 0xFF) !=0)
            return FALSE;
    } else { //422 only need 8 byte align
        if((YUVPlane.Cur_Y0.miuAddress & 0x7) !=0)
            return FALSE;
    }
    if(appPVR_Info->m_bFrameMode==0) {
        if((YUVPlane.Cur_Y1.miuAddress & 0xFF) !=0)
            return FALSE;
        if((YUVPlane.Cur_C1.miuAddress & 0xFF) !=0)
            return FALSE;
    }
    if(bForceIframe) {
        mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_SET_FORCEDI, (MS_VIRT)NULL);
    }
    //sem_post(&madp_sem_input);
    ret = MAdp_MFE_Proc_nonThread(YUVPlane);
    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
    return ret;

}
MS_BOOL _MApi_MFE_DeInit(PVR_Info* mfe_Info)
{

    MS_S32 err;
    MS_U32 temp=0;
    appPVR_Info = mfe_Info;
    if(appPVR_Info->mfe_state != MFE_Idle) {
        ms_dprintf(ADP_L4,"MApi_MFE_DeInitialize state check error.[%d] \
            Still Force deinitialize the mfe..\n",appPVR_Info->mfe_state);
    }

    MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_DeInit);

    appPVR_Info->MfeApiInfo.Encode_stop = 1;
    err = mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_FINISH, (MS_VIRT)temp);
    MfeDrvMemFree(appPVR_Info->mfe_drvHandle, (const MS_S8*)("mfe_drvHandle"));
    if(appPVR_Info->enableISR) {
        ms_dprintf(ADP_L1,"MFE close interrupt\n");
        mfe_close_int();
    }
#if defined(_SUPPORT_JPE_)
    if(appPVR_Info->nCodecType == JPEG_ENC)
        deinit_fdc();
#endif
    appPVR_Info = NULL;
    //MfeDrvMemFree(appPVR_Info, (const MS_S8*)("appPVR_Info"));
    //MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Invalid);


    return (err == 0);


}

MS_BOOL _MApi_MFE_GetOBuf(PVR_Info* mfe_Info,MEMMAP_t* obuf)
{
    appPVR_Info = mfe_Info;
    memcpy(obuf,&appPVR_Info->BitstreamOutBuffer,sizeof(MEMMAP_t));

    return TRUE;
}

MS_BOOL _MApi_MFE_SetBitrateFramerate(PVR_Info* mfe_Info)
{

    appPVR_Info = mfe_Info;
    if(appPVR_Info->nBitrate== 0 ) {
        ms_dprintf(ADP_L0,"bitrate cannot be 0.\n");
        MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
        return FALSE;
    }
    if(appPVR_Info->FrameRatex100 == 0) {
        ms_dprintf(ADP_L0,"framerate cannot be 0.\n");
        MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
        return FALSE;
    }
    mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_RESET_BITRATE_AND_FRAMERATE,  (MS_VIRT)(appPVR_Info));

    return TRUE;
}

MS_BOOL _MApi_MFE_PowerOff(PVR_Info* mfe_Info)
{

    POWERON_t power_Info;
    appPVR_Info = mfe_Info;
    power_Info.is_off = 1;
    power_Info.clock_level = MFE_CLK_FAST;
    mfe_ioctl((MS_VIRT)NULL, MFE_IOC_POWEROFF, (MS_VIRT)&power_Info);

    return TRUE;
}

MS_BOOL _MApi_MFE_ISREnable(PVR_Info* mfe_Info)
{

    appPVR_Info = mfe_Info;
    if(mfe_Info->enableISR) {
        mfe_open_int();
    } else {
        mfe_close_int();
    }
    mfe_ioctl((MS_VIRT)NULL, MFE_IOC_SET_ISR, (MS_VIRT)(appPVR_Info));
    return TRUE;
}

MS_BOOL _MApi_MFE_SetColorFormat(PVR_Info* mfe_Info)
{

    appPVR_Info = mfe_Info;
#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_)
            mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_SET_COLOR_FORMAT,  (MS_VIRT)(appPVR_Info));
#else
            ms_dprintf(ADP_L0,"Not support MFE_ENC_SETCONF_SET_COLORFORMAT on this chip\n");
            MAdp_MFE_Change_State(&appPVR_Info->mfe_state,MFE_Idle);
            return FALSE;
#endif

    return TRUE;
}

MS_BOOL _MApi_MFE_ResetSPSPPS(PVR_Info* mfe_Info)
{
    appPVR_Info = mfe_Info;
    mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_RESET_SPSPPS, (MS_VIRT)NULL);

    return TRUE;
}

MS_BOOL _MApi_MFE_SetFrameType(PVR_Info* mfe_Info, MS_S32 frametype)
{
    appPVR_Info = mfe_Info;
    if(frametype == I_VOP)
        mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_SET_FORCEDI, (MS_VIRT)NULL);

    return TRUE;
}

MS_BOOL _MApi_MFE_GetHWCap(PVR_Info* mfe_Info, MFE_HwCapV0 *pDrv, MS_U32 *len)
{

    MS_U16 u16CopiedLength = sizeof(MFE_HwCapV0);
    MFE_HwCapV0 DrvHW;
    appPVR_Info = mfe_Info;
    mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_HWCAP,  (MS_VIRT)(&DrvHW));

    if(pDrv->u16ApiHW_Length == 0){
        ms_dprintf(ADP_L0, "Null paramter or Wrong u16ApiHW_Length!!\n");
        return 0;
    }

    if(pDrv->u32ApiHW_Version < 1){
        ms_dprintf(ADP_L0, "Wrong u32ApiHW_Version please check!!\n");
        return 0;
    }

    //new AP + old lib, only the length corresponding to old lib has meaning.
    if(pDrv->u32ApiHW_Version > API_HWCAP_VERSION0){
        ms_dprintf(ADP_L2, "old lib version has only length: %u\n", (unsigned int)sizeof(MFE_HwCapV0));
        ms_dprintf(ADP_L2, "we still copy the min size of both structure, but AP should take care of it.\n");
        u16CopiedLength = sizeof(MFE_HwCapV0);
    }

    //old AP + new lib, driver shouldn't access to the space which doesn't exist in old structure
    if((pDrv->u32ApiHW_Version < API_HWCAP_VERSION0) || (pDrv->u16ApiHW_Length < sizeof(MFE_HwCapV0))){
        ms_dprintf(ADP_L2, "new lib version access by old structure!!\n");
        u16CopiedLength = pDrv->u16ApiHW_Length;
    }

    DrvHW.u32ApiHW_Version = API_HWCAP_VERSION0;
    DrvHW.u16ApiHW_Length = u16CopiedLength;
    DrvHW.bIsSupportEncoder = TRUE;
    DrvHW.u8MaxEncodeFrame = 0;
    DrvHW.bSupportSubVidWin = FALSE;
    DrvHW.bSupportMpeg4 = FALSE;
    DrvHW.bSupportH264 = TRUE;
    memcpy(pDrv, &DrvHW, u16CopiedLength);
    *len = u16CopiedLength;
    return TRUE;
}

MS_BOOL _MApi_MFE_GetOutputInfo(PVR_Info *mfe_Info, MS_S32 *frametype, MS_S32 *size)
{
    appPVR_Info = mfe_Info;
    if (appPVR_Info->mfe_state != MFE_Idle) {
        ms_dprintf(ADP_L0, "state check error.[%d]\n", appPVR_Info->mfe_state);
        return FALSE;
    }
    *size = appPVR_Info->BitstreamOutBuffer.size;
    mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_GET_FRAME_TYPE, (MS_VIRT)(frametype));

    return TRUE;
}

MS_BOOL _MApi_MFE_SetVUI(PVR_Info *mfe_Info, VUI_Info* vui_info)
{
    appPVR_Info = mfe_Info;
    if (appPVR_Info->mfe_state != MFE_Idle && appPVR_Info->mfe_state != MFE_Init) {
        ms_dprintf(ADP_L0, "state check error.[%d]\n", appPVR_Info->mfe_state);
        return FALSE;
    }
    mfe_ioctl((MS_VIRT)appPVR_Info->mfe_drvHandle, MFE_IOC_SET_VUI, (MS_VIRT)(vui_info));

    return TRUE;
}
