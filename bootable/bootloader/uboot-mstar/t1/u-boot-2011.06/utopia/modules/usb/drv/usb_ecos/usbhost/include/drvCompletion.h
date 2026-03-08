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


#ifndef __LINUX_COMPLETION_H
#define __LINUX_COMPLETION_H

#include "MsCommon.h"

struct completion {
  U32 done;
};

#define COMPLETION_INITIALIZER(work) \
  {0}

#define DECLARE_COMPLETION(work) \
  struct completion work = COMPLETION_INITIALIZER(work)

static __inline__ void init_completion(struct completion *x)
{
  x->done = 0;
}

extern void wait_for_completion(struct completion *x);

static __inline__ void complete(struct completion *x)
{
  x->done++;
}

#define INIT_COMPLETION(x)  ((x).done = 0)

#endif
