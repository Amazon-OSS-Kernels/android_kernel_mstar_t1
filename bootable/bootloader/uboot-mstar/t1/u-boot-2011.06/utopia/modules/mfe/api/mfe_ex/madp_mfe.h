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



#ifdef __cplusplus
extern "C"
{
#endif
#include "MFE_chip.h"
#include "MsTypes.h"
#include "drv_mfe_st.h"
#include "apiVOIPMFE.h"
//input method: _SCALER_IN_, _FILE_IN_, _YUVGEN_, and _MUXTS_

// #define _FILE_IN_
//#define _AUDIO_FILE_IN_
//#define _YUVGEN_

//#define _MUXTS_
//#define _MUX_WITH_AUDIO_


//#define AUDIO_MUX_DUMP

//#define _EN_ADP_CRC_ //for testing
//#define CRC_Accumulate	//acc CRC value for overnight testing
//#define DEBUG_GET
//#define TIME_MEASURE

#define MFE_MIU 0UL //kernel mode

#if defined(_YUVGEN_)||defined(_FILE_IN_)
	#define TARGET_FRAME 10UL
#endif

#define FRAM_BUF_SIZE 512UL*1024UL //output bsp buffer

typedef struct _MFE_INPUT_BUF_INFO {
    MS_U32 mfe_miu_offset;
    MS_U32 mfe_buf_pa_withoffset;
    MS_U32 mfe_buf_pa_withoutoffset;
    MS_U32 mfe_buf_size;
    MS_U32 mfe_buf_va;
    MS_U32 mfe_buf_va_end;
} MFE_INPUT_BUF_INFO;


#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)&&defined(_MFE_UTOPIA_)
		#include "MsTypes.h"
		#define E_SYS_MMAP_MFE 0x02010000UL //0
		#define MAdp_MPool_VA2PA(a) MS_VA2PA((MS_VIRT)(a))
		#define MAdp_MPool_PA2KSEG0(a) MS_PA2KSEG0((MS_PHY)(a)) // for cached memory
		#define MAdp_MPool_PA2KSEG1(a) MS_PA2KSEG1((MS_PHY)(a)) // for non-cached memory
		#define open_dev(a, b) 1UL
             #define MIU_OFFSET 0x18000000UL
#elif defined(__MOBILE_CASE__)
		#include "msutil/MsTypes.h"
		#define MAdp_MPool_VA2PA(a) MS_VA2PA((MS_VIRT)(a))
		#define MAdp_MPool_PA2KSEG1(a) MS_PA2KSEG1((MS_PHY)(a))
		#define open_dev(a, b) 1UL
#else
    #define open_dev(a, b) 1UL
	#define E_SYS_MMAP_MFE 0UL
       #define MIU_OFFSET 0x0000000000UL
	#ifdef WIN32
        #include "MsTypes.h"
        #include "pmem_mm.h"
		#include <windows.h>
		#define usleep(time) Sleep(time)
		#define MS_VA2PA(a) pmem_mm_va2pa((MS_VIRT)(a))
		#define MS_PA2KSEG1(a) pmem_mm_pa2va((MS_PHY)(a))
		#define MAdp_MPool_VA2PA(a) pmem_mm_va2pa((MS_VIRT)(a))
		#define MAdp_MPool_PA2KSEG1(a) pmem_mm_pa2va((MS_PHY)(a))
    #else
		#ifndef BOOLEAN
			#define BOOLEAN bool
		#endif
		#define MAdp_MPool_VA2PA(a) (((MS_VIRT)(a))-MEM_LOGMAPPHY)
		#define MAdp_MPool_PA2KSEG1(a) (((MS_PHY)(a))+MEM_LOGMAPPHY)
	#endif
#endif //defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)


#ifndef MUX
#define MUX(a, b, c) ((a) ? (b) : (c))
#endif


MS_BOOL MAdp_MFE_Encode(void/*U8 frc*/);
MS_S32 MAdp_MFE_Finish(void);


#if defined(_MFE_T8_)&& (!defined(_KERNEL_MODE_))
#define MAdp_MPool_Init()
MS_S32 MAdp_MPool_GetBlockVa(MS_U32 opt, MS_U32 *buf_start, MS_U32 *buf_size);
#endif

//set memory addr and size, miu0 or miu1.
#if defined(_MFE_T8_)&& (!defined(_KERNEL_MODE_))
MS_S32 MAdp_MFE_SetBuffInfo(MS_S32 miu_type,MS_U32 mfe_pa_without_offset_addr,MS_U32 miu_offset,MS_U32 mem_size);
#endif


//this function can write data to DRAM. We can use trace32:"data.save" to get data from FPGA to PC.
                                              //data pointer, offset of 0x82C00000(physical DRAM addr,size(bytes) )
void MADP_MFE_WRITE(MS_U8* data,MS_U32 offset,MS_U32 size);


#ifdef __cplusplus
}
#endif

#endif

