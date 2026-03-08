/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <stdio.h>
#include <stddef.h>
#include <exports.h>
#include <MsCmdTable.h>
#include <config.h>
#include <mstarstr.h>
#include <MsBoot.h>
#include <ShareType.h>
#include <drvPM.h>
#if CONFIG_RESCUE_ENV  && CONFIG_RESCUE_ENV_IR_TRIGGER
#include <MsSystem.h>
#endif

#if (CONFIG_URSA_UNION)
#include <ursa/ursa_common.h>
#endif

#ifdef UFBL_FEATURE_IDME
#include <idme.h>
#endif

void Customer_Register_Process(void)
{
    if(is_str_resume())
    {
        Add_Command_Table("cpu", 0, STAGE_PROCESS);

        #if (!(CONFIG_LOGO_STORE_IN_MBOOT) && !(CONFIG_LOGO_STORE_NOT_SET))
        Add_Command_Table ("dbtable_init" , 0, STAGE_PROCESS);
        #endif

        Add_Command_Table ("updatemiureg" , 0, STAGE_PROCESS);

        #if defined (CONFIG_SECURITY_BOOT)
        Add_Command_Table ("SecureInit" , 0, STAGE_PROCESS);  //should run before frcup
        #endif

        #if(ENABLE_URSA_8==1)
        Add_Command_Table ("InitUrsa8" , 0, STAGE_PROCESS);
        #endif

        Add_Command_Table ("panel_pre_init" , 0, STAGE_PROCESS);

        //Add_Command_Table ("led_green" , 0, STAGE_PROCESS);

        #if(ENABLE_XC_Init==1)
		#if (CONFIG_URSA_UNION)
		if ((MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U9) || (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U11))
		#endif
        {
            Add_Command_Table ("xc_init" , 0, STAGE_PROCESS);
        }
        #endif

        #if(ENABLE_CVBS_OUT==1)
        Add_Command_Table ("cvbs" , 0, STAGE_PROCESS);
        #endif

        #if(ENABLE_HDMI_TX == 1)
        Add_Command_Table ("hdmi init" , 0, STAGE_PROCESS);
        #endif

        #if(ENABLE_URSA_8==1)
        Add_Command_Table ("ursa8_lvds_on" , 0, STAGE_PROCESS);
        #endif
        if((E_PM_WAKEUPSRC_RTC != MDrv_PM_GetWakeupSource()))
        {
             #ifdef CONFIG_DISPLAY_LOGO
             char *BootlogoFile = NULL;
             char BootlogoCmd[CMD_BUF]  = {0};
             BootlogoFile = getenv("BootlogoFile");
             if( BootlogoFile==NULL )
             {
                 strncpy(BootlogoCmd,"bootlogo",CMD_BUF);
             }
             else
             {
                 char *p = strchr(BootlogoFile, ';');
                 if (p)
                     // Terminate the string to avoid injected commands
                     *p = '\0';

                 snprintf(BootlogoCmd, CMD_BUF, "bootlogo 0 0 1 1 %s", BootlogoFile);
             }
             char* opt_logo = NULL;
             opt_logo = getenv("logo");
             if(opt_logo != NULL)
             {
                 int logo_on = 0;
                 logo_on = simple_strtol(opt_logo, NULL, 10);
                 if(logo_on > 0 && logo_on < 3)
                 {
                     Add_Command_Table (BootlogoCmd , 0, STAGE_PROCESS);
                     #if defined (CONFIG_URSA_6M40)
                     Add_Command_Table ("ursa_lvds_on" , 0, STAGE_PROCESS);
                     #endif
                     #if defined (CONFIG_URSA_8)
                     Add_Command_Table ("ursa8_lvds_on" , 0, STAGE_PROCESS);
                     #endif
                 }
             }
             else
             {
                 Add_Command_Table (BootlogoCmd , 0, STAGE_PROCESS);
                 #if defined (CONFIG_URSA_6M40)
                 Add_Command_Table ("ursa_lvds_on" , 0, STAGE_PROCESS);
                 #endif
             }
             #endif
        }
        #if (CONFIG_URSA12_VB1_FRC_BRINGUP)
        Add_Command_Table ("frcup",0,STAGE_PROCESS);
        #endif
        #if defined (CONFIG_URSA_6M40)
        Add_Command_Table("ursa_lvds_on", 0, STAGE_PROCESS);
        Add_Command_Table("ursa_osd_unmute", 0, STAGE_PROCESS);
        #endif

        #if defined (CONFIG_TEE)
        #if defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)|| defined(CONFIG_ARMv7_OPTEE)
        Add_Command_Table ("readArmFw;readOptee" , 0, STAGE_TOKERNEL);
        #else
        #if (ENABLE_MSTAR_R2_STR == 1)
        Add_Command_Table ("r2resume" ,0, STAGE_PROCESS);
        #else
        Add_Command_Table ("readNuttx" ,0, STAGE_PROCESS);
        Add_Command_Table ("bootNuttx" ,0, STAGE_PROCESS);
        #endif
        #endif
        #endif

        #if (CONFIG_MUNICH_PNL_EN == 0)
        #if (CONFIG_ENABLE_RTPM)
        Add_Command_Table ("runtime_pm" , 0, STAGE_PROCESS);
        #endif
        #endif
        #if defined (CONFIG_TEE)
        #if !defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE) && !defined(CONFIG_ARMv7_OPTEE) && !defined(CONFIG_MSTAR_R2_STR)
        Add_Command_Table ("wait_tee_ready" ,0, STAGE_PROCESS);
        #endif
        #endif
        if((E_PM_WAKEUPSRC_RTC != MDrv_PM_GetWakeupSource()))
        {
            Add_Command_Table("panel_post_init", 0,STAGE_PROCESS);
        }
        //ursa6
        #if defined(CONFIG_URSA6_VB1) || defined(CONFIG_URSA_UNION)
        #if defined(CONFIG_URSA_UNION)
        if (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U6)
        #endif
        {
            Add_Command_Table ("send_I2C_cmd_to_ursa6" , 0, STAGE_PROCESS);
        }
        #endif

        //ursa9
        #if defined(CONFIG_URSA9_VB1) || defined(CONFIG_URSA_UNION)
        #if defined(CONFIG_URSA_UNION)
        if ((MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U9) || (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U11))
        #endif
        {
            Add_Command_Table ("send_I2C_cmd_to_ursa9" , 0, STAGE_PROCESS);
        }
        #endif

        #if (CONFIG_LOCAL_DIMMING)
        Add_Command_Table("local_dimming", 0,STAGE_PROCESS);
        #endif

        Add_Command_Table ("checkstr" , 0, STAGE_PROCESS);
    }


    #if defined (CONFIG_URSA_6M40)
    Add_Command_Table("ursa_lvds_off", 0, STAGE_PROCESS);
    Add_Command_Table("delay 50", 0, STAGE_PROCESS);
    #if !defined(CONFIG_ENABLE_4K2K_PANEL)
    Add_Command_Table("ursa_2k_mode", 0, STAGE_PROCESS);
    #endif
    #endif

    #if ENABLE_SSC
       Add_Command_Table("MIU_SSC", 0, STAGE_PROCESS);
    #endif

    Add_Command_Table("cpu", 0, STAGE_PROCESS);

    #if defined (CONFIG_MTK_WIFI_7668)
    Add_Command_Table("wifi_Reset", 0, STAGE_PROCESS);
    #endif

     //Register customter Process
    #if defined(CONFIG_LOAD_ENV_FROM_SN) || defined(CONFIG_Customer)
    Add_Command_Table ("envload" , 0, STAGE_PROCESS);
    #endif

    #if defined (CONFIG_MBOOT_VERSION)
    Add_Command_Table ("mbootver_set" , 0, STAGE_PROCESS);
    #endif

    #if(ENABLE_URSA_8==1)
    Add_Command_Table ("InitUrsa8" , 0, STAGE_PROCESS);
    #endif

    #if(ENABLE_MODULE_BOOT_IR==1)
    Add_Command_Table ("ir_delay" , 0, STAGE_PROCESS);//for IR detect delay
    #endif


    //************************************************************
    // original from App_Register_Process()
    //************************************************************
    #if CONFIG_RESCUE_ENV
    #if CONFIG_RESCUE_ENV_AUTO_TRIGGER
    Add_Command_Table ("BrickTerminator" , 0, STAGE_PROCESS);
    #else
    #if CONFIG_RESCUE_ENV_IR_TRIGGER
    {
        unsigned int u32Value = 0;
        char *pEnv = getenv(BRICK_TERMINATOR_IR_TRIGGERED_STRING);
        if(NULL != pEnv)
        {
            u32Value = (unsigned int)simple_strtoul(pEnv, NULL, 10);
            if(1 == u32Value)
            {
                Add_Command_Table ("BrickTerminator" , 0, STAGE_PROCESS);
            }
        }
    }
    #endif //CONFIG_RESCUE_ENV_IR_TRIGGER
    #endif //CONFIG_RESCUE_ENV_AUTO_TRIGGER
    #endif //CONFIG_RESCUE_ENV

    #ifdef CONFIG_ENABLE_SYNC_MMAP
    Add_Command_Table ("sync_mmap" , 0, STAGE_PROCESS);
    #endif

    #if (!(CONFIG_LOGO_STORE_IN_MBOOT) && !(CONFIG_LOGO_STORE_NOT_SET))
    Add_Command_Table ("dbtable_init" , 0, STAGE_PROCESS);
    #endif

    Add_Command_Table ("updatemiureg" , 0, STAGE_PROCESS);

    #if defined (CONFIG_SECURITY_BOOT)
    Add_Command_Table ("SecureInit" , 0, STAGE_PROCESS);
    #endif

    #if (ENABLE_UTEST == 1)
    Add_Command_Table ("utest" , 0, STAGE_PROCESS);
    #endif

    #ifdef CONFIG_DISPLAY_VERSIONINFO
    Add_Command_Table ("mversion" , 0, STAGE_PROCESS);
    #endif

    #ifdef CONFIG_BOOT2_WRITEPROTECT
    Add_Command_Table ("mmc writeprotect.boot2" , 0, STAGE_PROCESS);
    #endif

    Add_Command_Table ("bootargs_set" , 0, STAGE_PROCESS);
    Add_Command_Table ("config2env" , 0, STAGE_PROCESS);
    Add_Command_Table ("wdt_enable 0", 0, STAGE_PROCESS);
}


//you can type "showtb 0" in mboot console to see registered cmd.
void Customer_Register_ToKernel(void)
{
    //Some flow must run after MBoot console entry to avoid MBoot halt.

   /*bootcheck:check boot mode(usb/oad/net upgrade, recovery, normal).
     If it is normal mode, just go through this command.
     you can mark this to implement your customer boot flow. */
    Add_Command_Table ("bootcheck" , 0, STAGE_TOKERNEL);

    Add_Command_Table ("wdt_enable" , 0, STAGE_TOKERNEL);// wdt_enable need to place in bootcheck's next cmd.


    Add_Command_Table ("if_boot_to_pm" , 0, STAGE_TOKERNEL);

    // if_boot_to_pm run system into power down，we will not init panel, in order to save power.
    // so we should plase panel_pre_init after if_boot_to_pm
    #ifdef CONFIG_POWER_MUSIC
    Add_Command_Table ("audio_preinit" , 0, STAGE_TOKERNEL);
    #endif

    #if (CONFIG_URSA12_VB1_FRC_BRINGUP)
    Add_Command_Table ("frcup",0,STAGE_TOKERNEL);
    #endif
    #if (CONFIG_PANEL_INIT)
    Add_Command_Table ("panel_pre_init" , 0, STAGE_TOKERNEL);
    #endif


    //**********************************************************************
    // original from App_Register_ToKernel()
    //**********************************************************************
	#if (CONFIG_MUNICH_PNL_EN)
	#if (CONFIG_ENABLE_RTPM)
    Add_Command_Table ("runtime_pm" , 0, STAGE_TOKERNEL);
	#endif
	#endif

    #if defined(CONFIG_ENABLE_4K2K_PANEL)
    #if defined(CONFIG_INX_NOVA_VB1)
    Add_Command_Table("inx_panel_set_init", 0, STAGE_TOKERNEL);
    #endif
    #endif

    Add_Command_Table("unlockcmi", 0, STAGE_TOKERNEL);

    #if (CONFIG_DOLBY)
    Add_Command_Table ("dolby_init" , 0, STAGE_TOKERNEL);
    #endif

    #if(ENABLE_XC_Init==1)
    #if (CONFIG_URSA_UNION)
    if ((MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U9) || (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U11))
    #endif
    {
        Add_Command_Table ("xc_init" , 0, STAGE_TOKERNEL);
    }
    #endif

    #if(ENABLE_CVBS_OUT==1)
    Add_Command_Table ("cvbs" , 0, STAGE_TOKERNEL);
    #endif

    #if (CONFIG_MUNICH_PNL_EN == 0)
    #if (CONFIG_ENABLE_RTPM)
    Add_Command_Table ("runtime_pm" , 0, STAGE_TOKERNEL);
	Add_Command_Table ("led mbx 0xaa 0xbd", 0, STAGE_TOKERNEL);
	#endif
    #endif

    #if (ENABLE_HDMI_TX == 1)
    Add_Command_Table ("hdmi init" , 0, STAGE_TOKERNEL);
    #endif

    #ifdef CONFIG_DISPLAY_LOGO
    char *BootlogoFile = NULL;
    char BootlogoCmd[CMD_BUF]  = {0};

    BootlogoFile = getenv("BootlogoFile");
    if( BootlogoFile==NULL )
    {
        strncpy(BootlogoCmd,"bootlogo",CMD_BUF);
    }
    else
    {
        char *p = strchr(BootlogoFile, ';');
        if (p)
            // Terminate the string to avoid injected commands
            *p = '\0';

        snprintf(BootlogoCmd, CMD_BUF, "bootlogo 0 0 1 1 %s", BootlogoFile);
    }

    char* opt_logo = NULL;
    opt_logo = getenv("logo");
    if(opt_logo != NULL)
    {
        int logo_on = 0;
        logo_on = simple_strtol(opt_logo, NULL, 10);
        if(logo_on > 0 && logo_on < 3)
        {
            Add_Command_Table (BootlogoCmd , 0, STAGE_TOKERNEL);
            #if defined (CONFIG_URSA_6M40)
            Add_Command_Table ("ursa_lvds_on" , 0, STAGE_TOKERNEL);
            #endif
            #if defined (CONFIG_URSA_8)
            Add_Command_Table ("ursa8_lvds_on" , 0, STAGE_TOKERNEL);
            #endif
        }
    }
    else
    {
        Add_Command_Table (BootlogoCmd , 0, STAGE_TOKERNEL);
        #if defined (CONFIG_URSA_6M40)
        Add_Command_Table ("ursa_lvds_on" , 0, STAGE_TOKERNEL);
        #endif
    }
    #endif

    #if ENABLE_STB_ECOS_BOOT
    Add_Command_Table ("read_boot_info" , 0, STAGE_TOKERNEL);
    #endif

    #if defined(CONFIG_INX_VB1) && defined(CONFIG_ENABLE_4K2K_PANEL)
    Add_Command_Table("panel_inx_vb1_init",0,STAGE_TOKERNEL);
    #endif

    #if (CONFIG_BOOTVIDEO == 1)
    Add_Command_Table("bootvideo 0",0,STAGE_TOKERNEL);
    #endif

    #ifdef CONFIG_POWER_MUSIC
    #if (ENABLE_MODULE_TEE==0)
    Add_Command_Table ("bootmusic" , 0, STAGE_TOKERNEL);
    #endif
    #endif

    #if (CONFIG_MSTAR_FASTBOOT_ENABLE)
    Add_Command_Table ("fastboot" , 0, STAGE_TOKERNEL);
    #endif

    #ifdef CONFIG_GENERIC_MMC
	Add_Command_Table ("mmc slcrelwrchk" , 0, STAGE_TOKERNEL);
    #endif


    #if defined(CONFIG_INX_NOVA_VB1)
    Add_Command_Table ("inx_nova_set_4k2k_2division", 0, STAGE_TOKERNEL);
    #endif

    #if defined(CONFIG_URSA_8)
    Add_Command_Table ("ursa8_set_osd_mode" , 0, STAGE_TOKERNEL);
    #endif

    #if defined(CONFIG_URSA_6M40)
    Add_Command_Table ("ursa_set_osd_mode" , 0, STAGE_TOKERNEL);
    #endif

    #if defined(CONFIG_UART_RX)
    Add_Command_Table ("uart_rx" , 0, STAGE_TOKERNEL);
    #endif

    #if defined(CONFIG_SEC_SYSTEM) // jump to non-TEE second kernel
    Add_Command_Table ("Boot2secKernel" , 0, STAGE_TOKERNEL);
    #endif

    #if (CONFIG_URSA_10)
    Add_Command_Table("send_I2C_cmd_to_ursa10" , 0,STAGE_TOKERNEL);
    #endif

    //*************** alway in bottom ****************
    #if (ENABLE_MODULE_TEE==1)
    #if defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)|| defined(CONFIG_ARMv7_OPTEE)
    Add_Command_Table ("readArmFw;readOptee" , 0, STAGE_TOKERNEL);
    /* ACOS_MOD_BEGIN {amazon_common_kernel_signing_scheme} */
    //Add_Command_Table ("readKL" , 0, STAGE_TOKERNEL);
    /* ACOS_MOD_END {amazon_common_kernel_signing_scheme} */
    #else
    Add_Command_Table ("readNuttx;bootNuttx" , 0, STAGE_TOKERNEL);
    /* ACOS_MOD_BEGIN {amazon_common_kernel_signing_scheme} */
    //Add_Command_Table ("readKL" , 0, STAGE_TOKERNEL);
    /* ACOS_MOD_END {amazon_common_kernel_signing_scheme} */
    Add_Command_Table ("wait_tee_ready" , 0, STAGE_TOKERNEL);
    #endif
    #if defined(CONFIG_URSA6_VB1) || defined(CONFIG_URSA_UNION)
    #if (CONFIG_URSA_UNION)
    if (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U6)
    #endif  // CONFIG_URSA_UNION
    {
        Add_Command_Table("send_I2C_cmd_to_ursa6" , 0,STAGE_TOKERNEL);
    }
    #endif  // CONFIG_URSA6_VB1 || CONFIG_URSA_UNION

    #if defined(CONFIG_URSA9_VB1) || defined(CONFIG_URSA_UNION)
    #if (CONFIG_URSA_UNION)
    if ((MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U9) || (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U11))
    #endif
    {
        Add_Command_Table("send_I2C_cmd_to_ursa9" , 0,STAGE_TOKERNEL);
    }
    #endif

    #if (CONFIG_PANEL_INIT)// Init Panel
    Add_Command_Table("panel_post_init", 0,STAGE_TOKERNEL);
    #endif

    #if (CONFIG_LOCAL_DIMMING)
    Add_Command_Table("local_dimming", 0,STAGE_TOKERNEL);
    #endif

    #ifdef CONFIG_POWER_MUSIC
    Add_Command_Table ("bootmusic" , 0, STAGE_TOKERNEL);
    #endif

    Add_Command_Table ("fcie_tsp_boot_sel 0" , 0, STAGE_TOKERNEL);

    /* ACOS_MOD_BEGIN {amazon_common_kernel_signing_scheme} */
    /* Add_Command_Table ("bootKL" , 0, STAGE_TOKERNEL); */
    char* bm_buf = NULL;
    bm_buf = getenv("bootmode");
    if(bm_buf != NULL)
    {
        int bootmode = atoi(bm_buf);
    	if (bootmode == IDME_BOOTMODE_DIAG) {
    	    Add_Command_Table("amzn_boot diag", 0, STAGE_TOKERNEL);
	}else if (bootmode == IDME_BOOTMODE_TRANSITION) {
	    Add_Command_Table("amzn_boot transition", 0, STAGE_TOKERNEL);
 	}else
            Add_Command_Table ("amzn_boot normal" , 0, STAGE_TOKERNEL);
    }
    /* ACOS_MOD_END {amazon_common_kernel_signing_scheme} */
    #endif  // endif ENABLE_MODULE_TEE

    #if (ENABLE_MODULE_TEE == 0)
    #if (ENABLE_SECURITY_BOOT == 1)
    Add_Command_Table ("readKL" , 0, STAGE_TOKERNEL);
    Add_Command_Table ("bootKL" , 0, STAGE_TOKERNEL);
    #endif
    #endif
    //*********************************************


}

void Customer_Register_MsInit(void)
{
        //Register customter MsInit
}
