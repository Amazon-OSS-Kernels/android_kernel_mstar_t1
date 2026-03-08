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
#ifndef _PTHREAD_MPA_H_
#define _PTHREAD_MPA_H_

#if defined(_WIN32) || defined(_MFE_T8_)
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#define pthread_cond_init(a, b)  pthread_cond_init(a, NULL)
#else //if defined(MIPSDEV)
#include <cyg/kernel/kapi.h>
#include <cyg/infra/diag.h>
#include <cyg/hal/plf_intr.h>

#define pthread_condattr_t
#define pthread_mutex_t          cyg_mutex_t
#define pthread_t                cyg_thread
#define pthread_cond_t           cyg_cond_t
#define pthread_mutex_init(a, b) cyg_mutex_init(a)
#define pthread_mutex_lock       cyg_mutex_lock
#define pthread_mutex_unlock     cyg_mutex_unlock
#define pthread_mutex_destroy    cyg_mutex_destroy
#define pthread_cond_init        cyg_cond_init
#define pthread_cond_signal      cyg_cond_signal
#define pthread_cond_wait(a, b)  cyg_cond_wait(a)
#define pthread_cond_destroy     cyg_cond_destroy
#define sem_init(a, b, c)        cyg_semaphore_init(a, c)
#define sem_post                 cyg_semaphore_post
#define sem_wait                 cyg_semaphore_wait
#define sem_destroy              cyg_semaphore_destroy
#define sem_t                    cyg_sem_t
//#define PARSER_THREAD_STACK_SIZE (30*1024)
//extern uchar   parser_thread_stack[PARSER_THREAD_STACK_SIZE];
#endif

#endif // HCOMMON_H

