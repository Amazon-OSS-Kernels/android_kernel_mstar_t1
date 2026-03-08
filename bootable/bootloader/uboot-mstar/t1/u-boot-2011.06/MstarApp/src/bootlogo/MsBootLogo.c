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

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------

#include <common.h>
#include <malloc.h>
#include <ShareType.h>
#include <bootlogo/MsBootLogo.h>
#include <bootlogo/MsPoolDB.h>
#include <MsApiJpd.h>
#include <MsApiGEGOP.h>
#include <MsVfs.h>
#include <MsSystem.h>
#include <MsApiPanel.h>
#include <MsUboot.h>
#include <MsDebug.h>
#include <MsRawIO.h>
#include <CusConfig.h>
#include <MsMmap.h>
#include <drvGPIO.h>
#include <MsBoot.h>
#include <miu/MsDrvMiu.h>
#include <MsEnvironment.h>

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if defined (CONFIG_LOGO_STORE_IN_SPI_FLASH)
#define FLASH_DEFAULT_TARGET  E_RAW_DATA_IN_SPI
#define FLASH_DEFAULT_PARTITION SPI_DEFAULT_PARTITION
#define FLASH_DEFAULT_VOLUME SPI_DEFAULT_VOLUME
#elif defined (CONFIG_LOGO_STORE_IN_NAND_FLASH)
#define FLASH_DEFAULT_TARGET  E_RAW_DATA_IN_NAND
#define FLASH_DEFAULT_PARTITION NAND_DEFAULT_PARTITION
#define FLASH_DEFAULT_VOLUME NAND_DEFAULT_VOLUME
#elif defined  (CONFIG_LOGO_STORE_IN_EMMC_FLASH)
#define FLASH_DEFAULT_TARGET  E_RAW_DATA_IN_MMC
#define FLASH_DEFAULT_PARTITION MMC_DEFAULT_PARTITION
#define FLASH_DEFAULT_VOLUME MMC_DEFAULT_VOLUME
#endif

#define BOOTLOGO_BUFFER_SIZE 0x400000

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern ST_DBTABLE gstDbtable;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if ( CONFIG_MINIUBOOT && (1==CONFIG_LOGO_STORE_IN_MBOOT) )
#else
static BOOLEAN bdbtableReady=FALSE;
MS_BOOL bAudioPreInit=FALSE;
#endif

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
extern int snprintf(char *str, size_t size, const char *fmt, ...);

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// @brief \b Function Name \b dbtable_init
// @brief \b Description   \b prepare the logo/music/panel/ursa information & data
// @return                 \b dbtable_init_Result
////////////////////////////////////////////////////////////////////////////////

#if (!(CONFIG_LOGO_STORE_IN_MBOOT) && !(CONFIG_LOGO_STORE_NOT_SET) && !(CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN))
int do_dbtable_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret =-1;
    int dbtable = 0;
    U32 u32LogoOffset = 0;
    char * p_str = NULL;
    UBOOT_TRACE("IN\n");

    ret = raw_io_config(FLASH_DEFAULT_TARGET,FLASH_DEFAULT_PARTITION,FLASH_DEFAULT_VOLUME);

    if(ret != 0)
    {
        bdbtableReady=FALSE;
        UBOOT_ERROR("raw_io_config setting fail!\n");
        return -1;
    }

    ret=MsApiChunkHeader_GetValue(CH_UBOOT_POOL_ROM_OFFSET,&u32LogoOffset);
    if(ret != 0)
    {
        bdbtableReady=FALSE;
        UBOOT_ERROR("get logo offset fail!\n");
        return -1;
    }

    UBOOT_DEBUG("u32LogoOffset=0x%x\n",(unsigned int)u32LogoOffset);
    if( (argc > 1) && (strcmp(argv[1],"1")==0))
    {
        UBOOT_DEBUG("sikp db_table\n");

        ret = write_dbtable(u32LogoOffset);
    }
    else
    {
        UBOOT_DEBUG("get db_table\n");
#ifdef UFBL_FEATURE_IDME
        char ModelPath[BUFFER_SIZE]="\0";

        p_str = getenv ("customer_ini_path");
        UBOOT_DEBUG("customer_ini_path %s\n", p_str);
        if(p_str != NULL)
        {
            if(!idme_get_var_external("model_name", ModelPath, BUFFER_SIZE-1))
            {
                UBOOT_DEBUG("model_name %s\n", ModelPath);

                if(strcmp(p_str, ModelPath) == 0)
                {
                    UBOOT_DEBUG("Mpool Model Setting\n");
                    ret =  read_dbtable(u32LogoOffset);
                }
                else
                {
                    UBOOT_DEBUG("Refresh Model Setting\n");
                    ret =  write_dbtable(u32LogoOffset);
                }
            }
            else
            {
                UBOOT_DEBUG("No idme model name\n");
                ret =  read_dbtable(u32LogoOffset);
            }
        }
        else
        {
            UBOOT_DEBUG("No customer_ini_path\n");
            ret =  write_dbtable(u32LogoOffset);
        }
#else
        p_str = getenv ("db_table");
        if(p_str != NULL)
        {

            dbtable = (int)simple_strtol(p_str, NULL, 10);
            UBOOT_DEBUG("db_table=%d\n",dbtable);
            if(dbtable == 0)
            {
                ret =  write_dbtable(u32LogoOffset);

            }
            else
            {
                ret =  read_dbtable(u32LogoOffset);
            }
        }
        else
        {
            UBOOT_DEBUG("No db_table\n");
            ret =  write_dbtable(u32LogoOffset);
        }
#endif
    }
    if(ret!=0)
    {
        bdbtableReady=FALSE;
        UBOOT_ERROR("dbtable init fail\n");
    }
    else
    {
        bdbtableReady=TRUE;
        UBOOT_TRACE("OK\n");
    }
    return ret;
}
#endif

#if (CONFIG_LOGO_STORE_IN_MBOOT)
static int LogoInMboot2Dram(MS_U32 *Dram_Addr)
{
    int ret = -1;
    MS_U32 u32LogoSize = 0;
    MS_U32 u32LogoOffset = 0;
    UBOOT_TRACE("IN\n");

    ret = mboot_raw_io_Config();
    if(ret != 0)
    {
        UBOOT_ERROR("raw_io_config setting fail!\n");
        return -1;
    }

    ret=MsApiChunkHeader_GetValue(CH_LOGO_ROM_OFFSET,&u32LogoOffset);
    if(ret!=0)
    {
        UBOOT_ERROR("get u32LogoOffset fail\n");
        return -1;
    }
    ret=MsApiChunkHeader_GetValue(CH_LOGO_ROM_SIZE,&u32LogoSize);
    if(ret!=0)
    {
        UBOOT_ERROR("get u32LogoSize fail\n");
        return -1;
    }

    *Dram_Addr=(MS_U32)malloc(u32LogoSize);
    if(Dram_Addr!=NULL)
    {
        if(raw_read(*Dram_Addr,u32LogoOffset,u32LogoSize)!=0)
        {
            UBOOT_ERROR("LogoInMboot2Dram Fail!!\n");
            return -1;
        }
    }

    UBOOT_TRACE("OK\n");
    return u32LogoSize;
}
#else
static unsigned int Load_Photo2Dram(char *SrcPath,U32 *Dram_Addr)
{
    U32 u32LogoSize=0;
    UBOOT_TRACE("IN\n");

    if(SrcPath==NULL)// from MPOOL:dbtable_init
    {
        UBOOT_DEBUG("No SrcPath\n");
        if(bdbtableReady==TRUE)
        {
            u32LogoSize = gstDbtable.dbdata[E_DB_LOGO].Size;
            UBOOT_DEBUG("u32LogoSize=0x%x\n",(unsigned int)u32LogoSize);
            *Dram_Addr=(U32)malloc(u32LogoSize);
            if((Dram_Addr==NULL) || (Read_LogoToDisplayAddr(*Dram_Addr)!=0)) //read logo from spi/emmc to dram
            {
                UBOOT_ERROR("Error: Read_LogoToDisplayAddr Fail\n");
                return 0;
            }
        }
    }
    else // from nand/emmc file system directly
    {
        char PathBuf[CMD_BUF] = "\0";
        vfs_mount(CUSTOMER);
        snprintf(PathBuf, sizeof(PathBuf), "%s/%s",CUSTOMER_PATH,SrcPath);
        UBOOT_DEBUG("PathBuf=%s\n",PathBuf);
        u32LogoSize = vfs_getsize(PathBuf);
        UBOOT_DEBUG("u32LogoSize=0x%x\n",(unsigned int)u32LogoSize);
        *Dram_Addr=(U32)malloc(u32LogoSize);
        if(Dram_Addr==NULL || (vfs_read((void *)(*Dram_Addr),PathBuf,0,u32LogoSize)!=0))
        {
            UBOOT_ERROR("Error: vfs_read Fail\n");
            return 0;
        }
    }
    UBOOT_DEBUG("Dram_Addr=0x%x\n",(unsigned int)Dram_Addr);
    UBOOT_TRACE("OK\n");
    return u32LogoSize;
}
#endif

int MsApi_GetLogoAddrs(unsigned int *phyAddr)
{
    char *cmdline =NULL;
    char bootlogo_buffer[CMD_BUF] ="\0";
    char *p_str = NULL;
    unsigned int u32LogoBufSize=0;
    extern U8 g_logoMiuIdx;
    extern U8 bDelayLogo;

    cmdline = getenv ("bootargs");
    if ((cmdline==NULL) || (strstr(cmdline, "delaylogo=true") == NULL) || (isBootToRecovery()))
    {
        // normal logo

        // use the bootlogo buffer to store bootlogo
        strncpy(bootlogo_buffer,"E_MMAP_ID_BOOTLOGO_BUFFER",strlen("E_MMAP_ID_BOOTLOGO_BUFFER")+1);
        if (get_addr_from_mmap(bootlogo_buffer, phyAddr) == 0)
        {
            return 0;
        }
        else if (get_value_from_env(E_DFB_FRAMEBUFFER_ADR, MMAP_E_DFB_FRAMEBUFFER_ADDR, phyAddr)!=0)
        {
            UBOOT_ERROR("get %s mmap fail\n",E_DFB_FRAMEBUFFER_ADR);
            return -1;
        }
    }
    else
    {
        // delay bootlogo

        // use the bootlogo buffer to store bootlogo
        strncpy(bootlogo_buffer,"E_MMAP_ID_BOOTLOGO_BUFFER",strlen("E_MMAP_ID_BOOTLOGO_BUFFER")+1);
        if (get_addr_from_mmap(bootlogo_buffer, phyAddr) == 0)
        {
            get_length_from_mmap(bootlogo_buffer,&u32LogoBufSize);
            if((u32LogoBufSize&0xfffffff) >= BOOTLOGO_BUFFER_SIZE)
            {
                g_logoMiuIdx = MsApi_Miu_GetID(*phyAddr);
                bDelayLogo = TRUE;
                return 0;
            }
        }

        // check if users define another buffer to store the bootlogo
        if ((p_str = getenv("bootlogo_buffer")) != NULL)
        {
            strncpy(bootlogo_buffer, p_str ,strlen(p_str));
        }
        // use the PVR buffer to store bootlogo, because the DFB buffer will be cleared during booting up
        else
        {
            strncpy(bootlogo_buffer,"E_MMAP_ID_PVR_DOWNLOAD",strlen("E_MMAP_ID_PVR_DOWNLOAD")+1);
        }

        if (get_addr_from_mmap(bootlogo_buffer,phyAddr) != 0)
        {
                UBOOT_ERROR("Cannot get the size of %s\n", bootlogo_buffer);
                return -1;
        }
        else
        {
            get_length_from_mmap(bootlogo_buffer,&u32LogoBufSize);
            if((u32LogoBufSize&0xfffffff) >= BOOTLOGO_BUFFER_SIZE)
            {
                g_logoMiuIdx = MsApi_Miu_GetID(*phyAddr);
                bDelayLogo = TRUE;
            }
            else
            {
                UBOOT_ERROR("fail to delay bootlogo because the size of buffer is less than 4MB (1366*768 *ARGB),\n");
                UBOOT_ERROR("but your buffer(%s) size is only 0x%x.\n",bootlogo_buffer,*phyAddr);
                UBOOT_ERROR("please check your mmap.ini first\n");
                return -1;
            }
        }
    }
    return 0;
}

int showLogo(LogoDispConfigParams stConfig,U32 JPD_OUTADDR)
{
    int ret = -1;
    GFX_BufferInfo src_info, dst_info;
    GFX_DrawRect rect;
    MS_BOOL bHorStretch=stConfig.bHorStretch;
    MS_BOOL bVerStretch=stConfig.bVerStretch;
    unsigned int u32Addr=0;
    UBOOT_TRACE("IN\n");

    src_info.u32Addr = JPD_OUTADDR;
    UBOOT_DEBUG("src_info.u32Addr=0x%x\n",(unsigned int)src_info.u32Addr);

    src_info.u32ColorFmt = GFX_FMT_YUV422;
    UBOOT_DEBUG("src_info.u32ColorFmt=0x%x\n",(unsigned int)src_info.u32ColorFmt);

    src_info.u32Width = (U32)JPG_GetAlignmentWidth();
    UBOOT_DEBUG("src_info.u32Width=0x%x\n",(unsigned int)src_info.u32Width);

    src_info.u32Height = (U32)JPG_GetAlignmentHeight();
    UBOOT_DEBUG("src_info.u32Height=0x%x\n",(unsigned int)src_info.u32Height);

    src_info.u32Pitch = (U32)JPG_GetAlignmentPitch()<<1;
    UBOOT_DEBUG("src_info.u32Pitch=0x%x\n",(unsigned int)src_info.u32Pitch);

    ret = MsApi_GetLogoAddrs(&u32Addr);
    if (ret != 0)
    {
        UBOOT_ERROR("get bootlogo addr error\n");
        return -1;
    }
    dst_info.u32Addr = (MS_PHYADDR) u32Addr;

    UBOOT_DEBUG("dst_info.u32Addr=0x%x\n",(unsigned int)dst_info.u32Addr);

    dst_info.u32ColorFmt = GFX_FMT_YUV422;
    UBOOT_DEBUG("dst_info.u32ColorFmt=0x%x\n",(unsigned int)dst_info.u32ColorFmt);

    rect.srcblk.x = 0;
    UBOOT_DEBUG("rect.srcblk.x=0x%x\n",(unsigned int)rect.srcblk.x);

    rect.srcblk.y = 0;
    UBOOT_DEBUG("rect.srcblk.y=0x%x\n",(unsigned int)rect.srcblk.y);

    rect.srcblk.width = JPG_GetAlignmentWidth()/*u16Pitch*/;
    UBOOT_DEBUG("rect.srcblk.width=0x%x\n",(unsigned int)rect.srcblk.width);


    rect.srcblk.height = JPG_GetAlignmentHeight();
    UBOOT_DEBUG("rect.srcblk.height=0x%x\n",(unsigned int)rect.srcblk.height);

    rect.dstblk.x = 0;
    UBOOT_DEBUG("rect.dstblk.x=0x%x\n",(unsigned int)rect.dstblk.x);

    rect.dstblk.y = 0;
    UBOOT_DEBUG("rect.dstblk.y=0x%x\n",(unsigned int)rect.dstblk.y);

    #if (ENABLE_MSTAR_KENYA == 1||ENABLE_MSTAR_KERES==1 || ENABLE_MSTAR_KIRIN==1 || ENABLE_MSTAR_KRIS==1 || ENABLE_MSTAR_KAYLA==1 || ENABLE_MSTAR_KAISER==1 || ENABLE_MSTAR_KRATOS==1 || ENABLE_MSTAR_KIWI==1)
        rect.dstblk.width  = ((g_IPanel.Width()+15)&0xFFFFF0)/*u16Pitch*/;
        rect.dstblk.height = g_IPanel.Height();

        dst_info.u32Width  = (U32)((g_IPanel.Width()+15)&0xFFFFF0);
        dst_info.u32Pitch  = (U32)(((g_IPanel.Width()+15)&0xFFFFF0)<<1);
        dst_info.u32Height = g_IPanel.Height();
    #else
    if( JPG_GetAlignmentWidth() > g_IPanel.Width())
    {
        rect.dstblk.width = ((g_IPanel.Width()+15)&0xFFFFF0)/*u16Pitch*/;
        dst_info.u32Width = (U32)((g_IPanel.Width()+15)&0xFFFFF0);
        dst_info.u32Pitch = (U32)(((g_IPanel.Width()+15)&0xFFFFF0)<<1);
    }
    else
    {
        rect.dstblk.width = ((JPG_GetAlignmentWidth()+15)&0xFFFFF0)/*u16Pitch*/;
        dst_info.u32Width = (U32)((JPG_GetAlignmentWidth()+15)&0xFFFFF0);
        dst_info.u32Pitch = (U32)(((JPG_GetAlignmentPitch()+15)&0xFFFFF0)<<1);
    }
    UBOOT_DEBUG("rect.dstblk.width=0x%x\n",(unsigned int)rect.dstblk.width);
    UBOOT_DEBUG("dst_info.u32Width=0x%x\n",(unsigned int)dst_info.u32Width);
    UBOOT_DEBUG("dst_info.u32Pitch=0x%x\n",(unsigned int)dst_info.u32Pitch);

    if( JPG_GetAlignmentHeight() > g_IPanel.Height())
    {
        rect.dstblk.height = g_IPanel.Height();
        dst_info.u32Height = g_IPanel.Height();
    }
    else
    {
        rect.dstblk.height = JPG_GetAlignmentHeight();
        dst_info.u32Height = (U32)JPG_GetAlignmentHeight();
    }
    UBOOT_DEBUG("rect.dstblk.height=0x%x\n",(unsigned int)rect.dstblk.height);
    UBOOT_DEBUG("dst_info.u32Height=0x%x\n",(unsigned int)dst_info.u32Height);
    #endif

    MsApiDisplay_Logo(&rect,&src_info,&dst_info,bHorStretch,bVerStretch,stConfig.u16DispX,stConfig.u16DispY);
    UBOOT_TRACE("OK\n");
    return 0;
}

int do_bootlogo (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = -1;
    U32 u32LogoSize=0;
    U32 JPD_InputADDR=0;
    U32 JPD_OUTADDR=0;
    char *PhotoPath=NULL;
    LogoDispConfigParams stConfig;
    UBOOT_TRACE("IN\n");

    printf("\n[AT][MB][bootlogo begin][%lu]\n", MsSystemGetBootTime());

    /* increase bootcount when show bootlogo*/
    idme_boot_info();

    if(argc > 5)
    {
        PhotoPath = argv[5];
        UBOOT_DEBUG("PhotoPath=%s\n",PhotoPath);
    }

    if(argc > 4)
    {
        stConfig.u16DispX = simple_strtoul(argv[1], NULL, 10);
        stConfig.u16DispY = simple_strtoul(argv[2], NULL, 10);
        stConfig.bHorStretch = simple_strtoul(argv[3], NULL, 10);
        stConfig.bVerStretch = simple_strtoul(argv[4], NULL, 10);
    }
    else
    {
        stConfig.u16DispX = 0;
        stConfig.u16DispY = 0;
        stConfig.bHorStretch = 1;
        stConfig.bVerStretch = 1;
    }

    UBOOT_DEBUG("u16DispX=0x%x,u16DispY=0x%x,bHorStretch=0x%x,bVerStretch=0x%x\n",stConfig.u16DispX,stConfig.u16DispY,stConfig.bHorStretch,stConfig.bVerStretch);

    if(IsPanelReady()!=0)
    {
        UBOOT_ERROR("Panle is not ready\n");
        return -1;
    }

#if (CONFIG_LOGO_STORE_IN_MBOOT)
    u32LogoSize = LogoInMboot2Dram(&JPD_InputADDR);
#else
    u32LogoSize = Load_Photo2Dram(PhotoPath,&JPD_InputADDR);
#endif

    UBOOT_DEBUG("u32LogoSize : 0x%x \n",u32LogoSize);
    if(u32LogoSize!= 0)
    {
        JPD_OUTADDR=MsApi_JPD_Decode((MS_U32)JPD_InputADDR, u32LogoSize);
        ret = showLogo(stConfig,JPD_OUTADDR);
        if(ret != 0)
        {
            UBOOT_ERROR("bootlogo error\n");
        }
    }

    if(((void*)JPD_InputADDR)!=NULL)
    {
        free((void*)JPD_InputADDR);
    }

    UBOOT_TRACE("OK\n");
    return ret;
}

#if (ENABLE_MODULE_POWER_MUSIC == 1)
#include <MsApiAudio.h>
#include <audio/MsDrvAudio.h>

int Load_Music2Dram(char *MusicPath,U32 Dram_Addr)
{
    U32 musicSize=0;
    UBOOT_TRACE("IN\n");

    if(MusicPath == NULL)
    {
        UBOOT_DEBUG("MusicPath is a null pointer\n");
        UBOOT_DEBUG("Dram_Addr=0x%x\n",Dram_Addr);
        //read music from spi/emmc to dram defalut logo
        if(Read_MusicToPlayAddr(Dram_Addr)!=0)
        {
            UBOOT_ERROR("Read_MusicToPlayAddr() failed\n");
            return -1;
        }
    }
    else // from nand/emmc directly
    {
        char PathBuf[128] = "\0";
        vfs_mount(CUSTOMER);
        snprintf(PathBuf, sizeof(PathBuf), "%s/%s",CUSTOMER_PATH,MusicPath);
        UBOOT_DEBUG("PathBuf=%s\n",PathBuf);
        musicSize = vfs_getsize(PathBuf);
        UBOOT_DEBUG("musicSize=0x%x,Dram_Addr=0x%x\n",(unsigned int)musicSize,(unsigned int)Dram_Addr);
        if(vfs_read((void *)(PA2NVA(Dram_Addr)),PathBuf,0,musicSize)==-1)
        {
            UBOOT_ERROR("vfs_read fail\n");
            return -1;
        }
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

int do_audiopreinit(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    int ret = 0;
    int music_on = 0;
    char* opt_music = NULL;

    opt_music = getenv("music");
    if(opt_music != NULL)
    {
        music_on = simple_strtol(opt_music, NULL, 10);
         if(music_on < 1 || music_on > 2)
            {
                printf("!!! [%s]  Skip Power Music  !!! \n",__FUNCTION__);
                return -1;
            }
    }
    //Adj_Volume_Off();      //
    Audio_Amplifier_OFF(); //
    ret =MsDrv_AudioPreInit();

    printf("[AT][MB][audio_preinit][%lu]\n", MsSystemGetBootTime());
    if(ret==-1)
    {
        UBOOT_ERROR("Audio PreInit Fail Skip Power Music !!! \n");
        bAudioPreInit = FALSE;
    }
    else
    {
        bAudioPreInit = TRUE;
    }

    UBOOT_TRACE("OK\n");

    return 1;
}

int do_bootmusic (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;
    char *MusicPath = NULL;
    U32 u32EsStartAddr = 0;
    int music_on = 0;
    char* opt_music = NULL;


    UBOOT_TRACE("IN\n");
    if(argc > 1)
    {
        MusicPath = argv[1];
    }

    opt_music = getenv("music");
    if(opt_music != NULL)
    {
        music_on = simple_strtol(opt_music, NULL, 10);
         if(music_on < 1 || music_on > 2)
            {
                printf("!!! [%s]  Skip Power Music  !!! \n",__FUNCTION__);
                return -1;
            }
    }

    if(bdbtableReady!=TRUE)
    {
        UBOOT_ERROR("dbtable is not ready\n");
        return -1;
    }

    if(bAudioPreInit!=TRUE)
    {
        UBOOT_ERROR("Audio PreInit is not ready\n");
        return -1;
    }
/// move to audio pre init --start--
#if 0
    Adj_Volume_Off();      //
    Audio_Amplifier_OFF(); //
    ret =MsDrv_AudioPreInit();

    if(ret==-1)
    {
        UBOOT_ERROR("Get Mmap for Audio Fail Skip Power Music !!! \n");
        return -1;
    }
#endif
/// move to audio pre init --end--

    u32EsStartAddr = MsDrv_GetEsStartAddr();

    ret = Load_Music2Dram(MusicPath, u32EsStartAddr);
    if(ret==-1)
    {
        UBOOT_ERROR("load music to dram fail\n");
    }
    else
    {
        //   MsOS_DelayTask(2000);
        play_powermusic_main();
        UBOOT_TRACE("OK\n");
    }
    return ret;
}
#endif


