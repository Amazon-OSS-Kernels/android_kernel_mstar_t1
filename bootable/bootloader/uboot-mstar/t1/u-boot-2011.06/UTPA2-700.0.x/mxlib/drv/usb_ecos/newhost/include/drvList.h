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
#ifndef _DRVLIST_H
#define _DRVLIST_H

#include "MsTypes.h"

/* list structure definition */
struct list_head {
    struct list_head *next;
    struct list_head *prev;
};

#define DECLARE_LIST(mylist)  \
      struct list_head mylist = { &(mylist), &(mylist) }

/**
     * @brief             Initial list structure data member
     *
     * @param pInitList   pointer of the list to be initialized.
     *
     * @return            None
     */
static __inline__ void ms_list_init (struct list_head *pInitList)
{
	pInitList->next = pInitList;
	pInitList->prev = pInitList;
}

/**
     * @brief                           Insert list after the specified list
     *
     * @param pNew                New list to be added.
     *
     * @param pInsert_after_it  Insert list after this list.
     *
     * @return            None
     */
static __inline__ void ms_insert_list_after( struct list_head *pNew, struct list_head *pInsert_after_it )
{
    struct list_head *next;
    next = pInsert_after_it->next;
    pInsert_after_it->next = pNew;
    pNew->next = next;
    pNew->prev = pInsert_after_it;
    next->prev = pNew;
}

/**
     * @brief              check if  is empty list
     *
     * @param pList    list to be checked.
     *
     * @return            boolean
     */
static __inline__ int ms_is_empty_list( struct list_head *pList )
{
    if (pList == pList->next)
        return TRUE;
    else
        return FALSE;
}

/**
     * @brief                              Insert list before the specified list
     *
     * @param pNew                   New list to be added.
     *
     * @param pInsert_before_it   Insert list before this list.
     *
     * @return            None
     */
static __inline__ void ms_insert_list_before( struct list_head *pNew, struct list_head *pInsert_before_it )
{
    struct list_head *prev;
    prev = pInsert_before_it->prev;
    pNew->prev = prev;
    pNew->next = pInsert_before_it;
    pInsert_before_it->prev = pNew;
    prev->next = pNew;
}

/**
     * @brief                        Remove current entry from list
     *
     * @param pNew             Current entry to be removed.
     *
     * @return                      None
     */
static __inline__ void ms_list_remove( struct list_head *pList )
{
    pList->prev->next = pList->next;
    pList->next->prev = pList->prev;
}

/**
     * @brief                        Remove current entry from list and initial the entry
     *
     * @param pEntry            Current entry to be removed.
     *
     * @return                      None
     */
static __inline__ void ms_list_remove_and_init(struct list_head *pEntry)
{
    ms_list_remove(pEntry);
    ms_list_init(pEntry);
}

/**
     * @brief                       Join lists.
     *
     * @param pList             The list to be added.
     *
     * @param pHead           The place
     *
     * @return                     None
     */
static __inline__ void ms_list_join(struct list_head *pList, struct list_head *pHead)
{
  struct list_head *pFirst = pList->next;
  struct list_head *pLast = pList->prev;
  struct list_head *pNext = pHead->next;

  pFirst->prev = pHead;
  pHead->next = pFirst;

  pLast->next = pNext;
  pNext->prev = pLast;
}

/**
     * @brief                       cast a member item to the container structure.
     *
     * @param _item_          member item pointer.
     *
     * @param _struct_        The type definition of the container structure
     *
     * @param _member_     The name of the _item_ in the structure _struct_
     *
     * @return                     the container structure pointer
     */
#define entry_to_container( _item_, _struct_, _member_ ) \
    ((_struct_ *)((char *)(_item_)-(char *)(offsetof(_struct_,_member_))))

#define list_for_loop( _item_, _list_ )   \
    for ( (_item_)=(_list_)->next; (_item_)!=(_list_); (_item_)=(_item_)->next )

#define list_for_each_entry(_list_, _head_, _item_)                     \
for ((_list_) = entry_to_container((_head_)->next, typeof(*_list_), _item_); \
     &((_list_)->_item_) != (_head_);                                 \
     (_list_) = entry_to_container((_list_)->_item_.next, typeof(*_list_), _item_))

#define list_for_loop_ex(_item_, _temp_, _list_) \
  for (_item_=(_list_)->next, _temp_=_item_->next; _item_!=(_list_); _item_=_temp_, _temp_=_item_->next)

#if 0
#define list_for_each_prev(_item_, _list_) \
    or (_item_ = (_list_)->prev; _item_ != (_list_); _item_ = _item_->prev)
#endif

#define INIT_LIST_HEAD( _list_ )   ms_list_init( _list_)
#define list_add( _newlist_, _list_add_after_ )  ms_insert_list_after( _newlist_, _list_add_after_ )
#define list_empty( _list_ )  ms_is_empty_list( _list_)
#define list_add_tail( _newlist_, _list_add_before_ )  ms_insert_list_before( _newlist_, _list_add_before_ )
#define list_del( _list_ )   ms_list_remove( _list_)
#define list_del_init( _list_ )   ms_list_remove_and_init( _list_)
#define __list_splice( _list_, _head_ )   ms_list_join( _list_, _head_ )
#define list_entry( _list_, _struct_, _member_ ) entry_to_container( _list_, _struct_, _member_ )
#define list_for_each_safe(_item_, _temp_, _list_)  list_for_loop_ex(_item_, _temp_, _list_)
#define list_for_each( _item_, _list_ )   list_for_loop( _item_, _list_ )

#endif


