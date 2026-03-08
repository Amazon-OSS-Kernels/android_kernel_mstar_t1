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
#include "mfe_type.h"


#define mfe_ioctl MDrv_MFE_Ioctl

MS_S32 MDrv_MFE_Ioctl(MS_VIRT MFE_fd, MS_U32 cmd, MS_VIRT arg);


#define MFE_IOC_MAXNR 21UL
#define MFE_IOC_MAGIC 0UL

#define MFE_IOC_INIT                         ((MFE_IOC_MAGIC<<8)|0UL)
#define MFE_IOC_SET_QUALITY                  ((MFE_IOC_MAGIC<<8)|1UL)
#define MFE_IOC_GET_DISPORDER                ((MFE_IOC_MAGIC<<8)|2UL)
#define MFE_IOC_GETBITS                      ((MFE_IOC_MAGIC<<8)|3UL)
#define MFE_IOC_ENC_ONEFRAME                 ((MFE_IOC_MAGIC<<8)|4UL)
#define MFE_IOC_FINISH                       ((MFE_IOC_MAGIC<<8)|5UL)
#define MFE_IOC_CLEAR_BITSBUF                ((MFE_IOC_MAGIC<<8)|6UL)
#define MFE_IOC_RESET_BITRATE                ((MFE_IOC_MAGIC<<8)|7UL)
#define MFE_IOC_RESET_FRAMERATE              ((MFE_IOC_MAGIC<<8)|8UL)
#define MFE_IOC_RESET_BITRATE_AND_FRAMERATE  ((MFE_IOC_MAGIC<<8)|9UL)
#define MFE_IOC_GET_VOL                      ((MFE_IOC_MAGIC<<8)|11UL)
#define MFE_IOC_POWEROFF                     ((MFE_IOC_MAGIC<<8)|14UL)
#define MFE_IOC_SETBITRATE                   ((MFE_IOC_MAGIC<<8)|15UL)
#define MFE_IOC_SET_FORCEDI                  ((MFE_IOC_MAGIC<<8)|17UL)
#define MFE_IOC_SET_MIU_OFFSET               ((MFE_IOC_MAGIC<<8)|18UL)
#define MFE_IOC_SET_RIU_BASE                 ((MFE_IOC_MAGIC<<8)|19UL)
#define MFE_IOC_GET_QTABLE                   ((MFE_IOC_MAGIC<<8)|20UL)
#define MFE_IOC_GET_DROPFLAG                 ((MFE_IOC_MAGIC<<8)|21UL)
#define MFE_IOC_GET_SKIPFLAG                 ((MFE_IOC_MAGIC<<8)|22UL)
#define MFE_IOC_GET_SKIPSHORTHEADER          ((MFE_IOC_MAGIC<<8)|23UL)
#define MFE_IOC_RESET_SPSPPS                 ((MFE_IOC_MAGIC<<8)|24UL)
#define MFE_IOC_SET_COLOR_FORMAT             ((MFE_IOC_MAGIC<<8)|25UL)
#define MFE_IOC_SET_ISR                      ((MFE_IOC_MAGIC<<8)|26UL)
#define MFE_IOC_GET_HWCAP                    ((MFE_IOC_MAGIC<<8)|27UL)
#define MFE_IOC_GET_FRAME_TYPE               ((MFE_IOC_MAGIC<<8)|28UL)
#define MFE_IOC_SET_VUI                      ((MFE_IOC_MAGIC<<8)|29UL)
#endif    // _MDRV_MFE_IO_H_

