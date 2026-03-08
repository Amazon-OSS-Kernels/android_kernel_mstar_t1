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

#ifndef __PRANA_DEVICE_H__
#define __PRANA_DEVICE_H__

#include "datatype.h"

#define DEVICE_NULL ((device_t)0)

typedef unsigned long device_t;

//
// device PM event
//
typedef enum
{
    E_EVT_SUSPEND   = 1,
    E_EVT_SHUTDOWN  = 2,
} eDEVEVT;

//
// poll types
//
#define POLLIN      0x01
#define POLLOUT     0x02
#define POLLHUP     0x04

//
// device I/O operations
//
struct devio
{
    int     (*open) (device_t dev, int mode);
    int     (*close)(device_t dev);
    int     (*read) (device_t dev, char *buf, size_t len, int off);
    int     (*write)(device_t dev, const char *buf, size_t len, int off);
    int     (*poll) (device_t dev, int types);
    int     (*ioctl)(device_t dev, int request, unsigned long arg);
    int     (*event)(eDEVEVT evt);
};


#define DEVICE_MAGIC    (('D' << 24) | ('E' << 16) | ('V' << 8) | 'I')

//
// device structure
//
struct device
{
    int             magic;              // magic number
    int             refcnt;             // reference count
    int             flags;              // device characteristics
    struct devio    *devio;             // device i/o table
};

#define device_valid(dev) ((dev)->magic == DEVICE_MAGIC)

MST_BEGIN_DECL

int device_read(device_t dev, char *buf, size_t len);
int device_write(device_t dev, const char *buf, size_t len);
int device_poll(device_t dev, int types);

MST_END_DECL

#endif /* __PRANA_DEVICE_H__ */
