/**
* Copyright (c) 2006 – 2018 MStar Semiconductor, Inc.
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
//******************************************************************************
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////

#ifndef _ACP_PRIV_H_
#define _ACP_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @file apiACP_private.h
/// @brief Byte ACP control
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

typedef ACP_Result      (*IOCTL_ACP_SET_PROTECTION)          (MS_BOOL , MS_BOOL, MS_U8);
typedef ACP_Result      (*IOCTL_ACP_SET_MV_BITCONTROL)       (MS_BOOL , MS_ACP_MV_BitControl);
typedef ACP_Result      (*IOCTL_ACP_DCS_PROTECTION)          (MS_BOOL , MS_U8);
typedef ACP_Result      (*IOCTL_ACP_DCS_SETACTIVATIONKEY)   (MS_U8 * , MS_U8);

typedef enum
{
    E_ACP_POOL_ID_INTERNAL_VARIABLE = 0,
    E_ACP_POOL_ID_MAX,
} E_ACP_POOL_ID;

typedef struct
{
    // flow control related
    MS_BOOL bResourceRegistered;
} ACP_RESOURCE_PRIVATE;

typedef struct _ACP_INSTANT_PRIVATE
{
    IOCTL_ACP_SET_PROTECTION                  fpACPSetProtection;
    IOCTL_ACP_SET_MV_BITCONTROL               fpACPSetMVBitControl;
    IOCTL_ACP_DCS_PROTECTION                  fpACPDCSProtection;
    IOCTL_ACP_DCS_SETACTIVATIONKEY            fpACPDCSSetActivationKey;
} ACP_INSTANT_PRIVATE;

ACP_Result MApi_ACP_SetProtection_U2(MS_BOOL bEnable, MS_BOOL bIsYPbPr, MS_U8 u8Type);
ACP_Result MApi_ACP_SetMV_BitControl_U2(MS_BOOL bEnable, MS_ACP_MV_BitControl MV_BitControl_Data);
ACP_Result MApi_DCS_SetProtection_U2(MS_BOOL bEnable, MS_U8 u8Type);
ACP_Result MApi_DCS_SetActivationKey_U2(MS_U8 *pu8ActivationKeyTbl, MS_U8 u8ActivationKeyTblSize);

void ACPRegisterToUtopia(void);
MS_U32 ACPOpen(void** ppInstance, const void* const pAttribute);
MS_U32 ACPClose(void* pInstance);
MS_U32 ACPIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);

#ifdef __cplusplus
}
#endif
#endif // _ACP_PRIV_H_
