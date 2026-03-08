/* this file collect debug library about thread */

void MDBShowAllThread()
{
	pr_debug("Thread back trace dump!!!\n");
    {
        struct task_struct *g, *p;

	pr_debug("Threads unsleeping...\n");
        do_each_thread(g, p) {
            if(p->state == TASK_INTERRUPTIBLE)
            	continue;
		pr_debug("< pid = %d , tgid = %d\n", p->pid, p->tgid);
            sched_show_task(p);
		pr_debug(">\n");
        } while_each_thread(g, p);

	pr_debug("\n\nThreads ALL...\n");
        do_each_thread(g, p) {
		pr_debug("< pid = %d , tgid = %d\n", p->pid, p->tgid);
            sched_show_task(p);
		pr_debug(">\n");
        } while_each_thread(g, p);
    }
}
