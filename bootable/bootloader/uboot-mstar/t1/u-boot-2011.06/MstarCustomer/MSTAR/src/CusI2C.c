/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

/******************************************************************************/
/*                    Header Files                                            */
/* ****************************************************************************/
#include <common.h>
#include <apiSWI2C.h>
#if ((ENABLE_MSTAR_KENYA == 1)||(ENABLE_MSTAR_KERES == 1)||(ENABLE_MSTAR_KIRIN == 1)||(ENABLE_MSTAR_KRIS == 1)||(ENABLE_MSTAR_KRATOS == 1)||(ENABLE_MSTAR_KIWI == 1)||(ENABLE_MSTAR_KAYLA == 1))
#include <drvIIC.h>
#endif

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

#if ((ENABLE_MSTAR_KAISERIN == 1) || (ENABLE_MSTAR_KENYA == 1))
#define E_I2C_BUS_NUM 8
static SWI2C_BusCfg I2CBusCfg[E_I2C_BUS_NUM]=
    {
        //Example for SW I2C => { 1, PAD_DDCR_CK, PAD_DDCR_DA, 60, }, //SW_I2C  /SCL_PAD /SDA_PAD /Delay
        // Bus-0
        {PAD_I2CM0_SCL,PAD_I2CM0_SDA,400}, //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
        // Bus-1
        {PAD_I2CM0_SCL, PAD_I2CM0_SDA,400},   //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
        // Bus-2
        {PAD_I2CM0_SCL, PAD_I2CM0_SDA,100},   //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
        // Bus-3
        {PAD_I2CM0_SCL, PAD_I2CM0_SDA,100},   //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
        // Bus-4
        {PAD_I2CM0_SCL, PAD_I2CM0_SDA,100},   //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
        // Bus-5
        {PAD_I2CM0_SCL, PAD_I2CM0_SDA,100},   //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
        // Bus-6
        {PAD_I2CM0_SCL, PAD_I2CM0_SDA,100},   //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
        // Bus-7
        {PAD_I2CM0_SCL, PAD_I2CM0_SDA,100},   //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
        // Others, add other bus here
    };
#elif (ENABLE_MSTAR_KERES == 1) || (ENABLE_MSTAR_KIRIN == 1) || (ENABLE_MSTAR_KRIS == 1)||(ENABLE_MSTAR_KRATOS == 1) || (ENABLE_MSTAR_KIWI == 1)||(ENABLE_MSTAR_KAYLA == 1)
//empty
#elif (ENABLE_MSTAR_NAPOLI== 1) || (ENABLE_MSTAR_MUNICH== 1)
#define E_I2C_BUS_NUM 1
static SWI2C_BusCfg I2CBusCfg[E_I2C_BUS_NUM]=
{
	//Example for SW I2C => { 1, PAD_DDCR_CK, PAD_DDCR_DA, 60, }, //SW_I2C	/SCL_PAD /SDA_PAD /Delay
	// Bus-0
	{PAD_DDCR_CK,PAD_DDCR_DA,400}, //IS_SW_I2C	/SCL_PAD /SDA_PAD /Delay
	// Bus-1
	//{PAD_TGPIO2, PAD_TGPIO3,400},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-2
	//{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-3
	//{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-4
	//{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-5
	//{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-6
	//{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-7
	//{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Others, add other bus here
};
#elif (ENABLE_MSTAR_KAISER == 1) || (ENABLE_MSTAR_KANO == 1) || (ENABLE_MSTAR_CURRY == 1) || (ENABLE_MSTAR_KASTOR == 1)
#define E_I2C_BUS_NUM 8
static SWI2C_BusCfg I2CBusCfg[E_I2C_BUS_NUM]=
 {
	//Example for SW I2C => { 1, PAD_DDCR_CK, PAD_DDCR_DA, 60, }, //SW_I2C    /SCL_PAD /SDA_PAD /Delay
 	// Bus-0
 	{PAD_I2CM0_SCL, PAD_I2CM0_SDA,400}, //IS_SW_I2C    /SCL_PAD /SDA_PAD /Delay
 	// Bus-1
 	{PAD_I2CM1_SCL, PAD_I2CM1_SDA,400},    //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
 	// Bus-2
     	{PAD_I2CM0_SCL, PAD_I2CM0_SDA,100},    //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-3
	{PAD_I2CM0_SCL, PAD_I2CM0_SDA,100},    //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-4
	{PAD_I2CM0_SCL, PAD_I2CM0_SDA,100},    //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
 	// Bus-5
	{PAD_I2CM0_SCL, PAD_I2CM0_SDA,100},    //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-6
	{PAD_I2CM0_SCL, PAD_I2CM0_SDA,100},    //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-7
	{PAD_I2CM0_SCL, PAD_I2CM0_SDA,100},    //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Others, add other bus here
};
#elif (MS_BOARD_TYPE_SEL == BD_MST029B_10AFVA_A3 || MS_BOARD_TYPE_SEL == BD_MST029B_BOX_A3)
#define E_I2C_BUS_NUM 8
static SWI2C_BusCfg I2CBusCfg[E_I2C_BUS_NUM]=
{
	//Example for SW I2C => { 1, PAD_DDCR_CK, PAD_DDCR_DA, 60, }, //SW_I2C	/SCL_PAD /SDA_PAD /Delay
	// Bus-0
	{PAD_DDCR_CK,PAD_DDCR_DA,400}, //IS_SW_I2C	/SCL_PAD /SDA_PAD /Delay
	// Bus-1
	{PAD_TGPIO2, PAD_TGPIO3,400},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-2
	{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-3
	{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-4
	{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-5
	{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-6
	{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-7
	{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Others, add other bus here
};
#elif (MS_BOARD_TYPE_SEL == BD_MST050B_10ART_13225)
#define E_I2C_BUS_NUM 3
static SWI2C_BusCfg I2CBusCfg[E_I2C_BUS_NUM]=
{
    //Example for SW I2C => { 1, PAD_DDCR_CK, PAD_DDCR_DA, 60, }, //SW_I2C  /SCL_PAD /SDA_PAD /Delay
    //Bus-0
    {PAD_DDCR_CK, PAD_DDCR_DA,60}, //EEPROM
    //Bus-1
    {PAD_TGPIO2, PAD_TGPIO3,60}, //TUNER
    //Bus-2
    {PAD_I2S_IN_BCK, PAD_I2S_IN_SD, 60}, //EDINBURGH
    //Bus-3
    //{PAD_DDCDB_CK, PAD_DDCDB_DA, 60}, //HDMI 1 DDC
    //Bus-4
    //{PAD_DDCDC_CK, PAD_DDCDC_DA, 60}, //HDMI 1 DDC
    //Bus-5
    //{PAD_DDCDD_CK, PAD_DDCDD_DA, 60}, //HDMI 1 DDC
};
#elif (MS_BOARD_TYPE_SEL == BD_MST065B_10ART_13225)
#define E_I2C_BUS_NUM 3
static SWI2C_BusCfg I2CBusCfg[E_I2C_BUS_NUM]=
{
    //Example for SW I2C => { 1, PAD_DDCR_CK, PAD_DDCR_DA, 60, }, //SW_I2C  /SCL_PAD /SDA_PAD /Delay
    //Bus-0
    {PAD_DDCR_CK, PAD_DDCR_DA,60}, //EEPROM
    //Bus-1
    {PAD_TGPIO2, PAD_TGPIO3,60}, //TUNER
    //Bus-2
    {PAD_I2S_IN_BCK, PAD_I2S_IN_SD, 60}, //EDINBURGH
    //Bus-3
    //{PAD_DDCDB_CK, PAD_DDCDB_DA, 60}, //HDMI 1 DDC
    //Bus-4
    //{PAD_DDCDC_CK, PAD_DDCDC_DA, 60}, //HDMI 1 DDC
    //Bus-5
    //{PAD_DDCDD_CK, PAD_DDCDD_DA, 60}, //HDMI 1 DDC
};
#else
#define E_I2C_BUS_NUM 8
static SWI2C_BusCfg I2CBusCfg[E_I2C_BUS_NUM]=
{
	//Example for SW I2C => { 1, PAD_DDCR_CK, PAD_DDCR_DA, 60, }, //SW_I2C	/SCL_PAD /SDA_PAD /Delay
	// Bus-0
	{PAD_DDCR_CK,PAD_DDCR_DA,400}, //IS_SW_I2C	/SCL_PAD /SDA_PAD /Delay
	// Bus-1
	{PAD_TGPIO2, PAD_TGPIO3,400},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-2
	{PAD_DDCDA_CK, PAD_DDCDA_DA,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-3
	{PAD_DDCDB_CK, PAD_DDCDB_DA,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-4
	{PAD_DDCDC_CK, PAD_DDCDC_DA,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-5
	{PAD_DDCDD_CK, PAD_DDCDD_DA,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-6
	{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Bus-7
	{PAD_TGPIO2, PAD_TGPIO3,100},	//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
	// Others, add other bus here
};
#endif

int msI2C_init(void)
{
#if (ENABLE_MSTAR_KENYA == 1)
    UNUSED(I2CBusCfg[0]);
#endif
#if ((ENABLE_MSTAR_KENYA == 1)||(ENABLE_MSTAR_KERES==1)||(ENABLE_MSTAR_KIRIN==1) ||(ENABLE_MSTAR_KRIS==1) ||(ENABLE_MSTAR_KRATOS==1) ||(ENABLE_MSTAR_KIWI==1)||(ENABLE_MSTAR_KAYLA == 1))
#if (CONFIG_OAD_IN_MBOOT == 1)
    HWI2C_PORT hwi2c_port;
    hwi2c_port = getI2CPort(0);
    printf("IIC port == %d\n", hwi2c_port);
    MDrv_IIC_Init(hwi2c_port);
#endif
#else
	MApi_SWI2C_Init(I2CBusCfg,E_I2C_BUS_NUM);
#endif
	return 0;
}

