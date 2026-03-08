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

#ifndef _USB_H_
#define _USB_H_

#include <MsTypes.h>
#include "include/drvUSBHost.h"
#include "include/drvCompletion.h"
#include "include/drvBitops.h"
#include "drvUsbHostConfig.h"
#include "drvMSC.h"

struct ms_usdata;
struct stUnusual_dev
{
    const char* vendorName;
    const char* productName;
    unsigned char  useProtocol;
    unsigned char  useTransport;
    int (*initFunction)(struct ms_usdata *);
    U32 flags;
};

/* usb storage flag, bit indicator */
#define MS_SINGLE_LUN      0x00000001
#define MS_SCM_MULT_TARG   0x00000020
#define MS_FIX_INQUIRY     0x00000040
#define MS_FIX_CAPACITY    0x00000080

/* usb storage flag, bit indicator shift */
#define MS_URB_ACTIVE	        18
#define MS_ABORTING             20
#define MS_DISCONNECTING        21
#define MS_RESETTING            22

#define NO_SUBMIT   ((1UL << MS_ABORTING) | \
            (1UL << MS_DISCONNECTING) | (1UL << MS_RESETTING))

/* usb storage current operating state */
#define IDLE_STATE          1 // there is the same copy in the drvKernel.h
#define RUNNING_STATE       2
#define RESETTING_STATE     3
#define ABORTING_STATE      4
#define MAX_USB_MSC_LUNS    8

#define MS_IOBUF_SIZE   64 // there is the same copy in the drvKernel.h

typedef int (*trans_cmnd)(stCmd_scsi*, struct ms_usdata*);
typedef int (*trans_reset)(struct ms_usdata*);
typedef void (*proto_cmnd)(stCmd_scsi*, struct ms_usdata*);
typedef void (*extra_data_destructor)(void *);

struct ms_usdata
{
    struct usb_device   *pusb_dev;
    struct usb_interface    *pusb_intf;
    struct stUnusual_dev   *unusual_dev;
    U32     flags;
    U32     bulk_send_pipe;
    U32     bulk_recv_pipe;
    U32     ctrl_send_pipe;
    U32     ctrl_recv_pipe;
    U32     int_recv_pipe;
    char            vendor[MSC_STRING_LEN];
    char            product[MSC_STRING_LEN];
    char            serial[MSC_STRING_LEN];
    unsigned char           subclass;
    unsigned char           protocol;
    unsigned char           max_lun;
    unsigned char           ifnum;  //interface number
    unsigned char           bInterval_ep;
    trans_cmnd      transport;
    trans_reset     transport_reset;
    proto_cmnd      proto_handler;
    stCmd_scsi      srb;
    Scsi_Device     device;
    int             usb_state;
    struct urb      *current_urb;
    struct urb      *data_urb;
    struct urb      *status_urb;
    struct usb_ctrlrequest  *cr;
    unsigned char   *iobuf;
    dma_addr_t      cr_dma;
    dma_addr_t      iobuf_dma;
    struct stCompletion   notify;
    void            *extra;
    extra_data_destructor	extra_destructor;

    BOOL bDevInit;
    struct LUN_Device* msc_device;

    unsigned char   ustor_id;
    unsigned char   host_id;
};

struct LUN_Device
{
    unsigned char u8LunNum;
    unsigned char u8DevType;
    U32 u32BlockSize;
    U32 u32BlockSize_log2;
    U32 u32BlockTotalNum;
    BOOL bDeviceReady;
    BOOL bWriteProtect;
    BOOL bFSInit;
};

extern struct usb_driver usb_storage_driver;

#define scsi_unlock(host)   osapi_spin_unlock_irq(host->host_lock)
#define scsi_lock(host)     osapi_spin_lock_irq(host->host_lock)
#define sg_address(psg)     (page_address((psg).page) + (psg).offset)

#define SupportOtherDevice	1

extern int ms_usb_msc_control_thread(void * __us);
extern BOOL ms_vChk_USB_LUNs(U8 us_id);
extern struct ms_usdata *Mass_stor_us[NUM_OF_ROOT_HUB*MAX_USTOR];

static inline   void Clr_Stor_Dev_Info(MS_U8 us_id)
{
    Mass_stor_us[us_id] = NULL;
}

static inline BOOL Is_Stor_Dev_Info_Valid(MS_U8 us_id)
{
    return (Mass_stor_us[us_id] != NULL);
}

static inline BOOL Is_Stor_Dev_Init(MS_U8 us_id)
{
    return (Mass_stor_us[us_id]->bDevInit);
}

static inline void Set_Stor_Dev_Init(MS_U8 us_id, BOOL bInit)
{
    Mass_stor_us[us_id]->bDevInit = bInit;
}

static inline unsigned char Get_Stor_Max_Lun(MS_U8 us_id)
{
    return (Mass_stor_us[us_id]->max_lun);
}

///////////// drvKernel.h /////////////
#define KERN_EMERG      "KERN_EMERG"    /* system is unusable   */
#define KERN_ALERT      "KERN_ALERT"    /* action must be taken immediately */
#define KERN_CRIT       "KERN_CRIT" /* critical conditions      */
#define KERN_NOTICE     "KERN_NOTICE"   /* normal but significant condition */

#endif
