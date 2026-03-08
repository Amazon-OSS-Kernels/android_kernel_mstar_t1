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


#ifndef SCSI_H
#define SCSI_H

#include <MsTypes.h>
#include "include/drvTypes.h"
#include <drvUsbcommon.h> // public include

#define MAX_COMMAND_SIZE	16
#define SCSI_SENSE_BUFFERSIZE    64

#define MAX_USTOR  8 // maximux mass storage device per root hub port
#define TEST_UNIT_READY         0x00
#define REQUEST_SENSE           0x03
#define INQUIRY                 0x12
#define MODE_SENSE              0x5a
#define MODE_SENSE6             0x1a
#define READ_CAPACITY           0x25
#define READ_10                 0x28
#define WRITE_10                0x2a

#define TYPE_ROM                0x05
#define TYPE_SES                0x0d

#define SUCCESS         0x2002
#define FAILED          0x2003

#define IDENTIFY_BASE       0x80
#define IDENTIFY(can_disconnect, lun)   (IDENTIFY_BASE |\
                ((can_disconnect) ?  0x40 : 0) |\
                ((lun) & 0x07))

#define DID_NO_CONNECT  0x01
#define DID_HCD_SHUTDOWN    0x02
#define DID_BAD_TARGET  0x04
#define DID_ABORT       0x05
#define DID_ERROR       0x07

#define SCSI_DATA_UNKNOWN       0
#define SCSI_DATA_WRITE         1
#define SCSI_DATA_READ          2
#define SCSI_DATA_NONE          3

#define MS_STAT_GOOD            0x00
#define MS_STAT_CHECK_CONDITION 0x02

#define CB_LENGTH_TEST_UNIT_READY   0x0C
#define CB_LENGTH_REQUEST_SENSE     0x0C
#define CB_LENGTH_INQUIRY           0x0C
#define CB_LENGTH_MODE_SENSE        0x0C
#define CB_LENGTH_READ_CAPACITY     0x0C
#define CB_LENGTH_READ_10           0x0C
#define CB_LENGTH_WRITE_10          0x0C

#define DATA_LENGTH_INQUIRY         36
#define DATA_LENGTH_MODE_SENSE      192
#define DATA_LENGTH_REQUEST_SENSE   18
#define DATA_LENGTH_READ_CAPACITY   8

typedef struct ScsiDevice
{
	// Get by INQUIRY command
	unsigned char u8DeviceType;
	unsigned char u8VendorID[8];
	unsigned char u8ProductID[16];
	unsigned char u8ProductVer[4];
} Scsi_Device;

struct scMsc_cmd
{
    U32 serial_number;
    unsigned char lun;
    unsigned char cmd_len;
    unsigned char sc_data_direction;
    unsigned char cmnd[MAX_COMMAND_SIZE];
    U32 req_buf_len;
    void *req_buf;
    int resid;
    unsigned char sense_buffer[SCSI_SENSE_BUFFERSIZE];
    U32 flags;
    void (*scsi_done) (struct scMsc_cmd *);
    int result;
    unsigned char tag;
};

typedef struct scMsc_cmd stCmd_scsi;

extern BOOL ms_bSCSI_INQUIRY(U8 uPort, unsigned char uLunNum, unsigned char *pIngBuf);
extern BOOL ms_bSCSI_READ_CAPACITY(U8 uPort, unsigned char uLunNum, U32 *pTotalBlks, U32 *pBlkSize);
extern BOOL ms_bSCSI_TEST_UNIT_READY(U8 uPort, unsigned char uLunNum, BOOL CheckCBI);
extern BOOL ms_bInit_USB_Disk(U8 uPort);
extern BOOL ms_bSCSI_Read_10(U8 uPort, unsigned char uLunNum, U32 u32BlockAddr, U32 u32BlockNum, unsigned char *u8Buffer);
extern BOOL ms_bSCSI_Write_10(U8 uPort, unsigned char uLunNum, U32 u32BlockAddr, U32 u32BlockNum, unsigned char *u8Buffer);
extern BOOL ms_bSCSI_Read_10_512(U8 uPort, unsigned char uLunNum, U32 u32BlockAddr, U32 u32BlockNum, unsigned char *u8Buffer);
extern BOOL ms_bSCSI_Write_10_512(U8 uPort, unsigned char uLunNum, U32 u32BlockAddr, U32 u32BlockNum, unsigned char *u8Buffer);
extern BOOL ms_bIsDevValid(U8 uPort, unsigned char LunNum);
extern unsigned char ms_u8GetDevType(U8 uPort, unsigned char LunNum);
extern void ms_vRemove_DISK_LUNs(U8 uPort);
#endif

