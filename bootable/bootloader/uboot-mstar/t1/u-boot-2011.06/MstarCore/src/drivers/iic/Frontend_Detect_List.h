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

/* READ ME
Keys:
1. The detect list implies the demod/tuner checking order
   You can exchange the devices' order in below list to change the detecting orders
   EX:  if #define TUNER 04TH_SCAN_DEVICE TUNER AV2012
        => It implies that the TUNER AV2012 will 4th be checked whether is onto the PCB

2. If a demod/tuner is not in the detection list (full as DEMOD_NULL/TUNER_NULL),
   the demod's/tuner's driver will be compiled as empty codes

3. The demod and tuner devices' name can be referenced in
   \\THEALE\Uranus\DDI_Misc\cus_mstar\project\mstar_demo\Frontend_Enum_Device.h

Note: AV2018,AV2028 can not be distinguished by SW
      So it does not work if av2018 and av2028 are put into the list at the same time
*/

//------------------------------------------------------------------------------------
//  Frontend external demod detection list
//------------------------------------------------------------------------------------
#define DEMOD_00TH_SCAN_DEVICE DEMOD_MSB1236C
#define DEMOD_01ST_SCAN_DEVICE DEMOD_MSB123X
#define DEMOD_02ND_SCAN_DEVICE DEMOD_MSB131X
#define DEMOD_03RD_SCAN_DEVICE DEMOD_NULL
#define DEMOD_04TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_05TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_06TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_07TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_08TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_09TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_10TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_11TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_12TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_13TH_SCAN_DEVICE DEMOD_NULL
#define DEMOD_14TH_SCAN_DEVICE DEMOD_NULL

/* Supported external demod devices
DEMOD_MSB1236C          DVBT/DVBT2 (Share driver with DEMOD_MSB1233C)
DEMOD_MSB123X           DVBT/DVBT2
DEMOD_MSB131X           DVBS/DVBS2
DEMOD_MSB1237           ATSC
*/

//------------------------------------------------------------------------------------
//  Frontend tuner detection list
//------------------------------------------------------------------------------------
#define TUNER_00TH_SCAN_DEVICE TUNER_MXL603
#define TUNER_01ST_SCAN_DEVICE TUNER_TDA18250A //if crystal is 16,set this to TUNER_TDA18250B
#define TUNER_02ND_SCAN_DEVICE TUNER_PHILIPS_TDA18250HN
#define TUNER_03RD_SCAN_DEVICE TUNER_TDA18260
#define TUNER_04TH_SCAN_DEVICE TUNER_AV2012
#define TUNER_05TH_SCAN_DEVICE TUNER_NULL
#define TUNER_06TH_SCAN_DEVICE TUNER_NULL
#define TUNER_07TH_SCAN_DEVICE TUNER_NULL
#define TUNER_08TH_SCAN_DEVICE TUNER_NULL
#define TUNER_09TH_SCAN_DEVICE TUNER_NULL
#define TUNER_10TH_SCAN_DEVICE TUNER_NULL
#define TUNER_11TH_SCAN_DEVICE TUNER_NULL
#define TUNER_12TH_SCAN_DEVICE TUNER_NULL
#define TUNER_13TH_SCAN_DEVICE TUNER_NULL
#define TUNER_14TH_SCAN_DEVICE TUNER_NULL

/* Supported external demod devices
TUNER_MXL603              DVBC/DVBT/DVBT2/ATSC/ISDBT
TUNER_TDA18250A           DVBC                       (Crystal_27, same ID with TUNER_TDA18250B)
TUNER_TDA18250B           DVBC                       (Crystal_16, same ID with TUNER_TDA18250A)
TUNER_PHILIPS_TDA18250HN  DVBC
TUNER_TDA18260            DVBC
TUNER_AV2012              DVBS/DVBS2                 (Share driver with AV2011)
TUNER_AV2018              DVBS/DVBS2                 (Same ID with TUNER_AV2028)
TUNER_AV2028              DVBS                       (Same ID with TUNER_AV2018)
*/

