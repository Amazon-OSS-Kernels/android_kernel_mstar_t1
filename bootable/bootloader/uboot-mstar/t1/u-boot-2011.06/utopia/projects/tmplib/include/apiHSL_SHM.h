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
#if defined (MSOS_TYPE_LINUX)
#ifndef FRHSL_SHM_H
#define FRHSL_SHM_H

/*******************************************************************************
** Includes
*******************************************************************************/

#include "MsTypes.h"

#ifdef  __cplusplus
extern "C" {
#endif


#define HSL_SHMEM_KEY 7995
#define HSL_SHMEM_SIZE 128

typedef struct
{
  char  data[HSL_SHMEM_SIZE];
  MS_BOOL handled;
}cHslSyncData;

typedef struct
{
  cHslSyncData* ptr;
  int           id;
}cHslSyncMem; // manage the real data access


/*******************************************************************************
* Prototype
*******************************************************************************/
MS_BOOL InitShareMemory(cHslSyncMem* syncMem, MS_BOOL create);
void FreeShareMemory(cHslSyncMem* syncMem);
MS_BOOL StoreShareMemory(cHslSyncMem* syncMem, void* source);
MS_BOOL RestoreShareMemory(cHslSyncMem* syncMem, void* dest);

void PackAtRespIntoHslCmdResp(const char* a_atResp);
void PackAtCmdEchoIntoHslCmdResp(MS_BOOL a_result, const char* a_atCmd);

#ifdef  __cplusplus
}
#endif

#endif

#endif
