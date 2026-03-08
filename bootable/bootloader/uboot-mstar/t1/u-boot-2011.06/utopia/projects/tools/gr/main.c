/**
* Copyright (c) 2006 – 2018 MStar Semiconductor, Inc.
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
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    main.c
/// @brief  main entry point of the application program
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
#include <fcntl.h>
#include "mdrv_types.h"

int SPILoad(void)
{
	FILE *fp;
	int size;
	unsigned int start;
	unsigned char *pp=0;

	char fn[256]; 
	printf("--- Start to load file from SPI or DRAM ---\n");
	printf("SPI base: 0xBFC00000, DRAM base: 0x80000000\n");
	printf("file name: ");	
	scanf("%s",&fn);
	printf("start address: 0x ");	
	scanf("%x",&start);
	printf("size (bytes): ");
	scanf("%d",&size);
       
	//pp = malloc( DEFAULT_SIZE ) ;
	pp = malloc( size+4 ) ;
	if( 0==pp ){
		printf( "alloc fail\n" ) ;
		return 0 ;
	}
 	
	//MAdp_getSPI(pp);
	MAdp_SPI_Load(start,size,pp);
	printf( "\nfirst: 0x%02X-0x%02X-0x%02X-0x%02X\n", pp[0],pp[1],pp[2],pp[3] );
	fp = fopen( fn, "w+" ) ;
	fwrite( pp, 1, size, fp ) ;
	fclose(fp) ;

	free(pp) ;

}


extern void MAdp_SYS_Init(void);
int main(int argc, char* argv[])
{
    //----------------------------------------------------------------------------------------------
    //  System Initialization
    //----------------------------------------------------------------------------------------------
    MAdp_SYS_Init(); //call adaptation function 

    SPILoad();   //gr 
    return 0;
}



