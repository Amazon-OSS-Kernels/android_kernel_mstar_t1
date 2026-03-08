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

#include "device.h"

#define DEVICE(dev) ((struct device *)(dev))

int device_read(device_t dev, char *buf, size_t len)
{
    struct device *d = DEVICE(dev);

    if (d->devio->read)
    {
        return d->devio->read(dev, buf, len, 0);
    }
    return -1;
}

int device_write(device_t dev, const char *buf, size_t len)
{
    struct device *d = DEVICE(dev);

    if (d->devio->write)
    {
        return d->devio->write(dev, buf, len, 0);
    }
    return -1;
}

int device_poll(device_t dev, int types)
{
    struct device *d = DEVICE(dev);

    if (d->devio->poll)
    {
        return d->devio->poll(dev, types);
    }
    return 0;
}
