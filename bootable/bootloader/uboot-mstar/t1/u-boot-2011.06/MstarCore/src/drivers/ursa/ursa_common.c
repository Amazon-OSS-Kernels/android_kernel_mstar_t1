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

#ifndef __DRV_URSA_COMMON_C__
#define __DRV_URSA_COMMON_C__
#include "ursa_common.h"
#include <command.h>
#include <common.h>
#include <MsSystem.h>
#include <MsDebug.h>
#include <MsSysUtility.h>
/*
 * creating the global URSA command table here.
 */

URSA_COMMON_STATUS MDrv_URSA_COM_Write_URSA_HandshakeType(void)
{
    UBOOT_TRACE("IN\n");

    URSA_COMMON_TYPE ursa_type = MDrv_URSA_COM_Get_URSA_Type();

    switch (ursa_type)
    {
        case URSA_COMMON_U6:
            Write2Byte(URSA_COMMON_HANDSHAKE_TYPE_DUMMY_REG, URSA_COMMON_URSA6_HANDSHAKE);
            UBOOT_TRACE("OK\n");
            return URSA_SUCCED;

        case URSA_COMMON_U7:
            Write2Byte(URSA_COMMON_HANDSHAKE_TYPE_DUMMY_REG, URSA_COMMON_URSA7_HANDSHAKE);
            UBOOT_TRACE("OK\n");
            return URSA_SUCCED;

        case URSA_COMMON_U8:        // it don't need handshake
            UBOOT_TRACE("OK\n");
            return URSA_SUCCED;

        case URSA_COMMON_U9:
            Write2Byte(URSA_COMMON_HANDSHAKE_TYPE_DUMMY_REG, URSA_COMMON_URSA7_HANDSHAKE);
            UBOOT_TRACE("OK\n");
            return URSA_SUCCED;

        case URSA_COMMON_KS2:
            Write2Byte(URSA_COMMON_HANDSHAKE_TYPE_DUMMY_REG, URSA_COMMON_URSA6_HANDSHAKE);
            UBOOT_TRACE("OK\n");
            return URSA_SUCCED;

        case URSA_COMMON_FRC:
            Write2Byte(URSA_COMMON_HANDSHAKE_TYPE_DUMMY_REG, URSA_COMMON_URSA6_HANDSHAKE);
            UBOOT_TRACE("OK\n");
            return URSA_SUCCED;

        case URSA_COMMON_UNKNOWN:
            UBOOT_ERROR("[URSA_COMMON] Unknown URSA type !\n");
            UBOOT_TRACE("OK\n");
            return URSA_FAILED;

        default:
            UBOOT_ERROR("[URSA_COMMON] ursa_type is a illeagal value !\n");
            UBOOT_TRACE("OK\n");
            return URSA_FAILED;
    }
}

URSA_COMMON_TYPE MDrv_URSA_COM_Get_URSA_Type(void)
{
    UBOOT_TRACE("IN\n");

    int ursa_type_num = URSA_COMMON_UNKNOWN;
    st_sys_misc_setting misc_setting;
    char *ursa_type = getenv("ursa_type");

    if (NULL != ursa_type)
    {
        ursa_type_num = simple_strtoul(ursa_type, NULL, 10);
    }
    else
    {
        Read_MiscSetting_ToFlash(&misc_setting);
        ursa_type_num = misc_setting.m_u32ursa_type;
    }

    switch (ursa_type_num)
    {
        case URSA_COMMON_U6:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is U6.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_U6;

        case URSA_COMMON_U7:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is U7.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_U7;

        case URSA_COMMON_U8:        // it don't need handshake
            UBOOT_DEBUG("[URSA_COMMON] ursa type is U8.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_U8;

        case URSA_COMMON_U9:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is U9.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_U9;

        case URSA_COMMON_KS2:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is KS2.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_KS2;

        case URSA_COMMON_FRC:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is FRC.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_FRC;

        case URSA_COMMON_U11:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is FRC.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_U11;

        case URSA_COMMON_UNKNOWN:
            UBOOT_ERROR("[URSA_COMMON] ursa_type is an illeagal value !\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_UNKNOWN;

        default:
            UBOOT_ERROR("[URSA_COMMON] ursa_type is a illeagal value !\n");
            return URSA_FAILED;
    }

}
#endif //__DRV_URSA_COMMON_C__
