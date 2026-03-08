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

#ifndef _Mstar_msTest_Header_
#define _Mstar_msTest_Header_

#ifdef CONFIG_DISPLAY_VERSIONINFO
#include <version.h>
int mversion (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
int do_msg (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_gettime(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#if defined(CONFIG_SECURITY_BOOT) && defined(CONFIG_SECURE_HW_IP)
int do_secure_driver_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
int do_cmdperformancetest(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

#endif