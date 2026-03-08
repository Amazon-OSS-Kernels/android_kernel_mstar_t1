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

#ifndef MS_SDMMC_H
#define MS_SDMMC_H

#include <linux/cdev.h>

//***********************************************************************************************************
// Config Setting (Externel)
//***********************************************************************************************************

#if defined(CONFIG_MSTAR_SDMMC_TWOCARDS)
#define EN_SDMMC_TWO_CARDS     (TRUE)
#else
#define EN_SDMMC_TWO_CARDS     (FALSE)
#endif

#if defined(CONFIG_MSTAR_SDMMC_REVCDZ)
#define EN_SDMMC_CDZREV        (TRUE)
#else
#define EN_SDMMC_CDZREV        (FALSE)
#endif

#if defined(CONFIG_MSTAR_SDMMC_REVWP)
#define EN_SDMMC_WPREV         (TRUE)
#else
#define EN_SDMMC_WPREV         (FALSE)
#endif

#if defined(CONFIG_MSTAR_SDMMC1_HOTP)
#define EN_SDMMC1_HOTP         (TRUE)
#else
#define EN_SDMMC1_HOTP         (FALSE)
#endif

#if defined(CONFIG_MSTAR_SDMMC2_HOTP)
#define EN_SDMMC2_HOTP         (TRUE)
#else
#define EN_SDMMC2_HOTP         (FALSE)
#endif

#if defined(CONFIG_MSTAR_SDMMC1_FAKECDZ)
#define EN_SDMMC1_FAKECDZ      (TRUE)
#else
#define EN_SDMMC1_FAKECDZ      (FALSE)
#endif

#if defined(CONFIG_MSTAR_SDMMC2_FAKECDZ)
#define EN_SDMMC2_FAKECDZ      (TRUE)
#else
#define EN_SDMMC2_FAKECDZ      (FALSE)
#endif

#if defined(CONFIG_MSTAR_SDMMC_HIGHCLK)
#define MAX_CLK_SPEED1         24600000
#define MAX_CLK_SPEED2         24600000
#else
#define MAX_CLK_SPEED1         19200000
#define MAX_CLK_SPEED2         19200000
#endif


//***********************************************************************************************************
typedef enum
{
	EV_SDMMC1 = 0,
	EV_SDMMC2 = 1,

} SlotEmType;

struct ms_sdmmc_host
{
	struct platform_device	*pdev;
	struct ms_sdmmc_slot *sdmmc_slot[2];
};

struct ms_sdmmc_slot
{
	struct mmc_host		*mmc;

	unsigned int	slotNo;   	//Slot No.
	unsigned int	mieIRQNo;	//MIE IRQ No.
	unsigned int	cdzIRQNo;	//CDZ IRQ No.
	unsigned int	irqIP;		//IP for INT
	unsigned int	currClk;	//Current Clock

	int ro;		  //WP
	int card_det;		//Card Detect

	/****** DMA buffer used for transmitting *******/
	u32 *dma_buffer;
	dma_addr_t dma_phy_addr;

	/***** Tasklet for hotplug ******/
	struct tasklet_struct   hotplug_tasklet;

};  /* struct ms_sdmmc_hot*/


#endif

