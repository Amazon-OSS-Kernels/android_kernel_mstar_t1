#include <linux/fs.h>
#include <linux/hugetlb.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/mmzone.h>
#include <linux/proc_fs.h>
#include <linux/quicklist.h>
#include <linux/seq_file.h>
#include <linux/swap.h>
#include <linux/vmstat.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/ctype.h>

#include <asm/setup.h>
#include <asm/cacheflush.h>
#include <linux/delay.h>

#include <include/mstar/mstar_chip.h>
#include <linux/spinlock.h>

#include <linux/module.h>
#include <linux/cpufreq.h>
#include <linux/smp.h>
#include <linux/io.h>
#include <linux/timer.h>
#include <linux/vmalloc.h>
#include <mstar/mpatch_macro.h>
#include <trace/events/sched.h>

#if (MP_PLATFORM_T_SENSOR_OBSERVATION == 1)
/*patch for add_timer issue(york)*/
#include <linux/kthread.h>
/*patch for add_timer issue*/
#endif

#include "./include/mach/hardware.h"
#include "./include/mach/platform.h"

#include <chip_dvfs_calibrating.h>
#include <linux/version.h>

#ifdef CONFIG_MSTAR_DVFS
#ifndef __MDRV_DVFS_H__
#include "mdrv_dvfs.h"
#include <mach/io.h>
#endif
#endif

#define DVFS_DEBUG KERN_DEBUG

#define MAX_DMSG_WRITE_BUFFER	64
#define CM_ID  	IO_ADDRESS(INTEGRATOR_HDR_ID)
#define CM_OSC	IO_ADDRESS(INTEGRATOR_HDR_OSC)
#define CM_STAT IO_ADDRESS(INTEGRATOR_HDR_STAT)
#define CM_LOCK IO_ADDRESS(INTEGRATOR_HDR_LOCK)
#define FREQ_CHECK_LINK_TIME (HZ)
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,23)
#define cpufreq_notify_transition(A, B, C)	cpufreq_notify_transition(A, B, C)
#endif	// 3.10.86 pass 3 parameters

bool forcibly_set_target_flag[CONFIG_NR_CPUS] = {0};
extern void change_interval(unsigned int old_freq, unsigned int new_freq);
extern unsigned int query_frequency(unsigned int cpu_id);
extern struct cpufreq_policy *cpufreq_cpu_data;
#ifndef CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE
extern void mstar_update_sched_clock(void);
extern unsigned int SC_MULT;
extern unsigned int SC_SHIFT;
#endif // CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE

/* str usage */
atomic_t ac_str_cpufreq = ATOMIC_INIT(0);
atomic_t disable_dvfs = ATOMIC_INIT(0);

#if (MP_PLATFORM_T_SENSOR_OBSERVATION == 1)
/*patch for add_timer issue(york)*/
static struct task_struct *t_sensor_tsk;
static void t_sensor_thread(void* arg);
/*patch for add_timer issue*/
#endif

/* proc usage */
//static DEFINE_SPINLOCK(set_freq_lock);
static atomic_t proc_is_open = ATOMIC_INIT(0);
static struct cpufreq_driver integrator_driver;
unsigned int current_frequency = 0;
atomic_t echo_calibrating_freq = ATOMIC_INIT(0);
unsigned int register_frequency = 0;
static int ORI_FREQ_KHZ;


/* on_demand handshake usage */
static atomic_t on_demand_handshake_is_open = ATOMIC_INIT(0);
DECLARE_WAIT_QUEUE_HEAD(DVFS_on_demand_event_waitqueue);
DECLARE_WAIT_QUEUE_HEAD(DVFS_on_demand_event_waitqueue_userspace_return);
EXPORT_SYMBOL(DVFS_on_demand_event_waitqueue);
EXPORT_SYMBOL(DVFS_on_demand_event_waitqueue_userspace_return);
DEFINE_SEMAPHORE(DVFS_on_demand_event_SEM);
DEFINE_SEMAPHORE(DVFS_disable_SEM);

/* this is for muji test cmdq (we don not use SAR to adjust voltage) */
#if defined(CONFIG_MSTAR_IIC) && defined(CONFIG_MSTAR_DVFS_KERNEL_IIC)
unsigned int start_userspace_ondemand_handshake = 1;
#else
unsigned int start_userspace_ondemand_handshake = 0;
#endif
unsigned int ready_to_change_cpufreq = 0;
unsigned int ready_to_change_voltage = 0;
unsigned int ready_to_change_voltage_type = 0;
unsigned int change_cnt = 0;
unsigned int finished_change_cnt = 0;
int voltage_change_result = 0;

unsigned int bootarg_dvfs_disable = 0;
#define mstar_debug_init_value 0
#define mstar_info_init_value 0
static unsigned int mstar_debug = mstar_debug_init_value;
static unsigned int mstar_info = mstar_info_init_value;

/* callback function usage */
static struct cpufreq_policy *ondemand_timer[CONFIG_NR_CPUS];
#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND
static struct timer_list Check_T_sensor_timer;
static void Mdrv_CPU_T_sensor_Check_callback(unsigned long value);
#else
static struct timer_list Check_Freq_timer;
static void Mdrv_CPU_Freq_Check_callback(unsigned long value);
#endif

static struct cpufreq_frequency_table mstar_freq_table[] = {
	{0x01, 0},
	{0x02, 0},
	{0, CPUFREQ_TABLE_END},
};
int get_freq(unsigned int cpu)
{
	return query_frequency(cpu) * 1000;
}

/*
 * Validate the speed policy.
 */
static int integrator_verify_policy(struct cpufreq_policy *policy)
{
	cpufreq_verify_within_limits(policy, policy->cpuinfo.min_freq, policy->cpuinfo.max_freq);

	return 0;
}

static unsigned int integrator_get(unsigned int cpu)
{
	cpumask_t cpus_allowed;
	unsigned int current_freq;

	//u_int cm_osc;
	//struct icst_vco vco;

	cpus_allowed = current->cpus_allowed;
	set_cpus_allowed(current, cpumask_of_cpu(cpu));
	BUG_ON(cpu != smp_processor_id());

	current_freq = get_freq(0); //Always get frequency from CPU0

	set_cpus_allowed(current, cpus_allowed);

	return current_freq;
}
#if (MP_PLATFORM_T_SENSOR_OBSERVATION == 1)
static void t_sensor_thread(void* arg){
	while(1){
		msleep(1000);
		Mdrv_CPU_T_sensor_Check_callback(NULL);
	}
}
#endif
static int integrator_cpufreq_init(struct cpufreq_policy *policy)
{
#if (MP_PLATFORM_T_SENSOR_OBSERVATION == 1)
	/*patch for add_timer issue(york)*/
	int ret;
	 /*patch for add_timer issue*/
#endif

	/* set default policy and cpuinfo */
	/* set default policy and cpuinfo */
	policy->cpuinfo.max_freq = policy->max = mstar_freq_table[1].frequency = MAX_CPU_FREQ;
	policy->cpuinfo.min_freq = policy->min = mstar_freq_table[0].frequency = MIN_CPU_FREQ;
    cpufreq_frequency_table_get_attr(mstar_freq_table, policy->cpu);
	policy->cpuinfo.transition_latency = TRANSITION_LATENCY; 	/* 1 ms, assumed */
	policy->cur = integrator_get(policy->cpu);

	ondemand_timer[policy->cpu]	= policy;
	pr_debug("\033[35mFunction = %s, Line = %d, policy->cpu=%d, integrator_get return %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, policy->cpu, policy->cur);

	/* move to here to let the timer works on cpu_0 */
	if(get_cpu() == 0)
	{
		put_cpu();
		atomic_set(&ac_str_cpufreq, policy->cur);
		pr_debug("\033[31mFunction = %s, Line = %d, policy->cpu %d, doing Mdrv_CPU_T_sensor_Check_callback, set ac_str_cpufreq to %dkHz\033[m\n", __PRETTY_FUNCTION__, __LINE__, policy->cpu, atomic_read(&ac_str_cpufreq));
#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND
#if (MP_PLATFORM_T_SENSOR_OBSERVATION == 0)
    	init_timer(&Check_T_sensor_timer);
    	Check_T_sensor_timer.data = 1;
    	Check_T_sensor_timer.function = Mdrv_CPU_T_sensor_Check_callback;
    	Check_T_sensor_timer.expires = jiffies + FREQ_CHECK_LINK_TIME;  // 1 second do once
    	add_timer(&Check_T_sensor_timer);
#else
	/*patch for add_timer issue(york)*/
	t_sensor_tsk = kthread_create(t_sensor_thread, NULL, "T_sensor_Check");
	kthread_bind(t_sensor_tsk, 0);
	if (IS_ERR(t_sensor_tsk)) {
		printk("create kthread for t_sensor temperature observation fail\n");
        	ret = PTR_ERR(t_sensor_tsk);
	        t_sensor_tsk = NULL;
        	goto out;
	}else
		wake_up_process(t_sensor_tsk);
	 /*patch for add_timer issue*/
#endif

#endif // CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND
	}
	else
	{
		put_cpu();
	}

	return 0;
#if (MP_PLATFORM_T_SENSOR_OBSERVATION == 1)
out:
    return ret;
#endif
}

typedef struct _IO_CPU_calibrating_INFO
{
	char* MESSAGE_BUFF;
	char CPUID;
	char  MESSAGE_LEN;
	int MID;
}IO_CPU_calibrating_INFO;

static int CPU_calibrating_proc_ioctl(struct file *filp, unsigned int cmd, IO_CPU_calibrating_INFO* message_buf)
{
	return 0;
}

int _CPU_calibrating_proc_write(const unsigned long, const unsigned long, int);
int __CPU_calibrating_proc_write(const unsigned long);
void write_cpufreq_to_RIU(const unsigned long);
static unsigned long read_echo_calibrating_freq(void);
static void write_echo_calibrating_freq(const unsigned long);

static unsigned long jiffies_boost_lasttime = 0;

static ssize_t CPU_calibrating_proc_write(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[MAX_DMSG_WRITE_BUFFER];
	long idx;
	unsigned long cpu_freq_in_khz = 0;
	unsigned long client_id = 0;

	if (!count)
		return count;

	if (count >= MAX_DMSG_WRITE_BUFFER)
		count = MAX_DMSG_WRITE_BUFFER - 1;

	/*
	 * Prevent Tainted Scalar Warning:
	 * Buffer can't be tainted because:
	 * 1. The count never exceeds MAX_DMSG_WRITE_BUFFER i.e. buffer size.
	 * 2. copy_from_user returns 0 in case of correct copy.
	 *So, we don't need to sanitize buffer.
	 *
	 */
	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	buffer[count] = '\0';

	if (buffer[0] == '/')
		idx = 3;
	else if (strict_strtol(buffer, 0, &idx) != 0)  //Force change str to decimal conversion (str, base, *converted_num), base is hex, decimal, or ...
		return -EINVAL;

	// idx[29:22] is the client id
	client_id = (idx >> 22) & 0xff;

	// idx[21:0] is the cpu frequency in Khz
	cpu_freq_in_khz = idx & 0x3FFFFF;

    //It sets CPU0 frequency for all CPUs
	_CPU_calibrating_proc_write(client_id, cpu_freq_in_khz, 0);

	return count;
}

DEFINE_MUTEX(mstar_cpufreq_lock);
static DEFINE_MUTEX(boost_client_mutex);
static LIST_HEAD(boost_client_head);

struct boost_client
{
	unsigned long client_id;
	unsigned long cpu_freq_in_khz;
	struct list_head list_head;
};

static struct boost_client *show_boost_client(void)
{
	struct boost_client *bc = NULL;
	mutex_lock(&boost_client_mutex);
	if (list_empty(&boost_client_head)) {
		/* do nothing */
	} else {
/*
		pr_debug(" -->[boost client] ");
*/
		list_for_each_entry(bc, &boost_client_head, list_head) {
/*
		pr_debug("(client id:%u %ukhz) ", bc->client_id, bc->cpu_freq_in_khz);
*/
	}
/*
		pr_debug("End\n");
*/
	}
	mutex_unlock(&boost_client_mutex);

	return bc;
}

static bool is_any_boost_client_running(void)
{
	bool i;

	mutex_lock(&boost_client_mutex);
	i = !list_empty(&boost_client_head);
	mutex_unlock(&boost_client_mutex);

	return i;
}

static int add_boost_client(unsigned long client_id, unsigned long cpu_freq_in_khz, void *arg)
{
	struct boost_client *bc = NULL;

	if ((bc = kmalloc(sizeof(*bc), GFP_KERNEL)) == NULL)
		return -ENOMEM;

	bc->client_id = client_id;
	bc->cpu_freq_in_khz = cpu_freq_in_khz;

	mutex_lock(&boost_client_mutex);
	list_add(&bc->list_head, &boost_client_head);
	mutex_unlock(&boost_client_mutex);

	return 0;
}

static struct boost_client *find_boost_client(unsigned long client_id)
{
	struct boost_client *i = NULL;
	struct boost_client *bc = NULL;

	mutex_lock(&boost_client_mutex);
	list_for_each_entry(i, &boost_client_head, list_head) {
		if (i->client_id == client_id) {
			bc = i;
		}
	}
	mutex_unlock(&boost_client_mutex);

	return bc;
}

static int del_boost_client(struct boost_client *bc)
{
	// remove the client node
	BUG_ON(!bc);

	mutex_lock(&boost_client_mutex);
	list_del(&bc->list_head);
	mutex_unlock(&boost_client_mutex);

	kfree(bc);

	return 0;
}

static bool del_all_boost_client(void)
{
	struct boost_client *bc = NULL;
	struct boost_client *bc2 = NULL;

	mutex_lock(&boost_client_mutex);
	list_for_each_entry_safe(bc, bc2, &boost_client_head, list_head) {
		list_del(&bc->list_head);
		kfree(bc);
	}
	mutex_unlock(&boost_client_mutex);

	return true;
}

static bool is_boost_client_running(struct boost_client *bc)
{
	return (bc != NULL) ? true : false;
}

static struct boost_client *find_boost_client_with_highest_priority(void)
{
	struct boost_client *i = NULL;
	struct boost_client *bc = NULL;

	mutex_lock(&boost_client_mutex);
	bc = list_entry(boost_client_head.next, struct boost_client, list_head);
	list_for_each_entry(i, &boost_client_head, list_head) {
		if(i->client_id < bc->client_id) {
			bc = i;
		}
	}
	mutex_unlock(&boost_client_mutex);

	return bc;
}

static unsigned long read_echo_calibrating_freq(void)
{
	return (unsigned long)atomic_read(&echo_calibrating_freq);
}

static void write_echo_calibrating_freq(const unsigned long value)
{
	atomic_set(&echo_calibrating_freq, value);
}

int _CPU_calibrating_proc_write(
		const unsigned long client_id,
		const unsigned long cpu_freq_in_khz,
                int cpu_id)
{
    struct cpufreq_policy *policy;
	int i;
    bool ret = 0;
#if (defined CONFIG_MP_DVFS_FORCE_USE_ONE_FREQ) || (defined CONFIG_MP_DVFS_FORCE_PINGO_TEST)
	return 0;
#endif

    mutex_lock(&mstar_cpufreq_lock);
	if( (!start_userspace_ondemand_handshake) || (atomic_read(&disable_dvfs) == 1) )
	{
		pr_debug("\033[34mFunction = %s, doing _CPU_calibrating_proc_write, but start_userspace_ondemand_handshake is %d\033[m\n", __PRETTY_FUNCTION__, start_userspace_ondemand_handshake);
		pr_debug("\033[35mFunction = %s, [Return], client_id = %lu and cpu_freq_in_khz =%lu, disable_dvfs is %d\033[m\n", __PRETTY_FUNCTION__, client_id, cpu_freq_in_khz, atomic_read(&disable_dvfs));
		ret = -1;
        goto not_change_cpu_freq;
	}

	pr_debug("[dvfs_boost] here comes a client with client_id = %lu and cpu_freq_in_khz = %lu\n",
			client_id, cpu_freq_in_khz);

	for_each_online_cpu(i)
	{
		policy = cpufreq_cpu_get(i);

		if(!policy)
		{
			printk("\033[31mFunction = %s, Line = %d, cpu%d do: [cpu%d] policy is NULL\033[m\n",
						__PRETTY_FUNCTION__, __LINE__, get_cpu(), i);
			put_cpu();
			goto not_change_cpu_freq;
		}
		cpufreq_cpu_put(policy);
	}

	// extend the timer
	jiffies_boost_lasttime = jiffies;
	struct boost_client *bc=0;

	if (cpu_freq_in_khz == 0x5566) {
		goto not_change_cpu_freq;

	} else if (cpu_freq_in_khz == 0) {
                bc = find_boost_client(client_id);
		if (is_boost_client_running(bc) == true) {
			del_boost_client(bc);

			if (is_any_boost_client_running() == true) {
				bc = find_boost_client_with_highest_priority();
				write_echo_calibrating_freq(bc->cpu_freq_in_khz);
				goto change_cpu_freq;
			} else {
#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND
				goto not_change_cpu_freq;
#else
				write_echo_calibrating_freq(1008000);
				goto change_cpu_freq;
#endif
			}
		} else {
			printk(KERN_WARNING "[dvfs_boost] this client is not running\n");
		}
	} else {
                bc = find_boost_client(client_id);
		// check if a client with same client_id already exists
		if (is_boost_client_running(bc) == true) {
			goto not_change_cpu_freq;
		} else {
			// registered a boost client with client_id and cpu_freq_in_khz
			add_boost_client(client_id, cpu_freq_in_khz, NULL);
			// echo_calibrating_freq = the cpu_freq_in_khz of the client with highest priority
			bc = find_boost_client_with_highest_priority();
			write_echo_calibrating_freq(bc->cpu_freq_in_khz);
			goto change_cpu_freq;
		}
	}

	// something wrong happened
	ret = -1;
	goto not_change_cpu_freq;

change_cpu_freq:
	__CPU_calibrating_proc_write(read_echo_calibrating_freq());
not_change_cpu_freq:
    mutex_unlock(&mstar_cpufreq_lock);
	return ret;
}


int __CPU_calibrating_proc_write(const unsigned long idx)
{
	struct cpufreq_freqs freqs;
	struct cpufreq_policy *policy;
    int i;

	if( (!start_userspace_ondemand_handshake) || (atomic_read(&disable_dvfs) == 1) )
	{
		pr_debug("\033[34mFunction = %s, doing __CPU_calibrating_proc_write, but start_userspace_ondemand_handshake is %d\033[m\n", __PRETTY_FUNCTION__, start_userspace_ondemand_handshake);
		pr_debug("\033[35m[Return] Function = %s, force setting cpufreq to %u, disable_dvfs is %d\033[m\n", __PRETTY_FUNCTION__, idx, atomic_read(&disable_dvfs));
		return 0;
	}
    if(idx > CONFIG_DVFS_CPU_CLOCK_MAX(0)) // Always query CPU 0
  	{
        printk(KERN_WARNING "\033[36m\n freq %d KHZ > CONFIG_DVFS_CPU_CLOCK_MAX %d KHZ ,not allowed\033[m\n", (int)idx, CONFIG_DVFS_CPU_CLOCK_MAX(0));
  		return -1;
  	}
	write_cpufreq_to_RIU(idx);
	register_frequency = idx;

#if (!defined CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE) && (!defined CONFIG_MP_GLOBAL_TIMER_12MHZ_PATCH)
	mstar_update_sched_clock();
#endif // CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE

	for_each_online_cpu(i)
	{
		ondemand_timer[i]->max = idx;
	}

	change_cpus_timer(__FUNCTION__, idx, 0);

#if (!defined CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE) && (!defined CONFIG_MP_GLOBAL_TIMER_12MHZ_PATCH)
	if(policy->cur != idx)
	{
		change_interval(freqs.old, freqs.new);
	}
#endif

	current_frequency = idx;

	return 0;
}

void write_cpufreq_to_RIU(const unsigned long new_scaling_cur_freq)
{
	pr_debug("\033[31mFunction = %s, Line = %d, write cpu_freq to RIU: 0x%X\033[m\n",
		__PRETTY_FUNCTION__, __LINE__, (unsigned int)(new_scaling_cur_freq/1000));
	reg_writew((new_scaling_cur_freq/1000), 0x1F200A1C);
}

#ifdef CONFIG_MSTAR_DVFS
void MDrvDvfsVoltageSetup(unsigned int dwCpuClock, unsigned int dwVoltage, unsigned int dwVoltageType, unsigned int dwCpu);
void MDrvDvfsVoltageSetup(unsigned int dwCpuClock, unsigned int dwVoltage, unsigned int dwVoltageType, unsigned int dwCpu)
{
	//dwVoltage = 145; // set this for checking if voltage setting correct
    if(start_userspace_ondemand_handshake == 1)
    {
    	ready_to_change_cpufreq = dwCpuClock;
    	ready_to_change_voltage = dwVoltage;
    	ready_to_change_voltage_type = dwVoltageType;
    	change_cnt++;

	up(&DVFS_on_demand_event_SEM);
	if (mstar_debug) {
		/*
		pr_debug("\033[35m[INFO] Data Exchange Count to User Space: %d\033[m\n", change_cnt);
		pr_debug("\033[35m[INFO] Voltage: %d\033[m\n", dwVoltage);
		pr_debug("\033[35m[INFO] VoltageType: %d\033[m\n", dwVoltageType);
		*/
	}
	wait_event_interruptible_timeout(DVFS_on_demand_event_waitqueue_userspace_return,
		finished_change_cnt == change_cnt , MAX_SCHEDULE_TIMEOUT);
	pr_debug("\033[35m[INFO] Data Exchange Count from User Space: %d\033[m\n",
			finished_change_cnt);
    }
    else
	pr_debug("\033[35m[Return] Function = %s, start_userspace_ondemand_handshake is %d\033[m\n",
			__PRETTY_FUNCTION__, start_userspace_ondemand_handshake);
}
EXPORT_SYMBOL(MDrvDvfsVoltageSetup);
#endif

#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND
static int integrator_set_target_on_demand(struct cpufreq_policy *policy, unsigned int target_freq, unsigned int relation)
{
	int cpu = policy->cpu;
	cpumask_t cpus_allowed;

	/* only cpu0 is allowed to adjust cpufreq(due to no mutex for now) */
	if(get_cpu() != 0)
	{
		put_cpu();
		return 0;
	}
	put_cpu();

	mutex_lock(&mstar_cpufreq_lock);
	if( (!start_userspace_ondemand_handshake) || (atomic_read(&disable_dvfs) == 1) )
	{
	pr_debug("\033[34mFunction = %s, doing integrator_set_target_on_demand, but start_userspace_ondemand_handshake is %d\033[m\n", __PRETTY_FUNCTION__, start_userspace_ondemand_handshake);
	pr_debug("\033[35mFunction = %s, disable_dvfs is %d\033[m\n",
			__PRETTY_FUNCTION__, atomic_read(&disable_dvfs));

		goto set_target_out;
	}
	else
	{
		/*
		 * Save this threads cpus_allowed mask.
		 */
		pr_debug("\033[35mFunction = %s, Line = %d, set cpufreq from %dKHz to %dKHz\033[m\n",
			_PRETTY_FUNCTION__, __LINE__, policy->cur, target_freq);
		cpus_allowed = current->cpus_allowed;

		/*
		 * Bind to the specified CPU.  When this call returns,
		 * we should be running on the right CPU.
		 */
		set_cpus_allowed(current, cpumask_of_cpu(cpu));
		BUG_ON(cpu != smp_processor_id());

		if(target_freq > policy->max)
		{
			printk(KERN_WARNING "\033[31mFunction = %s, Line = %d, adjust target_freq from %dKHz to policy->max %dKHz (over_max case), forcibly_set_target_flag is %d\033[m\n",
					__PRETTY_FUNCTION__, __LINE__, target_freq, policy->max, forcibly_set_target_flag[cpu]);
			target_freq = policy->max;
		}

		if (is_any_boost_client_running() == true)
		{
			target_freq = policy->max;
		}

		/* update timer, cpu_freq(RIU), and scaling_cur_freq for all cpus */
		change_cpus_timer((char *)__FUNCTION__, target_freq, cpu);

		/*
		 * Restore the CPUs allowed mask.
		 */
		set_cpus_allowed(current, cpus_allowed);

	}

set_target_out:
	mutex_unlock(&mstar_cpufreq_lock);
	return 0;
}
#else
static int integrator_set_target(struct cpufreq_policy *policy,
				 unsigned int target_freq,
				 unsigned int relation)
{
	cpumask_t cpus_allowed;
	int cpu = policy->cpu;
	struct cpufreq_freqs freqs;

	/*
	 * Save this threads cpus_allowed mask.
	 */
	cpus_allowed = current->cpus_allowed;

	/*
	 * Bind to the specified CPU.  When this call returns,
	 * we should be running on the right CPU.
	 */
	set_cpus_allowed(current, cpumask_of_cpu(cpu));
	BUG_ON(cpu != smp_processor_id());

  	freqs.old = get_freq(0); //  johnson, always get CPU0 frequency

	/* icst_hz_to_vco rounds down -- so we need the next
	 * larger freq in case of CPUFREQ_RELATION_L.
	 */
	if (relation == CPUFREQ_RELATION_L)
		target_freq += 999;
	if (target_freq > policy->max)
		target_freq = policy->max;

  	freqs.new = get_freq(0); //  johnson, always get CPU0 frequency
	freqs.cpu = policy->cpu;

	if (freqs.old == freqs.new) {
		set_cpus_allowed(current, cpus_allowed);
		return 0;
	}

	cpufreq_notify_transition(policy, &freqs, CPUFREQ_PRECHANGE);

	/*
	 * Restore the CPUs allowed mask.
	 */
	set_cpus_allowed(current, cpus_allowed);

	cpufreq_notify_transition(policy, &freqs, CPUFREQ_POSTCHANGE);

	return 0;
}
#endif

void change_cpus_timer(char *caller, unsigned int target_freq, unsigned int cpu_id)
{
	int i = 0;
	int ret[CONFIG_NR_CPUS] = {0};
	int ret_value = 1;
	unsigned int ori_target_freq = 0;
	struct cpufreq_policy *other_cpu_policy;
	struct cpufreq_freqs freqs;

	if(bootarg_dvfs_disable)
		return;

	pr_debug("\033[36mcaller: %s, target_freq: %d KHz\033[m\n", caller, target_freq);
	ori_target_freq = target_freq;

	for_each_online_cpu(i)
	{
		other_cpu_policy = cpufreq_cpu_get(i);

		if(!other_cpu_policy) // to avoid some cpus have not had its policy
		{
			printk("\033[35mFunction = %s, Line = %d, cpu%d do: [cpu%d] policy is NULL\033[m\n", __PRETTY_FUNCTION__, __LINE__, get_cpu(), i);

			put_cpu();
			ret[i] = 0;
      		ret_value = ret_value * ret[i];
			continue;
		}

		ret[i] = 1;
		ret_value = ret_value * ret[i];
		cpufreq_cpu_put(other_cpu_policy);
	}

    if (ret_value)
    {
	/*
	 * set all cpus to new cpu_freq(scaling_cur_freq), and set the timer
	 * always let wait_for_voltage to do voltage_change,
	 * adjust target_freq according to Temperature,
	 * write adjusted target_freq to RIU, then use new target_freq to set timer, jiffes
	 */

	/* rise voltage first, wait for voltage change(accroding to target_freq) */
	target_freq = MDrvDvfsProc(target_freq, 0);
	pr_debug("\033[35mFunction = %s, Line = %d, the target_freq is changed(by MDrvDvfsProc), from %d KHz to %d KHz\033[m\n", __PRETTY_FUNCTION__, __LINE__, ori_target_freq, target_freq);

	/* pass in userspace voltage change*/
	if (voltage_change_result != 1) {

#if (!defined CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE) && (!defined CONFIG_MP_GLOBAL_TIMER_12MHZ_PATCH)
	    mstar_update_sched_clock();
#endif // CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE
	    for_each_online_cpu(i) {
		other_cpu_policy = cpufreq_cpu_get(i);

		if
(!other_cpu_policy) {
		    pr_debug("\033[35mFunction = %s, Line = %d, cpu%d do: [cpu%d] policy is NULL\033[m\n",
				__PRETTY_FUNCTION__, __LINE__, get_cpu(), i);
		    put_cpu();
		    BUG_ON(!other_cpu_policy);
		}

		if (other_cpu_policy->cur == target_freq) {
		    cpufreq_cpu_put(other_cpu_policy);
		    continue;
		}

		freqs.cpu = i;
		freqs.old = other_cpu_policy->cur;
		freqs.new = target_freq;

		if (i == 0) {
		pr_debug("\033[31mFunction = %s, Line = %d, cpu%d do: [cpu%d] adjust cpufreq from %d KHZ to %d KHZ\033[m\n",
		__PRETTY_FUNCTION__, __LINE__, get_cpu(), i, freqs.old, freqs.new);
		put_cpu();
		}

		cpufreq_notify_transition(other_cpu_policy, &freqs, CPUFREQ_PRECHANGE);
		cpufreq_notify_transition(other_cpu_policy, &freqs, CPUFREQ_POSTCHANGE);
		cpufreq_cpu_put(other_cpu_policy);
		trace_cpufreq_change(freqs.cpu, freqs.old, freqs.new); /*for meansure */
	}

#if (!defined CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE) && (!defined CONFIG_MP_GLOBAL_TIMER_12MHZ_PATCH)
			if(other_cpu_policy->cur != target_freq)
			{
				change_interval(freqs.old, freqs.new);
			}
#endif
    		current_frequency = target_freq;
    	}
    }
}

#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND
static struct cpufreq_driver integrator_driver = {
	.verify		= integrator_verify_policy,
	.target		= integrator_set_target_on_demand,
	.get		= integrator_get,
	.init		= integrator_cpufreq_init,
	.name		= "ondemand",
};
#else
static struct cpufreq_driver integrator_driver = {
	.verify		= integrator_verify_policy,
	.target		= integrator_set_target,
	.get		= integrator_get,
	.init		= integrator_cpufreq_init,
	.name		= "integrator",
};
#endif

static int CPU_calibrating_proc_open(struct inode *inode, struct file *file)
{
	if (atomic_read(&proc_is_open))
		return -EACCES;

	atomic_set(&proc_is_open, 1);
	return 0;
}

static int CPU_calibrating_proc_release(struct inode *inode, struct file * file)
{

	WARN_ON(!atomic_read(&proc_is_open));
	atomic_set(&proc_is_open, 0);
	return 0;
}

static int on_demand_handshake_proc_open(struct inode *inode, struct file *file)
{
	if(!bootarg_dvfs_disable)
		start_userspace_ondemand_handshake = 1;

	if(atomic_read(&on_demand_handshake_is_open))
		return -EACCES;

	atomic_set(&on_demand_handshake_is_open, 1);

	return 0;
}

static int on_demand_handshake_proc_release(struct inode *inode, struct file * file)
{
	pr_debug("\033[35mFunction = %s, Line = %d, set start_userspace_ondemand_handshake to be 0\033[m\n", __PRETTY_FUNCTION__, __LINE__);
	start_userspace_ondemand_handshake = 0;

	WARN_ON(!atomic_read(&on_demand_handshake_is_open));
	atomic_set(&on_demand_handshake_is_open, 0);
	return 0;
}

ssize_t on_demand_handshake_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	ON_Demand_From_Userspace from_user_data;

	if(!start_userspace_ondemand_handshake)
	{
		pr_debug("\033[35mFunction = %s, start_userspace_ondemand_handshake is %d\033[m\n", __PRETTY_FUNCTION__, start_userspace_ondemand_handshake);
		return  -EFAULT;
	}
	else
	{
		if(!count)
		{
			pr_debug("\033[35mFunction = %s, Line = %d\033[m\n", __PRETTY_FUNCTION__, __LINE__);
			return count;
		}

		if(count >= MAX_DMSG_WRITE_BUFFER)
			count = MAX_DMSG_WRITE_BUFFER - 1;

		copy_from_user(&from_user_data, buf, sizeof(from_user_data));
		finished_change_cnt = from_user_data.from_userspace_finished_change_cnt;
		voltage_change_result = from_user_data.from_userspace_voltage_change_result;

		pr_debug("\033[33m[result is %d] input: %d, wake_up wait_queue for write_down\033[m\n", voltage_change_result, finished_change_cnt);
		wake_up_interruptible(&DVFS_on_demand_event_waitqueue_userspace_return);	// to wake_up a wait_queue waiting for voltage change

		return count;
	}
}

ssize_t on_demand_handshake_proc_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	ON_Demand_To_Userspace to_userspace_data;
	int err;

	if(!start_userspace_ondemand_handshake)
	{
		if(bootarg_dvfs_disable)
		{
			pr_debug("\033[31mFunction = %s, Line = %d, [Warning] DVFS=disable is set @ bootargs, the dvfs_thread will wait forever\033[m\n", __PRETTY_FUNCTION__, __LINE__);
			down_interruptible(&DVFS_disable_SEM);
		}

		pr_debug("\033[35mFunction = %s, start_userspace_ondemand_handshake is %d\033[m\n", __PRETTY_FUNCTION__, start_userspace_ondemand_handshake);
		return  -EFAULT;
	}
	else
	{
		down_interruptible(&DVFS_on_demand_event_SEM);

		to_userspace_data.to_userspace_cpufreq = ready_to_change_cpufreq;
		to_userspace_data.to_userspace_voltage = ready_to_change_voltage;
		to_userspace_data.to_userspace_voltage_type = ready_to_change_voltage_type;
		to_userspace_data.to_userspace_change_cnt = change_cnt;

		err = copy_to_user((void *)buf, &to_userspace_data, sizeof(to_userspace_data));

		*ppos += sizeof(to_userspace_data);

		return sizeof(to_userspace_data);
	}
}

const struct file_operations proc_CPU_calibrating_operations = {
	.write      = CPU_calibrating_proc_write,
	.open       = CPU_calibrating_proc_open,
	.release    = CPU_calibrating_proc_release,
  	.unlocked_ioctl  = CPU_calibrating_proc_ioctl,
};

const struct file_operations proc_on_demand_handshake_operations = {
	.open       = on_demand_handshake_proc_open,
	.write      = on_demand_handshake_proc_write,
	.read		= on_demand_handshake_proc_read,
	.release    = on_demand_handshake_proc_release,
};

#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND

#define BOOST_AGING_TIMEOUT_IN_MS 600000 //10*60*1000

static DEFINE_SPINLOCK(T_sensor_lock);
static void Mdrv_CPU_T_sensor_Check_callback(unsigned long value)
{
	int i = 0;
    int cpu = 0; // Always set CPU 0;
	int T_sensor_max_freq = 0;
	unsigned long echo_calibrating_freq_tmp = 0;

	if (is_any_boost_client_running() == true) {
		if (jiffies_to_msecs(jiffies - jiffies_boost_lasttime) > BOOST_AGING_TIMEOUT_IN_MS) {
			pr_debug("[dvfs boost] timout happens and delete all of the running clients\n");
			del_all_boost_client();
#if !defined(CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND)
            __CPU_calibrating_proc_write(1008000);
#endif
		} else {
			// nothing
		}
	} else {
		// nothing
	}

	if(!start_userspace_ondemand_handshake)
	{
		spin_lock(&T_sensor_lock);
#if (MP_PLATFORM_T_SENSOR_OBSERVATION == 0)
		Check_T_sensor_timer.expires = jiffies + FREQ_CHECK_LINK_TIME;
		add_timer(&Check_T_sensor_timer);
#endif
		spin_unlock(&T_sensor_lock);

		pr_debug("\033[34mFunction = %s, return Mdrv_CPU_T_sensor_Check_callback, start_userspace_ondemand_handshake is %d\033[m\n", __PRETTY_FUNCTION__, start_userspace_ondemand_handshake);
		return;
	}

	T_sensor_max_freq = MDrvDvfsQueryCpuClockByTemperature(0);
	pr_debug("\033[34mFunction = %s, Line = %d, do Mdrv_CPU_T_sensor_Check_callback: %d, forcibly_set_target_flag is %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, T_sensor_max_freq, forcibly_set_target_flag[cpu]);

#if (!defined CONFIG_MP_DVFS_FORCE_PINGO_TEST)
	if (is_any_boost_client_running() == true)
	{
		echo_calibrating_freq_tmp = read_echo_calibrating_freq();
		if(echo_calibrating_freq_tmp < T_sensor_max_freq)
		{
			T_sensor_max_freq = echo_calibrating_freq_tmp;
		}
	}
	else if(T_sensor_max_freq > MAX_CPU_FREQ)
	{
		T_sensor_max_freq = MAX_CPU_FREQ;
	}

	for_each_online_cpu(i)
	{
#if 0
		if(T_sensor_max_freq < ondemand_timer[i]->min)
		{
			T_sensor_max_freq = ondemand_timer[i]->min;
		}
#endif
		if(T_sensor_max_freq == 100000)	// this is a special case, only adjust the cpu_voltage only
		{
            T_sensor_max_freq = ondemand_timer[i]->max;
            forcibly_set_target_flag[cpu] = 5;
        }
        else
		{
		    ondemand_timer[i]->max = T_sensor_max_freq;

    		if(T_sensor_max_freq < ondemand_timer[i]->cur)
    		{
			pr_debug("\033[31mFunction = %s, Line = %d, T_sensor_max_freq < ondemand_timer[%d]->cur\033[m\n", __func__, __LINE__, i);
			pr_debug("\033[34mFunction = %s, Line = %d, T_sensor_max_freq: %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, T_sensor_max_freq);
			pr_debug("\033[34mFunction = %s, Line = %d, ondemand_timer[%d]->cur: %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, i, ondemand_timer[i]->cur);

				/* change to set a flag to forcibly trigger integrator_set_target_on_demand() in od_check_cpu() */
    			forcibly_set_target_flag[cpu] = 5;
    		}
			else
			{
				if( (i == 0) && (forcibly_set_target_flag[cpu] > 0) )
				{
					forcibly_set_target_flag[cpu] --;
				}
			}
        }
	}
#endif

	spin_lock(&T_sensor_lock);
#if (MP_PLATFORM_T_SENSOR_OBSERVATION == 0)
	Check_T_sensor_timer.expires = jiffies + FREQ_CHECK_LINK_TIME;
	add_timer(&Check_T_sensor_timer);
#endif
	spin_unlock(&T_sensor_lock);
}
#else
static void Mdrv_CPU_Freq_Check_callback(unsigned long value)
{
	int freq, i;
	struct cpufreq_freqs freqs;
	struct cpufreq_policy *policy;

	if(*(volatile u32 *)(0xfd200a00) == 0x3697)
   	{
   		freq = *(volatile u32 *)(0xfd200a04);
   		freq = freq * 1000;

   		if (freq != current_frequency)
   		{
#if (!defined CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE) && (!defined CONFIG_MP_GLOBAL_TIMER_12MHZ_PATCH)
			mstar_update_sched_clock();
#endif // CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE
			pr_debug("\033[36m\nFunction = %s, Line = %d, (freq = %d KHZ) != (current_frequency = %d KHZ)\033[m\n", __PRETTY_FUNCTION__, __LINE__, freq, current_frequency);
			if(freq < MIN_CPU_FREQ)
			{
				printk(KERN_WARNING "\033[36m\n freq %d KHZ < MIN_CPU_FREQ %d KHZ ,not allowed\033[m\n",freq,MIN_CPU_FREQ);
				return;
			}

			if(freq > MAX_CPU_FREQ)
			{
				printk(KERN_WARNING "\033[36m\n freq %d KHZ > MAX_CPU_FREQ %d KHZ ,not allowed\033[m\n",freq,MAX_CPU_FREQ);
				return;
			}

			register_frequency = freq;

			for_each_online_cpu(i)
	        {
				policy=cpufreq_cpu_get(i);
	        	freqs.cpu = i;
		        freqs.old = current_frequency;
		        freqs.new = freq;
		        cpufreq_notify_transition(policy, &freqs, CPUFREQ_PRECHANGE);
              	cpufreq_notify_transition(policy, &freqs, CPUFREQ_POSTCHANGE);
            	cpufreq_cpu_put(policy);
            }

			current_frequency = freq;
#if (!defined CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE) && (!defined CONFIG_MP_GLOBAL_TIMER_12MHZ_PATCH)
			change_interval(freqs.old, freqs.new);
#endif
   		}
   	}
	Check_Freq_timer.expires = jiffies + FREQ_CHECK_LINK_TIME;
	add_timer(&Check_Freq_timer);
}
#endif

static int __init init_procfs_msg(void)
{
	ORI_FREQ_KHZ = query_frequency(0) * 1000; // Query CPU0 frequency

	current_frequency = ORI_FREQ_KHZ;

	pr_debug("\033[35mFunction = %s, Line = %d, [cpu %d] current_frequency = %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, get_cpu(), current_frequency);
	put_cpu();

	cpufreq_register_driver(&integrator_driver);
	proc_create("CPU_calibrating", S_IRUSR | S_IWUSR, NULL, &proc_CPU_calibrating_operations);
	proc_create("on_demand_ctl"  , S_IRUSR | S_IWUSR, NULL, &proc_on_demand_handshake_operations);
	sema_init(&DVFS_on_demand_event_SEM, 0);

#if !defined(CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND)
	init_timer(&Check_Freq_timer);
    Check_Freq_timer.data = 1;
    Check_Freq_timer.function = Mdrv_CPU_Freq_Check_callback;
    Check_Freq_timer.expires = jiffies + FREQ_CHECK_LINK_TIME;
    add_timer(&Check_Freq_timer);
#endif

	return 0;
}

static int __init CPU_calibrating_init(void)
{
	pr_debug("\033[35mFunction = %s, Line = %d, [cpu %d] do CPU_calibrating_init\033[m\n", __PRETTY_FUNCTION__, __LINE__, get_cpu());
	put_cpu();

	init_procfs_msg();
	return 0;
}

static int __init DVFS_enable(char *str)
{
    if(strcmp(str, "disable") == 0)
    {
        printk("\nDVFS_disable\n");
	    bootarg_dvfs_disable = 1;
	    sema_init(&DVFS_disable_SEM, 0);
    }
    else
    {
	    bootarg_dvfs_disable = 0;
    }
    return 0;
}
early_param("DVFS", DVFS_enable);

module_init(CPU_calibrating_init);
