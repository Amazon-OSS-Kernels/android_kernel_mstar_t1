/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CMD_MS_CUS_SYSTEM__
#define __CMD_MS_CUS_SYSTEM__


#define BOOT_STATUS_CUSTOMER_ACTIONS_STR    "customer-actions\0"
#define BOOT_STATUS_ACTION_RELOADENV_BIT    (char)(0x01 << 0)
#define BOOT_STATUS_ACTION_RELOADPANEL_BIT  (char)(0x01 << 1)

int do_check_str_resume(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_setmbootver (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_envload (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_loadenv(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_setFRC(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ursa_lvds_on (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ir_delay(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ursa_lvds_off(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ursa_2k_mode_on (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ursa_osd_unmute (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_inx_panel_set_init (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_inx_panel_set_fhd (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_inx_panel_set_4k2k (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_inx_nova_set_4k2k_2division (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ursa8_lvds_on (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_ursa8_set_osd_mode(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_panel_inx_vb1_init (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int Set_4K2K_OP0(void);
int do_ursa_set_osd_mode(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#if (CONFIG_URSA12_VB1_FRC_BRINGUP)
int do_frc_bringup(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_frc_send(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_frc_recv(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#endif

