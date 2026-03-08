/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <command.h>
#include <config.h>
#include <CusSystem.h>


#if (ENABLE_MODULE_ANDROID_BOOT == 1 )
U_BOOT_CMD(
    envload, CONFIG_SYS_MAXARGS, 1,    do_envload,
    "envload   - reload the nand environment.\n",
    NULL
);
#endif

#if defined (CONFIG_MBOOT_VERSION)
U_BOOT_CMD(
    mbootver_set, CONFIG_SYS_MAXARGS, 1,    do_setmbootver,
    "setmbootver   -  for customer show in factory menu.\n",
    NULL
);
#endif

//#if defined(CONFIG_LOAD_ENV_FROM_SN)
U_BOOT_CMD(
    loadenv ,    CONFIG_SYS_MAXARGS,    1,     do_loadenv,
    "loadenv   - load env for nand \n",
    "    -  \n"
);
//#endif

#if defined (CONFIG_SET_4K2K_MODE)
U_BOOT_CMD(
    set_FRC ,    CONFIG_SYS_MAXARGS,    1,     do_setFRC,
    "set_FRC   - set FRC \n",
    "    -  \n"
);
#endif

#if defined (CONFIG_URSA_6M40)
#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    ursa_lvds_on,   CONFIG_SYS_MAXARGS, 1,    do_ursa_lvds_on,
    "ursa_lvds_on   - set ursa lvds on\n",
    NULL
);
U_BOOT_CMD(
    ursa_lvds_off,   CONFIG_SYS_MAXARGS, 1,    do_ursa_lvds_off,
    "ursa_lvds_off   - set ursa lvds off\n",
    NULL
);


#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
		ursa_osd_unmute,	CONFIG_SYS_MAXARGS, 1,	  do_ursa_osd_unmute,
		"ursa_osd_unmute	- ursa_osd_unmute\n",
		NULL
	);
U_BOOT_CMD(
		ursa_2k_mode,	CONFIG_SYS_MAXARGS, 1,	  do_ursa_2k_mode_on,
		"ursa_2k_mode	- ursa_2k_mode_on\n",
		NULL
	);

U_BOOT_CMD(
        ursa_set_osd_mode,   CONFIG_SYS_MAXARGS, 1,    do_ursa_set_osd_mode,
        "ursa_set_osd_mode   - do_ursa_set_osd_mode\n",
        NULL
    );
#endif

#if defined (CONFIG_ENABLE_4K2K_PANEL)
#if defined (CONFIG_INX_VB1) || defined (CONFIG_INX_NOVA_VB1)
U_BOOT_CMD(
    inx_panel_set_init,   CONFIG_SYS_MAXARGS, 1,    do_inx_panel_set_init,
    "inx_panel_sete_init   - init inx panel\n",
    NULL
);

U_BOOT_CMD(
    inx_panel_set_fhd,   CONFIG_SYS_MAXARGS, 1,    do_inx_panel_set_fhd,
    "panel_set_fhd   - set panel as fhd mode\n",
    NULL
);

U_BOOT_CMD(
    inx_panel_set_4k2k,   CONFIG_SYS_MAXARGS, 1,    do_inx_panel_set_4k2k,
    "panel_set_4k2k   - set panel as 4k2k mode\n",
    NULL
);
#endif

#if defined (CONFIG_INX_VB1)
U_BOOT_CMD(
    panel_inx_vb1_init,    CONFIG_SYS_MAXARGS, 1,    do_panel_inx_vb1_init,
    "panel_inx_vb1_init   - panel_inx_vb1_init\n",
    NULL
);
#endif

#if defined (CONFIG_INX_NOVA_VB1)
U_BOOT_CMD(
    inx_nova_set_4k2k_2division,   CONFIG_SYS_MAXARGS, 1,    do_inx_nova_set_4k2k_2division,
    "inx_nova_set_4k2k_2division   - set 4k2k in 2 division\n",
    NULL
);
#endif
#endif
#if defined (CONFIG_URSA_8)
#ifdef CONFIG_MSTAR_STR_MINISIZE
U_BOOT_CMD_STR(
#else
U_BOOT_CMD(
#endif
    ursa8_lvds_on,   CONFIG_SYS_MAXARGS, 1,    do_ursa8_lvds_on,
    "ursa8_lvds_on   - set ursa8 lvds on\n",
    NULL
);

U_BOOT_CMD(
    ursa8_set_osd_mode,   CONFIG_SYS_MAXARGS, 1,    do_ursa8_set_osd_mode,
    "ursa8_set_osd_mode   - do_ursa8_set_osd_mode\n",
    NULL
);

#endif

#if (ENABLE_MODULE_BOOT_IR == 1)
U_BOOT_CMD(
    ir_delay, CONFIG_SYS_MAXARGS, 1,    do_ir_delay,
    "ir_delay   -  delay 300ms for IR detect.\n",
    NULL
);
#endif

#if (CONFIG_URSA12_VB1_FRC_BRINGUP)
U_BOOT_CMD(
    frcup,   CONFIG_SYS_MAXARGS, 1,    do_frc_bringup,
    "frcup   - do_frc_bringup\n",
    NULL
);

U_BOOT_CMD(
    frcsnd,   CONFIG_SYS_MAXARGS, 1,    do_frc_send,
    "frcsnd   - do_frc_send\n",
    "[Usage] - frcsnd [u8CmdIdx] [u32Data]\n"
);

U_BOOT_CMD(
    frcrcv,   CONFIG_SYS_MAXARGS, 1,    do_frc_recv,
    "frcrcv   - do_frc_recv\n",
    "[Usage] - do_frc_recv\n"
);

#endif

