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
#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <datatype.h>
#include <mhal_arch.h>
#include <ptrace.h>
#include <arch_timer.h>

typedef    unsigned long    u64;

/*
 * This struct defines the way the registers are stored on the stack during an
 * exception. Note that sizeof(struct pt_regs) has to be a multiple of 16 (for
 * stack alignment). struct user_pt_regs must form a prefix of struct pt_regs.
 */
struct pt_regs {
    u64 regs[31];
    u64 sp;
    u64 pc;
    u64 pstate;
};

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
    unsigned long temp;

    asm volatile("msr DAIFSet, #2\n"
                 : "=r" (temp)
                 :
                 : "memory");
    return 0;
}

void mhal_interrupt_enable(void)
{
    unsigned long temp;

    asm volatile("msr DAIFClr, #2\n"
                 : "=r" (temp)
                 :
                 : "memory");
    return ;
}

void mhal_interrupt_restore(int old)
{
    return ;
}

void mhal_interrupt_mask_all(void)
{
    mhal_interrupt_disable();
}

void mhal_interrupt_mask(MHAL_INTERRUPT_TYPE intr_num)
{
    return ;
}

void mhal_interrupt_unmask(MHAL_INTERRUPT_TYPE intr_num)
{
    return ;
}

void mhal_interrupt_lock(void)
{
    return ;
}

void mhal_interrupt_unlock(void)
{
    return ;
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
    int i;

    printf("pc : [<%016llx>] lr : [<%016llx>] pstate: %08llx\n",
           regs->pc, regs->regs[30], regs->pstate);
    printf("sp : %016llx\n", regs->sp);
    for (i = 29; i >= 0; i--) {
    	printf("x%-2d: %016llx ", i, regs->regs[i]);
    	if (i % 2 == 0)
    		printf("\n");
    }
    printf("\n");
}

void do_el1_sync_invalid (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nel1_sync_invalid.....\n");
}

void do_el1_irq_invalid (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nel1_irq_invalid.....\n");
}

void do_el1_fiq_invalid (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nel1_fiq_invalid.....\n");
}

void do_el1_error_invalid  (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nel1_error_invalid.....\n");
}

void do_el1_sync  (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nel1_sync.....\n");
}

void do_el0_sync (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nel0_sync.....\n");
}

void do_el0_irq (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nel0_irq.....\n");
}

void do_el0_fiq_invalid (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nel0_fiq_invalid.....\n");
}

void do_el0_error_invalid (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nel0_error_invalid.....\n");
}

void do_el0_sync_invalid (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nel0_sync_invalid.....\n");
}

void do_el0_irq_invalid (struct pt_regs *pt_regs)
{
    show_regs (pt_regs);
    printf ("\nel0_irq_invalid.....\n");
}

void do_irq (void)
{
    unsigned long picvec;
    mhal_isr_t    isr;

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
    unsigned long ctrl;
    unsigned long cntvct;    //virtual time count
    unsigned long cntv_cval; //virtual time compare value
    unsigned long picvec;
    mhal_isr_t    isr;

    ctrl = arch_timer_reg_read(ARCH_TIMER_VIRT_ACCESS, ARCH_TIMER_REG_CTRL);

    if (ctrl & ARCH_TIMER_CTRL_IT_STAT) 
    {
        ctrl |= ARCH_TIMER_CTRL_IT_MASK;
        arch_timer_reg_write(ARCH_TIMER_VIRT_ACCESS, ARCH_TIMER_REG_CTRL, ctrl);
        
        picvec = E_INTERRUPT_TICK_TIMER;

        isr = intr_table[picvec].isr;
        if (isr)
        {
            isr();
        }
    }

    /*Generic timer is 12MHZ, 0x3000 means 1ms*/
    cntvct = arch_timer_reg_read(ARCH_TIMER_VIRT_ACCESS, ARCH_TIMER_REG_CT);
    cntv_cval = cntvct + 0x3000;
    arch_timer_reg_write(ARCH_TIMER_VIRT_ACCESS, ARCH_TIMER_REG_CVAL, cntv_cval);
   
    /* Enable virtual timer and unmask interrupt*/
    ctrl = arch_timer_reg_read(ARCH_TIMER_VIRT_ACCESS, ARCH_TIMER_REG_CTRL) ;
    ctrl &= (~ARCH_TIMER_CTRL_IT_MASK);
    arch_timer_reg_write(ARCH_TIMER_VIRT_ACCESS, ARCH_TIMER_REG_CTRL, ctrl);
}

void mhal_fiq_merge (void)
{
    unsigned int    dwRegisterData = 0;

    dwRegisterData = *(volatile unsigned int*)(RIUBASE + (0x123964 << 1));
    dwRegisterData &= 0xFFDF;
    dwRegisterData |= 0x0050;
    *(volatile unsigned int*)(RIUBASE + (0x123964 << 1)) = dwRegisterData;
}
