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
#ifndef _MDRV_MFE_IO_H_
#define _MDRV_MFE_IO_H_

#include "MFE_chip.h"
#if defined(_MFE_T8_)&&!defined(_KERNEL_MODE_)
#include "mfe_type.h"
#endif

#if defined(_MFE_T8_)&&defined(_KERNEL_MODE_)
	#include <linux/fs.h>
	#include "mdrv_types.h"
#elif defined(MIPSDEV) //defined(_MFE_BIG2_)&&defined(_MIPS_PLATFORM_)
	#include <sys/bsdtypes.h>
#endif

#ifndef WIN32
       #if defined(_MFE_T8_)&&defined(_KERNEL_MODE_)

       #else
       #include <poll.h>       // poll
       #include <sys/ioctl.h>
       #endif
#endif

#if defined(_MFE_T8_)&&defined(_KERNEL_MODE_)
//	#define open_dev open
typedef struct file mfe_struct_file;
typedef struct inode mfe_struct_inode;
	int MDrv_MFE_Ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);
#else
//	#define open_dev(a, b) 1
	#define poll(a, b, c) 1
	#define ioctl MDrv_MFE_Ioctl

    void MDrv_MFE_SetOSRegBase( MFE_U32 u32RegBaseAddr );

#ifdef _WIN32
    int MDrv_MFE_Ioctl(MFE_S32 MFE_fd, unsigned int cmd, unsigned long  arg);
#else
    int MDrv_MFE_Ioctl(MFE_S32 MFE_fd, unsigned int cmd, unsigned long  arg);
#endif
#endif

#define MFE_IOC_MAXNR           21
#if defined(_MFE_BIG2_) || ( defined(_MFE_T8_)&&(!defined(_KERNEL_MODE_)) )

#define MFE_IOC_MAGIC 0

	#define MFE_IOC_INIT            ((MFE_IOC_MAGIC<<8)|0)
	#define MFE_IOC_GET_DISPORDER	((MFE_IOC_MAGIC<<8)|2)
	#define MFE_IOC_GETBITS         ((MFE_IOC_MAGIC<<8)|3)
	#define MFE_IOC_ENC_ONEFRAME    ((MFE_IOC_MAGIC<<8)|4)
	#define MFE_IOC_FINISH          ((MFE_IOC_MAGIC<<8)|5)
	#define MFE_IOC_CLEAR_BITSBUF   ((MFE_IOC_MAGIC<<8)|6)
       #define MFE_IOC_RESET_BITRATE ((MFE_IOC_MAGIC<<8)|7)
       #define MFE_IOC_RESET_FRAMERATE ((MFE_IOC_MAGIC<<8)|8)
	#define MFE_IOC_RESET_BITRATE_AND_FRAMERATE    ((MFE_IOC_MAGIC<<8)|9)
	#define MFE_IOC_GET_VOL        ((MFE_IOC_MAGIC<<8)|11)
	#define MFE_IOC_POWEROFF       ((MFE_IOC_MAGIC<<8)|14)
       #define MFE_IOC_SETBITRATE ((MFE_IOC_MAGIC<<8)|15)
       #define MFE_IOC_SET_FORCEDI    ((MFE_IOC_MAGIC<<8)|17)
       #define MFE_IOC_SET_MIU_OFFSET ((MFE_IOC_MAGIC<<8)|18)
       #define MFE_IOC_SET_RIU_BASE ((MFE_IOC_MAGIC<<8)|19)
       #define MFE_IOC_CHECK_STATUS ((MFE_IOC_MAGIC<<8)|20)
#else

#define MFE_IOC_MAGIC           'E'

	#define MFE_IOC_INIT            _IO(MFE_IOC_MAGIC,  0)
	#define MFE_IOC_GET_DISPORDER	_IOR(MFE_IOC_MAGIC, 2, U32)
	#define MFE_IOC_GETBITS         _IOR(MFE_IOC_MAGIC, 3, U32)
	#define MFE_IOC_ENC_ONEFRAME    _IOR(MFE_IOC_MAGIC, 4, U32)
	#define MFE_IOC_FINISH          _IOR(MFE_IOC_MAGIC, 5, U32)
	#define MFE_IOC_CLEAR_BITSBUF   _IOR(MFE_IOC_MAGIC, 6, U32)
       #define MFE_IOC_RESET_BITRATE _IOR(MFE_IOC_MAGIC, 7, U32)
       #define MFE_IOC_RESET_FRAMERATE _IOR(MFE_IOC_MAGIC, 8, U32)
       #define MFE_IOC_RESET_BITRATE_AND_FRAMERATE    _IOR(MFE_IOC_MAGIC,  9)
	#define MFE_IOC_GET_VOL        _IOR(MFE_IOC_MAGIC, 11, U32)
	#define MFE_IOC_POWEROFF       _IOR(MFE_IOC_MAGIC, 14, U32)
       #define MFE_IOC_SETBITRATE _IOR(MFE_IOC_MAGIC, 15, U32)
       #define MFE_IOC_SET_FORCEDI    _IO(MFE_IOC_MAGIC, 17)
       #define MFE_IOC_SET_MIU_OFFSET _IOR(MFE_IOC_MAGIC, 18, U32)
       #define MFE_IOC_SET_RIU_BASE _IOR(MFE_IOC_MAGIC, 19, U32)
       #define MFE_IOC_CHECK_STATUS _IOR(MFE_IOC_MAGIC, 20, U32)
#endif


typedef struct{
    unsigned char* data;
    unsigned int offset;
    unsigned int size;
    MFE_U32 buf_VA;
    MFE_U32 buf_size;
} WRITE_INFO;

    //for debug, Write data to DRAM, then you can use TV tool to dump data from FPGA to PC.
    #define OUT_DRAM 0x82C00000
    //#define OUT_DRAM 0x0001680000
    #define _APVR_WRITE_DRAM(bias)		(*(volatile MFE_U8 *) ( bias))

#endif	// _MDRV_MFE_IO_H_
