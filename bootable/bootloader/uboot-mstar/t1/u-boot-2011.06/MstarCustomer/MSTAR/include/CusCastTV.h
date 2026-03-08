/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CMD_MS_CUS_CASTTV__
#define __CMD_MS_CUS_CASTTV__

#include <MsTypes.h>
int  get_spinner_bin(void);
int do_png_decode (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_Timer_test(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
MS_U32 Spinner_Timer_ISR_Register(void);
int decode_all_png(MS_U8 u8MaxPngCount);
void gop_init_spinner(MS_U32 XPos, MS_U32 YPos);
void stop_Spinner_Timer_ISR(void);
int do_show_spinner(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif