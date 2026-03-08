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

int main(int argc, char* argv[])
{
    FILE *t_fpin;
    FILE *t_fpout;
    unsigned int t_filesize = 0;
    unsigned int align_size = 0;
	unsigned int padding_size = 0;
    unsigned char *t_buf = NULL;
	unsigned char i;
    int t_i;

	if (argc < 3)
	{
	    printf("[Error]please enter the correct input!\n");
        return -1;
	}

	//get align size
	align_size=strtoul(argv[2], NULL, 16);
	//printf("align_size : 0x%x\n",align_size);

    if(align_size == 0)
    {
        printf("[Warning] Ignore Alignment Size : 0x%x\n",align_size);
        return 0;
    }

    t_fpin = fopen(argv[1], "rb");
    if (NULL == t_fpin)
    {
        printf("[Error]cannot open %s\n", argv[1]);
        return -1;
    }

	//get file size
	fseek(t_fpin, 0, SEEK_END);
	t_filesize = ftell(t_fpin);
	fseek(t_fpin, 0, SEEK_SET);
	fclose(t_fpin);
	//printf("t_filesize : 0x%x\n",t_filesize);

	//get padding size
	if(t_filesize > align_size)
	{
        if(t_filesize % align_size != 0)
        {
		    padding_size = align_size - (t_filesize % align_size);
        }
        else
        {
            padding_size = 0;
        }
		//printf("padding_size = 0x%x\n",padding_size);
	}
	else if(t_filesize == align_size)
	{
	    //unnecessary padding
		padding_size = 0;
		//printf("padding_size = 0x%x\n",padding_size);
		return 0;
	}
	else
	{
		padding_size = align_size - t_filesize;
    	//printf("padding_size = 0x%x\n",padding_size);
	}

	// put pending data to target file
	t_buf = (unsigned char*)malloc(padding_size);
	if (NULL == t_buf)
	{
		printf("[Error]memory allocation.\n");
		return -1;
	}
	t_fpout = fopen(argv[1], "ab");
	if (NULL == t_fpout)
	{
		printf("[Error]cannot open %s\n", argv[1]);
		return -1;
	}
	memset(t_buf,0,padding_size);
	fwrite(t_buf, sizeof(unsigned char), padding_size, t_fpout);

    free(t_buf);
    fclose(t_fpout);
    return 0;
}
