/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CMD_CUS_UPGRADE_H__
#define __CMD_CUS_UPGRADE_H__
#include <common.h>
#include <MsTypes.h>
#include <MsUboot.h>

int do_custar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_costar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_cnstar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ota_zip_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_usb_bin_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_wipe_datacache_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_systemrestore(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ursa7_upgrade(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ursa7_reset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

