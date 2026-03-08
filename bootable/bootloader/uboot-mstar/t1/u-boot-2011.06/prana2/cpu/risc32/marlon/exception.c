/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//******************************************************************************
#include "datatype.h"
#include "mhal_arch.h"

static struct
{
    mhal_isr_t  isr;
    U32         data;
} expt_table[E_EXCEPTION_MAX + 1];

void mhal_exception_attach(MHAL_EXCEPTION_TYPE expt_num, mhal_isr_t isr, U32 data)
{
    int old;
    if ((U32)expt_num <= E_EXCEPTION_MAX)
    {
        __mhal_interrupt_disable(old);
        expt_table[expt_num].isr = isr;
        expt_table[expt_num].data = data;
        __mhal_interrupt_restore(old);
    }
}

void mhal_exception_detach(MHAL_EXCEPTION_TYPE expt_num)
{
    if ((U32)expt_num <= E_EXCEPTION_MAX)
    {
        expt_table[expt_num].isr = 0;
    }
}

void mhal_exception_handler(MHAL_SavedRegisters *regs, int vector)
{
    mhal_isr_t isr = 0;

    if ((U32)vector <= E_EXCEPTION_MAX)
    {
        isr = expt_table[vector].isr;
        if (isr)
        {
            isr(regs, expt_table[vector].data);
        }
    }

    if (!isr)
    {
        printf("Exception: %d\n", vector);

        printf("r0 : %08X  r1 : %08X  r2 : %08X  r3 : %08X\n", regs->r[0],  regs->r[1],  regs->r[2],  regs->r[3]);
        printf("r4 : %08X  r5 : %08X  r6 : %08X  r7 : %08X\n", regs->r[4],  regs->r[5],  regs->r[6],  regs->r[7]);
        printf("r8 : %08X  r9 : %08X  r10: %08X  r11: %08X\n", regs->r[8],  regs->r[9],  regs->r[10], regs->r[11]);
        printf("r12: %08X  r13: %08X  r14: %08X  r15: %08X\n", regs->r[12], regs->r[13], regs->r[14], regs->r[15]);
        printf("r16: %08X  r17: %08X  r18: %08X  r19: %08X\n", regs->r[16], regs->r[17], regs->r[18], regs->r[19]);
        printf("r20: %08X  r21: %08X  r22: %08X  r23: %08X\n", regs->r[20], regs->r[21], regs->r[22], regs->r[23]);
        printf("r24: %08X  r25: %08X  r26: %08X  r27: %08X\n", regs->r[24], regs->r[25], regs->r[26], regs->r[27]);
        printf("r28: %08X  r29: %08X  r30: %08X  r31: %08X\n", regs->r[28], regs->r[29], regs->r[30], regs->r[31]);

        printf("sr : %08X  pc : %08X  eear: %08X\n", regs->sr, regs->pc, regs->eear);

        dump_backtrace();
        while (1);
    }
}
