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

#include "MsCommon.h"
#include "drvUSBHwCtl.h"
#include "drvEHCI.h"
#include "drvUSB.h"

void XBYTE_OR(MS_U32 Addr, MS_U8 offset, MS_U8 val)
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

void XBYTE_AND(MS_U32 Addr, MS_U8 offset,MS_U8 val)
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

void XBYTE_SET(MS_U32 Addr, MS_U8 offset,MS_U8 val)
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

MS_U8 XBYTE_READ(MS_U32 Addr, MS_U8 offset)
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
#include "drvEHCI.h"
void ResetMstarUsb(struct ehci_hcd *ehci)
{
    U32 reg_cmd, reg_inten;

    reg_inten = ehci_readl((U32)&ehci->regs->intr_enable);
    
    // 20110324: only reset UHC
    // TODO: without UTMI TX/RX reset after K1
    reg_cmd = ehci_readl((U32)&ehci->regs->command);
    ehci_writel(reg_cmd | CMD_RESET, (U32)&ehci->regs->command);
    //MsOS_DelayTask(2);    
    while(ehci_readb((U32)&ehci->regs->command) & CMD_RESET); // wait for reset done
    
    // restore UHC register
    ehci_writel (reg_inten, (U32)&ehci->regs->intr_enable);     
    ehci_writel (reg_cmd, (U32)&ehci->regs->command);
}
void UTMI_ORXBYTE_EX(MS_U8 offset,MS_U8 val, MS_U32 base)
{
    XBYTE_OR(base, offset, val);
}

void UTMI_ANDXBYTE_EX(MS_U8 offset,MS_U8 val, MS_U32 base)
{
    XBYTE_AND(base, offset, val);
}

void UTMI_SETXBYTE_EX(MS_U8 offset,MS_U8 val, MS_U32 base)
{
    XBYTE_SET(base, offset, val);
}

MS_U8 UTMI_READXBYTE_EX(MS_U8 offset, MS_U32 base)
{
    return XBYTE_READ(base, offset);
}

#if USBC_IP_SUPPORT // USBC control
void usbc_irq(MS_U32 regUTMI, MS_U32 regUSBC, struct s_UsbcInfo *pUsbc)
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
void _usbc_on_intr(InterruptNum eIntNum)
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
    usbc_irq(pChip->reg[p].baseUTMI, pChip->reg[p].baseUSBC, &pChip->usbc_ip[p]);
    MsOS_EnableInterrupt(eIntNum);

}
void init_usbc_intr(MS_U8 p)
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
#ifndef  MS_NOSAPI
    MsOS_AttachInterrupt(intNum, _usbc_on_intr);
    MsOS_EnableInterrupt(intNum);
#endif
}
#endif
