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
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifndef __UBOOT__
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#endif

#include "MFE_chip.h"
#include "mfe_type.h"
#include "mfe_common.h"
#include "madp_ms_dprintf.h"

#ifdef WIN32
#include <windows.h>
#endif
#ifdef __MFE_G2__
#include <fcntl.h>
#include <sys/ioctl.h>
#endif


#ifdef _MFE_UTOPIA_
static MS_S32 madp_sem_input = -1;


//#define NO_INTERRUPT

void MDrv_MFE_ISR(InterruptNum eIntNum)
{
    ms_dprintf(ADP_L2, "get in MDrv_MFE_ISR\n");
    MsOS_ReleaseSemaphore(madp_sem_input);
}
#else
static MS_S32 mfe_irq_fd = -1;
#endif

void mfe_wait_int(MS_S32 isIsrMode)
{

    if(!isIsrMode)
        return;
#ifdef _MFE_UTOPIA_

    ms_dprintf(ADP_L2, "Wait MFE ISR...\n");
    if(MsOS_ObtainSemaphore(madp_sem_input, MSOS_WAIT_FOREVER) == FALSE) {
//    if(MsOS_ObtainSemaphore(madp_sem_input, 300) == FALSE) {
        ms_dprintk(ADP_L0, "Wait MFE ISR...fail!!\n");
        MsOS_DisableInterrupt((InterruptNum)E_INT_IRQ_MFE);
    }
#else
    if (mfe_irq_fd >= 0)
    {
        MS_U32 icount;
        read(mfe_irq_fd, &icount, 4);
    }
#endif
}

void mfe_open_int(void)
{
#ifdef _MFE_UTOPIA_
    if(madp_sem_input != -1)
        return;
    madp_sem_input = MsOS_CreateSemaphore(0,E_MSOS_FIFO ,"ADP_MFE_SEM") ;
    MsOS_AttachInterrupt((InterruptNum)E_INT_IRQ_MFE,MDrv_MFE_ISR);
#elif defined(WIN32)
#else
    if (mfe_irq_fd < 0)
    {
        mfe_irq_fd = open("/dev/int_mfe", O_RDWR | O_SYNC);
        if (mfe_irq_fd < 0)
        {
            ms_dprintf(ADP_L0,"ERR: MFE IRQ Open Failed\n");
            return;
        }

        ms_dprintf(ADP_L1,"NOTE: MFE IRQ Open OK\n");
    }
#endif
}

void mfe_close_int(void)
{
#ifdef _MFE_UTOPIA_
    if(madp_sem_input != -1) {
        MsOS_ReleaseSemaphore(madp_sem_input);
        MsOS_DeleteSemaphore(madp_sem_input);
        MsOS_DisableInterrupt((InterruptNum)E_INT_IRQ_MFE);
        MsOS_DetachInterrupt(E_INT_IRQ_MFE);
        madp_sem_input = -1;
    }
#elif defined(WIN32)
#else
    if (mfe_irq_fd >= 0)
    {
        close(mfe_irq_fd);
        mfe_irq_fd = -1;

        ms_dprintf(ADP_L1,"NOTE: MFE IRQ Close OK\n");
    }
#endif
}
