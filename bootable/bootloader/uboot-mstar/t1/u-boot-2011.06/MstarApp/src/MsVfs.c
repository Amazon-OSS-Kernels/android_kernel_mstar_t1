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

#include <common.h>
#include <watchdog.h>
#include <command.h>
#include <malloc.h>   /* for free() prototype */
#include <stdio.h>
#include <post.h>
#include <ShareType.h>
#include <MsDebug.h>
#include <MsVfs.h>
#include <MsUtility.h>
#include <CusConfig.h>
#include <MsTypes.h>
//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
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
static BOOLEAN bVfsReady=FALSE;
//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
extern int snprintf(char *str, size_t size, const char *fmt, ...);
extern void ubi_get_part_name(char *name);
extern void ubifs_get_sb_name(char *name);

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

#ifdef CONFIG_UBI
int vfs_mount(char *volume)
{
    char buffer[CMD_BUF]="\0";
    char last_ubi_partname[32];
    char last_sb_name[32];
    int ret=-1;
    UBOOT_TRACE("IN\n");

    if(volume==NULL)
    {
        bVfsReady=FALSE;
        UBOOT_ERROR("Input parameter 'volume' is a null pointer\n");
        return -1;
    }


	memset(last_ubi_partname, 0, 32);
	memset(last_sb_name,0,32);
	ubi_get_part_name(last_ubi_partname);
	ubifs_get_sb_name(last_sb_name);

	if( strcmp(last_sb_name,volume) == 0)
    {
		UBOOT_DEBUG("%s is already mount!!\n",volume);
		return 0;
	}

    ret=vfs_umount();
    if(ret==-1)
    {
        bVfsReady=FALSE;
        UBOOT_ERROR("umount previous filesystem fail\n");
        return -1;
    }

    //Assume this volume is in UBI partition
    if(strcmp(last_ubi_partname, UBIPART) != 0)
	{
        memset(buffer,0,CMD_BUF);
        snprintf(buffer,CMD_BUF,"ubi part %s",UBIPART);
        ret=run_command(buffer,0);
    }
    else
        ret = 0;

    if(ret!=-1)
    {
        memset(buffer,0,CMD_BUF);
        snprintf(buffer,CMD_BUF,"ubifsmount %s",volume);
        UBOOT_DEBUG("cmd=%s\n",buffer);
        ret=run_command(buffer,0);
        if(ret==-1)
        {
            UBOOT_ERROR("ubifsmount fail (Assume this volume is in UBI(or ubi) partition)\n");
        }
        else
        {
            bVfsReady=TRUE;
            UBOOT_TRACE("OK\n");
            return ret;
        }
    }
    return ret;
}

int vfs_umount(void)
{
    int ret = 0;
    UBOOT_TRACE("IN\n");
    if(bVfsReady==TRUE)
    {
        ret = run_command("ubifsumount",0);
    }
    if(ret==0)
    {
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_ERROR("unmount fail\n");
    }
    bVfsReady=FALSE;
  	return ret;
}



U32 vfs_getsize(char *filedir)
{
    int ret = -1;
	U32 filesize = 0;
    char *buffer=NULL;
    UBOOT_TRACE("IN\n");
    if(filedir==NULL)
    {
        UBOOT_ERROR("filedir is a null pointer\n");
        return FALSE;
    }
    if(bVfsReady==FALSE)
    {
        UBOOT_ERROR("filesystem is not mounted\n");
        return FALSE;
    }
    buffer=malloc(CMD_BUF);
    if(buffer==NULL)
    {
        UBOOT_ERROR("malloc buffer fail\n");
        return FALSE;
    }
    memset(buffer,0,CMD_BUF);

    snprintf(buffer, CMD_BUF, "ubifsfilesize %s 0x%x",filedir,&filesize);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    ret = run_command(buffer, 0);
    if(ret==-1)
    {
        UBOOT_ERROR("ubifsfilesize %s fail\n",filedir);
        filesize=0;
    }
    else
    {
        UBOOT_TRACE("OK\n");
    }
    free(buffer);
	return filesize;
}
int vfs_read(void* addr,char* filedir,unsigned int offset,unsigned int size )
{
    int ret = -1;
    char *buffer=NULL;
    UBOOT_TRACE("IN\n");

    if(addr==NULL)
    {
        UBOOT_ERROR("addr is a null pointer\n");
        return -1;
    }

    if(filedir==NULL)
    {
        UBOOT_ERROR("filedir is a null pointer\n");
        return -1;
    }

    if(bVfsReady==FALSE)
    {
        UBOOT_ERROR("filesystem is not mounted\n");
        return -1;
    }
    buffer=malloc(CMD_BUF);
    if(buffer==NULL)
    {
        UBOOT_ERROR("malloc buffer fail\n");
        return -1;
    }
    memset(buffer,0,CMD_BUF);
    snprintf(buffer, CMD_BUF, "ubifspartload %x %s %x %x", (unsigned int)addr, filedir, offset,size);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    ret = run_command(buffer, 0);
    if(ret==-1)
    {
        UBOOT_ERROR("ubifspartload %s fail\n",filedir);
    }
    else
    {
        UBOOT_TRACE("OK\n");
    }
    free(buffer);
	return ret;
}

int vfs_write(void* addr,char* filedir,unsigned int offset,unsigned int size )
{
    UBOOT_TRACE("IN\n");

    if(addr==NULL)
    {
        UBOOT_ERROR("addr is a null pointer\n");
        return -1;
    }
    if(filedir==NULL)
    {
        UBOOT_ERROR("filedir is a null pointer\n");
        return -1;
    }

    if(bVfsReady==FALSE)
    {
        UBOOT_ERROR("filesystem is not mounted\n");
        return -1;
    }
    UBOOT_ERROR("Not implement now\n");
    return -1;
}




#else
static char* mmc_volume = "";

#if defined(CONFIG_DUAL_SYSTEM)
#include <MsBoot.h>
static char vfs_mmc_volume[32] = {0};
int vfs_mount(char *volume)
{
    int ret = 0;
    UBOOT_TRACE("IN\n");

    strcpy(vfs_mmc_volume, volume);
    if (0 != read_bootflag())
    {
        strcat(vfs_mmc_volume, DUAL_SYSTEM_PARTITION_SUFFIX);
    }

    mmc_volume = vfs_mmc_volume;
    UBOOT_INFO("mmc change mount : %s\n", mmc_volume);
    bVfsReady=TRUE;
    UBOOT_TRACE("OK\n");
    return ret;
}
#else
int vfs_mount(char *volume)
{
    int ret = 0;
    UBOOT_TRACE("IN\n");
    mmc_volume = volume;
    UBOOT_INFO("mmc change mount : %s\n",mmc_volume);
    bVfsReady=TRUE;
    UBOOT_TRACE("OK\n");
	return ret;
}
#endif

int vfs_umount(void)
{
    int ret = 0;
    UBOOT_TRACE("IN\n");
    mmc_volume = "";
    bVfsReady=FALSE;
    UBOOT_TRACE("OK\n");
	return ret;
}

U32 vfs_getsize(char *filedir)
{
	U32 filesize = 0;
    int ret = 0;
    char *buffer=NULL;

    UBOOT_TRACE("IN\n");

    if(filedir==NULL)
    {
        UBOOT_ERROR("filedir is a null pointer\n");
        return FALSE;
    }

    if(bVfsReady==FALSE)
    {
        UBOOT_ERROR("filesystem is not mounted\n");
        return FALSE;
    }

    buffer=malloc(CMD_BUF);
    if(buffer==NULL)
    {
        UBOOT_ERROR("malloc buffer fail\n");
        return FALSE;
    }
    memset(buffer,0,CMD_BUF);
    //snprintf(cmd, CMD_BUF, "ext4filesize mmc 0:%s %s",mmc_partition,filedir);
    snprintf(buffer, CMD_BUF, "ext4filesize mmc 0:%s %s %x",mmc_volume,filedir,(U32)&filesize);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    ret=run_command(buffer, 0);
    if(ret==-1)
    {
        UBOOT_DEBUG("ext4filesize mmc fail\n");
        filesize=0;
    }
    else
    {
        UBOOT_TRACE("OK\n");
    }
    free(buffer);
	return filesize;
}

int vfs_read(void* addr,char* filedir,unsigned int offset,unsigned int size )
{
    int ret = -1;
    char *buffer=NULL;

    UBOOT_TRACE("IN\n");

    if(addr==NULL)
    {
        UBOOT_ERROR("addr is a null pointer\n");
        return -1;
    }

    if(filedir==NULL)
    {
        UBOOT_ERROR("filedir is a null pointer\n");
        return -1;
    }

    if(bVfsReady==FALSE)
    {
        UBOOT_ERROR("filesystem is not mounted\n");
        return -1;
    }

    buffer=malloc(CMD_BUF);
    if(buffer==NULL)
    {
        UBOOT_ERROR("malloc buffer fail\n");
        return -1;
    }
    memset(buffer,0,CMD_BUF);
    snprintf(buffer, CMD_BUF, "ext4partload mmc 0:%s %x %s %x %x",mmc_volume, (unsigned int)addr, filedir, offset,size);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    ret = run_command(buffer, 0);
    if(ret==-1)
    {
      UBOOT_ERROR("ext4load mmc fail\n");
    }
    else
    {
      UBOOT_TRACE("OK\n");
    }
    free(buffer);
	return ret;
}

int vfs_write(void* addr,char* filedir,unsigned int offset,unsigned int size )
{
    UBOOT_TRACE("IN\n");

    if(addr==NULL)
    {
        UBOOT_ERROR("addr is a null pointer\n");
        return -1;
    }

    if(filedir==NULL)
    {
        UBOOT_ERROR("filedir is a null pointer\n");
        return -1;
    }

    if(bVfsReady==FALSE)
    {
        UBOOT_ERROR("filesystem is not mounted\n");
        return -1;
    }
    UBOOT_ERROR("Not implement now\n");
    return -1;
}

#endif
