/* mstar-tv.h - Keytable for mstar_tv Remote Controller
 *
 * keymap imported from ir-keymaps.c
 *
 * Copyright (c) 2010 by Mauro Carvalho Chehab <mchehab@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#include <media/rc-core.h>
//#include <media/rc-map.h>
#include <linux/module.h>
/*
 * Jimmy Hsu <jimmy.hsu@mstarsemi.com>
 * this is the remote control that comes with the mstar smart tv
 * which based on STAOS standard.
 */

static struct rc_map_table mstar_tv[] = {
    { 0x7F46, KEY_POWER }, /* Power toggle */
    { 0x7F50, KEY_0 },
    { 0x7F49, KEY_1 },
    { 0x7F55, KEY_2 },
    { 0x7F59, KEY_3 },
    { 0x7F4D, KEY_4 },
    { 0x7F51, KEY_5 },
    { 0x7F5D, KEY_6 },
    { 0x7F48, KEY_7 },
    { 0x7F54, KEY_8 },
    { 0x7F58, KEY_9 },
    { 0x7F47, KEY_RED },
    { 0x7F4B, KEY_GREEN },
    { 0x7F57, KEY_YELLOW },
    { 0x7F5B, KEY_BLUE },
    { 0x7F52, KEY_UP },
    { 0x7F13, KEY_DOWN },
    { 0x7F06, KEY_LEFT },
    { 0x7F1A, KEY_RIGHT },
    { 0x7F0F, KEY_ENTER },
    { 0x7F1F, KEY_CHANNELUP },
    { 0x7F19, KEY_CHANNELDOWN },
    { 0x7F16, KEY_VOLUMEUP },
    { 0x7F15, KEY_VOLUMEDOWN },
    { 0x7F03, KEY_PAGEUP },
    { 0x7F05, KEY_PAGEDOWN },
    { 0x7F17, KEY_HOME},
    { 0x7F07, KEY_MENU },
    { 0x7F1B, KEY_BACK },
    { 0x7F5A, KEY_MUTE },
    { 0x7F0D, KEY_RECORD },     // DVR
    { 0x7F42, KEY_HELP },       // GUIDE
    { 0x7F14, KEY_INFO },
    { 0x7F40, KEY_KP0 },        // WINDOW
    { 0x7F04, KEY_KP1 },        // TV_INPUT
    { 0x7F0E, KEY_REWIND },
    { 0x7F12, KEY_FORWARD },
    { 0x7F02, KEY_PREVIOUSSONG },
    { 0x7F1E, KEY_NEXTSONG },
    { 0x7F01, KEY_PLAY },
    { 0x7F1D, KEY_PAUSE },
    { 0x7F11, KEY_STOP },
    { 0x7F44, KEY_AUDIO },      // (C)SOUND_MODE
    { 0x7F56, KEY_CAMERA },     // (C)PICTURE_MODE
    { 0x7F4C, KEY_ZOOM },       // (C)ASPECT_RATIO
    { 0x7F5C, KEY_CHANNEL },    // (C)CHANNEL_RETURN
    { 0x7F45, KEY_SLEEP },      // (C)SLEEP
    { 0x7F4A, KEY_EPG },        // (C)EPG
    { 0x7F10, KEY_LIST },       // (C)LIST
    { 0x7F53, KEY_SUBTITLE },   // (C)SUBTITLE
    { 0x7F41, KEY_FN_F1 },      // (C)MTS
    { 0x7F4E, KEY_FN_F2 },      // (C)FREEZE
    { 0x7F0A, KEY_FN_F3 },      // (C)TTX
    { 0x7F09, KEY_FN_F4 },      // (C)CC
    { 0x7F1C, KEY_FN_F5 },      // (C)TV_SETTING
    { 0x7F08, KEY_FN_F6 },      // (C)SCREENSHOT
    { 0x7F0B, KEY_F1 },         // MSTAR_BALANCE
    { 0x7F18, KEY_F2 },         // MSTAR_INDEX
    { 0x7F00, KEY_F3 },         // MSTAR_HOLD
    { 0x7F0C, KEY_F4 },         // MSTAR_UPDATE
    { 0x7F4F, KEY_F5 },         // MSTAR_REVEAL
    { 0x7F5E, KEY_F6 },         // MSTAR_SUBCODE
    { 0x7F43, KEY_F7 },         // MSTAR_SIZE
    { 0x7F5F, KEY_F8 },         // MSTAR_CLOCK
    { 0x7FFE, KEY_POWER2 },     // FAKE_POWER
    { 0x7FFF, KEY_OK },         // KEY_OK

    // 2nd IR controller.
    { 0x7D46, KEY_POWER },
    { 0x7D4C, KEY_MUTE },

    { 0x7D57, KEY_RED },
    { 0x7D40, KEY_GREEN },
    { 0x7D55, KEY_YELLOW },
    { 0x7D16, KEY_BLUE },

    { 0x7D12, KEY_REWIND },
    { 0x7D51, KEY_FORWARD },
    { 0x7D53, KEY_PREVIOUSSONG },
    { 0x7D10, KEY_NEXTSONG },
    { 0x7D56, KEY_PLAYPAUSE },//--Play/Pause
    { 0x7D17, KEY_STOP },//STOP

    { 0x7D47, KEY_CAMERA },//PICTURE_MODE
    { 0x7D43, KEY_AUDIO },//SOUND_MODE
    { 0x7D11, KEY_SLEEP },/* POWER_OFF (not power toggle) */
    { 0x7D58, KEY_ZOOM },//ASPECT_MODE
    { 0x7D44, KEY_LIST },
    { 0x7D50, KEY_FN_F1 },//NICAM-OVERSEAS
    { 0x7D1A, KEY_FN_F3 },//TEXT-OVERSEAS

    { 0x7D52, KEY_HOME },//--ึ๗าณ

    { 0x7D48, KEY_UP },         // UP
    { 0x7D4D, KEY_DOWN },        // DOWN
    { 0x7D4E, KEY_LEFT },        // LEFT
    { 0x7D49, KEY_RIGHT },        // RIGHT
    { 0x7D4A, KEY_ENTER },

    { 0x7D45, KEY_MENU },
    { 0x7D0D, KEY_BACK },        // EXIT

    { 0x7D0C, KEY_VOLUMEUP },
    { 0x7D19, KEY_VOLUMEDOWN },
    { 0x7D4B, KEY_KP1 },        // SOURCE
    { 0x7D18, KEY_CHANNEL },    //view back
    { 0x7D0F, KEY_CHANNELUP },
    { 0x7D5A, KEY_CHANNELDOWN },

    { 0x7D01, KEY_1 },
    { 0x7D02, KEY_2 },
    { 0x7D03, KEY_3 },
    { 0x7D04, KEY_4 },
    { 0x7D05, KEY_5 },
    { 0x7D06, KEY_6 },
    { 0x7D07, KEY_7 },
    { 0x7D08, KEY_8 },
    { 0x7D09, KEY_9 },
    { 0x7D00, KEY_0 },
    { 0x7D4F, KEY_INFO }, /*display*/
    { 0x7D5b, KEY_TEXT }, /*CC*/
    { 0x7D5F, KEY_CUSTOM_1 }, /* Netflix */
    { 0x7D8C, KEY_CUSTOM_2 },
    { 0x7D8D, KEY_CUSTOM_3 },
    { 0x7D5E, KEY_CUSTOM_2 },
    { 0X7D3B, KEY_CUSTOM_3 },

    { 0x7D96, KEY_TIME }, /* SLEEP_TIMER */
    { 0x7D97, KEY_WAKEUP }, /* POWER_ON (not power toggle) */

    { 0x7D98, KEY_FN_F7 }, /* HDMI 1 */
    { 0x7D99, KEY_FN_F8 }, /* HDMI 2 */
    { 0x7D9A, KEY_FN_F9 }, /* HDMI 3 */
    { 0x7D9B, KEY_FN_F10 }, /* HDMI 4 */
    { 0x7D9C, KEY_FN_F11 }, /* Composite 1 */
    { 0x7D9D, KEY_FN_F12 }, /* Component */
    { 0x7D9E, KEY_TUNER }, /* Antenna */

};

static struct rc_map_list mstar_tv_map = {
	.map = {
		.scan    = mstar_tv,
		.size    = ARRAY_SIZE(mstar_tv),
		.rc_type = RC_TYPE_UNKNOWN,	/* Legacy IR type */
		.name    = RC_MAP_MSTAR_TV,
	}
};

static int __init init_rc_map_mstar_tv(void)
{
	return rc_map_register(&mstar_tv_map);
}

static void __exit exit_rc_map_mstar_tv(void)
{
	rc_map_unregister(&mstar_tv_map);
}

module_init(init_rc_map_mstar_tv)
module_exit(exit_rc_map_mstar_tv)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mauro Carvalho Chehab <mchehab@redhat.com>");
