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

#ifndef __DRV_URSA_COMMON_H__
#define __DRV_URSA_COMMON_H__

#include <MsTypes.h>

#define URSA_COMMON_HANDSHAKE_TYPE_DUMMY_REG     0x0EA2
#define URSA_COMMON_URSA6_HANDSHAKE                0x2  // U6, FRC and KS2
#define URSA_COMMON_URSA7_HANDSHAKE                0x4  // U7 and U9


/*
 * URSA status code:
 * successful operation returns URSA_SUCCED;
 * failed operation returns URSA_FAILED.
 */
typedef enum {
    URSA_SUCCED = 0,
    URSA_FAILED,
} URSA_COMMON_STATUS;

/*
 * URSA type
 */
typedef enum {
    URSA_COMMON_UNKNOWN = 0,
    URSA_COMMON_U6 = 6,
    URSA_COMMON_U7 = 7,
    URSA_COMMON_U8 = 8,
    URSA_COMMON_U9 = 9,
    URSA_COMMON_KS2 = 1000,
    URSA_COMMON_FRC = 2000,
    URSA_COMMON_U11 = 11,
} URSA_COMMON_TYPE;

URSA_COMMON_STATUS MDrv_URSA_COM_Write_URSA_HandshakeType(void);
URSA_COMMON_TYPE MDrv_URSA_COM_Get_URSA_Type(void);

#endif //__DRV_URSA_COMMON_H__
