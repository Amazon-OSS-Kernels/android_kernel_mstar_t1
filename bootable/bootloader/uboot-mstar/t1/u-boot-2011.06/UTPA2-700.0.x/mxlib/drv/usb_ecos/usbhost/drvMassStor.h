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


/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#ifndef _USB_H_
#define _USB_H_

#include <MsTypes.h>
#include "include/drvUSB.h"
#include "include/drvCompletion.h"
#include "drvUsbHostConfig.h"
#include "drvSCSI.h"

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/

struct us_data;

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
struct us_unusual_dev
{
        /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	const char* vendorName;
	const char* productName;
	unsigned char  useProtocol;
	unsigned char  useTransport;
	int (*initFunction)(struct us_data *);
	U32 flags;

};

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define US_FL_SINGLE_LUN      0x00000001
#define US_FL_MODE_XLATE      0
#define US_FL_IGNORE_SER      0
#define US_FL_SCM_MULT_TARG   0x00000020
#define US_FL_FIX_INQUIRY     0x00000040
#define US_FL_FIX_CAPACITY    0x00000080
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define US_FLIDX_URB_ACTIVE	18
#define US_FLIDX_SG_ACTIVE	19
#define US_FLIDX_ABORTING	20
#define US_FLIDX_DISCONNECTING	21
#define DONT_SUBMIT	((1UL << US_FLIDX_ABORTING) | \
			 (1UL << US_FLIDX_DISCONNECTING))
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define US_STATE_IDLE		1
#define US_STATE_RUNNING	2
#define US_STATE_RESETTING	3
#define US_STATE_ABORTING	4
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
#define MAX_USB_STOR_LUNS       8   //USB_2
//#define USB_STOR_STRING_LEN 32
#define US_IOBUF_SIZE		64
typedef int (*trans_cmnd)(Scsi_Cmnd*, struct us_data*);
typedef int (*trans_reset)(struct us_data*);
typedef void (*proto_cmnd)(Scsi_Cmnd*, struct us_data*);
typedef void (*extra_data_destructor)(void *);
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
struct us_data {
	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
//	struct semaphore	dev_semaphore;
	struct usb_device	*pusb_dev;
	struct usb_interface	*pusb_intf;
	struct us_unusual_dev   *unusual_dev;
	U32		flags;
	U32		send_bulk_pipe;
	U32		recv_bulk_pipe;
	U32		send_ctrl_pipe;
	U32		recv_ctrl_pipe;
	U32		recv_intr_pipe;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	char			vendor[USB_STOR_STRING_LEN];
	char			product[USB_STOR_STRING_LEN];
	char			serial[USB_STOR_STRING_LEN];
	char			*transport_name;
	char			*protocol_name;
	unsigned char			subclass;
	unsigned char			protocol;
	unsigned char			max_lun;

	unsigned char			ifnum;
	unsigned char			ep_bInterval;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	trans_cmnd		transport;
	trans_reset		transport_reset;
	proto_cmnd		proto_handler;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
//	struct Scsi_Host	*host;
	Scsi_Cmnd		srb;
	Scsi_Device    	device;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
//	int			pid;
	int			sm_state;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	struct urb		*current_urb;
	struct urb		*data_urb;
	struct urb		*status_urb;
	struct usb_ctrlrequest	*cr;
//	struct usb_sg_request	current_sg;
	unsigned char		*iobuf;
	dma_addr_t		cr_dma;
	dma_addr_t		iobuf_dma;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
//	struct semaphore	sema;
	struct completion	notify;

	/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
	void			*extra;
	extra_data_destructor	extra_destructor;

	// For one Device but have many LUN
	BOOL bDevInit;
	struct LUN_Device* Mass_stor_device;

    unsigned char   ustor_id;
    unsigned char   host_id;
};

struct LUN_Device
{
	unsigned char u8LunNum;

	unsigned char u8DevType;

	// Get by READ CAPACITY command
	U32 u32BlockSize;
	U32 u32BlockSize_log2;
	U32 u32BlockTotalNum;

	// Get by TEST UNIT READY command
	BOOL bDeviceReady;

	// Save Write Protection information
	BOOL bWriteProtect;

	// Is File system mounted
	BOOL bFSInit;
};


/* The structure which defines our driver */
extern struct usb_driver usb_storage_driver;

/* Function to fill an inquiry response. See usb.c for details */
// YPING : this function fill USB information into scsi inquiry data.
extern void fill_inquiry_response(struct us_data *us,
	unsigned char *data, U32 data_len);

/* The scsi_lock() and scsi_unlock() macros protect the sm_state and the
 * single queue element srb for write access */
#define scsi_unlock(host)	spin_unlock_irq(host->host_lock)
#define scsi_lock(host)		spin_lock_irq(host->host_lock)
#define sg_address(psg)		(page_address((psg).page) + (psg).offset)

///////////// YPING add define //////////////
#define SupportOtherDevice	1

/**
 * test_and_clear_bit - Clear a bit and return its old value
 * @nr: Bit to set
 * @addr: Address to count from
 *
 * This operation is atomic and cannot be reordered.
 * It also implies a memory barrier.
 */
/*
extern int Test_and_Clear_Bit(int nr, void *addr);
extern int Test_Bit(int nr, long * addr);
*/
extern int usb_stor_control_thread(void * __us);
extern int  usb_stor_init(void);
extern void  usb_stor_exit(void);
extern void vChk_USB_LUNs(U8 uPort);
extern void usb_stor_release_resources(struct us_data *us);
/* The structure which use to save data for mass stor */

extern struct us_data *Mass_stor_us[NUM_OF_ROOT_HUB*MAX_USTOR]; //USB_2

static inline   void Clr_Stor_Dev_Info(MS_U8 uPort)
{
	Mass_stor_us[uPort] = NULL;
}

static inline BOOL Is_Stor_Dev_Info_Valid(MS_U8 uPort)
{
	return (Mass_stor_us[uPort] != NULL);
}

static inline BOOL Is_Stor_Dev_Init(MS_U8 uPort)
{
	return (Mass_stor_us[uPort]->bDevInit);
}

static inline void Set_Stor_Dev_Init(MS_U8 uPort, BOOL bInit)
{
	Mass_stor_us[uPort]->bDevInit = bInit;
}

static inline unsigned char Get_Stor_Max_Lun(MS_U8 uPort)
{
	return (Mass_stor_us[uPort]->max_lun);
}

///////////// drvKernel.h /////////////
#define	KERN_EMERG		"KERN_EMERG"	/* system is unusable			*/
#define	KERN_ALERT		"KERN_ALERT"	/* action must be taken immediately	*/
#define	KERN_CRIT		"KERN_CRIT"	/* critical conditions			*/
#define	KERN_NOTICE	"KERN_NOTICE"	/* normal but significant condition	*/



#endif
