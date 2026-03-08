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
#include <exports.h>
#include <environment.h>
#include <errno.h>
#include <MsTypes.h>
#include <MsDebug.h>
#include <MsRawIO.h>
#include <MsEnvironment.h>
#include <MsMmap.h>
#include <MsUtility.h>
#include <miu/MsDrvMiu.h>
#include <MsSystem.h>
#include <MsApiSpi.h>

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MMAP_ID_LXMEM "E_LX_MEM"
#define MMAP_ID_EMAC "E_EMAC_MEM"
#define MMAP_ID_GMAC "E_GMAC_MEM"

#define MMAP_ID_GE "E_MST_GEVQ"
#define MMAP_ID_GOP "E_MST_GOP_REGDMA"
#define MMAP_ID_PM51_USAGE "E_MMAP_ID_PM51_USAGE_MEM"
#define MMAP_ID_FLUSH_BUFFER "E_MMAP_ID_FLUSH_BUFFER"
#define MMAP_ID_PM51_CODE "E_MMAP_ID_PM51_CODE_MEM"
#define MMAP_ID_XC_MAIN_FB "E_MMAP_ID_XC_MAIN_FB"
#define MMAP_ID_FRC_4K2K "E_MMAP_ID_FRC_4K2K"


#define ENV_DRAM_LEN      "DRAM_LEN"
#define ENV_LX_MEM_LEN    "LX_MEM"
#define ENV_EMAC_MEM_LEN  "EMAC_MEM"
#define ENV_GMAC_MEM      "GMAC_MEM"

#define ENV_LX_MEM2       "LX_MEM2"
#define ENV_LX_MEM3       "LX_MEM3"
#define ENV_PM51_ADDR     "PM51_ADDR"
#define ENV_PM51_LEN      "PM51_LEN"



#define ENV_GE_MEM "GE_ADDR"
#define ENV_GE_LEN "GE_LEN"
#define ENV_GOP_MEM "GOP_ADDR"
#define ENV_GOP_LEN "GOP_LEN"

#define ENV_SYNC_MMAP "sync_mmap"

#if defined(__ARM__)
#define MIU1_LOGIC_ADR_OFFSET 0xA0000000
#else
#define MIU1_LOGIC_ADR_OFFSET 0x60000000
#endif

#if (CONFIG_OAD_IN_MBOOT)
#define OAD_IN_MBOOT         "OAD_IN_MBOOT"
#endif

#define ENV_51ONRAM         "51OnRam"
#if ((ENABLE_BOOTING_FROM_EXT_SPI_WITH_PM51==1)||(ENABLE_BOOTING_FROM_OTP_WITH_PM51==1))
#define ENV_51ONRAM_CFG         "1"
#else
#define ENV_51ONRAM_CFG         "0"
#endif
extern BOOLEAN bIsLoadMmapSuccess;
char miuID[][MAX_MMAP_ID_LEN]={
    "MIU0_GROUP_SELMIU",
    "MIU0_GROUP_PRIORITY",
    "MIU1_GROUP_SELMIU",
    "MIU1_GROUP_PRIORITY",
    "MIU2_GROUP_SELMIU",
    "MIU2_GROUP_PRIORITY"
};

char mmapID[][MAX_MMAP_ID_LEN]={
        "E_LX_MEM",
        "E_LX_MEM2",
        "E_LX_MEM3",
        "E_EMAC_MEM",
        "E_GMAC_MEM",
        "E_MMAP_ID_NUTTX_MEM",
        "E_MMAP_ID_HW_AES_BUF",
        "E_MMAP_ID_PM51_CODE_MEM",
        "E_MMAP_ID_PM51_USAGE_MEM",
        "E_MMAP_ID_MAD_R2",
        "E_MMAP_ID_MAD_SE",
        "E_MMAP_ID_MAD_DEC",
        "E_MMAP_ID_MAD_COMMON",
        "E_MMAP_ID_JPD_READ",
        "E_MMAP_ID_JPD_WRITE",
        "E_MMAP_ID_PHOTO_INTER",
        "E_DFB_FRAMEBUFFER",
        "E_MMAP_ID_GOP_FB",
        "E_MMAP_ID_XC_MAIN_FB",
        "E_MMAP_ID_FRC_4K2K",
        "E_MMAP_ID_VE",
        "E_MMAP_ID_VDEC_CPU",
        "E_MMAP_ID_VDEC_BITSTREAM",
        "E_MMAP_ID_VDEC_SUB_BITSTREAM",
        "E_MMAP_ID_VDEC_FRAMEBUFFER", // VDEC_FRAMEBUFFER
        "E_MMAP_ID_MBOOT_MEM_USAGE",
        "E_MMAP_ID_DIP_10M_2",
        "E_MMAP_ID_CAMERA",
        "E_MMAP_ID_BOOTLOGO_BUFFER",
        "E_MMAP_ID_RECOVERY_BUFFER",
        "E_MMAP_ID_LZO_MEM_USAGE",
        "E_MMAP_ID_XC_FRC_PQ",
        "E_MMAP_ID_XC_FRC_R",
        "E_MMAP_ID_XC_FRC_L",
        "E_MMAP_ID_XC1_MAIN_FB",
        "E_MMAP_ID_XC_SELF",
#if (ENABLE_HAS_LD==1)
        "E_MMAP_ID_LOCAL_DIMMING", // local dimming_BUFFER
#endif
#if (CONFIG_URSA12_VB1_FRC_BRINGUP)
        "E_MMAP_ID_FRC_R2",
#endif
        "E_MMAP_ID_DMX_SECBUF",
#if (CONFIG_BOOTVIDEO == 1)
    "E_MMAP_ID_VDEC_AEON",
    "E_MMAP_ID_VDEC_FRAME_BUF",
    "E_MMAP_ID_VDEC_BITSTREAM",
    "E_MMAP_ID_XC_MENULOAD_BUF",
    "E_MMAP_ID_VIDEO_FILE_IN",
#endif
        };

#if (ENABLE_MODULE_ENV_IN_SERIAL)
#define SECTOR_SIZE   0x10000
#define ENV_SECTOR_SIZE   SECTOR_SIZE
#elif (ENABLE_MODULE_ENV_IN_NAND)
extern  int ubi_get_volume_size(char *, size_t* );
extern int ubi_get_leb_size(void);
#define SECTOR_SIZE   ubi_get_leb_size()
#define ENV_SECTOR_SIZE   SECTOR_SIZE
#elif (ENABLE_MODULE_ENV_IN_MMC)
#define SECTOR_SIZE   0x200
#define ENV_SECTOR_SIZE   0x10000
#endif


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern unsigned int cfg_env_offset;
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
extern int snprintf(char *str, size_t size, const char *fmt, ...);
//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
static int del_cfg(char *source,char *delCfg);
static int set_info_exchange_cfg(void);
static int set_security_cfg(void);
static int set_panelinit_cfg(void);
static void if_51OnRam_set(void);
static int set_env_cfg(void);
int syncMmap2bootargs(void);
static int _syncLen2bootargs(char *id, char *envName);
static int _syncAddr2bootargs(char *id, char *envName);
int delMmapInfoFromEnv(void);
int saveMmapInfoToEnv(void);
int syncMmapToEnv(void);
int processMmapInfo(BOOLEAN save2env);
int processMmapInfoOnEnv(char *ptr, BOOLEAN save2env);
int checkUbootFromEnv(void);

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------


int get_value_from_env(char *id, unsigned int defval, unsigned int *addr)
{
    char *pEnv=NULL;

    UBOOT_TRACE("IN\n");
    if((id==NULL)||(addr==NULL))
    {
        UBOOT_ERROR("One of the parameters is a null pointer\n");
        return -1;
    }
	UBOOT_DEBUG("id=%s\n",id);

    pEnv = getenv(id);
    if(pEnv)
    {
        *addr = (int)simple_strtoul(pEnv, NULL, 16);
         UBOOT_DEBUG("*addr=0x%x from environment\n",*addr);
    }
    else
    {
        if(defval!=NO_DEFAULT_MMAP_VALUE)
        {
            *addr = defval;
	        UBOOT_DEBUG("*addr=0x%x from defval\n",*addr);
		}
		else
		{
	        UBOOT_DEBUG("no mmap(%s) information!!\n",id);
            return -1;
		}
    }

    UBOOT_TRACE("OK\n");
    return 0;
}


static int del_cfg(char *source,char *delCfg)
{
    char *substr = NULL;
    char *pPreEnvEnd = NULL;

    UBOOT_TRACE("IN\n");
    if(source==NULL)
    {
        UBOOT_ERROR("The input parameter 'source' is a null pointer\n");
        return -1;
    }

    if(delCfg==NULL)
    {
        UBOOT_ERROR("The input parameter 'delCfg' is a null pointer\n");
        return -1;
    }

    if(strlen(source)==0)
    {
        UBOOT_ERROR("The length of source is zero\n");
        return -1;
    }


    substr=strstr(source,delCfg);
    if(substr == NULL)
    {
        return 1;
    }
    else
    {
        pPreEnvEnd = strchr(substr,' ');
        if(((unsigned int)pPreEnvEnd-(unsigned int)source+1)<strlen(source))
        {
            strcpy(substr,pPreEnvEnd+1); //the +1 is for skip space
        }
        else
        {
            UBOOT_DEBUG("This member is the last one in the bootargs\n");
            //clear the rst of size.
            *(substr-1)='\0';
            memset(substr,0,strlen(substr));
        }
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

int do_del_boogargs_cfg (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
   int ret;
   UBOOT_TRACE("IN\n");
   if(argc!=2)
   {
        return cmd_usage(cmdtp);
   }

   ret = del_bootargs_cfg(argv[1],0);
   if(ret==0)
   {
        UBOOT_TRACE("OK\n");
   }
   else
   {
        UBOOT_ERROR("delete %s fail\n",argv[1]);
   }
   return ret;
}

int del_bootargs_cfg(char *delCfg,MS_BOOL bDontSaveEnv)
{
    int ret=0;
    char *bootarg=NULL;
    char *OriArg = NULL;
    UBOOT_TRACE("IN\n");
    if(delCfg==NULL)
    {
        UBOOT_ERROR("The input parameter 'delCfg' is a null pointer\n");
        return -1;
    }

    bootarg = getenv("bootargs");
    if(bootarg==NULL)
    {
        UBOOT_ERROR("No env 'bootargs'\n");
        return -1;
    }

    OriArg=malloc(strlen(bootarg)+1);
    if(OriArg==NULL)
    {
        UBOOT_ERROR("malloc for tempBuf fail\n");
        return -1;
    }
    memset(OriArg,0,strlen(bootarg)+1);
    strcpy(OriArg,bootarg);

    ret=del_cfg(OriArg,delCfg);
    if(ret==0)
    {
        UBOOT_DEBUG("OriArg=%s\n",OriArg);
        setenv("bootargs", OriArg);
        if(0 == bDontSaveEnv)
	    {
		    saveenv();
		}
        UBOOT_TRACE("OK\n");
    }
    else if(ret==1)
    {
        UBOOT_DEBUG("No[%s] in bootargs\n",delCfg);
    }
    else
    {
        UBOOT_ERROR("delete %s fail\n",delCfg);
    }
    free(OriArg);
    return ret;
}

int do_set_bootargs_cfg (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
   int ret;
   unsigned int prefixLen=0;
   unsigned int contentLen=0;
   char *buffer=NULL;
   UBOOT_TRACE("IN\n");
   if(argc!=3)
   {
        return cmd_usage(cmdtp);
   }
   prefixLen=strlen(argv[1]);
   contentLen=strlen(argv[2]);
   //2? for '=' and the end symbol
   #define BUFFER_SIZE (prefixLen+contentLen+2)
   buffer=malloc(BUFFER_SIZE);
   if(buffer==NULL)
   {
       UBOOT_ERROR("malloc fail\n");
       return -1;
   }
   snprintf(buffer,BUFFER_SIZE,"%s=%s",argv[1],argv[2]);
   UBOOT_DEBUG("buffer: %s\n",buffer);
   ret = set_bootargs_cfg(argv[1],buffer,0);
   if(ret==0)
   {
        UBOOT_TRACE("OK\n");
   }
   else
   {
        UBOOT_ERROR("delete %s fail\n",argv[1]);
   }
   free(buffer);
   #undef BUFFER_SIZE
   return ret;
}

int set_bootargs_cfg(char * prefix_cfg,char *setCfg,MS_BOOL bDontSaveEnv)
{
    char *preCheck=NULL;
    char *bootargs = NULL;
    char *OriArg = NULL;
    char *NewArg = NULL;
    int NewArgLen = 0;
    UBOOT_TRACE("IN\n");

    if(prefix_cfg==NULL)
    {
        UBOOT_ERROR("The input parameter 'prefix_cfg' is a null pointer\n");
        return -1;
    }

    if(setCfg==NULL)
    {
        UBOOT_ERROR("The input parameter 'setCfg' is a null pointer\n");
        return -1;
    }

    //get origin bootargs
    bootargs = getenv("bootargs");
    if (bootargs == NULL)
    {
        UBOOT_INFO("Warnning!!!!\nbootargs doesn't exist, add a new one!!\n");
        setenv("bootargs",setCfg);
        saveenv();
        return 0;
    }
    preCheck=strstr(bootargs,setCfg);
    if(preCheck!=0)
    {
        UBOOT_DEBUG("%s has already existed\n",setCfg);
        UBOOT_TRACE("OK\n");
        return 0;
    }

    OriArg = (char*)malloc(strlen(bootargs) + 1);
    if(OriArg == NULL)
    {
        UBOOT_ERROR("malloc for OriArg fail\n");
        return -1;
    }
    strncpy(OriArg, bootargs,strlen(bootargs)+1);
    UBOOT_DEBUG("OriArg=%s\n",OriArg);
    //if setCfg exist, delete it.
    del_cfg(OriArg,prefix_cfg);
    UBOOT_DEBUG("OriArg=%s\n",OriArg);
    //add the NewCfg to Arg's tail.
    NewArgLen = strlen(OriArg) + strlen(setCfg) + 2 ;
    NewArg = malloc(NewArgLen);

    if(NewArg==NULL)
    {
        free(OriArg);
        UBOOT_ERROR("malloc for NewArg fail\n");
        return -1;
    }
    UBOOT_DEBUG("OriArg=%s\n",OriArg);

    snprintf(NewArg, NewArgLen, "%s %s", OriArg, setCfg);
    setenv("bootargs", NewArg);
    if(0 == bDontSaveEnv)
    {
        saveenv();
    }
    free(OriArg);
    free(NewArg);

    UBOOT_TRACE("OK\n");
    return 0;
}

#if defined (CONFIG_POINTS_HANDLER_ENABLE)
#define INFO_EN_CHECKPOINTS_CFG     "en_chk_p"
int do_add_bootcheckpoints (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]){
    int ret = 0;
    char str[2];
    UBOOT_TRACE("IN\n");

    if(argc > 1)   // any value to disable "test customer performance index"
    {
        str[0] = '0';
    }
    else
    {
        str[0] = '1';
    }
    str[1] = '\0';

    ret = setenv("en_chk_p", str);
    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
        saveenv();
    }
    else
    {
        UBOOT_ERROR("set en_chk_p=1 to bootargs fail\n");
    }

    return ret;
}
#endif

int do_testmode_set(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    #define STR_SIZE 20
    #define OPTION_LIST "autotest","moduletest","hsl","mdebugtest"
    char TestItem[][STR_SIZE]={OPTION_LIST};
    char *TargetItem = NULL;
    char *buffer= NULL;
    int ret = 0;
    int i=0;

    UBOOT_TRACE("IN\n");
    if (argc < 2)
    {
        return cmd_usage(cmdtp);
    }
    else
    {
        TargetItem = argv[1];
    }

    // delete All TestItem in bootargs
    for(i=0; i<(sizeof(TestItem)/STR_SIZE); i++)
    {
        del_bootargs_cfg(TestItem[i],0);
    }

    // set test TargetItem into bootargs
    if(strncmp(TargetItem,"null",4)==0)
    {
        UBOOT_INFO("clear testmode in bootargs\n");
    }
    else
    {
        buffer=malloc(STR_SIZE+5);//'=true' = 6
        if(buffer==NULL)
        {
            UBOOT_ERROR("malloc fail\n");
            return -1;
        }
        sprintf(buffer,"%s=true",TargetItem);
        UBOOT_DEBUG("buffer = %s\n",buffer);
        ret=set_bootargs_cfg(TargetItem,buffer,0);
    }
    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("set %s=ture to bootargs fail\n",TargetItem);
    }
    free(buffer);
    return ret;
}

static int set_security_cfg(void)
{
    int ret = 0;
    UBOOT_TRACE("IN\n");
    ret=set_bootargs_cfg(SECURITY_ENV_CFG_PREFIX,SECURITY_ENV_CFG,0);
    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("set SECURITY=ON/OFF to bootargs fail\n");
    }
    return ret;
}

static int set_info_exchange_cfg(void)
{
    char* pInfo = getenv(INFO_EXCHANGE_CFG);
    UBOOT_TRACE("IN\n");
    if (!pInfo || strcmp(pInfo, INFO_EXCHANGE_STORAGE) != 0)
    {
        UBOOT_DEBUG("write %s=%s\n",INFO_EXCHANGE_CFG,INFO_EXCHANGE_STORAGE);
        setenv(INFO_EXCHANGE_CFG, INFO_EXCHANGE_STORAGE);
#if CONFIG_MINIUBOOT
#else
        saveenv();
#endif
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

static int set_env_cfg(void)
{
   int ret=0;
   char *buf=NULL;
   UBOOT_TRACE("IN\n");

   buf=malloc(CMD_BUF);
   if(buf==NULL)
   {
       UBOOT_ERROR("malloc fail\n");
       return -1;
   }
   memset(buf,0,CMD_BUF);
   snprintf(buf,CMD_BUF,"ENV_VAR_OFFSET=0x%x", cfg_env_offset);
   UBOOT_DEBUG("cmd=%s\n",buf);

   ret=set_bootargs_cfg("ENV_VAR_OFFSET",buf,0);
   if(ret==-1)
   {
        UBOOT_ERROR("set %s to bootargs fail\n",buf);
        free(buf);
        return -1;
   }
   memset(buf,0,CMD_BUF);
   snprintf(buf,CMD_BUF,"ENV_VAR_SIZE=0x%x", CONFIG_ENV_VAR_SIZE);
   UBOOT_DEBUG("cmd=%s\n",buf);

   ret=set_bootargs_cfg("ENV_VAR_SIZE",buf,0);
   if(ret==-1)
   {
        UBOOT_ERROR("set %s to bootargs fail\n",buf);
        free(buf);
        return -1;
   }

   ret=set_bootargs_cfg(ENV_CFG_PREFIX,ENV_CFG,0);
   if(ret==-1)
   {
        free(buf);
        UBOOT_ERROR("set %s to bootargs fail.\n",ENV_CFG);
        return -1;
   }
   free(buf);
   UBOOT_TRACE("OK\n");
   return ret;
}

#define LOGO_IN_MBOOT "BOOTLOGO_IN_MBOOT"

static int set_panelinit_cfg(void)
{
    int ret =0;
    UBOOT_TRACE("IN\n");

#if ((CONFIG_PANEL_INIT) || (CONFIG_MINIUBOOT))
    UBOOT_DEBUG("do config panel init\n");
    ret = set_bootargs_cfg(LOGO_IN_MBOOT,LOGO_IN_MBOOT,0);
#else
    UBOOT_DEBUG("no config panel init\n");
    ret = del_bootargs_cfg(LOGO_IN_MBOOT,0);
#endif
    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("set %s to bootargs fail\n",LOGO_IN_MBOOT);
    }
    return ret;
}

static void if_51OnRam_set(void)
{
    char* pInfo= NULL;
    UBOOT_TRACE("IN\n");
    pInfo = getenv(ENV_51ONRAM);
    if (pInfo == NULL)
    {
        UBOOT_DEBUG("setenv %s %s\n",ENV_51ONRAM,ENV_51ONRAM_CFG);
        setenv(ENV_51ONRAM,ENV_51ONRAM_CFG);
        saveenv();
    }
    UBOOT_TRACE("OK\n");
}

int do_config2env( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    char* pInfo;
    pInfo = NULL;
#if (CONFIG_OAD_IN_MBOOT)
    pInfo = getenv(OAD_IN_MBOOT);
    if (pInfo == NULL)
    {
        UBOOT_DEBUG("setenv %s 1\n",OAD_IN_MBOOT);
        setenv(OAD_IN_MBOOT, "1");
        saveenv();
    }
#endif

#if (CONFIG_ENABLE_RTPM)

    pInfo = getenv("rtpm");
    if(pInfo==NULL)
    {
        UBOOT_DEBUG("No env 'rtpm'\n");
        setenv("rtpm","1");
        saveenv();
    }
#else
    pInfo = getenv("rtpm");
    if(pInfo!=NULL)
    {
        UBOOT_DEBUG("Clean env 'rtpm'\n");
        run_command("setenv rtpm",0);
        saveenv();
    }
#endif

    if_51OnRam_set();

    UBOOT_TRACE("OK\n");
    return 0;
}


int do_set_bootargs( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    UBOOT_INFO("============= set bootargs ===============\n");
    set_info_exchange_cfg();
    set_panelinit_cfg();
    set_env_cfg();
    set_security_cfg();
    UBOOT_TRACE("OK\n");
    return 0;
}

int syncMmapToEnv(void)
{
    UBOOT_TRACE("IN\n");
    delMmapInfoFromEnv();
    saveMmapInfoToEnv();
    UBOOT_TRACE("OK\n");
    return 0;
}

extern unsigned int u32UbootStart;
extern unsigned int u32UbootEnd;
int checkUbootFromEnv(void)
{
    char *ptr=NULL;
    unsigned int pEnv_addr=0;
    unsigned int pEnv_len=0;

    UBOOT_TRACE("IN\n");

    ptr=getenv("E_MMAP_ID_MBOOT_MEM_USAGE_ADR");
    if (ptr!=NULL && getenv("no_compare_uboot")==NULL)
    {
        pEnv_addr=simple_strtol(ptr, NULL, 16);

        ptr=getenv("E_MMAP_ID_MBOOT_MEM_USAGE_LEN");
        if (ptr != NULL)
        {
            pEnv_len=simple_strtol(ptr, NULL, 16);
        }
        else
        {
            UBOOT_DEBUG("There is no information about MBOOT relocation LEN in mmap.ini\n");
            return -1;
        }

        pEnv_addr = PA2VA(pEnv_addr);

        if (pEnv_addr==CONFIG_UBOOT_LOADADDR)
        {
            UBOOT_DEBUG("UBOOT Memory Address and MMAP match\n");
        }
        else
        {
            UBOOT_ERROR("UBOOT Memory Address and MMAP mismatch!!!\n");
            UBOOT_ERROR("UBOOT Memory Address:0x%x\n",CONFIG_UBOOT_LOADADDR);
            UBOOT_ERROR("Address in MMAP:0x%x\n",pEnv_addr);
            UBOOT_ERROR("Skip this check mechanism by:\n");
            UBOOT_ERROR("setenv no_compare_uboot 1\n");
            jump_to_console();
        }

        if (u32UbootStart<pEnv_addr || (u32UbootEnd>pEnv_addr+pEnv_len) )
        {
            UBOOT_ERROR("UBOOT relocation ERROR\n");
            UBOOT_ERROR("Address in mmap.ini:0x%x\n",pEnv_addr);
            UBOOT_ERROR("Length in mmap.ini:0x%x\n",pEnv_len);
            UBOOT_ERROR("Start address for relocation:0x%x\n",u32UbootStart);
            UBOOT_ERROR("End address for relocation:0x%x\n",u32UbootEnd);
            UBOOT_ERROR("Skip this check mechanism by:\n");
            UBOOT_ERROR("setenv no_compare_uboot 1\n");
            jump_to_console();
        }

        UBOOT_DEBUG("UBOOT relocation correct\n");
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

int delMmapInfoFromEnv(void)
{
    return processMmapInfo(FALSE);
}

int saveMmapInfoToEnv(void)
{
    int ret =0;
    int i=0;

    ret = processMmapInfo(TRUE);

    // Deal with MIU Setting
    for(i=0;i<(sizeof(miuID)/MAX_MMAP_ID_LEN);i++)
    {
        setMiu2env(miuID[i]);
    }

    return ret;
}

int processMmapInfo(BOOLEAN save2env)
{
    #define ENV_STR_LEN 512
    int i=0;
    char *ptr=NULL;
    char tmp_str[ENV_STR_LEN]="\0";
    UBOOT_TRACE("IN\n");

    // Deal with Static mmapID
    for(i=0;i<(sizeof(mmapID)/MAX_MMAP_ID_LEN);i++)
    {
        ptr=mmapID[i];
        processMmapInfoOnEnv(ptr,save2env);
    }

    // Deal with Dynamic mmapID
    ptr=getenv("DynMmapID");
    if(ptr !=NULL)
    {
        strncpy(tmp_str,ptr,ENV_STR_LEN-1);
        ptr=strtok(tmp_str," ");
        while(ptr!=NULL)
        {
            processMmapInfoOnEnv(ptr,save2env);
            ptr=strtok(NULL," ");
        }
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

int processMmapInfoOnEnv(char *ptr, BOOLEAN save2env)
{
    U32 addr=0xFFFF;
    U32 len=0xFFFF;

    if(ptr ==NULL)
    {
       return 0;
    }
    UBOOT_DEBUG("mmapID=%s\n",ptr);
    if(save2env==TRUE)
    {
        if(0==set_mmap2env(ptr, &addr,&len))
        {
            save_addr_to_env(ptr,addr);
            save_len_to_env(ptr,len);
        }
    }
    else
    {
        del_addr_from_env(ptr);
        del_len_from_env(ptr);
    }

    return 0;
}

int syncMmap2bootargs(void)
{
    int ret=0;
    unsigned int addr=0;
    unsigned int len=0;
    char buf[CMD_BUF]="\0";

    UBOOT_TRACE("IN\n");
    //0. clean LX  information
    del_bootargs_cfg(ENV_LX_MEM_LEN,TRUE);
    del_bootargs_cfg(ENV_LX_MEM2,TRUE);
    del_bootargs_cfg(ENV_LX_MEM3,TRUE);

    //1. Sync LX_MEM
    ret=_syncLen2bootargs(MMAP_ID_LXMEM,ENV_LX_MEM_LEN);
    if(ret!=0)
    {
        UBOOT_ERROR("sync LX_MEM fail\n");
        return -1;
    }

    //2. Sync LX_MEM2
    ret=get_value_from_env(E_LX_MEM2_ADR, NO_DEFAULT_MMAP_VALUE, &addr);
    if(ret!=0)
    {
        UBOOT_INFO("No %s information \n",E_LX_MEM2_ADR);
    }
    else
    {
        ret=get_value_from_env(E_LX_MEM2_LEN, NO_DEFAULT_MMAP_VALUE, &len);
        if(ret!=0)
        {
            UBOOT_ERROR("get %s fail\n",E_LX_MEM2_LEN);
            return -1;
        }
        memset(buf,0,sizeof(buf));
        snprintf(buf,CMD_BUF,"%s=0x%llx,0x%x",ENV_LX_MEM2,PA2BA(addr),len);
        UBOOT_DEBUG("buf=%s\n",buf);
        ret=set_bootargs_cfg(ENV_LX_MEM2,buf,1);
        if(ret!=0)
        {
            UBOOT_ERROR("sync LX_MEM2 fail\n");
            return -1;
        }
    }

    //3. Sync LX_MEM3
    ret=get_value_from_env(E_LX_MEM3_ADR, NO_DEFAULT_MMAP_VALUE, &addr);
    if(ret!=0)
    {
        UBOOT_INFO("No %s information \n",E_LX_MEM3_ADR);
    }
    else
    {
        ret=get_value_from_env(E_LX_MEM3_LEN, NO_DEFAULT_MMAP_VALUE, &len);
        if(ret!=0)
        {
            UBOOT_ERROR("get %s fail\n",E_LX_MEM3_LEN);
            return -1;
        }
        memset(buf,0,sizeof(buf));
        snprintf(buf,CMD_BUF,"%s=0x%llx,0x%x",ENV_LX_MEM3,PA2BA(addr),len);
        UBOOT_DEBUG("buf=%s\n",buf);
        ret=set_bootargs_cfg(ENV_LX_MEM3,buf,1);
        if(ret!=0)
        {
            UBOOT_ERROR("sync LX_MEM3 fail\n");
            return -1;
        }
    }

    //4. Sync EMAC_MEM
    ret=_syncLen2bootargs(MMAP_ID_EMAC,ENV_EMAC_MEM_LEN);
    if(ret!=0)
    {
        UBOOT_DEBUG("No EMAC_MEM information\n");
    }
    //4.1 Sync MMAP_ID_GMAC
    ret=get_value_from_env(E_GMAC_MEM_ADR, NO_DEFAULT_MMAP_VALUE, &addr);
    if(ret!=0)
    {
        UBOOT_DEBUG("No ID_GMAC information \n");
    }
    else
    {
        ret=get_value_from_env(E_GMAC_MEM_LEN, NO_DEFAULT_MMAP_VALUE, &len);
        if(ret!=0)
        {
            UBOOT_ERROR("get %s fail\n",E_GMAC_MEM_LEN);
            return -1;
        }
        memset(buf,0,sizeof(buf));
        snprintf(buf,CMD_BUF,"%s=0x%llx,0x%x",ENV_GMAC_MEM, PA2BA(addr),len);
        UBOOT_DEBUG("buf=%s\n",buf);
        ret=set_bootargs_cfg(ENV_GMAC_MEM,buf,1);
        if(ret!=0)
        {
            UBOOT_ERROR("sync %s fail\n",ENV_GMAC_MEM);
            return -1;
        }
    }

    //5. Sync PM51_ADDR
    ret=get_value_from_env(E_MMAP_ID_PM51_USAGE_MEM_ADR, NO_DEFAULT_MMAP_VALUE, &addr);
    if(ret!=0)
    {
        UBOOT_DEBUG("No %s information\n",E_MMAP_ID_PM51_USAGE_MEM_ADR);
    }
    else
    {
        ret=_syncAddr2bootargs(MMAP_ID_PM51_USAGE,ENV_PM51_ADDR);
        if(ret!=0)
        {
            UBOOT_ERROR("get PM51_ADDR fail\n");
            return -1;
        }

        //6. Sync PM51_LEN
        ret=_syncLen2bootargs(MMAP_ID_PM51_USAGE,ENV_PM51_LEN);
        if(ret!=0)
        {
            UBOOT_ERROR("get PM51_LEN fail\n");
            return -1;
        }

    }

    UBOOT_TRACE("OK\n");
    return 0;
}

static int _syncLen2bootargs(char *id, char *envName)
{
    int ret=0;
    unsigned int len=0;
    char buf[CMD_BUF];
    UBOOT_TRACE("IN\n");
    if(id==NULL)
    {
        UBOOT_ERROR("id is a null pointer\n");
        return -1;
    }
    if(envName==NULL)
    {
        UBOOT_ERROR("envName is a null pointer\n");
        return -1;
    }

    ret=get_length_from_mmap(id, &len);
    if(ret!=0)
    {
        UBOOT_DEBUG("no %s information!!\n",envName);
        return -1;
    }
    memset(buf,0,sizeof(buf));
    snprintf(buf,CMD_BUF,"%s=0x%x",envName,len);
    UBOOT_DEBUG("buf=%s\n",buf);
    ret=set_bootargs_cfg(envName,buf,1);
    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("sync %s to bootargs fail\n",id);
    }
    return 0;
}

static int _syncAddr2bootargs(char *id, char *envName)
{
    int ret=0;
    unsigned int addr=0;
    char buf[CMD_BUF]="\0";
    UBOOT_TRACE("IN\n");
    if(id==NULL)
    {
        UBOOT_ERROR("id is a null pointer\n");
        return -1;
    }
    if(envName==NULL)
    {
        UBOOT_ERROR("envName is a null pointer\n");
        return -1;
    }

    ret=get_addr_from_mmap(id, &addr);
    if(ret!=0)
    {
        UBOOT_ERROR("get %s fail\n",envName);
        return -1;
    }

    memset(buf,0,sizeof(buf));
    snprintf(buf,CMD_BUF,"%s=0x%llx",envName,PA2BA(addr));
    UBOOT_DEBUG("buf=%s\n",buf);
    ret=set_bootargs_cfg(envName,buf,1);
    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("sync %s to bootargs fail\n",id);
    }
    return 0;
}

int setMmapInfo2Env(char *id_mmap, char *env_addr, char *env_len)
{
    char *buf=NULL;
    char *pEnv_addr=NULL;
    char *pEnv_len=NULL;
    int ret=0;
    unsigned int addr=0;
    unsigned int len=0;
    UBOOT_TRACE("IN\n");

    if((id_mmap==NULL)||(env_addr==NULL)||(env_len==NULL))
    {
        UBOOT_ERROR("One of the parameters is null pointer\n");
        return -1;
    }
    UBOOT_DEBUG("id_mmap=%s\n",id_mmap);
    UBOOT_DEBUG("env_addr=%s\n",env_addr);
    UBOOT_DEBUG("env_len=%s\n",env_len);

    pEnv_addr=getenv(env_addr);
    pEnv_len=getenv(env_len);
    if((pEnv_addr==NULL)||(pEnv_len==NULL))
    {
        buf=malloc(CMD_BUF);
        if(buf==NULL)
        {
            UBOOT_ERROR("malloc for buf fail\n");
            return -1;
        }
        memset(buf,0,CMD_BUF);

        ret=get_addr_from_mmap(id_mmap, &addr);
        if(ret!=0)
        {
            free(buf);
            UBOOT_ERROR("get %s addr fail\n",id_mmap);
            return -1;
        }

        ret=get_length_from_mmap(id_mmap, &len);
        if(ret!=0)
        {
            free(buf);
            UBOOT_ERROR("get %s len fail\n",id_mmap);
            return -1;
        }
        snprintf(buf,CMD_BUF,"setenv %s 0x%x;setenv %s 0x%x;saveenv",env_addr,addr,env_len,len);
        UBOOT_DEBUG("cmd=%s\n",buf);
        ret=run_command(buf,0);
        if(ret!=0)
        {
            free(buf);
            UBOOT_ERROR("set %s=%x to env fail\n",env_addr,addr);
            return -1;
        }
        free(buf);
    }
    else
    {
        UBOOT_DEBUG("%s has already existed\n",id_mmap);
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

int do_mmap_to_env( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret=0;
    UBOOT_TRACE("IN\n");
    if(argc!=4)
    {
        cmd_usage(cmdtp);
        return -1;
    }
    if((argv[1]==NULL)||(argv[2]==NULL)||(argv[3]==NULL))
    {
        UBOOT_ERROR("One of the parameters is null pointer\n");
        return -1;
    }
    ret=setMmapInfo2Env(argv[1],argv[2],argv[3]);
    if(ret!=0)
    {
        UBOOT_ERROR("handle %s fail\n",argv[1]);
        return -1;
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

void set_kernel_protect_information(void)
{
    char cmdBuf[CMD_BUF]="\0";
    unsigned int dram_length0=0;
    unsigned int dram_length1=0;
    unsigned int dram_length2=0;

    dram_length0=get_specific_dram_length(0);
    dram_length1=get_specific_dram_length(1);
    dram_length2=get_specific_dram_length(2);
#if ENABLE_MONACO
    snprintf(cmdBuf,CMD_BUF,"DRAM_SIZE1=0x%x DRAM_SIZE2=0x%x DRAM_SIZE3=0x%x",dram_length0,dram_length1,dram_length2);
#else
    snprintf(cmdBuf,CMD_BUF,"DRAM_SIZE1=0x%x DRAM_SIZE2=0x%x",dram_length0,dram_length1);
#endif
    UBOOT_DEBUG("command : setenv KERNEL_PROTECT %s\n",cmdBuf);
    setenv("KERNEL_PROTECT",cmdBuf);
}

int do_sync_mmap_to_env( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char cmdBuf[CMD_BUF]="\0";
    char *pEnv=NULL;
    char* force_upgrade = getenv("force_upgrade");
    unsigned char sync_mmap=0;
    UBOOT_TRACE("IN\n");

    if(NULL == force_upgrade)
    {
        if(argc!=1)
        {
            cmd_usage(cmdtp);
            return -1;
        }
        pEnv=getenv(ENV_SYNC_MMAP);
        if(pEnv!=NULL)
        {
            sync_mmap=simple_strtol(pEnv, NULL, 16);
        }
        else
        {
            UBOOT_DEBUG("no env 'sync_mmap'\n");
            sync_mmap=1;
        }

       if(sync_mmap==1)
       {
            // set bIsLoadMmapSuccess TRUE, force to reload mmap from mmap.ini
            bIsLoadMmapSuccess = TRUE;

            // sync mmap.ini to uboot environment.
            syncMmapToEnv();

            //Sync bootargs
            if(syncMmap2bootargs()==-1)
            {
                UBOOT_ERROR("sync the part of mmap info to bootargs fail\n");
                return -1;
            }

            // set CMA information to bootargs
            setCMA2bootargs();

            //setting Kernel Protect
            set_kernel_protect_information();

            // check UBOOT DRAM address and length
#if defined(__ARM__)
            checkUbootFromEnv();
#endif

            if(bIsLoadMmapSuccess == TRUE)
            {
                memset(cmdBuf,0,CMD_BUF);
                snprintf(cmdBuf,CMD_BUF,"setenv %s 0",ENV_SYNC_MMAP);
                UBOOT_DEBUG("cmdBuf=%s\n",cmdBuf);
                if(0!=run_command(cmdBuf,0))
                {
                    UBOOT_ERROR("fail\n");
                    return -1;
                }
#if CONFIG_RESCUE_ENV
                run_command("BrickTerminator backup_env_end",0);
#endif
            }
        }
        else{
            UBOOT_DEBUG("The mmap info in env is the newest\n");
        }
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

MS_BOOL Get_LX_MemInfo(LX_NUM LxMem, unsigned int *addr, unsigned int *size)
{
    int ret =FALSE;
    switch(LxMem)
    {
        case E_LX:
            if (get_value_from_env(E_LX_MEM_ADR,NO_DEFAULT_MMAP_VALUE, addr)==0)
            {
                get_value_from_env(E_LX_MEM_LEN,NO_DEFAULT_MMAP_VALUE, size);
                UBOOT_DEBUG("Lx1 start:[0x%x]Size[0x%x]\n",*addr,*size);
                ret=TRUE;
            }
            break;
        case E_LX2:
            if (get_value_from_env(E_LX_MEM2_ADR,NO_DEFAULT_MMAP_VALUE, addr)==0)
            {
                get_value_from_env(E_LX_MEM2_LEN,NO_DEFAULT_MMAP_VALUE, size);
                UBOOT_DEBUG("Lx2 start:[0x%x]Size[0x%x]\n",*addr,*size);
                ret=TRUE;
            }
            break;
        case E_LX3:
            if (get_value_from_env(E_LX_MEM3_ADR,NO_DEFAULT_MMAP_VALUE, addr)==0)
            {
                get_value_from_env(E_LX_MEM3_LEN,NO_DEFAULT_MMAP_VALUE, size);
                UBOOT_DEBUG("Lx3 start:[0x%x]Size[0x%x]\n",*addr,*size);
                ret=TRUE;
            }
            break;
        default:
            UBOOT_DEBUG("exception lxmem number %d\n",LxMem);
            break;
    }
    return ret;
}
#ifdef CONFIG_LZO
MS_U32 Get_lzo_addr_Info(void)
{
    U32 addr  =0;

    if (get_value_from_env("E_MMAP_ID_LZO_MEM_USAGE_ADR",NO_DEFAULT_MMAP_VALUE, &addr)==0)
    {
        UBOOT_DEBUG("E_MMAP_ID_LZO_MEM_USAGE_ADR start:[0x%x]\n",addr);
        return PA2VA((MS_U32)addr);
    }
    else
    {
        UBOOT_DEBUG("E_MMAP_ID_LZO_MEM_USAGE_ADR start:[0x%x]\n",CONFIG_UNLZO_DST_ADDR);
        return CONFIG_UNLZO_DST_ADDR;
    }

}
#endif
#define BOOTTIME_SBOOT_STR "BOOTTIME_SBOOT"
#define BOOTTIME_UBOOT_STR "BOOTTIME_UBOOT"
#include <stdlib.h>
#include <malloc.h>
#include "MsSystem.h"

unsigned long G_MS_BOOTTIME_SBOOT=1; // global variable for storing the boot time used in sboot (ms)
unsigned long G_MS_BOOTTIME_UBOOT=1; // global variable for storing the boot time used in sboot (ms)

void _boottime_set_to_env(void)
{
    extern int snprintf(char *str, size_t size, const char *fmt, ...);
    char *strEnv = NULL;
    const int u32EnvSize = CMD_BUF;

    G_MS_BOOTTIME_UBOOT = MsSystemGetBootTime();

    strEnv = malloc(u32EnvSize);
    if(strEnv  != NULL)
    {
        memset(strEnv , 0, u32EnvSize);
        snprintf(strEnv , u32EnvSize-1, "%s=%lu", BOOTTIME_SBOOT_STR, G_MS_BOOTTIME_SBOOT);
        if(0 != set_bootargs_cfg((char*)BOOTTIME_SBOOT_STR, strEnv, 1))
        {
            printf("%s: Error: set_bootargs_cfg failed at %d\n", __func__, __LINE__);
        }

        memset(strEnv , 0,u32EnvSize);
        snprintf(strEnv , u32EnvSize-1, "%s=%lu", BOOTTIME_UBOOT_STR, G_MS_BOOTTIME_UBOOT);
        if(0 != set_bootargs_cfg((char*)BOOTTIME_UBOOT_STR, strEnv, 1))
        {
            printf("%s: Error: set_bootargs_cfg failed at %d\n", __func__, __LINE__);
        }
        free(strEnv );
    }
}


unsigned int GetRealOffset(unsigned int u32EnvOffset)
{
    unsigned int u32Size=0;
    unsigned int u32RealOffset=0;
    int ret = 0;
#if (ENABLE_MODULE_ENV_IN_SERIAL)
    ret = getSpiSize(&u32Size);
    u32RealOffset = u32Size - (u32EnvOffset*SECTOR_SIZE);
#elif (ENABLE_MODULE_ENV_IN_NAND)
    /*char cmd_buf[CMD_BUF]="\0";
    sprintf(cmd_buf, "ubi part %s", NAND_DEFAULT_PARTITION);
    run_command(cmd_buf, 0);*/
    check_ubi_partition(NAND_DEFAULT_VOLUME,NAND_DEFAULT_PARTITION);
    ubi_get_volume_size(NAND_DEFAULT_VOLUME,&u32Size);
    UBOOT_DEBUG("u32Size : 0x%x\n",u32Size);
    UBOOT_DEBUG("ubi_get_leb_size : 0x%x\n",SECTOR_SIZE);
    u32RealOffset = u32Size - (u32EnvOffset*SECTOR_SIZE);
#elif (ENABLE_MODULE_ENV_IN_MMC)
    #include <MsMmc.h>
    ret = get_mmc_partsize(MMC_DEFAULT_PARTITION,&u32Size);
    u32RealOffset = u32Size - (u32EnvOffset*SECTOR_SIZE);
#else
    #error "please set the correct uboot environment storage!!\n"
#endif
    if(0 != ret)
    {
        UBOOT_ERROR("Error, at %d", __LINE__);
        return 0;
    }
    return u32RealOffset;
}

#if CONFIG_RESCUE_ENV
extern uchar default_environment_rescue[];
env_t *env_ptr_rescue = NULL;
extern int default_environment_rescue_size;

void env_relocate_spec_rescue (void);
int restoreenv_from_rescue(void)
{
    //NOTE: 1. write current env to flash
//    saveenv_rescue();
    //NOTE: 2. do recovery
    ssize_t    len=0;
    char *res=NULL;
    unsigned int u32EnvOffset=0;
    UBOOT_TRACE("IN\n");

    memset (env_ptr_rescue, 0, CONFIG_ENV_SIZE);
    env_relocate_spec_rescue();

    res = (char *)&(env_ptr_rescue->data);
    len = hexport_r(&env_htab_rescue, '\0', &res, ENV_SIZE);
    if (len < 0)
    {
        UBOOT_ERROR("Cannot export environment: errno = %d\n", errno);
        return -1;
    }
    MsApiChunkHeader_GetValue(CH_UBOOT_ENVIRONMENT_ROM_OFFSET,&u32EnvOffset);
    u32EnvOffset = GetRealOffset(u32EnvOffset);
    env_ptr_rescue->crc   = crc32(0, env_ptr_rescue->data, ENV_SIZE);
    int ret = raw_write((unsigned int)env_ptr_rescue, u32EnvOffset, CONFIG_ENV_SIZE);
    int retBak = raw_write((unsigned int)env_ptr_rescue, u32EnvOffset + ENV_SECTOR_SIZE, CONFIG_ENV_SIZE);
    if( (ret != 0) || (retBak != 0) )
    {
        return -1;
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

int saveenv_rescue(void)
{
    ssize_t    len=0;
    char *res=NULL;
    unsigned int u32EnvRescueOffset = 0;
    UBOOT_TRACE("IN\n");
    res = (char *)&(env_ptr_rescue->data);
    len = hexport_r(&env_htab_rescue, '\0', &res, ENV_SIZE);
    if (len < 0) {
        error("Cannot export environment: errno = %d\n", errno);
        return -1;
    }

    env_ptr_rescue->crc   = crc32(0, env_ptr_rescue->data, ENV_SIZE);
    MsApiChunkHeader_GetValue(CH_RESCURE_ENVIRONMENT_ROM_OFFSET,&u32EnvRescueOffset);
    u32EnvRescueOffset = GetRealOffset(u32EnvRescueOffset);
    int ret = raw_write((unsigned int)env_ptr_rescue,u32EnvRescueOffset , CONFIG_ENV_SIZE);
    int retBak = raw_write((unsigned int)env_ptr_rescue,u32EnvRescueOffset + ENV_SECTOR_SIZE, CONFIG_ENV_SIZE);
    if( (ret != 0) || (retBak != 0) )
    {
        return -1;
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

void env_relocate_spec_rescue (void)
{
    extern void set_default_env_rescue(const char *s);
    unsigned int u32EnvRescueOffset = 0;
    void* pEnvBuf = NULL;
    void* pEnvBufBak = NULL;
    int ret=0;
    UBOOT_TRACE("IN\n");

    pEnvBuf = malloc(CONFIG_ENV_SIZE);
    if(pEnvBuf==NULL)
    {
        UBOOT_ERROR("malloc() failed\n");
        return;
    }
    memset(pEnvBuf, 0, CONFIG_ENV_SIZE);
    MsApiChunkHeader_GetValue(CH_RESCURE_ENVIRONMENT_ROM_OFFSET,&u32EnvRescueOffset);
    u32EnvRescueOffset = GetRealOffset(u32EnvRescueOffset);
    printf("u32EnvRescueOffset = 0x%x\n",u32EnvRescueOffset);

    ret = raw_read((unsigned int)pEnvBuf, u32EnvRescueOffset, CONFIG_ENV_SIZE);
    if(ret != 0)
    {
        free(pEnvBuf);
        UBOOT_ERROR("Write CONFIG_ENV_RESCUE_ADDR Fail\n");
        return;
    }

    pEnvBufBak = malloc(CONFIG_ENV_SIZE);
    if(pEnvBufBak==NULL)
    {
        free(pEnvBuf);
        UBOOT_ERROR("malloc() failed\n");
        return;
    }
    memset(pEnvBufBak, 0, CONFIG_ENV_SIZE);

    ret = raw_read((unsigned int)pEnvBufBak, u32EnvRescueOffset+ENV_SECTOR_SIZE, CONFIG_ENV_SIZE);
    if(ret != 0)
    {
        UBOOT_ERROR("Write CONFIG_ENV_RESCUE_ADDR_BAK Fail\n");
        goto Exit;
    }

    size_t offsetCrc = offsetof(env_t, crc);
    size_t offsetData = offsetof(env_t, data);

    ulong crc = *(ulong *)(pEnvBuf + offsetCrc);
    ulong crcNew = crc32(0, pEnvBuf + offsetData, ENV_SIZE);
    ulong crcBak = *(ulong *)(pEnvBufBak + offsetCrc);
    ulong crcNewBak = crc32(0, pEnvBufBak + offsetData, ENV_SIZE);

    UBOOT_DEBUG("crc:       0x%04lx\n",  crc);//DBG
    UBOOT_DEBUG("crcNew:    0x%04lx\n",  crcNew);//DBG
    UBOOT_DEBUG("crcBak:    0x%04lx\n",  crcBak);//DBG
    UBOOT_DEBUG("crcNewBak: 0x%04lx\n",  crcNewBak);//DBG


    if ( (crc != crcNew) && (crcBak != crcNewBak) )
    {
        set_default_env_rescue("!bad CRC");
        memcpy (env_ptr_rescue->data, default_environment_rescue, default_environment_rescue_size);
        env_ptr_rescue->crc = crc32(0, env_ptr_rescue->data, ENV_SIZE);
        // gd->env_valid = 1;
        goto Exit;
    }

    if (crc != crcNew)
    {
        raw_write((unsigned int)pEnvBufBak,u32EnvRescueOffset, CONFIG_ENV_SIZE);
    }
    else if (crcBak != crcNewBak)
    {
        raw_write((unsigned int)pEnvBuf,u32EnvRescueOffset+ENV_SECTOR_SIZE, CONFIG_ENV_SIZE);
    }

    memcpy(env_ptr_rescue, (crc != crcNew) ? pEnvBufBak : pEnvBuf, CONFIG_ENV_SIZE);

    ret = env_import_rescue((const char *)env_ptr_rescue, 1);
    if (ret)
    {
        // gd->env_valid = 1;
    }
    else
    {
        UBOOT_ERROR("env_import_rescue() failed\n");
    }
    UBOOT_TRACE("OK\n");
Exit:
    free(pEnvBufBak);
    free(pEnvBuf);
}
#endif

