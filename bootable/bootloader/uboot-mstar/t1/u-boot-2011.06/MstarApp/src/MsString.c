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
/******************************************************************************/
/*                    Header Files                                            */
/* ****************************************************************************/
#include <common.h>
#include <malloc.h>
#include <MsString.h>
#include <ShareType.h>
#include <linux/ctype.h>

char* strlower(char* str)
{
    int i = 0;
    if(str == NULL)
    {
       return NULL;
    }
    while(str[i])
    {
       str[i] = tolower(str[i]);
       i++;
    }
    return str;
}

char* array_reverse(char * array,int ArraySize)
{
   int i=0;
   char tmp;
   if(array == NULL)
       return NULL;
   for(i=0;i<(ArraySize/2);i++)
   {
       tmp=array[ArraySize-1-i];
       array[ArraySize-1-i]=array[i];
       array[i]=tmp;
   }
   return array;
}

char* strrps(char* source,char* findstr, char *repstr)
{
    int i= 0;
    int Replenth = 0;
    char *tmp = NULL;
    char *substr = NULL;

    substr=strstr(source,findstr);
    if(substr == NULL)
        return NULL;
    tmp=(char *)malloc(strlen(substr)+1);
    if(tmp == NULL)
        return NULL;
    strcpy(tmp,substr+strlen(findstr));
    Replenth = strlen(repstr);
    for(i=0 ; i < Replenth; i++)
    {
        substr[i]=repstr[i];
    }
    substr[Replenth]='\0';
    strcat(substr,tmp);
    free(tmp);

    return source;
}

char* ParaReplace(char *str,int argc,char * const argv[])
{
    int index = 0;
    static char tmpstr[CMD_BUF];
    char *strptr = NULL;
    char findstr[3];
    findstr[0]='$';
    findstr[2]='\0';
    memset(tmpstr,0,sizeof(tmpstr));
    strcpy(tmpstr,str);
    strptr = tmpstr;
    while((strptr=strstr(strptr,"$"))!= NULL)
    {
        if( *(strptr+1)>'0' && *(strptr+1) < '9') //check the parameter is between 0 to 9.
        {
            index = *(strptr+1) - '0' ;
            if(index > argc)
            {
                printf("Error: Parameters is bigger than argc!\n");
                return NULL;
            }
            else
            {
                findstr[1]=*(strptr+1);
                strrps(tmpstr,findstr,argv[index]);
            }
        }
        else
        {
            strptr=strptr+1;
        }
    }
    return tmpstr;
}