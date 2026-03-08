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

#include <command.h>
#include <MsSysUtility.h>

#if !(CONFIG_MINIUBOOT)

U_BOOT_CMD(
    riu, 5, 0, do_riu,
    "riu  - riu command\n",
    "wword [target][value]\n"
    "riu rword [target]\n"
    "riu wbyte [target][value]\n"
    "riu rbyte [target]\n"
    "riu bit   [target][bit][(1/0)]\n"
);

U_BOOT_CMD(gpio, 5, 1, do_gpio,
    "GPIO Command:",
    "(for 2nd parameter, you must type at least 3 characters)\n"
    "gpio output <gpio#> <1/0>         : ex: gpio output 169 1\n"
    "gpio input/get <gpio#>            : ex: gpio input 10  (gpio 10 set as input)\n"
    "gpio toggle <gpio#>               : ex: gpio tog 49 (toggle)\n"
    "gpio state <gpio#>                : ex: gpio sta 49 (get i/o status(direction) & pin status)\n"
    "gpio list [num_of_pins]           : ex: gpio list 10 (list GPIO1~GPIO10 status)\n"
    "gpio check <gpio#> <IN/OUT> <1/0> : ex: gpio check 10 OUT 0 (check i/o status(direction) & pin status)\n"
    );

U_BOOT_CMD(sar, 2, 1, do_sar,
    "sar Command:",
    "(0-base, SAR0~SAR5)\n"
    "sar <ch#> : ex: sar 0       // read sar channel 0\n"
    );

U_BOOT_CMD(cpu, CONFIG_SYS_MAXARGS, 1, do_cpu,
    "cpu Command:",
    "cpu // set cpu clock to 1400MHz\n"
    );
#endif


