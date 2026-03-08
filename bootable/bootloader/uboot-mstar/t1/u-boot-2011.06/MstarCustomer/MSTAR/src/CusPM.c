/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <command.h>
#include <common.h>
#include <ShareType.h>
#include <msAPI_Power.h>
#include <drvWDT.h>
#include <MsSystem.h>
#include <MsEnvironment.h>
#include <CusConfig.h>
#include <MsVfs.h>
#include <MsDebug.h>
#include <drvBDMA.h>
#include <MsApiPM.h>
#include <MsSysUtility.h>
#include <idme.h>
#if (CONFIG_ENABLE_WAKE_CEC)
#include <apiCEC.h>
#endif
#include <drvPM.h>
#include <MsUboot.h>
#include <MsMmap.h>
#include <MsDrvCache.h>
#if (ENABLE_MSTAR_NIKON==1) || (ENABLE_MSTAR_MILAN==1)
#ifndef  CUS_IR_HEAD_FILE//Please define it in board file for customization
#include "IR_MSTAR_DTV.h"
#else
#include CUS_IR_HEAD_FILE
#endif
#endif

#define BOOT_REASON_PM_ADDR_OFFSET		0x060a
#define PM_SPARE_COLD_BOOT_POWER_SUPPLY          0
#define PM_SPARE_WARM_BOOT_HW_WDOG               (1U << 1)
#define PM_SPARE_WARM_BOOT_KERNEL_WDOG	         (1U << 2)
#define PM_SPARE_WARM_BOOT_SW                    (1U << 3)
#define PM_SPARE_WARM_BOOT_KERNEL_PANIC          (1U << 4)
#define PM_SPARE_BOOT_LONG_PWR_KEY_PRESS     	 (1U << 5)
#define PM_SPARE_THERMAL_BOOT_SOC            	 (1U << 6)
#define PM_SPARE_SHUTDOWN_SW			 (1U << 7)
#define PM_SPARE_SCREEN_STATE			 (1U << 10)

#define BYPASS_SECONDARY_BOOT                    (1U << 2)

#define PM_REG_BASE             (0x0700 * 2)
#define REG_PM_DUMMY_WAKEUP_SOURCE             ((PM_REG_BASE + 0x39 * 2))
#define WKUP_SRC_SPECIAL 0x0F

static BOOLEAN g_skip_backlight = FALSE;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
BOOLEAN is_skip_backlight()
{
    printf("g_skip_backlight = %d \n", g_skip_backlight);
    return g_skip_backlight;
}
//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------

#if (ENABLE_MSTAR_NIKON==1) || (ENABLE_MSTAR_MILAN==1)
static int SetPm2L1(void);
#else
static int SetPm2Sram(void);
#endif
BOOLEAN get_poweroff_flag(void);
#if (CONFIG_WDT_RESET_BY_ESD)
static void set_poweroff_flag(BOOLEAN bEnable);
#endif
static int If_Boot_To_PM(void);
static BOOLEAN check_pm_standby(void);
static int PM51_PowerDown(void);
static unsigned int get_pm51_program_counter(void);

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

static int PM51_PowerDown(void)
{
    char *s = NULL;
    int iStrCrc = 0;

    UBOOT_TRACE("IN\n");
    if(IsHouseKeepingBootingMode()==TRUE)
    {
        // bring up 51
        if( IsBootingFromMaskRom() == TRUE )
        {
            #if (ENABLE_MSTAR_EDISON==1)
                UBOOT_DEBUG("=== PATCH FOR EDISON GTV ROMBOOT BUT PM IN SPI ===\n");
                msAPI_SetSPIOffsetForMCU();
            #elif (ENABLE_MSTAR_NIKON==1)
                UBOOT_DEBUG("=== NiKon ROMBOOT BUT PM IN L1 ===\n");
                return SetPm2L1();
            #elif (ENABLE_MSTAR_MILAN==1)
                UBOOT_DEBUG("=== Milan ROMBOOT BUT PM IN L1 ===\n");
                return SetPm2L1();
            #else
            UBOOT_DEBUG("== SetPm2Sram ==\n");
            if(SetPm2Sram()!=0)
            {
                UBOOT_ERROR("SetPm2Sram Fail!!\n");
                return -1;
            }
            #endif
        }
        else
        {
            msAPI_SetSPIOffsetForMCU();
        }
    }
    #if (CONFIG_MSTAR_RT_PM_IN_SPI)
    MDrv_PM_STR_CheckFactoryPowerOnModePassword();
    #endif

    if(strcmp(getenv("factory_poweron_mode"), "power_test") == 0)
    {
        s = getenv("str_crc");
        if(NULL != s)
        {
            iStrCrc = (int) simple_strtoul (s, NULL, 10);
            msAPI_Power_SetStrConfig(iStrCrc);
        }
        else
        {
            msAPI_Power_SetStrConfig(1);
        }
    }
    else
    {
        msAPI_Power_SetStrConfig(1);
    }

    //msAPI_Power_SetStrConfig( (strcmp(getenv("str_crc"), "0")==0) ? FALSE : TRUE ) ;
    msAPI_Power_PowerDown_EXEC();
    UBOOT_TRACE("OK\n");
    return 0;
}

#if (ENABLE_MSTAR_NIKON==1) || (ENABLE_MSTAR_MILAN==1)
extern BOOLEAN msAPI_KeyPad_Initialize(void);

static int SetPm2L1(void)
{
    U32 u32Addr=0;
    U32 u32AddrVA=0;
    U32 u32AddrNVA=0;
    char PMPath[CMD_BUF]="\0";
    UBOOT_TRACE("IN\n");
    //*(volatile U32*)(0xBF203DC4) = 0x0002;
#if (CONFIG_KEYPAD)
    msAPI_KeyPad_Initialize();
#endif
    static PM_WakeCfg PmWakeCfg =
        {
            .bPmWakeEnableIR = TRUE,
#if (CONFIG_KEYPAD)
            .bPmWakeEnableSAR = TRUE,
#else
            .bPmWakeEnableSAR = FALSE,
#endif
            .bPmWakeEnableGPIO0 = FALSE,
            .bPmWakeEnableGPIO1 = FALSE,
            .bPmWakeEnableUART1 = FALSE,
            .bPmWakeEnableSYNC = FALSE,
            .bPmWakeEnableESYNC = FALSE,

            .bPmWakeEnableRTC0 = TRUE,
            .bPmWakeEnableRTC1 = TRUE,
            .bPmWakeEnableDVI0 = FALSE,
            .bPmWakeEnableDVI2 = FALSE,
            .bPmWakeEnableCEC = TRUE,
            .bPmWakeEnableAVLINK = FALSE,

            .u8PmWakeIR =
            {   //IR wake-up key define
                IRKEY_POWER, 0x03, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF
            },

            .u8PmWakeIR2 =
            {   //IR wake-up key define
                IRKEY_POWER, 0x03, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF
            }
        };
    char* s = getenv(POWER_KEY_NAME);
    if (s)
    {
        PmWakeCfg.u8PmWakeIR[0] = simple_strtoul(s, NULL, 16);
    }

    flush_cache((MS_U32)&PmWakeCfg, sizeof(PM_WakeCfg));

    if(MDrv_PM_Init((PM_WakeCfg*)VA2PA((MS_U32) &PmWakeCfg))==0)
    {
        UBOOT_ERROR("MDrv_PM_Init fail !!!\n");
    }

    if(get_value_from_env(E_MMAP_ID_PM51_USAGE_MEM_ADR, NO_DEFAULT_MMAP_VALUE,&u32Addr)!=0)
    {
	    UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_PM51_USAGE_MEM_ADR);
        return -1;
    }

    if(vfs_mount(CONFIG)!= 0)
    {
        UBOOT_ERROR("vfs_mount fail\n");
        return -1;
    }
    snprintf(PMPath,sizeof(PMPath),"%s/PM.bin",CONFIG_PATH);
    u32AddrVA=PA2VA(u32Addr);
    u32AddrNVA=PA2NVA(u32Addr);

    if(vfs_read((void*)u32AddrVA,PMPath,0,0x10000) != 0)
    {
        UBOOT_ERROR("vfs_read fail !!! \n");
        return -1;
    }
    udelay(50);


    UBOOT_DEBUG("Load PM To DRAM PA[0x%x]VA[0x%x]NVA[0x%x] \n",u32Addr,u32AddrVA,u32AddrNVA);

    MsOS_DisableAllInterrupts();
    flush_cache(u32AddrVA,0x10000);
    Chip_Flush_Memory();

    enable_cache(0);

    ric_fill_icache((void *)(u32AddrNVA), 0x8000);
    ric_fill_dcache((void *)(u32AddrNVA+0x8000), 0x8000);
    //enable_cache(1); // Move to PM

    printf("[%s][%s] Start PM at :0x%x \n\n",__FILE__,__FUNCTION__, (u32AddrVA|0x400));
    console_init();
    udelay(50);

    asm volatile (
            "move $9, %[i0]\n\t" \
            "j $9\n\t" \
            "nop\n\t" \
            : \
            : [i0] "r"(u32AddrVA|0x400) \
            : "memory" \
    );
    while(1);

    UBOOT_TRACE("OK\n");
    return 0;
}

#else
static int SetPm2Sram(void)
{
    U32 u32Addr=0;

    char PMPath[CMD_BUF]="\0";
    UBOOT_TRACE("IN\n");

    if(get_value_from_env(E_MMAP_ID_PM51_USAGE_MEM_ADR, NO_DEFAULT_MMAP_VALUE,&u32Addr)!=0)
    {
	    UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_PM51_USAGE_MEM_ADR);
        return -1;
    }

    if(vfs_mount(CONFIG)!= 0)
    {
        UBOOT_ERROR("vfs_mount fail\n");
        return -1;
    }

    snprintf(PMPath,sizeof(PMPath),"%s/PM.bin",CONFIG_PATH);
    if(vfs_read((void*)PA2NVA(u32Addr),PMPath,0,CONFIG_PM_SIZE) != 0)
    {
        UBOOT_ERROR("vfs_read fail\n");
        return -1;
    }
    if(MDrv_BDMA_CopyHnd(u32Addr, 0x0, CONFIG_PM_SIZE, E_BDMA_SDRAM2SRAM1K_HK51, 0) != TRUE )
    {
        UBOOT_ERROR("MDrv_BDMA_CopyHnd fail\n");
        return -1;
    }
    MDrv_PM_SetSRAMOffsetForMCU();
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif



#if (CONFIG_WDT_RESET_BY_ESD)
static void set_poweroff_flag(BOOLEAN bEnable)///ac power off also use this flag -> dc_poweroff
{
    UBOOT_TRACE("IN\n");

#if (ENABLE_ENV_IN_NAND == 1)
    ////Do nothing because vfs_write is not implement now
#else
    BOOLEAN bOrgValue = FALSE;
    if (strcmp(getenv("dc_poweroff"), "1") == 0)
    {
        bOrgValue = TRUE;
    }

    if (bEnable != bOrgValue)
    {
        if(bEnable)
        {
            setenv("dc_poweroff", "1");
        }
        else
        {
            setenv("dc_poweroff", "0");
        }
        saveenv();
    }
#endif
    UBOOT_TRACE("OK\n");
}
#endif

static BOOLEAN check_pm_standby(void)
{
    BOOLEAN ret = FALSE;
    UBOOT_TRACE("IN\n");
	unsigned int boot_reason = Read2Byte(BOOT_REASON_PM_ADDR_OFFSET);
    unsigned short wakeup_reason = Read2Byte(REG_PM_DUMMY_WAKEUP_SOURCE);
	 if((boot_reason == PM_SPARE_BOOT_LONG_PWR_KEY_PRESS) &&
                (atoi(getenv("bootmode")) == IDME_BOOTMODE_DIAG))
    {
		return TRUE;
	}


    if( (getenv("dev_flags") != NULL) && ((atoi(getenv("dev_flags")))&BYPASS_SECONDARY_BOOT) )
         return FALSE;

    if((getenv("force_onetime_standby")!=NULL) && (strcmp(getenv("force_onetime_standby"), "1") == 0))
    {
         setenv("force_onetime_standby",NULL);
         setenv("MstarUpgrade_complete","1");
         saveenv();
         ret = TRUE;
    }
    else if(strcmp(getenv("factory_poweron_mode"), "power_test") == 0)
    {
         ret = TRUE;
    }
    //Secondary mode, AC on will always enter standby mode, except system is runing the SW upgrade.
    else if((strcmp(getenv("factory_poweron_mode"), "secondary") == 0) &&
            (strcmp(getenv("upgrade_mode"), "null")==0))
    {
	#if (CONFIG_ENABLE_WAKE_CEC)
	MS_CEC_INIT_INFO stMsCECInfo;
	stMsCECInfo.u32XTAL_CLK_Hz = 12000000UL;
	MApi_CEC_InitChip(&stMsCECInfo);
        MApi_CEC_ConfigWakeUp();
	#endif

	if(((boot_reason==PM_SPARE_COLD_BOOT_POWER_SUPPLY)||
        (boot_reason&PM_SPARE_SHUTDOWN_SW)) &&
		(atoi(getenv("bootmode"))==IDME_BOOTMODE_NORMAL))
	{
		printf("Enter PM standy mode \n");
		ret = TRUE;
	}
	else
	{
        if((boot_reason&PM_SPARE_SCREEN_STATE) && ((boot_reason&PM_SPARE_WARM_BOOT_HW_WDOG)||(boot_reason&PM_SPARE_WARM_BOOT_KERNEL_PANIC)))
		{
			printf("Screen is off when watch dog, kernel panic happens or power loss, boot to android silently \n");
            Write2Byte(REG_PM_DUMMY_WAKEUP_SOURCE, WKUP_SRC_SPECIAL);
            g_skip_backlight = TRUE;
            Write2Byte(BOOT_REASON_PM_ADDR_OFFSET, boot_reason&~PM_SPARE_SCREEN_STATE);
		}
        printf("Boot to android with screen on \n");
		ret = FALSE;
	}
    }
    else if(strcmp(getenv("factory_poweron_mode"), "memory") == 0) //Memory, DC off -> AC off -> AC on -> Standby
    {
        #if (CONFIG_MSTAR_RT_PM_IN_SPI)
        if(MDrv_PM_STR_CheckFactoryPowerOnMode_Second(1)!= 1)
        {
            return FALSE;
        }
        #endif
        ret = get_poweroff_flag();
    }

    UBOOT_TRACE("OK\n");
    return ret;
}

static unsigned int get_pm51_program_counter(void)
{
    unsigned int u32ProgramCounter = 0;
    UBOOT_TRACE("IN\n");
    u32ProgramCounter = *(volatile U32*)(MS_RIU_MAP+(0x10fe<<1));
    u32ProgramCounter = (u32ProgramCounter&0xff)<<16;
    u32ProgramCounter |= *(volatile U32*)(MS_RIU_MAP+(0x10fc<<1));
    UBOOT_TRACE("OK\n");
    return u32ProgramCounter;
}

static int If_Boot_To_PM(void)
{
    EN_POWER_ON_MODE ePowerMode=EN_POWER_DC_BOOT;
    UBOOT_TRACE("IN\n");

#if (WDT_STANDBY_MODE==1)
    MDrv_WDT_Init(E_WDT_DBGLV_ALL);

    if(MDrv_WDT_IsReset() && get_poweroff_flag())
    {

        MDrv_WDT_ClearRstFlag();
        return 0;
    }
#endif
    ePowerMode=msAPI_Power_QueryPowerOnMode();

    if ( EN_POWER_AC_BOOT == ePowerMode)
    {
        if( (getenv("factory_poweron_mode") == NULL) ||
            (strcmp(getenv("factory_poweron_mode"), "direct") == 0))  //change mode to secondary mode
        {
            setenv("factory_poweron_mode", "secondary");
            saveenv();
        }
        if(getenv("dc_poweroff") == NULL)
        {
            setenv("dc_poweroff", "0");
            saveenv();
        }

#if (CONFIG_WDT_RESET_BY_ESD)
        MS_BOOL bWDTResetToPowerOff = FALSE;
    #if (WDT_STANDBY_MODE == 0)
        MDrv_WDT_Init(E_WDT_DBGLV_ALL);
    #endif
        if(MDrv_WDT_IsReset())      // when we running ESD test, system will reset by wdt
        {
            UBOOT_DEBUG("System is reset by watchdog !!!\n");
            if(!get_poweroff_flag())
            {
                UBOOT_DEBUG("The last status is power on, system will not enter in standby mode\n");
                return 0;
            }
            else
            {
                UBOOT_DEBUG("The last status is power down, system will enter in standby mode\n");
                bWDTResetToPowerOff = TRUE;
            }
        }
#endif

#if (CONFIG_WDT_RESET_BY_ESD)
        if(bWDTResetToPowerOff || check_pm_standby())
#else
        if(check_pm_standby())
#endif
        {
            #if (CONFIG_WDT_RESET_BY_ESD)
            if (!bWDTResetToPowerOff)
            {
                set_poweroff_flag(TRUE);
            }
            #endif

            // load mtk bt patch , and send woble cmd
            #if (CONFIG_MTK_BT_USB)
            run_command("setMtkBT", 0);
            #endif

            // load broadcom bt patch , and configure woble
            #if (CONFIG_BRCM_BT_USB)
            int retry = 0, rtn = -1;
            while(retry < 3 && rtn == -1)
            {
                rtn = run_command("setBrcmBT", 0);
                retry++;
            }
            #endif

            run_command("led_red", 0);

            //PM51_PowerDown();
            run_command("pm51 standby",0);
        }

        else
        {
            //run_command("led_green", 0);
            //to do: patch need to remove
            #if(ENABLE_MSTAR_KENYA_BD_MST188A_D02A==1)
                WriteByte(0x1422,0x09);
                WriteByte(0x1423,0x09);
                WriteByte(0x1424,0x02);
            #endif
        }
    }

    else
    {
        //run_command("led_green", 0);
        //to do: patch need to remove
        #if(ENABLE_MSTAR_KENYA_BD_MST188A_D02A==1)
            WriteByte(0x1422,0x09);
            WriteByte(0x1423,0x09);
            WriteByte(0x1424,0x02);
        #endif
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

int do_if_boot_to_pm( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    If_Boot_To_PM();
    return 0;
}

int do_pm51( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret =0;
    char * u8Command = NULL;
    UBOOT_TRACE("IN\n");
    if (argc < 2 || argv[1]==NULL)
    {
        cmd_usage(cmdtp);
        return 0;
    }
    u8Command = argv[1];

    if(strncmp(u8Command, "stop", 4) == 0)
    {
        UBOOT_INFO("Stop PM51!\n");
        *(volatile U32*)(MS_RIU_MAP+(0x1018<<1))=0x0000;
    }
    else if(strncmp(u8Command, "start", 6) == 0)
    {
        UBOOT_INFO("Start PM51!\n");
        *(volatile U32*)(MS_RIU_MAP+(0x1018<<1))=0x000e;
    }
    else if(strncmp(u8Command, "readpc", 6) == 0)
    {
        UBOOT_INFO("PM51[PC]=0x%x\n",get_pm51_program_counter());
    }
    else if(strncmp(u8Command, "standby", 7) == 0)
    {
         ret = PM51_PowerDown();
    }
    UBOOT_TRACE("OK\n");
    return ret;
}


