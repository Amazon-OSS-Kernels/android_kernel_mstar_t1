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

#include <unistd.h>     //close
#include <fcntl.h>      // open
#include <sys/ioctl.h>  // ioctl
#include <sys/mman.h>
#include "MsCommon.h"
#include "halMPool.h"
#include "halCHIP.h"
#include "drvMMIO.h"
#include <string.h>
#include <errno.h>
#include <pthread.h>

#if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER)
#include "MsOS.h"
#include "utopia.h"
#endif

#if defined (ANDROID)
#include <cutils/log.h>
#endif

#if defined (TV_OS)
#include "mdrv_semutex_io.h"
#include <pthread.h>
#endif

//--------------------------------------------------------------------------------------------------
//  Internal define
//--------------------------------------------------------------------------------------------------
#ifdef _Debug
#define MPOOL_DEBUG
#endif

//--------------------------------------------------------------------------------------------------
//  New Internal Variable of MPool Implementation
//--------------------------------------------------------------------------------------------------
#define SUPPORT_PARTIAL_MAPPING 1
static MS_U32		MPOOL_MAPPING;
#define MAX_MAPPINGSIZE 200

#define MMAP_NONCACHE true
#define MMAP_CACHE false

#define ION_ALIGN 0x1000    // align to PAGE_SIZE, remember this ALIGN must <= PAGE_SIZE

#if (defined ANDROID) && (defined TV_OS)
#include <cutils/log.h>
#define printf LOGD
#ifndef LOGD // android 4.1 rename LOGx to ALOGx
#define LOGD ALOGD
#endif
#endif
/*
static MS_BOOL             _Mpool_use[MAX_MAPPINGSIZE];//={-1}
static MS_U8               _Mpool_NonCache[MAX_MAPPINGSIZE];

static void*               _pMpoolAddrVStart[MAX_MAPPINGSIZE];// = { NULL, NULL};
static void*               _pMpoolAddrVEnd[MAX_MAPPINGSIZE];// = { NULL, NULL};
static MS_U32              _u32MpoolAddrPhys[MAX_MAPPINGSIZE];// = { 0, 0};
static MS_U32              _u64MpoolSize[MAX_MAPPINGSIZE];// = { 0, 0};
static MS_U32              _u32MPoolVa2PaOff[MAX_MAPPINGSIZE];// = { 0, 0};
*/

//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------
static MS_S32              _s32MPoolFd = -1;
static MS_S32              _s32FdION = -1;
static void*               _pAddrVirtStart[2] = { NULL, NULL};
static void*               _pAddrVirtEnd[2] = { NULL, NULL};
#if (!SUPPORT_PARTIAL_MAPPING)
static MS_SIZE              _tAddrPhys[2] = { 0, 0};
static MS_U32              _u32Size[2] = { 0, 0};
#endif
static MS_SIZE              _tVa2PaOff[2] = { 0, 0};

static MS_VIRT u32MpoolRegBase;

typedef struct
{
    MS_VIRT u64VirtStart;
    MS_VIRT u64VirtEnd;
    MS_U64 u64Phyaddr;
    MS_U64 u64MpoolSize;
    MS_BOOL bIsUsed;
    MS_SIZE s32V2Poff;
    MS_BOOL bNonCache;
    MS_U8 u8MiuSel;
    MS_BOOL bIsDyn;
} MPOOL_INFO;

static MsOSMPool_DbgLevel    _u32MPoolDBGLevel;
static MPOOL_INFO mpool_info[MAX_MAPPINGSIZE];
static pthread_mutex_t _MsOS_MPool_Mutex = PTHREAD_MUTEX_INITIALIZER;

#if defined (TV_OS)
static  MS_S32  _s32SemutexFD = -1;
static  void*     _psAddr;
static pthread_mutex_t          _MsOS_Openfd_Mutex = PTHREAD_MUTEX_INITIALIZER;
#endif
//--------------------------------------------------------------------------------------------------
//  Internal macros
//--------------------------------------------------------------------------------------------------
#ifdef MPOOL_DEBUG
    #if defined (ANDROID)
    #ifndef LOGI // android 4.1 rename LOGx to ALOGx
    #define LOGI ALOGI
    #endif
    #define MPOOL_ERROR(fmt, args...)           LOGI("[MPOOL USER ERR][%06d]     " fmt, __LINE__, ## args)
    #define MPOOL_WARN(fmt, args...)            LOGI("[MPOOL USER WARN][%06d]    " fmt, __LINE__, ## args)
    #define MPOOL_PRINT(fmt, args...)           LOGI("[MPOOL USER][%06d]     " fmt, __LINE__, ## args)
    #else
    #define MPOOL_ERROR(fmt, args...)           printf("[MPOOL USER ERR][%06d]     " fmt, __LINE__, ## args)
    #define MPOOL_WARN(fmt, args...)            printf("[MPOOL USER WARN][%06d]    " fmt, __LINE__, ## args)
    #define MPOOL_PRINT(fmt, args...)           printf("[MPOOL USER][%06d]     " fmt, __LINE__, ## args)
    #endif

    #define MPOOL_ASSERT(_bool, _f)             if (!(_bool)) { (_f); MS_ASSERT(0); }
#else
    #define MPOOL_ERROR(fmt, args...)           do{} while (0)
    #define MPOOL_WARN(fmt, args...)            do{} while (0)
    #define MPOOL_PRINT(fmt, args...)           do{} while (0)
    #define MPOOL_ASSERT(_bool, _f)             if (!(_bool)) { (_f); }
#endif

#define MPOOL_IS_CACHE_ADDR(addr)              ( ( ((MS_VIRT)_pAddrVirtStart[0] <= (MS_VIRT)(addr)) && ((MS_VIRT)_pAddrVirtEnd[0] > (MS_VIRT)addr) ) ? TRUE : FALSE )
#define MPOOL_IS_UNCACHE_ADDR(addr)            ( ( ((MS_VIRT)_pAddrVirtStart[1] <= (MS_VIRT)(addr)) && ((MS_VIRT)_pAddrVirtEnd[1] > (MS_VIRT)addr) ) ? TRUE : FALSE )

#define MPOOL_DBG_MSG(debug_level, x)     do { if (_u32MPoolDBGLevel >= (debug_level)) (x); } while(0)

static MS_S32 _s32MapMutexId = -1;
static MS_U8  _u8MapMutex[] = {"MPool_Mutex"};

//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
MS_BOOL MsOS_MPool_IsInitialized(void)
{
	MsOS_MPool_SetDbgLevel(E_MsOSMPool_DBG_Release);

	if (0 <= _s32MPoolFd)
	{
		printf("\033[35mFunction = %s, Line = %d, [Warning] MPOOL is already Initialized\033[m\n", __PRETTY_FUNCTION__, __LINE__);
		return TRUE;
	}

	return FALSE;
}

MS_BOOL MsOS_MPool_Init(void)
{
    MS_PHY u32size;

	MsOS_MPool_SetDbgLevel(E_MsOSMPool_DBG_Release);

    if (0 <= _s32MPoolFd)
    {
        MPOOL_WARN("%s is initiated more than once\n", __FUNCTION__);
        return FALSE;
    }

    memset(mpool_info, 0, sizeof(MPOOL_INFO)*MAX_MAPPINGSIZE);

    if (0 > (_s32MPoolFd = open("/dev/malloc", O_RDWR)))
    {
        MPOOL_ERROR("Open /dev/malloc fail\n");
        MS_ASSERT(0);
        return FALSE;
    }
	else
	{
	}

    //Create Mutex
    if(_s32MapMutexId < 0)
    {
		MsOS_Init();  // Before MsOS_CreateMutex, need to do MsOS_Init()
        _s32MapMutexId = MsOS_CreateMutex(E_MSOS_FIFO, (char*)_u8MapMutex, MSOS_PROCESS_SHARED);
    }
    if(_s32MapMutexId < 0)
    {
        printf("%s MsOS_CreateMutex failed!!\n", __FUNCTION__);
        return FALSE;
    }

#if defined (TV_OS)
	pthread_mutex_lock(&_MsOS_Openfd_Mutex);
    if(_s32SemutexFD < 0)
    {
		_s32SemutexFD = open("/dev/semutex", O_RDWR);
        if(0 > _s32SemutexFD)
        {
            MPOOL_ERROR("Open /dev/semutex fail, _s32SemutexFD = %d \n",_s32SemutexFD);
            MS_ASSERT(0);
            pthread_mutex_unlock(&_MsOS_Openfd_Mutex);
            return FALSE;
        }
    }
    pthread_mutex_unlock(&_MsOS_Openfd_Mutex);
#endif

    if(!MDrv_MMIO_GetBASE(&u32MpoolRegBase, &u32size, MS_MODULE_PM))
    {
        MPOOL_ERROR("%s Get IOMAP Base faill!\n",__FUNCTION__);
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL _MPOOL_MutexLock(void)
    {
    if (_s32MapMutexId != -1)
    {
        return MsOS_ObtainMutex(_s32MapMutexId, MSOS_WAIT_FOREVER);
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MsOS_ION_IsInitialized(void)
{
	MsOS_MPool_SetDbgLevel(E_MsOSMPool_DBG_Release);

	if (0 <= _s32FdION)
	{
		printf("\033[35mFunction = %s, Line = %d, [Warning] ION is already Initialized\033[m\n", __PRETTY_FUNCTION__, __LINE__);
		return TRUE;
	}
	else
		printf("\033[35mFunction = %s, Line = %d, ION is not Initialized\033[m\n", __PRETTY_FUNCTION__, __LINE__);

	return FALSE;
}

MS_BOOL MsOS_ION_Init(void)
{
	printf("\033[35mOpen /dev/ion ...\033[m\n");
    if (0 <= _s32FdION)
    {
        printf("\033[35m ION Fd is opened\033[m\n");
        return FALSE;
    }

    if (0 > (_s32FdION = open("/dev/ion", O_RDWR)))
    {
        printf("\033[35mOpen /dev/ion fail\033[m\n");
        return FALSE;
    }

	printf("\033[35mOpen /dev/ion success\033[m\n");
    return TRUE;
}

static MS_BOOL _MPOOL_MutexUnlock(void)
{
    if (_s32MapMutexId != -1)
    {
        return MsOS_ReleaseMutex(_s32MapMutexId);
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MsOS_MPool_Close(void)
{
    if (0 > _s32MPoolFd)
    {
        MPOOL_WARN("%s is closed before initiated\n", __FUNCTION__);
        return FALSE;
    }

    if(_s32MapMutexId != -1)
    {
        MsOS_DeleteMutex(_s32MapMutexId);
        _s32MapMutexId = -1;
    }

    close(_s32MPoolFd);
    _s32MPoolFd = -1;

#if defined (TV_OS)
    pthread_mutex_lock(&_MsOS_Openfd_Mutex);//coverity Data race condition
    if (0 > _s32SemutexFD)
    {
        MPOOL_WARN("%s is closed before initiated\n", __FUNCTION__);
        pthread_mutex_unlock(&_MsOS_Openfd_Mutex);//coverity Data race condition
        return FALSE;
    }
    close(_s32SemutexFD);
    _s32SemutexFD = -1;
    _psAddr = NULL;
    pthread_mutex_unlock(&_MsOS_Openfd_Mutex);//coverity Data race condition
#endif

    return TRUE;
}

MS_BOOL MsOS_ION_Close(void)
{
    if(0 > _s32FdION)
    {
        MPOOL_WARN("%s is closed before initiated\n", __FUNCTION__);
        return FALSE;
    }

    close(_s32FdION);
    _s32MPoolFd = -1;

    return TRUE;
}

#if defined (TV_OS)
//~!~get sharememory size
MS_U32 MsOS_GetSHMSize(void)
{
    MS_U32  u32SHMSize = 0;

    pthread_mutex_lock(&_MsOS_Openfd_Mutex);
    if(_s32SemutexFD < 0)
    {
        _s32SemutexFD = open("/dev/semutex", O_RDWR);

        if(0 > _s32SemutexFD)
        {
            MPOOL_ERROR("Open /dev/semutex fail, _s32SemutexFD = %d \n",_s32SemutexFD);
            MS_ASSERT(0);
            pthread_mutex_unlock(&_MsOS_Openfd_Mutex);
            return 0;
        }
    }
    pthread_mutex_unlock(&_MsOS_Openfd_Mutex);


    if(ioctl(_s32SemutexFD,MDRV_SEMUTEX_GET_SHMSIZE, &u32SHMSize))
    {
        MPOOL_ERROR("get SHM size failed!\n");
        MS_ASSERT(0);
        return 0;
    }

    return u32SHMSize;
}

//~!~ expand sharemem
#ifdef CONFIG_UTOPIA_SHM_EXPAND_SUPPORT
MS_BOOL MsOS_SHM_Expand(MS_U32 u32ShmSize, MS_U32 u32ExpandSize)
{
    printf("[msos] expanding shared memory %d\n", u32ExpandSize);
    if(_s32SemutexFD < 0)
    {
        MPOOL_ERROR("%s: mapping sharemem with invalid dev fd \n", __FUNCTION__);
        return FALSE;
    }
    u32ExpandSize += u32ShmSize;

    if(ioctl(_s32SemutexFD,MDRV_SEMUTEX_EXPAND_SHAREMEMORY,&u32ExpandSize))
    {
        MPOOL_ERROR("create an anonymous sharememory failed! \n");
        close(_s32SemutexFD);
        return FALSE;
    }

    return TRUE;
}
#endif

//~!~ map sharemem
MS_VIRT MsOS_Mapping_SharedMem(MS_U32 u32ShmSize,MS_BOOL *bInit)
{
    //void * pAddr; unused variable - fixed compile warning
    *bInit = FALSE;

    if(_s32SemutexFD < 0)
    {
        MPOOL_ERROR("%s: mapping sharemem with invalid dev fd \n", __FUNCTION__);
        //return NULL; fixed compile warning
        return 0;
    }

//!!!
//there is a limitation, you must map sharemem first with the most largest len!
//that is , if you map with len A, then map with B(B > A), problem happens!!!

    if(_psAddr)
    {
        MPOOL_ERROR("~!~ 2 MsOS_Mapping_SharedMem = %p \n",_psAddr);
        return (MS_VIRT)_psAddr;
    }

    MPOOL_ERROR("~!~!~%s: pid = %d \n", __FUNCTION__,getpid());

    if(ioctl(_s32SemutexFD,MDRV_SEMUTEX_CREATE_SHAREMEMORY,&u32ShmSize))
    {
        MPOOL_ERROR("create an anonymous sharememory failed! \n");
        close(_s32SemutexFD);
        //return NULL; fixed compile warning
        return 0;
    }

    *bInit = ioctl(_s32SemutexFD,MDRV_SEMUTEX_QUERY_ISFIRST_SHAREMEMORY,NULL);
#ifdef CONFIG_UTOPIA_SHM_EXPAND_SUPPORT
    _psAddr = mmap(NULL,MsOS_GetSHMSize(),PROT_READ | PROT_WRITE,MAP_SHARED,_s32SemutexFD,0);
#else
    _psAddr = mmap(NULL,u32ShmSize,PROT_READ | PROT_WRITE,MAP_SHARED,_s32SemutexFD,0);
#endif

    //LOGE("u32ShmSize= %d, _s32SemutexFD = %d, pAddr= %p \n",u32ShmSize,_s32SemutexFD,_psAddr);

    if (_psAddr == MAP_FAILED)
    {
        MPOOL_ERROR("[%s][%d] fail\n", __FUNCTION__, __LINE__);
        close(_s32SemutexFD);
        //return NULL; fixed compile warning
        return 0;
    }

    //LOGE("~!~ 1 MsOS_Mapping_SharedMem = %p \n",(MS_U32)_psAddr);

    //*bInit = TRUE;
    return (MS_VIRT)_psAddr;

}

//~!~create namedmutex
MS_S32 MsOS_CreateNamedMutex(MS_S8 *ps8MutexName)
{
    MS_S32  s32Index;

    pthread_mutex_lock(&_MsOS_Openfd_Mutex);
    if(_s32SemutexFD < 0)
    {
        _s32SemutexFD = open("/dev/semutex", O_RDWR);

        if(0 > _s32SemutexFD)
        {
            MPOOL_ERROR("Open /dev/semutex fail, _s32SemutexFD = %d \n",_s32SemutexFD);
            MS_ASSERT(0);
            pthread_mutex_unlock(&_MsOS_Openfd_Mutex);
            return FALSE;
        }
    }
    pthread_mutex_unlock(&_MsOS_Openfd_Mutex);

#if 0
    CREATE_MUTEX_ARG createmutex;
    createmutex.pnameaddr = (unsigned long)ps8MutexName;
    createmutex.length = strlen(ps8MutexName);
    s32Index = ioctl(_s32SemutexFD,MDRV_SEMUTEX_CREATE_MUTEX,&createmutex);
#else
    s32Index = ioctl(_s32SemutexFD,MDRV_SEMUTEX_CREATE_MUTEX,ps8MutexName);
#endif

    if(s32Index < MUTEX_INDEX_BEGIN)
    {
        MPOOL_WARN("the returned mutex index invalid!!! \n");
        return -1;
    }

    //s32Index -= MUTEX_INDEX_BEGIN;

    return s32Index;
}

//~!~lock mutex
MS_BOOL MsOS_LockMutex(MS_S32 u32Index,MS_S32 flag)
{
    MS_S32 res = 0;
    if(_s32SemutexFD < 0)
    {
        MPOOL_WARN("%s: Lock mutex with invalid dev fd \n", __FUNCTION__);
        return FALSE;
    }

    if(u32Index < MUTEX_INDEX_BEGIN)
    {
        MPOOL_WARN("%s invalid mutex index : u32Index = %d,flag = %d \n", __FUNCTION__,u32Index,flag);
        return FALSE;
    }

    if(flag == 0)
    {
        res = -1;
        while(res == -1)
        {
            res = ioctl(_s32SemutexFD,MDRV_SEMUTEX_LOCK,&u32Index);
            if(res == -1 && errno == EINTR)
                usleep(1000);
        }
    }
    else if(flag == -1)
        res = ioctl(_s32SemutexFD,MDRV_SEMUTEX_TRY_LOCK,&u32Index);
    else
    {
        LOCKARG lockarg;
        lockarg.index = u32Index;
        lockarg.time = flag;
        res = ioctl(_s32SemutexFD,MDRV_SEMUTEX_LOCK_WITHTIMEOUT,&lockarg);
    }
    if(res == 0)
        return TRUE;
    else
        return FALSE;
}


//~!~lock mutex
MS_BOOL MsOS_UnlockMutex(MS_S32 u32Index, MS_S32 flag)
{
    MS_S32 res = 0;

	if(_s32SemutexFD < 0)
    {
        MPOOL_WARN("%s: Lock mutex with invalid dev fd \n", __FUNCTION__);
        return FALSE;
    }

    if(u32Index < MUTEX_INDEX_BEGIN)
    {
        MPOOL_WARN("%s invalid mutex index  : u32Index = %d,flag = %d \n", __FUNCTION__,u32Index,flag);
        return FALSE;
    }

    res = ioctl(_s32SemutexFD,MDRV_SEMUTEX_UNLOCK,&u32Index);

    if(res == 0)
        return TRUE;
    else
        return FALSE;
}

MS_BOOL MsOS_CrossThreadUnlockMutex(MS_S32 u32Index, MS_BOOL bEnable)
{
    MS_S32 res = 0;
    CROSS_THREAD_UNLOCK_INFO unlock_info = {0};

    if(_s32SemutexFD < 0)
    {
        MPOOL_WARN("%s: Lock mutex with invalid dev fd \n", __FUNCTION__);
        return FALSE;
    }
    if(u32Index < MUTEX_INDEX_BEGIN)
    {
        MPOOL_WARN("%s invalid mutex index  : u32Index = %d \n", __FUNCTION__,u32Index);
        return FALSE;
    }
    unlock_info.index = u32Index;
    unlock_info.flag = (TRUE == bEnable)?E_CROSS_THREAD_UNLOCK_ENABLE:E_CROSS_THREAD_UNLOCK_DISABLE;
    res = ioctl(_s32SemutexFD,MDRV_SEMUTEX_SET_CROSS_THREAD_UNLOCK,&unlock_info);
    if(res == 0)
        return TRUE;
    else
    {
        perror("ioctl MDRV_SEMUTEX_SET_CROSS_THREAD_UNLOCK");
        return FALSE;
    }
}

//~!~lock mutex
MS_BOOL MsOS_DeleteNamedMutexbyIndex(MS_S32 u32Index)
{
    MS_S32 res = 0;

	if(_s32SemutexFD < 0)
    {
        MPOOL_WARN("%s: Lock mutex with invalid dev fd \n", __FUNCTION__);
        return FALSE;
    }

    if(u32Index < MUTEX_INDEX_BEGIN)
    {
        MPOOL_WARN("%s invalid mutex index \n", __FUNCTION__);
        return FALSE;
    }

    res = ioctl(_s32SemutexFD,MDRV_SEMUTEX_DEL_MUTEX,&u32Index);

    if(res == 0)
        return TRUE;
    else
        return FALSE;
}
#endif

//~!~create named semaphore
MS_S32 MsOS_CreateNamedSemaphore(char *ps8SemaName, MS_U32 u32SemaNum)
{
    CREATE_SEMA_ARG semaarg;
    MS_S32  s32Index;
    MS_U32  u32MaxLen;

    pthread_mutex_lock(&_MsOS_Openfd_Mutex);
    if(_s32SemutexFD < 0)
    {
        _s32SemutexFD = open("/dev/semutex", O_RDWR);

        if(0 > _s32SemutexFD)
        {
            MPOOL_ERROR("Open /dev/semutex fail, _s32SemutexFD = %d \n",_s32SemutexFD);
            MS_ASSERT(0);
            pthread_mutex_unlock(&_MsOS_Openfd_Mutex);
            return -1;
        }
    }
    pthread_mutex_unlock(&_MsOS_Openfd_Mutex);

    if (NULL == ps8SemaName)
        return -1;
    if (strlen((const char *)ps8SemaName) >= (SEMA_NAME_LEN-1))
    {
        MPOOL_WARN("%s: Warning strlen(%s) is longer than SEMA_NAME_LEN(%d). Oversize char will be discard.\n",
            __FUNCTION__, ps8SemaName, SEMA_NAME_LEN);
    }
    if (0 == (u32MaxLen = MIN(strlen((const char *)ps8SemaName), (SEMA_NAME_LEN-1))))
    {
        MPOOL_ERROR("%s: Input string is empty.\n", __FUNCTION__);
        return -1;
    }
    strncpy((char*)semaarg.semaname, (const char*)ps8SemaName, u32MaxLen);
    semaarg.semaname[u32MaxLen] = '\0';
    semaarg.semanum = u32SemaNum;

    s32Index = ioctl(_s32SemutexFD, MDRV_SEMUTEX_CREATE_SEMAPHORE, &semaarg);

    if(s32Index < SEMAPHORE_INDEX_BEGIN)
    {
        MPOOL_WARN("%s the returned mutex index %d invalid!!!\n", __FUNCTION__, s32Index);
        return -1;
    }

    return s32Index;
}

MS_BOOL MsOS_ResetNamedSemaphore(MS_S32 u32Index)
{
    MS_S32 res = 0;

    if(_s32SemutexFD < 0)
    {
        MPOOL_WARN("%s: Reset semaphore with invalid dev fd.\n", __FUNCTION__);
        return FALSE;
    }

    if(u32Index < SEMAPHORE_INDEX_BEGIN)
    {
        MPOOL_WARN("%s invalid semaphore index : u32Index = %d.\n", __FUNCTION__, u32Index);
        return FALSE;
    }

    res = ioctl(_s32SemutexFD, MDRV_SEMUTEX_SEMA_RESET, &u32Index);

    if(res == 0)
        return TRUE;
    else
        return FALSE;
}

MS_BOOL MsOS_ObtainNamedSemaphore(MS_S32 u32Index, MS_S32 flag)
{
    MS_S32 res = 0;

    if(_s32SemutexFD < 0)
    {
        MPOOL_WARN("%s: Obtain semaphore with invalid dev fd.\n", __FUNCTION__);
        return FALSE;
    }

    if(u32Index < SEMAPHORE_INDEX_BEGIN)
    {
        MPOOL_WARN("%s invalid semaphore index : u32Index = %d.\n", __FUNCTION__, u32Index);
        return FALSE;
    }

    if(flag == -1)
        res = ioctl(_s32SemutexFD, MDRV_SEMUTEX_SEMA_TRY_LOCK, &u32Index);
    else
    {
        res = -1;
        while(res == -1)
        {
            res = ioctl(_s32SemutexFD, MDRV_SEMUTEX_SEMA_LOCK, &u32Index);
            if(res == -1 && errno == EINTR)
                usleep(1000);
            else
            {
                MPOOL_ERROR("%s lock semaphore error: %d.\n", __FUNCTION__, errno);
                break;
            }
        }
    }

    if(res == 0)
        return TRUE;
    else
        return FALSE;
}

MS_BOOL MsOS_ReleaseNamedSemaphore(MS_S32 u32Index, MS_S32 flag)
{
    MS_S32 res = 0;

    if(_s32SemutexFD < 0)
    {
        MPOOL_WARN("%s: Obtain semaphore with invalid dev fd.\n", __FUNCTION__);
        return FALSE;
    }

    if(u32Index < SEMAPHORE_INDEX_BEGIN)
    {
        MPOOL_WARN("%s invalid semaphore index : u32Index = %d.\n", __FUNCTION__, u32Index);
        return FALSE;
    }

    res = ioctl(_s32SemutexFD, MDRV_SEMUTEX_SEMA_UNLOCK, &u32Index);

    if(res == 0)
        return TRUE;
    else
        return FALSE;
}


// @FIXME: support one pool at this stage
MS_BOOL MsOS_MPool_Get(void** ppAddrVirt, MS_U32* pu32AddrPhys, MS_U32* pu32Size, MS_BOOL bNonCache)
{
#if(!SUPPORT_PARTIAL_MAPPING)
    if(MPOOL_MAPPING) return TRUE; //we had already mapped by function MsOS_MPool_Mapping()

    DevMalloc_MPool_Info_t  stPoolInfo;
    MS_VIRT                  ptrAddrVirt;
    MS_U32            bCache = (bNonCache) ? 0: 1;
    MS_U32                  u32Idx = (bCache) ? 0: 1;

    if (NULL == _pAddrVirtStart[u32Idx])
    {
        if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_INFO, &stPoolInfo))
        {
            return FALSE;
        }
        if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_CACHE, &bCache))
        {
            return FALSE;
        }
        if ((MS_VIRT)MAP_FAILED == (ptrAddrVirt = (MS_VIRT)mmap(0, stPoolInfo.u32Size, PROT_READ | PROT_WRITE, MAP_SHARED, _s32MPoolFd, 0)))
        {
            MS_ASSERT(0);
            return FALSE;
        }

        _pAddrVirtStart[u32Idx] = (void*)ptrAddrVirt;
        _pAddrVirtEnd[u32Idx]   = (void*)(ptrAddrVirt + stPoolInfo.u32Size);
        _tAddrPhys[u32Idx]    = stPoolInfo.u32Addr;
        _u32Size[u32Idx]        = stPoolInfo.u32Size;
        _tVa2PaOff[u32Idx]    = (MS_SIZE)_pAddrVirtStart[u32Idx] - (MS_SIZE)_tAddrPhys[u32Idx];
    }

    if (ppAddrVirt)
    {
        *ppAddrVirt   = _pAddrVirtStart[u32Idx];
    }
    if (pu32AddrPhys)
    {
        *pu32AddrPhys = _tAddrPhys[u32Idx];
    }
    if (pu32Size)
    {
        *pu32Size     = _u32Size[u32Idx];
    }
#endif
    return TRUE;
}

static void _MPool_Check_aligned(MS_U64 u64Offset, MS_U64 u64MapSize)
{
    if(u64Offset&0xfff)
    {
        printf("!!! Important issue !!!\n");
        printf("The memory mapping address is not aligned, please make sure of that\n");
        MS_ASSERT(0);
		//assert(0);
    }

    if(u64MapSize&0xfff)
    {
        printf("!!! Important issue !!!\n");
        printf("The memory mapping size is not aligned, please make sure of that\n");
		MS_ASSERT(0);
        //assert(0);
    }

    return;
}

static MS_U8 _MPOOL_DELAY_BINDING(MS_S32 idx)
{
    DevMalloc_MPool_Info_t  stPoolInfo;
    MS_U64                  u64AddrVirt;
    MS_U32            bCache = (mpool_info[idx].bNonCache) ? 0: 1;

	MS_U8 	u8Check_MiuSel = 0;
	MS_U64 	u64Check_Offset = 0;

    stPoolInfo.u64Addr = mpool_info[idx].u64Phyaddr;
    stPoolInfo.u64Size = mpool_info[idx].u64MpoolSize;
    stPoolInfo.u64Interval = MIU_INTERVAL;
    stPoolInfo.u8MiuSel = mpool_info[idx].u8MiuSel;

	/* use u64Phyaddr to get MIU, offset */
	_phy_to_miu_offset(u8Check_MiuSel, u64Check_Offset, stPoolInfo.u64Addr);

	if(u8Check_MiuSel != stPoolInfo.u8MiuSel)
	{
		printf("\033[35mFunction = %s, Line = %d, [Error] miu_setting is wrong\033[m\n", __PRETTY_FUNCTION__, __LINE__);
		return FALSE;
	}
	stPoolInfo.u64Addr = u64Check_Offset;

    //prevent race condition cause mpool mapping size modified in the kernel layer
    pthread_mutex_lock(&_MsOS_MPool_Mutex);

    if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_SET, &stPoolInfo))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;
    }
    if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_CACHE, &bCache))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;
    }
    if ((MS_VIRT)MAP_FAILED == (u64AddrVirt = (MS_VIRT)mmap(0, mpool_info[idx].u64MpoolSize, PROT_READ | PROT_WRITE, MAP_SHARED, _s32MPoolFd, 0)))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        MS_ASSERT(0);
        return FALSE;
    }

    //prevent race condition cause mpool mapping size modified in the kernel layer
    pthread_mutex_unlock(&_MsOS_MPool_Mutex);

    mpool_info[idx].u64VirtStart = u64AddrVirt;
    mpool_info[idx].u64VirtEnd = (u64AddrVirt + mpool_info[idx].u64MpoolSize);

	mpool_info[idx].s32V2Poff = mpool_info[idx].u64VirtStart - mpool_info[idx].u64Phyaddr;

    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtStart =%tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtStart));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtEnd = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtEnd));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64Phyaddr = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64Phyaddr));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64MpoolSize = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64MpoolSize));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].s32V2Poff = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].s32V2Poff));

    return TRUE;
}

inline MS_PHY MsOS_MPool_VA2PA(MS_VIRT pAddrVirt)
{
    if (MPOOL_IS_CACHE_ADDR(pAddrVirt))
    {
        return (MS_PHY)HAL_MsOS_MPool_VA2PA(pAddrVirt,_tVa2PaOff[0]);
    }

    if (MPOOL_IS_UNCACHE_ADDR(pAddrVirt))
    {
       return (MS_PHY)HAL_MsOS_MPool_VA2PA(pAddrVirt,_tVa2PaOff[1]);
    }

    // we currently will go to here
    MS_S32 i;
    for(i = 0; i <MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed)
        {
            if(!((mpool_info[i].u64VirtStart <= pAddrVirt) && (pAddrVirt < mpool_info[i].u64VirtEnd)))
                continue;


            return (MS_PHY)HAL_MsOS_MPool_VA2PA(pAddrVirt, mpool_info[i].s32V2Poff);
        }
    }
	#if defined (__aarch64__)
    printf("ERROR!!! CAN'T SUCCESSFULLY CONVERT FROM VA(%lx) TO PA(%x)\n", pAddrVirt,0);
	#else
    printf("ERROR!!! CAN'T SUCCESSFULLY CONVERT FROM VA(%tx) TO PA(%x)\n", (ptrdiff_t)pAddrVirt,0);
	#endif
    return (MS_PHY) 0xFFFFFFFFUL;
}

inline MS_VIRT MsOS_MPool_PA2KSEG1(MS_PHY pAddrPhys) // un-cache
{
    MS_VIRT pAddrVirt;

    if (NULL == _pAddrVirtStart[1])
    {
        MS_S32 i;
        for(i = 0; i < MAX_MAPPINGSIZE;i ++)
        {
            if(!mpool_info[i].bIsUsed)
            {
                continue;   // due to unmap, we can not use break
            }
            if(!mpool_info[i].bNonCache)
            {
                continue;
            }

	        // we do PA_REGION check here, to check if pAddrPhys is located in mpool_info[i], to prevent we do mmap for some dynamic_mapping but not used
	        if(! ((mpool_info[i].u64Phyaddr <= pAddrPhys) && (pAddrPhys < (mpool_info[i].u64Phyaddr + mpool_info[i].u64MpoolSize))) )
	        {
	            continue;
	        }

            if(mpool_info[i].bIsDyn)
            {
                _MPOOL_MutexLock();
	            if((mpool_info[i].s32V2Poff == 0) && (mpool_info[i].u64VirtStart == 0))
                {
                    if(!_MPOOL_DELAY_BINDING(i))
                    {
						#if defined (__aarch64__)
                        printf("PA2KSEG1 delay binding fail!, pa = %lx", mpool_info[i].u64Phyaddr);
						#else
                        printf("PA2KSEG1 delay binding fail!, pa = %llx", mpool_info[i].u64Phyaddr);
						#endif
						_MPOOL_MutexUnlock();
			            return (MS_VIRT)NULL;
                    }
		        }
	            _MPOOL_MutexUnlock();
            }

            pAddrVirt = HAL_MsOS_MPool_PA2VA(pAddrPhys, mpool_info[i].u64Phyaddr, mpool_info[i].u64MpoolSize, mpool_info[i].s32V2Poff, ENABLE_PARTIAL_MAPPING);

            if(pAddrVirt != 0)
            {
                return pAddrVirt;
            }
        }

        MS_S32 idx;
        for(idx = 0; idx < MAX_MAPPINGSIZE; idx++)
        {
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtStart =%tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtStart));
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtEnd = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtEnd));
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64Phyaddr = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64Phyaddr));
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64MpoolSize = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64MpoolSize));
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].s32V2Poff = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].s32V2Poff));
        }
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1,printf("MsOS_MPool_PA2KSEG1 return NULL pAddrPhys =%llX\n", (unsigned long long)pAddrPhys));

        //assert(0);
        return (MS_VIRT) 0x0;
    }

    MPOOL_ASSERT(_tVa2PaOff[1], printf("MsOS_MPool_Init/MsOS_MPool_Get for un-cache have not involked yet\n"));

    pAddrVirt = HAL_MsOS_MPool_PA2VA(pAddrPhys, 0, 0xffffffff, _tVa2PaOff[0], DISABLE_PARTIAL_MAPPING);

    return pAddrVirt;
}

inline MS_VIRT MsOS_MPool_PA2KSEG0(MS_PHY pAddrPhys) // cache
{
    MS_VIRT pAddrVirt;

    if (NULL == _pAddrVirtStart[0])
    {
        MS_S32 i;
        for(i = 0; i < MAX_MAPPINGSIZE; i++)
        {
            if(!mpool_info[i].bIsUsed)
            {
                continue;   // due to unmap, we can not use break
            }
            if(mpool_info[i].bNonCache)
            {
                continue;
            }

    	    // we do PA_REGION check here, to check if pAddrPhys is located in mpool_info[i], to prevent we do mmap for some dynamic_mapping but not used
    	    if(! ((mpool_info[i].u64Phyaddr <= pAddrPhys) && (pAddrPhys < (mpool_info[i].u64Phyaddr + mpool_info[i].u64MpoolSize))) )
    	    {
    		    continue;
    	    }

            if(mpool_info[i].bIsDyn)
            {
    	        _MPOOL_MutexLock();
    	        if((mpool_info[i].s32V2Poff == 0) && (mpool_info[i].u64VirtStart == 0))
        	    {
        		    if(!_MPOOL_DELAY_BINDING(i))
    	            {
    	                printf("PA2KSEG0 delay binding fail!, pa = %tX", (ptrdiff_t)mpool_info[i].u64Phyaddr);

            			_MPOOL_MutexUnlock();
                        return (MS_VIRT)NULL;
    	            }
    	        }
                //mpool_info[i].bIsDyn = false    // joe, need to be discussed
	            _MPOOL_MutexUnlock();
            }

            pAddrVirt = HAL_MsOS_MPool_PA2VA(pAddrPhys, mpool_info[i].u64Phyaddr, mpool_info[i].u64MpoolSize, mpool_info[i].s32V2Poff, ENABLE_PARTIAL_MAPPING);

            if(pAddrVirt != 0)
            {
                return pAddrVirt;
            }
        }

        MS_S32 idx;
        for(idx = 0; idx < MAX_MAPPINGSIZE; idx++)
        {
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtStart = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtStart));
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtEnd = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtEnd));
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64Phyaddr = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64Phyaddr));
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64MpoolSize = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64MpoolSize));
			MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].s32V2Poff = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].s32V2Poff));
        }
        MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1,printf("MsOS_MPool_PA2KSEG0 return NULL\n"));
        //assert(0);
        return (MS_VIRT) 0x0;
    }

    MPOOL_ASSERT(_tVa2PaOff[0], printf("MsOS_MPool_Init/MsOS_MPool_Get for cache have not involked yet\n"));

    pAddrVirt = HAL_MsOS_MPool_PA2VA(pAddrPhys, 0, 0xffffffff, _tVa2PaOff[0], DISABLE_PARTIAL_MAPPING);

    return pAddrVirt;
}

inline MS_BOOL MsOS_MPool_Dcache_Flush_All()
{
    if (HAL_MsOS_MPool_Dcache_Flush_All(_s32MPoolFd) == FALSE)
    {
        printf("FAIL:%d\n",__LINE__);
        return FALSE;
    }

    return TRUE;
}

inline MS_BOOL MsOS_MPool_Dcache_Flush(MS_VIRT pAddrVirt, MS_SIZE tSize)
{
    MS_PHY pAddrPhys;

    pAddrPhys = MsOS_MPool_VA2PA(pAddrVirt);

    /* to prevent the va is a not mapped va ==> MsOS_MPool_VA2PA will return 0xFFFFFFFF */
    if(pAddrPhys == 0xFFFFFFFF)
    {
        printf("FAIL:%d\n",__LINE__);
        return FALSE;
    }

    /* to prevent the va_end is a not mapped va ==> MsOS_MPool_VA2PA will return 0xFFFFFFFF */
	if( MsOS_MPool_VA2PA(pAddrVirt + tSize) == 0xFFFFFFFF )
	{
		MsOS_MPool_Dcache_Flush_All();
		return TRUE;
	}

    if (HAL_MsOS_MPool_Dcache_Flush(_s32MPoolFd, pAddrVirt, tSize, pAddrPhys) == FALSE)
    {
        printf("FAIL:%d\n",__LINE__);
        return FALSE;
    }

    return TRUE;
}


MS_BOOL MsOS_MPool_Mapping(MS_U8 u8MiuSel, MS_SIZE tOffset, MS_SIZE tMapSize, MS_U8 u8MapMode) // 0:cache 1: noncache
{
    MS_S32 i,idx = 0;
    DevMalloc_MPool_Info_t  stPoolInfo;
    MS_VIRT                  ptrAddrVirt;
    MS_U32            bCache = 0; //(bNonCache) ? 0: 1;
    MS_BOOL     bNonCache =0;

    if(u8MapMode == MSOS_CACHE_BOTH || u8MapMode == MSOS_CACHE_USERSPACE || u8MapMode == MSOS_CACHE_KERNEL )
    {
        bNonCache = 0;
        bCache = 1;
    }else
    {
        bNonCache =1;
        bCache = 0;
    }

    if(u8MapMode == MSOS_CACHE_BOTH || u8MapMode == MSOS_NON_CACHE_BOTH || u8MapMode == MSOS_CACHE_USERSPACE || u8MapMode == MSOS_NON_CACHE_USERSPACE)
    {
    stPoolInfo.u64Addr = tOffset;
    stPoolInfo.u64Size = tMapSize;
    stPoolInfo.u64Interval = MIU_INTERVAL;
    stPoolInfo.u8MiuSel = u8MiuSel;

    _MPool_Check_aligned(tOffset, tMapSize);
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("u32Offset = %lX, u32MapSize = %lX, u8MapMode = %02x\n", (unsigned long)tOffset, (unsigned long)tMapSize, u8MapMode));

    pthread_mutex_lock(&_MsOS_MPool_Mutex);
    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed == false)
        {
            idx = i;
            break;
        }
    }
    if(i >= MAX_MAPPINGSIZE)
    {
        MPOOL_ERROR("Not enough MPool, must increase MAX_MAPPINGSIZE!!\n");
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;
    }
    //prevent race condition cause mpool mapping size modified in the kernel layer


    if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_SET, &stPoolInfo))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;
    }

    if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_CACHE, &bCache))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;
    }
    if ((MS_VIRT)MAP_FAILED == (ptrAddrVirt = (MS_VIRT)mmap(0, tMapSize, PROT_READ | PROT_WRITE, MAP_SHARED, _s32MPoolFd, 0)))
    {
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        MS_ASSERT(0);
        return FALSE;
    }
    mpool_info[idx].bIsUsed = true;
    if(bNonCache)
    {
        mpool_info[idx].bNonCache = MMAP_NONCACHE;
    }
    else
    {
        mpool_info[idx].bNonCache = MMAP_CACHE;
    }
    //prevent race condition cause mpool mapping size modified in the kernel layer
    pthread_mutex_unlock(&_MsOS_MPool_Mutex);

    mpool_info[idx].u64VirtStart = ptrAddrVirt;
    mpool_info[idx].u64VirtEnd = (ptrAddrVirt + tMapSize);
    mpool_info[idx].u8MiuSel = u8MiuSel;
    mpool_info[idx].bIsDyn = false;

    _miu_offset_to_phy(u8MiuSel, tOffset, mpool_info[idx].u64Phyaddr);

    mpool_info[idx].u64MpoolSize = tMapSize;
    mpool_info[idx].s32V2Poff = mpool_info[idx].u64VirtStart - mpool_info[idx].u64Phyaddr;

    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtStart = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtStart));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtEnd = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtEnd));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64Phyaddr = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64Phyaddr));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64MpoolSize = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64MpoolSize));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].s32V2Poff = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].s32V2Poff));

    MPOOL_MAPPING = 1;
    }

    #if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER)
    if(u8MapMode == MSOS_CACHE_USERSPACE || u8MapMode == MSOS_NON_CACHE_USERSPACE)
        return TRUE;


    MS_S32              _s32UtopiaFd = -1;
    KDrvMPool_Info_t    stPoolInfoKernel;

    if (0 > (_s32UtopiaFd = open("/proc/utopia", O_RDWR)))
    {
        printf("MPool : Open /proc/utopia fail\n");

    }else
    {
        stPoolInfoKernel.u32Addr = tOffset;
        stPoolInfoKernel.u32Size = tMapSize;
        stPoolInfoKernel.u32Interval = MIU_INTERVAL;
        stPoolInfoKernel.u8MiuSel = u8MiuSel;
        stPoolInfoKernel.bcached = !(bNonCache);

        if(ioctl(_s32UtopiaFd,UTOPIA_IOCTL_MODULE_DYNAMIC_MAPPING,&stPoolInfoKernel))
        {
            printf("MPool : UTOPIA_IOCTL_MODULE_DYNAMIC_MAPPING failed\n");

        }
        close(_s32UtopiaFd);//coverity resource leak
    }
    #endif

    return TRUE;
}

MS_BOOL MsOS_MPool_Mapping_Dynamic(MS_U8 u8MiuSel, MS_SIZE u32Offset, MS_SIZE u32MapSize, MS_U8 u8MapMode)
{
    MS_S32 i,idx=0;

    MS_BOOL     bNonCache =0;


    if(u8MapMode == MSOS_CACHE_BOTH || u8MapMode == MSOS_CACHE_USERSPACE || u8MapMode == MSOS_CACHE_KERNEL )
    {
        bNonCache = 0;
    }else
    {
        bNonCache =1;
    }
    #if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER)
    KDrvMPool_Info_t  stPoolInfo;

    MS_S32              _s32UtopiaFd = -1;
    if(u8MapMode == MSOS_CACHE_BOTH || u8MapMode == MSOS_NON_CACHE_BOTH || u8MapMode == MSOS_CACHE_KERNEL || u8MapMode == MSOS_NON_CACHE_KERNEL)
    {


    if (0 > (_s32UtopiaFd = open("/proc/utopia", O_RDWR)))
    {
        printf("MPool : Open /proc/utopia fail\n");

    }else
    {
        stPoolInfo.u32Addr = u32Offset;
        stPoolInfo.u32Size = u32MapSize;
        stPoolInfo.u32Interval = MIU_INTERVAL;
        stPoolInfo.u8MiuSel = u8MiuSel;
        stPoolInfo.bcached = !(bNonCache);

        if(ioctl(_s32UtopiaFd,UTOPIA_IOCTL_MODULE_DYNAMIC_MAPPING,&stPoolInfo))
        {
            printf("MPool : UTOPIA_IOCTL_MODULE_DYNAMIC_MAPPING failed\n");

        }
    }
    }
    #endif

    _MPool_Check_aligned(u32Offset, u32MapSize);
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("u32Offset = %tX, u32MapSize = %tX, u8MapMode = %02x\n", (ptrdiff_t)u32Offset, (ptrdiff_t)u32MapSize, u8MapMode));

    if(u8MapMode == MSOS_CACHE_BOTH || u8MapMode == MSOS_NON_CACHE_BOTH || u8MapMode == MSOS_CACHE_USERSPACE || u8MapMode == MSOS_NON_CACHE_USERSPACE)
    {

    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed == false)
        {
            idx = i;
            break;
        }
    }
    if(i >= MAX_MAPPINGSIZE)
    {
        MPOOL_ERROR("Not enough MPool, must increase MAX_MAPPINGSIZE!!\n");
        return FALSE;
    }

    mpool_info[idx].bIsUsed = true;
    if(bNonCache)
    {
        mpool_info[idx].bNonCache = MMAP_NONCACHE;
    }
    else
    {
        mpool_info[idx].bNonCache = MMAP_CACHE;
    }
    mpool_info[idx].u64VirtStart = 0;
    mpool_info[idx].u64VirtEnd = 0;
    mpool_info[idx].u8MiuSel = u8MiuSel;
    mpool_info[idx].bIsDyn = true;

    _miu_offset_to_phy(u8MiuSel, u32Offset, mpool_info[idx].u64Phyaddr);

    mpool_info[idx].u64MpoolSize = u32MapSize;
    mpool_info[idx].s32V2Poff = 0;

    MPOOL_MAPPING = 1;
    }
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("MPOOL_MAPPING =%d\n",MPOOL_MAPPING));//coverity MPOOL_MAPPING unused
#if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER)
    if(0 < _s32UtopiaFd)
        close(_s32UtopiaFd);//coverity resource leak
#endif
    return TRUE;
}

MS_BOOL MsOS_MPool_UnMapping(MS_VIRT ptrVirtStart, MS_SIZE tMapSize)
{
    MS_S32 i;

    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed == true)
        {
            if( (mpool_info[i].u64VirtStart == ptrVirtStart) && (mpool_info[i].u64MpoolSize == tMapSize) )
            {
                if( !munmap((MS_VIRT *)mpool_info[i].u64VirtStart, mpool_info[i].u64MpoolSize) )
                {
                	printf("mpool[%td] Unmapping PA:[%tX], VA:[%tX], Size:[%tX] done.\n", (ptrdiff_t)i, (ptrdiff_t)mpool_info[i].u64Phyaddr, (ptrdiff_t)mpool_info[i].u64VirtStart, (ptrdiff_t)mpool_info[i].u64MpoolSize);
					mpool_info[i].bIsUsed = false;
                    mpool_info[i].u64Phyaddr = 0;
                    mpool_info[i].u64VirtStart = 0;
                    mpool_info[i].u64VirtEnd = 0;
                    mpool_info[i].u64MpoolSize = 0;
                    mpool_info[i].s32V2Poff = 0;

                    /* cause default value is 0 */
                    mpool_info[i].bNonCache = 0;
                    mpool_info[i].u8MiuSel = 0;
                    mpool_info[i].bIsDyn = false;
                }
                else
                {
                    printf("Unmapping PA:[%tX], VA:[%tX], Size:[%tX] fail.\n", (ptrdiff_t)mpool_info[i].u64Phyaddr, (ptrdiff_t)mpool_info[i].u64VirtStart, (ptrdiff_t)mpool_info[i].u64MpoolSize);

                    break;
                }

                return TRUE;
            }
        }
    }
    printf("Unmapping Fail !! VA:[%tX], Size:[%tX].\n", (ptrdiff_t)ptrVirtStart, (ptrdiff_t)tMapSize);

    return FALSE;
}

MS_BOOL MsOS_MPool_Kernel_Detect(MS_PHY *lx_addr, MS_U64 *lx_size, MS_PHY *lx2_addr, MS_U64 *lx2_size)
{
    DevMalloc_MPool_Kernel_Info_t kernel_info;
    if (ioctl(_s32MPoolFd, MALLOC_IOC_MPOOL_KERNELDETECT, &kernel_info))
    {
        return FALSE;
    }

   *lx_addr = kernel_info.u64lxAddr;
   *lx_size = kernel_info.u64lxSize;
   *lx2_addr = kernel_info.u64lx2Addr-0x40000000;
   *lx2_size = kernel_info.u64lx2Size;

   //MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("lx_addr = %lx, lx_size = %lx\n", (MS_U64)lx_addr, lx_size));
   //MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("lx2_addr = %lx, lx2_size = %lx\n", (MS_U64)lx2_addr, lx2_size));

  return TRUE;
}

void  MsOS_MPool_SetDbgLevel(MsOSMPool_DbgLevel DbgLevel)
{
    _u32MPoolDBGLevel = DbgLevel;
}

MS_BOOL MsOS_MPool_SetWatchPT(MS_VIRT ptrAddrVirt, MS_U32 u32ASID, MS_U8 u8Global, MS_U8 u8WType, MS_U32 u32Mask)
{
	DevMalloc_MPool_Watchpt_Info_t stWPTInfo;
	stWPTInfo.mask = u32Mask;
	stWPTInfo.u64AddrVirt = ptrAddrVirt;
	stWPTInfo.ASID = u32ASID;
	stWPTInfo.global = u8Global;
	stWPTInfo.rwx = u8WType;
	if(ioctl(_s32MPoolFd, MALLOC_IOC_SETWATCHPT, &stWPTInfo)){
		printf("ioctl failed\n");
		return FALSE;
	}
	printf("this is api test after ioctl by york\n");
	return TRUE;
}

MS_BOOL MsOS_MPool_GetWatchPT(char *str)
{
	printf("Read watchpoint register\n");
	if(ioctl(_s32MPoolFd, MALLOC_IOC_GETWATCHPT, str)){
		printf("ioctl failed\n");
		return FALSE;
	}
	printf("%s\n",str);
	return TRUE;
}

MS_BOOL __attribute__((weak)) MsOS_MPool_Add_PA2VARange(MS_U64 u64PhysAddr, MS_VIRT u64VirtAddr, MS_SIZE u64MapSize, MS_BOOL bNonCache)
{
    MS_BOOL find = FALSE;
    MS_U64 u64AddrOffset = 0;
    MS_U8 u8MiuSel = 0;
    MS_U32 i, idx = 0;

    _MPool_Check_aligned(u64PhysAddr, u64MapSize);
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("u64PhysAddr = %tX, u64MapSize = %tX, u64VirtAddr = %tX\n", (ptrdiff_t)u64PhysAddr, (ptrdiff_t)u64MapSize, (ptrdiff_t)u64VirtAddr));

    //for multi-thread access
    pthread_mutex_lock(&_MsOS_MPool_Mutex);

    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if(mpool_info[i].bIsUsed == false)
        {
            mpool_info[i].bIsUsed = true;
            if(bNonCache)
                mpool_info[i].bNonCache = MMAP_NONCACHE;
            else
                mpool_info[i].bNonCache = MMAP_CACHE;

            idx = i;
            find = TRUE;

            break;
        }
    }

    if(!find)
    {
        printf("Not enough MPool, must increase MAX_MAPPINGSIZE!!\n");
        pthread_mutex_unlock(&_MsOS_MPool_Mutex);
        return FALSE;
    }

    _phy_to_miu_offset(u8MiuSel, u64AddrOffset, u64PhysAddr);   // get miu & offset
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("u64AddrOffset =0x%x\n",(unsigned int)u64AddrOffset));//coverity u64AddrOffset unused

    mpool_info[idx].u64VirtStart = u64VirtAddr;
    mpool_info[idx].u64VirtEnd = (u64VirtAddr + u64MapSize);
    mpool_info[idx].u8MiuSel = u8MiuSel;
    mpool_info[idx].bIsDyn = false;
    mpool_info[idx].u64Phyaddr = u64PhysAddr;
    mpool_info[idx].u64MpoolSize = u64MapSize;
    mpool_info[idx].s32V2Poff = mpool_info[idx].u64VirtStart - mpool_info[idx].u64Phyaddr;

    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtStart =%tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtStart));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64VirtEnd = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64VirtEnd));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64Phyaddr = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64Phyaddr));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].u64MpoolSize = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].u64MpoolSize));
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("mpool_info[%td].s32V2Poff = %tX\n", (ptrdiff_t)idx, (ptrdiff_t)mpool_info[idx].s32V2Poff));

    MPOOL_MAPPING = 1;
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("MPOOL_MAPPING =%d\n",MPOOL_MAPPING));//coverity MPOOL_MAPPING unused
    pthread_mutex_unlock(&_MsOS_MPool_Mutex);

    return TRUE;
}

MS_BOOL __attribute__((weak)) MsOS_MPool_Remove_PA2VARange(MS_U64 u64PhysAddr, MS_VIRT u64VirtAddr, MS_SIZE u64MapSize, MS_BOOL bNonCache)
{
    MS_BOOL ret = FALSE, mNonCache = MMAP_NONCACHE;
    MS_U32 i;

    _MPool_Check_aligned(u64PhysAddr, u64MapSize);
    MPOOL_DBG_MSG(E_MsOSMPool_DBG_L1, printf("u64PhysAddr = %tX, u64MapSize = %tX, u64VirtAddr = %tX\n", (ptrdiff_t)u64PhysAddr, (ptrdiff_t)u64MapSize, (ptrdiff_t)u64VirtAddr));

    if(bNonCache)
        mNonCache= MMAP_NONCACHE;
    else
        mNonCache= MMAP_CACHE;

    //multi-thread access
    pthread_mutex_lock(&_MsOS_MPool_Mutex);

    for (i = 0; i < MAX_MAPPINGSIZE; i++)
    {
        if((mpool_info[i].bIsUsed == false) || (mpool_info[i].bNonCache != mNonCache))
            continue;

        if((mpool_info[i].u64VirtStart == u64VirtAddr)
            && (mpool_info[i].u64Phyaddr == u64PhysAddr)
            && (mpool_info[i].u64MpoolSize == u64MapSize))
        {
             ret = TRUE;
             memset(&mpool_info[i], 0, sizeof(mpool_info[i]));
             break;
        }
    }

    pthread_mutex_unlock(&_MsOS_MPool_Mutex);
    return ret;
}
