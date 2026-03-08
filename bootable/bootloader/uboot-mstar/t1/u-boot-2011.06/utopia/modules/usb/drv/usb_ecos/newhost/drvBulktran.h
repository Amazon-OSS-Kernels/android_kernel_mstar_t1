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


#ifndef _TRANSPORT_H_
#define _TRANSPORT_H_

#include <MsTypes.h>
#include  "include/drvConfig.h"
#include "drvMassStor.h"
#include "drvMSC.h"
#include <drvUsbcommon.h> // public include

/*=========Sub Classes=========*/
#define MS_RBC_SC       0x01    /* Typically, flash devices */
#define MS_ATAPI_SC     0x02    /* CD-ROM */
#define MS_QIC_SC       0x03    /* QIC-157 Tapes */
#define MS_UFI_SC       0x04    /* Floppy */
#define MS_SFF_SC       0x05    /* Removable media */
#define MS_SCSI_SC      0x06    /* Transparent */
#define MS_LSD_SC       0x07    /* LSD FS */
#define MS_DEVICE_SC    0xff    /* Use device's value */
/*=============================*/
/*==========Protocols==========*/
#define MS_CBI_PR       0x00    /* CBI */
#define MS_CB_PR        0x01    /* CB */
#define MS_BULK_PR      0x50    /* BOT */
#define MS_DEVICE_PR    0xff
/*=============================*/

struct stBulk_cb 
{
    U32 Inscription;
    U32 Label;
    U32 DataLength;
    unsigned char   Flag;
    unsigned char   Lun;
    unsigned char   Len;
    unsigned char   CDB[16];
};

struct stBulk_cs 
{
    U32 Inscription;
    U32 Label;
    U32 Remains;
    unsigned char   Class;
};

#define BULK_CB_WRAP_LEN    31
#define BULK_CB_SIGN        0x43425355
#define BULK_CS_WRAP_LEN    13
#define BULK_CS_SIGN        0x53425355

#define BULK_CS_OLYMPUS_SIGN    0x55425355
#define BULK_STAT_OK            0
#define BULK_STAT_FAIL          1
#define BULK_STAT_PHASE         2

#define BULK_RESET_REQUEST  0xff
#define BULK_GET_MAX_LUN    0xfe

#define XFER_GOOD   0
#define XFER_SHORT  1
#define XFER_STALLED    2
#define XFER_LONG   3
#define XFER_ERROR  4
#define XFER_NO_DEV    5
#define XFER_HCD_SHUTDOWN    6

#define TRANSPORT_GOOD      0
#define TRANSPORT_FAILED    1   
#define TRANSPORT_NO_SENSE  2  
#define TRANSPORT_ERROR     3   
#define TRANSPORT_NO_DEVICE  4
#define TRANSPORT_HCD_SHUTDOWN     5 

#define MS_CBI_ADSC     0

#if SupportOtherDevice
extern void ms_usb_qic157_command(stCmd_scsi*, struct ms_usdata*);
extern void ms_usb_ufi_command(stCmd_scsi*, struct ms_usdata*);
#endif
extern void ms_usb_transparent_scsi_command(stCmd_scsi*, struct ms_usdata*);
extern int ms_usb_CB_transport(stCmd_scsi*, struct ms_usdata*);
extern int ms_usb_CB_reset(struct ms_usdata*);
extern int ms_usb_bulk_transport(stCmd_scsi*, struct ms_usdata*);
extern int ms_usb_bulk_reset(struct ms_usdata*);
extern void ms_usb_invoke_transport(stCmd_scsi*, struct ms_usdata*);
extern int ms_usb_control_msg(struct ms_usdata *ptr, U32 pipe,
        unsigned char req, unsigned char rt, U16 val, U16 idx, void *data, U16 len, int time_out);
extern int ms_usb_msc_clear_halt(struct ms_usdata *ptr, U32 pipe);
extern int ms_usb_bulk_transfer_buf(struct ms_usdata *ptr, U32 pipe, void *buf, U32 len, U32 *act_len);
#endif
