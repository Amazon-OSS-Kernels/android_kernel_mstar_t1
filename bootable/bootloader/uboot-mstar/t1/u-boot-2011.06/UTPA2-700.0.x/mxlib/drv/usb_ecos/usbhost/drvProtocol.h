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


#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <MsTypes.h>
#include "drvSCSI.h"
#include "drvMassStor.h"

/* Sub Classes */

#define US_SC_RBC	0x01		/* Typically, flash devices */
#define US_SC_8020	0x02		/* CD-ROM */
#define US_SC_QIC	0x03		/* QIC-157 Tapes */
#define US_SC_UFI	0x04		/* Floppy */
#define US_SC_8070	0x05		/* Removable media */
#define US_SC_SCSI	0x06		/* Transparent */
#define US_SC_ISD200    0x07		/* ISD200 ATA */
#define US_SC_MIN	US_SC_RBC
#define US_SC_MAX	US_SC_ISD200

#define US_SC_DEVICE	0xff		/* Use device's value */

#if SupportOtherDevice
extern void usb_stor_ATAPI_command(Scsi_Cmnd*, struct us_data*);
extern void usb_stor_qic157_command(Scsi_Cmnd*, struct us_data*);
extern void usb_stor_ufi_command(Scsi_Cmnd*, struct us_data*);
#endif

extern void usb_stor_transparent_scsi_command(Scsi_Cmnd*, struct us_data*);

#endif
