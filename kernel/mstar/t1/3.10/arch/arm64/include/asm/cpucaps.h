/*
 * arch/arm64/include/asm/cpucaps.h
 *
 * Copyright (C) 2016 ARM Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __ASM_CPUCAPS_H
#define __ASM_CPUCAPS_H

#define ARM64_WORKAROUND_CLEAN_CACHE            0
#define ARM64_HAS_SYSREG_GIC_CPUIF              1
#define ARM64_HAS_PAN                           2
#define ARM64_HARDEN_BRANCH_PREDICTOR		3
#define ARM64_HAS_NO_HW_PREFETCH                4

#define ARM64_NCAPS				5

#endif /* __ASM_CPUCAPS_H */
