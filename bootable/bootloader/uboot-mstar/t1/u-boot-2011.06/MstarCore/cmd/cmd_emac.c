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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   cmd_emac.c
/// @brief  u-boot EMAC Driver
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------#include "e1000.h"

/*
 * EMAC Read/Write Utilities
 */
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <net.h>

//#include <mdrv_emac.h>
/*-----------------------------------------------------------------------
 * Definitions
 */
#define EMAC_INPUT_LEN                  2
#define EMAC_ADDR_INPUT_LEN                  7

#define MAX_EMAC_BYTES 0x1000	/* Maximum number of bytes we can handle */

extern int MDrv_EMAC_initialize(bd_t * bis);
void MDrv_EMAC_PowerMode(u8 mode);
extern void MDrv_EMAC_WaveTest(u8 mode);

extern u8 MY_MAC[6];
/*
 * emac init
 *
 * Syntax:
 *   estart
 */
#define MACADDR_ENV    "macaddr"
#define MACADDR_FORMAT "XX:XX:XX:XX:XX:XX"

int do_emac (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    bd_t * bis=NULL;
    char * macaddr;

    if (   NULL != (macaddr = getenv(MACADDR_ENV))
        && strlen(macaddr) == strlen(MACADDR_FORMAT)
        && ':' == macaddr[2]
        && ':' == macaddr[5]
        && ':' == macaddr[8]
        && ':' == macaddr[11]
        && ':' == macaddr[14]
        )
    {
        macaddr[2]  = '\0';
        macaddr[5]  = '\0';
        macaddr[8]  = '\0';
        macaddr[11] = '\0';
        macaddr[14] = '\0';

        MY_MAC[0]   = (u8)simple_strtoul(&(macaddr[0]),  NULL, 16);
        MY_MAC[1]   = (u8)simple_strtoul(&(macaddr[3]),  NULL, 16);
        MY_MAC[2]   = (u8)simple_strtoul(&(macaddr[6]),  NULL, 16);
        MY_MAC[3]   = (u8)simple_strtoul(&(macaddr[9]),  NULL, 16);
        MY_MAC[4]   = (u8)simple_strtoul(&(macaddr[12]), NULL, 16);
        MY_MAC[5]   = (u8)simple_strtoul(&(macaddr[15]), NULL, 16);

        /* set back to ':' or the environment variable would be destoried */ // REVIEW: this coding style is dangerous
        macaddr[2]  = ':';
        macaddr[5]  = ':';
        macaddr[8]  = ':';
        macaddr[11] = ':';
        macaddr[14] = ':';
    }

    MDrv_EMAC_initialize(bis);
	printf("(Re)start EMAC...\n");

	return 0;
}

int do_ewavetest (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    u8 mode = simple_strtoul(argv[1], NULL, 16);

    printf("start EMAC ewavetest mode:%d...\n", mode);
    MDrv_EMAC_WaveTest(mode);
    printf("finish EMAC ewavetest mode:%d...\n", mode);

return 0;
}

int do_setMac(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *cmdline_buf = NULL;
    if ((argc < EMAC_ADDR_INPUT_LEN) || (argc > EMAC_ADDR_INPUT_LEN))
    {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    MY_MAC[0]    = simple_strtoul(argv[1], NULL, 16);
    MY_MAC[1]    = simple_strtoul(argv[2], NULL, 16);
    MY_MAC[2]    = simple_strtoul(argv[3], NULL, 16);
    MY_MAC[3]    = simple_strtoul(argv[4], NULL, 16);
    MY_MAC[4]    = simple_strtoul(argv[5], NULL, 16);
    MY_MAC[5]    = simple_strtoul(argv[6], NULL, 16);

    cmdline_buf = malloc(64);
    if(cmdline_buf == NULL)
    {
        printf("%s: malloc() failed, at %d\n", __func__, __LINE__); //cyber
        return -1;
    }
    sprintf(cmdline_buf, "setenv "MACADDR_ENV" %02X:%02X:%02X:%02X:%02X:%02X", MY_MAC[0], MY_MAC[1], MY_MAC[2], MY_MAC[3], MY_MAC[4], MY_MAC[5]);

    run_command(cmdline_buf, 0);
    run_command("saveenv", 0);
    free(cmdline_buf);

    printf("New MAC Address is %02X:%02X:%02X:%02X:%02X:%02X\n", MY_MAC[0], MY_MAC[1], MY_MAC[2], MY_MAC[3], MY_MAC[4], MY_MAC[5]);

    // Set MAC address ------------------------------------------------------
    //MHal_EMAC_Write_SA1_MAC_Address(sa1[0],sa1[1],sa1[2],sa1[3],sa1[4],sa1[5]);

    return 0;
}

extern int emac_init;
extern void MDrv_EMAC_DumpReg(void);
int do_dumpMac(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    if (!emac_init)
    {
        printf("Please start emac first!\n");
        return 0;
    }
   	printf("Dump EMAC Register\n");
    MDrv_EMAC_DumpReg();
    return 0;
}

extern u8 MDrv_EMAC_Bist(u8 bEnable);
extern int NetLoop(proto_t protocol);
int do_bistMac(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    u8 count = simple_strtoul(argv[1], NULL, 16);

    if (!emac_init)
    {
        printf("Please start emac first!\n");
        return 0;
    }

    if (0 == count)
        printf("Usage:\n ebist count (count:0~255 times)\n Ex: ebist 10\n");
    else
    {
        u8 idx= 0;
        char buffer[256], ip_addr[][15] =
            {
                "10.0.0.253",
                "127.0.0.1",
                "172.16.90.254",
                "192.168.0.1",
                "224.30.0.253",
                "0"
            };

        if (MDrv_EMAC_Bist(1))
        {
            for (; idx < count; idx++)
            {
                memset(buffer, 0 , sizeof(buffer));
                sprintf(buffer, "ping %s", ip_addr[idx%5]);
                printf("\n=========>  %s for PHY Loopback test.....\n", buffer);
                run_command(buffer, 0);
            }
        }
        MDrv_EMAC_Bist(0);
    }
    return 0;
}

int do_Loopback(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    if (!emac_init)
    {
        printf("Please start emac first!\n");
        return 0;
    }

    MDrv_EMAC_Bist(2);
    return 0;
}

int do_loadspi (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    ulong ram_start;
    ulong spi_start;
    ulong len,i;
	if (argc != 4) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	spi_start = simple_strtoul(argv[1], NULL, 10);
	ram_start = simple_strtoul(argv[2], NULL, 10);
	len       = simple_strtoul(argv[3], NULL, 10);
    printf("copy from spi(%lux) to ram(%lux) with len=%lux\n",spi_start,ram_start,len);

    for(i=0;i<len;i+=4)
    {
        if(0==i%1024)printf(".");
        *((ulong*)(ram_start+i))=*((ulong*)(spi_start+i));
        //printf("[%x]=%x,[%x]=%x\n",ram_start+i,*((ulong*)(ram_start+i)),spi_start+i,*((ulong*)(spi_start+i)));
    }

	return 0;
}

int do_emacpowerdown (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MDrv_EMAC_PowerMode(0);
    return 0;
}

/***************************************************/

U_BOOT_CMD(
	estart, EMAC_INPUT_LEN,	1,	do_emac,
	"EMAC start",
	"reset  - reset EMAC controller\n"
	"start  - start EMAC controller\n"
);

U_BOOT_CMD(
	macaddr, EMAC_ADDR_INPUT_LEN,	1,	do_setMac,
	"setup EMAC MAC addr",
    "XX XX XX XX XX XX"
);

U_BOOT_CMD(
	ewavetest, EMAC_INPUT_LEN, 1,	do_ewavetest,
	"EMAC wave test",
	"1  - 100M\n"
	"2  - 10M\n"
);

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
	ebist, EMAC_INPUT_LEN,	1,	do_bistMac,
	"PHY loopback test",
	"ebist - PHY loopback test\n"
);

U_BOOT_CMD(
	edump, EMAC_INPUT_LEN,	1,	do_dumpMac,
	"EMAC Register settings dump",
    "edump - EMAC Register settings dump\n"
);

U_BOOT_CMD(
	eloopback, EMAC_INPUT_LEN,	1,	do_Loopback,
	"Long loopback test",
    "eloopback - Long loopback test\n"
);

U_BOOT_CMD(
	loadspi, 4,	4,	do_loadspi,
	"load data from SPI",
    "XX XX XX XX XX XX"
);

U_BOOT_CMD(
	epd, 4,	4,	do_emacpowerdown,
	"emac power down",
    "XX XX XX XX XX XX"
);
#endif

