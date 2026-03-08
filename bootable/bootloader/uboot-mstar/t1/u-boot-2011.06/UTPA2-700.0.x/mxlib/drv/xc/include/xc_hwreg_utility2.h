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
#ifndef _HWREG_UTILITY_H_
#define _HWREG_UTILITY_H_


#include "MsCommon.h"

//!! Do not include this header in driver or api level

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define XC_MUTEX
#define XC_MUTEX_DBG	0
#define SETWINDOW_MUTEX
#define SETWINDOW_MUTEX_DBG 0
#define XC_SEMAPHORE_DBG 0
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
extern MS_VIRT _XC_RIU_BASE;     // This should be inited before XC library starting.
extern MS_VIRT _PM_RIU_BASE;     // This should be inited before XC library starting.

extern MS_VIRT _HDCP_RIU_BASE;
extern MS_VIRT _DDC_RIU_BASE;

#define _BITMASK(loc_msb, loc_lsb) ((1U << (loc_msb)) - (1U << (loc_lsb)) + (1U << (loc_msb)))
#define BITMASK(x) _BITMASK(1?x, 0?x)
#define HBMASK    0xFF00
#define LBMASK    0x00FF

#define RIU_MACRO_START     do {
#define RIU_MACRO_END       } while (0)

// Address bus of RIU is 16 bits.
#define RIU_READ_BYTE(addr)         ( READ_BYTE( _XC_RIU_BASE + (addr) ) )
#define RIU_READ_2BYTE(addr)        ( READ_WORD( _XC_RIU_BASE + (addr) ) )
#define RIU_WRITE_BYTE(addr, val)   { WRITE_BYTE( _XC_RIU_BASE + (addr), val) }
#define RIU_WRITE_2BYTE(addr, val)  { WRITE_WORD( _XC_RIU_BASE + (addr), val) }

// Address bus of RIU is 16 bits for PM //alex_tung
#define PM_RIU_READ_BYTEM(addr)        ( READ_BYTE( _PM_RIU_BASE + (addr) ) )
#define PM_RIU_READ_2BYTE(addr)        ( READ_WORD( _PM_RIU_BASE + (addr) ) )
#define PM_RIU_WRITE_BYTE(addr, val)   { WRITE_BYTE( _PM_RIU_BASE + (addr), val) }
#define PM_RIU_WRITE_2BYTE(addr, val)  { WRITE_WORD( _PM_RIU_BASE + (addr), val) }

//Address bus of RIU for HDCP for T3
#define HDCP_RIU_READ_BYTE(addr)         ( READ_BYTE( _HDCP_RIU_BASE + (addr) ) )
#define HDCP_RIU_READ_2BYTE(addr)        ( READ_WORD( _HDCP_RIU_BASE + (addr) ) )
#define HDCP_RIU_WRITE_BYTE(addr, val)   { WRITE_BYTE( _HDCP_RIU_BASE + (addr), val) }
#define HDCP_RIU_WRITE_2BYTE(addr, val)  { WRITE_WORD( _HDCP_RIU_BASE + (addr), val) }

//Address bus of RIU for DDC for T3
#define DDC_RIU_READ_1BYTE(addr)         ( READ_BYTE( _HDCP_RIU_BASE + (addr) ) )
#define DDC_RIU_READ_2BYTE(addr)         ( READ_WORD( _HDCP_RIU_BASE + (addr) ) )
#define DDC_RIU_WRITE_1BYTE(addr, val)   { WRITE_BYTE( _HDCP_RIU_BASE + (addr), val) }
#define DDC_RIU_WRITE_2BYTE(addr, val)   { WRITE_WORD( _HDCP_RIU_BASE + (addr), val) }

#define _XC_SELECT_INTERNAL_VARIABLE(INST_ID) MDrv_XC_Resource_Mapping(INST_ID,E_XC_POOL_ID_INTERNAL_VARIABLE)
#define _XC_SELECT_INTERNAL_REGISTER(INST_ID) MDrv_XC_Resource_Mapping(INST_ID,E_XC_POOL_ID_INTERNAL_REGISTER)
#define _XC_SELECT_INTERNAL_MENULOAD(INST_ID) MDrv_XC_Resource_Mapping(INST_ID,E_XC_POOL_ID_INTERNAL_MENULOAD)

#define _dbg_semaphore E_XC_ID_VAR
// if anyone wants to see log inside android,
// please replace the "printf" with "ALOGE" so that it can be printed in android process
#if (XC_SEMAPHORE_DBG)
    #define _XC_SEMAPHORE_ENTRY(pInstance,eID_TYPE)                                                              \
            if (eID_TYPE == _dbg_semaphore) \
                printf("1 [%s,%5d]==========================Prepare to get semaphore, ID = %d\n",__func__,__LINE__,eID_TYPE);      \
            if(MDrv_XC_Get_Semaphore(pInstance,eID_TYPE) != UTOPIA_STATUS_SUCCESS)                               \
            {                                                                                         \
                printf("[%s,%5d] Get XC Semaphore failed\n",__FUNCTION__,__LINE__);                   \
            }                                                                                          \
            if (eID_TYPE == _dbg_semaphore) \
                printf("[%s,%5d]Semaphore Got\n\n",__func__,__LINE__);

    #define _XC_SEMAPHORE_RETURN(pInstance,eID_TYPE)                                                             \
            if (eID_TYPE == _dbg_semaphore) \
                printf("0 [%s,%5d]==========================prepare to release semaphore, ID = %d\n",__func__,__LINE__,eID_TYPE);  \
            MDrv_XC_Release_Semaphore(pInstance,eID_TYPE);                                                             \
            if (eID_TYPE == _dbg_semaphore) \
                printf("[%s,%5d]Semaphore Returned\n\n",__func__,__LINE__);

#else
    #define _XC_SEMAPHORE_ENTRY(pInstance,eID_TYPE)  \
        if(MDrv_XC_Get_Semaphore(pInstance,eID_TYPE) != UTOPIA_STATUS_SUCCESS)                               \
            {      }

    #define _XC_SEMAPHORE_RETURN(pInstance,eID_TYPE)                                                             \
                MDrv_XC_Release_Semaphore(pInstance,eID_TYPE);
#endif


#define _XC_MUTEX_TIME_OUT                 MSOS_WAIT_FOREVER

// Mutex function
#define _XC_ENTRY(pInstance)     _XC_ENTRY_MUTEX(pInstance,_XC_Mutex)
#define _XC_RETURN(pInstance)    _XC_RETURN_MUTEX(pInstance,_XC_Mutex)

#ifdef XC_MUTEX
    #if(XC_MUTEX_DBG)
        #define _XC_ENTRY_MUTEX(pInstance,_mutex_)                                                                  \
                _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_REG);                                               \
                printf("1,==========================Prepare to get mutex\n");                             \
                printf("[%s][%s][%06d]\n",__FILE__,__FUNCTION__,__LINE__);    \
                if(!MsOS_ObtainMutex(_mutex_, _XC_MUTEX_TIME_OUT))                     \
                {                                                                        \
                    printf("==========================\n");                              \
                    printf("[%s][%s][%06d] Mutex taking timeout\n",__FILE__,__FUNCTION__,__LINE__);    \
                }
        #define _XC_RETURN_MUTEX(pInstance,_mutex_)                                                                \
                printf("0,==========================prepare to release mutex\n");                        \
                printf("[%s][%s][%06d] \n",__FILE__,__FUNCTION__,__LINE__);    \
                MsOS_ReleaseMutex(_mutex_);                                                              \
                _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_REG);
                                    //return _ret;
    #else
        #define _XC_ENTRY_MUTEX(pInstance,_mutex_)                                                                  \
                _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_REG);                                  \
                if(!MsOS_ObtainMutex(_mutex_, _XC_MUTEX_TIME_OUT))                     \
                {                                                                        \
                }
        #define _XC_RETURN_MUTEX(pInstance,_mutex_)                                               \
                MsOS_ReleaseMutex(_mutex_);                                             \
                _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_REG);
                                    //return _ret;
    #endif
#else // #if not def XC_MUTEX
    #define _XC_ENTRY_MUTEX(_mutex_)     while(0)
    #define _XC_RETURN_MUTEX(_mutex_)    while(0)
#endif // #ifdef _XC_MUTEX

#define _SETWINDOW_MUTEX_TIME_OUT           MSOS_WAIT_FOREVER

// Mutex function
#define _SETWINDOW_ENTRY()     _SETWINDOW_ENTRY_MUTEX(_SetWindow_Mutex)
#define _SETWINDOW_RETURN()    _SETWINDOW_RETURN_MUTEX(_SetWindow_Mutex)

#ifdef SETWINDOW_MUTEX
    #if(SETWINDOW_MUTEX_DBG)
        #define _SETWINDOW_ENTRY_MUTEX(_mutex_)                                                                  \
                printf("1,==========================\n");                              \
                printf("[%s][%s][%06d]\n",__FILE__,__FUNCTION__,__LINE__);    \
                if(!MsOS_ObtainMutex(_mutex_, _SETWINDOW_MUTEX_TIME_OUT))                     \
                {                                                                        \
                    printf("==========================\n");                              \
                    printf("[%s][%s][%06d] Mutex taking timeout\n",__FILE__,__FUNCTION__,__LINE__);    \
                }
        #define _SETWINDOW_RETURN_MUTEX(_mutex_)                                                                \
                printf("0,==========================\n");                              \
                printf("[%s][%s][%06d] \n",__FILE__,__FUNCTION__,__LINE__);    \
                MsOS_ReleaseMutex(_mutex_);
                                    //return _ret;
    #else
        #define _SETWINDOW_ENTRY_MUTEX(_mutex_)                                                                  \
                if(!MsOS_ObtainMutex(_mutex_, _SETWINDOW_MUTEX_TIME_OUT))                     \
                {                                                                        \
                }
        #define _SETWINDOW_RETURN_MUTEX(_mutex_)    MsOS_ReleaseMutex(_mutex_);
                                    //return _ret;
    #endif
#else // #if not def SETWINDOW_MUTEX
    #define _SETWINDOW_ENTRY_MUTEX(_mutex_)     while(0)
    #define _SETWINDOW_RETURN_MUTEX(_mutex_)    while(0)
#endif // #ifdef _SETWINDOW_MUTEX

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
#ifdef MVIDEO_C
#define INTERFACE
INTERFACE MS_S32            _XC_Mutex = -1;
INTERFACE MS_S32            _XC_ISR_Mutex = -1;
INTERFACE MS_S32            _SetWindow_Mutex = -1;
#ifdef MSOS_TYPE_LINUX_KERNEL
INTERFACE wait_queue_head_t _XC_EventQueue;
INTERFACE MS_U32            _XC_EventFlag = 0;
INTERFACE MS_BOOL           _XC_VSyncRun = FALSE;
INTERFACE MS_U32            _XC_VSyncCount = 0;
INTERFACE MS_U32            _XC_VSyncMax = 0;
#endif
#else
#define INTERFACE extern
INTERFACE MS_S32            _XC_Mutex;
INTERFACE MS_S32            _XC_ISR_Mutex;
INTERFACE MS_S32            _SetWindow_Mutex;
#ifdef MSOS_TYPE_LINUX_KERNEL
INTERFACE wait_queue_head_t _XC_EventQueue;
INTERFACE MS_U32            _XC_EventFlag;
INTERFACE MS_BOOL           _XC_VSyncRun;
INTERFACE MS_U32            _XC_VSyncCount;
INTERFACE MS_U32            _XC_VSyncMax;
#endif
#endif

INTERFACE MS_U32 _XC_Device_Offset[2];//MAX_XC_DEVICE_NUM

//=============================================================
// Standard Form

#define MDrv_ReadByte( u32Reg )   RIU_READ_BYTE(((u32Reg) << 1) - ((u32Reg) & 1))

#define MDrv_Read2Byte( u32Reg )    (RIU_READ_2BYTE((u32Reg)<<1))

#define MDrv_Read4Byte( u32Reg )   ( (MS_U32)RIU_READ_2BYTE((u32Reg)<<1) | ((MS_U32)RIU_READ_2BYTE(((u32Reg)+2)<<1)<<16 )  )

#define MDrv_ReadRegBit( u32Reg, u8Mask )   (RIU_READ_BYTE(((u32Reg)<<1) - ((u32Reg) & 1)) & (u8Mask))

#define MDrv_WriteRegBit( u32Reg, bEnable, u8Mask )                                     \
    RIU_MACRO_START                                                                     \
    RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) , (bEnable) ? (RIU_READ_BYTE(  (((u32Reg) <<1) - ((u32Reg) & 1))  ) |  (u8Mask)) :                           \
                                (RIU_READ_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) ) & ~(u8Mask)));                            \
    RIU_MACRO_END

#define MDrv_WriteByte( u32Reg, u8Val )                                                 \
    RIU_MACRO_START                                                                     \
    RIU_WRITE_BYTE(((u32Reg) << 1) - ((u32Reg) & 1), u8Val);   \
    RIU_MACRO_END

#define MDrv_Write2ByteMask( u32Reg, u16Val, u16Mask) \
    RIU_MACRO_START                                     \
    RIU_WRITE_2BYTE(u32Reg<<1,   (RIU_READ_2BYTE(u32Reg<<1) & ~(u16Mask)) | (u16Val & u16Mask)) \
    RIU_MACRO_END

#define MDrv_Write2Byte( u32Reg, u16Val )                                               \
    RIU_MACRO_START                                                                     \
    if ( ((u32Reg) & 0x01) )                                                        \
    {                                                                               \
        RIU_WRITE_BYTE(((u32Reg) << 1) - 1, (MS_U8)((u16Val)));                                  \
        RIU_WRITE_BYTE(((u32Reg) + 1) << 1, (MS_U8)((u16Val) >> 8));                             \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        RIU_WRITE_2BYTE( ((u32Reg)<<1) ,  u16Val);                                                       \
    }                                                                               \
    RIU_MACRO_END

#define MDrv_Write3Byte( u32Reg, u32Val )   \
    RIU_MACRO_START                         \
    if ((u32Reg) & 0x01)                                                                \
    {                                                                                               \
        RIU_WRITE_BYTE((u32Reg << 1) - 1, u32Val);                                    \
        RIU_WRITE_2BYTE( (u32Reg + 1)<<1 , ((u32Val) >> 8));                                      \
    }                                                                                           \
    else                                                                                        \
    {                                                                                               \
        RIU_WRITE_2BYTE( (u32Reg) << 1,  u32Val);                                                         \
        RIU_WRITE_BYTE( (u32Reg + 2) << 1 ,  ((u32Val) >> 16));                             \
    }                           \
    RIU_MACRO_END

#define MDrv_Write4Byte( u32Reg, u32Val )                                               \
    RIU_MACRO_START                                                                     \
    if ((u32Reg) & 0x01)                                                      \
    {                                                                                               \
        RIU_WRITE_BYTE( ((u32Reg) << 1) - 1 ,  u32Val);                                         \
        RIU_WRITE_2BYTE( ((u32Reg) + 1)<<1 , ( (u32Val) >> 8));                                      \
        RIU_WRITE_BYTE( (((u32Reg) + 3) << 1) ,  ((u32Val) >> 24));                           \
    }                                                                                               \
    else                                                                                                \
    {                                                                                                   \
        RIU_WRITE_2BYTE( (u32Reg) <<1 ,  u32Val);                                                             \
        RIU_WRITE_2BYTE(  ((u32Reg) + 2)<<1 ,  ((u32Val) >> 16));                                             \
    }                                                                     \
    RIU_MACRO_END

#define MDrv_WriteByteMask( u32Reg, u8Val, u8Msk )                                      \
    RIU_MACRO_START                                                                     \
    RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)), (RIU_READ_BYTE((((u32Reg) <<1) - ((u32Reg) & 1))) & ~(u8Msk)) | ((u8Val) & (u8Msk)));                   \
    RIU_MACRO_END




//=============================================================
// Just for Scaler
#if ENABLE_REGISTER_SPREAD

#define SC_W2BYTE( u32Id, u32Reg, u16Val)\
            CHECK_XC_SWDS_ENABLE_START\
            ({if((((u32Reg) & 0xFFFF) >> 8) >= _XC_Device_Offset[1])\
              {\
                   RIU_WRITE_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFFFF)) << 1 , u16Val  ) ; \
              }\
              else\
              {\
                   RIU_WRITE_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFFFF) + (_XC_Device_Offset[u32Id] << 8) ) << 1 , u16Val  ) ; \
              }})\
            CHECK_XC_SWDS_ENABLE_END

#define SC_R2BYTE( u32Id, u32Reg ) \
            ( { ((((u32Reg) & 0xFFFF) >> 8) >= _XC_Device_Offset[1])\
            ? RIU_READ_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFFFF) ) << 1 )  \
            : RIU_READ_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFFFF) + (_XC_Device_Offset[u32Id] << 8) ) << 1 ); } )

#define SC_W4BYTE( u32Id, u32Reg, u32Val)\
            CHECK_XC_SWDS_ENABLE_START\
            ({if((((u32Reg) & 0xFFFF) >> 8) >= _XC_Device_Offset[1])\
              {\
                   RIU_WRITE_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFFFF) ) <<1, (MS_U16)((u32Val) & 0x0000FFFF) ) ;  \
                   RIU_WRITE_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFFFF) + 2 ) << 1, (MS_U16)(((u32Val) >> 16) & 0x0000FFFF) );\
              }\
              else\
              {\
                   RIU_WRITE_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFFFF) + (_XC_Device_Offset[u32Id] << 8) ) <<1, (MS_U16)((u32Val) & 0x0000FFFF) ) ;  \
                   RIU_WRITE_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFFFF) + (_XC_Device_Offset[u32Id] << 8) + 2 ) << 1, (MS_U16)(((u32Val) >> 16) & 0x0000FFFF) );\
              }})\
            CHECK_XC_SWDS_ENABLE_END

#define SC_R4BYTE( u32Id, u32Reg )\
            ( { ((((u32Reg) & 0xFFFF) >> 8) >= _XC_Device_Offset[1])\
            ? RIU_READ_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFFFF)) << 1 ) | (MS_U32)(RIU_READ_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFFFF) + 2 ) << 1 )) << 16 \
            : RIU_READ_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFFFF) + (_XC_Device_Offset[u32Id] << 8) ) << 1 ) | (MS_U32)(RIU_READ_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFFFF) + (_XC_Device_Offset[u32Id] << 8) + 2 ) << 1 )) << 16 ; } )

#define SC_R2BYTEMSK( u32Id, u32Reg, u16mask)\
            ( { ((((u32Reg) & 0xFFFF) >> 8) >= _XC_Device_Offset[1])\
            ? RIU_READ_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFFFF) ) << 1 ) & (u16mask)\
            : RIU_READ_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFFFF) + (_XC_Device_Offset[u32Id] << 8) ) << 1 ) & (u16mask) ; } )

#define SC_W2BYTEMSK( u32Id, u32Reg, u16Val, u16Mask)\
            CHECK_XC_SWDS_ENABLE_START\
            ({if((((u32Reg) & 0xFFFF) >> 8) >= _XC_Device_Offset[1])\
              {\
                   RIU_WRITE_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFFFF) ) << 1, (RIU_READ_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFFFF) ) << 1) & ~(u16Mask) ) | ((u16Val) & (u16Mask)) ) ;\
              }\
              else\
              {\
                   RIU_WRITE_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFFFF) + (_XC_Device_Offset[u32Id] << 8) ) << 1, (RIU_READ_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFFFF) + (_XC_Device_Offset[u32Id] << 8) ) << 1) & ~(u16Mask) ) | ((u16Val) & (u16Mask)) ) ; \
              }})\
            CHECK_XC_SWDS_ENABLE_END

#else
#define SC_W2BYTE( u32Id, u32Reg, u16Val)\
            CHECK_XC_SWDS_ENABLE_START\
            ({if((((u32Reg) & 0xFFFF) >> 8) >= _XC_Device_Offset[1])\
              {\
                   RIU_WRITE_2BYTE(REG_SCALER_BASE << 1, ((u32Reg) >> 8) & 0x00FF ) ; \
                   RIU_WRITE_2BYTE( (REG_SCALER_BASE +((u32Reg) & 0xFF) ) << 1 , u16Val  ) ;\
              }\
              else\
              {\
                   RIU_WRITE_2BYTE(REG_SCALER_BASE << 1, (((u32Reg) >> 8) & 0x00FF) + _XC_Device_Offset[u32Id] ) ; \
                   RIU_WRITE_2BYTE( (REG_SCALER_BASE +((u32Reg) & 0xFF) ) << 1 , u16Val  ) ;\
              }})\
            CHECK_XC_SWDS_ENABLE_END

#define SC_R2BYTE( u32Id, u32Reg ) \
            ({if((((u32Reg) & 0xFFFF) >> 8) >= _XC_Device_Offset[1])\
              {\
                   RIU_WRITE_2BYTE(REG_SCALER_BASE << 1, ( (u32Reg) >> 8) & 0x00FF ) ; \
              }\
              else\
              {\
                   RIU_WRITE_2BYTE(REG_SCALER_BASE << 1, (( (u32Reg) >> 8) & 0x00FF) + _XC_Device_Offset[u32Id] ) ; \
              }\
                RIU_READ_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFF) )<<1 ) ; } )

#define SC_W4BYTE( u32Id, u32Reg, u32Val)\
            CHECK_XC_SWDS_ENABLE_START\
            ({if((((u32Reg) & 0xFFFF) >> 8) >= _XC_Device_Offset[1])\
              {\
                   RIU_WRITE_2BYTE(REG_SCALER_BASE<<1, ((u32Reg) >> 8) & 0x00FF) ; \
                   RIU_WRITE_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFF) ) <<1, (MS_U16)((u32Val) & 0x0000FFFF) ) ; \
                   RIU_WRITE_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFF) + 2 ) << 1, (MS_U16)(((u32Val) >> 16) & 0x0000FFFF) );\
              }\
              else\
              {\
                   RIU_WRITE_2BYTE(REG_SCALER_BASE<<1, (((u32Reg) >> 8) & 0x00FF) + _XC_Device_Offset[u32Id] ) ; \
                   RIU_WRITE_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFF) ) <<1, (MS_U16)((u32Val) & 0x0000FFFF) ) ; \
                   RIU_WRITE_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFF) + 2 ) << 1, (MS_U16)(((u32Val) >> 16) & 0x0000FFFF) );\
              }})\
            CHECK_XC_SWDS_ENABLE_END

#define SC_R4BYTE( u32Id, u32Reg )\
            ({if((((u32Reg) & 0xFFFF) >> 8) >= _XC_Device_Offset[1])\
              {\
                   RIU_WRITE_2BYTE(REG_SCALER_BASE << 1, ((u32Reg) >> 8) & 0x00FF ) ; \
              }\
              else\
              {\
                   RIU_WRITE_2BYTE(REG_SCALER_BASE << 1, (((u32Reg) >> 8) & 0x00FF) + _XC_Device_Offset[u32Id] ) ; \
              }\
                RIU_READ_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFF) ) << 1 ) | (MS_U32)(RIU_READ_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFF) + 2 ) << 1 )) << 16; } )

#define SC_R2BYTEMSK( u32Id, u32Reg, u16mask)\
            ({if((((u32Reg) & 0xFFFF) >> 8) >= _XC_Device_Offset[1])\
              {\
                   RIU_WRITE_2BYTE(REG_SCALER_BASE << 1, ((u32Reg) >> 8) & 0x00FF ) ;  \
              }\
              else\
              {\
                   RIU_WRITE_2BYTE(REG_SCALER_BASE << 1, (((u32Reg) >> 8) & 0x00FF) + _XC_Device_Offset[u32Id] ) ; \
              }\
                RIU_READ_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFF) ) << 1) & (u16mask) ; } )

#define SC_W2BYTEMSK( u32Id, u32Reg, u16Val, u16Mask)\
            CHECK_XC_SWDS_ENABLE_START\
            ({if((((u32Reg) & 0xFFFF) >> 8) >= _XC_Device_Offset[1])\
              {\
                   RIU_WRITE_2BYTE(REG_SCALER_BASE << 1, ((u32Reg) >> 8) & 0x00FF ) ; \
                   RIU_WRITE_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFF) ) << 1, (RIU_READ_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFF) ) << 1) & ~(u16Mask) ) | ((u16Val) & (u16Mask)) ) ;\
              }\
              else\
              {\
                   RIU_WRITE_2BYTE(REG_SCALER_BASE << 1, (((u32Reg) >> 8) & 0x00FF) + _XC_Device_Offset[u32Id] ) ; \
                   RIU_WRITE_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFF) ) << 1, (RIU_READ_2BYTE( (REG_SCALER_BASE + ((u32Reg) & 0xFF) ) << 1) & ~(u16Mask) ) | ((u16Val) & (u16Mask)) ) ;\
              }})\
            CHECK_XC_SWDS_ENABLE_END

#endif

//=============================================================
// Just for MOD

#if ENABLE_REGISTER_SPREAD
#define MOD_W2BYTE( u32Reg, u16Val)\
            ( { RIU_WRITE_2BYTE( (REG_MOD_BASE + ((u32Reg) & 0xFFFF)) << 1 , u16Val  ) ; } )

#define MOD_R2BYTE( u32Reg ) \
            ( { RIU_READ_2BYTE( (REG_MOD_BASE + ((u32Reg) & 0xFFFF)) <<1 ) ; } )

#define MOD_R2BYTEMSK( u32Reg, u16mask)\
            ( { RIU_READ_2BYTE( (REG_MOD_BASE + ((u32Reg) & 0xFFFF)) << 1) & (u16mask) ; } )

#define MOD_W2BYTEMSK( u32Reg, u16Val, u16Mask)\
            ( { RIU_WRITE_2BYTE( (REG_MOD_BASE + ((u32Reg) & 0xFFFF)) << 1, (RIU_READ_2BYTE( (REG_MOD_BASE + ((u32Reg) & 0xFFFF) ) << 1) & ~(u16Mask) ) | ((u16Val) & (u16Mask)) ) ; })

#define PWM_R2BYTEMSK( u32Reg, u16mask)\
            ( { RIU_READ_2BYTE( (REG_PWM_BASE + ((u32Reg) & 0xFFFF)) << 1) & (u16mask) ; } )

#else
#define MOD_W2BYTE( u32Reg, u16Val)\
            ( { RIU_WRITE_2BYTE(REG_MOD_BASE << 1, ((u32Reg) >> 8) & 0x00FF ); \
                RIU_WRITE_2BYTE( (REG_MOD_BASE + ((u32Reg) & 0xFF) ) << 1, u16Val ); } )

#define MOD_R2BYTE( u32Reg ) \
            ( { RIU_WRITE_2BYTE(REG_MOD_BASE << 1, ((u32Reg) >> 8) & 0x00FF ); \
                RIU_READ_2BYTE( (REG_MOD_BASE + ((u32Reg) & 0xFF) ) << 1) ; } )

#define MOD_R2BYTEMSK( u32Reg, u16mask)\
            ( { RIU_WRITE_2BYTE(REG_MOD_BASE << 1, ((u32Reg) >> 8) & 0x00FF ); \
                RIU_READ_2BYTE( (REG_MOD_BASE + ((u32Reg) & 0xFF) ) << 1) & (u16mask); })

#define MOD_W2BYTEMSK( u32Reg, u16Val, u16Mask)\
            ( { RIU_WRITE_2BYTE(REG_MOD_BASE << 1, ((u32Reg) >> 8) & 0x00FF ); \
                RIU_WRITE_2BYTE( (REG_MOD_BASE + ((u32Reg) & 0xFF) )<<1 , (RIU_READ_2BYTE( (REG_MOD_BASE + ((u32Reg) & 0xFF) ) << 1 ) & ~(u16Mask)) | ((u16Val) & (u16Mask)) ); } )
#endif
/// new MOD bank for MOD_A, and original is fo MOD_D, from Monaco
#define MOD_A_W2BYTE( u32Reg, u16Val)\
            ( { RIU_WRITE_2BYTE( (REG_MOD_A_BASE + ((u32Reg) & 0xFF) ) << 1, u16Val ); } )

#define MOD_A_R2BYTE( u32Reg ) \
            ( { RIU_READ_2BYTE( (REG_MOD_A_BASE + ((u32Reg) & 0xFF) ) << 1) ; } )

#define MOD_A_R2BYTEMSK( u32Reg, u16mask)\
            ( { RIU_READ_2BYTE( (REG_MOD_A_BASE + ((u32Reg) & 0xFF) ) << 1) & (u16mask); })

#define MOD_A_W2BYTEMSK( u32Reg, u16Val, u16Mask)\
            ( { RIU_WRITE_2BYTE( (REG_MOD_A_BASE + ((u32Reg) & 0xFF) )<<1 , (RIU_READ_2BYTE( (REG_MOD_A_BASE + ((u32Reg) & 0xFF) ) << 1 ) & ~(u16Mask)) | ((u16Val) & (u16Mask)) ); } )


//=============================================================
//General ( Make sure u32Reg is not ODD
#define W2BYTE( u32Reg, u16Val) RIU_WRITE_2BYTE( (u32Reg) << 1 , u16Val )


#define R2BYTE( u32Reg ) RIU_READ_2BYTE( (u32Reg) << 1)

#define W4BYTE( u32Reg, u32Val)\
            ( { RIU_WRITE_2BYTE( (u32Reg) << 1, ((u32Val) & 0x0000FFFF) ); \
                RIU_WRITE_2BYTE( ( (u32Reg) + 2) << 1 , (((u32Val) >> 16) & 0x0000FFFF)) ; } )

#define R4BYTE( u32Reg )\
            ( { ((RIU_READ_2BYTE( (u32Reg) << 1)) | ((MS_U32)(RIU_READ_2BYTE( ( (u32Reg) + 2 ) << 1) ) << 16)) ; } )

#define R2BYTEMSK( u32Reg, u16mask)\
            ( { RIU_READ_2BYTE( (u32Reg)<< 1) & (u16mask) ; } )

#define W2BYTEMSK( u32Reg, u16Val, u16Mask)\
            ( { RIU_WRITE_2BYTE( (u32Reg)<< 1 , (RIU_READ_2BYTE((u32Reg) << 1) & ~(u16Mask)) | ((u16Val) & (u16Mask)) ) ; } )

#define W3BYTE( u32Reg, u32Val)\
            ( { RIU_WRITE_2BYTE( (u32Reg) << 1,  u32Val); \
                RIU_WRITE_BYTE( (u32Reg + 2) << 1 ,  ((u32Val) >> 16)); } )


//=============================================================
// Just for PM registers

/*
    R/W register forced to use 8 bit address, everytime need to r/w 2 bytes with mask

    32bit address     16 bit address        8 bit address
        0                   0                       0
        1                   x                       1
        2
        3
        4                   1                       2
        5                   x                       3
        6
        7
        8                   2                       4
        9                   x                       5
        A
        B                                                                                  */

// to read 0x2F03[3], please use R1BYTE(0x2F03, 3:3)
#define PM_R1BYTE(u32Addr, u8mask)            \
    (READ_BYTE (_PM_RIU_BASE + (u32Addr << 1) - (u32Addr & 1)) & BMASK(u8mask))

// to write 0x2F02[4:3] with 2'b10, please use W1BYTE(0x2F02, 0x02, 4:3)
#define PM_W1BYTE(u32Addr, u8Val, u8mask)     \
    (WRITE_BYTE(_PM_RIU_BASE + (u32Addr << 1) - (u32Addr & 1), (PM_R1BYTE(u32Addr, 7:0) & ~BMASK(u8mask)) | (BITS(u8mask, u8Val) & BMASK(u8mask))))
#if 1//alex_tung
// access PM registers
// u32Addr must be 16bit aligned
#define PM_R2BYTE( u32Reg, u16mask)\
            ( PM_RIU_READ_2BYTE( (u32Reg)<< 1) & u16mask )
    // u32Addr must be 16bit aligned
#define PM_W2BYTE( u32Reg, u16Val, u16Mask)\
            (  PM_RIU_WRITE_2BYTE( (u32Reg)<< 1 , (PM_RIU_READ_2BYTE((u32Reg) << 1) & ~(u16Mask)) | ((u16Val) & (u16Mask)) )  )
#else
// u32Addr must be 16bit aligned
#define PM_R2BYTE(u32Addr, u16mask)            \
    (READ_WORD (_PM_RIU_BASE + (u32Addr << 1)) & BMASK(u16mask))

// u32Addr must be 16bit aligned
#define PM_W2BYTE(u32Addr, u16Val, u16mask)    \
    (WRITE_WORD(_PM_RIU_BASE + (u32Addr << 1), (PM_R2BYTE(u32Addr, 15:0) & ~BMASK(u16mask)) | (BITS(u16mask, u16Val) & BMASK(u16mask))))
#endif
//=============================================================
// Just for HDCP registers for T3
#define HDCP_W2BYTE( u32Reg, u16Val) HDCP_RIU_WRITE_2BYTE( (u32Reg) << 1 , u16Val )

#define HDCP_R2BYTE( u32Reg )  HDCP_RIU_READ_2BYTE( (u32Reg) << 1)

#define HDCP_R2BYTEMSK( u32Reg, u16mask)\
            ( { HDCP_RIU_READ_2BYTE( (u32Reg)<< 1) & u16mask ; } )

#define HDCP_W2BYTEMSK( u32Reg, u16Val, u16Mask)\
            ( { HDCP_RIU_WRITE_2BYTE( (u32Reg)<< 1 , (HDCP_RIU_READ_2BYTE((u32Reg) << 1) & ~(u16Mask)) | ((u16Val) & (u16Mask))) ; } )

//=============================================================
//for DDC registers for T3

#define DDC_WRITE( u32Reg, u16Val) DDC_RIU_WRITE_2BYTE( (u32Reg) << 1 , u16Val )

#define DDC_READ( u32Reg )  DDC_RIU_READ_2BYTE( (u32Reg) << 1)
#define DDC_READ_MASK( u32Reg, u16mask)\
            ( { DDC_RIU_READ_2BYTE( (u32Reg)<< 1) & u16mask ; } )
#define DDC_WRITE_MASK( u32Reg, u16Val, u16Mask)\
            ( { DDC_RIU_WRITE_2BYTE( (u32Reg)<< 1 , (DDC_RIU_READ_2BYTE((u32Reg) << 1) & ~(u16Mask)) | ((u16Val) & (u16Mask))) ; } )



//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------






#undef INTERFACE

#endif

