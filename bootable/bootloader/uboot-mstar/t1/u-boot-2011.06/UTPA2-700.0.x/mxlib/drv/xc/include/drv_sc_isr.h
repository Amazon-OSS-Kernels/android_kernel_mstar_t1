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
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _MDRV_XC_ISR_H_
#define _MDRV_XC_ISR_H_

#ifdef _MDRV_XC_ISR_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

// XC interrupt
#define MAX_ISR_NUM_OF_EACH_INT     (10)

typedef struct __attribute__((packed))
{
    void*  aryXC_ISR_Param;         // parameter will be passed to ISR when calling it.
#if !defined (__aarch64__)
    MS_U32 u32AlignmentDummy0;
#endif
    SC_InterruptCb aryXC_ISR;       // attach ISR
#if !defined (__aarch64__)
    MS_U32 u32AlignmentDummy1;
#endif
} XC_ISR_INFO;

//////////////////////////////////
// Interrupt Function
//////////////////////////////////
INTERFACE MS_U8   MDrv_XC_InterruptGetMaxIntNum(void *pInstance);
INTERFACE MS_BOOL MDrv_XC_InterruptAvaliable(void *pInstance, SC_INT_SRC eIntNum);

INTERFACE MS_BOOL MDrv_XC_InterruptIsAttached(void *pInstance, SC_INT_SRC eIntNum, SC_InterruptCb pIntCb, void * pParam);
INTERFACE MS_BOOL MDrv_XC_InterruptAttach(void *pInstance, SC_INT_SRC eIntNum, SC_InterruptCb pIntCb, void * pParam);
INTERFACE MS_BOOL MDrv_XC_InterruptDeAttach(void *pInstance, SC_INT_SRC eIntNum, SC_InterruptCb pIntCb, void * pParam);
INTERFACE MS_BOOL MDrv_XC_InterruptAttachWithoutMutex(void *pInstance, SC_INT_SRC eIntNum, SC_InterruptCb pIntCb, void * pParam);
INTERFACE MS_BOOL MDrv_XC_InterruptDeAttachWithoutMutex(void *pInstance, SC_INT_SRC eIntNum, SC_InterruptCb pIntCb, void * pParam);

#ifdef MSOS_TYPE_LINUX_KERNEL
INTERFACE void MDrv_SC_set_interrupt(void *pInstance, SC_INT_SRC u8IntSrc, MS_BOOL bEnable);
irqreturn_t MDrv_SC_isr(InterruptNum eIntNum, void* dev_id);
irqreturn_t MDrv_SC1_isr(InterruptNum eIntNum, void* dev_id);
#else
INTERFACE void MDrv_SC_isr(InterruptNum eIntNum);
INTERFACE void MDrv_SC1_isr(InterruptNum eIntNum);
#endif

#undef INTERFACE
#endif //_MDRV_XC_ISR_H_

