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
{ 0x7F46, KEY_POWER },
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
{ 0x7F0D, KEY_RECORD },     /* DVR */
{ 0x7F42, KEY_HELP },       /* GUIDE */
{ 0x7F14, KEY_INFO },
{ 0x7F40, KEY_KP0 },        /* WINDOW */
{ 0x7F04, KEY_KP1 },        /* TV_INPUT */
{ 0x7F0E, KEY_REWIND },
{ 0x7F12, KEY_FORWARD },
{ 0x7F02, KEY_PREVIOUSSONG },
{ 0x7F1E, KEY_NEXTSONG },
{ 0x7F01, KEY_PLAY },
{ 0x7F1D, KEY_PAUSE },
{ 0x7F11, KEY_STOP },
{ 0x7F44, KEY_AUDIO },      /* (C)SOUND_MODE */
{ 0x7F56, KEY_CAMERA },     /* (C)PICTURE_MODE */
{ 0x7F4C, KEY_ZOOM },       /* (C)ASPECT_RATIO */
{ 0x7F5C, KEY_CHANNEL },    /* (C)CHANNEL_RETURN */
{ 0x7F45, KEY_SLEEP },      /* (C)SLEEP */
{ 0x7F4A, KEY_EPG },        /* (C)EPG */
{ 0x7F10, KEY_LIST },       /* (C)LIST */
{ 0x7F53, KEY_SUBTITLE },   /* (C)SUBTITLE */
{ 0x7F41, KEY_FN_F1 },      /* (C)MTS */
{ 0x7F4E, KEY_FN_F2 },      /* (C)FREEZE */
{ 0x7F0A, KEY_FN_F3 },      /* (C)TTX */
{ 0x7F09, KEY_FN_F4 },      /* (C)CC */
{ 0x7F1C, KEY_FN_F5 },      /* (C)TV_SETTING */
{ 0x7F08, KEY_FN_F6 },      /* (C)SCREENSHOT */
{ 0x7F0B, KEY_F1 },         /* MSTAR_BALANCE */
{ 0x7F18, KEY_F2 },         /* MSTAR_INDEX */
{ 0x7F00, KEY_F3 },         /* MSTAR_HOLD */
{ 0x7F0C, KEY_F4 },         /* MSTAR_UPDATE */
{ 0x7F4F, KEY_F5 },         /* MSTAR_REVEAL */
{ 0x7F5E, KEY_F6 },         /* MSTAR_SUBCODE */
{ 0x7F43, KEY_F7 },         /* MSTAR_SIZE */
{ 0x7F5F, KEY_F8 },         /* MSTAR_CLOCK */
{ 0x7FFE, KEY_POWER2 },     /* FAKE_POWER */
{ 0x7FFF, KEY_OK },         /* KEY_OK */

/* Amazon IR controller */
{ 0x7D46, KEY_POWER },
{ 0x7D4C, KEY_MUTE },
{ 0x7D16, KEY_REWIND },
{ 0x7D17, KEY_FORWARD },
{ 0x7D5B, KEY_PLAYPAUSE },
{ 0x7D9F, KEY_HOME },

{ 0x7D48, KEY_UP },
{ 0x7D4D, KEY_DOWN },
{ 0x7D4E, KEY_LEFT },
{ 0x7D49, KEY_RIGHT },
{ 0x7D4A, KEY_ENTER },

{ 0x7D45, KEY_MENU },
{ 0x7D0D, KEY_BACK },

{ 0x7D0C, KEY_VOLUMEUP },
{ 0x7D19, KEY_VOLUMEDOWN },
{ 0x7D0F, KEY_CHANNELUP },
{ 0x7D5A, KEY_CHANNELDOWN },

{ 0x7D5F, KEY_CUSTOM_1 },
{ 0x7DA1, KEY_CUSTOM_2 },
{ 0x7DA2, KEY_CUSTOM_3 },
{ 0x7DA3, KEY_CUSTOM_4 },
{ 0x7DA4, KEY_BUTTON_1 },
{ 0x7DA5, KEY_BUTTON_2 },
{ 0x7DA0, KEY_SEARCH },
{ 0x7D14, KEY_LIVETV},
{ 0x7D96, KEY_TIME},
{ 0x7DB1, KEY_LAST},
{ 0x7DFF, KEY_IRFALLBACK},
/* {KEY_LIVETV} No IR key */

/* Toshiba IR controller */
{ 0xBF12, KEY_POWER  },
{ 0xBF15, KEY_SLEEP  },
{ 0xBF0F, KEY_KP1    }, /* TV_INPUT */
{ 0xBF4C, KEY_FN_F2  }, /* Screen Freeze */
{ 0xBF2D, KEY_CAMERA }, /* PICTURE_MODE */
{ 0xBF54, KEY_F7     }, /* SIZE */
{ 0xBF00, KEY_0      },
{ 0xBF01, KEY_1      },
{ 0xBF02, KEY_2      },
{ 0xBF03, KEY_3      },
{ 0xBF04, KEY_4      },
{ 0xBF05, KEY_5      },
{ 0xBF06, KEY_6      },
{ 0xBF07, KEY_7      },
{ 0xBF08, KEY_8      },
{ 0xBF09, KEY_9      },
{ 0xBF0A, KEY_DOT    },
{ 0xBF5D, KEY_FAVORITES	},
{ 0xBF1A, KEY_VOLUMEUP },
{ 0xBF1E, KEY_VOLUMEDOWN },
{ 0xBF10, KEY_MUTE },

{ 0xBF62, KEY_LANGUAGE }, /* Select second audio for CVAA */
{ 0xBF60, KEY_ASSIST },   /* Assist function for CVAA */

{ 0xBF1B, KEY_CHANNELUP    },
{ 0xBF1F, KEY_CHANNELDOWN  },
{ 0xBF82, KEY_UP           },
{ 0xBF1D, KEY_LEFT         },
{ 0xBF89, KEY_ENTER        },
{ 0xBF19, KEY_RIGHT        },
{ 0xBF83, KEY_DOWN         },
{ 0xBF80, KEY_MENU         },
{ 0xBF58, KEY_HOME         },
{ 0xBFD9, KEY_BACK         },
{ 0xBF84, KEY_INFO         },

{ 0xBF17, KEY_PREVIOUS     }, /* Channel RTN */
{ 0xBF57, KEY_SUBTITLE	   }, /* C. CAPTION */

{ 0xBF34, KEY_REWIND       },
{ 0xBF30, KEY_PLAYPAUSE    },
{ 0xBF31, KEY_STOP         },
{ 0xBF33, KEY_FORWARD      },

{ 0xBF7F, KEY_SLEEP        },
{ 0xBF7E, KEY_WAKEUP	   },
{ 0xBF20, KEY_TV_COMPOSITE },     /* Composite */
{ 0xBF22, KEY_TV_INPUT_HDMI_1 },  /* HDMI 1 */
{ 0xBF23, KEY_TV_INPUT_HDMI_2 },  /* HDMI 2 */
{ 0xBF24, KEY_TV_INPUT_HDMI_3 },  /* HDMI 3 */
{ 0xBF3D, KEY_TV_ANTENNA_CABLE }, /* Antenna */
{ 0xBF38, KEY_TV_INPUT_HDMI_1 },  /* Additional HDMI 1 code */
{ 0xBF3A, KEY_TV_INPUT_HDMI_3 },  /* Additional HDMI 3 code */

/* Insignia IR remote */
{ 0x051D, KEY_KP1	},
{ 0x050F, KEY_POWER	},
{ 0x0513, KEY_SLEEP	},
{ 0x0553, KEY_FN_F2	}, /* Screen Freeze */
{ 0x0546, KEY_CAMERA	}, /* PICUTRE Mode */
{ 0x0544, KEY_F7	}, /* Zoom, Picture size */
{ 0x0500, KEY_1		},
{ 0x0501, KEY_2		},
{ 0x0502, KEY_3		},
{ 0x0503, KEY_4		},
{ 0x0504, KEY_5		},
{ 0x0505, KEY_6		},
{ 0x0506, KEY_7		},
{ 0x0507, KEY_8		},
{ 0x0508, KEY_9		},
{ 0x0541, KEY_DOT	},
{ 0x0509, KEY_0		},
{ 0x054B, KEY_FAVORITES },
{ 0x050C, KEY_VOLUMEUP	},
{ 0x050D, KEY_VOLUMEDOWN},
{ 0x050E, KEY_MUTE	},
{ 0x0511, KEY_LANGUAGE	}, /* SAP, Second Audio Program */
{ 0x059D, KEY_ASSIST	}, /* Assist function for CVAA */
{ 0x050A, KEY_CHANNELUP	},
{ 0x050B, KEY_CHANNELDOWN},
{ 0x0542, KEY_UP	},
{ 0x0516, KEY_LEFT	},
{ 0x0518, KEY_ENTER	},
{ 0x0515, KEY_RIGHT	},
{ 0x0543, KEY_DOWN	},
{ 0x0514, KEY_MENU	},
{ 0x051B, KEY_HOME	}, /* Exit key */
{ 0x057B, KEY_BACK	},
{ 0x0517, KEY_INFO	},
{ 0x0510, KEY_BACK	}, /* KEY_BACK */
{ 0x051C, KEY_TEXT	}, /* Caption */
{ 0x057E, KEY_REWIND	},
{ 0x0570, KEY_PLAYPAUSE	},
{ 0x0571, KEY_STOP	},
{ 0x057D, KEY_FORWARD	},
{ 0x05B9, KEY_HOME	},
/* { 0x054A, KEY_OPTIONS },*/
{ 0x05AF, KEY_LIVETV	},
{ 0x0520, KEY_CUSTOM_1	},
{ 0x0527, KEY_CUSTOM_2	},
{ 0x0522, KEY_CUSTOM_3	},
{ 0x0523, KEY_CUSTOM_4	},
/* { 0x05AA, KEY_AVR_INPUT },*/
{ 0x05A2, KEY_TV_INPUT_HDMI_1	},
{ 0x05A3, KEY_TV_INPUT_HDMI_2	},
{ 0x05A4, KEY_TV_INPUT_HDMI_3	},
{ 0x054F, KEY_TV_ANTENNA_CABLE	},
{ 0x0561, KEY_WAKEUP },
{ 0x0560, KEY_SLEEP  },
{ 0x054C, KEY_TV_COMPOSITE },
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
