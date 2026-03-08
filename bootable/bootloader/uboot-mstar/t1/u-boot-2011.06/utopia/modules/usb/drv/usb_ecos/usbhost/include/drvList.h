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
#ifndef _LINUX_LIST_H
#define _LINUX_LIST_H

#include "MsTypes.h"
#include <stddef.h>
#include "linux/drvList.h"

#define LIST_POISON1  ((void *) 0x00100100)
#define LIST_POISON2  ((void *) 0x00200200)

static __inline__ void list_del_init(struct list_head *entry)
{
  list_del(entry);
  INIT_LIST_HEAD(entry); 
}

static __inline__ void __list_splice(struct list_head *list,
         struct list_head *head)
{
  struct list_head *first = list->next;
  struct list_head *last = list->prev;
  struct list_head *at = head->next;

  first->prev = head;
  head->next = first;

  last->next = at;
  at->prev = last;
}

static __inline__ void list_splice(struct list_head *list, struct list_head *head)
{
  if (!list_empty(list))
    __list_splice(list, head);
}

static __inline__ void list_splice_init(struct list_head *list,
            struct list_head *head)
{
  if (!list_empty(list)) {
    __list_splice(list, head);
    INIT_LIST_HEAD(list);
  }
}

#define get_list_entry(ptr, type, member, member_type, result) \
{ \
        const member_type *__mptr = (ptr);  \
        result = (type *)( (char *)__mptr - offsetof(type,member) ); \
}

#define list_for_each_prev(pos, head) \
  for (pos = (head)->prev, prefetch(pos->prev); pos != (head); \
          pos = pos->prev, prefetch(pos->prev))
          
#define list_for_each_safe(pos, n, head) \
  for (pos = (head)->next, n = pos->next; pos != (head); \
    pos = n, n = pos->next)


#endif


