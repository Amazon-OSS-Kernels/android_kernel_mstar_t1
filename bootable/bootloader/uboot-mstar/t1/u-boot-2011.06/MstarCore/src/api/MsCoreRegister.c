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

#include <MsCmdTable.h>
#include <config.h>

void Core_Register_MsInit(void)
{
    #if defined (CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH)
    Add_Command_Table ("refreshcis" , 0, STAGE_MSINIT);
    #endif
    //'initDbgLevel must be executed in begining
    #if defined(CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN) && defined (CONFIG_MSTAR_TOOL_PROGRAM)
    Add_Command_Table ("nandprogramforrom" , 0, STAGE_MSINIT);
    #endif
    //jump program nand
    #if defined(CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN) && defined (CONFIG_MSTAR_TOOL_CMDLINE)
    Add_Command_Table ("skipnandprogramforrom" , 0, STAGE_MSINIT);
    #endif
    //check bootflag
    #if defined(CONFIG_DUAL_SYSTEM)
    Add_Command_Table ("sboot_verify_check" , 0, STAGE_MSINIT);
    #endif

    Add_Command_Table ("initDbgLevel" , 0, STAGE_MSINIT);

    #if (CONFIG_APP_IN_SPI)
    Add_Command_Table ("spi ptinit" , 0, STAGE_MSINIT);
    #endif

}

