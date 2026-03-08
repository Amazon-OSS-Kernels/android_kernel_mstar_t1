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

#ifndef _MS_API_URSA_
#define _MS_API_URSA_

int do_send_I2C_cmd_to_ursa6(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_send_I2C_cmd_to_ursa9(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_send_I2C_cmd_unlock_transfer_board(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_send_I2C_cmd_to_ursa(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif //_MS_API_URSA_

