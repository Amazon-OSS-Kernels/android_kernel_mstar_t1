/*
 * linux/kernel/irq/handle.c
 *
 * Copyright (C) 1992, 1998-2006 Linus Torvalds, Ingo Molnar
 * Copyright (C) 2005-2006, Thomas Gleixner, Russell King
 *
 * This file contains the core interrupt handling code.
 *
 * Detailed information is available in Documentation/DocBook/genericirq
 *
 */

#include <linux/irq.h>
#include <linux/random.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/kernel_stat.h>

#include <trace/events/irq.h>

#include "internals.h"

/**
 * handle_bad_irq - handle spurious and unhandled irqs
 * @irq:       the interrupt number
 * @desc:      description of the interrupt
 *
 * Handles spurious and unhandled IRQ's. It also prints a debugmessage.
 */
void handle_bad_irq(unsigned int irq, struct irq_desc *desc)
{
	print_irq_desc(irq, desc);
	kstat_incr_irqs_this_cpu(irq, desc);
	ack_bad_irq(irq);
}

/*
 * Special, empty irq handler:
 */
irqreturn_t no_action(int cpl, void *dev_id)
{
	return IRQ_NONE;
}

static void warn_no_thread(unsigned int irq, struct irqaction *action)
{
	if (test_and_set_bit(IRQTF_WARNED, &action->thread_flags))
		return;

	printk(KERN_WARNING "IRQ %d device %s returned IRQ_WAKE_THREAD "
	       "but no thread function available.", irq, action->name);
}

static void irq_wake_thread(struct irq_desc *desc, struct irqaction *action)
{
	/*
	 * In case the thread crashed and was killed we just pretend that
	 * we handled the interrupt. The hardirq handler has disabled the
	 * device interrupt, so no irq storm is lurking.
	 */
	if (action->thread->flags & PF_EXITING)
		return;

	/*
	 * Wake up the handler thread for this action. If the
	 * RUNTHREAD bit is already set, nothing to do.
	 */
	if (test_and_set_bit(IRQTF_RUNTHREAD, &action->thread_flags))
		return;

	/*
	 * It's safe to OR the mask lockless here. We have only two
	 * places which write to threads_oneshot: This code and the
	 * irq thread.
	 *
	 * This code is the hard irq context and can never run on two
	 * cpus in parallel. If it ever does we have more serious
	 * problems than this bitmask.
	 *
	 * The irq threads of this irq which clear their "running" bit
	 * in threads_oneshot are serialized via desc->lock against
	 * each other and they are serialized against this code by
	 * IRQS_INPROGRESS.
	 *
	 * Hard irq handler:
	 *
	 *	spin_lock(desc->lock);
	 *	desc->state |= IRQS_INPROGRESS;
	 *	spin_unlock(desc->lock);
	 *	set_bit(IRQTF_RUNTHREAD, &action->thread_flags);
	 *	desc->threads_oneshot |= mask;
	 *	spin_lock(desc->lock);
	 *	desc->state &= ~IRQS_INPROGRESS;
	 *	spin_unlock(desc->lock);
	 *
	 * irq thread:
	 *
	 * again:
	 *	spin_lock(desc->lock);
	 *	if (desc->state & IRQS_INPROGRESS) {
	 *		spin_unlock(desc->lock);
	 *		while(desc->state & IRQS_INPROGRESS)
	 *			cpu_relax();
	 *		goto again;
	 *	}
	 *	if (!test_bit(IRQTF_RUNTHREAD, &action->thread_flags))
	 *		desc->threads_oneshot &= ~mask;
	 *	spin_unlock(desc->lock);
	 *
	 * So either the thread waits for us to clear IRQS_INPROGRESS
	 * or we are waiting in the flow handler for desc->lock to be
	 * released before we reach this point. The thread also checks
	 * IRQTF_RUNTHREAD under desc->lock. If set it leaves
	 * threads_oneshot untouched and runs the thread another time.
	 */
	desc->threads_oneshot |= action->thread_mask;

	/*
	 * We increment the threads_active counter in case we wake up
	 * the irq thread. The irq thread decrements the counter when
	 * it returns from the handler or in the exit path and wakes
	 * up waiters which are stuck in synchronize_irq() when the
	 * active count becomes zero. synchronize_irq() is serialized
	 * against this code (hard irq handler) via IRQS_INPROGRESS
	 * like the finalize_oneshot() code. See comment above.
	 */
	atomic_inc(&desc->threads_active);

	wake_up_process(action->thread);
}

#ifdef CONFIG_MP_INTR_ERROR_CHECK_NON_STOP
#include <chip_int.h>

static DEFINE_PER_CPU(unsigned long, intr_cnt_init);
static DEFINE_PER_CPU(unsigned long, intr_abnormal_flag);
static DEFINE_PER_CPU(unsigned long, intr_cnt);
static DEFINE_PER_CPU(struct timeval, time_val);

#define MAX_IRQNUM NR_IRQS
#define QUERY_CNT 50000
#define QUERY_SECOND 1
static DEFINE_SPINLOCK(intr_rec_lock);
static unsigned intr_rec[MAX_IRQNUM] = {0};

/*
 *   irq_abnormal_check is to detect abnormal interrupt activity,
 *   use reference count to keep track of every incoming signal.
 *   We record a specific number(QUERY_CNT) of interrupt and check
 *   whether the period(QUERY_SECOND) is reasonable or not.
 *
 *   Input@desc: the descriptor of incoming interrupt
 *   Input@type: 0 for noraml interrupt source (timer/hw interrupt)
 *               1 for IPI message
 *   Input@val:  the number for IPI messag if type is IPI message(1)
 *
 */
void irq_abnormal_check(struct irq_desc *desc, unsigned int type, unsigned int val)
{
	unsigned int cpu_id = smp_processor_id();
	struct timeval new_time;

	if (per_cpu(intr_abnormal_flag, cpu_id)) {
		raw_spin_lock(&intr_rec_lock);
		if (type)
			intr_rec[val]++;
		else
			intr_rec[desc->irq_data.irq]++;
		raw_spin_unlock(&intr_rec_lock);
	}

	if (per_cpu(intr_cnt, cpu_id)++ >= QUERY_CNT) {
		do_gettimeofday(&new_time);

		if (per_cpu(intr_cnt_init, cpu_id)) {
			if (new_time.tv_sec - per_cpu(time_val, cpu_id).tv_sec < QUERY_SECOND) {
				if (per_cpu(intr_abnormal_flag, cpu_id) == 0) {
					per_cpu(intr_abnormal_flag, cpu_id) = 1;
				} else {
					unsigned int i, irq_the_most = 0;
					raw_spin_lock(&intr_rec_lock);
					for (i = 1; i < MAX_IRQNUM; i++)
						if (intr_rec[i] > intr_rec[irq_the_most])
							irq_the_most = i;
					raw_spin_unlock(&intr_rec_lock);

					printk(KERN_WARNING "==================================================================\n");
					printk(KERN_WARNING "==================================================================\n");
					printk(KERN_WARNING "Abnormal interrupt behavior detected!!! over %d interrupts within %d sec. cpu = %d\n", QUERY_CNT, QUERY_SECOND, cpu_id);

					if (irq_the_most >= MSTAR_FIQ_BASE) {
						printk(KERN_WARNING "Try to disable irq %d to ease the system\n", irq_the_most);
						irq_disable(irq_to_desc(irq_the_most));
					}

					printk(KERN_WARNING "==================================================================\n");
					printk(KERN_WARNING "==================================================================\n");

					per_cpu(intr_abnormal_flag, cpu_id) = 0;
				}
			}
		} else {
			per_cpu(intr_cnt_init, cpu_id) = 1;
			per_cpu(intr_abnormal_flag, cpu_id) = 0;
		}

		per_cpu(intr_cnt, cpu_id) = 0;
		per_cpu(time_val, cpu_id).tv_sec = new_time.tv_sec;
	}
}
#endif

irqreturn_t
handle_irq_event_percpu(struct irq_desc *desc, struct irqaction *action)
{
	irqreturn_t retval = IRQ_NONE;
	unsigned int flags = 0, irq = desc->irq_data.irq;

	do {
		irqreturn_t res;

		trace_irq_handler_entry(irq, action);
		res = action->handler(irq, action->dev_id);
		trace_irq_handler_exit(irq, action, res);

		if (WARN_ONCE(!irqs_disabled(),"irq %u handler %pF enabled interrupts\n",
			      irq, action->handler))
			local_irq_disable();

		switch (res) {
		case IRQ_WAKE_THREAD:
			/*
			 * Catch drivers which return WAKE_THREAD but
			 * did not set up a thread function
			 */
			if (unlikely(!action->thread_fn)) {
				warn_no_thread(irq, action);
				break;
			}

			irq_wake_thread(desc, action);

			/* Fall through to add to randomness */
		case IRQ_HANDLED:
			flags |= action->flags;
			break;

		default:
			break;
		}

		retval |= res;
		action = action->next;
	} while (action);

	add_interrupt_randomness(irq, flags);

	if (!noirqdebug)
		note_interrupt(irq, desc, retval);

#ifdef CONFIG_MP_INTR_ERROR_CHECK_NON_STOP
	irq_abnormal_check(desc, 0, 0);
#endif

	return retval;
}

irqreturn_t handle_irq_event(struct irq_desc *desc)
{
	struct irqaction *action = desc->action;
	irqreturn_t ret;

	desc->istate &= ~IRQS_PENDING;
	irqd_set(&desc->irq_data, IRQD_IRQ_INPROGRESS);
	raw_spin_unlock(&desc->lock);

	ret = handle_irq_event_percpu(desc, action);

	raw_spin_lock(&desc->lock);
	irqd_clear(&desc->irq_data, IRQD_IRQ_INPROGRESS);
	return ret;
}
