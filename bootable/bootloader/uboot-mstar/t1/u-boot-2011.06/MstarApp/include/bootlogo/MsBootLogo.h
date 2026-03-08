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

#ifndef _MS_DisplayLogo_H_
#define _MS_DisplayLogo_H_
#include <ShareType.h>

typedef struct
{
    U16 u16DispX;
    U16 u16DispY;
    U8 bHorStretch;
    U8 bVerStretch;
}LogoDispConfigParams;

#if(CONFIG_COMPRESSED_UBOOT==1)
#define LOGO_OFFSET_FROM_THE_END 0x70000 // SPI:448KB
#else
#define LOGO_OFFSET_FROM_THE_END 0x80000 // SPI:512KB
#endif
int do_dbtable_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_bootlogo (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_audiopreinit(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_bootmusic (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

