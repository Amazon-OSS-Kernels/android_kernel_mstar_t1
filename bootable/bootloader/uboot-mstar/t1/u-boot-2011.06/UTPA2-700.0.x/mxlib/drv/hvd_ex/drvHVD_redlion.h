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

////////////////////////////////////////////////////////////////////////////////////////////////////
// file   drvHVD_redlion.h
// @brief  HVD Driver Interface
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DRV_HVD_REDLION_H__
#define __DRV_HVD_REDLION_H__

#if defined(CHIP_T2)
#define HVD_ISR_VECTOR    (32+8)
#elif defined(CHIP_T3) || defined(CHIP_JANUS)
#define HVD_ISR_VECTOR    (32+8)
#else
#define HVD_ISR_VECTOR    (32+8)
#endif

typedef int irqreturn_t;
typedef irqreturn_t (*irq_handler_t)(int, void *);

void* MDrv_SYS_PA2NonCacheSeg( void* pAddrPA );
void printk(char* pformat,...);
void msleep(unsigned int msecs);
int request_irq(unsigned int irq, irq_handler_t handler,
		unsigned long irqflags, const char *devname, void *dev_id);
void free_irq(unsigned int irq, void *dev_id);
void enable_irq(unsigned int irq);
void disable_irq(unsigned int irq);
unsigned long MHal_H264_GetSyetemTime( void );
void MHal_H264_Delay_ms( unsigned long ms );


#define OSAL_HVD_ISR_Attach( pfnISRCB)   request_irq(HVD_ISR_VECTOR,pfnISRCB,0,"h264",NULL)
#define OSAL_HVD_ISR_Detach()       free_irq(HVD_ISR_VECTOR ,NULL)
#define OSAL_HVD_ISR_Enable()       //enable_irq(HVD_ISR_VECTOR)
#define OSAL_HVD_ISR_Disable()      //disable_irq(HVD_ISR_VECTOR)

#endif // #ifndef __DRV_HVD_REDLION_H__
