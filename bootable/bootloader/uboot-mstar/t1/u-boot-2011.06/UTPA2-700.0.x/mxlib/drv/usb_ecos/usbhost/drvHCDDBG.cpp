/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//
//
//
//
//
//
//
//
//
//******************************************************************************

#include "MsCommon.h"
#include "include/drvConfig.h"
#include "include/drvCompiler.h"
#include "include/drvPorts.h"
#include "include/drvIO.h"
#include "include/drvPCIMEM.h"
#include "include/drvList.h"
#include "include/drvTimer.h"
#include "include/drvKernel.h"
// USB related implemented header files
#include "include/drvUSB.h"
#include "drvHCD.h"
#include "include/drvCPE_EHCI.h"

#include "drvEHCI.h"

#ifdef EHCI_VERBOSE_DEBUG

static __inline__ char * edstring(U32 ed_type,char *tmp)
{
  switch (ed_type) {
  case PIPE_CONTROL:
        strcpy(tmp,"ctrl");
        break;
  case PIPE_BULK:
        strcpy(tmp, "bulk");
        break;
  case PIPE_INTERRUPT:
        strcpy(tmp,"intr");
        break;
  default:
        strcpy(tmp,"isoc");
        break;
  };
  return tmp;
}

#define pipestring(pipe, tmp_str) edstring(usb_pipetype(pipe),tmp_str)

/* debug| print the main components of an URB
 * small: 0) header + data packets 1) just header
 */
static void urb_print (struct urb * urb, char * str, int small)
{
  U32 pipe= urb->pipe;
  char tmp_str[10];

  if (!urb->dev || !urb->dev->bus) {
    dbg("%s URB: no dev", str);
    return;
  }

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  dbg("[urb_print]%s %p dev=%d ep=%d%s-%s flags=%x",
        str,
        urb,
        usb_pipedevice (pipe),
        usb_pipeendpoint (pipe),
        usb_pipeout (pipe)? "out" : "in",
        pipestring (pipe,tmp_str),
        urb->transfer_flags);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  dbg("[urb_print] len=%d/%d stat=%d",
        urb->actual_length,
        urb->transfer_buffer_length,
        urb->status);

  /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
  if (!small) {
    int i, len;

    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    if (usb_pipecontrol (pipe)) {
      printk (KERN_DEBUG __func__ ": setup(8):");
      for (i = 0; i < 8 ; i++)
        printk (" %02x", ((unsigned char *) urb->setup_packet) [i]);
      printk ("\n");
    }
    /*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/
    if (urb->transfer_buffer_length > 0 && urb->transfer_buffer) {
      printk (KERN_DEBUG __func__ ": data(%d/%d):",
        urb->actual_length,
        urb->transfer_buffer_length);
      len = usb_pipeout (pipe)?
            urb->transfer_buffer_length: urb->actual_length;
      for (i = 0; i < 16 && i < len; i++)
        printk (" %02x", ((unsigned char *) urb->transfer_buffer) [i]);
      printk ("%s stat:%d\n", i < len? "...": "", urb->status);
    }
  }
}

#endif

