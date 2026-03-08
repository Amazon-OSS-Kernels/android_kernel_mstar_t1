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


#ifndef _TRANSPORT_H_
#define _TRANSPORT_H_

#include <MsTypes.h>
#include  "include/drvConfig.h"
//#include <kernel/blkdev.h>
#include "drvMassStor.h"
#include "drvSCSI.h"
/* Protocols */

#define US_PR_CBI    0x00		/* CBI */
#define US_PR_CB	0x01		/* CB */
#define US_PR_BULK	0x50		/* BOT */
#ifdef CONFIG_USB_STORAGE_HP8200e
#define US_PR_SCM_ATAPI	0x80
#endif
#ifdef CONFIG_USB_STORAGE_SDDR09
#define US_PR_EUSB_SDDR09	0x81
#endif
#ifdef CONFIG_USB_STORAGE_SDDR55
#define US_PR_SDDR55	0x82
#endif
#define US_PR_DPCM_USB  0xf0

#ifdef CONFIG_USB_STORAGE_FREECOM
#define US_PR_FREECOM   0xf1
#endif

#ifdef CONFIG_USB_STORAGE_DATAFAB
#define US_PR_DATAFAB   0xf2
#endif

#ifdef CONFIG_USB_STORAGE_JUMPSHOT
#define US_PR_JUMPSHOT  0xf3
#endif

#define US_PR_DEVICE	0xff

struct bulk_cb_wrap {
	U32	Signature;
	U32	Tag;
	U32	DataTransferLength;
	unsigned char	Flags;
	unsigned char	Lun;
	unsigned char	Length;
	unsigned char	CDB[16];
};

#define US_BULK_CB_WRAP_LEN	31
#define US_BULK_CB_SIGN		0x43425355
#define US_BULK_FLAG_IN		1
#define US_BULK_FLAG_OUT	0

struct bulk_cs_wrap {
	U32	Signature;
	U32	Tag;
	U32	Residue;
	unsigned char	Status;
	unsigned char	Filler[18];
};

#define US_BULK_CS_WRAP_LEN	13
#define US_BULK_CS_SIGN		0x53425355

#define US_BULK_CS_OLYMPUS_SIGN		0x55425355
#define US_BULK_STAT_OK		0
#define US_BULK_STAT_FAIL	1
#define US_BULK_STAT_PHASE	2

#define US_BULK_RESET_REQUEST	0xff
#define US_BULK_GET_MAX_LUN	0xfe


#define USB_STOR_XFER_GOOD	0
#define USB_STOR_XFER_SHORT	1
#define USB_STOR_XFER_STALLED	2
#define USB_STOR_XFER_LONG	3
#define USB_STOR_XFER_ERROR	4
#define USB_STOR_XFER_NO_DEV    5

#define USB_STOR_TRANSPORT_GOOD	   0
#define USB_STOR_TRANSPORT_FAILED  1
#define USB_STOR_TRANSPORT_NO_SENSE 2
#define USB_STOR_TRANSPORT_ERROR   3
#define USB_STOR_TRANSPORT_NO_DEVICE  4

#define US_CBI_ADSC		0

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
extern int usb_stor_CBI_transport(Scsi_Cmnd*, struct us_data*);

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
extern int usb_stor_CB_transport(Scsi_Cmnd*, struct us_data*);
extern int usb_stor_CB_reset(struct us_data*);

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
extern int usb_stor_Bulk_transport(Scsi_Cmnd*, struct us_data*);
extern int usb_stor_Bulk_max_lun(struct us_data*);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
extern int usb_stor_Bulk_reset(struct us_data*);

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
extern void usb_stor_invoke_transport(Scsi_Cmnd*, struct us_data*);
extern void usb_stor_stop_transport(struct us_data*);

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
extern int usb_stor_control_msg(struct us_data *us, U32 pipe,
		unsigned char request, unsigned char requesttype, U16 value, U16 index,
		void *data, U16 size, int timeout);
extern int usb_stor_clear_halt(struct us_data *us, U32 pipe);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

extern int usb_stor_ctrl_transfer(struct us_data *us, U32 pipe,
		unsigned char request, unsigned char requesttype, U16 value, U16 index,
		void *data, U16 size);
extern int usb_stor_intr_transfer(struct us_data *us, void *buf,
		U32 length);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
extern int usb_stor_bulk_transfer_buf(struct us_data *us, U32 pipe,
		void *buf, U32 length, U32 *act_len);




#endif
