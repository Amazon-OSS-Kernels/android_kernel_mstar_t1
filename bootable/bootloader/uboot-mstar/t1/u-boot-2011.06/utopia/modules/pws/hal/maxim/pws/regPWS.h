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
//******************************************************************************
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////

#ifndef _REGPWS_H_
#define _REGPWS_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
#define BIT0    (0x0001UL)
#define BIT1    (0x0002UL)
#define BIT2    (0x0004UL)
#define BIT3    (0x0008UL)
#define BIT4    (0x0010UL)
#define BIT5    (0x0020UL)
#define BIT6    (0x0040UL)
#define BIT7    (0x0080UL)
#define BIT8    (0x0100UL)
#define BIT9    (0x0200UL)
#define BIT10   (0x0400UL)
#define BIT11   (0x0800UL)
#define BIT12   (0x1000UL)
#define BIT13   (0x2000UL)
#define BIT14   (0x4000UL)
#define BIT15   (0x8000UL)

#define PM_OFF_FLAG         (0x0E68UL)
#define CHIPTOP(x)          (0x1E00UL + x)

//PM
#define PM_ADC_RGB              (0x2508UL)
#define PM_ADC_Y                (0x2509UL)
#define PM_GMC                  (0x250AUL)
#define PM_CVBS_BUF_OUT         (0x2570UL)
#define PM_DAC_CVBS             (0x258CUL)
#define PM_ADC_VCTRL_L          (0x2534UL)
#define PM_ADC_VCTRL_H          (0x2535UL)
#define PM_ADC_ICTRL_RGB_L      (0x2536UL)
#define PM_ADC_ICTRL_RGB_H      (0x2537UL)
#define PM_ADC_VTRL_YC_L        (0x253AUL)
#define PM_ADC_VTRL_YC_H        (0x253BUL)
#define PM_ADC_ICTRL_YC_L       (0x253CUL)
#define PM_ADC_ICTRL_YC_H       (0x253DUL)

//H264
#define SVD_REG_BASE            (0x1B00UL)  // 0x1B00 - 0x1BFF
#define REG_SVD_SW_RST          ( SVD_REG_BASE + 0X0A*2 )
    #define REG_SVD_RESET_SWRST         BIT0
    #define REG_SVD_RESET_CPURST        BIT1
    #define REG_SVD_RESET_SWRST_FIN     BIT2
    #define REG_SVD_RESET_CPURST_FIN    BIT3

#define MVD_CTRL                (0x1100UL)
    #define MVD_CTRL_RST                BIT0
    #define MVD_CTRL_INIT               BIT2

#endif
