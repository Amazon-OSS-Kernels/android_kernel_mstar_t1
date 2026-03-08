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
#include <MsTypes.h>

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <drvBDMA.h>
#include <bootlogo/MsPoolDB.h>
#include <MsVfs.h>
#include <MsUtility.h>
#include <MsDebug.h>
#include <MsSystem.h>
#include <MsRawIO.h>
#include <drvSERFLASH.h>
#include <CusConfig.h>
#include <MsMmap.h>
#include <MsEnvironment.h>
#include <MsApiPanel.h>
#include <uboot_mmap.h>
#include <bootlogo/iniparser.h>
#include <bootlogo/MsBootLogo.h>


#ifdef UFBL_FEATURE_IDME
#include <idme.h>
#define DEFAULT_MODEL_NAME "/config/model/Customer_1.ini"
#endif

#if (CONFIG_URSA_6M30)
#include <../drivers/mstar/ursa/ursa_6m30.h>
#endif

#if (ENABLE_URSA_8 ==1 )
#include <../drivers/mstar/ursa/ursa_8.h>
#endif

#if (ENABLE_URSA_6M40==1 )
#include <../drivers/mstar/ursa/ursa_6m40.h>
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
char const *sDB_INDEX[]=
{
    "E_DB_LOGO",
    "E_DB_MUSIC",
    "E_DB_BOARD_PARA",
    "E_DB_PANEL_PARA",
    "E_DB_URSA_6M30",
    "E_DB_URSA",
    "E_DB_TCON",
    "E_DB_PNL_VB1",
    "E_DB_PNL_GOPMUX",
    "E_DB_MISC",
    "E_DB_MAX"
};
ST_DBTABLE gstDbtable;

static char gPnlPath[BUFFER_SIZE];
static char gBoardPath[BUFFER_SIZE];
static char gLogoPath[BUFFER_SIZE];
static char gMusicPath[BUFFER_SIZE];
static char gGopMuxPath[BUFFER_SIZE];
static char gTconBinPath[BUFFER_SIZE];
#if (CONFIG_URSA_UNION == 1)
static char gModulePath[BUFFER_SIZE];
#endif
#define MPoolVolumeOffset(addr, u32DbtableOffset) (addr-u32DbtableOffset)
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
pnl_VB1ChannelOrder stVB1Channel1order;
st_sys_misc_setting gstSysMiscSetting;
st_pwm_setting gstPWMSetting;
static U8 *pDbBufferAddr = 0;
static U32 u32Dbload_size = 0;

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
extern char * strncpy(char * dest,const char *src,size_t count);
extern int snprintf(char *str, size_t size, const char *fmt, ...);
extern unsigned int GetRealOffset(unsigned int);

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
int Load_DataToDbBuffer(U32 u32DbtableOffset, DB_INDEX db_index, U32 pu32DataAddr);
int Load_LogoToFlash(U32 u32DbtableOffset);
int Load_PanelSetting_ToFlash(U32 u32DbtableOffset);
int Load_BoardSetting_ToFlash(U32 u32DbtableOffset);
int Load_MusicToFlash(U32 u32DbtableOffset);
int Load_GopMuxToFlash(U32 u32DbtableOffset);
int Load_TConToFlash(U32 u32DbtableOffset);
int Load_PnlVb1ToFlash(U32 u32DbtableOffset);
int Load_PWMSetting_ToFlash(U32 u32DbtableOffset);
int Load_MiscSetting_ToFlash(U32 u32DbtableOffset);

static void _print_dbtable_info(void);
void count_data_crc32_save(U32 addr,int size,DB_INDEX db_index);
int count_data_crc32_cmp(U32 addr,int size, DB_INDEX db_index);
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

int parse_sys_ini(char* path,char* ModelPath)
{
    int ret = -1;
    char *script = NULL;
    char str[BUFFER_SIZE];
    char *s;
    U32 filesize = 0;
    UBOOT_TRACE("IN\n");

    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        goto exit;
    }
    UBOOT_INFO("sizeof( sys ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);
    // select mode via env customer_ini_path
    s = getenv ("customer_ini_path");

#ifdef UFBL_FEATURE_IDME
    UBOOT_INFO("customer_ini_path env value --> %s\n",s);

    if (!idme_get_var_external("model_name", ModelPath, BUFFER_SIZE-1))
    {
    	setenv("customer_ini_path",ModelPath);
    }
    else
    {
    	setenv("customer_ini_path",DEFAULT_MODEL_NAME);
    }

    UBOOT_INFO("customer_ini_path env value --> %s\n", ModelPath);

    ret = 0;
    //run_command("saveenv", 0);

#else
    if(s)
    {
        UBOOT_INFO("customer_ini_path env value --> %s\n",s);
        UBOOT_INFO("Half model.ini path --> %s\n",s);
        snprintf(ModelPath, strlen(s), "%s",s);
        script = loadscript(ModelPath,&filesize);
        if(script == NULL)
        {
            if(Profile_GetString("model", "gModelName", "", str, sizearray(str)))
            {
                UBOOT_INFO("Half model.ini path --> %s\n",str);
                snprintf(ModelPath, BUFFER_SIZE, "%s",str);
                UBOOT_INFO("Full model.ini  path --> %s\n",ModelPath);
            }
            setenv("customer_ini_path",ModelPath);
        }
        UBOOT_INFO("Full model.ini  path --> %s\n",ModelPath);
        ret = 0;
    }
    else
    {
        if(Profile_GetString("model", "gModelName", "", str, sizearray(str)))
        {
            UBOOT_INFO("Half model.ini path --> %s\n",str);
            snprintf(ModelPath, BUFFER_SIZE, "%s",str);
            UBOOT_INFO("Full model.ini  path --> %s\n",ModelPath);
            ret = 0;
        }
        setenv("customer_ini_path",ModelPath);
    }
#endif

exit:
    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return ret;
}

int parse_dfb_ini(char* path, st_gopmux_para *gopmux_para)
{
    int ret = 0;
    char *script = NULL;
    char str[BUFFER_SIZE];
    U32 filesize = 0;
    UBOOT_TRACE("IN\n");
	long n;

    script = loadscript(path,&filesize);
    if(script == NULL)
    {
    	ret = -1;
        goto exit;
    }
    UBOOT_INFO("sizeof( dfb ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);

	n = Profile_GetInteger("DirectFBRC", "DFBRC_LAYERCOUNTS", 0);
    gopmux_para->m_u16SettingCounts = n;
	UBOOT_DEBUG("gopmux_para->m_u16SettingCounts:%d\n", gopmux_para->m_u16SettingCounts);
	if (gopmux_para->m_u16SettingCounts > 0)
	{
        gopmux_para->m_u8SettingType = 1;

    	n = Profile_GetInteger("DirectFBRC", "DFBRC_LAYER0_GOPINDEX", 0);
        gopmux_para->m_u16Setting0GopIndex= n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting0GopIndex:%d\n", gopmux_para->m_u16Setting0GopIndex);

    	n = Profile_GetInteger("DirectFBRC", "DFBRC_LAYER1_GOPINDEX", 0);
        gopmux_para->m_u16Setting1GopIndex = n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting1GopIndex:%d\n", gopmux_para->m_u16Setting1GopIndex);

    	n = Profile_GetInteger("DirectFBRC", "DFBRC_LAYER2_GOPINDEX", 0);
        gopmux_para->m_u16Setting2GopIndex = n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting2GopIndex:%d\n", gopmux_para->m_u16Setting2GopIndex);

    	n = Profile_GetInteger("DirectFBRC", "DFBRC_LAYER3_GOPINDEX", 0);
        gopmux_para->m_u16Setting3GopIndex = n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting3GopIndex:%d\n", gopmux_para->m_u16Setting3GopIndex);

    	n = Profile_GetInteger("DirectFBRC", "DFBRC_LAYER4_GOPINDEX", 0);
        gopmux_para->m_u16Setting4GopIndex = n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting4GopIndex:%d\n", gopmux_para->m_u16Setting4GopIndex);
	}
	else
	{
        gopmux_para->m_u8SettingType = 0;

	    n = Profile_GetInteger("DirectFBRC", "DFBRC_MUXCOUNTS", 0);
        gopmux_para->m_u16SettingCounts= n;
    	UBOOT_DEBUG("gopmux_para->m_u16SettingCounts:%d\n", gopmux_para->m_u16SettingCounts);

	    n = Profile_GetInteger("DirectFBRC", "DFBRC_MUX0_GOPINDEX", 0);
        gopmux_para->m_u16Setting0GopIndex= n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting0GopIndex:%d\n", gopmux_para->m_u16Setting0GopIndex);

	    n = Profile_GetInteger("DirectFBRC", "DFBRC_MUX1_GOPINDEX", 0);
        gopmux_para->m_u16Setting1GopIndex = n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting1GopIndex:%d\n", gopmux_para->m_u16Setting1GopIndex);

	    n = Profile_GetInteger("DirectFBRC", "DFBRC_MUX2_GOPINDEX", 0);
        gopmux_para->m_u16Setting2GopIndex = n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting2GopIndex:%d\n", gopmux_para->m_u16Setting2GopIndex);

	    n = Profile_GetInteger("DirectFBRC", "DFBRC_MUX3_GOPINDEX", 0);
        gopmux_para->m_u16Setting3GopIndex = n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting3GopIndex:%d\n", gopmux_para->m_u16Setting3GopIndex);

	    n = Profile_GetInteger("DirectFBRC", "DFBRC_MUX4_GOPINDEX", 0);
        gopmux_para->m_u16Setting4GopIndex = n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting4GopIndex:%d\n", gopmux_para->m_u16Setting4GopIndex);

    }
    const U8 IP0 = 0;
    U8 gopidx = 0;
    U8 layerid = 0;
    U8 layercount = 0;
    char layername[BUFFER_SIZE];

    layercount = Profile_GetInteger("DirectFBRC", "DFBRC_MST_GOP_COUNTS", -1);
    memset(layername,0 ,sizeof(layername));

    for (layerid = 0; layerid < layercount; layerid++)
    {
        snprintf(layername, sizeof(layername), "DFBRC_MST_GOP_DSTPLANE_%d", layerid);
        if (Profile_GetInteger("DirectFBRC", layername, -1) == IP0)
        {
            snprintf(layername, sizeof(layername), "DFBRC_MST_GOP_AVAILABLE_%d", layerid);
            gopidx = Profile_GetInteger("DirectFBRC", layername, -1);

            memset(str,0 ,sizeof(str));
            snprintf(str,sizeof(str),"%u",gopidx);
            gstSysMiscSetting.m_u8LogoGopIdx = gopidx;
            UBOOT_INFO("logo -> [%s]\n",str);

            ret = 0;
            goto exit;
        }
    }

exit:
    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return ret;
}

int _Vb1Str2Array(char *str,unsigned char *pu8Vb1Array)
{
    unsigned char u8Number =0;
    char puTmp[5]={0};

    UBOOT_DEBUG("spilitting string : %s\n",str);
    while ((str=strstr(str,"0x"))!=NULL)
    {
        memcpy(puTmp,str,sizeof(puTmp)-1);
        u8Number=simple_strtoul(puTmp,NULL,16);
        //UBOOT_INFO("0x%x ",(unsigned int)u8Number);
        *(pu8Vb1Array++)=u8Number;
        str+=2;
    }
    UBOOT_INFO("\n");
    return 0;
}

void _parse_model_vb1(void)
{
    #define VB1_SECTION_NAME "VB1_Connector"
    char str[BUFFER_SIZE]={0};
    UBOOT_TRACE("IN\n");

    // init stVB1Channel1order
    memset(&stVB1Channel1order,0,sizeof(stVB1Channel1order));

    // 16V_Order
    if(Profile_GetString(VB1_SECTION_NAME, "16V_Order","", str, sizearray(str)))
    {
        _Vb1Str2Array(str,stVB1Channel1order.u8Vb116vOrder);
    }

    // 8V_Order
    if(Profile_GetString(VB1_SECTION_NAME, "8V_Order","", str, sizearray(str)))
    {
        _Vb1Str2Array(str,stVB1Channel1order.u8Vb18vOrder);
    }

    // 4V_Order
    if(Profile_GetString(VB1_SECTION_NAME, "4V_Order","", str, sizearray(str)))
    {
        _Vb1Str2Array(str,stVB1Channel1order.u8Vb14vOrder);
    }

    // 2V_Order
    if(Profile_GetString(VB1_SECTION_NAME, "2V_Order","", str, sizearray(str)))
    {
        _Vb1Str2Array(str,stVB1Channel1order.u8Vb12vOrder);
    }

    // 4O_Order
    if(Profile_GetString(VB1_SECTION_NAME, "4O_Order","", str, sizearray(str)))
    {
        _Vb1Str2Array(str,stVB1Channel1order.u8Vb14OOrder);
    }

    // 2O_Order
    if(Profile_GetString(VB1_SECTION_NAME, "2O_Order","", str, sizearray(str)))
    {
        _Vb1Str2Array(str,stVB1Channel1order.u8Vb12OOrder);
    }

    UBOOT_TRACE("OK\n");
    return;
}

int parse_model_ini(char* path, char* PnlPath, char* BoardPath, char* TconBinPath)
{
    int ret = -1;//
    char *script = NULL;
    char *panel_name = NULL;
    char str[BUFFER_SIZE];
    U32 filesize = 0;
    UBOOT_TRACE("IN\n");
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
#ifdef UFBL_FEATURE_IDME
        script = loadscript(DEFAULT_MODEL_NAME, &filesize);
        if(script == NULL)
        {
            goto exit;
        }
        else
        {
            setenv("customer_ini_path",DEFAULT_MODEL_NAME);
        }
#else
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            goto exit;
        }
#endif
    }
    UBOOT_INFO("sizeof( model ini script ) --------> %u bytes\n",filesize);

    panel_name = "m_pPanelName";

    Profile_Init((char *)script,filesize);
    if(Profile_GetString("panel", panel_name,"", str, sizearray(str)))
    {
        UBOOT_INFO("Half panel.ini path --> %s\n",str);
        snprintf(PnlPath,BUFFER_SIZE,"%s",str);
        UBOOT_INFO("Full panel.ini  path --> %s\n",PnlPath);
        ret = 0;
    }
    else
    {
        goto exit;
    }

    if(Profile_GetString("board", "m_pBoardName","", str, sizearray(str)))
    {
        UBOOT_INFO("Half board.ini path --> %s\n",str);
        snprintf(BoardPath,BUFFER_SIZE,"%s",str);
        UBOOT_INFO("Full board.ini  path --> %s\n",BoardPath);
        ret = 0;
    }
    else
    {
        goto exit;
    }
    if(Profile_GetString("BOOTINI", "BOOTINI_PATH","", str, sizearray(str)))
    {
        setenv("boot_ini_path",str);

        UBOOT_INFO("boot.ini path --> %s\n",str);
        ret = 0;
    }
    if(Profile_GetString("TCON_BIN", "TCON_FILE","", str, sizearray(str)))
    {
        snprintf(TconBinPath,BUFFER_SIZE,"%s",str);
        UBOOT_INFO("board.ini  path --> %s\n",TconBinPath);
        ret = 0;
    }
    _parse_model_vb1();

#if (CONFIG_PANEL_INIT == 1)
    memset(str,0,sizeof(str));
    int n = Profile_GetInteger("PANEL_SWING_LEVEL", "SWING_LEVEL", -1);

    if (n >= 0)
    {
        snprintf(str,sizeof(str),"%u",n);
        UBOOT_INFO("swing_level -> [%s]\n",str);
        gstSysMiscSetting.m_u16Panel_SwingLevel = n;
    }
    else
    {
        gstSysMiscSetting.m_u16Panel_SwingLevel = 250;
    }
#endif

#if (CONFIG_URSA_UNION == 1)
    memset(gModulePath,0,sizeof(gModulePath));
    if(Profile_GetString("module", "m_pModuleName","", gModulePath, sizearray(gModulePath)))
    {
        UBOOT_DEBUG("module path: %s\n", gModulePath);
    }
    else
    {
        UBOOT_ERROR("get module path error\n");
    }
#endif


    if(Profile_GetString("MISC_MIRROR_CFG", "MIRROR_OSD","", str, sizearray(str)))
    {
        UBOOT_INFO("MIRROR_OSD is  --> %s\n",str);
        if((strcmp(str, "True") == 0)||(strcmp(str, "1") == 0))
        {
            // MIRROR_OSD_TYPE   # 0:normal type.  1:Horizontal-mirror only.  2:Vertical-mirror only.  3:HV-mirror.
            if(Profile_GetString("MISC_MIRROR_CFG", "MIRROR_OSD_TYPE","", str, sizearray(str)))
            {
                UBOOT_INFO("MIRROR_OSD_TYPE is  --> %s\n",str);
                if((strcmp(str, "1") == 0))
                    gstSysMiscSetting.m_u8MirrorMode = 3;
                else if((strcmp(str, "2") == 0))
                    gstSysMiscSetting.m_u8MirrorMode = 2;
                else
                    gstSysMiscSetting.m_u8MirrorMode = 1;
            }
            else
            {
                gstSysMiscSetting.m_u8MirrorMode = 1;
            }
        }
        else if((strcmp(str, "False") == 0)||(strcmp(str, "0") == 0))
        {
            gstSysMiscSetting.m_u8MirrorMode = 0;
        }
        else
        {

            UBOOT_INFO("MIRROR_OSD is  [True] or [False], other value can not be parsed\n");
        }
    }
    else
    {
        goto exit;
    }
exit:
    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return ret;
}

#if (CONFIG_URSA_UNION == 1)
int parse_module_ini()
{
    int ret = 0;//
    char *script = NULL;
    char str[BUFFER_SIZE];
    U32 filesize = 0;
    U32 u32Urs_Type = 0;
    UBOOT_TRACE("IN\n");
    script = loadscript(gModulePath,&filesize);
    if(script == NULL)
    {
        ret = -1;
        goto exit;
    }
    UBOOT_INFO("sizeof( boot ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);
    u32Urs_Type = Profile_GetInteger("M_URSA", "F_URSA_URSA_TYPE", 0);
    gstSysMiscSetting.m_u32ursa_type = u32Urs_Type;
    UBOOT_DEBUG("ursa_type = %d \n",u32Urs_Type);
exit:
    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return ret;
}
#endif

int parse_boot_ini(char* path)
{
    int ret = 0;//
    int n = 0;
    char *script = NULL;
    char str[BUFFER_SIZE];
    U32 filesize = 0;

    UBOOT_TRACE("IN\n");
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        ret = -1;
        goto exit;
    }
    UBOOT_INFO("sizeof( boot ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);

    //Music
    if(Profile_GetString("MUSIC_CFG", "MUSIC_NAME","", str, sizearray(str)))
    {
        snprintf(gMusicPath,sizeof(gMusicPath),"%s/%s",CUSTOMER_PATH,str);
    }
    else
    {
        // use default music file
        UBOOT_DEBUG("no MUSIC_NAME in %s, use default music file\n",path);
        snprintf(gMusicPath,sizeof(gMusicPath),"%s/boot0.mp3",CUSTOMER_PATH);
    }
    UBOOT_INFO("gMusicPath --> %s\n",gMusicPath);

    //Logo
    if(Profile_GetString("LOGO_CFG", "LOGO_NAME","", str, sizearray(str)))
    {
        snprintf(gLogoPath,sizeof(gLogoPath),"%s/%s",CUSTOMER_PATH,str);
    }
    else
    {
        // use default logo file
        UBOOT_DEBUG("no LOGO_NAME in %s, use default logo file\n",path);
        snprintf(gLogoPath,sizeof(gLogoPath),"%s/boot0.jpg",CUSTOMER_PATH);
    }
    UBOOT_INFO("gLogoPath --> %s\n",gLogoPath);

    //Music volume
    n = Profile_GetInteger("MUSIC_VOL_CFG", "MUSIC_VOL", 0);
    if((n >= 0)&&(n <= 0x7F))
    {
        snprintf(str,sizeof(str),"0x%x",n);
        UBOOT_INFO("m_u32MusicVol -> [%s]\n",str);
        gstSysMiscSetting.m_u32MusicVol = n;
    }
    else
    {
        gstSysMiscSetting.m_u32MusicVol = 30;
    }

exit:
    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return ret;
}


int parse_ini(void)
{
    int ret = -1;
    char *p_str;
    char InPath [BUFFER_SIZE]="\0";
    char OutPath[BUFFER_SIZE]="\0";
    memset(gPnlPath,0,sizeof(gPnlPath));
    memset(gBoardPath,0,sizeof(gBoardPath));
    memset(gMusicPath,0,sizeof(gMusicPath));
    memset(gLogoPath,0,sizeof(gLogoPath));
    memset(gGopMuxPath,0,sizeof(gGopMuxPath));
    memset(gTconBinPath,0,sizeof(gTconBinPath));

    UBOOT_TRACE("IN\n");
    vfs_mount(CONFIG);
    snprintf(InPath,sizeof(InPath),"%s/sys.ini",CONFIG_PATH);
    ret = parse_sys_ini(InPath,OutPath);
    if(ret)
    {
        goto exit;
    }

    memset(InPath,0,sizeof(InPath));
    memcpy(InPath,OutPath,sizeof(OutPath));
    vfs_mount(CONFIG);
    ret = parse_model_ini(InPath, gPnlPath, gBoardPath, gTconBinPath);
    if(ret)
    {
        goto exit;
    }

    #if (CONFIG_URSA_UNION == 1)
    ret = parse_module_ini();
    if(ret)
    {
        goto exit;
    }
    #endif

    vfs_mount(CUSTOMER);
    memset(InPath,0,sizeof(InPath));
    p_str = getenv ("boot_ini_path");
    ret = parse_boot_ini(p_str);
    if(ret)
    {
        goto exit;
    }
exit:
    UBOOT_TRACE("OK\n");
    return ret;
}

int prepare_nand_dbtable(U32 u32DbtableOffset)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    memset((void*)&gstDbtable, 0, sizeof(gstDbtable));
    vfs_mount(CUSTOMER);

#if(ENABLE_DISPLAY_LOGO)
    gstDbtable.dbdata[E_DB_LOGO].Size = vfs_getsize(gLogoPath);
    UBOOT_DEBUG("gLogoPath=%s\n",gLogoPath);
    UBOOT_DEBUG("gstDbtable.dbdata[E_DB_LOGO].Size=0x%x\n", gstDbtable.dbdata[E_DB_LOGO].Size);

    if(gstDbtable.dbdata[E_DB_LOGO].Size > 0)
    {
        UBOOT_DEBUG("%s size =:0x%08x \n",gLogoPath, gstDbtable.dbdata[E_DB_LOGO].Size);
    }
    else
    {
        UBOOT_ERROR(" get logo_size fail...>>>:%d\n", gstDbtable.dbdata[E_DB_LOGO].Size);
        return ret;
    }
#endif

#if ENABLE_POWER_MUSIC
    //get music_size
    gstDbtable.dbdata[E_DB_MUSIC].Size = vfs_getsize(gMusicPath);
    UBOOT_DEBUG("gMusicPath=%s\n",gMusicPath);
    UBOOT_DEBUG("gstDbtable.dbdata[E_DB_MUSIC].Size=0x%x\n",gstDbtable.dbdata[E_DB_MUSIC].Size);
    if( gstDbtable.dbdata[E_DB_MUSIC].Size > 0)
    {
        UBOOT_DEBUG("%s size =:0x%08x \n",gMusicPath,gstDbtable.dbdata[E_DB_MUSIC].Size);
    }
    else
    {
        UBOOT_ERROR(" get music_size fail...>>>:%d\n",gstDbtable.dbdata[E_DB_MUSIC].Size);
        return ret; //althought music is set off, logo can be showed if it is set on
    }
#endif


#if (CONFIG_PANEL_INIT)
    gstDbtable.dbdata[E_DB_PANEL_PARA].Size = sizeof(PanelType);
    gstDbtable.dbdata[E_DB_BOARD_PARA].Size = sizeof(st_board_para);
#endif

#if(ENABLE_ENABLE_URSA == 1)
#if(ENABLE_URSA_6M30 == 1)
    gstDbtable.dbdata[E_DB_URSA_6M30].Size = sizeof(ursa_6m30_cmd_table);
#elif (ENABLE_URSA_8 == 1)|| (ENABLE_URSA_6M40 == 1)
    gstDbtable.dbdata[E_DB_URSA].Size = sizeof(ursa_cmd_table);
#endif
#endif

    gstDbtable.dbdata[E_DB_PNL_GOPMUX].Size = sizeof(st_gopmux_para);

    if(strcmp(getenv("enable_tcon_panel"), "1") == 0)
    {
        gstDbtable.dbdata[E_DB_TCON].Size = vfs_getsize(gTconBinPath);
        UBOOT_DEBUG("gLogoPath=%s\n",gTconBinPath);
        UBOOT_DEBUG("gstDbtable.dbdata[E_DB_TCON].Size=0x%x\n",gstDbtable.dbdata[E_DB_TCON].Size);

        if(gstDbtable.dbdata[E_DB_TCON].Size > 0)
        {
            UBOOT_DEBUG("%s size =:0x%08x \n",gTconBinPath,gstDbtable.dbdata[E_DB_TCON].Size);
        }
        else
        {
            UBOOT_ERROR(" get tcon_size fail...>>>:%d\n",gstDbtable.dbdata[E_DB_TCON].Size);
            return ret;
        }
    }
    gstDbtable.dbdata[E_DB_PNL_VB1].Size = sizeof(pnl_VB1ChannelOrder);
    gstDbtable.dbdata[E_DB_PWM].Size = sizeof(st_pwm_setting);
    gstDbtable.dbdata[E_DB_MISC].Size = sizeof(st_sys_misc_setting);

    //calculate gstDbtable offset
    gstDbtable.dbdata[E_DB_LOGO].Offset       = u32DbtableOffset + sizeof(gstDbtable);
    gstDbtable.dbdata[E_DB_MUSIC].Offset      = gstDbtable.dbdata[E_DB_LOGO].Offset       + gstDbtable.dbdata[E_DB_LOGO].Size;
    gstDbtable.dbdata[E_DB_BOARD_PARA].Offset = gstDbtable.dbdata[E_DB_MUSIC].Offset      + gstDbtable.dbdata[E_DB_MUSIC].Size;
    gstDbtable.dbdata[E_DB_PANEL_PARA].Offset = gstDbtable.dbdata[E_DB_BOARD_PARA].Offset + gstDbtable.dbdata[E_DB_BOARD_PARA].Size;
    gstDbtable.dbdata[E_DB_URSA_6M30].Offset  = gstDbtable.dbdata[E_DB_PANEL_PARA].Offset + gstDbtable.dbdata[E_DB_PANEL_PARA].Size;
    gstDbtable.dbdata[E_DB_URSA].Offset       = gstDbtable.dbdata[E_DB_URSA_6M30].Offset  + gstDbtable.dbdata[E_DB_URSA_6M30].Size;
    gstDbtable.dbdata[E_DB_TCON].Offset       = gstDbtable.dbdata[E_DB_URSA].Offset       + gstDbtable.dbdata[E_DB_URSA].Size;
    gstDbtable.dbdata[E_DB_PNL_VB1].Offset    = gstDbtable.dbdata[E_DB_TCON].Offset       + gstDbtable.dbdata[E_DB_TCON].Size;
    gstDbtable.dbdata[E_DB_PNL_GOPMUX].Offset = gstDbtable.dbdata[E_DB_PNL_VB1].Offset    + gstDbtable.dbdata[E_DB_PNL_VB1].Size;
    gstDbtable.dbdata[E_DB_PWM].Offset        = gstDbtable.dbdata[E_DB_PNL_GOPMUX].Offset + gstDbtable.dbdata[E_DB_PNL_GOPMUX].Size;
    gstDbtable.dbdata[E_DB_MISC].Offset       = gstDbtable.dbdata[E_DB_PWM].Offset        + gstDbtable.dbdata[E_DB_PWM].Size;


    _print_dbtable_info();

    {
        U32 used_size = 0;
        U32 mpool_size = 0;
        // Get MPool volume/partition size
        mpool_size = GetRealOffset(0);

        u32Dbload_size =  sizeof(gstDbtable)
                        + gstDbtable.dbdata[E_DB_LOGO].Size
                        + gstDbtable.dbdata[E_DB_MUSIC].Size
                        + gstDbtable.dbdata[E_DB_BOARD_PARA].Size
                        + gstDbtable.dbdata[E_DB_PANEL_PARA].Size
                        + gstDbtable.dbdata[E_DB_URSA_6M30].Size
                        + gstDbtable.dbdata[E_DB_URSA].Size
                        + gstDbtable.dbdata[E_DB_TCON].Size
                        + gstDbtable.dbdata[E_DB_PNL_VB1].Size
                        + gstDbtable.dbdata[E_DB_PNL_GOPMUX].Size
                        + gstDbtable.dbdata[E_DB_PWM].Size
                        + gstDbtable.dbdata[E_DB_MISC].Size;

        //roughly calculate mpool data used size
        used_size = u32DbtableOffset + u32Dbload_size + env_sector_size;

        UBOOT_DEBUG("u32DbtableOffset  =: 0x%x \n", u32DbtableOffset);
        UBOOT_DEBUG("u32Dbload_size    =: 0x%x \n", u32Dbload_size);
        UBOOT_DEBUG("used_size         =: 0x%x \n", used_size);
        UBOOT_DEBUG("mpool_size        =: 0x%x \n", mpool_size);

        // mpool data is over mpool volume/partition size
        if(used_size >= mpool_size)
        {
            UBOOT_ERROR("mpool data is over mpool volume/partition size\n");
            jump_to_console();
        }

    }

    pDbBufferAddr = malloc(u32Dbload_size);
    if(pDbBufferAddr == NULL)
    {
        UBOOT_ERROR("malloc dbload memory error !");
        return ret;
    }

    // copy db_table to DB Pool memory buffer
    memset((void*)pDbBufferAddr,0,u32Dbload_size);
    ret = 0;


    UBOOT_TRACE("OK\n");
    return ret;
}

int write_dbtable(U32 u32DbtableOffset)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    memset(&gstSysMiscSetting,0,sizeof(st_sys_misc_setting));
    memset(&gstPWMSetting,0,sizeof(st_pwm_setting));

    parse_ini();
    if(prepare_nand_dbtable(u32DbtableOffset)==-1)
    {
        if(pDbBufferAddr)
        {
            free(pDbBufferAddr);
            pDbBufferAddr = NULL;
        }
        UBOOT_ERROR("flash database not ready...>>>\n");
        return ret;
    }
    else
    {

#if ENABLE_DISPLAY_LOGO
        Load_LogoToFlash(u32DbtableOffset);
#endif
#if ENABLE_POWER_MUSIC
        Load_MusicToFlash(u32DbtableOffset);
#endif
#if (CONFIG_PANEL_INIT)

        char *p_str = NULL;
        p_str = getenv ("panel_path");
        if(NULL != p_str)
        {
            memcpy(gPnlPath, p_str, sizeof(gPnlPath));
        }
        Load_PanelSetting_ToFlash(u32DbtableOffset);
        Load_BoardSetting_ToFlash(u32DbtableOffset);
#endif
#if ENABLE_ENABLE_URSA
#if ENABLE_URSA_6M30
        //Load_Ursa6m30Para_ToFlash();
#elif (ENABLE_URSA_8 == 1) || (ENABLE_URSA_6M40 == 1)
        Load_UrsaPara_ToFlash(u32DbtableOffset);
#endif
#endif
        Load_GopMuxToFlash(u32DbtableOffset);

        if(strcmp(getenv("enable_tcon_panel"), "1") == 0)
        {
            Load_TConToFlash(u32DbtableOffset);
        }

        Load_PnlVb1ToFlash(u32DbtableOffset);
        Load_PWMSetting_ToFlash(u32DbtableOffset);
        Load_MiscSetting_ToFlash(u32DbtableOffset);

        //calculate crc32 of gstDbtable
        gstDbtable.u32CrcCheck = crc32(0, (unsigned char *)&gstDbtable, sizeof(ST_DBDATA)*E_DB_MAX);
        UBOOT_DEBUG("gstDbtable.u32CrcCheck=%08x\n",gstDbtable.u32CrcCheck);

        // copy db_table to DB Pool memory buffer
        memcpy((void*)pDbBufferAddr,(const void *)&gstDbtable,sizeof(gstDbtable));

        // save DB Pool data to flash
        ret = raw_write((unsigned int)(pDbBufferAddr), u32DbtableOffset, u32Dbload_size);
        if(ret == -1)
        {
            UBOOT_ERROR("write data to flash fail...>>>\n");
        }
        else
        {
            UBOOT_TRACE("OK\n");
        }
        if(pDbBufferAddr)
        {
            free(pDbBufferAddr);
            pDbBufferAddr = NULL;
        }

        setenv("db_table","1");
        saveenv();
        ret = 0;
    }
    UBOOT_TRACE("OK\n");
    return ret;
}

void _print_dbtable_info(void)
{
    UBOOT_DEBUG("read_dbtable success...>>>\n");
    UBOOT_DEBUG("read table >>> \n");
    UBOOT_DEBUG("logo  offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_LOGO].Offset,        gstDbtable.dbdata[E_DB_LOGO].Size,       (gstDbtable.dbdata[E_DB_LOGO].Offset       + gstDbtable.dbdata[E_DB_LOGO].Size));
    UBOOT_DEBUG("music offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_MUSIC].Offset,       gstDbtable.dbdata[E_DB_MUSIC].Size,      (gstDbtable.dbdata[E_DB_MUSIC].Offset      + gstDbtable.dbdata[E_DB_MUSIC].Size));
    UBOOT_DEBUG("board offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_BOARD_PARA].Offset,  gstDbtable.dbdata[E_DB_BOARD_PARA].Size, (gstDbtable.dbdata[E_DB_BOARD_PARA].Offset + gstDbtable.dbdata[E_DB_BOARD_PARA].Size));
    UBOOT_DEBUG("panel offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_PANEL_PARA].Offset,  gstDbtable.dbdata[E_DB_PANEL_PARA].Size, (gstDbtable.dbdata[E_DB_PANEL_PARA].Offset + gstDbtable.dbdata[E_DB_PANEL_PARA].Size));
    UBOOT_DEBUG("ursa 6m30  offset[0x%08x] size[0x%08x] end[0x%08x]\n", gstDbtable.dbdata[E_DB_URSA_6M30].Offset,   gstDbtable.dbdata[E_DB_URSA_6M30].Size,  (gstDbtable.dbdata[E_DB_URSA_6M30].Offset  + gstDbtable.dbdata[E_DB_URSA_6M30].Size));
    UBOOT_DEBUG("ursa  offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_URSA].Offset,        gstDbtable.dbdata[E_DB_URSA].Size,       (gstDbtable.dbdata[E_DB_URSA].Offset       + gstDbtable.dbdata[E_DB_URSA].Size));
    UBOOT_DEBUG("tcon  offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_TCON].Offset,        gstDbtable.dbdata[E_DB_TCON].Size,       (gstDbtable.dbdata[E_DB_TCON].Offset       + gstDbtable.dbdata[E_DB_TCON].Size));
    UBOOT_DEBUG("vb1   offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_PNL_VB1].Offset,     gstDbtable.dbdata[E_DB_PNL_VB1].Size,    (gstDbtable.dbdata[E_DB_PNL_VB1].Offset    + gstDbtable.dbdata[E_DB_PNL_VB1].Size));
    UBOOT_DEBUG("gopmux  offset[0x%08x] size[0x%08x] end[0x%08x]\n",    gstDbtable.dbdata[E_DB_PNL_GOPMUX].Offset,  gstDbtable.dbdata[E_DB_PNL_GOPMUX].Size, (gstDbtable.dbdata[E_DB_PNL_GOPMUX].Offset + gstDbtable.dbdata[E_DB_PNL_GOPMUX].Size));
    UBOOT_DEBUG("pwm   offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_PWM].Offset,         gstDbtable.dbdata[E_DB_PWM].Size,        (gstDbtable.dbdata[E_DB_PWM].Offset        + gstDbtable.dbdata[E_DB_PWM].Size));
    UBOOT_DEBUG("misc  offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_MISC].Offset,        gstDbtable.dbdata[E_DB_MISC].Size,       (gstDbtable.dbdata[E_DB_MISC].Offset       + gstDbtable.dbdata[E_DB_MISC].Size));
}
int read_dbtable(U32 u32DbtableOffset)
{
    if(-1 == raw_read((U32)&gstDbtable,u32DbtableOffset,sizeof(gstDbtable)))
    {
        UBOOT_ERROR("ERROR: read_mmc_dbtable fail...>>>\n");
        return -1;
    }
    else
    {
        _print_dbtable_info();
    }
    U32 checksum = 0,savesum = 0;
    checksum = crc32(0, (unsigned char *)&gstDbtable, sizeof(ST_DBDATA)*E_DB_MAX);
    savesum = gstDbtable.u32CrcCheck;

    if(checksum != savesum)
    {
        UBOOT_ERROR("Error : savesum =%08x , checksum= %08x \n",savesum,checksum);
        UBOOT_ERROR("Reload the dbtable from AP !!!\n");
        return write_dbtable(u32DbtableOffset);
    }
    return 0;
}


void count_data_crc32_save(U32 addr,int size,DB_INDEX db_index)
{
    gstDbtable.dbdata[db_index].u32CrcCheck = crc32(0,(unsigned char *)addr,size);
    UBOOT_DEBUG("%s checksum =: 0x%08x\n",sDB_INDEX[db_index],gstDbtable.dbdata[db_index].u32CrcCheck);
}

int count_data_crc32_cmp(U32 addr,int size, DB_INDEX db_index)
{
    int ret = -1;
    U32 checksum,savesum;
    checksum = crc32(0,(unsigned char *)addr,size);

    savesum = gstDbtable.dbdata[db_index].u32CrcCheck;

    if(checksum == savesum)
    {
        UBOOT_DEBUG("## CRC check done, %s\n", sDB_INDEX[db_index]);
        ret = 0;
    }
    else
    {
        UBOOT_ERROR("checksum :0X%x ; savesum :0X%x\n",checksum,savesum);
        UBOOT_ERROR("## CRC check Error, %s\n", sDB_INDEX[db_index]);
        setenv("db_table","0");
        saveenv();
    }
    return ret;
}

int Read_FlashDataToAddr(U32 u32DstAddr, DB_INDEX db_index)
{
	int ret = -1;
    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("gstDbtable.dbdata[%s] Offset =:0x%x ... Size=: 0x%x\n",
                sDB_INDEX[db_index],
                gstDbtable.dbdata[db_index].Offset,
                gstDbtable.dbdata[db_index].Size);

    if(-1 == raw_read(u32DstAddr, gstDbtable.dbdata[db_index].Offset, gstDbtable.dbdata[db_index].Size))
    {
        UBOOT_ERROR("ERROR: Read_FlashDataToAddr fail...>>>\n");
    }
    else
    {
        UBOOT_DEBUG("Read_FlashDataToAddr success...>>>\n");
        //crc check
        if(count_data_crc32_cmp(u32DstAddr,gstDbtable.dbdata[db_index].Size,db_index)==-1)
        {
            UBOOT_ERROR("check FlashDataToAddr fail..>>>\n");
            return ret;
        }
        else
        {
            ret = 0;
        }
    }
    UBOOT_TRACE("OK\n");
    return ret;
}

int Load_DataToDbBuffer(U32 u32DbtableOffset, DB_INDEX db_index, U32 pu32DataAddr)
{
	UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("gstDbtable.dbdata[%s] Offset =:0x%x ... Size=: 0x%x\n",
                sDB_INDEX[db_index],
                gstDbtable.dbdata[db_index].Offset,
                gstDbtable.dbdata[db_index].Size);
	// copy data to db pool memory buffer
    memcpy( (void*)(pDbBufferAddr + MPoolVolumeOffset(gstDbtable.dbdata[db_index].Offset,u32DbtableOffset)),
            (void*)pu32DataAddr,
            gstDbtable.dbdata[db_index].Size);

    count_data_crc32_save(pu32DataAddr,gstDbtable.dbdata[db_index].Size,db_index);
	UBOOT_TRACE("OK\n");
	return 0;
}

#if ENABLE_DISPLAY_LOGO
int Load_LogoToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    unsigned int *pu32LogoAddr=NULL;
    unsigned int u32fileSize   = 0;
    UBOOT_TRACE("IN\n");

    vfs_mount(CUSTOMER);
    u32fileSize=vfs_getsize(gLogoPath);
    UBOOT_DEBUG("u32fileSize=0x%x;gstDbtable.dbdata[E_DB_LOGO].Size=0x%x\n",u32fileSize,gstDbtable.dbdata[E_DB_LOGO].Size);
    pu32LogoAddr=malloc(u32fileSize);
    if(pu32LogoAddr == NULL)
    {
        UBOOT_ERROR("malloc pu32LogoAddr memory error !");
        return -1;
    }
    memset((void*)pu32LogoAddr,0,gstDbtable.dbdata[E_DB_LOGO].Size);
    if (vfs_read((void *)pu32LogoAddr,gLogoPath,0,u32fileSize) == 0)
    {
        ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_LOGO, (U32)pu32LogoAddr);
    }
    else
    {
        UBOOT_ERROR("ubifsload data fail ...>>>\n");
    }

    free(pu32LogoAddr);
    pu32LogoAddr = NULL;
    UBOOT_TRACE("OK\n");
    return ret;
}
int Read_LogoToDisplayAddr(U32 u32LogoAddr)
{
    int ret = -1;
    ret = Read_FlashDataToAddr(u32LogoAddr, E_DB_LOGO);
    return ret;
}
#endif

#if ENABLE_POWER_MUSIC
int Load_MusicToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    unsigned int *pu32MusicAddr=NULL;
    U32 u32fileSize=0;
    UBOOT_TRACE("IN\n");
    vfs_mount(CUSTOMER);
    u32fileSize=vfs_getsize(gMusicPath);
    UBOOT_DEBUG("u32fileSize=0x%x;gstDbtable.dbdata[E_DB_MUSIC].Size=0x%x\n",u32fileSize,gstDbtable.dbdata[E_DB_MUSIC].Size);
    pu32MusicAddr = malloc(u32fileSize);
    if(pu32MusicAddr == NULL)
    {
        UBOOT_ERROR("malloc pu32MusicAddr memory error !");
        return -1;
    }
    memset((void*)pu32MusicAddr,0,gstDbtable.dbdata[E_DB_MUSIC].Size);
    if (vfs_read((void *)pu32MusicAddr,gMusicPath,0,u32fileSize) == 0)
    {
        ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_MUSIC, (U32)pu32MusicAddr);
    }
    else
    {
        UBOOT_ERROR("load mp3 file faile>>>>>>>>>>\n");
    }
    free(pu32MusicAddr);
    pu32MusicAddr = NULL;
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_MusicToPlayAddr(MS_U32 u32MusicDramAddr)
{
    int ret = -1;
    U32 u32MusicAddr = (U32)(PA2NVA(u32MusicDramAddr));
    // Clear ES1 buffer = 64K
    memset((void*)(u32MusicAddr), 0, 65536);
    ret = Read_FlashDataToAddr(u32MusicAddr, E_DB_MUSIC);
    return ret;
}

#endif

#if (CONFIG_PANEL_INIT)
int parse_pnl_ini(char *path,PanelType *p_data)
{
    long n;
    char *script = NULL;
    char str[BUFFER_SIZE];
    U32 filesize = 0;
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            return -1;
        }
    }
    UBOOT_INFO("sizeof( panel ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);
    n = Profile_GetInteger("panel", "m_bPanelDither", 0);
    UBOOT_DEBUG("m_bPanelDither = %ld \n",n);
    p_data->m_bPanelDither = n;
    memset(str,0,sizeof(str));
    Profile_GetString("panel", "m_pPanelName","", str, sizearray(str));

    n = Profile_GetInteger("panel", "m_ePanelLinkType", 0);
    UBOOT_DEBUG("m_ePanelLinkType = %ld \n",n);
    p_data->m_ePanelLinkType = (APIPNL_LINK_TYPE)n;
    //Profile_GetString("panel", "m_ePanelLinkExtType","", str, sizearray(str));
    n = Profile_GetInteger("panel", "m_ePanelLinkExtType", 0);
    UBOOT_DEBUG("m_ePanelLinkExtType = %ld \n",n);
//    setenv("panel_ext_type", str);
    gstSysMiscSetting.m_u16Panel_ext_type = n;
	Profile_GetString("panel", "m_u16PANEL_LVDS_CONNECT_TYPE","", str, sizearray(str));
    UBOOT_DEBUG("m_u16PANEL_LVDS_CONNECT_TYPE = %s \n",str);
    strncpy(gstSysMiscSetting.m_Panel_LVDS_Connect_Type, str, sizeof(gstSysMiscSetting.m_Panel_LVDS_Connect_Type));
    //saveenv();

    n = Profile_GetInteger("panel", "m_bPanelDualPort", 0);
    UBOOT_DEBUG("m_bPanelDualPort = %ld \n",n);
    p_data->m_bPanelDualPort = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapPort", 0);
    UBOOT_DEBUG("m_bPanelSwapPort  = %ld \n",n);
    p_data->m_bPanelSwapPort = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapOdd_ML", 0);
    UBOOT_DEBUG("m_bPanelSwapOdd_ML = %ld \n",n);
    p_data->m_bPanelSwapOdd_ML  = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapEven_ML", 0);
    UBOOT_DEBUG("m_bPanelSwapEven_ML = %ld \n",n);
    p_data->m_bPanelSwapEven_ML = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapOdd_RB", 0);
    UBOOT_DEBUG("m_bPanelSwapOdd_RB = %ld \n",n);
    p_data->m_bPanelSwapOdd_RB = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapEven_RB", 0);
    UBOOT_DEBUG("m_bPanelSwapEven_RB = %ld \n",n);
    p_data->m_bPanelSwapEven_RB = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapLVDS_POL", 0);
    UBOOT_DEBUG("m_bPanelSwapLVDS_POL = %ld \n",n);
    p_data->m_bPanelSwapLVDS_POL = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapLVDS_CH", 0);
    UBOOT_DEBUG("m_bPanelSwapLVDS_CH = %ld \n",n);
    p_data->m_bPanelSwapLVDS_CH = n;
    n = Profile_GetInteger("panel", "m_bPanelPDP10BIT", 0);
    UBOOT_DEBUG("m_bPanelPDP10BIT = %ld \n",n);
    p_data->m_bPanelPDP10BIT = n;
    n = Profile_GetInteger("panel", "m_bPanelLVDS_TI_MODE", 0);
    UBOOT_DEBUG("m_bPanelLVDS_TI_MODE = %ld \n",n);
    p_data->m_bPanelLVDS_TI_MODE = n;
    n = Profile_GetInteger("panel", "m_ucPanelDCLKDelay", 0);
    UBOOT_DEBUG("m_ucPanelDCLKDelay = %ld \n",n);
    p_data->m_ucPanelDCLKDelay = n;
    n = Profile_GetInteger("panel", "m_bPanelInvDCLK", 0);
    UBOOT_DEBUG("m_bPanelInvDCLK = %ld \n",n);
    p_data->m_bPanelInvDCLK = n;
    n = Profile_GetInteger("panel", "m_bPanelInvDE", 0);
    UBOOT_DEBUG("m_bPanelInvDE = %ld \n",n);
    p_data->m_bPanelInvDE = n;
    n = Profile_GetInteger("panel", "m_bPanelInvHSync", 0);
    UBOOT_DEBUG("m_bPanelInvHSync = %ld \n",n);
    p_data->m_bPanelInvHSync = n;
    n = Profile_GetInteger("panel", "m_bPanelInvVSync", 0);
    UBOOT_DEBUG("m_bPanelInvVSync = %ld \n",n);
    p_data->m_bPanelInvVSync = n;
    n = Profile_GetInteger("panel", "m_ucPanelDCKLCurrent", 0);
    UBOOT_DEBUG("m_ucPanelDCKLCurrent = %ld \n",n);
    p_data->m_ucPanelDCKLCurrent = n;
    n = Profile_GetInteger("panel", "m_ucPanelDECurrent", 0);
    UBOOT_DEBUG("m_ucPanelDECurrent = %ld \n",n);
    p_data->m_ucPanelDECurrent = n;
    n = Profile_GetInteger("panel", "m_ucPanelODDDataCurrent", 0);
    UBOOT_DEBUG("m_ucPanelODDDataCurrent = %ld \n",n);
    p_data->m_ucPanelODDDataCurrent = n;
    n = Profile_GetInteger("panel", "m_ucPanelEvenDataCurrent", 0);
    UBOOT_DEBUG("m_ucPanelEvenDataCurrent = %ld \n",n);
    p_data->m_ucPanelEvenDataCurrent = n;
    n = Profile_GetInteger("panel", "m_wPanelOnTiming1", 0);
    UBOOT_DEBUG("m_wPanelOnTiming1 = %ld \n",n);
    p_data->m_wPanelOnTiming1 = n;
    n = Profile_GetInteger("panel", "m_wPanelOnTiming2", 0);
    UBOOT_DEBUG("m_wPanelOnTiming2 = %ld \n",n);
    p_data->m_wPanelOnTiming2 = n;
    n = Profile_GetInteger("panel", "m_wPanelOffTiming1", 0);
    UBOOT_DEBUG("m_wPanelOffTiming1 = %ld \n",n);
    p_data->m_wPanelOffTiming1 = n;
    n = Profile_GetInteger("panel", "m_wPanelOffTiming2", 0);
    UBOOT_DEBUG("m_wPanelOffTiming2 = %ld \n",n);
    p_data->m_wPanelOffTiming2 = n;
    n = Profile_GetInteger("panel", "m_ucPanelHSyncWidth", 0);
    UBOOT_DEBUG("m_ucPanelHSyncWidth  = %ld \n",n);
    p_data->m_ucPanelHSyncWidth = n;
    n = Profile_GetInteger("panel", "m_ucPanelHSyncBackPorch", 0);
    UBOOT_DEBUG("m_ucPanelHSyncBackPorch  = %ld \n",n);
    p_data->m_ucPanelHSyncBackPorch = n;

    n = Profile_GetInteger("panel", "m_ucPanelVSyncWidth", 0);
    UBOOT_DEBUG("m_ucPanelVSyncWidth = %ld \n",n);
    p_data->m_ucPanelVSyncWidth = n;
    n = Profile_GetInteger("panel", "m_ucPanelVBackPorch", 0);
    UBOOT_DEBUG("m_ucPanelVBackPorch = %ld \n",n);
    p_data->m_ucPanelVBackPorch = n;

    n = Profile_GetInteger("panel", "m_wPanelHStart", 0);
    UBOOT_DEBUG("m_wPanelHStart = %ld \n",n);
    p_data->m_wPanelHStart = n;
    n = Profile_GetInteger("panel", "m_wPanelVStart", 0);
    UBOOT_DEBUG("m_wPanelVStart = %ld \n",n);
    p_data->m_wPanelVStart = n;
    n = Profile_GetInteger("panel", "m_wPanelWidth", 0);
    UBOOT_DEBUG("m_wPanelWidth = %ld \n",n);
    p_data->m_wPanelWidth = n;
    n = Profile_GetInteger("panel", "m_wPanelHeight", 0);
    UBOOT_DEBUG("m_wPanelHeight = %ld \n",n);
    p_data->m_wPanelHeight = n;

    n = Profile_GetInteger("panel", "m_wPanelMaxHTotal", 0);
    UBOOT_DEBUG("m_wPanelMaxHTotal = %ld \n",n);
    p_data->m_wPanelMaxHTotal = n;
    n = Profile_GetInteger("panel", "m_wPanelHTotal", 0);
    UBOOT_DEBUG("m_wPanelHTotal = %ld \n",n);
    p_data->m_wPanelHTotal = n;
    n = Profile_GetInteger("panel", "m_wPanelMinHTotal", 0);
    UBOOT_DEBUG("m_wPanelMinHTotal = %ld \n",n);
    p_data->m_wPanelMinHTotal = n;
    n = Profile_GetInteger("panel", "m_wPanelMaxVTotal", 0);
    UBOOT_DEBUG("m_wPanelMaxVTotal = %ld \n",n);
    p_data->m_wPanelMaxVTotal = n;
    n = Profile_GetInteger("panel", "m_wPanelVTotal", 0);
    UBOOT_DEBUG("m_wPanelVTotal = %ld \n",n);
    p_data->m_wPanelVTotal = n;

    n = Profile_GetInteger("panel", "m_wPanelMinVTotal", 0);
    UBOOT_DEBUG("m_wPanelMinVTotal = %ld \n",n);
    p_data->m_wPanelMinVTotal = n;
    n = Profile_GetInteger("panel", "m_dwPanelMaxDCLK", 0);
    UBOOT_DEBUG("m_dwPanelMaxDCLK = %ld \n",n);
    p_data->m_dwPanelMaxDCLK = n;
    n = Profile_GetInteger("panel", "m_dwPanelDCLK", 0);
    UBOOT_DEBUG("m_dwPanelDCLK = %ld \n",n);
    p_data->m_dwPanelDCLK = n;
    gstSysMiscSetting.m_u16PanelDCLK = n;

    n = Profile_GetInteger("panel", "m_dwPanelMinDCLK", 0);
    UBOOT_DEBUG("m_dwPanelMinDCLK = %ld \n",n);
    p_data->m_dwPanelMinDCLK = n;
    n = Profile_GetInteger("panel", "m_wSpreadSpectrumStep", 0);
    UBOOT_DEBUG("m_wSpreadSpectrumStep = %ld \n",n);
    p_data->m_wSpreadSpectrumStep = n;
    n = Profile_GetInteger("panel", "m_wSpreadSpectrumSpan", 0);
    UBOOT_DEBUG("m_wSpreadSpectrumSpan = %ld \n",n);
    p_data->m_wSpreadSpectrumSpan = n;
    n = Profile_GetInteger("panel", "m_ucDimmingCtl", 0);
    UBOOT_DEBUG("m_ucDimmingCtl = %ld \n",n);
    p_data->m_ucDimmingCtl = n;
    n = Profile_GetInteger("panel", "m_ucMaxPWMVal", 0);
    UBOOT_DEBUG("m_ucMaxPWMVal = %ld \n",n);
    p_data->m_ucMaxPWMVal = n;
    n = Profile_GetInteger("panel", "m_ucMinPWMVal", 0);
    UBOOT_DEBUG("m_ucMinPWMVal = %ld \n",n);
    p_data->m_ucMinPWMVal = n;
    n = Profile_GetInteger("panel", "m_bPanelDeinterMode", 0);
    UBOOT_DEBUG("m_bPanelDeinterMode = %ld \n",n);
    p_data->m_bPanelDeinterMode = n;
    n = Profile_GetInteger("panel", "m_ucPanelAspectRatio", 0);
    UBOOT_DEBUG("m_ucPanelAspectRatio = %ld \n",n);
    p_data->m_ucPanelAspectRatio = (E_PNL_ASPECT_RATIO)n;

    n = Profile_GetInteger("panel", "m_u16LVDSTxSwapValue", 0);
    UBOOT_DEBUG("m_u16LVDSTxSwapValue = %ld \n",n);
    p_data->m_u16LVDSTxSwapValue = n;
    n = Profile_GetInteger("panel", "m_ucTiBitMode", 0);
    UBOOT_DEBUG("m_ucTiBitMode = %ld \n",n);
    p_data->m_ucTiBitMode = (APIPNL_TIBITMODE)n;

    n = Profile_GetInteger("panel", "m_ucOutputFormatBitMode", 0);
    UBOOT_DEBUG("m_ucOutputFormatBitMode = %ld \n",n);
    p_data->m_ucOutputFormatBitMode = (APIPNL_OUTPUTFORMAT_BITMODE)n;

    n = Profile_GetInteger("panel", "m_bPanelSwapOdd_RG", 0);
    UBOOT_DEBUG("m_bPanelSwapOdd_RG = %ld \n",n);
    p_data->m_bPanelSwapOdd_RG = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapEven_RG", 0);
    UBOOT_DEBUG("m_bPanelSwapEven_RG = %ld \n",n);
    p_data->m_bPanelSwapEven_RG = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapOdd_GB", 0);
    UBOOT_DEBUG("m_bPanelSwapOdd_GB = %ld \n",n);
    p_data->m_bPanelSwapOdd_GB = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapEven_GB", 0);
    UBOOT_DEBUG("m_bPanelSwapEven_GB = %ld \n",n);
    p_data->m_bPanelSwapEven_GB = n;
    n = Profile_GetInteger("panel", "m_bPanelDoubleClk", 0);
    UBOOT_DEBUG("m_bPanelDoubleClk = %ld \n",n);
    p_data->m_bPanelDoubleClk = n;
    n = Profile_GetInteger("panel", "m_dwPanelMaxSET", 0);
    UBOOT_DEBUG("m_dwPanelMaxSET = %ld \n",n);
    p_data->m_dwPanelMaxSET = n;
    n = Profile_GetInteger("panel", "m_dwPanelMinSET", 0);
    UBOOT_DEBUG("m_dwPanelMinSET = %ld \n",n);
    p_data->m_dwPanelMinSET = n;
    n = Profile_GetInteger("panel", "m_ucOutTimingMode", 0);
    UBOOT_DEBUG("m_ucOutTimingMode = %ld \n",n);
    p_data->m_ucOutTimingMode = (APIPNL_OUT_TIMING_MODE)n;
#if 1//use pwm setting in panel.ini and save it to env
    n = Profile_GetInteger("panel", "u32PeriodPWM", 0);
    UBOOT_INFO("u32PeriodPWM = %ld \n",n);
    gstPWMSetting.m_u32PWMPeriod = n;


    n = Profile_GetInteger("panel", "u16DivPWM", 0);
    UBOOT_DEBUG("u16DivPWM = %ld \n",n);
    gstPWMSetting.m_u16DivPWM = n;

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "u32DutyPWM", 0);// Duty use Div value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_INFO("u32DutyPWM = %s\n",str);
    gstPWMSetting.m_u32PWMDuty = n;

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "bPolPWM", 0);// Duty use polpwm value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bPolPWM = %s\n",str);
    gstPWMSetting.m_bPolPWM = n;

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "u16MaxPWMvalue", 0);// Duty use u16MaxPWMvalue value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u16MaxPWMvalue = %s\n",str);
    gstPWMSetting.m_u16maxPWM = n;

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "u16MinPWMvalue", 0);// Duty use u16MinPWMvalue value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u16MinPWMvalue = %s\n",str);
    gstPWMSetting.m_u16minPWM = n;
    memset(str,0,sizeof(str));

#endif
    if(script)
        free(script);
    return 0;
}

int Load_PanelSetting_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    PanelType pnl;
    UBOOT_TRACE("IN\n");
    memset(&pnl,0,sizeof(pnl));
    vfs_mount(CONFIG);

    if(parse_pnl_ini(gPnlPath,&pnl)!=0)
    {
        UBOOT_ERROR("parse_pnl_ini fail !!\n");
        return ret;
    }

    UBOOT_DEBUG("panel para size =: 0x%08x \n",sizeof(pnl));
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_PANEL_PARA, (U32)&pnl);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_PanelParaFromflash(PanelType * panel_data)
{
    int ret = -1;
    ret = Read_FlashDataToAddr((U32)panel_data, E_DB_PANEL_PARA);
    UBOOT_DEBUG("panel_data.m_wPanelVStart=: 0x%x\n",panel_data->m_wPanelVStart);
    UBOOT_DEBUG("panel_data.m_wPanelWidth=: 0x%x\n",panel_data->m_wPanelWidth);
    UBOOT_DEBUG("panel_data.m_wPanelHeight=: 0x%x\n",panel_data->m_wPanelHeight);
    return ret;
}

int parse_board_ini(char *path, st_board_para *p_data)
{
    long n;
    char *script = NULL;
    U32 filesize = 0;
    char str[BUFFER_SIZE];
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            return -1;
        }
    }

    memset(str,0,sizeof(str));
    UBOOT_INFO("sizeof( board ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);

    n = Profile_GetInteger("PanelRelativeSetting", "m_u8BOARD_PWM_PORT", 2);
    UBOOT_DEBUG("m_u8BOARD_PWM_PORT = %ld \n",n);
    p_data->m_u8BOARD_PWM_PORT = n;
    gstPWMSetting.m_u16PWMPort = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_u16BOARD_LVDS_CONNECT_TYPE", 0);
    UBOOT_DEBUG("m_u16BOARD_LVDS_CONNECT_TYPE = %ld \n",n);
    p_data->m_u16BOARD_LVDS_CONNECT_TYPE = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_bPANEL_PDP_10BIT", 0);
    UBOOT_DEBUG("m_bPANEL_PDP_10BIT = %ld \n",n);
    p_data->m_bPANEL_PDP_10BIT = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_bPANEL_SWAP_LVDS_POL", 0);
    UBOOT_DEBUG("m_bPANEL_SWAP_LVDS_POL = %ld \n",n);
    p_data->m_bPANEL_SWAP_LVDS_POL = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_bPANEL_SWAP_LVDS_CH", 0);
    UBOOT_DEBUG("m_bPANEL_SWAP_LVDS_CH = %ld \n",n);
    p_data->m_bPANEL_SWAP_LVDS_CH = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_bPANEL_CONNECTOR_SWAP_PORT", 0);
    UBOOT_DEBUG("m_bPANEL_CONNECTOR_SWAP_PORT = %ld \n",n);
    p_data->m_bPANEL_CONNECTOR_SWAP_PORT = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_u16LVDS_PN_SWAP_L", 0);
    UBOOT_DEBUG("m_u16LVDS_PN_SWAP_L = %ld \n",n);
    p_data->m_u16LVDS_PN_SWAP_L = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_u16LVDS_PN_SWAP_H", 0);
    UBOOT_DEBUG("m_u16LVDS_PN_SWAP_H = %ld \n",n);
    p_data->m_u16LVDS_PN_SWAP_H = n;

    // parsing [PanelDivisonGPIO_1]
    Profile_GetString("PanelDivisonGPIO_1", "PAD_NAME_1","", str, sizearray(str));
    UBOOT_DEBUG("PanelDivisonGPIO_1 = %s \n",str);
    strncpy(p_data->m_sGPIO1_PAD_NAME, str, sizeof(p_data->m_sGPIO1_PAD_NAME));

    n = Profile_GetInteger("PanelDivisonGPIO_1", "GPIO_INDEX_1", 0);
    UBOOT_DEBUG("GPIO_INDEX = %ld \n",n);
    p_data->m_u16GPIO1_INDEX = n;

    n = Profile_GetInteger("PanelDivisonGPIO_1", "VALUE_1", 0);
    UBOOT_DEBUG("VALUE = %ld \n",n);
    p_data->m_u8GPIO1_VALUE = n;

    // parsing [PanelDivisonGPIO_2]
    Profile_GetString("PanelDivisonGPIO_2", "PAD_NAME_2","", str, sizearray(str));
    UBOOT_DEBUG("PanelDivisonGPIO_2 = %s \n",str);
    strncpy(p_data->m_sGPIO2_PAD_NAME, str, sizeof(p_data->m_sGPIO2_PAD_NAME));

    n = Profile_GetInteger("PanelDivisonGPIO_2", "GPIO_INDEX_2", 0);
    UBOOT_DEBUG("GPIO_INDEX = %ld \n",n);
    p_data->m_u16GPIO2_INDEX = n;

    n = Profile_GetInteger("PanelDivisonGPIO_2", "VALUE_2", 0);
    UBOOT_DEBUG("VALUE = %ld \n",n);
    p_data->m_u8GPIO2_VALUE = n;

    if(script)
        free(script);
    return 0;
}

int Load_BoardSetting_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    st_board_para stBoard;
    UBOOT_TRACE("IN\n");
    memset(&stBoard,0,sizeof(stBoard));
    vfs_mount(CONFIG);
    if(parse_board_ini(gBoardPath, &stBoard)!=0)
    {
        UBOOT_ERROR("parse_board_ini fail !!\n");
        return ret;
    }

    UBOOT_DEBUG("board para size =: 0x%08x \n",sizeof(stBoard));
    UBOOT_DEBUG("st_board_para.m_u8BOARD_PWM_PORT =: 0x%x \n",stBoard.m_u8BOARD_PWM_PORT);
    UBOOT_DEBUG("st_board_para.m_u16BOARD_LVDS_CONNECT_TYPE =: 0x%x \n",stBoard.m_u16BOARD_LVDS_CONNECT_TYPE);
    UBOOT_DEBUG("st_board_para.m_u8BOARD_PWM_PORT =: 0x%x \n",stBoard.m_u8BOARD_PWM_PORT);
    UBOOT_DEBUG("st_board_para.m_bPANEL_PDP_10BIT =: 0x%x \n",stBoard.m_bPANEL_PDP_10BIT);
    UBOOT_DEBUG("st_board_para.m_bPANEL_SWAP_LVDS_POL =: 0x%x \n",stBoard.m_bPANEL_SWAP_LVDS_POL);
    UBOOT_DEBUG("st_board_para.m_bPANEL_SWAP_LVDS_CH =: 0x%x \n",stBoard.m_bPANEL_SWAP_LVDS_CH);
    UBOOT_DEBUG("st_board_para.m_bPANEL_CONNECTOR_SWAP_PORT =: 0x%x \n",stBoard.m_bPANEL_CONNECTOR_SWAP_PORT);
    UBOOT_DEBUG("st_board_para.m_u16LVDS_PN_SWAP_L =: 0x%x \n",stBoard.m_u16LVDS_PN_SWAP_L);
    UBOOT_DEBUG("st_board_para.m_u16LVDS_PN_SWAP_H =: 0x%x \n",stBoard.m_u16LVDS_PN_SWAP_H);
    UBOOT_DEBUG("st_board_para.m_sGPIO1_PAD_NAME =: %s \n",stBoard.m_sGPIO1_PAD_NAME);
    UBOOT_DEBUG("st_board_para.m_u16GPIO1_INDEX =: 0x%x \n",stBoard.m_u16GPIO1_INDEX);
    UBOOT_DEBUG("st_board_para.m_u8GPIO1_VALUE =: 0x%x \n",stBoard.m_u8GPIO1_VALUE);
    UBOOT_DEBUG("st_board_para.m_sGPIO2_PAD_NAME =: %s \n",stBoard.m_sGPIO2_PAD_NAME);
    UBOOT_DEBUG("st_board_para.m_u16GPIO2_INDEX =: 0x%x \n",stBoard.m_u16GPIO2_INDEX);
    UBOOT_DEBUG("st_board_para.m_u8GPIO2_VALUE =: 0x%x \n",stBoard.m_u8GPIO2_VALUE);
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_BOARD_PARA, (U32)&stBoard);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_BoardParaFromflash(st_board_para * board_data)
{
    int ret = -1;
    ret = Read_FlashDataToAddr((U32)board_data, E_DB_BOARD_PARA);
    return ret;
}

#endif


#if (ENABLE_ENABLE_URSA == 1)
#if (ENABLE_URSA_6M30 == 1)
int iniparser_6m30_cmd(ursa_6m30_cmd_table * cmd_table)
{
    int ret = -1;
    char * script = NULL;
    char InPath[BUFFER_SIZE]= {0};
    U32 filesize = 0;
    vfs_mount(CONFIG);
    snprintf(InPath,sizeof(InPath),"%s/6m30_mode.ini",CONFIG_PATH);
    script = loadscript(InPath,&filesize);
    if(script != NULL)
    {
        Profile_Init(script,filesize);
        cmd_table->URSA_6M30_LVDS_CHANNEL_SWAP_MODE = Profile_GetInteger("MISC_6M30_CFG", "6M30_LVDS_CHANNEL_SWAP_MODE", 0);
        cmd_table->URSA_6M30_IN_LVDS_TIMODE = Profile_GetInteger("MISC_6M30_CFG", "6M30_IN_LVDS_TIMODE", 0);
        cmd_table->URSA_6M30_IN_LVDS_TI_BITNUM = Profile_GetInteger("MISC_6M30_CFG", "6M30_IN_LVDS_TI_BITNUM", 0);
        cmd_table->URSA_6M30_OUT_LVDS_TIMODE = Profile_GetInteger("MISC_6M30_CFG", "6M30_OUT_LVDS_TIMODE", 0);
        cmd_table->URSA_6M30_OUT_LVDS_TI_BITNUM = Profile_GetInteger("MISC_6M30_CFG", "6M30_OUT_LVDS_TI_BITNUM", 0);

        cmd_table->URSA_6M30_SSC_SEL = Profile_GetInteger("MISC_6M30_CFG", "6M30_SSC_SEL", 0);
        cmd_table->URSA_6M30_SSC_ENABLE = Profile_GetInteger("MISC_6M30_CFG", "6M30_SSC_ENABLE", 0);
        cmd_table->URSA_6M30_SSC_FREQ = Profile_GetInteger("MISC_6M30_CFG", "6M30_SSC_FREQ", 0);
        cmd_table->URSA_6M30_SSC_PERCENT = Profile_GetInteger("MISC_6M30_CFG", "6M30_SSC_PERCENT", 0);

        UBOOT_DEBUG(" --->>> 6M30_LVDS_CHANNEL_SWAP_MODE is %x \n",cmd_table->URSA_6M30_LVDS_CHANNEL_SWAP_MODE);
        UBOOT_DEBUG(" --->>> 6M30_IN_LVDS_TIMODE is %x \n",cmd_table->URSA_6M30_IN_LVDS_TIMODE);
        UBOOT_DEBUG(" --->>> 6M30_IN_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_6M30_IN_LVDS_TI_BITNUM);
        UBOOT_DEBUG(" --->>> 6M30_OUT_LVDS_TIMODE is %x \n",cmd_table->URSA_6M30_OUT_LVDS_TIMODE);
        UBOOT_DEBUG(" --->>> 6M30_OUT_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_6M30_OUT_LVDS_TI_BITNUM);

        UBOOT_DEBUG(" --->>> 6M30_SSC_SEL is %x \n",cmd_table->URSA_6M30_SSC_SEL);
        UBOOT_DEBUG(" --->>> 6M30_SSC_ENABLE is %x \n",cmd_table->URSA_6M30_SSC_ENABLE);
        UBOOT_DEBUG(" --->>> 6M30_SSC_FREQ is %x \n",cmd_table->URSA_6M30_SSC_FREQ);
        UBOOT_DEBUG(" --->>> 6M30_SSC_PERCENT is %x \n",cmd_table->URSA_6M30_SSC_PERCENT);
        ret = 0;
    }
    else
    {
        UBOOT_ERROR("ubifsload 6m30.ini fail...>>>\n");
    }
    free(script);
    return ret;
}
int Read_Ursa_6m30_Para(ursa_6m30_cmd_table * cmd_table)
{
    int ret = -1;
    ret = Read_FlashDataToAddr((U32)cmd_table, E_DB_URSA_6M30);

    UBOOT_DEBUG(" cmd_table.6M30_LVDS_CHANNEL_SWAP_MODE is %x \n",cmd_table->URSA_6M30_LVDS_CHANNEL_SWAP_MODE);
    UBOOT_DEBUG(" cmd_table.6M30_IN_LVDS_TIMODE is %x \n",cmd_table->URSA_6M30_IN_LVDS_TIMODE);
    UBOOT_DEBUG(" cmd_table.6M30_IN_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_6M30_IN_LVDS_TI_BITNUM);
    UBOOT_DEBUG(" cmd_table.6M30_OUT_LVDS_TIMODE is %x \n",cmd_table->URSA_6M30_OUT_LVDS_TIMODE);
    UBOOT_DEBUG(" cmd_table.6M30_OUT_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_6M30_OUT_LVDS_TI_BITNUM);

    UBOOT_DEBUG(" cmd_table.6M30_SSC_SEL is %x \n",cmd_table->URSA_6M30_SSC_SEL);
    UBOOT_DEBUG(" cmd_table.6M30_SSC_ENABLE is %x \n",cmd_table->URSA_6M30_SSC_ENABLE);
    UBOOT_DEBUG(" cmd_table.6M30_SSC_FREQ is %x \n",cmd_table->URSA_6M30_SSC_FREQ);
    UBOOT_DEBUG(" cmd_table.6M30_SSC_PERCENT is %x \n",cmd_table->URSA_6M30_SSC_PERCENT);
    return ret;
}
int Load_Ursa6m30Para_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    ursa_6m30_cmd_table cmd_table= {0};
    UBOOT_TRACE("IN\n");
    if(iniparser_6m30_cmd(&cmd_table)==0)
    {
        UBOOT_DEBUG("6m30 para size =: 0x%08x \n",sizeof(cmd_table));
        UBOOT_DEBUG("gstDbtable.dbdata[E_DB_URSA_6M30].Offset =: 0x%x\n",gstDbtable.dbdata[E_DB_URSA_6M30].Offset);
        ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_URSA_6M30, (U32)&cmd_table);
    }
    else
    {
        UBOOT_ERROR("parser_6m30_cmd fail ...>>>\n");
    }
    UBOOT_TRACE("OK\n");
    return ret;
}

int Ursa_6M30_Setting(ursa_6m30_cmd_table * cmd_table)
{
    int ret = -1;
    MDrv_Ursa_6M30_ChanelSwap_Mode(cmd_table->URSA_6M30_LVDS_CHANNEL_SWAP_MODE);
    g_UrsaCMDGenSetting.g_InitInLvdsAll.bTIMode = cmd_table->URSA_6M30_IN_LVDS_TIMODE;
    g_UrsaCMDGenSetting.g_InitInLvdsAll.BitNums = cmd_table->URSA_6M30_IN_LVDS_TI_BITNUM;
    g_UrsaCMDGenSetting.g_OutLvdsMode.bTIMode = cmd_table->URSA_6M30_OUT_LVDS_TIMODE;
    g_UrsaCMDGenSetting.g_OutLvdsMode.PanelBitNums = cmd_table->URSA_6M30_OUT_LVDS_TI_BITNUM;

    MDrv_Ursa_6M30_Data_Init();

    g_UrsaCMDGenSetting.g_SscFreqPercent.SscSel = cmd_table->URSA_6M30_SSC_SEL;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscEN = cmd_table->URSA_6M30_SSC_ENABLE;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscFreq = cmd_table->URSA_6M30_SSC_FREQ;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscPercent = cmd_table->URSA_6M30_SSC_PERCENT;

    if(MDrv_Ursa_6M30_3D_Cmd(UC_SET_SSC_FREQ_PERCENT)>0)
    {
        ret = 0;
    }

    return ret;
}

#elif (ENABLE_URSA_8 == 1) || (ENABLE_URSA_6M40 == 1)

int parse_model_ini_ForUrsa(char* path, char* UrsaInIPath)
{
    int ret = -1;//
    char *script = NULL;
    char str[BUFFER_SIZE]= {0};
    U32 filesize = 0;
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            goto exit;
        }
    }
    UBOOT_INFO("sizeof( model ini script ) --------> %u bytes\n",filesize);

    Profile_Init((char *)script,filesize);
    if(Profile_GetString("Ursa", "m_pUrsaName","", str, sizearray(str)))
    {
        UBOOT_INFO("Half Ursa.ini path --> %s\n",str);
        snprintf(UrsaInIPath,BUFFER_SIZE,"%s",str);
        UBOOT_INFO("Full Ursa.ini  path --> %s\n",UrsaInIPath);
        ret = 0;
    }
    else
    {
        goto exit;
    }

exit:
    if(script)
        free(script);
    return ret;
}

int Parser_Ursa_Ini(ursa_cmd_table * cmd_table)
{
    int ret = -1;
    char * script = NULL;
    char InPath[BUFFER_SIZE]= {0};
    char OutPath[BUFFER_SIZE]= {0};
    char UrsaBinName[BUFFER_SIZE]= {0};
    U32 filesize = 0;
    vfs_mount(CONFIG);
    memset(InPath,0,sizeof(InPath));
    snprintf(InPath,sizeof(InPath),"%s/sys.ini",CONFIG_PATH);
    ret = parse_sys_ini(InPath,OutPath);
    if(ret)
    {
        goto Ursa_exit;
    }

    memset(InPath,0,sizeof(InPath));
    memcpy(InPath,OutPath,sizeof(OutPath));
    ret = parse_model_ini_ForUrsa(InPath,OutPath);
    if(ret)
    {
        goto Ursa_exit;
    }
    UBOOT_INFO("Full Ursa.ini path --> %s\n",OutPath);

    memset(InPath,0,sizeof(InPath));
    memcpy(InPath,OutPath,sizeof(OutPath));
    script = loadscript(InPath,&filesize);
    if(script != NULL)
    {
        Profile_Init(script,filesize);
        cmd_table->URSA_EXTERNAL = Profile_GetBoolean("URSA_PARA_CFG", "URSA_EXTERNAL", 1);
        cmd_table->URSA_MEMC_ENABLE = Profile_GetInteger("URSA_PARA_CFG", "URSA_MEMC_ENABLE", 0);
        cmd_table->URSA_LVDS_CHANNEL_SWAP_MODE = Profile_GetInteger("URSA_PARA_CFG", "URSA_LVDS_CHANNEL_SWAP_MODE", 0);
        cmd_table->URSA_LVDS_SWAP_POL = Profile_GetInteger("URSA_PARA_CFG", "URSA_LVDS_SWAP_POL", 0);
        cmd_table->URSA_IN_LVDS_TIMODE = Profile_GetInteger("URSA_PARA_CFG", "URSA_IN_LVDS_TIMODE", 0);
        cmd_table->URSA_IN_LVDS_TI_BITNUM = Profile_GetInteger("URSA_PARA_CFG", "URSA_IN_LVDS_TI_BITNUM", 0);
        cmd_table->URSA_OUT_LVDS_TIMODE = Profile_GetInteger("URSA_PARA_CFG", "URSA_OUT_LVDS_TIMODE", 0);
        cmd_table->URSA_OUT_LVDS_TI_BITNUM = Profile_GetInteger("URSA_PARA_CFG", "URSA_OUT_LVDS_TI_BITNUM", 0);

        cmd_table->URSA_SSC_SEL = Profile_GetInteger("URSA_PARA_CFG", "URSA_SSC_SEL", 0);
        cmd_table->URSA_SSC_ENABLE = Profile_GetInteger("URSA_PARA_CFG", "URSA_SSC_ENABLE", 0);
        cmd_table->URSA_SSC_FREQ = Profile_GetInteger("URSA_PARA_CFG", "URSA_SSC_FREQ", 0);
        cmd_table->URSA_SSC_PERCENT = Profile_GetInteger("URSA_PARA_CFG", "URSA_SSC_PERCENT", 0);
        cmd_table->URSA_OSD_PROTECT_MODE = Profile_GetInteger("URSA_PARA_CFG", "URSA_OSD_PROTECT_MODE", 0);

        UBOOT_DEBUG(" --->>> URSA_EXTERNAL is %x \n",cmd_table->URSA_EXTERNAL);
        UBOOT_DEBUG(" --->>> URSA_MEMC_ENABLE is %x \n",cmd_table->URSA_MEMC_ENABLE);
        UBOOT_DEBUG(" --->>> URSA_LVDS_CHANNEL_SWAP_MODE is %x \n",cmd_table->URSA_LVDS_CHANNEL_SWAP_MODE);
        UBOOT_DEBUG(" --->>> URSA_LVDS_SWAP_POL is %x \n",cmd_table->URSA_LVDS_SWAP_POL);
        UBOOT_DEBUG(" --->>> URSA_IN_LVDS_TIMODE is %x \n",cmd_table->URSA_IN_LVDS_TIMODE);
        UBOOT_DEBUG(" --->>> URSA_IN_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_IN_LVDS_TI_BITNUM);
        UBOOT_DEBUG(" --->>> URSA_OUT_LVDS_TIMODE is %x \n",cmd_table->URSA_OUT_LVDS_TIMODE);
        UBOOT_DEBUG(" --->>> URSA_OUT_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_OUT_LVDS_TI_BITNUM);

        UBOOT_DEBUG(" --->>> URSA_SSC_SEL is %x \n",cmd_table->URSA_SSC_SEL);
        UBOOT_DEBUG(" --->>> URSA_SSC_ENABLE is %x \n",cmd_table->URSA_SSC_ENABLE);
        UBOOT_DEBUG(" --->>> URSA_SSC_FREQ is %x \n",cmd_table->URSA_SSC_FREQ);
        UBOOT_DEBUG(" --->>> URSA_SSC_PERCENT is %x \n",cmd_table->URSA_SSC_PERCENT);
        UBOOT_DEBUG(" --->>> URSA_OSD_PROTECT_MODE is %x \n",cmd_table->URSA_OSD_PROTECT_MODE);

        if(cmd_table->URSA_EXTERNAL == FALSE)
        {
            if(Profile_GetString("URSA_FILE_CFG", "m_pUrsaBinName","", gstSysMiscSetting.m_Ursa_Bin_Name, sizearray(gstSysMiscSetting.m_Ursa_Bin_Name)))
            {
                UBOOT_INFO("Full Ursa Bin Name --> %s\n",UrsaBinName);
                ret = 0;
            }
            else
            {
                goto Ursa_exit;
            }
        }
        ret = 0;
    }
    else
    {
        UBOOT_ERROR("ubifsload 6m30.ini fail...>>>\n");
    }

Ursa_exit:
    if(script)
        free(script);
    return ret;
}
int Read_Ursa_Para(ursa_cmd_table * cmd_table)
{
    int ret = -1;
    ret = Read_FlashDataToAddr((U32)cmd_table, E_DB_URSA);

    UBOOT_DEBUG(" cmd_table.URSA_EXTERNAL is %x \n",cmd_table->URSA_EXTERNAL);
    UBOOT_DEBUG(" cmd_table.URSA_MEMC_ENABLE is %x \n",cmd_table->URSA_MEMC_ENABLE);
    UBOOT_DEBUG(" cmd_table.URSA_LVDS_CHANNEL_SWAP_MODE is %x \n",cmd_table->URSA_LVDS_CHANNEL_SWAP_MODE);
    UBOOT_DEBUG(" cmd_table.URSA_LVDS_SWAP_POL is %x \n",cmd_table->URSA_LVDS_SWAP_POL);
    UBOOT_DEBUG(" cmd_table.URSA_IN_LVDS_TIMODE is %x \n",cmd_table->URSA_IN_LVDS_TIMODE);
    UBOOT_DEBUG(" cmd_table.URSA_IN_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_IN_LVDS_TI_BITNUM);
    UBOOT_DEBUG(" cmd_table.URSA_OUT_LVDS_TIMODE is %x \n",cmd_table->URSA_OUT_LVDS_TIMODE);
    UBOOT_DEBUG(" cmd_table.URSA_OUT_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_OUT_LVDS_TI_BITNUM);

    UBOOT_DEBUG(" cmd_table.URSA_SSC_SEL is %x \n",cmd_table->URSA_SSC_SEL);
    UBOOT_DEBUG(" cmd_table.URSA_SSC_ENABLE is %x \n",cmd_table->URSA_SSC_ENABLE);
    UBOOT_DEBUG(" cmd_table.URSA_SSC_FREQ is %x \n",cmd_table->URSA_SSC_FREQ);
    UBOOT_DEBUG(" cmd_table.URSA_SSC_PERCENT is %x \n",cmd_table->URSA_SSC_PERCENT);

	UBOOT_DEBUG(" cmd_table.URSA_OSD_PROTECT_MODE is %x \n",cmd_table->URSA_OSD_PROTECT_MODE);
    return ret;
}
int Load_UrsaPara_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    ursa_cmd_table cmd_table= {0};
    UBOOT_TRACE("IN\n");
    if(Parser_Ursa_Ini(&cmd_table)==0)
    {
        UBOOT_DEBUG("Ursa para size =: 0x%08x \n",sizeof(cmd_table));
        UBOOT_DEBUG("gstDbtable.dbdata[E_DB_URSA].Offset =: 0x%x\n",gstDbtable.dbdata[E_DB_URSA].Offset);

        ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_URSA, (U32)&cmd_table);
    }
    else
    {
        UBOOT_ERROR("parser_cmd fail ...>>>\n");
    }
    UBOOT_TRACE("OK\n");
    return ret;
}

#if (ENABLE_URSA_8 == 1)
int Ursa_8_Setting(ursa_cmd_table * cmd_table)
{
    int ret = -1;
    MDrv_Ursa_8_ChanelSwap_Mode(cmd_table->URSA_LVDS_CHANNEL_SWAP_MODE);
    //g_UrsaCMDGenSetting.g_InitInLvdsAll.bTIMode = cmd_table->URSA_6M30_IN_LVDS_TIMODE;
    //g_UrsaCMDGenSetting.g_InitInLvdsAll.BitNums = cmd_table->URSA_6M30_IN_LVDS_TI_BITNUM;
    g_UrsaCMDGenSetting.g_OutLvdsMode.bLvdsSwapPol = cmd_table->URSA_LVDS_SWAP_POL;
    g_UrsaCMDGenSetting.g_OutLvdsMode.bTIMode = cmd_table->URSA_OUT_LVDS_TIMODE;
    g_UrsaCMDGenSetting.g_OutLvdsMode.PanelBitNums = cmd_table->URSA_OUT_LVDS_TI_BITNUM;

    g_UrsaCMDGenSetting.g_OsdMode.protect_mode = cmd_table->URSA_OSD_PROTECT_MODE;
    //MDrv_Ursa_6M30_Data_Init();
    udelay(50*1000);
    MDrv_Ursa_8_3D_Cmd ( UC_SET_OUT_LVDS_MODE);

    g_UrsaCMDGenSetting.g_SscFreqPercent.SscSel = cmd_table->URSA_SSC_SEL;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscEN = cmd_table->URSA_SSC_ENABLE;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscFreq = cmd_table->URSA_SSC_FREQ;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscPercent = cmd_table->URSA_SSC_PERCENT;
    udelay(50*1000);
    if(MDrv_Ursa_8_3D_Cmd(UC_SET_SSC_FREQ_PERCENT)>0)
    {
        ret = 0;
    }

    return ret;
}
#elif (ENABLE_URSA_6M40 == 1)
int Ursa_6M40_Syetting(ursa_cmd_table * cmd_table)
{
    int ret = -1;
    g_UrsaCMDGenSetting.g_OutLvdsMode.PanelBitNums = cmd_table->URSA_OUT_LVDS_TI_BITNUM;
    g_UrsaCMDGenSetting.g_OsdMode.protect_mode = cmd_table->URSA_OSD_PROTECT_MODE;
	UBOOT_INFO("g_UrsaCMDGenSetting.g_OsdMode.protect_mode=%d\n",g_UrsaCMDGenSetting.g_OsdMode.protect_mode);

    g_UrsaCMDGenSetting.g_SscFreqPercent.SscSel = cmd_table->URSA_SSC_SEL;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscEN = cmd_table->URSA_SSC_ENABLE;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscFreq = cmd_table->URSA_SSC_FREQ;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscPercent = cmd_table->URSA_SSC_PERCENT;

    return ret;
}
#endif

#endif

#endif

int Load_GopMuxToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    st_gopmux_para GopMux;
    UBOOT_TRACE("IN\n");
    memset(&GopMux,0,sizeof(GopMux));
    vfs_mount(CONFIG);

	snprintf(gGopMuxPath,sizeof(gGopMuxPath),"%s/dfbrc.ini",CONFIG_PATH);
	ret = parse_dfb_ini(gGopMuxPath, &GopMux);
	if(ret)
	{
		 UBOOT_ERROR(" parse_dfb_ini fail...\n");
		//goto exit;
	}

    UBOOT_DEBUG("GopMux para size =: 0x%08x \n",sizeof(GopMux));

    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_PNL_GOPMUX, (U32)&GopMux);
    UBOOT_TRACE("OK\n");
    return ret;


}

int Read_Gop_Mux_ParaFromFlash(st_gopmux_para *gopmux_para)
{
    int ret = -1;
    ret = Read_FlashDataToAddr((U32)gopmux_para, E_DB_PNL_GOPMUX);

    UBOOT_DEBUG("gopmux_para.m_u16SettingCounts=: 0x%x\n",gopmux_para->m_u16SettingCounts);
    UBOOT_DEBUG("gopmux_para.m_u16Setting0GopIndex=: 0x%x\n",gopmux_para->m_u16Setting0GopIndex);
    UBOOT_DEBUG("gopmux_para.m_u16Setting1GopIndex=: 0x%x\n",gopmux_para->m_u16Setting1GopIndex);
    UBOOT_DEBUG("gopmux_para.m_u16Setting2GopIndex=: 0x%x\n",gopmux_para->m_u16Setting2GopIndex);
    UBOOT_DEBUG("gopmux_para.m_u16Setting3GopIndex=: 0x%x\n",gopmux_para->m_u16Setting3GopIndex);
    UBOOT_DEBUG("gopmux_para.m_u16Setting4GopIndex=: 0x%x\n",gopmux_para->m_u16Setting4GopIndex);
    return ret;
}

int Load_TConToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    unsigned int *pu32TConAddr=NULL;
    unsigned int u32fileSize   = 0;
    UBOOT_TRACE("IN\n");

    vfs_mount(CONFIG);
    u32fileSize=vfs_getsize(gTconBinPath);
    UBOOT_DEBUG("u32fileSize=0x%x;gstDbtable.dbdata[E_DB_TCON].Offset=0x%x\n",u32fileSize,gstDbtable.dbdata[E_DB_TCON].Offset);
    pu32TConAddr=malloc(u32fileSize);
    if(pu32TConAddr == NULL)
    {
        UBOOT_ERROR("malloc pu32TConAddr memory error !");
        return -1;
    }
    memset((void*)pu32TConAddr,0,gstDbtable.dbdata[E_DB_TCON].Size);
    if (vfs_read((void *)pu32TConAddr,gTconBinPath,0,u32fileSize) == 0)
    {
        ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_TCON, (U32)pu32TConAddr);
    }
    else
    {
        UBOOT_ERROR("vfs_read data fail ...>>>\n");
    }

    free(pu32TConAddr);
    pu32TConAddr = NULL;
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_TConFromFlash(U32 u32TConAddr)
{
    int ret = 0;
    ret = Read_FlashDataToAddr(u32TConAddr, E_DB_TCON);
    return ret;
}

int Load_PnlVb1ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_PNL_VB1, (U32)&stVB1Channel1order);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_PnlVb1FromFlash(U32 u32PnlVb1Addr)
{
    int ret = 0;
    ret = Read_FlashDataToAddr(u32PnlVb1Addr, E_DB_PNL_VB1);
    return ret;
}

int Load_MiscSetting_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_MISC, (U32)&gstSysMiscSetting);
    UBOOT_DEBUG("gstSysMiscSetting.m_u32MusicVol               = 0x%x\n", (unsigned int)gstSysMiscSetting.m_u32MusicVol);
    UBOOT_DEBUG("gstSysMiscSetting.m_u8LogoGopIdx              = 0x%x\n", gstSysMiscSetting.m_u8LogoGopIdx);
    UBOOT_DEBUG("gstSysMiscSetting.m_u16Panel_SwingLevel       = 0x%x\n", gstSysMiscSetting.m_u16Panel_SwingLevel);
    UBOOT_DEBUG("gstSysMiscSetting.m_u8MirrorMode              = 0x%x\n", gstSysMiscSetting.m_u8MirrorMode);
    UBOOT_DEBUG("gstSysMiscSetting.m_u16Panel_ext_type         = 0x%x\n", gstSysMiscSetting.m_u16Panel_ext_type);
    UBOOT_DEBUG("gstSysMiscSetting.m_Panel_LVDS_Connect_Type   = %s\n"  , gstSysMiscSetting.m_Panel_LVDS_Connect_Type);
    UBOOT_DEBUG("gstSysMiscSetting.m_u16PanelDCLK              = 0x%x\n", gstSysMiscSetting.m_u16PanelDCLK);
    UBOOT_DEBUG("gstSysMiscSetting.m_u32ursa_type              = 0x%x\n", (unsigned int)gstSysMiscSetting.m_u32ursa_type);
    UBOOT_DEBUG("gstSysMiscSetting.m_Ursa_Bin_Name             = %s\n"  , gstSysMiscSetting.m_Ursa_Bin_Name);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_MiscSetting_ToFlash(st_sys_misc_setting * misc_data)
{
    int ret = -1;
    ret = Read_FlashDataToAddr((U32)misc_data, E_DB_MISC);
    UBOOT_DEBUG("misc_data->m_u32MusicVol               =: 0x%x\n", (unsigned int)misc_data->m_u32MusicVol);
    UBOOT_DEBUG("misc_data->m_u8LogoGopIdx              =: 0x%x\n", misc_data->m_u8LogoGopIdx);
    UBOOT_DEBUG("misc_data->m_u16Panel_SwingLevel       =: 0x%x\n", misc_data->m_u16Panel_SwingLevel);
    UBOOT_DEBUG("misc_data->m_u8MirrorMode              =: 0x%x\n", misc_data->m_u8MirrorMode);
    UBOOT_DEBUG("misc_data->m_u16Panel_ext_type         =: 0x%x\n", misc_data->m_u16Panel_ext_type);
    UBOOT_DEBUG("misc_data->m_Panel_LVDS_Connect_Type   =: %s\n"  , misc_data->m_Panel_LVDS_Connect_Type);
    UBOOT_DEBUG("misc_data->m_u16PanelDCLK              =: 0x%x\n", misc_data->m_u16PanelDCLK);
    UBOOT_DEBUG("misc_data->m_u32ursa_type              =: 0x%x\n", (unsigned int)misc_data->m_u32ursa_type);
    UBOOT_DEBUG("misc_data->m_Ursa_Bin_Name             =: %s\n"  , misc_data->m_Ursa_Bin_Name);
    return ret;
}

int Load_PWMSetting_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_PWM, (U32)&gstPWMSetting);
    UBOOT_DEBUG("gstPWMSetting.m_u32PWMPeriod              = 0x%x\n", (unsigned int)gstPWMSetting.m_u32PWMPeriod);
    UBOOT_DEBUG("gstPWMSetting.m_u16DivPWM                 = 0x%x\n", gstPWMSetting.m_u16DivPWM);
    UBOOT_DEBUG("gstPWMSetting.m_u32PWMDuty                = 0x%x\n", (unsigned int)gstPWMSetting.m_u32PWMDuty);
    UBOOT_DEBUG("gstPWMSetting.m_bPolPWM                   = 0x%x\n", gstPWMSetting.m_bPolPWM);
    UBOOT_DEBUG("gstPWMSetting.m_u16maxPWM                 = 0x%x\n", gstPWMSetting.m_u16maxPWM);
    UBOOT_DEBUG("gstPWMSetting.m_u16minPWM                 = 0x%x\n", gstPWMSetting.m_u16minPWM);
    UBOOT_DEBUG("gstPWMSetting.m_u16PWMPort                = 0x%x\n", gstPWMSetting.m_u16PWMPort);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_PWMSetting_ToFlash(st_pwm_setting * pwm_data)
{
    int ret = -1;
    ret = Read_FlashDataToAddr((U32)pwm_data, E_DB_PWM);
    UBOOT_DEBUG("pwm_data->m_u32PWMPeriod              = 0x%x\n", (unsigned int)pwm_data->m_u32PWMPeriod);
    UBOOT_DEBUG("pwm_data->m_u16DivPWM                 = 0x%x\n", pwm_data->m_u16DivPWM);
    UBOOT_DEBUG("pwm_data->m_u32PWMDuty                = 0x%x\n", (unsigned int)pwm_data->m_u32PWMDuty);
    UBOOT_DEBUG("pwm_data->m_bPolPWM                   = 0x%x\n", pwm_data->m_bPolPWM);
    UBOOT_DEBUG("pwm_data->m_u16maxPWM                 = 0x%x\n", pwm_data->m_u16maxPWM);
    UBOOT_DEBUG("pwm_data->m_u16minPWM                 = 0x%x\n", pwm_data->m_u16minPWM);
    UBOOT_DEBUG("pwm_data->m_u16PWMPort                = 0x%x\n", pwm_data->m_u16PWMPort);
    return ret;
}
