/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
////////////////////////////////////////////////////////////////////////////////
//
//
//

#ifndef _MANHATTAN_BANDWIDTH_REGTABLE_H_
#define _MANHATTAN_BANDWIDTH_REGTABLE_H_

typedef enum
{
    BWTABLE_Normal_mode,
    BWTABLE_1080p_mode,
    BWTABLE_HDMI4kx2k_mode,
    BWTABLE_HDMI4kx2k_RGB_BwdHigh_mode,
    BWTABLE_HDMI4kx2k_RGB_BwdMed_mode,
    BWTABLE_HDMI4kx2k_RGB_BwdIdle_mode,
    BWTABLE_HDMI4kx2k_YUV444_BwdHigh_mode,
    BWTABLE_HDMI4kx2k_YUV444_BwdMed_mode,
    BWTABLE_HDMI4kx2k_YUV444_BwdIdle_mode,
    BWTABLE_HDMI4kx2k_YUV422_BwdHigh_mode,
    BWTABLE_HDMI4kx2k_YUV422_BwdMed_mode,
    BWTABLE_HDMI4kx2k_YUV422_BwdIdle_mode,
    BWTABLE_dummy_mode,
    BWTABLE_MM4kx2k_YUV422_BwdHigh_mode,
    BWTABLE_MM4kx2k_YUV422_BwdMed_mode,
    BWTABLE_MM4kx2k_YUV422_BwdIdle_mode,
    BWTABLE_NUMS
}
BWTABLE_Group;

#endif
