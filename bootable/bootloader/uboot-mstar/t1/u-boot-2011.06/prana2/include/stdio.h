/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#ifndef __CHAKRA_STDIO_H__
#define __CHAKRA_STDIO_H__

/* in case of non-OS include stdio.h, remove lint error 49 */
/*lint -e49 */
#include <stdarg.h>
//#include <stdlib.h>

#if defined (__arm__) || defined (__aarch64__)
//******************************************************************************
//******************************************************************************
#include <stddef.h>
#endif

#define PRINTF_CHAR_MAX     256

#if defined (__mips__) ||  defined (__aeon__)
#if defined(CROSSCOMPILE_MIPS_LINUX_GUN) && ( CROSSCOMPILE_MIPS_LINUX_GUN == 1)
typedef unsigned int   size_t;
#else
typedef unsigned long   size_t;
#endif

#endif

#ifdef __cplusplus
extern "C" {
#endif

int vsnprintf(char *str, size_t size, const char *format, va_list ap);
int vsprintf(char *str, const char *format, va_list ap) __attribute__ ((deprecated));
int snprintf(char *str, size_t size, const char *format, ...);
int sprintf(char *str, const char *format, ...) __attribute__ ((deprecated));
int printf(const char *format, ...);
int printk(const char *format, ...);
int putchar(int c);
int puts(const char *s);

int getchar(void);

/* extension */
int kbhit(void);
unsigned int int_sqrt(unsigned int n);
void dump_backtrace(void);
void setconsole(int device_handle);

#ifdef __cplusplus
}
#endif

#endif /* __CHAKRA_STDIO_H__ */
