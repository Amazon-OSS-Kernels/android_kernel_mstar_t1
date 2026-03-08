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

#ifndef __MS_MMAP_H__
#define __MS_MMAP_H__
#include <ShareType.h>

typedef struct
{
    U8     u8Name[32];
    U32    u32gID;
    U32    u32Addr;
    U32    u32Size;
    U8     u8Layer;
    U32    u32Align;
    U32    u32miuID;
    U32    u32CMA_HID;
}MMapInfo_s;

int do_get_mmap(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int set_mmap2env(char *id, U32 *addr, U32 *len);
int setMiu2env(char *miuID);

//-------------------------------------------------------------------------------------------------
/// get address from supernova's mmap
/// @param id                                  \b IN: id.
/// @param addr                               \b OUT: The address of the id
/// @return int                                  \b OUT: 0:success, -1:fail
/// example:
/// #define E_MMAP_ID_COPROCESSOR_AVAILABLE                        0x0000000000
/// E_MMAP_ID_COPROCESSOR_AVAILABLE is defined in supernova's mmap.
//   If we want to get the address of this item, we have input id "E_MMAP_ID_COPROCESSOR"
///  get_addr_from_mmap("E_MMAP_ID_COPROCESSOR", &addr);
//-------------------------------------------------------------------------------------------------
int get_addr_from_mmap(char *id, U32 *addr);

//-------------------------------------------------------------------------------------------------
/// get length from supernova's mmap
/// @param id                                  \b IN: id.
/// @param len                               \b OUT: The length of the id
/// @return int                                  \b OUT: 0:success, -1:fail
/// example:
/// #define E_MMAP_ID_COPROCESSOR_LEN                              0x0000200000
/// E_MMAP_ID_COPROCESSOR_LEN is defined in supernova's mmap.
//   If we want to get the address of this item, we have input id "E_MMAP_ID_COPROCESSOR"
///  get_length_from_mmap("E_MMAP_ID_COPROCESSOR", &len);
//-------------------------------------------------------------------------------------------------
int get_length_from_mmap(char *id, U32 *len);

//-------------------------------------------------------------------------------------------------
/// get miu interval from SN's mmap
/// @param interval                                  \b OUT: The length of the interval
/// @return int                                  \b OUT: 0:success, -1:fail
//-------------------------------------------------------------------------------------------------
int get_miu_interval(U32 *interval);

//-------------------------------------------------------------------------------------------------
/// get dram length from SN's mmap
/// @param length                                  \b OUT: The length of the dram
/// @return int                                  \b OUT: 0:success, -1:fail
//-------------------------------------------------------------------------------------------------
int get_dram_length(U32 *length);
unsigned int get_specific_dram_length(int miu_number);


int save_addr_to_env(char *id, U32 addr);

int del_addr_from_env(char *id);

int save_len_to_env(char *id, U32 len);

int del_len_from_env(char *id);

int get_mmap(char *id, MMapInfo_s *mmapInfo);

void setCMA2bootargs(void);

int isDetectMmap(void);


#endif
