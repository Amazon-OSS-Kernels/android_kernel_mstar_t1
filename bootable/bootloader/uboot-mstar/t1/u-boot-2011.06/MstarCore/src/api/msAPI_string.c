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

#include <common.h>
#include <command.h>
#include <config.h>
#include <MsTypes.h>
#include <MsOS.h>
#include "language.h"
#include "LoaderCharset.h"
#include "LoaderCharset_Eng.h"


#define STRBUFF_LENGTH       256
#define OSD_LANGUAGE         "osd_language"
#define LANGSTR(ID,OFFSET)   pLangArray[(ID)*6+(OFFSET)]
#define LANGSTRLEN(ID)       (LANGSTR(ID,0)<<8)|(LANGSTR(ID,1))
#define LANGSTRADDR(ID)      (LANGSTR(ID,2)<<24)|(LANGSTR(ID,3)<<16)|(LANGSTR(ID,4)<<8)|LANGSTR(ID,5)
#define ENGLISH              0
#define CHINESE              1
#define RUSSIAN              2

typedef struct{
  char *language;
  MS_U8 languagenum;
}languageID;

languageID languageid[]={
    {"English",ENGLISH},
    {"Chinese",CHINESE},
    {"Russian",RUSSIAN},
};
static MS_U8 CHAR_BUFFER[STRBUFF_LENGTH];
const MS_U8* pLoaderCharacterset = NULL;
const MS_U8* pLoaderFont = NULL;
const MS_U8* pLoaderWidthData=NULL;
extern int strcmp(const char * cs,const char * ct);

MS_U8 get_languageID(void)
{
    int  i,size;
    MS_U8* language=NULL;
    MS_BOOL env=FALSE;
    language=(MS_U8*)getenv(OSD_LANGUAGE);
    if(language == NULL)
    {
        printf("Failed to get the language from the environment,please check");
        return 0;
    }

    size=sizeof(languageid)/sizeof(languageID);
    for(i=0;i<size;i++)
    {
        if(strcmp((const char *)language,(const char *)languageid[i].language) == 0)
        {
            env= TRUE;
            break;
        }
        else
        {
            env=FALSE;
        }
    }
    if(env == TRUE)
    {
        return languageid[i].languagenum;
    }
    else
    {
        return languageid[0].languagenum;
    }
}

MS_U8* OSD_GetString(MS_U32 id)
{
    const MS_U8* pLangArray=NULL;
    MS_U8 LangID;
    MS_U16 strlength;
    MS_U32 straddr;
    MS_U16 i=0;
    memset(CHAR_BUFFER,0,STRBUFF_LENGTH);
    id=id&0xffff;
    LangID=get_languageID();
    switch(LangID)
    {
        case 0:
            pLangArray=language0;
            break;
        case 1:
            pLangArray=language1;
            pLoaderCharacterset=LoaderCharacterset1;
            pLoaderFont=LoaderFont1;
            pLoaderWidthData=LoaderWidthData1;
            break;
        case 2:
            pLangArray=language2;
            pLoaderCharacterset=LoaderCharacterset2;
            pLoaderFont=LoaderFont2;
            pLoaderWidthData=LoaderWidthData2;
            break;
        case 3:
            pLangArray=language3;
            pLoaderCharacterset=LoaderCharacterset3;
            pLoaderFont=LoaderFont3;
            pLoaderWidthData=LoaderWidthData3;
            break;
        case 4:
            pLangArray=language4;
            pLoaderCharacterset=LoaderCharacterset4;
            pLoaderFont=LoaderFont4;
            pLoaderWidthData=LoaderWidthData4;
            break;
        case 5:
            pLangArray=language5;
            pLoaderCharacterset=LoaderCharacterset5;
            pLoaderFont=LoaderFont5;
            pLoaderWidthData=LoaderWidthData5;
            break;
        case 6:
            pLangArray=language6;
            pLoaderCharacterset=LoaderCharacterset6;
            pLoaderFont=LoaderFont6;
            pLoaderWidthData=LoaderWidthData6;
            break;
        case 7:
            pLangArray=language7;
            pLoaderCharacterset=LoaderCharacterset7;
            pLoaderFont=LoaderFont7;
            pLoaderWidthData=LoaderWidthData7;
            break;
        default:
            printf("no more language");
            break;
    }
    strlength=LANGSTRLEN(id);
    straddr=LANGSTRADDR(id);
    for(i=0;i<strlength;i++)
    {
        CHAR_BUFFER[i]=pLangArray[straddr+i];
    }
    CHAR_BUFFER[i]=' '; //space key

    return CHAR_BUFFER;

}

