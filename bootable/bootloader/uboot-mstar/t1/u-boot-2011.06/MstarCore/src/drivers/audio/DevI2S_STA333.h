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

#ifndef _DEVI2S_H_
#define _DEVI2S_H_

#ifndef _BIT0
#define _BIT0  0x0001
#endif
#ifndef _BIT1
#define _BIT1  0x0002
#endif
#ifndef _BIT2
#define _BIT2  0x0004
#endif
#ifndef _BIT3
#define _BIT3  0x0008
#endif
#ifndef _BIT4
#define _BIT4  0x0010
#endif
#ifndef _BIT5
#define _BIT5  0x0020
#endif
#ifndef _BIT6
#define _BIT6  0x0040
#endif
#ifndef _BIT7
#define _BIT7  0x0080
#endif
#ifndef _BIT8
#define _BIT8  0x0100
#endif
#ifndef _BIT9
#define _BIT9  0x0200
#endif
#ifndef _BIT10
#define _BIT10 0x0400
#endif
#ifndef _BIT11
#define _BIT11 0x0800
#endif
#ifndef _BIT12
#define _BIT12 0x1000
#endif
#ifndef _BIT13
#define _BIT13 0x2000
#endif
#ifndef _BIT14
#define _BIT14 0x4000
#endif
#ifndef _BIT15
#define _BIT15 0x8000
#endif

#define DDX_I2C_WRITE_ADD							        0x38
#define STA333_ADDR     (((U16) E_I2C_BUS_SYS << 8) | DDX_I2C_WRITE_ADD)
#define DDX_CONFIGURE_A								0x00
#define DDX_CONFIGURE_B								0x01
#define DDX_CONFIGURE_C								0x02
#define DDX_CONFIGURE_D								0x03
#define DDX_CONFIGURE_E								0x04
#define DDX_CONFIGURE_F								0x05

#define DDX_MUTE									0x06
#define DDX_MAIN_VOLUME								0x07
#define DDX_CHANNEL1_VOL							0x08
#define DDX_CHANNEL2_VOL							0x09
#define DDX_CHANNEL3_VOL 							0x0a

#define DDX_AUTO1			 						0x0b
#define DDX_AUTO2			 						0x0c
#define DDX_AUTO3			 						0x0d

#define DDX_CHANNEL1_CFG 							0x0e
#define DDX_CHANNEL2_CFG 							0x0f
#define DDX_CHANNEL3_CFG 							0x10

#define DDX_TONEBASS								0x11
#define DDX_L1AR_RATE								0x12
#define DDX_L1AR_THRESHOLD							0x13
#define DDX_L2AR_RATE								0x14
#define DDX_L2AR_THRESHOLD							0x15

#define		REG_CFADDR1			0x16	/* Cfaddr1 */
#define		REG_B1CF1			0x17	/* B1cf1 */
#define		REG_B1CF2			0x18	/* B1cf2 */
#define		REG_B1CF3			0x19	/* B1cf3 */
#define		REG_B2CF1			0x1a	/* B2cf1 */
#define		REG_B2CF2			0x1b	/* B2cf2 */
#define		REG_B2CF3			0x1c	/* B2cf3 */
#define		REG_A1CF1			0x1d	/* A1cf1 */
#define		REG_A1CF2			0x1e	/* A1cf2 */
#define		REG_A1CF3			0x1f	/* A1cf3 */
#define		REG_A2CF1			0x20	/* A2cf1 */
#define		REG_A2CF2			0x21	/* A2cf2 */
#define		REG_A2CF3			0x22	/* A2cf3 */
#define		REG_B0CF1			0x23	/* B0cf1 */
#define		REG_B0CF2			0x24	/* B0cf2 */
#define		REG_B0CF3			0x25	/* B0cf3 */
#define		REG_CFUD			0x26	/* Cfud */


#define MAX_VOLUME				50

void DevI2s_STA333_Init(void);
void DevI2S_STA333_SubwoofValue(MS_U8 Subwoof);
#if 0//(ENABLE_WALL_MODE)
void DevI2SSetCFTableByMode(SoundWallModeType mode);
#else
void DevI2S_STA333_SetCFTable(void);
#endif
void DecI2S_STA333_Patch(BYTE ucCtl);
MS_BOOL STA333_GetPowerDNStatus(void);
#endif
