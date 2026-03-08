/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __MEMORY_MAP_SEL_H__
#define __MEMORY_MAP_SEL_H__

//------ SYSTEM MEMORY MAP ---------------------------------------------------------
#if defined(CONFIG_MSTAR_TITANIA_MMAP_256MB)
    #include "mmap_256mb.h"
#elif defined(CONFIG_MSTAR_TITANIA_MMAP_256MB_256MB)
    #include "mmap_256mb_256mb.h"
#elif defined(CONFIG_MSTAR_TITANIA_MMAP_512MB)
    #include "mmap_512mb.h"
#elif defined(CONFIG_MSTAR_TITANIA_MMAP_512MB_256MB)
    #include "mmap_512mb_256mb.h"
#elif defined(CONFIG_MSTAR_TITANIA_MMAP_512MB_512MB)
    #include "mmap_512mb_512mb.h"
#elif defined(CONFIG_MSTAR_TITANIA_MMAP_1024MB_256MB)
    #include "mmap_1024mb_256mb.h"
#elif defined(CONFIG_MSTAR_TITANIA_MMAP_1024MB_512MB)
    #include "mmap_1024mb_512mb.h"
#elif defined(CONFIG_MSTAR_TITANIA_MMAP_1024MB_1024MB)
    #include "mmap_1024mb_1024mb.h"
#else
    #error "[ERROR] Unknown MMAP Selection"
#endif

#endif /* __MEMORY_MAP_SEL_H__ */
