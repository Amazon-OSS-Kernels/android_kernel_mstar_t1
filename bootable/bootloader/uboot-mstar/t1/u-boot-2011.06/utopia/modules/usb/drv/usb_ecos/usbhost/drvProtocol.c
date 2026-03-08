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

#include <MsCommon.h>
#include "drvProtocol.h"
#include "drvMassStor.h"
//#include "debug.h"
#include "drvTransport.h"
#if 1

#ifdef US_DEBUG
#define US_DEBUGP(x,...) printk(x,__VA_ARGS__)
#else
#define US_DEBUGP(x,...)
#endif

static void *
find_data_location(Scsi_Cmnd *srb)
{
	if (srb->use_sg) 
	{
		printk("find_data_location : unsupport scatter function.");
		return (void *) srb->request_buffer;
	} 
	else
		return (void *) srb->request_buffer;
}

static void fix_inquiry_data(Scsi_Cmnd *srb)
{
	unsigned char *data_ptr;

	if (srb->cmnd[0] != INQUIRY)
		return;

	if (srb->request_bufflen < 3)
		return;

	data_ptr = (unsigned char*) find_data_location(srb);

	if ((data_ptr[2] & 7) == 2)
		return;

	US_DEBUGP("Fixing INQUIRY data to show SCSI rev 2 - was %d\n",
		  data_ptr[2] & 7);

	data_ptr[2] = (data_ptr[2] & ~7) | 2;
}

static void fix_read_capacity(Scsi_Cmnd *srb)
{
	unsigned char *dp;
	U32 capacity;

	if (srb->cmnd[0] != READ_CAPACITY)
		return;

	dp = (unsigned char*) find_data_location(srb);

	capacity = (dp[0]<<24) + (dp[1]<<16) + (dp[2]<<8) + (dp[3]);
	US_DEBUGP("US: Fixing capacity: from %ld to %ld\n",
	       capacity+1, capacity);
	capacity--;
	dp[0] = (capacity >> 24);
	dp[1] = (capacity >> 16);
	dp[2] = (capacity >> 8);
	dp[3] = (capacity);
}

#if SupportOtherDevice
void usb_stor_qic157_command(Scsi_Cmnd *srb, struct us_data *us)
{
	for (; srb->cmd_len<12; srb->cmd_len++)
		srb->cmnd[srb->cmd_len] = 0;

	srb->cmd_len = 12;

	usb_stor_invoke_transport(srb, us);
	if (srb->result == SAM_STAT_GOOD) {
		fix_inquiry_data(srb);
	}
}

void usb_stor_ATAPI_command(Scsi_Cmnd *srb, struct us_data *us)
{

	for (; srb->cmd_len<12; srb->cmd_len++)
		srb->cmnd[srb->cmd_len] = 0;

	srb->cmd_len = 12;

	usb_stor_invoke_transport(srb, us);

	if (srb->result == SAM_STAT_GOOD) {
		fix_inquiry_data(srb);
	}
}


void usb_stor_ufi_command(Scsi_Cmnd *srb, struct us_data *us)
{
	for (; srb->cmd_len<12; srb->cmd_len++)
		srb->cmnd[srb->cmd_len] = 0;

	srb->cmd_len = 12;

	switch (srb->cmnd[0]) {

	case INQUIRY:
		srb->cmnd[4] = 36;
		break;

	//case MODE_SENSE_10:
	case MODE_SENSE:	//GGYY
		srb->cmnd[7] = 0;
		srb->cmnd[8] = 8;
		break;

	case REQUEST_SENSE:
		srb->cmnd[4] = 18;
		break;
	} /* end switch on cmnd[0] */

	usb_stor_invoke_transport(srb, us);

	if (srb->result == SAM_STAT_GOOD) {
		fix_inquiry_data(srb);
	}
}
#endif

void usb_stor_transparent_scsi_command(Scsi_Cmnd *srb, struct us_data *us)
{
	usb_stor_invoke_transport(srb, us);

	if (srb->result == SAM_STAT_GOOD) {
		fix_inquiry_data(srb);

		if (us->flags & US_FL_FIX_CAPACITY)
			fix_read_capacity(srb);
	}
}
#endif	//#

