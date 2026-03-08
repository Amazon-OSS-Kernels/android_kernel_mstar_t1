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

#include <exports.h>
#include <MsTypes.h>
#include <msAPI_Power.h>
#include <drvBDMA.h>
#include <drvPM.h>
#include <drvWDT.h>
#include <MsApiMiu.h>
#include <MsDebug.h>
#include <MsMmap.h>
#include <MsSystem.h>
#include <MsSysUtility.h>
#include <MsEnvironment.h>
#include <CusConfig.h>
#include <MsVfs.h>


extern int snprintf(char *str, size_t size, const char *fmt, ...);

#if (CONFIG_ENABLE_RTPM)
#define RUNTIME_PM_START_SIZE 0x10000
#define u32Dummy 0x0EA2
#if(CONFIG_MSTAR_URSA6_VB1)
#define URSA 0x2 
#elif(CONFIG_MSTAR_URSA9_VB1) 
#define URSA 0x4
#endif
int do_run_time_pm( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN");
    U32 u32Addr=0;
    char cmd[CMD_BUF] ="\0";
    memset(cmd,0,sizeof(cmd));    
    
#if(CONFIG_ENABLE_URSA_RTPM)
    Write2Byte(u32Dummy, URSA);
#endif

   if(get_value_from_env(E_MMAP_ID_PM51_USAGE_MEM_ADR, NO_DEFAULT_MMAP_VALUE,&u32Addr)!=0)
   {
       UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_PM51_USAGE_MEM_ADR);
       return -1;
   }

    UBOOT_DEBUG("=================================================\n");
    UBOOT_DEBUG("    u32Addr=0x%x\n",u32Addr);
    UBOOT_DEBUG("    RUNTIME_PM_START_SIZE=0x%x\n",RUNTIME_PM_START_SIZE);
    UBOOT_DEBUG("=================================================\n");

#if (CONFIG_MSTAR_RT_PM_IN_SPI)    
    MDrv_BDMA_CopyHnd(0x20000,u32Addr,RUNTIME_PM_START_SIZE,E_BDMA_FLASH2SDRAM,0);
#elif (CONFIG_MSTAR_RT_PM_IN_NAND)    
    snprintf(cmd, sizeof(cmd)-1, "nand read.e 0x%08lX RTPM 10000", (U32)(PA2NVA(u32Addr)));    
#elif (CONFIG_MSTAR_RT_PM_IN_EMMC)
    snprintf(cmd, sizeof(cmd)-1, "mmc read.p 0x%08lX RTPM 0x10000", (U32)(PA2NVA(u32Addr)));  
#else
    #error
#endif
    if(0 != run_command(cmd, 0))
    {
        UBOOT_ERROR("%s: '%s' fails, at %d\n", __func__, cmd, __LINE__);
        return -1;
    }    
    
    MsApi_RunTimePmProtect(u32Addr, (u32Addr+RUNTIME_PM_START_SIZE));
    MDrv_PM_SetDRAMOffsetForMCU(u32Addr);                     // Wake up DDR_PM    

	UBOOT_TRACE("OK\n");
    return 0;
}
#endif

MS_BOOL pm_check_back_ground_active(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_BOOL bActive = FALSE;
    if(E_PM_FAIL  == MDrv_PM_IsActiveStandbyMode(&bActive))
    {
        printf("\x1b[37;41m ===== [%s:%s:%d] MDrv_PM_IsBackGroundModeActive FAIL ===== \x1b[0m\n",__FILE__,__FUNCTION__,__LINE__);
    }
    if(bActive)
    {
        UBOOT_DEBUG(" Active Standby Mode TRUE =====\n");
    }
    else
    {
        UBOOT_DEBUG(" Active Standby Mode False =====\n");
    }
    //FOR TM14 STR Resume  
    if(E_PM_WAKEUPSRC_AVLINK==MDrv_PM_GetWakeupSource())
    {
        bActive = TRUE;
        UBOOT_DEBUG(" MDrv_PM_GetWakeupSource TRUE =====\n");

    }
    return bActive;
}

MS_BOOL get_poweroff_flag(void)
{
    ///ac power off also use this flag -> dc_poweroff
    BOOLEAN ret = FALSE;
    UBOOT_TRACE("IN\n");

#if (ENABLE_ENV_IN_NAND == 1)
    if(vfs_mount(CUSTOMER)!=-1)
    {
        char PathBuf[64] = "\0";
        snprintf(PathBuf, sizeof(PathBuf), "%s/dc_poweroff", CUSTOMER_PATH);
        if(vfs_getsize(PathBuf) > 0)
        {
            UBOOT_DEBUG("dc_poweroff is Ture\n");;
            ret = TRUE;
        }
        else
        {
            UBOOT_DEBUG("dc_poweroff is False\n");;
            ret = FALSE;
        }
    }
#else
    if(getenv("dc_poweroff") == NULL)
    {
        setenv("dc_poweroff", "0");
        saveenv();
    }
                
    if(strcmp(getenv("dc_poweroff"), "1") == 0)
    {
        ret = TRUE;
    }
#endif
    UBOOT_TRACE("OK\n");
    return ret;
}
