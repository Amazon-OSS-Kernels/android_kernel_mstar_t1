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

#ifndef CYGONCE_FS_JFFS2_LIST_H
#define CYGONCE_FS_JFFS2_LIST_H


/* -----------------------------------------------------------------------*/

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 

#include <stddef.h>
#include <cyg/infra/cyg_type.h>

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 

struct list_head {
    struct list_head *next;
    struct list_head *prev;
};

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
        struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD( _list_ )              \
CYG_MACRO_START                               \
(_list_)->next = (_list_)->prev = (_list_);   \
CYG_MACRO_END

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
 
static __inline__ void
list_add( struct list_head *newent, struct list_head *afterthisent )
{
    struct list_head *next = afterthisent->next;
    newent->next = next;
    newent->prev = afterthisent;
    afterthisent->next = newent;
    next->prev = newent;
} 

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
static __inline__ void
list_add_tail( struct list_head *newent, struct list_head *beforethisent )
{
    struct list_head *prev = beforethisent->prev;
    newent->prev = prev;
    newent->next = beforethisent;
    beforethisent->prev = newent;
    prev->next = newent;
}

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
static __inline__ void
list_del( struct list_head *ent )
{
    ent->prev->next = ent->next;
    ent->next->prev = ent->prev;
} 

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 

static __inline__ int
list_empty( struct list_head *list )
{
    return ( list->next == list );
} 

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 

#define list_entry( _list_, _type_, _member_ ) \
    ((_type_ *)((char *)(_list_)-(char *)(offsetof(_type_,_member_))))

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 

#define list_for_each( _ent_, _list_ )   \
    for ( (_ent_) = (_list_)->next;      \
    (_ent_) != (_list_);                 \
    (_ent_) = (_ent_)->next )

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
#define list_for_each_entry(_list_, _head_, _item_)                     \
for ((_list_) = list_entry((_head_)->next, typeof(*_list_), _item_); \
     &((_list_)->_item_) != (_head_);                                 \
     (_list_) = list_entry((_list_)->_item_.next, typeof(*_list_), _item_))

/*USB HOST USB HOST USB HOST USB HOST USB HOST USB HOST*/ 
#endif 

