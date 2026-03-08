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
#include <stdio.h>
#include <assert.h>
#include "datatype.h"
#include "mhal_arch.h"
#include "ptrace.h"

struct interrupt
{
    mhal_isr_t  isr;
    U32         data;
};

static struct interrupt intr_table[E_INTERRUPT_MAX + 1];

static volatile int nr_intr_locks;
static volatile int saved_intr_state;

int mhal_interrupt_disable(void)
{
    unsigned long old,temp,value;

    asm volatile ("mrs %0, cpsr\n"
                  "mrs %2, cpsr\n"
                  "orr %1, %0, #0xc0\n"
                  "msr cpsr_c, %1"
                  : "=r" (old), "=r" (temp), "=r" (value)
                  :
                  : "memory");
    value = value & 0xc0;
    return (int)value;
}

void mhal_interrupt_enable(void)
{
    /* enable IRQ/FIQ interrupts */
    unsigned long temp;

    asm volatile("mrs %0, cpsr\n"
                 "bic %0, %0, #0x80\n"
                 "bic %0, %0, #0x40\n"
                 "msr cpsr_c, %0"
                 : "=r" (temp)
                 :
                 : "memory");
}

void mhal_interrupt_restore(int old)
{
    unsigned long temp,value;
    value = (unsigned long)old;

    asm volatile("mrs %0, cpsr\n"
                 "bic %0, %0, #0x80\n"
                 "bic %0, %0, #0x40\n"
                 "orr %0, %0, %1\n"
                 "msr cpsr_c, %0"
                 : "=r" (temp), "=r" (value)
                 :
                 : "memory");
}

void mhal_interrupt_mask_all(void)
{
    mhal_interrupt_disable();
}

void mhal_interrupt_mask(MHAL_INTERRUPT_TYPE intr_num)
{
    unsigned long temp,value;

    if(intr_num == E_INTERRUPT_FIQ)
        value = 0x40;
    else if(intr_num == E_INTERRUPT_IRQ)
        value = 0x80;
    else
        return;

    asm volatile("mrs %0, cpsr\n"
                 "bic %0, %0, #0x80\n"
                 "bic %0, %0, #0x40\n"
                 "orr %0, %0, %1\n"
                 "msr cpsr_c, %0"
                 : "=r" (temp), "=r" (value)
                 :
                 : "memory");
}

void mhal_interrupt_unmask(MHAL_INTERRUPT_TYPE intr_num)
{
    unsigned long temp;

    if(intr_num == E_INTERRUPT_FIQ)
    {
         asm volatile("mrs %0, cpsr\n"
                      "bic %0, %0, #0x40\n"
                      "msr cpsr_c, %0"
                      : "=r" (temp)
                      :
                      : "memory");
    }
    else if(intr_num == E_INTERRUPT_IRQ)
    {
         asm volatile("mrs %0, cpsr\n"
                      "bic %0, %0, #0x80\n"
                      "msr cpsr_c, %0"
                      : "=r" (temp)
                      :
                      : "memory");
    }
    else
        return;
}

void mhal_interrupt_lock(void)
{
    int spr_sr;

    spr_sr = mhal_interrupt_disable();
    if (++nr_intr_locks == 1)
    {
        saved_intr_state = spr_sr;
    }

    ASSERT(nr_intr_locks > 0);
}

void mhal_interrupt_unlock(void)
{
    ASSERT(nr_intr_locks > 0);

    if (--nr_intr_locks == 0)
    {
       mhal_interrupt_restore(saved_intr_state);
    }
}

void mhal_interrupt_attach(MHAL_INTERRUPT_TYPE intr_num, mhal_isr_t isr, U32 dat)
{
    if (intr_num <= E_INTERRUPT_MAX)
    {
        intr_table[intr_num].isr = isr;
        intr_table[intr_num].data = dat;
        mhal_interrupt_unmask(intr_num);
    }
}

void mhal_interrupt_detach(MHAL_INTERRUPT_TYPE intr_num)
{
    if (intr_num <= E_INTERRUPT_MAX)
    {
        intr_table[intr_num].isr = 0;
        mhal_interrupt_mask(intr_num);
    }
}

void show_regs (struct pt_regs *regs)
{
    unsigned long flags;
    const char *processor_modes[] = {
    "USER_26",  "FIQ_26",   "IRQ_26",   "SVC_26",
    "UK4_26",   "UK5_26",   "UK6_26",   "UK7_26",
    "UK8_26",   "UK9_26",   "UK10_26",  "UK11_26",
    "UK12_26",  "UK13_26",  "UK14_26",  "UK15_26",
    "USER_32",  "FIQ_32",   "IRQ_32",   "SVC_32",
    "UK4_32",   "UK5_32",   "UK6_32",   "ABT_32",
    "UK8_32",   "UK9_32",   "UK10_32",  "UND_32",
    "UK12_32",  "UK13_32",  "UK14_32",  "SYS_32",
    };

    flags = condition_codes (regs);

    printf ("\npc : [<%08lx>]      lr : [<%08lx>]\n"
        "sp : %08lx  ip : %08lx  fp : %08lx\n",
        instruction_pointer (regs),
        regs->ARM_lr, regs->ARM_sp, regs->ARM_ip, regs->ARM_fp);
    printf ("r10: %08lx  r9 : %08lx  r8 : %08lx\n",
        regs->ARM_r10, regs->ARM_r9, regs->ARM_r8);
    printf ("r7 : %08lx  r6 : %08lx  r5 : %08lx  r4 : %08lx\n",
        regs->ARM_r7, regs->ARM_r6, regs->ARM_r5, regs->ARM_r4);
    printf ("r3 : %08lx  r2 : %08lx  r1 : %08lx  r0 : %08lx\n",
        regs->ARM_r3, regs->ARM_r2, regs->ARM_r1, regs->ARM_r0);
    printf ("Flags: %c%c%c%c",
        flags & CC_N_BIT ? 'N' : 'n',
        flags & CC_Z_BIT ? 'Z' : 'z',
        flags & CC_C_BIT ? 'C' : 'c', flags & CC_V_BIT ? 'V' : 'v');
    printf ("  IRQs %s  FIQs %s  Mode %s%s\n",
        interrupts_enabled (regs) ? "on" : "off",
        fast_interrupts_enabled (regs) ? "on" : "off",
        processor_modes[processor_mode (regs)],
        thumb_mode (regs) ? " (T)" : "");
}

void do_undefined_instruction (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nUndefined instruction......\n");
}

void do_software_interrupt (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nSoftware interrupt.....\n");
}

void do_prefetch_abort (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nPrefetch abort.....\n");
}

void do_data_abort (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nData abort.....\n");
}

void do_not_used (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nNot used.....\n");
}

void do_fiq (void)
{
    unsigned long picvec;
    mhal_isr_t    isr;

    picvec = E_INTERRUPT_FIQ;

    isr = intr_table[picvec].isr;
    if (isr)
    {
       isr();
    }
}

void do_irq (void)
{
    unsigned long picvec;
    mhal_isr_t    isr;

    picvec = E_INTERRUPT_IRQ;

    isr = intr_table[picvec].isr;
    if (isr)
    {
        isr();
    }

    //Merge Mstar FIQ handler into IRQ
    picvec = E_INTERRUPT_FIQ;

    isr = intr_table[picvec].isr;
    if (isr)
    {
       isr();
    }
}

void timer_interrupt_handler (void)
{
    unsigned long   picvec;
    mhal_isr_t      isr;
    unsigned int    dwRegisterValue = 0;

    asm volatile("mrc p15, 0, %0, c14, c2, 1" : "=r" (dwRegisterValue));
    if((dwRegisterValue & 0x04) != 0)
    {
        //Disable Physical Timer
        asm volatile("mcr p15, 0, %0, c14, c2, 1" : : "r" (0));

        picvec = E_INTERRUPT_TICK_TIMER;

        isr = intr_table[picvec].isr;
        if (isr)
        {
            isr();
        }

        //Reload Initial Value of Physical Timer
        asm volatile("mcr p15, 0, %0, c14, c2, 0" : : "r" (TIMER_CLOCK_FREQ/ 1000));

        //Enable Physical Timer
        asm volatile("mcr p15, 0, %0, c14, c2, 1" : : "r" (1));
    }
}

void mhal_fiq_merge (void)
{
    unsigned int    dwRegisterData = 0;

    dwRegisterData = *(volatile unsigned int*)(RIUBASE + (0x123964 << 1));
    dwRegisterData &= 0xFFDF;
    dwRegisterData |= 0x0050;
    *(volatile unsigned int*)(RIUBASE + (0x123964 << 1)) = dwRegisterData;
}
