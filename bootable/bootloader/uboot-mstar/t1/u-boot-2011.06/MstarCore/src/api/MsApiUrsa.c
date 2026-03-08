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

#include <command.h>
#include <common.h>
#include <MsDebug.h>
#include <MsApiUrsa.h>
#include <MsApiPanel.h>
#include <panel/MsDrvPanel.h>
#include <bootlogo/MsPoolDB.h>

#if defined(CONFIG_URSA6_VB1) || defined(CONFIG_URSA_UNION)
#include <ursa/ursa_6m38.h>
#endif

#if defined(CONFIG_URSA9_VB1) || defined(CONFIG_URSA_UNION)
#include <ursa/ursa_9.h>
#endif

#if defined(CONFIG_URSA_10)
#include <ursa/ursa_10.h>
#endif

#include <ursa/ursa_vb1.h>

#if defined (CONFIG_URSA6_VB1) || defined(CONFIG_URSA_UNION)
int do_send_I2C_cmd_to_ursa6(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#if defined(CONFIG_MSTAR_MONACO) || defined(CONFIG_MSTAR_MUJI) || defined(CONFIG_MSTAR_MONET) || defined(CONFIG_MSTAR_MANHATTAN) || defined(CONFIG_MSTAR_MESSI) || defined(CONFIG_MSTAR_MASERATI)|| defined(CONFIG_MSTAR_MACAN) || defined(CONFIG_MSTAR_MAXIM)
    if(0 != IsPanelReady())
    {
        UBOOT_ERROR("Panel is not ready\n");
        return -1;
    }
    MDrv_Ursa_6M38_SWI2C_Init();
    MDrv_Ursa_6M38_Mute(TRUE);
    //MDrv_Ursa_6M38_4K2K60_init();
#if defined(CONFIG_MSTAR_MUJI) || defined(CONFIG_MSTAR_MONET) || defined(CONFIG_MSTAR_MANHATTAN) || defined(CONFIG_MSTAR_MESSI) || defined(CONFIG_MSTAR_MASERATI)|| defined(CONFIG_MSTAR_MACAN) || defined(CONFIG_MSTAR_MAXIM)// Fix handshake issue, abc123 0795280.
    udelay(30000);
    MApi_PNL_SetVideoHWTraining(TRUE);  // Enable HW training.
    udelay(40000);
#endif
    MDrv_Ursa_6M38_4K2K_DisableAutoMute();
    MDrv_Ursa_6M38_Mute(FALSE);
#else
    PanelType panel_data;
    memset(&panel_data, 0, sizeof(panel_data));

    if(is_str_resume())
    {
        U32 u32PanelConfigsAddr;
		if(get_value_from_env(E_MMAP_ID_VDEC_CPU_ADR,NO_DEFAULT_MMAP_VALUE, &u32PanelConfigsAddr)!=0)

        {
            UBOOT_ERROR("get E_MMAP_ID_VDEC_CPU mmap fail\n");
            return -1;
        }
        UBOOT_DEBUG("E_MMAP_ID_VDEC_CPU = 0x%x\n", u32PanelConfigsAddr);
        UBOOT_DEBUG("(U32)(PA2NVA(u32PanelConfigsAddr)) = 0x%x\n", (U32)(PA2NVA(u32PanelConfigsAddr)));
        memcpy(&panel_data, (U32*)(PA2NVA(u32PanelConfigsAddr)), sizeof(PanelType));
    }
    else
    {
        if(Read_PanelParaFromflash(&panel_data)!=0)
        {
            UBOOT_ERROR("%s: Read_PanelParaFromflash() failed, at %d\n", __func__, __LINE__);
            return -1;
        }
    }

    if(panel_data.m_wPanelWidth == 3840 && panel_data.m_wPanelHeight == 2160)
        MDrv_Ursa_6M38_4K2K_init();
    else
        MDrv_Ursa_6M38_Set_2_lane_VB1_per_init();

    if(panel_data.m_wPanelWidth == 1920 && panel_data.m_wPanelHeight == 1080)
        MDrv_Ursa_6M38_Set_2_lane_VB1();
#endif
    return 0;
}
#endif

#if defined (CONFIG_URSA9_VB1) || defined(CONFIG_URSA_UNION)
int do_send_I2C_cmd_to_ursa9(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    PanelType panel_data;
    MDrv_Ursa_9_SWI2C_Init();
    if (LINK_VBY1_10BIT_2LANE == getLinkExtType())
    {
        MDrv_Ursa_9_Set_Lane_VB1_per_init(2, 2);
    }
    else if (LINK_VBY1_10BIT_4LANE == getLinkExtType())
    {
        MDrv_Ursa_9_Set_Lane_VB1_per_init(4, 4);
    }
    #if (ENABLE_MSTAR_MUJI==1 || ENABLE_MSTAR_MONACO ==1 || ENABLE_MSTAR_MONET==1 || ENABLE_MSTAR_MANHATTAN==1 || ENABLE_MSTAR_MASERATI==1|| ENABLE_MSTAR_MACAN==1 || ENABLE_MSTAR_MAXIM==1)
    else if (LINK_VBY1_10BIT_8LANE == getLinkExtType())
    {
        MDrv_Ursa_9_Set_Lane_VB1_per_init(4, 8);
    }
    #endif

    memset(&panel_data, 0, sizeof(panel_data));
    if(Read_PanelParaFromflash(&panel_data)!=0)
    {
        UBOOT_ERROR("%s: Read_PanelParaFromflash() failed, at %d\n", __func__, __LINE__);
        return -1;
    }

    if (LINK_VBY1_10BIT_2LANE == getLinkExtType())
    {
        MDrv_Ursa_9_Set_Lane_VB1(1920, panel_data.m_wPanelWidth);
    }
    else if (LINK_VBY1_10BIT_4LANE == getLinkExtType())
    {
        MDrv_Ursa_9_Set_Lane_VB1(3840, panel_data.m_wPanelWidth);
    }
    #if (ENABLE_MSTAR_MUJI==1 || ENABLE_MSTAR_MONACO ==1 || ENABLE_MSTAR_MONET==1 || ENABLE_MSTAR_MANHATTAN==1 ||  ENABLE_MSTAR_MASERATI==1|| ENABLE_MSTAR_MACAN==1 || ENABLE_MSTAR_MAXIM==1)
    else if (LINK_VBY1_10BIT_8LANE == getLinkExtType())
    {
        MDrv_Ursa_9_Set_Lane_VB1(3840, panel_data.m_wPanelWidth);
    }
    #if (ENABLE_MSTAR_MUJI==1 || ENABLE_MSTAR_MONET==1 || ENABLE_MSTAR_MANHATTAN==1 ||  ENABLE_MSTAR_MASERATI==1|| ENABLE_MSTAR_MACAN==1 || ENABLE_MSTAR_MAXIM==1)
    mdelay(30);
    MApi_PNL_SetVideoHWTraining(TRUE);  // Enable HW training.
    mdelay(40);
    #endif
    #endif
    return 0;
}
#endif

int do_send_I2C_cmd_unlock_transfer_board(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char* opt_unlock = NULL;
    opt_unlock = getenv("unlock_cmi");
    if(opt_unlock != NULL)
    {
       if(strcmp(opt_unlock,"1")==0)
        {
            UBOOT_TRACE("IN\n");
            MDrv_Cmi_Transfer_Unlock();
            UBOOT_TRACE("OK\n");
        }
    }
    return 0;
}


#if defined(CONFIG_URSA_10)
int do_send_I2C_cmd_to_ursa10(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MDrv_Ursa_10_Lvds_init();
    return 0;
}
#endif

#if defined(CONFIG_URSA6_VB1) ||  defined(CONFIG_URSA_10) || defined(CONFIG_URSA9_VB1) || defined(CONFIG_URSA_UNION)
int do_send_I2C_cmd_to_ursa(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#if defined (CONFIG_URSA6_VB1) || defined(CONFIG_URSA_UNION)
    do_send_I2C_cmd_to_ursa6(cmdtp,flag,argc,argv);
#endif
#if defined (CONFIG_URSA9_VB1) || defined(CONFIG_URSA_UNION)
    do_send_I2C_cmd_to_ursa9(cmdtp,flag,argc,argv);
#endif
#if defined(CONFIG_URSA_10)
    do_send_I2C_cmd_to_ursa10(cmdtp,flag,argc,argv);
#endif
    return 0;
}
#endif
