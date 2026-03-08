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
////////////////////////////////////////////////////////////////////////////////

#ifndef __PRANA_FILE_H__
#define __PRANA_FILE_H__

/// @file file.h
/// @brief file like interface for driver and file

#include "datatype.h"

struct file;

/// file operations for device and file
struct file_operations
{
    int     (*open) (struct file *f, int mode);
    int     (*close)(struct file *f);
    int     (*read) (device_t dev, char *buf, size_t len, int off);
    int     (*write)(device_t dev, const char *buf, size_t len, int off);
    int     (*poll) (device_t dev, int types);
    int     (*ioctl)(device_t dev, int request, unsigned long arg);
    int     (*event)(int evt);          // device PM event
};

/// file structure
struct file
{
    int                     magic;      ///< magic number
    int                     refcnt;     ///< reference count
    int                     flags;      ///< file characteristics
    struct file_operations *fops;       ///< file operations
};

MST_BEGIN_DECL

/// open file
int open(char *filename, int mode);

/// close file
int close(int fd);

int read(int fd, char *buf, size_t len);
int write(int fd, const char *buf, size_t len);
int ioctl(int fd, int request, unsigned long arg);

MST_END_DECL

#endif /* __PRANA_FILE_H__ */
