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

//#include <MsCommon.h> // NUSED
#include "drvUSBHwCtl.h"
#include "drvEHCI.h"
//#include "drvUSB.h" // NUSED
/* applying drvUSB.h (inside drvUSBHwCtl.h) */

void ms_XBYTE_OR(MS_U32 Addr, MS_U8 offset, MS_U8 val)
{
    MS_U16 temp;

    if (offset & 1)
    {
        temp=*(MS_U16 volatile   *)(Addr+(offset-1)*2);
        *(MS_U16 volatile   *)(Addr+(offset-1)*2)=(((MS_U16)val)<<8) | (temp );
    }
    else
    {
        temp=*(MS_U16 volatile   *)(Addr+offset*2);
        *(MS_U16 volatile   *)(Addr+offset*2)=(temp )|val;
     }
}

void ms_XBYTE_AND(MS_U32 Addr, MS_U8 offset,MS_U8 val)
{
    MS_U16 temp;

    if (offset &1)
    {
        temp=*(MS_U16 volatile   *)(Addr+(offset-1)*2);
        *(MS_U16 volatile   *)(Addr+(offset-1)*2)=((((MS_U16)val)<<8)|0xff) & (temp );
    }
    else
    {
        temp=*(MS_U16 volatile   *)(Addr+offset*2);
        *(MS_U16 volatile   *)(Addr+offset*2)=(temp & (0xff00|val) );

     }
}

void ms_XBYTE_SET(MS_U32 Addr, MS_U8 offset,MS_U8 val)
{
    MS_U16 temp;

    if (offset &1)
    {
        temp=*(MS_U16 volatile   *)(Addr+(offset-1)*2);
        *(MS_U16 volatile   *)(Addr+(offset-1)*2)=((temp & 0x00ff) | (((MS_U16)val)<<8));
    }
    else
    {
        temp=*(MS_U16 volatile   *)(Addr+offset*2);
        *(MS_U16 volatile   *)(Addr+offset*2)=((temp & 0xff00) |val );
     }
}

MS_U8 ms_XBYTE_READ(MS_U32 Addr, MS_U8 offset)
{
    MS_U16 temp;
    MS_U8  uRetVal = 0;

    if (offset &1)
    {
        temp=*(MS_U16 volatile   *)(Addr+(offset-1)*2);
        uRetVal = (MS_U8) (temp >> 8);
    }
    else
    {
        temp=*(MS_U16 volatile   *)(Addr+offset*2);
        uRetVal = (MS_U8) temp;
    }

    //diag_printf("XBYTE_READ: Addr: %X, offset: %X, uRetVal: %X\n", Addr, offset, uRetVal);
    return uRetVal;
}
// ------------------------------------------------------------------------
extern void ms_ehci_softrst(struct ehci_hcd  *pEhci);
void ms_ResetMstarUsb(struct usb_hcd *hcd)
{
    U32 temp;
    struct ehci_hcd *ehci = hcd_to_ehci(hcd);

    //disable force enter FSmode, 20130220 place here
    temp = hcd_reg_readl((U32)&ehci->op_regs->bus_control);
    temp &= ~0x80;
    hcd_reg_writel(temp, (U32)&ehci->op_regs->bus_control);

#if 1
    ms_ehci_softrst(ehci);
#else
    // FIXME: define var here will cause compile error/worning
    U32 reg_cmd, reg_inten;

    reg_inten = hcd_reg_readl((U32)&ehci->op_regs->usbintr);

    // 20110324: only reset UHC
    // TODO: without UTMI TX/RX reset after K1
    reg_cmd = hcd_reg_readl((U32)&ehci->op_regs->usbcmd);
    hcd_reg_writel(reg_cmd | USBCMD_RESET, (U32)&ehci->op_regs->usbcmd);
    MsOS_DelayTask(2);
    while(hcd_reg_readb((U32)&ehci->op_regs->usbcmd) & USBCMD_RESET); // wait for reset done

    // restore UHC register
    hcd_reg_writel (reg_inten, (U32)&ehci->op_regs->usbintr);
    hcd_reg_writel (reg_cmd, (U32)&ehci->op_regs->usbcmd);
#endif
}

inline BOOL ms_RoothubPortConnected(struct usb_hcd *hcd)
{
    struct ehci_hcd *ehci = hcd_to_ehci(hcd);
    U32 regv;

    regv = hcd_reg_readw((U32)&ehci->op_regs->portsc[0]);
    return (regv & PORTSC_CONNECT) ? TRUE : FALSE;
}

inline BOOL ms_RoothubPortEnabled(struct usb_hcd *hcd)
{
    struct ehci_hcd *ehci = hcd_to_ehci(hcd);
    U32 regv;

    regv = hcd_reg_readw((U32)&ehci->op_regs->portsc[0]);
    return (regv & PORTSC_PE) ? TRUE : FALSE;
}

inline BOOL ms_isHcdRunning(struct usb_hcd *hcd)
{
    struct ehci_hcd *ehci = hcd_to_ehci(hcd);
    U32 regv;

    regv = hcd_reg_readw((U32)&ehci->op_regs->usbcmd);
    return (regv & USBCMD_RUN) ? TRUE : FALSE;
}

inline void ms_forceHcdRun(struct usb_hcd *hcd)
{
    struct ehci_hcd *ehci = hcd_to_ehci(hcd);
    U32 regv;

    regv = hcd_reg_readw((U32)&ehci->op_regs->usbsts);
    if (regv & USBSTS_HALT)
    {
        diag_printf("[UM-disconnect] force RUN!!!\n");
        hcd_reg_writel(USBCMD_RUN | hcd_reg_readl((U32)&ehci->op_regs->usbcmd),
            (U32)&ehci->op_regs->usbcmd);
    }
}

// ------------------------------------------------------------------------

void ms_UTMI_ORXBYTE_EX(MS_U8 offset,MS_U8 val, MS_U32 base)
{
    ms_XBYTE_OR(base, offset, val);
}

void ms_UTMI_ANDXBYTE_EX(MS_U8 offset,MS_U8 val, MS_U32 base)
{
    ms_XBYTE_AND(base, offset, val);
}

void ms_UTMI_SETXBYTE_EX(MS_U8 offset,MS_U8 val, MS_U32 base)
{
    ms_XBYTE_SET(base, offset, val);
}

MS_U8 ms_UTMI_READXBYTE_EX(MS_U8 offset, MS_U32 base)
{
    return ms_XBYTE_READ(base, offset);
}

void ms_ehci_interrupt_enable (struct usb_hcd *hcd)
{
    struct ehci_hcd  *pEhci = hcd_to_ehci (hcd);
    U32 u32Reg_t;

    u32Reg_t = (HOST20_USBINTR_IntOnAsyncAdvance |
            HOST20_USBINTR_SystemError |
            HOST20_USBINTR_PortChangeDetect |
            HOST20_USBINTR_USBError |
            HOST20_USBINTR_CompletionOfTransaction);
    hcd_reg_writel (u32Reg_t, (U32)&pEhci->op_regs->usbintr);

    u32Reg_t = hcd_reg_readl((U32)&pEhci->op_regs->usbsts);
    hcd_reg_writel (u32Reg_t, (U32)&pEhci->op_regs->usbsts); //clear all pending interrupt

    u32Reg_t = hcd_reg_readl((U32)&pEhci->op_regs->bus_control);
    u32Reg_t|= INT_POLAR;
    hcd_reg_writel (u32Reg_t, (U32)&pEhci->op_regs->bus_control); // set interrupt polarity high
}

void ms_ehci_interrupt_disable (struct usb_hcd *hcd)
{
    struct ehci_hcd  *pEhci = hcd_to_ehci (hcd);
    U32 u32Reg_t;
    hcd_reg_writel (0, (U32)&pEhci->op_regs->usbintr);

    u32Reg_t = hcd_reg_readl((U32)&pEhci->op_regs->usbsts);
    hcd_reg_writel (u32Reg_t, (U32)&pEhci->op_regs->usbsts); //clear all pending interrupt
}

#if USBC_IP_SUPPORT // USBC control
void ms_usbc_irq(MS_U32 regUTMI, MS_U32 regUSBC, struct s_UsbcInfo *pUsbc)
{
    U16 status, vbus_t;

    pUsbc->intSts = status = usb_readw((void*)(regUSBC+0x6*2));
    vbus_t = usb_readw((void*)(regUTMI+0x30*2)) & 0x20;  // bit[5]
    pUsbc->eventType = vbus_t ? 1 : 0;
    diag_printf("<usbc_irq> status change(%x) vbus(%x)\n", status, vbus_t);
    status &= pUsbc->intEn;
    usb_writew(status, (void*)(regUSBC+0x6*2)); // write 1 clear status
    if (status)
        pUsbc->eventFlag = 1;
}

extern struct s_ChipUsbHostDef *pCurrentChip;
void ms_usbc_on_intr(InterruptNum eIntNum)
{
    struct s_ChipUsbHostDef *pChip = pCurrentChip;
    MS_U8 p;

    if (pChip == NULL)
        return;
    MsOS_DisableInterrupt(eIntNum);
    for (p = 0; p < pChip->nRootHub; p++)
    {
        if (eIntNum == pChip->reg[p].usbcIRQ)
            break;
    }
    ms_usbc_irq(pChip->reg[p].baseUTMI, pChip->reg[p].baseUSBC, &pChip->usbc_ip[p]);
    MsOS_EnableInterrupt(eIntNum);

}
void ms_init_usbc_intr(MS_U8 p)
{
    struct s_ChipUsbHostDef *pChip = pCurrentChip;
    struct s_UsbcInfo *pUsbc = &pChip->usbc_ip[p];
    MS_U8 intNum = pChip->reg[p].usbcIRQ;
    MS_U32 regUSBC = pChip->reg[p].baseUSBC;

    pUsbc->portNum = p;
    pUsbc->eventFlag = 0;
    pUsbc->intEn = USBCINTR_VBusValidChange;
    //pUsbc->intEn = USBCINTR_AValidChange; // for testing
    pUsbc->int_pol = 1;

    diag_printf("<init_usbc_intr> port: %d, enable %x\n", p, pUsbc->intEn);
    //usb_writeb((pUsbc->int_pol << 2) || usb_readb((void*)(regUSBC+0x2*2)), (void*)(regUSBC+0x2*2)); // set interrupt polarity
    usb_writew(pUsbc->intEn, (void*)(regUSBC+0x6*2)); // clear interrupt status
    usb_writew(pUsbc->intEn, (void*)(regUSBC+0x4*2)); // set interrupt enable
    MsOS_AttachInterrupt(intNum, ms_usbc_on_intr);
    MsOS_EnableInterrupt(intNum);
}
#endif
