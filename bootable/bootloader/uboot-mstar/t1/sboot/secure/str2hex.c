/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Convert binary file into HEX bytes for being included in a .c file.
int   str2hex(char*   pInput,   unsigned   char*   pOutput,   int*   len)
{
    char c1, c2;
    int i, length;

    length = strlen(pInput);
    if(length%2)
    {
        return 0;
    }

    for(i=0; i<length; i++)
    {
        pInput[i] = toupper(pInput[i]);
    }

    for(i = 0; i< strlen(pInput)/2; i++)
    {
        c1 = pInput[2*i];
        c2 = pInput[2*i+1];
        if(c1<'0' || (c1 > '9' && c1 <'A') || c1 > 'F')
        {
            continue;
        }
        if(c2<'0' || (c2 > '9' && c2 <'A') || c2 > 'F')
        {
            continue;
        }
        c1 = c1>'9' ? c1-'A'+10 : c1 -'0';
        c2 = c2>'9' ? c2-'A'+10 : c2 -'0';
        pOutput[i] = c1<<4 | c2;
    }
    *len = i;
    return 1;
}

int main(int argc, char* argv[])
{
    FILE* t_fpin;
    FILE *t_fpout;
    int   t_filesize = 0;
    unsigned char* t_buf;
    int  t_i;

    if (3 != argc) {
        printf("Usage: %s <input>\n", argv[0]);
        return 0;
    }

    t_fpin = fopen(argv[1], "rb");
    if (NULL == t_fpin)
    {
        printf("[Error]cannot open %s\n", argv[1]);
        return -1;
    }

    t_fpout = fopen(argv[2], "wb");
    if (NULL == t_fpout)
    {
        printf("[Error]cannot open %s\n", argv[2]);
        return -1;
    }
    // get file size
    fseek(t_fpin, 0, SEEK_END);
    t_filesize = ftell(t_fpin);
    fseek(t_fpin, 0, SEEK_SET);

    t_buf = (unsigned char*)malloc(t_filesize);
    if (NULL == t_buf)
    {
        printf("[Error]memory allocation.\n");
        return -1;
    }

    fread(t_buf, 1, t_filesize, t_fpin);
    unsigned char *hexbuf = (unsigned char *)malloc(t_filesize);
    str2hex(t_buf, hexbuf, &t_filesize);
    fwrite(hexbuf, sizeof(unsigned char),t_filesize,t_fpout);


    free(t_buf);
    free(hexbuf);
    fclose(t_fpin);
    fclose(t_fpout);
    return 0;
}

