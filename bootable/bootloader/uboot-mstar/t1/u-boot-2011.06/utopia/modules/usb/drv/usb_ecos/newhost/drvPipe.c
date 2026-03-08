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

//#include <MsCommon.h> // NUSED

//#include "include/drvConfig.h" // NUSED
//#include "include/drvCompiler.h" // NUSED
//#include "include/drvErrno.h" // NUSED
//#include "include/drvPorts.h" // NUSED
//#include "include/drvTimer.h" // NUSED
//#include "include/drvList.h" // NUSED
//#include "include/drvKernel.h" // NUSED

// USB related header files
//#include "include/drvUSBHost.h" // NUSED
#include "drvUsbd.h"

/**
     * @brief               create new urb
     *
     * @param           int iso_packets
     * @param           int mem_flags     
     *
     * @return          allocated urb
     */
struct urb *ms_usb_alloc_urb(int mem_flags)
{
    struct urb *ms_urb;

    ms_urb = (struct urb *)kmalloc(sizeof(struct urb), mem_flags);
    if (!ms_urb) 
    {
        diag_printf("alloc_urb: kmalloc failed%s","\n");
        return NULL;
    }
    memset(ms_urb, 0, sizeof(*ms_urb));
    ms_urb->count = (atomic_t)osapi_ATOMIC_INIT(1);
    osapi_spin_lock_init(&ms_urb->lock);
    return ms_urb;
}

/**
     * @brief               test urb reference count and free the memory
     *
     * @param           struct urb *ms_urb
     *
     * @return          none
     */
void ms_usb_free_urb(struct urb *ms_urb)
{
    if (ms_urb)
        if (osapi_atomic_dec_and_test(&ms_urb->count))
            kfree(ms_urb);
}

/**
     * @brief               increase urb reference count
     *
     * @param           struct urb *urb
     *
     * @return          current urb
     */
struct urb * ms_usb_get_urb(struct urb *ms_urb)
{
    if (ms_urb) 
    {
        osapi_atomic_inc(&ms_urb->count);
        return ms_urb;
    } 
    else
        return NULL;
}

/**
     * @brief               submit urb to the queue
     *
     * @param           struct urb *ms_urb
     * @param           int mem_flags     
     *
     * @return          error code
     */
int ms_usb_submit_urb(struct urb *ms_urb, int mem_flags)
{
    int ms_pipe, temp_p, max_p;
    struct usb_device  *pUdev;
    struct ms_usb_bus_ops  *usb_op;
    int      is_out;

    if (!ms_urb || ms_urb->hcpriv || !ms_urb->complete_func)
        return -EINVAL;
    if (!(pUdev = ms_urb->dev) || (pUdev->eState < USB_STATE_DEFAULT) ||
        (!pUdev->bus) || (pUdev->u32DevNum <= 0))
        return -ENODEV;
    if (!(usb_op = pUdev->bus->bus_ops) || !usb_op->ms_hcd_submit_urb)
        return -ENODEV;    

    ms_urb->s32Status = -EINPROGRESS;
    ms_urb->u32ActualLength = 0;    

    ms_pipe = ms_urb->u32Pipe;
    temp_p = usb_pipetype (ms_pipe);
    is_out = usb_pipeout (ms_pipe);
  
    if (!usb_pipecontrol (ms_pipe) && pUdev->eState < USB_STATE_CONFIGURED)
        return -ENODEV;

    if (usb_endpoint_halted (pUdev, usb_pipeendpoint (ms_pipe), is_out))
        return -EPIPE;

    max_p = usb_maxpacket (pUdev, ms_pipe, is_out);
    if (max_p <= 0) 
    {
        ms_usbhost_err ("<%s> wrong endpoint %d-%s on usb-%s-%s (bad maxpacket %d)",
            __FUNCTION__,
            usb_pipeendpoint (ms_pipe), is_out ? "OUT" : "IN",
            pUdev->bus->bus_name, pUdev->devpath,
            max_p);
        return -EMSGSIZE;
    }    

    if (ms_urb->u32TransferBufferLength < 0)
        return -EMSGSIZE;

    //only support interrupt pipe
    if (temp_p == EP_INTERRUPT) 
    {        
            if (ms_urb->u32Interval <= 0)
                return -EINVAL;
            switch (pUdev->eSpeed) 
            {
                case USB_HIGH_SPEED:  /* units are microframes */
                    // NOTE usb handles 2^15
                    if (ms_urb->u32Interval > (1024 * 8))
                        ms_urb->u32Interval = 1024 * 8;
                    temp_p = 1024 * 8;
                    break;
                case USB_FULL_SPEED:  /* units are frames/msec */
                case USB_LOW_SPEED:
                    {
                        if (ms_urb->u32Interval > 255)
                            return -EINVAL;
                        // NOTE ohci only handles up to 32
                        temp_p = 128;
                    } 
                    break;
                default:
                    return -EINVAL;
            }
            while (temp_p > ms_urb->u32Interval)
                temp_p >>= 1;
            ms_urb->u32Interval = temp_p;
    }

    return usb_op->ms_hcd_submit_urb (ms_urb, mem_flags);
}

/**
     * @brief               unlink urb from the queue
     *
     * @param           struct urb *ms_urb
     *
     * @return          error code
     */
int ms_usb_unlink_urb(struct urb *ms_urb)
{
    if (ms_urb && ms_urb->dev && ms_urb->dev->bus && ms_urb->dev->bus->bus_ops)
        return ms_urb->dev->bus->bus_ops->ms_hcd_unlink_urb(ms_urb);
    else
        return -ENODEV;
}

