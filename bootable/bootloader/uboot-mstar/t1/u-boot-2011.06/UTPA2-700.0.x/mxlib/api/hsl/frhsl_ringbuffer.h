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
/**************************************************************************
 **************************************************************************/
/** \file
 * \brief This is the header file for the FrHsl ring buffer module.
 *
 */

#ifndef FRHSL_RINGBUFFER_H
#define FRHSL_RINGBUFFER_H

#include <pthread.h>

#include "apiHSL.h"
#if HSL_TCPIP == true
#include "frhsl_autotest.h"
#include "frhsl_cfg.h"

#endif

#ifdef __cpluplus
extern "C"
{
#endif

#if defined(MSOS_TYPE_LINUX)

#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>

enum HSLOutputType
{
    HSL_UART = 'U',
    HSL_FILE = 'F',
    HSL_SOCKET = 'S',
};

#define FR_HSL_FILE_DESCRIPTOR             (HSL_UART)

#define HSL_DEVICE_PATH "/dev/hsl"
#define CANNOT_OPEN_MSG "Cannot open /dev/hsl\n"

#define HSL_IOC_MAGIC               'h'

#define MDRV_HSL_SET_OUTPUT_PATH                _IOW(HSL_IOC_MAGIC, 1, MS_U8)
#define MDRV_HSL_CLEAR_OUTPUT_FILE                _IO(HSL_IOC_MAGIC, 2)

#if HSL_TCPIP
#define PUTCS(dataptr, length, port, hsl) CmdSend(hsl, dataptr, length)
#define FFLUSH(port)
#else
#define PUTCS(dataptr, length, port, hsl) \
      {\
    int fd; \
    fd = open(HSL_DEVICE_PATH, O_RDWR); \
    if (fd <= 0) { \
      write(STDERR_FILENO, CANNOT_OPEN_MSG, strlen(CANNOT_OPEN_MSG)); \
    } \
    else \
    { \
      if (0==ioctl(fd, MDRV_HSL_SET_OUTPUT_PATH, (port)) ) \
      { \
        write(fd, (dataptr), (length)); \
      } \
      close(fd); \
    } \
      }

#define FFLUSH(port) \
      { \
    int fd; \
    fd = open(HSL_DEVICE_PATH, O_RDWR); \
    if (fd <= 0) { \
      write(STDERR_FILENO, CANNOT_OPEN_MSG, strlen(CANNOT_OPEN_MSG)); \
    } \
    else \
    { \
      if (0==ioctl(fd, MDRV_HSL_SET_OUTPUT_PATH, (port)) ) \
      { \
        fsync(fd); \
      } \
      close(fd); \
    } \
      }
#endif

#define HSL_CLEAR_OUTPUT_FILE() \
      { \
    int fd; \
    fd = open(HSL_DEVICE_PATH, O_RDWR); \
    if (fd <= 0) { \
      write(STDERR_FILENO, CANNOT_OPEN_MSG, strlen(CANNOT_OPEN_MSG)); \
    } \
    else \
    { \
      ioctl(fd, MDRV_HSL_CLEAR_OUTPUT_FILE); \
      close(fd); \
    } \
      }

#else

#define FR_HSL_FILE_DESCRIPTOR             (STDOUT_FILENO)

#define PUTCS(dataptr, length, port) \
      { \
    MS_U32 i; \
    for( i=0 ; i<length ; i++) \
    { \
      write(port, dataptr, length); \
    } \
      }

#define FFLUSH(port)        fsync(port)

#endif

#define BUFFER_FLAG_NONE            0x0000
#define BUFFER_FLAG_DUMP            0x0001
#define BUFFER_FLAG_EMERGENCY_DUMP  0x0002
#define BUFFER_FLAG_TX              0x0004
#define BUFFER_FLAG_FLUSH            0x0008

struct stListNode;

MS_BOOL bufferInitialize(stHslController* const _hsl, const MS_PHYADDR physAddr, const MS_U32 size);
MS_BOOL bufferFinalize(stHslController* const _hsl);
MS_BOOL bufferAllocateWrite(stHslController* const _hsl, const MS_U32 dataSize, stListNode** const s_dataInfo);
MS_BOOL bufferDumpOldestDataSegment(stHslController* const _hsl);
void bufferAcknowledgeWrite(stHslController* const _hsl, stListNode* const s_dataInfo);
MS_BOOL bufferProcessCommand(stHslController* const _hsl, CmdInfo* const pCmd);

// Function prototypes
void *bufferWriteThread(void*);        // output thread function
MS_BOOL initThread( hslThread* aThread);

#ifdef __cpluplus
}
#endif

#endif // FRHSL_RINGBUFFER_H
