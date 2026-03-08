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
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "core/device.h"

static device_t console;

void setconsole(int device_handle)
{
    console = (device_t) device_handle;
}

int __putchar(int c)
{
    char x;

    if (console == DEVICE_NULL)
    {
        return -1;
    }
    if (c == '\n')
    {
        x = '\r';
        device_write(console, &x, 1);
    }
    x = c;
    device_write(console, &x, 1);
    return c;
}

int __kbhit(void)
{
    if (console != DEVICE_NULL)
    {
        return device_poll(console, POLLIN);
    }
    return 0;
}

int __getchar(void)
{
    char x;

    if ((console != DEVICE_NULL) && (device_read(console, &x, 1) == 1))
    {
        return (int) (unsigned char) x;
    }
    return -1;
}

int putchar(int)   __attribute__ ((weak, alias("__putchar")));
int kbhit(void)    __attribute__ ((weak, alias("__kbhit")));
int getchar(void)  __attribute__ ((weak, alias("__getchar")));
