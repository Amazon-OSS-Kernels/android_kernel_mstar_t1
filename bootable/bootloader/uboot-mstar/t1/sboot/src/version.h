/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
/////////////////////////////////////////////////////////////////

#ifndef  _API_SBOOT_H_
#define _API_SBOOT_H_
//-------------------------------------------------------------------------------------------------
// Version Control
//-------------------------------------------------------------------------------------------------
#define MSIF_TAG                    {'M','S','I','F'}                   // MSVC
#define MSIF_CLASS                  {'0','0'}                           // DRV/API (DDI)
#define MSIF_CUS                    {'0','0','S','3'}
#define MSIF_QUALITY                0
#define MSIF_MOD                    {'M','B','O','T'}
#define MSIF_VER                   {'0','2','0','6'}
#define MSIF_SBT_CHANGELIST         {'0','0','0','0','0','0','0','0'}    //P4 ChangeList Number
#define MSIF_MAJORVER               {'0','2'}
#define MSIF_MINORVER               {'0','6'}
#define MSIF_OS                     '2'

#define SBT_VER                  /* Character String for SBOOT version               */  \
    MSIF_TAG,                       /* 'MSIF'                                           */  \
    MSIF_CLASS,                     /* '00'                                             */  \
    MSIF_CUS,                       /* '00S0'                                           */  \
    MSIF_QUALITY,                   /* 0                                                */  \
    MSIF_MOD,                       /* 'SBT_'                                           */  \
    MSIF_VER,                      /* '0000'                                         */  \
    MSIF_SBT_CHANGELIST,            /* CL#                                              */  \
    {'0','0','0'}

#define SBT_VER_MSIF                /* Character String for SBOOT version               */  \
    MSIF_TAG,                       /* 'MSIF'                                           */  \
    MSIF_CLASS,                     /* '00'                                             */  \
    {0,0},                          /* '0x0000'                                         */  \
    {0,0},                          /* '0x0000'                                         */  \
    {0,0},                          /* '0x0000'                                         */  \
    '0',                                                                                    \
    MSIF_MOD,                       /* 'MBOT'                                           */  \
    MSIF_MAJORVER,                  /* '02'                                             */  \
    MSIF_MINORVER,                  /* '06'                                             */  \
    MSIF_SBT_CHANGELIST,            /* CL#                                              */  \
    MSIF_OS

typedef union _MSIF_Version
{
    struct _DDI
    {
        U8                       tag[4];
        U8                       type[2];
        U16                      customer;
        U16                      model;
        U16                      chip;
        U8                       cpu;
        U8                       name[4];
        U8                       version[2];
        U8                       build[2];
        U8                       change[8];
        U8                       os;
    } DDI;
    struct _MW
    {
        U8                                     tag[4];
        U8                                     type[2];
        U16                                    customer;
        U16                                    mod;
        U16                                    chip;
        U8                                     cpu;
        U8                                     name[4];
        U8                                     version[2];
        U8                                     build[2];
        U8                                     changelist[8];
        U8                                     os;
    } MW;
    struct _APP
    {
        U8                                     tag[4];
        U8                                     type[2];
        U8                                     id[4];
        U8                                     quality;
        U8                                     version[4];
        U8                                     ver[4];
        U8                                     changelist[8];
        U8                                     reserve[3];
    } APP;
    struct _BOOT
    {
        U8                                     tag[4];
        U8                                     type[2];
        U8                                     customer[2];
        U8                                     mod[2];
        U8                                     chip[2];
        U8                                     cpu;
        U8                                     name[4];
        U8                                     version[2];
        U8                                     build[2];
        U8                                     changelist[8];
        U8                                     os;
    } BOOT;
} MSIF_Version;

const MSIF_Version _sbt_version = {
    .APP = { SBT_VER }
};

MSIF_Version _sbt_version_msif = {
    .BOOT = { SBT_VER_MSIF }
};

#endif //_API_SBOOT_H_
