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

#ifdef __AEONR2__
    #define NON_PM_RIU_BASE  0xA0200000
    #define CPU_CLOCK_REG    0xB25
    #define AEON_CLOCK_FREQ  ((RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG) == 0x28)? 216000000 : \
                              (RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG) == 0x2C)? 192000000 : \
                              (RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG) == 0x34)? 160000000 : 12000000)
#else
    #define NON_PM_RIU_BASE  0xA0200000
    #define CPU_CLOCK_REG    0xB24
    #define AEON_CLOCK_FREQ  ((RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG) == 0x80)? 216000000 : \
                              (RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG) == 0x84)? 192000000 : \
                              (RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG) == 0x88)? 172800000 : \
                              (RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG) == 0x8C)? 160000000 : \
                              (RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG) == 0x90)? 144000000 : \
                              (RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG) == 0x94)? 108000000 : 12000000)
#endif

struct interrupt
{
    mhal_isr_t  isr;
    U32         data;
};

static void __mhal_default_isr(MHAL_SavedRegisters *regs, U32 vector)
{
    printf("\nmhal_default_isr vector: %d regs: %p\n", vector, regs);
    dump_backtrace();
    while(1);
}

void mhal_default_isr(MHAL_SavedRegisters *regs, U32 vector) __attribute__ ((weak, alias("__mhal_default_isr")));

static struct interrupt intr_table[E_INTERRUPT_MAX + 1];

static volatile int nr_intr_locks;
static volatile int saved_intr_state;

int  mhal_interrupt_disable(void)
{
    int spr_sr;
    __mhal_interrupt_disable(spr_sr);
    return spr_sr;
}

void mhal_interrupt_enable(void)
{
    __mhal_interrupt_enable();
}

void mhal_interrupt_restore(int old)
{
    __mhal_interrupt_restore(old);
}

void mhal_interrupt_mask_all(void)
{
    mtspr(SPR_PICMR, 0x00000000);
}

void mhal_interrupt_mask(MHAL_INTERRUPT_TYPE intr_num)
{
    __mhal_interrupt_mask(intr_num);
}

void mhal_interrupt_unmask(MHAL_INTERRUPT_TYPE intr_num)
{
    __mhal_interrupt_unmask(intr_num);
}

void mhal_interrupt_lock(void)
{
    int spr_sr;

    __mhal_interrupt_disable(spr_sr);
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
        __mhal_interrupt_restore(saved_intr_state);
    }
}

void mhal_interrupt_attach(MHAL_INTERRUPT_TYPE intr_num, mhal_isr_t isr, U32 dat)
{
    if (intr_num <= E_INTERRUPT_MAX)
    {
        intr_table[intr_num].isr = isr;
        intr_table[intr_num].data = dat;
    }
}

void mhal_interrupt_detach(MHAL_INTERRUPT_TYPE intr_num)
{
    if (intr_num <= E_INTERRUPT_MAX)
    {
        intr_table[intr_num].isr = 0;
    }
}

#define ISR_TIMER0_PERIOD_US        1000UL  // : 1000us, unit:us, Timer 0 Interrupt period

void mhal_interrupt_handler(MHAL_SavedRegisters *regs, int vector)
{
    unsigned long spr;
    unsigned long picvec;
    mhal_isr_t    isr;

    UNUSED(vector);

    spr = mfspr(SPR_PICSR);
    picvec = __mhal_lsbit_index(spr);
    isr = intr_table[picvec].isr;
    if (isr)
    {
       isr(regs, intr_table[picvec].data);
    }
    else
    {
       mhal_default_isr(regs, picvec);
    }
    if (picvec)
    {
        picvec--;
        spr = mfspr(SPR_PICMR);
        mtspr(SPR_PICMR, spr & ~(1 << picvec));
        mtspr(SPR_PICSR, mfspr(SPR_PICSR) & ~(1 << picvec));
        mtspr(SPR_PICMR, spr);
    }

    if(picvec==0)
    {
        mtspr(SPR_TTMR, 0x60000000 | (AEON_CLOCK_FREQ / ISR_TIMER0_PERIOD_US));
    }
}

void mhal_interrupt_dumpvector()
{
    unsigned int i;
    for(i=0;i< E_INTERRUPT_MAX;i++)
    {
        printf("vector %d addr:%lx data:%lx\n",i,intr_table[i].isr,intr_table[i].data);
    }
}

