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

#ifndef _DRVIPAUTH_H_
#define _DRVIPAUTH_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "MsTypes.h"
#include "MsCommon.h"

#ifndef DLL_PUBLIC
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#endif

#ifndef DLL_LOCAL
#define DLL_LOCAL __attribute__ ((visibility ("hidden")))
#endif

#define MSIF_IPAUTH_LIB_CODE               {'I','P','A','_'} //Lib code
#define MSIF_IPAUTH_LIBVER                 {'0','0'}      //LIB version
#define MSIF_IPAUTH_BUILDNUM               {'0','0' }    //Build Number
#define MSIF_IPAUTH_CHANGELIST             {'0','0','1','1','5','7','5','8'} //P4 ChangeList Number

#define IPAUTH_CONTROL_VIDEO_H264           27
#define IPAUTH_CONTROL_XC_DVBC              39

#define IPAUTH_VER                         /* Character String for DRV/API version             */  \
    MSIF_TAG,                              /* 'MSIF'                                           */  \
    MSIF_CLASS,                            /* '00:driver, 02:app, 03:mw                        */  \
    MSIF_CUS,                              /* 0x6666                                           */  \
    MSIF_MOD,                              /* 0x0001                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_IPAUTH_LIB_CODE,                  /* IP__                                             */  \
    MSIF_IPAUTH_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_IPAUTH_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_IPAUTH_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

#define IPAUTH_SHM_NAME    "Linux IPAUTH driver"
typedef struct
{
    MS_U32  g_ShmID;
    MS_U32  g_u32ClientCounter;
    MS_U8   g_IpControl[32];
}ST_IPAUTH_SHARED_VARS;

typedef struct
{
    MS_U32 erk[64];     /* encryption round keys */
    MS_U32 drk[64];     /* decryption round keys */
    MS_S32 nr;             /* number of rounds */
}
ST_AES_CONTEXT;

// extern MS_BOOL MApp_IPAUTH_GetLibVer(const MSIF_Version **ppVersion);
DLL_PUBLIC void MApi_AUTH_Process(MS_U8 *string, MS_U8 *hash);
DLL_PUBLIC MS_U8 MApi_AUTH_State(void);
DLL_PUBLIC MS_U8 MDrv_AUTH_IPCheck(MS_U8 u8Bit);
// DLL_PUBLIC MS_BOOL MDrv_AUTH_InitialVars(void);
// DLL_PUBLIC ST_IPAUTH_SHARED_VARS * MDrv_AUTH_AllocateVars(void);
// DLL_PUBLIC MS_BOOL MDrv_AUTH_ResetDefaultVars(ST_IPAUTH_SHARED_VARS * pIPAUTHShared);
// DLL_PUBLIC void MDrv_AUTH_GetHashInfo (MS_U8 *hashinfo);


// DLL_LOCAL MS_S32 MDrv_AUTH_AES_Set_Key( ST_AES_CONTEXT *ctx, MS_U8 *key, MS_S32 nbits );
// DLL_LOCAL void MDrv_AUTH_AES_Encrypt( ST_AES_CONTEXT *ctx, MS_U8 input[16], MS_U8 output[16] );
// DLL_LOCAL void MDrv_AUTH_AES_Decrypt( ST_AES_CONTEXT *ctx, MS_U8 input[16], MS_U8 output[16] );


#ifdef __cplusplus

}
#endif

#endif //_DRVIPAUTH_H_
