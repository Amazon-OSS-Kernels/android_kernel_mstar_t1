/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//
//
//
//
//
//
//
//
//
//******************************************************************************
///////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   debug_util.c
/// @brief  debug register module for all other IPs
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

// Common Definition
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
#include "MsTypes.h"

// Internal Definition

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define DBG_MSG(x)      //x

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
struct DBG_LIST_ENTRY
{
    const char *Func_Name;
    const char *Func_Help;
    MS_DBG_LINK *pAryDbgLink;
    struct DBG_LIST_ENTRY *next;
};

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static struct DBG_LIST_ENTRY *pDbgRoot = NULL;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _MsOS_Dbg_PrintMenuBanner(const char *banner)
{
    printf("============\n");
    printf("  %s\n", banner);
    printf("============\n");
}

static void _MsOS_Dbg_PrintMenu(void)
{
    struct DBG_LIST_ENTRY *pCurrentEntry = pDbgRoot;

    _MsOS_Dbg_PrintMenuBanner("Main");

    while(pCurrentEntry != NULL)
    {
        printf("%s\t:%s\n", pCurrentEntry->Func_Name, pCurrentEntry->Func_Help);
        pCurrentEntry = pCurrentEntry->next;
    }
}

static void _MsOS_Dbg_PrintSubMenu(const char *Func_Name, MS_DBG_LINK *pAryDbgLink)
{
    // no more parameters, print sub menu banner
    _MsOS_Dbg_PrintMenuBanner(Func_Name);

    if(pAryDbgLink != NULL)
    {
        while(pAryDbgLink->pCallBack != NULL)
        {
            printf("%s\t:%s\n", pAryDbgLink->Func_Name, pAryDbgLink->Func_Help);
            pAryDbgLink++;
        }
    }
}

static MS_BOOL _MsOS_Dbg_IsRegisterd(const char *Func_Name)
{
    struct DBG_LIST_ENTRY *pCurrentEntry = pDbgRoot;

    if(pCurrentEntry == NULL)
    {
        return FALSE;
    }
    else
    {
        while(pCurrentEntry->next != NULL)
        {
            if(strcmp(pCurrentEntry->Func_Name, Func_Name) == 0)
            {
                MS_CRITICAL_MSG(printf("[%s] %s registered\n", __func__, Func_Name));
                return TRUE;
            }
            else
            {
                pCurrentEntry = pCurrentEntry->next;
            }
        }
        return FALSE;
    }
}

static MS_BOOL _MsOS_Dbg_ExecuteCB(int argc, char *argv[])
{
    // for example, input is "PNL Init"
    struct DBG_LIST_ENTRY *pCurrentEntry = pDbgRoot;
    int i;

    for(i=0; i<argc; i++)
    {
        DBG_MSG(printf("argv[%d]=%s\n",i,argv[i]));
    }

    while(pCurrentEntry != NULL)
    {
        DBG_MSG(printf("ExecuteCB(%s, %s)\n", argv[0], pCurrentEntry->Func_Name));

        if(strcmp(argv[0], pCurrentEntry->Func_Name) == 0)
        {
            // match, execute call back
            return MsOS_Dbg_ExecuteSubCB(pCurrentEntry->Func_Name, argc-1, argv+1, pCurrentEntry->pAryDbgLink);
        }
        else
        {
            pCurrentEntry = pCurrentEntry->next;
        }
    }

    // not found associate command
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL MsOS_Dbg_ExecuteSubCB(const char *Func_Name, int argc, char *argv[], MS_DBG_LINK *pAryDbgLink)
{
    if(pAryDbgLink != NULL)
    {
        if(argc == 0)
        {
            _MsOS_Dbg_PrintSubMenu(Func_Name, pAryDbgLink);
            return TRUE;
        }
        else
        {
            while(pAryDbgLink->pCallBack != NULL)
            {
                if(strcmp(argv[0], pAryDbgLink->Func_Name) == 0)
                {
                    // found call back, execute it
                    return pAryDbgLink->pCallBack(argc-1, argv+1);
                }
                else
                {
                    pAryDbgLink++;
                }
            }

            return FALSE;
        }
    }

    return FALSE;
}

MS_BOOL MsOS_Dbg_Regist(const char *Func_Name, const char *Func_Help, MS_DBG_LINK *pAryDbgLink)
{
    struct DBG_LIST_ENTRY *pNewEntry;
    struct DBG_LIST_ENTRY *pCurrentEntry;

    // check if registerd before
    if(_MsOS_Dbg_IsRegisterd(Func_Name))
    {
        return FALSE;
    }
    else
    {
        pNewEntry = (struct DBG_LIST_ENTRY *) malloc(sizeof(struct DBG_LIST_ENTRY));
        if(pNewEntry == NULL)
        {
            MS_CRITICAL_MSG(printf("[%s] new entry failed\n", __func__));
            return FALSE;
        }
        else
        {
            pNewEntry->Func_Name = Func_Name;
            pNewEntry->Func_Help = Func_Help;
            pNewEntry->pAryDbgLink = pAryDbgLink;
            pNewEntry->next = NULL;
        }

        if(pDbgRoot == NULL)
        {
            // 1st time register
            pDbgRoot = pNewEntry;
        }
        else
        {
            // add to the end of the list
            pCurrentEntry = pDbgRoot;
            while(pCurrentEntry->next != NULL)
            {
                pCurrentEntry = pCurrentEntry->next;
            }
            pCurrentEntry->next = pNewEntry;
        }

        return TRUE;
    }
}

MS_BOOL MsOS_Dbg_ParseCmd(char *Cmd, MS_U32 u32CmdLen)
{
    char * token;
    const char delimiters[] = " \r";
    int argc = 0;
    char *argv[20];

    DBG_MSG(printf("In %s:%d\n",__func__,__LINE__));

    if((u32CmdLen == 1) && (*Cmd == 0x0D))
    {
        // enter only, dump main menu
        _MsOS_Dbg_PrintMenu();
        return TRUE;
    }
    else
    {
        // separate cmd to two part, token and remain cmd
        token = strtok(Cmd, delimiters);
        while ( token != NULL)
        {
            argv[argc] = token;
            DBG_MSG(printf("argv[%d]=%s\n", argc, argv[argc]));
            argc++;
            token = strtok(NULL, delimiters);
        }

        // check parameters
        if((argc == 0) || (argv[0] == NULL))
        {
            // error
            return FALSE;
        }
        else
        {
            // find registered func and execute it
            return _MsOS_Dbg_ExecuteCB(argc, argv);
        }
    }
}

void MsOS_RegMyDbg(void)
{
    return;
}

