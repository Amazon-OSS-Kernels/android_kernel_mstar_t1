/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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
#include <common.h>
#include <command.h>
#include <MsApiUrsa.h>

#if defined(CONFIG_URSA6_VB1) || defined(CONFIG_URSA_UNION)
#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    send_I2C_cmd_to_ursa6, CONFIG_SYS_MAXARGS, 1,    do_send_I2C_cmd_to_ursa6,
    "send_I2C_cmd_to_ursa6   -  send I2C command to URSA6\n",
    NULL
);
#endif

U_BOOT_CMD(
    unlockcmi, CONFIG_SYS_MAXARGS, 1,    do_send_I2C_cmd_unlock_transfer_board,
    "unlock cmi transfer board   -  unlock\n",
    NULL
);

#if defined(CONFIG_URSA_10)
U_BOOT_CMD(
    send_I2C_cmd_to_ursa10, CONFIG_SYS_MAXARGS, 1,    do_send_I2C_cmd_to_ursa10,
    "send_I2C_cmd_to_ursa10   -  send_I2C_cmd_to_ursa10\n",
    NULL
);
#endif

#if defined(CONFIG_URSA9_VB1) || defined(CONFIG_URSA_UNION)
#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    send_I2C_cmd_to_ursa9, CONFIG_SYS_MAXARGS, 1,    do_send_I2C_cmd_to_ursa9,
    "send_I2C_cmd_to_ursa9   -  send I2C command to URSA9\n",
    NULL
);
#endif

#if defined(CONFIG_URSA6_VB1) ||  defined(CONFIG_URSA_10) || defined(CONFIG_URSA9_VB1) || defined(CONFIG_URSA_UNION)
#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    send_I2C_cmd_to_ursa, CONFIG_SYS_MAXARGS, 1,    do_send_I2C_cmd_to_ursa,
    "send_I2C_cmd_to_ursa   -  send I2C command to URSA\n",
    NULL
);
#endif