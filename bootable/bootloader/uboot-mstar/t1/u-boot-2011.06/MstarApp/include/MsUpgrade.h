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

#ifndef __Ms_UPGRADE_H__
#define __Ms_UPGRADE_H__

#include <MsTypes.h>
#include <ShareType.h>
#include <common.h>

#if(ENABLE_MODULE_NET==1)
int do_mstar (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#ifdef HB_CUS
int do_ustar_hb(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[]);
int do_check_usb_upgrade(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_auto_update (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#if(ENABLE_MODULE_USB == 1)
int do_ustar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_udstar (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
#if ((ENABLE_MODULE_NETUPDATE == 1)||(ENABLE_MODULE_OAD== 1))
int do_nstar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ostar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
#if CONFIG_RESCUE_ENV
int do_bstar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#if (ENABLE_MODULE_ENV_UPGRADE_FROM_BANK == 1)
int do_rstar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#if ((ENABLE_MODULE_OAD == 1) || (ENABLE_MODULE_USB == 1)||(ENABLE_MODULE_NETUPDATE == 1))
int do_mboot_upgrade(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

int read_dynamic_script_size(char *ScriptBuf);
#endif
