/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//
//
//
//
//
//
//
//
//
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvSEM_priv.h
/// @brief  SEM Driver Interface
///
////////////////////////////////////////////////////////////////////////////////////////////////////



#ifndef _DRVSEM_PRIV_H_
#define _DRVSEM_PRIV_H_

#ifdef __cplusplus
extern "C"
{
#endif

// driver version
#define SEM_VERSION            0x00000001

enum {
    MDrv_CMD_SEM_Get_Resource,
    MDrv_CMD_SEM_Free_Resource,
    MDrv_CMD_SEM_Reset_Resource,
    MDrv_CMD_SEM_Get_ResourceID,
    MDrv_CMD_SEM_Get_Num,
    MDrv_CMD_SEM_GetLibVer,
    MDrv_CMD_SEM_Lock,
    MDrv_CMD_SEM_Unlock,
    MDrv_CMD_SEM_Delete,
} eSEMIoctlOpt;

//ioctl function pointer type
typedef MS_BOOL (*IOCTL_SEM_GETRESOURCE)(MS_U8, MS_U16);
typedef MS_BOOL (*IOCTL_SEM_FREERESOURCE)(MS_U8, MS_U16);
typedef MS_BOOL (*IOCTL_SEM_RESETRESOURCE)(MS_U8);
typedef MS_BOOL (*IOCTL_SEM_GETRESOURCEID)(MS_U8, MS_U16*);
typedef MS_U32 (*IOCTL_SEM_GETNUM)(void);
typedef MS_BOOL (*IOCTL_SEM_GETLIBVER)(const MSIF_Version **);
typedef MS_BOOL (*IOCTL_SEM_LOCK)(MS_U32, MS_U32);
typedef MS_BOOL (*IOCTL_SEM_UNLOCK)(MS_U32);
typedef MS_BOOL (*IOCTL_SEM_DELETE)(MS_U32);

// INSTANCE Private(ioctl function pointer)
typedef struct _SEM_INSTANT_PRIVATE
{
    IOCTL_SEM_GETRESOURCE       fpSEMGetResource;
    IOCTL_SEM_FREERESOURCE      fpSEMFreeResource;
    IOCTL_SEM_RESETRESOURCE     fpSEMResetResource;
    IOCTL_SEM_GETRESOURCEID     fpSEMGetResourceID;
    IOCTL_SEM_GETNUM            fpSEMGetNum;
    IOCTL_SEM_GETLIBVER         fpSEMGetLibVer;
    IOCTL_SEM_LOCK              fpSEMLock;
    IOCTL_SEM_UNLOCK            fpSEMUnlock;
    IOCTL_SEM_DELETE            fpSEMDelete;
}SEM_INSTANT_PRIVATE;

//////////////////////////////////////
// function parameter structure //////
//////////////////////////////////////
// for cmd MDrv_SEM_Get_Resource
typedef struct _SEM_GETRESOURCE_PARAM
{
    MS_U8 u8SemID;
    MS_U16 u16ResId;
}SEM_GETRESOURCE_PARAM, *PSEM_GETRESOURCE_PARAM;

// for cmd MDrv_SEM_Free_Resource
typedef struct _SEM_FREERESOURCE_PARAM
{
    MS_U8 u8SemID;
    MS_U16 u16ResId;
}SEM_FREERESOURCE_PARAM, *PSEM_FREERESOURCE_PARAM;

// for cmd MDrv_SEM_Reset_Resource
typedef struct _SEM_RESETRESOURCE_PARAM
{
    MS_U8 u8SemID;
}SEM_RESETRESOURCE_PARAM, *PSEM_RESETRESOURCE_PARAM;

// for cmd MDrv_SEM_Get_ResourceID
typedef struct _SEM_GETRESOURCEID_PARAM
{
    MS_U8 u8SemID;
    MS_U16* pu16ResId;
}SEM_GETRESOURCEID_PARAM, *PSEM_GETRESOURCEID_PARAM;

// for cmd MDrv_SEM_GetLibVer
typedef struct _SEM_GETLIBVER_PARAM
{
    const MSIF_Version **ppVersion;
}SEM_GETLIBVER_PARAM, *PSEM_GETLIBVER_PARAM;

// for cmd MDrv_SEM_Lock
typedef struct _SEM_LOCK_PARAM
{
    MS_U32 u32SemID;
    MS_U32 u32WaitMs;
}SEM_LOCK_PARAM, *PSEM_LOCK_PARAM;

// for cmd MDrv_SEM_Unlock
typedef struct _SEM_UNLOCK_PARAM
{
    MS_U32 u32SemID;
}SEM_UNLOCK_PARAM, *PSEM_UNLOCK_PARAM;

// for cmd MDrv_SEM_Delete
typedef struct _SEM_DELETE_PARAM
{
    MS_U32 u32SemID;
}SEM_DELETE_PARAM, *PSEM_DELETE_PARAM;

//////////////////////////////////////
// function for utopia2.0 ////////////
//////////////////////////////////////
void SEMRegisterToUtopia(void);
MS_U32 SEMOpen(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 SEMClose(void* pInstance);
MS_U32 SEMIoctl(void* pInstance, MS_U32 u32Cmd, void *pArgs);


// Resource_Private
typedef struct _SEM_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}SEM_RESOURCE_PRIVATE;

#ifdef __cplusplus
}
#endif

#endif // _DRVSEM_PRIV_H_
