/**
* Copyright (c) 2006 â€“ 2016 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   mdrv_ldm_app.c
/// @brief  MStar localdimming io Interface
/// @author MStar Semiconductor Inc.
/// @attention
///////////////////////////////////////////////////////////////////////////////////////////////////


//=============================================================================
// Include Files
//=============================================================================


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>  

//#include <linux/delay.h>
//#include <asm/string.h>

//=============================================================================
// Compile options
//=============================================================================


//=============================================================================
// Local Defines
//=============================================================================

//=============================================================================
// Debug Macros
//=============================================================================

//=============================================================================
// Macros
//=============================================================================
// use 'L' as magic number
#define MDRV_LDM_IOC_MAGIC                  ('L')
#define MDRV_LDM_IOC_INIT                   _IO(MDRV_LDM_IOC_MAGIC, 0)
#define MDRV_LDM_IOC_DEINIT                 _IO(MDRV_LDM_IOC_MAGIC, 1)
#define MDRV_LDM_IOC_ENABLE                 _IOW(MDRV_LDM_IOC_MAGIC, 2, int)
#define MDRV_LDM_IOC_DISABLE                _IOW(MDRV_LDM_IOC_MAGIC, 3, int)
#define MDRV_LDM_IOC_SUSPEND                _IOW(MDRV_LDM_IOC_MAGIC, 4, int)
#define MDRV_LDM_IOC_RESUME                 _IOW(MDRV_LDM_IOC_MAGIC, 5, int)
#define MDRV_LDM_IOC_STRENGTH               _IOW(MDRV_LDM_IOC_MAGIC, 6, int)
#define MDRV_LDM_IOC_GET_DATA               _IOR(MDRV_LDM_IOC_MAGIC, 7, int)
#define MDRV_LDM_IOC_DEBUG                  _IOW(MDRV_LDM_IOC_MAGIC, 8, int)
#define MDRV_LDM_IOC_CUS_PATH               _IOW(MDRV_LDM_IOC_MAGIC, 9, int)
#define MDRV_LDM_IOC_DEMO_PATTERN           _IOW(MDRV_LDM_IOC_MAGIC, 10, int)
#define MDRV_LDM_IOC_DUMP_DRAM              _IOW(MDRV_LDM_IOC_MAGIC, 11, int)
#define MDRV_LDM_IOC_MAX_NR                 (12)


typedef enum
{
    E_DATA_TYPE_LDF=0x01,
    E_DATA_TYPE_LDB=0x02,
    E_DATA_TYPE_SPI=0x03,
    E_DATA_TYPE_MAX   
}EN_GET_DATA_TYPE;

typedef enum
{
    E_DEBUG_LEVEL_ERROR=0x01,
    E_DEBUG_LEVEL_INFO=0x02,
    E_DEBUG_LEVEL_DEBUG=0x04,
    E_DEBUG_LEVEL_ALL=0x07,
    E_DEBUG_LEVEL_MAX   
}EN_DEBUG_LEVEL;

typedef enum
{
    E_DEMO_PATTERN_SWITCH_SINGLE_LED=0x00,
    E_DEMO_PATTERN_LEFT_RIGHT_HALF=0x01,
    E_DEMO_PATTERN_MARQUEE=0x02,
    E_DEMO_PATTERN_LEFT_RIGHT_COLOR_SHELTER=0x03,
    E_DEMO_PATTERN_MAX
}EN_DEMO_PATTERN;

typedef struct
{
    char aCusPath[64];
    char aCusPathU[64];
}ST_DRV_LD_CUS_PATH;

typedef struct
{
    EN_GET_DATA_TYPE eDataType;
    long addr;
}ST_DRV_LD_GET_DATA;


typedef struct
{
    char bOn;   //bool
    EN_DEMO_PATTERN enDemoPattern;
    int u16LEDNum;
}ST_DRV_LD_DEMO_PATTERN;

//=============================================================================
// Local Variables
//=============================================================================

//=============================================================================
// Global Variables
//=============================================================================

//=============================================================================
// Local Function Prototypes
//=============================================================================

//=============================================================================
// Local Function
//=============================================================================

/*
1 init                       load LD moduleÊ±µ÷ÓÃ
2 deinit                   LD module¹Ø±ÕÊ±µ÷ÓÃ, dynamic load moduleÐèÓëinit¶ÔÓ¦call£¬static load¿É²»call
3 enable/disable     ²Ëµ¥¶¯Ì¬¿ª¹Ø
4.strength              ÁÁ¶Èµ÷½Ú[0, 0xff]
5 suspend,             STRÇ°µ÷ÓÃ
6 resume£¬         STR´ý»úÆðÀ´µ÷ÓÃ£¬²»ÓÃÌí¼Ó£¬kernelÄ¬ÈÏµ÷ÓÃ
ÒÔÉÏ£¬local dimming ¹¦ÄÜÖ»Ðèµ÷ÓÃ1 init  Óë3enable¼´¿ÉÆô¶¯ÉúÐ§£¬
 str´ý»úÐèµ÷ÓÃ5suspend£¬´Ë3ÌõiocommandÎª±ØÐë
*/
//-------------------------------------------------------------------------------------------------
/// test local dimming ioctll
/// @return int
/// @attention £ºÖ»Ðèµ÷ÓÃ1 init Óë3enable¼´¿ÉÆô¶¯ str´ý»úÐèµ÷ÓÃ5suspend£¬´Ë3ÌõiocommandÎª±ØÐë
/// <b>[OBAMA] <em></em></b>
//-------------------------------------------------------------------------------------------------
int main(int argc,char **argv)
{
    int fd = 0;
    int i = 0;
    int retval = 0;
    ST_DRV_LD_CUS_PATH stPath;
    ST_DRV_LD_GET_DATA stData;
    ST_DRV_LD_DEMO_PATTERN stPattern;
    long mmapaddr = 0;
    int counter = 0;
    long temp = 0;


    if (argc < 2)
    {
        printf("--------------test simply, argv[0]=%s\n",argv[0]);
        exit(-1);
    }
    /*
    for(i=0;i<argc;i++)
    {
        printf("argc=%d ,argv[%d]=%s\n",argc,i,argv[i]);
    }
    */
    char *commands[]  = {
                        "./testapp", 
                        "debug", 
                        "cuspath", 
                        "init", 
                        "enable", 
                        "getdata", 
                        "disable", 
                        "strength", 
                        "demo", 
                        "dump", 
                        "deinit"};

    fd = open("/dev/localdimming", O_RDONLY);//O_RDWR
    if (fd < 0)
    {
        printf("open /dev/localdimming error fd :0x%x \n", fd );
        perror("open /dev/localdimming error  \n");
        exit(-1);
    }

    //./data/testapp debug 7
    if (!strcmp(commands[1], argv[1]))
    {
        temp = atol(argv[2]);
        printf("+++++++++++The command is %s, parameter %u !\n", argv[1],temp);
        retval = ioctl(fd, MDRV_LDM_IOC_DEBUG, &temp);  //before init
        if (0 != retval)
        {
            printf("ioctl MDRV_LDM_IOC_DEBUG error retval :%d \n", retval);
            perror("ioctl MDRV_LDM_IOC_DEBUG \n");
            exit(-1);
        }
    }

    //./data/testapp cuspath /config/ldm/ /mnt/usb/38B9-3308/ldm/
    if (!strcmp(commands[2], argv[1]))
    {
        printf("+++++++++++The command is %s, parameter1 %s, parameter2 %s !\n", argv[1],argv[2],argv[3]);
        strcpy(stPath.aCusPath, argv[2]);
        strcpy(stPath.aCusPathU, argv[3]);
        retval = ioctl(fd, MDRV_LDM_IOC_CUS_PATH, &stPath);  //before init
        if (0 != retval)
        {
            printf("ioctl MDRV_LDM_IOC_CUS_PATH error retval :%d \n", retval);
            perror("ioctl MDRV_LDM_IOC_CUS_PATH \n");
            exit(-1);
        }
    }

    //./data/testapp init 0x3e580000
    if (!strcmp(commands[3], argv[1]))
    {
        mmapaddr = strtol(argv[2],NULL,16);
        printf("+++++++++++The command is %s, parameter 0x%x !\n", argv[1],mmapaddr);
        //mmapaddr = temp;
        retval = ioctl(fd, MDRV_LDM_IOC_INIT,&mmapaddr);
        if (0 != retval)
        {
            printf("ioctl MDRV_LDM_IOC_INIT error retval :%d \n", retval);
            perror("ioctl MDRV_LDM_IOC_INIT \n");
            exit(-1);
        }
    }
    
    //./data/testapp enable
    if (!strcmp(commands[4], argv[1]))
    {
        printf("+++++++++++The command is %s !\n", argv[1]);
        retval = ioctl(fd, MDRV_LDM_IOC_ENABLE);
        if (0 != retval)
        {
            printf("ioctl MDRV_LDM_IOC_ENABLE error retval :%d \n", retval);
            perror("ioctl MDRV_LDM_IOC_ENABLE \n");
            exit(-1);
        }
    }

    //./data/testapp getdata 1
    if (!strcmp(commands[5], argv[1]))
    {
        temp = strtol(argv[2],NULL,10);
        printf("+++++++++++The command is %s, parameter 0x%x !\n", argv[1],temp);
        stData.eDataType = temp;//E_DATA_TYPE_LDF;
        stData.addr = 0;
        retval = ioctl(fd, MDRV_LDM_IOC_GET_DATA, &stData);
        if (0 != retval)
        {
            printf("ioctl MDRV_LDM_IOC_GET_DATA error retval :%d \n", retval);
            perror("ioctl MDRV_LDM_IOC_GET_DATA \n");
            exit(-1);
        }
        //mm=pa2va(stData.addr)
        //printf(mm)
    }
    
     //./data/testapp disable 250
    if (!strcmp(commands[6], argv[1]))
    {
        temp = strtol(argv[2],NULL,10);
        printf("+++++++++++The command is %s, parameter 0x%x !\n", argv[1],temp);
        retval = ioctl(fd, MDRV_LDM_IOC_DISABLE,&temp);
         if (0 != retval)
         {
             printf("ioctl MDRV_LDM_IOC_DISABLE error retval :%d \n", retval);
             perror("ioctl MDRV_LDM_IOC_DISABLE \n");
             exit(-1);
         }
    }

     //./data/testapp strength 20
    if (!strcmp(commands[7], argv[1]))
    {
        temp = strtol(argv[2],NULL,10);
        printf("+++++++++++The command is %s, parameter 0x%x !\n", argv[1],temp);
        retval = ioctl(fd, MDRV_LDM_IOC_STRENGTH,&temp);
         if (0 != retval)
         {
             printf("ioctl MDRV_LDM_IOC_STRENGTH error retval :%d \n", retval);
             perror("ioctl MDRV_LDM_IOC_STRENGTH \n");
             exit(-1);
         }
    }

    //turn on demo: ./data/testapp demo 0 1 33, turn off: ./data/testapp demo 0 0 33
    if (!strcmp(commands[8], argv[1]))
    {
        temp = strtol(argv[2],NULL,10);
        i = strtol(argv[3],NULL,10);
        counter = strtol(argv[4],NULL,10);
        //printf("+++++++++++The command is %s, parameter 0x%x !\n", argv[1],temp, counter);
        stPattern.enDemoPattern = temp;//E_DEMO_PATTERN_SWITCH_SINGLE_LED;//
        stPattern.bOn = i;//1;//true;
        if(0 == temp)
        {        
            temp = strtol(argv[5],NULL,10);
            i = strtol(argv[6],NULL,10);
            for(;counter<=temp;counter++)
            {
                stPattern.u16LEDNum = counter;
                retval = ioctl(fd, MDRV_LDM_IOC_DEMO_PATTERN, &stPattern);
                if (0 != retval)
                {
                    printf("ioctl MDRV_LDM_IOC_DEMO_PATTERN E_DEMO_PATTERN_SWITCH_SINGLE_LED error retval :%d \n", retval);
                    perror("ioctl MDRV_LDM_IOC_DEMO_PATTERN E_DEMO_PATTERN_SWITCH_SINGLE_LED \n");
                    exit(-1);
                }

                //delay
                //sleep(1);
                usleep(i);
            }
        }
        else
        {
            stPattern.u16LEDNum = counter;
            retval = ioctl(fd, MDRV_LDM_IOC_DEMO_PATTERN, &stPattern);
            if (0 != retval)
            {
                printf("ioctl MDRV_LDM_IOC_DEMO_PATTERN E_DEMO_PATTERN_SWITCH_SINGLE_LED error retval :%d \n", retval);
                perror("ioctl MDRV_LDM_IOC_DEMO_PATTERN E_DEMO_PATTERN_SWITCH_SINGLE_LED \n");
                exit(-1);
            }
        }
    }

    //./data/testapp dump 1
    if (!strcmp(commands[9], argv[1]))
    {
        EN_GET_DATA_TYPE eDataType = 0;
        
        temp = strtol(argv[2],NULL,10);
        printf("+++++++++++The command is %s, parameter 0x%x !\n", argv[1],temp);
        eDataType = temp;//E_DATA_TYPE_LDF;
        retval = ioctl(fd, MDRV_LDM_IOC_DUMP_DRAM, &eDataType);
        if (0 != retval)
        {
            printf("ioctl MDRV_LDM_IOC_DUMP_DRAM error retval :%d \n", retval);
            perror("ioctl MDRV_LDM_IOC_DUMP_DRAM \n");
            exit(-1);
        }
        //mm=pa2va(stData.addr)
        //printf(mm)
    }

    //./data/testapp deinit
    if (!strcmp(commands[10], argv[1]))
    {
        printf("+++++++++++The command is %s !\n", argv[1]);
        retval = ioctl(fd, MDRV_LDM_IOC_DEINIT);
         if (0 != retval)
         {
             printf("ioctl MDRV_LDM_IOC_DEINIT error retval :%d \n", retval);
             perror("ioctl MDRV_LDM_IOC_DEINIT \n");
             exit(-1);
         }
    }
    
   // return 0;

 /*       
    retval = ioctl(fd, MDRV_LDM_IOC_SUSPEND);
    if (0 != retval)
    {
        perror("ioctl MDRV_LDM_IOC_SUSPEND error\n");
        exit(-1);
    }
    printf("ioctl MDRV_LDM_IOC_SUSPEND successfully\n");
*/
    close(fd);

    return 0;
}
