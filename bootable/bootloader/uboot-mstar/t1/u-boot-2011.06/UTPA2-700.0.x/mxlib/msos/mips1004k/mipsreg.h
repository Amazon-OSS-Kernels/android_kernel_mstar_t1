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
#ifndef _ASM_MIPSREG_H
#define _ASM_MIPSREG_H

/*
 * Coprocessor 0 register names
 */
#define CP0_INDEX $0
#define CP0_RANDOM $1
#define CP0_ENTRYLO0 $2
#define CP0_ENTRYLO1 $3
#define CP0_CONF $3
#define CP0_CONTEXT $4
#define CP0_PAGEMASK $5
#define CP0_WIRED $6
#define CP0_INFO $7
#define CP0_BADVADDR $8
#define CP0_COUNT $9
#define CP0_ENTRYHI $10

/*
 * Functions to access the R10000 performance counters.  These are basically
 * mfc0 and mtc0 instructions from and to coprocessor register with a 5-bit
 * performance counter number encoded into bits 1 ... 5 of the instruction.
 * Only performance counters 0 to 1 actually exist, so for a non-R10000 aware
 * disassembler these will look like an access to selection 0 or 1.
 */

/*
 * Macros to access the system control coprocessor
 */
#define __read_32bit_c0_register(src, selectionection)				\
({ int __res;								\
	if (selectionection == 0)							\
		__asm__ __volatile__(					\
			"mfc0\t%0, " #src "\n\t"			\
			: "=r" (__res));				\
	else								\
		__asm__ __volatile__(					\
			".set\tmips32\n\t"				\
			"mfc0\t%0, " #src ", " #selectionection "\n\t"		\
			".set\tmips0\n\t"				\
			: "=r" (__res));				\
	__res;								\
})


#define __write_32bit_c0_register(register, selection, val)			\
do {									\
	if (selection == 0)							\
		__asm__ __volatile__(					\
			"mtc0\t%z0, " #register "\n\t"			\
			: : "Jr" ((unsigned int)(val)));		\
	else								\
		__asm__ __volatile__(					\
			".set\tmips32\n\t"				\
			"mtc0\t%z0, " #register ", " #selection "\n\t"	\
			".set\tmips0"					\
			: : "Jr" ((unsigned int)(val)));		\
} while (0)


#define __read_ulong_c0_register(reg, selection)				\
	(unsigned long) __read_32bit_c0_register(reg, selection) ;		\

#define __write_ulong_c0_register(reg, selection, val)			\
do {									\
		__write_32bit_c0_register(reg, selection, val);		\
} while (0)


#define write_c0_by_index(value)	__write_32bit_c0_register($0, 0, value)
#define write_c0_entrylow0(value)	__write_ulong_c0_register($2, 0, value)
#define write_c0_entrylow1(value)	__write_ulong_c0_register($3, 0, value)
#define read_c0_by_pagemask()	__read_32bit_c0_register($5, 0)
#define write_c0_by_pagemask(value)	__write_32bit_c0_register($5, 0, value)
#define read_c0_with_wired()		__read_32bit_c0_register($6, 0)
#define write_c0_with_wired(value)	__write_32bit_c0_register($6, 0, value)
#define read_c0_entryhigh()	__read_ulong_c0_register($10, 0)
#define write_c0_entryhigh(value)	__write_ulong_c0_register($10, 0, value)


/*
 * TLB operations.
 *
 * It is responsibility of the caller to take care of any TLB hazards.
 */
static inline void tlb_index_write(void)
{
	__asm__ __volatile__(
		".set noreorder\n\t"
		"tlbwi\n\t"
		".set reorder");
}


#endif /* _ASM_MIPSREG_H */
