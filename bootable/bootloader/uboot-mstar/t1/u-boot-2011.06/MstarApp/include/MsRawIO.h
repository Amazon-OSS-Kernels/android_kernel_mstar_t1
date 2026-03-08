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

#ifndef __MSRAWIO_H__
#define __MSRAWIO_H__
#include <CusRawIO.h>
typedef enum{
    E_RAW_DATA_IN_SPI=0,
    E_RAW_DATA_IN_NAND,
    E_RAW_DATA_IN_MMC,
    E_RAW_DATA_INVALED,
    E_RAW_DATA_MAX=E_RAW_DATA_INVALED
}EN_RAW_DATA_TARGET;

int do_raw_io_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_raw_io_config(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_raw_io_status(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_raw_io_config_push(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_raw_io_config_pop(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_raw_read(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_raw_write(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int check_ubi_partition(char * volName,char *partitionName);


//-------------------------------------------------------------------------------------------------
/// Config the taret for raw data read/write.
/// @param  target                                  \b IN: where the raw data is.
/// @param  partition                                  \b IN: which partition, if the taget device doesn't have partition, please input 'UNNECESSARY_PARTITION'
/// @param  volume                                  \b IN: which volume, if the taget device doesn't have volume, please input 'UNNECESSARY_PARTITION'
/// @return  int                                       \b OUT: 0:success, -1: fail
//-------------------------------------------------------------------------------------------------
int raw_io_config(EN_RAW_DATA_TARGET target, char *partition, char *volume);

//-------------------------------------------------------------------------------------------------
/// Read raw data from the taget device
/// @param  addr                                     \b IN: the address of input buffer
/// @param  offset                                   \b IN: the offset in the target device
/// @param  len                                       \b IN: read len
/// @return  int                                       \b OUT: 0:success, -1: fail
//-------------------------------------------------------------------------------------------------
int raw_read(unsigned int addr, unsigned int offset, unsigned int len);

//-------------------------------------------------------------------------------------------------
/// write raw data from the taget device
/// @param  addr                                     \b IN: the address of input buffer
/// @param  offset                                   \b IN: the offset in the target device
/// @param  len                                       \b IN: write len
/// @return  int                                       \b OUT: 0:success, -1: fail
//-------------------------------------------------------------------------------------------------
int raw_write(unsigned int addr, unsigned int offset, unsigned int len);
//-------------------------------------------------------------------------------------------------
/// get raw config status
/// @return  EN_RAW_DATA_TARGET                        \b OUT: raw config status
//-------------------------------------------------------------------------------------------------
EN_RAW_DATA_TARGET get_raw_status(void);

//-------------------------------------------------------------------------------------------------
/// raw_io push config setting to internal stack
//-------------------------------------------------------------------------------------------------
void raw_io_config_push(void);
//-------------------------------------------------------------------------------------------------
/// raw_io pop config setting to internal stack
//-------------------------------------------------------------------------------------------------
void raw_io_config_pop(void);

//-------------------------------------------------------------------------------------------------
/// Config the taret for raw data read/write for mboot.bin location
//-------------------------------------------------------------------------------------------------
int mboot_raw_io_Config(void);

#if defined (CONFIG_DOUBLE_MBOOT) && !defined (CONFIG_DUAL_SYSTEM)
//-------------------------------------------------------------------------------------------------
/// Config the taret for raw data read/write for mboot.bin location
//-------------------------------------------------------------------------------------------------
int mbootbak_raw_io_Config(void);
#endif

#endif //__MSRAWIO_H__

