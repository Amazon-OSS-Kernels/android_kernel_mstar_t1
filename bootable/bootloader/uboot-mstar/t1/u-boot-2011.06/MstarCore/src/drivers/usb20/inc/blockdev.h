/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of
* the GNU General Public License as published by the Free Software Foundation;
* either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program;
* if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
* MA 02111-1307, USA.
*/

#ifndef _BLOCK_DEV_H
#define _BLOCK_DEV_H

typedef ulong lbaint_t;

typedef struct block_dev_desc {
	int		if_type;
	int		dev;
	unsigned char	part_type;
	unsigned char	target;
	unsigned char	lun;
	unsigned char	type;
	unsigned char	removable;
	lbaint_t		lba;
	unsigned long	blksz;
	char		vendor [40+1];
	char		product[20+1];
	char		revision[8+1];
	unsigned long	(*block_read)(int dev,
				      unsigned long start,
				      lbaint_t blkcnt,
				      void *buffer);
	unsigned long	(*block_write)(int dev,
				       unsigned long start,
				       lbaint_t blkcnt,
				       const void *buffer);
	void		*priv;
}block_dev_desc_t;

#define IF_TYPE_USB		4 // for USB device

/* Part types */
#define PART_TYPE_UNKNOWN	0x00 // unknown OS

/* Device types */
#define DEV_TYPE_UNKNOWN	0xff // unknown device

#define MS_BLOCK_DEV_DESC block_dev_desc_t
MS_BLOCK_DEV_DESC* usb_stor_get_dev(int dev);

#endif	/* _BLOCK_DEV_H */
