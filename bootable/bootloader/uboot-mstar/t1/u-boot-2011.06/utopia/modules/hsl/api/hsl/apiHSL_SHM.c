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
////////////////////////////////////////////////////////////////////////////////

#if defined (MSOS_TYPE_LINUX)
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>


#include "apiHSL.h"
#include "apiHSL_SHM.h"
#include "frhsl_autotest.h"

static pthread_mutex_t hslShmMutex;

MS_BOOL InitShareMemory(cHslSyncMem* syncMem, MS_BOOL create)
{
    if (pthread_mutex_init(&hslShmMutex, NULL))
    {
        printf("HSL: initMutex error\n");
        return false;
    }

    pthread_mutex_lock(&hslShmMutex);
  
    if( create )
    {
        syncMem->id = shmget(HSL_SHMEM_KEY, sizeof(cHslSyncData), IPC_CREAT| IPC_EXCL | 0777);
        if( syncMem->id == -1)
        {
            printf("HSL Can't get share memory!\nYour share memory might be off(SHMconfig option)!\n");
            return false;
        }
    }
    else
    {
        syncMem->id = shmget(HSL_SHMEM_KEY, 0, 0);
        if( syncMem->id == -1)
        {
            printf("HSL Can't locate share memory!\n");
            return false;
        }
    }

    syncMem->ptr = shmat(syncMem->id, NULL, 0);
    if( syncMem->ptr == NULL)
    {
        printf("HSL Map share memory failed!\n");
        return false;
    }

    syncMem->ptr->handled = true;
    pthread_mutex_unlock(&hslShmMutex);

    return true;
}


void FreeShareMemory(cHslSyncMem* syncMem)
{
    pthread_mutex_lock(&hslShmMutex);

    if(0 != shmdt(syncMem->ptr))
    {
        printf("shmdt failed!\n");
    }

    if(0 != shmctl(syncMem->id, IPC_RMID, NULL))
    {
        printf("shmctl(IPC_RMID) failed!\n");
    }
    pthread_mutex_unlock(&hslShmMutex);

    if(0 != pthread_mutex_destroy(&hslShmMutex))
    {
        printf("mutex_destroy error\n");
    }
}


MS_BOOL StoreShareMemory(cHslSyncMem* syncMem, void* source)
{
    MS_BOOL result = false;

    if( (syncMem->id == -1) ||
      (syncMem->ptr == NULL))
    {
        return false;
    }

    pthread_mutex_lock(&hslShmMutex);
    if( syncMem->ptr->handled )
    {
        syncMem->ptr->handled = false;
        memcpy(syncMem->ptr->data, source, HSL_SHMEM_SIZE);
        result = true;
    }
    pthread_mutex_unlock(&hslShmMutex);

    return result;
}

MS_BOOL RestoreShareMemory(cHslSyncMem* syncMem, void* dest)
{
    MS_BOOL result = false;

    if( (syncMem->id == -1) ||
        (syncMem->ptr == NULL))
    {
        return false;
    }

    pthread_mutex_lock(&hslShmMutex);
    if( syncMem->ptr->handled == false)
    {
        memcpy(dest, syncMem->ptr->data, HSL_SHMEM_SIZE);
        syncMem->ptr->handled = true;
        result = true;
    }
    pthread_mutex_unlock(&hslShmMutex);

    return result;
}
#endif
