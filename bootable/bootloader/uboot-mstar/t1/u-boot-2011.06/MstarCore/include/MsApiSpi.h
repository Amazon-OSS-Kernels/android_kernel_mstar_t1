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

#ifndef _MSAPISPI_H_
#define _MSAPISPI_H_
#include <drvSERFLASH.h>
//-------------------------------------------------------------------------------------------------
/// get serial flash basic information
/// @param info                             \b IN: input buffer
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int getSpiInfo(SERFLASH_Info *info);

//-------------------------------------------------------------------------------------------------
/// get spi size
/// @SpiSize                                 \b IN: SpiSize point
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int getSpiSize(unsigned int *u32SpiSize);

//-------------------------------------------------------------------------------------------------
/// add a new partition in SPI
/// @name                                    \b IN: The name of partition
/// @size                                    \b IN: The size of partition
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int add_partition(char *name, unsigned size);

//-------------------------------------------------------------------------------------------------
/// delete a partition that is in SPI
/// @name                                    \b IN: The name of partition
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int del_partition(char *name);

//-------------------------------------------------------------------------------------------------
/// delete all partitions that are all in SPI
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int del_all_partitions(void);

//-------------------------------------------------------------------------------------------------
/// list all partition info on console
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int list_partition(void);

//-------------------------------------------------------------------------------------------------
/// get the partition information
/// @name                                    \b IN: The name of partition
/// @offset                                    \b OUT: The offset of partition
/// @size                                      \b OUT: The size of partition
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int get_spi_partition_info(char *name, unsigned int *offset, unsigned int *size);

//-------------------------------------------------------------------------------------------------
/// init spi paritiotn layout function
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int init_spi_partition(void);

//-------------------------------------------------------------------------------------------------
/// add mboot partition
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int add_mboot_partition(void);





#endif  // _MSAPISPI_H_

