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
#ifndef __ARM_BITOPS_H
#define __ARM_BITOPS_H

#include "MsTypes.h"
//extern int _test_and_clear_bit_le(U32 bit, U32 *p);
extern int _test_and_clear_bit_be(U32 bit, U32 *p);
//extern int _test_and_set_bit_le(U32 bit, U32 *p);
extern int _test_and_set_bit_be(U32 bit, U32 *p);

/*
 * ffs: find first bit set. This is defined the same way as
 * the libc and compiler builtin ffs routines, therefore
 * differs in spirit from the above ffz (man ffs).
 */

static __inline__ int generic_ffs(int x)
{
  int r = 1;

  if (!x)
    return 0;
  if (!(x & 0xffff)) {
	  x >>= 16;
    r += 16;
  }
  if (!(x & 0xff)) {
    x >>= 8;
    r += 8;
  }
  if (!(x & 0xf)) {
    x >>= 4;
    r += 4;
  }
  if (!(x & 3)) {
    x >>= 2;
    r += 2;
  }
  if (!(x & 1)) {
    x >>= 1;
    r += 1;
  }
  return r;
}

/*
 * This routine doesn't need to be atomic.
 */

static __inline__ int __test_bit(int nr, const U32 * p)
{
  return (p[nr >> 5] >> (nr & 31)) & 1UL;
}


/*
 * ffs: find first bit set. This is defined the same way as
 * the libc and compiler builtin ffs routines, therefore
 * differs in spirit from the above ffz (man ffs).
 */

#define ffs(x) generic_ffs(x)
#define test_bit(nr,p)          __test_bit(nr,p)
#define clear_bit(nr,addr,type) *(addr) = (volatile type )( (~( (type)1 << (type)nr)) & (type)(*((volatile type *)(addr))))
#define set_bit(nr,addr,type)   *(addr) = (volatile type )( ( ( (type)1 << (type)nr)) | (type)(*((volatile type *)(addr))))


static __inline__ int _test_and_clear_bit_le(U32 nr, U32 *p)
{
	int oldVal = (*p >> (nr & 31)) & 1UL;

	clear_bit(nr, p, U32);

	return oldVal;
}

static __inline__ int _test_and_set_bit_le(U32 nr, U32 *p)
{
	int oldVal = (*p >> (nr & 31)) & 1UL;

	set_bit(nr, p, U32);

	return oldVal;
}


#ifndef __ARMEB__
 // These are the little endian, atomic definitions.
#define test_and_set_bit(nr,p)    _test_and_set_bit_le(nr,p)
#define test_and_clear_bit(nr,p)  _test_and_clear_bit_le(nr,p)
extern int find_next_zero_bit_le (void *p, int size, int offset);
#define find_next_zero_bit(p,size,offset)  find_next_zero_bit_le(p,size,offset)
#else
 // These are the big endian, atomic definitions.
#define test_and_set_bit(nr,p)    _test_and_set_bit_be(nr,p)
#define test_and_clear_bit(nr,p)  _test_and_clear_bit_be(nr,p)
#endif


#endif /* _ARM_BITOPS_H */
