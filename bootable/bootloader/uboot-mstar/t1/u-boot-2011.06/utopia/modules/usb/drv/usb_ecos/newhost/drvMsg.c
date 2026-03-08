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
//#include <cyg/hal/hal_if.h> // NUSED

#include  "include/drvConfig.h"

#ifdef CONFIG_DEBUG
#define DEBUG
#else
#undef DEBUG
#endif

//#include  "include/drvCompiler.h" // NUSED
//#include  "include/drvErrno.h" // NUSED
//#include  "include/drvPorts.h" // NUSED
//#include  "include/drvPCIMEM.h" // NUSED
//#include  "include/drvTimer.h" // NUSED
//#include  "include/drvList.h" // NUSED

//#include "include/drvUSBHost.h" // NUSED
//#include "drvUsbd.h" // NUSED
//#include "include/drvCPE_AMBA.h" // NUSED
#include "drvEHCI.h"

/**
     * @brief               complete function of URB
     *
     * @param urb
     * @param regs
     *
     * @return          none
     */
static void ms_usb_api_completion(struct urb *urb, struct stPtRegs *regs)
{
    struct usb_api_data *pAwd = (struct usb_api_data *)urb->pContext;

    if (urb->SetDMABuf != NULL)
    {
        //MsOS_FreeMemory(urb->SetDMABuf, gs32NonCachedPoolID_MIU0 );
        Usb_FreeNonCachedMemory(urb->SetDMABuf);

        urb->SetDMABuf = NULL;
    }

    if (urb->TxDMABuf != NULL)
    {
        if (usb_pipein(urb->u32Pipe))
        {
        #ifdef DEBUG_PERFORMANCE
            if (urb->u32TransferBufferLength >= 512)
            diag_printf("ms_usb_api_completion: copy length: 0x%lx \n", urb->u32TransferBufferLength);
        #endif
            memcpy(urb->pTransferBuffer, urb->TxDMABuf, urb->u32TransferBufferLength);
        }
        //MsOS_FreeMemory(urb->TxDMABuf,gs32NonCachedPoolID_MIU0 );
        Usb_FreeNonCachedMemory(urb->TxDMABuf);

        urb->TxDMABuf = NULL;
    }

    pAwd->done = 1;
    wmb();

    wake_up(&pAwd->wqh);
}

/**
     * @brief               Start the URB and wait until it complete
     *
     * @param urb
     * @param timeout
     * @param act_len
     *
     * @return          function status
     */
static int ms_usb_send_urb(struct urb *urb, int timeout, int* act_len)
{
    struct usb_api_data stAwd;
    int status;
    U32 uDelay=0;

    stAwd.done = 0;

    urb->pContext = &stAwd;
    status = ms_usb_submit_urb(urb, GFP_ATOMIC);
    if (status)
    {
        ms_usb_free_urb(urb);
        return status;
    }

    while ( timeout && (stAwd.done == 0) )
    {
        if ( uDelay > (U32) timeout)
        {
            timeout =0;
            break;
        }
        else
        {
            mdelay(1);//tick base is 1 ms
            //HAL_DELAY_US(1000); // NUSED
            uDelay+=1;
        }
        rmb();
    }    

    if (!timeout && !stAwd.done)
    {
        diag_printf("usb_control/bulk_msg: timeout... %d \n", (int)urb->s32Status);
        if (urb->s32Status != -EINPROGRESS)
        {
            status = urb->s32Status;
        }
        else
        {
            struct usb_hcd * hcd =  urb->dev->bus->hcpriv;
            int retv;

            diag_printf("<%s> set Bad Device............\n", __FUNCTION__);
            hcd->isBadDevice = TRUE;
            hcd->badDeviceCnt++;

            /* Linux kernel put 5 seconds wait time for control commands,  we put 3 seconds here */
            if ((retv = ms_usb_unlink_urb(urb)) == -ETIMEDOUT)
            {
                diag_printf("timeout retry %d\n", retv);
                retv = ms_usb_unlink_urb(urb);
            }
            diag_printf("timeout unllnk urb error %d\n", retv);
            status = -ETIMEDOUT;
        }
    }
    else
        status = urb->s32Status;

    if (act_len)
        *act_len = urb->u32ActualLength;

    ms_usb_free_urb(urb);
    return status;
}

/**
     * @brief               Send a control setup command
     *
     * @param pDev
     * @param pipe
     * @param u8Reg
     * @param urReg_type
     * @param u16Val
     * @param u16Index
     * @param data
     * @param u16Size
     * @param timeout
     *
     * @return          function status
     */
int ms_usb_control_cmd(
        struct usb_device *pDev,
        unsigned int pipe,
        unsigned char u8Req,
        unsigned char u8Req_type,
        unsigned short u16Val,
        unsigned short u16Index,
        void *data,
        unsigned short u16Size,
        int timeout)
{
    struct usb_ctrlrequest *dr = (struct usb_ctrlrequest*) kmalloc(sizeof(struct usb_ctrlrequest), GFP_NOIO);
    struct urb *pUrb;
    int ret;
    int length = 0;
    U32 addr_t;

    if (!dr)
        return -ENOMEM;

    addr_t = (U32)dr;
    dr->bRequestType= u8Req_type;
    dr->bRequest = u8Req;
    dr->wValue = u16Val;
    dr->wIndex = u16Index;
    dr->wLength = u16Size;

    pUrb = ms_usb_alloc_urb(GFP_NOIO);
    if (!pUrb)
    {
        ret = -ENOMEM;
        goto Func_done;
    }

    ms_usb_stuff_control_urb(pUrb, pDev, pipe, (unsigned char*)dr, data, u16Size,
            ms_usb_api_completion, 0);
    ret = ms_usb_send_urb(pUrb, timeout, &length);
    if (ret < 0)
    {
        ms_debug_err("ms_usb_send_urb retv:%d, RT:%x,R:%x,V:%x,I:%x,L:%x\n", 
            ret, dr->bRequestType, dr->bRequest, dr->wValue, dr->wIndex, dr->wLength);
    }
    else
        ret = length;

Func_done:
    kfree((void *)addr_t);

    return ret;
}

/**
     * @brief               Send a bulk stansaction
     *
     * @param pUsb_dev
     * @param pipe
     * @param data
     * @param len
     * @param actual_length
     * @param timeout
     *
     * @return          function status
     */
#if 0 // NUSED     
int ms_usb_bulk_msg(
        struct usb_device *pUsb_dev,
        unsigned int pipe,
        void *data,
        int len,
        int *actual_length,
        int timeout)
{
    struct urb *urb;

    if (len < 0)
        return -EINVAL;

    urb=ms_usb_alloc_urb(GFP_KERNEL);
    if (!urb)
        return -ENOMEM;

    ms_usb_stuff_bulk_urb(urb, pUsb_dev, pipe, data, len,
            ms_usb_api_completion, 0);

    return ms_usb_send_urb(urb,timeout,actual_length);
}
#endif

/**
     * @brief               get descriptor from the device
     *
     * @param pDev
     * @param u8Type
     * @param u8Index
     * @param pBuf
     * @param size
     *
     * @return          function status
     */
int ms_usb_get_descriptor(
        struct usb_device *pDev,
        unsigned char u8Type,
        unsigned char u8Index,
        void *pBuf,
        int size)
{
    int retry = 3;
    int relt;

    memset(pBuf,0,size);

    while (retry--)
    {
        relt = ms_usb_control_cmd(pDev, usb_rcvctrlpipe(pDev, 0),
                USB_REQ_GET_DESCRIPTOR, USB_DIR_IN,
                (u8Type << 8) + u8Index, 0, pBuf, size,
                HZ * USB_CTRL_GET_TIMEOUT);

        if ( (relt > 0) || (relt == -EPIPE) )
            break;
    }

    return relt;
}

/**
     * @brief               get the device descriptor from the device
     *
     * @param pDev
     *
     * @return          function status
     */
int ms_usb_get_dev_descriptor(struct usb_device *pDev)
{

    return ms_usb_get_descriptor(pDev, USB_DT_DEVICE,
            0, &pDev->descriptor,
            sizeof(pDev->descriptor));
}

/**
     * @brief               Clear halt status of the endpoint
     *
     * @param pDev
     * @param pipe
     *
     * @return          function status
     */
int ms_usb_clear_halt(struct usb_device *pDev, int pipe)
{
    int retval;
    int endp;

    if (usb_pipein (pipe))
        endp = usb_pipeendpoint(pipe) | USB_DIR_IN;
    else
        endp = usb_pipeendpoint(pipe);

    retval = ms_usb_control_cmd(pDev, usb_sndctrlpipe(pDev, 0),
            USB_REQ_CLEAR_FEATURE, USB_RECIP_ENDPOINT, 0, endp, NULL, 0,
            HZ * USB_CTRL_SET_TIMEOUT);

    if (retval < 0)
        return retval;
    else
    {
        usb_settoggle(pDev, usb_pipeendpoint(pipe), usb_pipeout(pipe), 0);
        usb_endpoint_running(pDev, usb_pipeendpoint(pipe), usb_pipeout(pipe));

        return 0;
    }

}

/**
     * @brief               disable the endpoint
     *
     * @param pDev
     * @param epaddr
     *
     * @return          none
     */
extern void ms_usb_hcd_flush_endpoint(struct usb_device *pUdev, int iEndpoint);
extern void ms_hcd_check_dev_urb (struct usb_device *pUdev, int iEndpoint);
void ms_usb_disable_endpoint(struct usb_device *pDev, U32 epaddr)
{
    U32 u32Ept = epaddr & USB_ENDPOINT_NUMBER_MASK;
    
    if (pDev && pDev->bus && pDev->bus->bus_ops && pDev->bus->bus_ops->ms_disable_ep)
    {
        if (pDev->pEpMaxPacketIn[u32Ept] || pDev->pEpMaxPacketOut[u32Ept])
        {
            ms_usb_hcd_flush_endpoint(pDev, epaddr);
            pDev->bus->bus_ops->ms_disable_ep(pDev, epaddr);
            ms_hcd_check_dev_urb(pDev, epaddr); // check urb
        }
    }
    else // never go through if bus_op->ms_disable_ep() declared
    {
        if (usb_endpoint_dir(epaddr))
        {
            usb_endpoint_halt(pDev, u32Ept, 1);
            pDev->pEpMaxPacketOut[u32Ept] = 0;
        }
        else
        {
            usb_endpoint_halt(pDev, u32Ept, 0);
            pDev->pEpMaxPacketIn[u32Ept] = 0;
        }
    }
}

/**
     * @brief               disable the interface
     *
     * @param pDev
     * @param pIntf
     *
     * @return          none
     */
void ms_usb_disable_interface(struct usb_device *pDev, struct usb_interface *pIntf)
{
    int EptIdx;
    struct usb_host_interface *hintf = &pIntf->altsetting[pIntf->act_altsetting];

    for (EptIdx = 0; EptIdx < hintf->desc.bNumEndpoints; ++EptIdx)
        ms_usb_disable_endpoint(pDev, hintf->endpoint[EptIdx].desc.bEndpointAddress);
}

/**
     * @brief               disable the device
     *
     * @param pDev
     * @param skip_ep0
     *
     * @return          none
     */
void ms_usb_disable_device(struct usb_device *pDev, int skip_ep0)
{
    int EptIdx;

    for (EptIdx = skip_ep0; EptIdx < 16; ++EptIdx)
    {
        ms_usb_disable_endpoint(pDev, EptIdx);
        ms_usb_disable_endpoint(pDev, EptIdx + USB_DIR_IN);
    }
}

/**
     * @brief               enable the endpoint
     *
     * @param pDev
     * @param pEpt_desc
     *
     * @return          none
     */
void ms_usb_enable_endpoint(struct usb_device *pDev,
    struct usb_endpoint_descriptor *pEpt_desc)
{
    unsigned int eptaddr = pEpt_desc->bEndpointAddress;
    unsigned int eptnum = eptaddr & USB_ENDPOINT_NUMBER_MASK;
    int is_control = ((pEpt_desc->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
        USB_ENDPOINT_XFER_CONTROL);

    if (usb_endpoint_dir(eptaddr) || is_control)
    {
        usb_endpoint_running(pDev, eptnum, 1);
        usb_settoggle(pDev, eptnum, 1, 0);
        pDev->pEpMaxPacketOut[eptnum] = pEpt_desc->wMaxPacketSize;
    }

    if (!usb_endpoint_dir(eptaddr) || is_control)
    {
        usb_endpoint_running(pDev, eptnum, 0);
        usb_settoggle(pDev, eptnum, 0, 0);
        pDev->pEpMaxPacketIn[eptnum] = pEpt_desc->wMaxPacketSize;
    }
}

/**
     * @brief               enable the interface
     *
     * @param pDev
     * @param pIntf
     *
     * @return          none
     */
void ms_usb_enable_interface(struct usb_device *pDev,
    struct usb_interface *pIntf)
{
    struct usb_host_interface *pAlt_set =
            &pIntf->altsetting[pIntf->act_altsetting];
    int i;

    for (i = 0; i < pAlt_set->desc.bNumEndpoints; ++i)
        ms_usb_enable_endpoint(pDev, &pAlt_set->endpoint[i].desc);
}

/**
     * @brief               set specific interface to the device
     *
     * @param pDev
     * @param interface
     * @param alternate
     *
     * @return          function status
     */
int ms_usb_set_interface(struct usb_device *pDev, int interface, int alternate)
{
    struct usb_interface *pIntf;
    int ret;
    int manual = 0;

    pIntf = ms_usb_ifnum_to_if(pDev, interface);
    if (!pIntf)
    {
        diag_printf("Invalid interface %d", interface);
        return -EINVAL;
    }

    if (alternate < 0 || alternate >= pIntf->num_altsetting)
        return -EINVAL;

    ret = ms_usb_control_cmd(pDev, usb_sndctrlpipe(pDev, 0),
                USB_REQ_SET_INTERFACE, USB_RECIP_INTERFACE,
                pIntf->altsetting[alternate]
                .desc.bAlternateSetting,
                interface, NULL, 0, HZ * 5);

    if (ret == -EPIPE && pIntf->num_altsetting == 1)
    {
        ms_debug_msg("manual set interface for pIntf %d, alt %d",
        interface, alternate);
        manual = 1;
    }
    else if (ret < 0)
        return ret;

    ms_usb_disable_interface(pDev, pIntf);

    pIntf->act_altsetting = alternate;

    if (manual)
    {
        struct usb_host_interface *iface_as = &pIntf->altsetting[alternate];
        int ii;

        for (ii = 0; ii < iface_as->desc.bNumEndpoints; ii++)
        {
            unsigned int epaddr = iface_as->endpoint[ii].desc.bEndpointAddress;
            unsigned int pipe =
            __pack_pipe(pDev, USB_ENDPOINT_NUMBER_MASK & epaddr)
            | (usb_endpoint_dir(epaddr) ? USB_DIR_OUT : USB_DIR_IN);

            ms_usb_clear_halt(pDev, pipe);
        }
    }

    ms_usb_enable_interface(pDev, pIntf);

    return 0;
}

/**
     * @brief               set specific configuration to the device
     *
     * @param pDev
     * @param config_val
     *
     * @return          function status
     */
int ms_usb_set_config(struct usb_device *pDev, int config_val)
{
    int i, ret;
    struct usb_host_config *cp = NULL;

    for (i=0; i<pDev->descriptor.bNumConfigurations; i++)
    {
        if (pDev->config[i].desc.bConfigurationValue == config_val)
        {
            cp = &pDev->config[i];
            break;
        }
    }
    if ((!cp && config_val != 0) || (cp && config_val == 0))
    {
        diag_printf("selecting invalid config_val %d", config_val);
        return -EINVAL;
    }

    if (pDev->eState != USB_STATE_ADDRESS)
        ms_usb_disable_device (pDev, 1);  // Skip ep0
    pDev->toggle[0] = pDev->toggle[1] = 0;
    pDev->halted[0] = pDev->halted[1] = 0;
    pDev->eState = USB_STATE_ADDRESS;

    if ((ret = ms_usb_control_cmd(pDev, usb_sndctrlpipe(pDev, 0),
        USB_REQ_SET_CONFIGURATION, 0, config_val, 0,
        NULL, 0, HZ * USB_CTRL_SET_TIMEOUT)) < 0)
        return ret;
    if (config_val)
        pDev->eState = USB_STATE_CONFIGURED;
    pDev->actconfig = cp;

    for (i = 0; i < cp->desc.bNumInterfaces; ++i)
    {
        struct usb_interface *intf = cp->interface[i];

        intf->act_altsetting = 0;
        ms_usb_enable_interface(pDev, intf);
    }

    return 0;
}


/**
     * @brief               get the string from the device
     *
     * @param pDev
     * @param index
     * @param pBuf
     * @param size
     *
     * @return          function status
     */
int ms_usb_string(struct usb_device *pDev, int index, char *pBuf, size_t size)
{
    unsigned char *pTmp_buf;
    int err, len;
    U32 u, idx;
    U32 addr_t;

    if ( (int)size <= 0 || !pBuf || !index)
        return -EINVAL;

    pBuf[0] = 0;
    pTmp_buf = (unsigned char*) kmalloc(256, GFP_KERNEL);

    if (!pTmp_buf)
        return -ENOMEM;

    addr_t = (U32)pTmp_buf;
    /* get langid for strings if it's not yet known */
    if (!pDev->u32HaveLangId)
    {
        err = ms_usb_control_cmd(pDev, usb_rcvctrlpipe(pDev, 0),
	        USB_REQ_GET_DESCRIPTOR, USB_DIR_IN,
	        (USB_DT_STRING << 8), 0, pTmp_buf, 4,
	        HZ * USB_CTRL_GET_TIMEOUT);

        if (err < 0)
        {
            ms_debug_msg("error getting string descriptor 0 (error=%d)", err);
            goto errout;
        }
        else if (err < 4 || pTmp_buf[0] < 4)
        {
            ms_debug_msg("string descriptor 0 too short%s","");
            err = -EINVAL;
            goto errout;
        }
        else
        {
            pDev->u32HaveLangId = -1;
            pDev->u32StringLangId = pTmp_buf[2] | (pTmp_buf[3]<< 8);
            /* always use the first langid listed */
            ms_debug_msg("USB device number %d default language ID 0x%x",
            pDev->u32DevNum, pDev->u32StringLangId);
        }
    }

    err = ms_usb_control_cmd(pDev, usb_rcvctrlpipe(pDev, 0),
            USB_REQ_GET_DESCRIPTOR, USB_DIR_IN,
            (USB_DT_STRING << 8) + index, pDev->u32StringLangId, pTmp_buf, 2,
            HZ * USB_CTRL_GET_TIMEOUT);

    if(err<2)
        goto errout;
    len=pTmp_buf[0];

    err = ms_usb_control_cmd(pDev, usb_rcvctrlpipe(pDev, 0),
            USB_REQ_GET_DESCRIPTOR, USB_DIR_IN,
            (USB_DT_STRING << 8) + index, pDev->u32StringLangId, pTmp_buf, len,
            HZ * USB_CTRL_GET_TIMEOUT);

    if (err < 0)
        goto errout;

    size--;
    for (idx = 0, u = 2; u < (U32) err; u += 2)
    {
        if (idx >= size)
            break;
        if (pTmp_buf[u+1])
            pBuf[idx++] = '?';
        else
            pBuf[idx++] = pTmp_buf[u];
    }
    pBuf[idx] = 0;
    err = idx;

errout:
    kfree((void *)addr_t);
    return err;
}
