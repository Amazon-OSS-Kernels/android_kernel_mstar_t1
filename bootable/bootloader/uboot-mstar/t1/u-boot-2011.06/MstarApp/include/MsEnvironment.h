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

#ifndef __MS_ENVIRONMENT_H__
#define __MS_ENVIRONMENT_H__

//#include <environment.h>
#include "MsTypes.h"

typedef enum
{
  E_LX = 0,
  E_LX2,
  E_LX3,
  E_MAX,
}LX_NUM;


#if defined(ENABLE_MODULE_ENV_IN_SERIAL)
# if defined(CONFIG_SECURITY_BOOT)
#  ifndef  SEC_INFOR_SIZE
#   define SEC_INFOR_SIZE    0x10000
#  endif
#  ifndef  SEC_INFOR_ADDR
#   define SEC_INFOR_ADDR    (CONFIG_ENV_ADDR - SEC_INFOR_SIZE)
#  endif
#  ifndef  SEC_INFOR_ADDR_BAK
#   define SEC_INFOR_ADDR_BAK    (SEC_INFOR_ADDR - SEC_INFOR_SIZE)
#  endif
#endif


#if  defined(CONFIG_ENV_IS_IN_NAND)
# if  defined(CONFIG_SECURITY_BOOT)
#  ifndef  SEC_INFOR_BLOCK_NUM
#   define SEC_INFOR_BLOCK_NUM    0x3
#  endif
#  ifndef  SEC_INFOR_ADDR
#   define SEC_INFOR_ADDR    (CONFIG_ENV_ADDR - (SEC_INFOR_BLOCK_NUM * nand->erasesize))
#  endif
#  ifndef  SEC_INFOR_ADDR_BAK
#   define SEC_INFOR_ADDR_BAK    (SEC_INFOR_ADDR - (SEC_INFOR_BLOCK_NUM * nand->erasesize))
#  endif
# endif
# endif
#endif

#define MAX_MMAP_ID_LEN 34
#define ENV_CFG_PREFIX  "ENV="
#if (ENABLE_MODULE_ENV_IN_NAND==1)
#define ENV_CFG         "ENV=NAND"
#elif (ENABLE_MODULE_ENV_IN_UBI == 1)
#define ENV_CFG         "ENV=UBI"
#elif (ENABLE_MODULE_ENV_IN_SERIAL==1)
#define ENV_CFG         "ENV=SERIAL"
//#elif (CONFIG_COMMANDS & CONFIG_ENV_IN_SERIAL_512K)
//#define ENV_CFG         "ENV=SERIAL_512K"
#elif (ENABLE_MODULE_ENV_IN_MMC == 1)
#define ENV_CFG         "ENV=EMMC"
#else
#error "Wrong ENV_CFG settings!"
#endif

#define SECURITY_ENV_CFG_PREFIX  "SECURITY="
#if defined(CONFIG_SECURITY_BOOT)
#define SECURITY_ENV_CFG         "SECURITY=ON"
#else
#define SECURITY_ENV_CFG         "SECURITY=OFF"
#endif

#define INFO_EXCHANGE_CFG           "info_exchange"
#if (ENABLE_MODULE_INFO_IN_UBI_FILE == 1)
#define INFO_EXCHANGE_STORAGE       "ubifile"
#elif (ENABLE_MODULE_INFO_IN_SERIAL_ENV == 1)
#define INFO_EXCHANGE_STORAGE       "spi"
#elif (ENABLE_MODULE_INFO_IN_MMC_ENV == 1)
#define INFO_EXCHANGE_STORAGE       "emmc"
#else
#define INFO_EXCHANGE_STORAGE       "unknow"
#endif


MS_U32 Get_lzo_addr_Info(void);


int do_testmode_set(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_set_bootargs( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_del_boogargs_cfg (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_set_bootargs_cfg (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_sync_mmap_to_env( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_mmap_to_env( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_config2env( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_add_bootcheckpoints (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
//-------------------------------------------------------------------------------------------------
/// get value from environment if available otherwise set value to default value
/// @param id                                  \b IN: environment variable name
/// @param defval                              \b IN: default value
/// @param addr                                \b IN: address of variable to hold the return value
/// @return char *                             \b OUT: -1:FAIL, 0:Sucess
//-------------------------------------------------------------------------------------------------
int get_value_from_env(char *id, unsigned int defval, unsigned int *addr);

//-------------------------------------------------------------------------------------------------
/// Delet the member that exist in bootargs
/// For example: del_bootargs_cfg("autotest",0)
/// @param delCfg                           \b IN: the member that exist in the env 'bootargs'
////@param bDontSaveEnv                      \b IN: 0, saveenv , 1, don't saveenv
/// @return  int                              \b OUT: 0, delete sucessfully , -1, delete fail
//-------------------------------------------------------------------------------------------------
int del_bootargs_cfg(char *delCfg,MS_BOOL bDontSaveEnv);

//-------------------------------------------------------------------------------------------------
/// Edit a new member to bootargs
/// For example: set_bootargs_cfg("autotest","autotest=ture",0);
/// @param prefix_cfg                           \b IN: the new member's prefix
/// @param setCfg                               \b IN: the new member's content
////@param bDontSaveEnv                           \b IN: 0, saveenv , 1, don't saveenv
/// @return  int                                   \b OUT: 0, edit sucessfully , -1, edit fail
//-------------------------------------------------------------------------------------------------
int set_bootargs_cfg(char * prefix_cfg,char *setCfg,MS_BOOL bDontSaveEnv);


//-------------------------------------------------------------------------------------------------
/// Restore the envirnoment variable from supernova's mmap
/// @param id_mmap                           \b IN: the id  in mmap
/// @param env_addr                               \b IN: the name of addr in env
/// @param env_len                               \b IN: the name of len in env
/// @return  int                                   \b OUT: 0, edit sucessfully , -1, edit fail
/// example:
/// #define E_MMAP_ID_PM51_USAGE_MEM_AVAILABLE                     0x0000200000
/// id = "E_MMAP_ID_PM51_USAG"
/// env_addr="PM51_ADDR"
/// env_leng="PM51_LEN"
//-------------------------------------------------------------------------------------------------
int setMmapInfo2Env(char *id_mmap, char *env_addr, char *env_len);

//-------------------------------------------------------------------------------------------------
/// get address and size from env by input LX_ID
/// @INPUT: LX_ID                  \b IN/OUT: addr and size.
//-------------------------------------------------------------------------------------------------
MS_BOOL Get_LX_MemInfo(LX_NUM LxMem, unsigned int *addr, unsigned int *size);

//-------------------------------------------------------------------------------------------------
/// Set the boot time information to envirnoment variable 'bootargs'
/// @param none
/// @return  none
//-------------------------------------------------------------------------------------------------
void _boottime_set_to_env(void);

int get_value_from_env(char *id, unsigned int defval, unsigned int *addr);
#if CONFIG_RESCUE_ENV
int    saveenv_rescue     (void);
#endif

#endif
