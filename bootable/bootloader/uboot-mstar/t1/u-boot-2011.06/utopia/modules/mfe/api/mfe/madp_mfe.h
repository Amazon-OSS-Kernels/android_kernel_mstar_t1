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
#ifndef _MADP_MFE_H_
#define _MADP_MFE_H_

#include "MFE_chip.h"
#if defined(_MFE_T8_)&&defined(_KERNEL_MODE_)
#include "mdrv_mfe_io.h" //include define: _MFE_T8_ _MIPS_PLATFORM_...etc
#endif
#include "mdrv_mfe_st.h"



#ifdef __cplusplus
extern "C"
{
#endif

//input method: _SCALER_IN_, _FILE_IN_, _YUVGEN_, and _MUXTS_

#define _SCALER_IN_

// #define _FILE_IN_
//#define _AUDIO_FILE_IN_
//#define _YUVGEN_

//#define _MUXTS_
//#define _MUX_WITH_AUDIO_

//#define NEW_APVR_INTERFACE

#define VVOIP_INTERFACE1
//#define AUDIO_MUX_DUMP

//#define _EN_ADP_CRC_ //for testing
//#define CRC_Accumulate	//acc CRC value for overnight testing
//#define DEBUG_GET
//#define TIME_MEASURE

#define MFE_MIU 0 //kernel mode

#if defined(_YUVGEN_)||defined(_FILE_IN_)
	#define TARGET_FRAME 10
#endif

#define FRAM_BUF_SIZE 512*1024 //output bsp buffer

typedef struct _MFE_INPUT_BUF_INFO {
    MFE_U32 mfe_miu_offset;
    MFE_U32 mfe_buf_pa_withoffset;
    MFE_U32 mfe_buf_pa_withoutoffset;
    MFE_U32 mfe_buf_size;
    MFE_U32 mfe_buf_va;
    MFE_U32 mfe_buf_va_end;
} MFE_INPUT_BUF_INFO;


#if defined(_YUVGEN_)||defined(_FILE_IN_)
// File IN
typedef struct _FILE_IN_INFO {
	FILE *infile;
	FILE *outfile;
	int nTotalFramesInFile;	// How many frames in the specified file.
	int nFrameSize;
} FILE_IN_INFO;
#endif

#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)
	#include "MuxCommon.h"
	#if defined(_KERNEL_MODE_)
//		#define E_SYS_MMAP_MFE E_SYS_MMAP_M4VE
		#include "linux/types.h"
		#define open_dev open
             //#define MIU_OFFSET 0x0000000000
            #if MFE_MIU==0
                #define MIU_OFFSET 0x0000000000
            #else
                #define MIU_OFFSET 0x0010000000
            #endif

	#else
		#include "MsTypes.h"
		#define E_SYS_MMAP_MFE 0x02010000//0
		#define MAdp_MPool_VA2PA(a) MS_VA2PA((MS_U32)(a))
		#define MAdp_MPool_PA2KSEG1(a) MS_PA2KSEG1((MS_U32)(a))
		#define open_dev(a, b) 1
             #define MIU_OFFSET 0x18000000
	#endif

#else
    #define open_dev(a, b) 1
	#include "memmap.h"
	#define E_SYS_MMAP_MFE 0
       #define MIU_OFFSET 0x0000000000
	#ifdef WIN32
		#include <windows.h>
		#define usleep(time) Sleep(time)
		#define MAdp_MPool_VA2PA(a) MAdp_MPool_VA2PA_Win32((U32)(a))
		#define MAdp_MPool_PA2KSEG1(a) MAdp_MPool_PA2VA_Win32((U32)(a))
	#else
		#ifndef BOOLEAN
			#define BOOLEAN bool
		#endif
		#define MAdp_MPool_VA2PA(a) (((U32)(a))-MEM_LOGMAPPHY)
		#define MAdp_MPool_PA2KSEG1(a) (((U32)(a))+MEM_LOGMAPPHY)
	#endif
#endif //defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)

#ifdef _MUXTS_
#include "TsMuxer.h"
#endif

#if defined(_MFE_BIG2_)&&defined(_MIPS_PLATFORM_)
//#define printf          diag_printf
#define usleep(time)    cyg_thread_delay(time)
#endif

#ifndef MUX
#define MUX(a, b, c) ((a) ? (b) : (c))
#endif


#if defined(VVOIP_INTERFACE1)
    #include "apiVOIPMFE.h"
#else
#define I_VOP        0        /* vop coding modes */
#define P_VOP        1
#define B_VOP        2

#if defined(NEW_APVR_INTERFACE)

typedef struct {
	U32 Currentindex;		// range = [0, DIP_DI_BUFFER_COUNT-1], MFE read index.
	int frame_num;	// number of frames in DI buffer.
} VENC_INFRAME_INFO;


typedef VENC_INFRAME_INFO* VENC_INFRAME_INFO_PTR;
typedef void (*GET_YUVFRAME_FUNC) (void* ppInfo);
typedef void (*CLEAR_BUF_STATUS_FUNC) (U16 nFrameCoded,void* ppInfo);
typedef void (*GET_AUDIO_DATA_FUNC) (U32 *AudAddr, U32 *AudSize, U32 *AudPTS, U8 *AudFrameCnt);

#endif


MFE_U32 MAdp_MFE_Init(float frame_rate,int test_case_format,int test_case_num);
MFE_BOOL MAdp_MFE_Encode(void/*U8 frc*/);
int MAdp_MFE_Finish(void);


#if defined(_MFE_T8_)&& (!defined(_KERNEL_MODE_))
#define MAdp_MPool_Init()
int MAdp_MPool_GetBlockVa(MFE_U32 opt, MFE_U32 *buf_start, MFE_U32 *buf_size);
#endif

//set memory addr and size, miu0 or miu1.
#if defined(_MFE_T8_)&& (!defined(_KERNEL_MODE_))
int MAdp_MFE_SetBuffInfo(int miu_type,unsigned int mfe_pa_without_offset_addr,unsigned int miu_offset,unsigned int mem_size);
#endif



#ifdef _MUXTS_
#ifdef DEBUG_READ
  void MPEG2TS_GetBuffer(U8** ppBuffer, U32* pSize, U32 readall);
  void MPEG2TS_GetBufferDone(U8* pBuffer);
#else
  void MPEG2TS_GetBuffer(U8** ppBuffer, U8 **ppBufReadPtr, U8 **ppBufWritePtr, IndexRingEntry **ppIndexPtr);
  void MPEG2TS_GetBufferDone(U8* pNewBufReadPtr, U32 IndexRead);
#endif
void MAdp_TSMUX_Init(MUX_TYPE MuxType, CodecID VideoType, CodecID AudioType,
					 S32 Width, S32 Height, float Framerate,
					 U8 *g_read_bbuf, IndexRingEntry *g_index_ring,
					 U32 read_bbuf_size, U32 index_ring_size);
void MPEG2TS_Set_DownloadPID(U32 VideoPID, U32 AudioPID);
// TSMUX will call function provide by APVR adaptation layer instead of calling audio driver directly
void MPEG2TS_Init_AudioData_Func(GET_AUDIO_DATA_FUNC getAudioDataCallbackFunc);

// Related DDI API: DDI_PVR_SetEncodingBitRate
// Bitrates to map into PVR_ENCODING_BITRATE_T: 6000000, 5000000, 400000, 3000000, 2000000
void MPEG2TS_SetVideoBitrate(U32 bitRate);

// start/stop MPEG2TS file encoding
// Related DDI API: DDI_PVR_StartDownload, DDI_PVR_StopDownload
void MPEG2TS_Video_Encode_Enable(BOOL enable);

// get current I frame buffer information, it will be used to generate thumbnail
// Related DDI API: DDI_PVR_GetThumbnail
void MPEG2TS_Get_I_Frame(/*YUV420_START***/U32 frameBuf, U32 bufSize, U32* pts);
BOOL MPEG2TS_isIFrame_Ready(U32* pts);
void MPEG2TS_Init_Buf_Info(U8 frameBufferCount, U16 width, U16 height);

#endif


#if defined(NEW_APVR_INTERFACE)
void MPEG2TS_Init_FrameBuf_Status_Func(GET_YUVFRAME_FUNC getBufStatusCallbackFunc,
            CLEAR_BUF_STATUS_FUNC clearBufStatusCallbackFunc);
#endif


//this function can write data to DRAM. We can use trace32:"data.save" to get data from FPGA to PC.
                                              //data pointer, offset of 0x82C00000(physical DRAM addr,size(bytes) )
void MADP_MFE_WRITE(unsigned char* data,unsigned int offset,unsigned int size);


#endif



#ifdef __cplusplus
}
#endif

#endif

