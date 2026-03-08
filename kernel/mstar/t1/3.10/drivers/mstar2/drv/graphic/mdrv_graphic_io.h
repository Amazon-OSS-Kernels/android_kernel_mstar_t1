/**
* Copyright (c) 2006 – 2017 MStar Semiconductor, Inc.
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

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// @file   mdrv_graphic_io.h
// @brief  Graphic Driver Interface
// @author MStar Semiconductor Inc.
////////////////////////////////////////////////////////////////////////////////////////////////*/

#ifndef MSTAR_FB_GRAPHIC_H
#define MSTAR_FB_GRAPHIC_H


/* ========================================================================= */
/* prototype of function */
static int mstar_fb_set_par(struct fb_info *pinfo);
static int mstar_fb_pan_display(struct fb_var_screeninfo *var, struct fb_info *info);
static int mstar_fb_check_var(struct fb_var_screeninfo *var, struct fb_info *info);
static int mstar_fb_blank(int blank, struct fb_info *info);
static int mstar_fb_setcolreg(unsigned regno, unsigned red, unsigned green, unsigned blue, unsigned transp, struct fb_info *info);
static int _mstar_fb_mmap(struct fb_info *pinfo, struct vm_area_struct *vma);
static void mstar_fb_destroy(struct fb_info *info);
static void mstar_fb_fillrect(struct fb_info *p, const struct fb_fillrect *rect);
static void mstar_fb_copyarea(struct fb_info *p, const struct fb_copyarea *area);
static void mstar_fb_imageblit(struct fb_info *p, const struct fb_image *image);
static MS_U32 get_line_length(int xres_virtual, int bpp);
static void _fb_strewin_update(struct fb_var_screeninfo *var);
static void _fb_gwin_enable(MS_BOOL bEnable);
static void _fb_buf_init(struct fb_info *pinfo, unsigned long pa);
static EN_DRV_GOPColorType get_color_fmt(struct fb_var_screeninfo *var);
#endif
