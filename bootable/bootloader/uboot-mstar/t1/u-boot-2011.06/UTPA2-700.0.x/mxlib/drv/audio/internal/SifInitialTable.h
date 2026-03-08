/**
 * Copyright (c) 2006 <96> 2016 MStar Semiconductor, Inc.
 * This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


enum
{
    FM_CARRIER1_ON_AMPLITUDE_THRESHOLD=0x00 ,
    FM_CARRIER1_OFF_AMPLITUDE_THRESHOLD=0x01,
    FM_CARRIER1_ON_NSR_THRESHOLD=0x02,
    FM_CARRIER1_OFF_NSR_THRESHOLD=0x03
};

enum
{
       AM_CARRIER1_ON_THRESHOLD=0x00,
       AM_CARRIER1_OFF_THRESHOLD=0x01
};

enum
{
    // use BIT_5 for Nicam
    NICAM_FM2NICAM_PHASE_ERROR_THR=0x20,
    NICAM_NICAM2FM_PHASE_ERROR_THR=0x21
};


enum
{
    BTSC_GAIN       =0x00,
    FM_StdM_GAIN    =0x01,
    FM_StdX_GAIN    =0x03,
    NICAM_GAIN      =0x04,
    AM_GAIN     =0x05
};

enum
{
    BTSC_MONO_ON_NSR_THRESHOLD          =0x00,
    BTSC_MONO_OFF_NSR_THRESHOLD     =0x01,
    BTSC_PILOT_ON_AMPLITUDE_THRESHOLD   =0x02,
    BTSC_PILOT_OFF_AMPLITUDE_THRESHOLD  =0x03,
    BTSC_SAP_ON_NSR_THRESHOLD           =0x04,
    BTSC_SAP_OFF_NSR_THRESHOLD          =0x05,
    BTSC_STEREO_ON_THRESHOLD            =0x06,
    BTSC_STEREO_OFF_THRESHOLD           =0x07,
    BTSC_SAP_ON_AMPLITUDE_THRESHOLD =0x08,
    BTSC_SAP_OFF_AMPLITUDE_THRESHOLD    =0x09
};

enum
{
    A2_CARRIER1_ON_AMPLITUDE_THRESHOLD      =0x00,
    A2_CARRIER1_OFF_AMPLITUDE_THRESHOLD =0x01,
    A2_CARRIER1_ON_NSR_THRESHOLD            =0x02,
    A2_CARRIER1_OFF_NSR_THRESHOLD           =0x03,
    A2_CARRIER2_ON_AMPLITUDE_THRESHOLD      =0x04,
    A2_CARRIER2_OFF_AMPLITUDE_THRESHOLD =0x05,
    A2_CARRIER2_ON_NSR_THRESHOLD            =0x06,
    A2_CARRIER2_OFF_NSR_THRESHOLD           =0x07,
    A2_PILOT_ON_AMPLITUDE_THRESHOLD     =0x08,
    A2_PILOT_OFF_AMPLITUDE_THRESHOLD        =0x09,
    A2_POLIT_MODE_VALID_RATIO               =0x0a,
    A2_POLIT_MODE_INVALID_RATIO             =0x0b,
    A2_POLIT_STEREO_ON_RATIO                    =0x0c,
    A2_POLIT_STEREO_OFF_RATIO               =0x0d,
    A2_M_POLIT_DUAL_ON_RATIO                =0x0e,
    A2_M_POLIT_DUAL_OFF_RATIO               =0x0f
};

//////////////////////////////////////////////////////////////



