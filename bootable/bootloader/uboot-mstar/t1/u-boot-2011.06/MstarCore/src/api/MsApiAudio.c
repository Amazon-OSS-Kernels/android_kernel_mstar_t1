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

#include <config.h>
#include <drvGPIO.h>
#include <audio/MsDrvAudio.h>

#if(ENABLE_POWER_MUSIC==1)
int play_powermusic_main(void)
{
    //Audio_Amplifier_ON(); // Too early, can't do Amplifier_ON here, need do it after melody set play command
    MsDrv_PlayMelody();
    return 0;
}
#endif