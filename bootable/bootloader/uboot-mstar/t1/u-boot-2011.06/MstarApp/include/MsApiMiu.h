/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
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

#ifndef _MS_API_MIU_
#define _MS_API_MIU_
#include <MsTypes.h>

int Set_DRAM_Size(void);
int MsApi_kernelProtect(void);
int do_MsApi_kernelProtect( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
void MsApi_RunTimePmProtect(unsigned int u32StartAddr, unsigned int u32EndAddr);
int do_MsApi_RunTimePmProtect( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

void MsApi_kernelProtectBist(void);
int do_MsApi_kernelProtectBist( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

void MsApi_NuttxProtect(MS_U32 u32StartAddr, MS_U32 u32EndAddr);
int do_MsApi_NuttxProtect(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

int MsApi_MiuProtect(void);
int do_MsApi_MiuProtect( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_set_miu(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

#endif

