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
#include <asm/cputype.h>
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
#include <linux/cpu.h>
#include <linux/io.h>
#include <linux/timer.h>
#include <linux/vmalloc.h>
#include <mstar/mpatch_macro.h>
#include <trace/events/sched.h>
#include <linux/seq_file.h>

#if (MP_PLATFORM_T_SENSOR_OBSERVATION == 1)
#include <linux/kthread.h>
#endif

#include "./include/mach/hardware.h"
#include "./include/mach/platform.h"
#include "mdrv_CPU_cluster_calibrating.h"

#include <chip_dvfs_calibrating.h>
#include <linux/version.h>

#ifdef CONFIG_MSTAR_DVFS
#ifndef __MDRV_DVFS_H__
#include "mdrv_dvfs.h"
#include <mach/io.h>
#endif
#endif


#define DVFS_DEBUG KERN_DEBUG
#define DVFS_LOCK_DEBUG KERN_DEBUG

#define BOOST_AGING_TIMEOUT_IN_MS 600000 /* 10(mins) * 60 * 1000 */
#define BOOST_DURATION_CHECK_PERIOD 20 /* check boost duration per 20ms */
#define MAX_DMSG_WRITE_BUFFER	64
#define CM_ID  	IO_ADDRESS(INTEGRATOR_HDR_ID)
#define CM_OSC	IO_ADDRESS(INTEGRATOR_HDR_OSC)
#define CM_STAT IO_ADDRESS(INTEGRATOR_HDR_STAT)
#define CM_LOCK IO_ADDRESS(INTEGRATOR_HDR_LOCK)
#define FREQ_CHECK_LINK_TIME (HZ)
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,23)
#define cpufreq_notify_transition(A, B, C)	cpufreq_notify_transition(A, B, C)
#endif	// 3.10.86 pass 3 parameters

struct task_struct *boost_duration_check_tsk;
extern int halTotalClusterNumber;
extern void change_interval(unsigned int old_freq, unsigned int new_freq);
extern unsigned int query_frequency(unsigned int cpu_id);
extern struct cpufreq_policy *cpufreq_cpu_data;
#ifndef CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE
extern void mstar_update_sched_clock(void);
extern unsigned int SC_MULT;
extern unsigned int SC_SHIFT;
#endif // CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE
extern int getClusterMainCpu(unsigned int cpu);
extern U32 getFreqRiuAddr(unsigned int cpu);
extern unsigned int get_cpu_midr(int cpu);

/* str usage */
atomic_t disable_dvfs = ATOMIC_INIT(0);

/* proc usage */
//static DEFINE_SPINLOCK(set_freq_lock);
static atomic_t proc_is_open = ATOMIC_INIT(0);
static struct cpufreq_driver integrator_driver;
unsigned int current_frequency = 0;
unsigned int register_frequency = 0;
static atomic_t t_sensor_proc_is_open = ATOMIC_INIT(0);


/* on_demand handshake usage */
static atomic_t on_demand_handshake_is_open = ATOMIC_INIT(0);
DECLARE_WAIT_QUEUE_HEAD(DVFS_on_demand_event_waitqueue);
DECLARE_WAIT_QUEUE_HEAD(DVFS_on_demand_event_waitqueue_userspace_return);
EXPORT_SYMBOL(DVFS_on_demand_event_waitqueue);
EXPORT_SYMBOL(DVFS_on_demand_event_waitqueue_userspace_return);
DEFINE_SEMAPHORE(DVFS_on_demand_event_SEM);
DEFINE_SEMAPHORE(DVFS_disable_SEM);
DEFINE_MUTEX(DVFS_on_demand_handshake_write_mutex);



/* this is for muji test cmdq (we don not use SAR to adjust voltage) */
#if defined(CONFIG_MSTAR_IIC) && defined(CONFIG_MSTAR_DVFS_KERNEL_IIC)
unsigned int start_userspace_ondemand_handshake = 1;
#else
unsigned int start_userspace_ondemand_handshake = 0;
#endif
unsigned int ready_to_change_cpufreq[CONFIG_NR_CPUS] = {0};
unsigned int ready_to_change_voltage[CONFIG_NR_CPUS] = {0};
unsigned int ready_to_change_voltage_type[CONFIG_NR_CPUS] = {0};
unsigned int ready_to_change_cpu[CONFIG_NR_CPUS] = {0};
unsigned int ready_to_change_cluster_id[CONFIG_NR_CPUS] = {0};
unsigned int change_cnt[CONFIG_NR_CPUS] = {0};
unsigned int finished_change_cnt[CONFIG_NR_CPUS] = {0};
int voltage_change_result[CONFIG_NR_CPUS] = {0};
bool forcibly_set_target_flag[CONFIG_NR_CPUS] = {0};

unsigned int bootarg_dvfs_disable = 0;
unsigned int bootarg_dvfs_t_sensor_disable = 0;
static unsigned int mstar_debug = 0;
static unsigned int mstar_info = 0;

struct mstar_cpufreq_policy
{
	struct cpufreq_policy *policy;
	unsigned int cluster; //It is used to be indicated the CPU belong which cluster is
	unsigned int cluster_m; //master CPU of cluster which we need to change
	unsigned long freq_riu; //RIU address for setting CPU frequency.
	atomic_t echo_calibrating_freq;
	atomic_t ac_str_cpufreq; //Used for STR.
	unsigned int cur_freq;
	unsigned int sys_max_freq;
	unsigned int jiffies_boost_lasttime;
#if (MP_PLATFORM_T_SENSOR_OBSERVATION == 1)
	struct task_struct *t_sensor_tsk;
#endif
	int t_sensor_max_freq;
	struct list_head boost_head;

	u32 over_temperature_mode;	/* set this if we are now over_temperature */
};

struct mstar_cpufreq_policy ondemand_timer[CONFIG_NR_CPUS];
#if defined(CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND) || defined(CONFIG_CPU_FREQ_DEFAULT_GOV_INTERACTIVE)
static struct timer_list Check_T_sensor_timer;
static void Mdrv_CPU_T_sensor_Check_callback(unsigned long value);
#else
static struct timer_list Check_Freq_timer;
static void Mdrv_CPU_Freq_Check_callback(unsigned long value);
#endif

/* Define cluster-specified locks */
DEFINE_MUTEX(mstar_cpufreq_lock_Little);
DEFINE_MUTEX(mstar_cpufreq_lock_Big);
DEFINE_MUTEX(mstar_voltagesetup);
#if (MP_PLATFORM_T_SENSOR_OBSERVATION == 1)
#define t_sensor_monitor_owner_cpu_init_value 0
/* specify the cpu owning the t_sentor thread */
unsigned int t_sensor_monitor_owner_cpu = t_sensor_monitor_owner_cpu_init_value;
#endif

static DEFINE_MUTEX(boost_client_mutex);

struct boost_client
{
	unsigned long client_id;
	unsigned long cpu_freq_in_khz;
	unsigned long boost_duration_ms;
	unsigned long jiffies_duration_starttime_ms;
	struct list_head list_head;
};
static struct boost_client *show_boost_client(int cpu_id);
static struct boost_client *find_boost_client(unsigned long client_id, int cpu_id);
static struct boost_client *update_boost_duration(unsigned long client_id, int set_cpu, unsigned int set_duration);
static void check_boost_duration(int cpu_id);

static void Mdrv_CpuFreq_Lock(int cpu_id, char *caller)
{
	if (ondemand_timer[cpu_id].cluster == 0) {
		mutex_lock(&mstar_cpufreq_lock_Little);
	} else if (ondemand_timer[cpu_id].cluster == 1) {
		mutex_lock(&mstar_cpufreq_lock_Big);
	} else {
		/*
		 * printk(KERN_ERR "\033[31m[Error] Function = %s, Line = %d, cpu_id: %d,
		 * can not get cluster\033[m\n", __PRETTY_FUNCTION__, __LINE__, cpu_id);
		*/
		BUG_ON(1);
	}
}

static void Mdrv_CpuFreq_UnLock(int cpu_id, char *caller)
{
	if (ondemand_timer[cpu_id].cluster == 0) {
		mutex_unlock(&mstar_cpufreq_lock_Little);
	} else if (ondemand_timer[cpu_id].cluster == 1) {
		mutex_unlock(&mstar_cpufreq_lock_Big);
	} else {
		/*
		 * printk(KERN_ERR "\033[31m[Error] Function = %s, Line = %d, cpu_id: %d,
		 * can not get cluster\033[m\n", __PRETTY_FUNCTION__, __LINE__, cpu_id);
		 */
		BUG_ON(1);
	}
}

void Mdrv_CpuFreq_All_Lock(char *caller)
{
	mutex_lock(&mstar_cpufreq_lock_Big);

	mutex_lock(&mstar_cpufreq_lock_Little);
}

void Mdrv_CpuFreq_All_UnLock(char *caller)
{
	mutex_unlock(&mstar_cpufreq_lock_Little);

	mutex_unlock(&mstar_cpufreq_lock_Big);
}

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

	current_freq = get_freq(cpu);

	set_cpus_allowed(current, cpus_allowed);

	return current_freq;
}

ssize_t dvfs_boost_duration_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	char buffer[MAX_DMSG_WRITE_BUFFER];
	unsigned int set_cpu = 0;
	unsigned int garbage = 0;
	unsigned long idx;
	unsigned long client_id = 0;
	unsigned int set_duration = 0;
	struct boost_client *bc = NULL;

	if (!count)
		return count;

	if (count >= MAX_DMSG_WRITE_BUFFER)
		count = MAX_DMSG_WRITE_BUFFER - 1;

	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	buffer[count] = '\0';

	if (sscanf(buffer, "%d %lu %d", &set_cpu, &idx, &garbage) == 3) {
		return -EINVAL;
	} else if (sscanf(buffer, "%d %lu", &set_cpu, &idx) == 2) {
		if (set_cpu >= CONFIG_NR_CPUS)
			return -EINVAL;

		/* idx[29:22] is the client id, the same as /proc/CPU_calibrating */
		client_id = (idx >> 22) & 0xff;

		/* idx[21:0] is the cpu boost_duration in ms */
		set_duration = idx & 0x3FFFFF;

		if (set_duration > BOOST_AGING_TIMEOUT_IN_MS)
			return -EINVAL;
		set_duration = BOOST_DURATION_CHECK_PERIOD * (set_duration/BOOST_DURATION_CHECK_PERIOD);

		/* find the boost_client, and update the boost_duration */
		bc = update_boost_duration(client_id, set_cpu, set_duration);
		if (bc == NULL)
			return -EINVAL;

		return count;
	}

	return -EINVAL;
}

int dvfs_boost_duration_seq_show(struct seq_file *s, void *v)
{
	int i = 0;

	for_each_online_cpu(i)
	{
		if (i != ondemand_timer[i].cluster_m)
			continue;

		show_boost_client(i);
	}

	return TRUE;
}

static int boost_duration_thread(void *arg)
{
	int i;

	while (1) {
		msleep(BOOST_DURATION_CHECK_PERIOD);

		for_each_online_cpu(i)
		{
			if (i != ondemand_timer[i].cluster_m)
				continue;

			check_boost_duration(i);
		}
	}
}

#if (MP_PLATFORM_T_SENSOR_OBSERVATION == 1)
static int t_sensor_thread(void *arg)
{
	while (1) {
		msleep(1000);

		/* this lock should be cluster-specified, or Little Cluster will hang
		 * here while Big Cluster hang. we disable thislock first
		 * we need to enable this lock, due to T_sensor_check and on_demand
		 * will both do MDrvDvfsVoltageSetup(), and thus the change_cnt will not equal!!
		 * Now, we use mstar_cpufreq_lock_Little here, because we will
		 * only let t_sensor_thread on Little_Cluster
		 */
		Mdrv_CpuFreq_Lock(t_sensor_monitor_owner_cpu, (char *)__func__);
		if (atomic_read(&disable_dvfs) != 1)
			Mdrv_CPU_T_sensor_Check_callback(0);
		Mdrv_CpuFreq_UnLock(t_sensor_monitor_owner_cpu, (char *)__func__);
	}

	return 0;
}
#endif

static int integrator_cpufreq_init(struct cpufreq_policy *policy)
{
	int cpu_id = get_cpu();
	put_cpu();
	int ret;
    struct task_struct *t_sensor_tsk;
    //ondemand_timer = kmalloc(sizeof(mstar_cpufreq_policy) * CONFIG_NR_CPUS, GFP_KERNEL);

	/* set default policy and cpuinfo */
	ret = cpufreq_frequency_table_cpuinfo(policy, cpufreq_frequency_get_table(policy->cpu));
	policy->min = policy->cpuinfo.min_freq = CONFIG_DVFS_CPU_CLOCK_MIN(policy->cpu);
	policy->max = policy->cpuinfo.max_freq = CONFIG_DVFS_CPU_IRBOOST_CLOCK(policy->cpu); //We only allow max freq from kernel is IR boost level, not system maximal freq
	policy->cpuinfo.transition_latency = TRANSITION_LATENCY; 	/* 1 ms, assumed */
	policy->cur = integrator_get(policy->cpu);

	ondemand_timer[policy->cpu].policy = policy;
	ondemand_timer[policy->cpu].cluster = getCpuCluster(policy->cpu);
	ondemand_timer[policy->cpu].cluster_m = getClusterMainCpu(policy->cpu);
	ondemand_timer[policy->cpu].freq_riu = getFreqRiuAddr(policy->cpu);
	atomic_set(&ondemand_timer[policy->cpu].echo_calibrating_freq, 0);
	INIT_LIST_HEAD(&(ondemand_timer[policy->cpu].boost_head));
/*
pr_debug("\033[31mFunction = %s, Line = %d, cluster = %d, policy->cpu = %d, cluster_m:
%d, max_freq: %u, min_freq: %u\033[m\n",
__PRETTY_FUNCTION__, __LINE__, ondemand_timer[policy->cpu].cluster, policy->cpu,
ondemand_timer[policy->cpu].cluster_m, policy->max, policy->min);
*/
	/* move to here to let the timer works on master cpu of cluster */
	if(policy->cpu == ondemand_timer[policy->cpu].cluster_m) // means it is master CPU of cluster
	{
		atomic_set(&ondemand_timer[policy->cpu].ac_str_cpufreq, policy->cur);
/*
		pr_debug("\033[31mFunction = %s, Line = %d, set cpu%d ac_str_cpufreq: %d KHz\033[m\n",
			__PRETTY_FUNCTION__, __LINE__, policy->cpu, ondemand_timer[policy->cpu].ac_str_cpufreq.counter);
*/
#if defined(CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND) || defined(CONFIG_CPU_FREQ_DEFAULT_GOV_INTERACTIVE)
#if (MP_PLATFORM_T_SENSOR_OBSERVATION == 0)
    	init_timer(&Check_T_sensor_timer);
    	Check_T_sensor_timer.data = 1;
    	Check_T_sensor_timer.function = Mdrv_CPU_T_sensor_Check_callback;
    	Check_T_sensor_timer.expires = jiffies + FREQ_CHECK_LINK_TIME;  // 1 second do once
    	add_timer(&Check_T_sensor_timer);
#else
	if (policy->cpu == t_sensor_monitor_owner_cpu) {
		if (!ondemand_timer[policy->cpu].t_sensor_tsk) {
			t_sensor_tsk = kthread_create(t_sensor_thread, NULL, "T_sensor_Check");

			kthread_bind(t_sensor_tsk, policy->cpu);

			if (IS_ERR(t_sensor_tsk)) {
				printk("create kthread for t_sensor temperature observation fail\n");
				ret = PTR_ERR(t_sensor_tsk);
				t_sensor_tsk = NULL;
				goto out;
			}else
				wake_up_process(t_sensor_tsk);

			ondemand_timer[policy->cpu].t_sensor_tsk = t_sensor_tsk;
			pr_debug("\033[31mCreate T sensor on cpu:%d, set str freq to %dkhz\033[m\n", policy->cpu, atomic_read(&ondemand_timer[policy->cpu].ac_str_cpufreq));
		}
		else {
			wake_up_process(ondemand_timer[policy->cpu].t_sensor_tsk);
			pr_debug("\033[31mWake up T sensor on cpu:%d\033[m\n", policy->cpu);
		}

		if (!boost_duration_check_tsk) {
			boost_duration_check_tsk = kthread_create(boost_duration_thread, NULL, "Boost_Duration_Check");
			kthread_bind(boost_duration_check_tsk, policy->cpu);

			if (IS_ERR(boost_duration_check_tsk)) {
				ret = PTR_ERR(boost_duration_check_tsk);
				boost_duration_check_tsk = NULL;
				BUG_ON(1);
			} else
				wake_up_process(boost_duration_check_tsk);
		}
	}
#endif
#endif // CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND
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
	IO_CPU_calibrating_INFO* bb = message_buf;
	char usr_buf[256];

	if (copy_from_user(usr_buf, bb->MESSAGE_BUFF, bb->MESSAGE_LEN))
	{
		printk(KERN_ERR "setgreq_proc_ioctl error\n");
		return -EFAULT;
	}

	return 0;
}

int _CPU_calibrating_proc_write(const unsigned long, const unsigned long, int);
int __CPU_calibrating_proc_write(const unsigned long, unsigned int);
void write_cpufreq_to_RIU(const unsigned long, int);
static unsigned long read_echo_calibrating_freq(int);
static void write_echo_calibrating_freq(const unsigned long, int);
static bool is_any_boost_client_running(int);
static int del_boost_client(unsigned long, int);
static struct boost_client *find_boost_client_with_highest_priority(int cpu_id);

static ssize_t CPU_calibrating_proc_write(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[MAX_DMSG_WRITE_BUFFER];
	long idx;
	unsigned long cpu_freq_in_khz = 0;
	unsigned long client_id = 0;
	unsigned int set_cpu = 0;
	unsigned int garbage = 0;

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
    {
	    idx = 3;
	} else if (sscanf(buffer, "%lu %d %d", &idx, &set_cpu, &garbage) == 3) {
		return -EINVAL;
	/* multi-cluster version, can specify cpu, as set_cpu */
	} else if (sscanf(buffer, "%d %lu", &set_cpu, &idx) == 2) {
		if (set_cpu > CONFIG_NR_CPUS)
			return -EINVAL;

        // idx[29:22] is the client id
	    client_id = (idx >> 22) & 0xff;

	    // idx[21:0] is the cpu frequency in Khz
	    cpu_freq_in_khz = idx & 0x3FFFFF;

		if (mstar_debug) {
			pr_debug("\033[34m%s %d: set cpu:%d client_id:%d cpu_freq_in_khz:%d\033[m\n", __PRETTY_FUNCTION__, __LINE__, set_cpu, client_id, cpu_freq_in_khz);
		}
        _CPU_calibrating_proc_write(client_id, cpu_freq_in_khz, set_cpu);
		return count;
    }
	else if (strict_strtol(buffer, 0, &idx) == 0)  //Force change str to decimal conversion (str, base, *converted_num), base is hex, decimal, or ...
	{
	    // idx[29:22] is the client id
	    client_id = (idx >> 22) & 0xff;

	    // idx[21:0] is the cpu frequency in Khz
	    cpu_freq_in_khz = idx & 0x3FFFFF;

		for (set_cpu = 0; set_cpu < CONFIG_NR_CPUS; set_cpu ++) {
				if (mstar_debug) {
					pr_debug("\033[34m%s %d: set cpu:%d client_id:%d cpu_freq_in_khz:%d\033[m\n", __PRETTY_FUNCTION__, __LINE__, set_cpu, client_id, cpu_freq_in_khz);
				}
                _CPU_calibrating_proc_write(client_id, cpu_freq_in_khz, set_cpu);
		}
		return count;
    }
	return -EINVAL;
}

static int CPU_calibrating_seq_show(struct seq_file *s, void *v)
{
	int i, j;
	unsigned int freq = 0;

	if (mstar_info) {
		for (i = 0; i < halTotalClusterNumber; i ++) {
			for_each_online_cpu(j)
			{
				//freq = query_frequency(i);
				if (ondemand_timer[j].cluster == i) {
					seq_printf(s, "CPU_part:%x: max_freq: %u | sys_freq: %u | min_freq:%u\n",
								MIDR_PARTNUM(get_cpu_midr(j)),
								CONFIG_DVFS_CPU_CLOCK_MAX(j),
								CONFIG_DVFS_CPU_IRBOOST_CLOCK(j),
								CONFIG_DVFS_CPU_CLOCK_MIN(j));
					break;
				}
			}
		}
	}

	if (mstar_debug) {
		pr_debug("Total %d %s\n", halTotalClusterNumber, (halTotalClusterNumber == 1 ? "cluster" : "clusters"));
		get_online_cpus();
		for (i = 0; i < halTotalClusterNumber; i ++)
		{
			pr_debug("Cluster %d CPU:", i);
			for_each_online_cpu(j)
			{
				if (ondemand_timer[j].cluster == i)
					pr_debug(" %d", ondemand_timer[j].policy->cpu);
			}
			for_each_online_cpu(j)
			{
				if (ondemand_timer[j].cluster == i) {
					show_boost_client(ondemand_timer[j].policy->cpu);
					break;
				}
			}
			pr_debug("\n");
		}
		put_online_cpus();
	}
	return 0;
}

static struct boost_client *show_boost_client(int cpu_id)
{
	struct boost_client *i = NULL;
	struct boost_client *bc = NULL;
	struct list_head *boost_client_head;
	int m = ondemand_timer[cpu_id].cluster_m;
	boost_client_head = &(ondemand_timer[m].boost_head);

	mutex_lock(&boost_client_mutex);
	if (list_empty(boost_client_head)) {
	}
	else {
		pr_debug(" -->[boost client for cpu: %d]\n", cpu_id);
		/*
		list_for_each_entry(i, boost_client_head, list_head) {
			pr_debug("(client id: %lu, bootst_freq: %lukhz)\n", i->client_id, i->cpu_freq_in_khz);
			pr_debug("boost_duration_ms: %lums, start_jiffies: %lums",
				i->boost_duration_ms, i->jiffies_duration_starttime_ms);
		}
		*/
		pr_debug("End\n");
	}
	mutex_unlock(&boost_client_mutex);

	return bc;
}

static bool is_any_boost_client_running(int cpu_id)
{
	bool i;
	struct list_head *boost_client_head;
	int m = ondemand_timer[cpu_id].cluster_m;
	boost_client_head = &(ondemand_timer[m].boost_head);

	mutex_lock(&boost_client_mutex);
	i = !list_empty(boost_client_head);
	mutex_unlock(&boost_client_mutex);
#if 0
    if (i)
        show_boost_client(cpu_id);
#endif

	return i;
}

static int add_boost_client(unsigned long client_id, unsigned long cpu_freq_in_khz, unsigned int cpu_id, void *arg)
{
	struct boost_client *bc = NULL;
        int m = ondemand_timer[cpu_id].cluster_m;

	if ((bc = kmalloc(sizeof(*bc), GFP_KERNEL)) == NULL)
		return -ENOMEM;

	bc->client_id = client_id;
	bc->cpu_freq_in_khz = cpu_freq_in_khz;

	mutex_lock(&boost_client_mutex);
	list_add(&bc->list_head, &(ondemand_timer[m].boost_head));
	bc->boost_duration_ms = 0;
	bc->jiffies_duration_starttime_ms = 0;
	mutex_unlock(&boost_client_mutex);

	return 0;
}

static struct boost_client *find_boost_client(unsigned long client_id, int cpu_id)
{
	struct boost_client *i = NULL;
	struct boost_client *bc = NULL;
	struct list_head *boost_client_head;
	int m = ondemand_timer[cpu_id].cluster_m;
	boost_client_head = &(ondemand_timer[m].boost_head);

	mutex_lock(&boost_client_mutex);
	list_for_each_entry(i, boost_client_head, list_head) {
		if (i->client_id == client_id) {
			bc = i;
		}
	}
	mutex_unlock(&boost_client_mutex);

	return bc;
}

static struct boost_client *update_boost_duration(unsigned long client_id, int set_cpu, unsigned int set_duration)
{
	struct boost_client *i = NULL;
	struct boost_client *bc = NULL;
	struct list_head *boost_client_head;
	int m = ondemand_timer[set_cpu].cluster_m;
	boost_client_head = &(ondemand_timer[m].boost_head);

	mutex_lock(&boost_client_mutex);
	/* find the boost_client, and update the boost_duration */
	list_for_each_entry(i, boost_client_head, list_head) {
		if (i->client_id == client_id)
			bc = i;
	}

	if (bc != NULL) {
		bc->boost_duration_ms = set_duration;
		bc->jiffies_duration_starttime_ms = jiffies_to_msecs(jiffies);
		/* once user set the duration, we then start to calculate the diff_jiffies,
		 use jiffies_duration_starttime to check if need del boost_client @ t-sensor callback */
	}

	mutex_unlock(&boost_client_mutex);
	return bc;
}

static void check_boost_duration(int cpu_id)
{
	struct boost_client *i = NULL;
	struct boost_client *bc = NULL;
	struct boost_client *delete_list[256];
	int delete_cnt, j;
	struct list_head *boost_client_head;
	int first_print;
	int m = ondemand_timer[cpu_id].cluster_m;
	boost_client_head = &(ondemand_timer[m].boost_head);

	delete_cnt = 0;
	first_print = 0;
	mutex_lock(&boost_client_mutex);

	if (!list_empty(boost_client_head)) {
		list_for_each_entry(i, boost_client_head, list_head) {
			if (i->jiffies_duration_starttime_ms > 0) {
				if ((jiffies_to_msecs(jiffies) - i->jiffies_duration_starttime_ms) >
				i->boost_duration_ms) {
					if (first_print == 0)
						first_print = 1;
					/*
					pr_debug("(boost_freq: %lukhz, boost_duration_ms: %lums)\n",
						i->cpu_freq_in_khz, i->boost_duration_ms);
					pr_debug("start_time: %lums, current_jiffies: %ums",
						i->jiffies_duration_starttime_ms, jiffies_to_msecs(jiffies));
					*/

					/* delete this client_id, add it to delete_list[]
					(we will later remove from boost_client_head and free it) */
					delete_list[delete_cnt] = i;
					delete_cnt++;
				}
			}
		}
	}

	for (j = 0; j < delete_cnt; j++) {
		list_del(&delete_list[j]->list_head);
		kfree(delete_list[j]);
	}

	/* having remove a client_id, so we need to update the policy->max */
	if (delete_cnt) {
		if (!list_empty(boost_client_head)) {
			bc = list_entry(boost_client_head->next, struct boost_client, list_head);
			list_for_each_entry(i, boost_client_head, list_head);
			{
				if (i->client_id < bc->client_id)
					bc = i;
			}
			write_echo_calibrating_freq(bc->cpu_freq_in_khz, cpu_id);
		}
	}

	mutex_unlock(&boost_client_mutex);

	return;
}

static int del_boost_client(unsigned long client_id, int cpu_id)
{
	struct boost_client *i = NULL;
	struct boost_client *bc = NULL;
	struct list_head *boost_client_head;
	int m = ondemand_timer[cpu_id].cluster_m;
	boost_client_head = &(ondemand_timer[m].boost_head);

	mutex_lock(&boost_client_mutex);

	/* serach client */
	list_for_each_entry(i, boost_client_head, list_head) {
		if (i->client_id == client_id)
			bc = i;
	}

	if (bc == NULL) {
		mutex_unlock(&boost_client_mutex);
		return 0;
	}

	// remove the client node
	list_del(&bc->list_head);
	mutex_unlock(&boost_client_mutex);

	kfree(bc);

	return 0;
}

static bool del_all_boost_client(int cpu_id)
{
	struct boost_client *bc = NULL;
	struct boost_client *bc2 = NULL;
	int m = ondemand_timer[cpu_id].cluster_m;
	struct list_head *boost_client_head;
	boost_client_head = &(ondemand_timer[m].boost_head);

	mutex_lock(&boost_client_mutex);
	list_for_each_entry_safe(bc, bc2, boost_client_head, list_head) {
		list_del(&bc->list_head);
		kfree(bc);
	}
	mutex_unlock(&boost_client_mutex);

	return true;
}

static bool is_boost_client_running(unsigned long client_id, int cpu_id)
{
	struct boost_client *i = NULL;
	struct boost_client *bc = NULL;
	struct list_head *boost_client_head;
	int m = ondemand_timer[cpu_id].cluster_m;
	boost_client_head = &(ondemand_timer[m].boost_head);

	mutex_lock(&boost_client_mutex);

	/* serach client */
	list_for_each_entry(i, boost_client_head, list_head) {
		if (i->client_id == client_id)
			bc = i;
	}
	mutex_unlock(&boost_client_mutex);

	return (bc != NULL) ? true : false;
}

static struct boost_client *find_boost_client_with_highest_priority(int cpu_id)
{
	struct boost_client *i = NULL;
	struct boost_client *bc = NULL;
    struct list_head *boost_client_head;
	int m = ondemand_timer[cpu_id].cluster_m;
    boost_client_head = &(ondemand_timer[m].boost_head);

	mutex_lock(&boost_client_mutex);
	bc = list_entry(boost_client_head->next, struct boost_client, list_head);
	list_for_each_entry(i, boost_client_head, list_head) {
		if(i->client_id < bc->client_id) {
			bc = i;
		}
	}
	mutex_unlock(&boost_client_mutex);

	return bc;
}

static unsigned long read_echo_calibrating_freq(int cpu_id)
{
    return (unsigned long)atomic_read(&(ondemand_timer[cpu_id].echo_calibrating_freq));
}

static void write_echo_calibrating_freq(const unsigned long value, int cpu_id)
{
    atomic_set(&(ondemand_timer[cpu_id].echo_calibrating_freq), value);
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

	/* if not cluster_master, return */
	if (cpu_id != ondemand_timer[cpu_id].cluster_m)
		return ret;

	Mdrv_CpuFreq_Lock(cpu_id, (char *)__func__);
	if( (!start_userspace_ondemand_handshake) || (atomic_read(&disable_dvfs) == 1) )
	{
/* this means utopia is not ready
pr_debug("\033[34mFunction = %s, [Return] start_userspace_ondemand_handshake is %d disable_dvfs is %d\033[m\n",
__PRETTY_FUNCTION__, start_userspace_ondemand_handshake,
atomic_read(&disable_dvfs));
*/
		ret = -1;
        goto not_change_cpu_freq;
	}
	pr_debug("[dvfs_boost] here comes a client id = %lu and cpu_freq_in_khz = %lu cpu:%d\n",
		client_id, cpu_freq_in_khz, cpu_id);

	get_online_cpus();
	for_each_online_cpu(i)
	{
		policy = cpufreq_cpu_get(i);

		if(!policy)
		{
			printk("\033[31mFunction = %s, Line = %d, cpu%d do: [cpu%d] policy is NULL\033[m\n",
						__PRETTY_FUNCTION__, __LINE__, get_cpu(), i);
			put_cpu();
			put_online_cpus();
			goto not_change_cpu_freq;
		}
		cpufreq_cpu_put(policy);
	}
	put_online_cpus();

	// extend the timer
	ondemand_timer[cpu_id].jiffies_boost_lasttime = jiffies;
	struct boost_client *bc = NULL;
	if (cpu_freq_in_khz == 0x5566) {
		goto not_change_cpu_freq;

	} else if (cpu_freq_in_khz == 0) {
		if (is_boost_client_running(client_id, cpu_id) == true) {
			del_boost_client(client_id, cpu_id);

			if (is_any_boost_client_running(cpu_id) == true) {
				bc = find_boost_client_with_highest_priority(cpu_id);
				write_echo_calibrating_freq(bc->cpu_freq_in_khz, cpu_id);
				goto change_cpu_freq;
			} else {
#if defined(CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND) || defined(CONFIG_CPU_FREQ_DEFAULT_GOV_INTERACTIVE)
				goto not_change_cpu_freq;
#else
				write_echo_calibrating_freq(1008000, cpu_id);
				goto change_cpu_freq;
#endif
			}
		} else {
			printk(KERN_WARNING "[dvfs_boost] this client is not running\n");
		}
	} else {
		bc = find_boost_client(client_id, cpu_id);
		// check if a client with same client_id already exists
		if (is_boost_client_running(client_id, cpu_id) == true) {
			goto not_change_cpu_freq;
		} else {
			// registered a boost client with client_id and cpu_freq_in_khz
			add_boost_client(client_id, cpu_freq_in_khz, cpu_id, NULL);
			// echo_calibrating_freq = the cpu_freq_in_khz of the client with highest priority
			bc = find_boost_client_with_highest_priority(cpu_id);
			write_echo_calibrating_freq(bc->cpu_freq_in_khz, cpu_id);
			goto change_cpu_freq;
		}
	}

	// something wrong happened
	ret = -1;
	goto not_change_cpu_freq;

change_cpu_freq:
	__CPU_calibrating_proc_write(read_echo_calibrating_freq(cpu_id), cpu_id);
not_change_cpu_freq:
	Mdrv_CpuFreq_UnLock(cpu_id, (char *)__func__);
	return ret;
}


int __CPU_calibrating_proc_write(const unsigned long idx, unsigned int cpu_id)
{
	struct cpufreq_freqs freqs;
	struct cpufreq_policy *policy;
    int i, set_freq = idx;

	if (cpu_id != ondemand_timer[cpu_id].cluster_m)
		return 0;


	if (set_freq > ondemand_timer[cpu_id].sys_max_freq) {
		if (mstar_debug)
			pr_debug("\033[34m%s %d: cpu:%d %dkhz > sys max:%dkhz, set freq to %dkhz\033[m\n",
				__PRETTY_FUNCTION__, __LINE__, cpu_id, set_freq, ondemand_timer[cpu_id].sys_max_freq, ondemand_timer[cpu_id].sys_max_freq);
		set_freq = ondemand_timer[cpu_id].sys_max_freq;
	}

	if (mstar_debug) {
		pr_debug("\033[34m%s %d: set cpu:%d %dkhz\033[m\n", __PRETTY_FUNCTION__, __LINE__, cpu_id, set_freq);
	}
	write_cpufreq_to_RIU(set_freq, cpu_id);
	register_frequency = set_freq;

#if (!defined CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE) && (!defined CONFIG_MP_GLOBAL_TIMER_12MHZ_PATCH)
	mstar_update_sched_clock();
#endif // CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE

	get_online_cpus();
	for_each_online_cpu(i)
	{
        if (ondemand_timer[i].cluster_m == cpu_id)
        {
			ondemand_timer[i].policy->max = set_freq;  // Belong this cluster, we set its max frequency to set_freq.
			ondemand_timer[i].cur_freq = set_freq;
			if (mstar_debug) {
				pr_debug("\033[35m %s %d: cpu_id:%d i:%d policy->max:%d set_freq:%d\033[m\n",
					__PRETTY_FUNCTION__, __LINE__, cpu_id, i,
					ondemand_timer[i].policy->max, set_freq);
			}
        }
	}
	put_online_cpus();

	change_cpus_timer(__FUNCTION__, idx, cpu_id);

#if (!defined CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE) && (!defined CONFIG_MP_GLOBAL_TIMER_12MHZ_PATCH)
	#error "Please fix policy operation in __CPU_calibrating_proc_write. "
	if(policy->cur != idx)
	{
		change_interval(freqs.old, freqs.new);
	}
#endif

	return 0;
}

void write_cpufreq_to_RIU(const unsigned long new_scaling_cur_freq, int cpu_id)
{
#if 1
#if 0
	int m = ondemand_timer[cpu_id].cluster_m;
	reg_writew((new_scaling_cur_freq/1000), ondemand_timer[m].freq_riu);
#endif
#else
    MHalDvfsCpuClockAdjustment((new_scaling_cur_freq/1000), cpu_id);
#endif
}

#ifdef CONFIG_MSTAR_DVFS
void MDrvDvfsVoltageSetup(unsigned int dwCpuClock, unsigned int dwVoltage, unsigned int dwVoltageType, unsigned int dwCpu);
void MDrvDvfsVoltageSetup(unsigned int dwCpuClock, unsigned int dwVoltage, unsigned int dwVoltageType, unsigned int dwCpu)
{
	/* We have to lock a lock here for Multi-Cluster Antutu from Maserati.
	 * This is to prevent T_sensor and Big_Cluster calling MDrvDvfsVoltageSetup()
	 * at the same time, which will cause finished_change_cnt != change_cnt
	 * You can see abc123 1139537, the case is T_sensor doing MHalDvfsPradoMonitor()
	 * from MDrvDvfsQueryCpuClockByTemperature(), and Big_Cluster doing MDrvDvfsVoltageSetup() at the same time

	 * T_sensor can only lock Little_Cluster_Lock, so Little_Cluster will not having any problem.
	 * The reason of why T_sensor doesnot lock Big_Cluster_Lock is due to Multi-Cluster Antutu
	 * will let Big_Cluster too busy to do dvfs_handle, including release Big_Cluster_Lock.
	 */
	long result = 0;
	voltage_change_result[dwCpu] = 0;
    if(start_userspace_ondemand_handshake == 1)
    {
		mutex_lock(&mstar_voltagesetup);
    	ready_to_change_cpufreq[dwCpu] = dwCpuClock;
    	ready_to_change_voltage[dwCpu] = dwVoltage;
    	ready_to_change_voltage_type[dwCpu] = dwVoltageType;
        ready_to_change_cpu[dwCpu] = 1;
    	ready_to_change_cluster_id[dwCpu] = ondemand_timer[dwCpu].cluster;
    	change_cnt[dwCpu]++;

		pr_debug("\033[35m[INFO] Data Exchange Count to User Space: %d\033[m\n", change_cnt[dwCpu]);
		pr_debug("\033[35m[INFO] Voltage: %d\033[m\n", dwVoltage);
		pr_debug("\033[35m[INFO] VoltageType: %d\033[m\n", dwVoltageType);
		pr_debug("\033[35m[INFO] CPU: %d Cluster: %d\033[m\n", dwCpu, ondemand_timer[dwCpu].cluster);

		up(&DVFS_on_demand_event_SEM);

		result = wait_event_timeout(DVFS_on_demand_event_waitqueue_userspace_return,
			finished_change_cnt[dwCpu] == change_cnt[dwCpu] , MAX_SCHEDULE_TIMEOUT);
		if (result == 0) {
			pr_err(" MDrvDvfsVoltageSetup wait_event_timeout expired \n");
			voltage_change_result[dwCpu] = 0;
		}

		mutex_unlock(&mstar_voltagesetup);
    }
    else
	pr_debug("\033[35m[Return] Function = %s, start_userspace_ondemand_handshake is %d\033[m\n", __PRETTY_FUNCTION__, start_userspace_ondemand_handshake);
}
EXPORT_SYMBOL(MDrvDvfsVoltageSetup);
#endif

#if defined(CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND) || defined(CONFIG_CPU_FREQ_DEFAULT_GOV_INTERACTIVE)
static int integrator_set_target_on_demand(struct cpufreq_policy *policy, unsigned int target_freq, unsigned int relation)
{
	int cpu = policy->cpu;
	int cpu_id = get_cpu(); put_cpu();
	cpumask_t cpus_allowed;

#if defined(CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND)
	/* only master CPU of cluster allowed to adjust cpufreq(due to no mutex for now) */
	if( ondemand_timer[cpu_id].cluster_m != cpu_id || cpu != cpu_id)
#else
	/* for interactive mode, the tsk owner cpu will be cpu0, so we only need to check policy->cpu */
	if (ondemand_timer[cpu].cluster_m != cpu)
#endif
		return 0;

	Mdrv_CpuFreq_Lock(cpu_id, (char *)__func__);
	if( (!start_userspace_ondemand_handshake) || (atomic_read(&disable_dvfs) == 1) )
	{
		pr_debug("\033[34mFunction = %s, start_userspace_ondemand_handshake is %d disable_dfvs is %d. Go out, do nothing!!\033[m\n",
                __PRETTY_FUNCTION__, start_userspace_ondemand_handshake, atomic_read(&disable_dvfs));
		goto set_target_out;
	}
	else
	{
		/*
		 * Save this threads cpus_allowed mask.
		 */
        if (mstar_debug) {
			pr_debug("\033[35mFunction = %s, Line = %d, cpu:%d set copufreq from %dKHz to %dKHz\033[m\n",
				__PRETTY_FUNCTION__, __LINE__, cpu, policy->cur, target_freq);
        }
		cpus_allowed = current->cpus_allowed;

		/*
		 * Bind to the specified CPU.  When this call returns,
		 * we should be running on the right CPU.
		 */
		set_cpus_allowed(current, cpumask_of_cpu(cpu_id));
#if defined(CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND)
		/*
		 * For interactive mode, only cpu0 will go into this
		 * set_target driver, so we do not check this
		 */
		BUG_ON(cpu != smp_processor_id());
#endif

		if(target_freq > policy->max)
		{
			printk(KERN_WARNING "\033[31mFunction = %s, Line = %d, adjust target_freq from %dKHz to policy->max %dKHz (over_max case), forcibly_set_target_flag[%d] is %d\033[m\n",
					__PRETTY_FUNCTION__, __LINE__, target_freq, policy->max, cpu_id, forcibly_set_target_flag[cpu_id]);
			target_freq = policy->max;
		}

		if (is_any_boost_client_running(cpu) == true)
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
	Mdrv_CpuFreq_UnLock(cpu_id, (char *)__func__);
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

  	freqs.old = get_freq(); //  johnson

	/* icst_hz_to_vco rounds down -- so we need the next
	 * larger freq in case of CPUFREQ_RELATION_L.
	 */
	if (relation == CPUFREQ_RELATION_L)
		target_freq += 999;
	if (target_freq > policy->max)
		target_freq = policy->max;

	freqs.new = get_freq(); //johnson
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
    int cid = ondemand_timer[cpu_id].cluster;

	if(bootarg_dvfs_disable)
		return;

    /* If cpu_id is not the master cpu of cluster, return it.
     * Example: Cluster 1 includes CPU0 and CPU1. The master CPU of cluster 1 is CPU0.
     * So, we just allow CPU0 pass through and then set target freq.
     * This is for performance issue, because we don't hope each CPU can set target freq.
     */
    if(cpu_id != ondemand_timer[cpu_id].cluster_m)
    {
		pr_debug("\033[35mFunction = %s, cpu:%d, cluster master cpu:%d. Return, do nothing\033[m\n",
			__PRETTY_FUNCTION__, cpu_id, ondemand_timer[cpu_id].cluster_m);
        return;
    }

    /* If target_freq is 54472, we set ac_str_cpufreq to target_freq.
     * It could be came from STR(472) flow.
     */
	if (target_freq == 54472) {
		target_freq = atomic_read(&ondemand_timer[cpu_id].ac_str_cpufreq);
/*
		pr_debug("\033[35mFunction = %s, Line = %d, cpu: %d, set ac_str_cpufreq: %d Khz\033[m\n",
			__PRETTY_FUNCTION__, __LINE__, cpu_id, target_freq);
*/
	}

	//spin_lock(&set_freq_lock);
	pr_debug("\033[36mcaller: %s, cluster: %d target_freq: %d KHz\033[m\n", caller, cid, target_freq);
	ori_target_freq = target_freq;

	get_online_cpus();
	for_each_online_cpu(i)
	{
        if (ondemand_timer[i].cluster == cid) // means the same cluster
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

			/* if "Big Cluster", "antutu is running" and "Not from T_sensor",
				we will not call MDrvDvfsProc(). */
			if ((getCpuCluster(i) != 0) && (find_boost_client(64, i)) &&
				((strcmp(caller, "__CPU_calibrating_proc_write") == 0) ||
				(strcmp(caller, "integrator_set_target_on_demand") == 0)))
				ret[i] = 0;	/* if once here, this time will not do MDrvDvfsProc() */
			else
				ret[i] = 1;	/* this is OK case */

		    ret_value = ret_value * ret[i];
		    cpufreq_cpu_put(other_cpu_policy);
        }
	}
	put_online_cpus();

    if (ret_value)
    {
		/*
		 * set all cpus to new cpu_freq(scaling_cur_freq), and set the timer
		 * always let wait_for_voltage to do voltage_change, adjust target_freq according to Temperature, write adjusted target_freq to RIU, then use new target_freq to set timer, jiffes
		 */

		/* rise voltage first, wait for voltage change(accroding to target_freq) */
        target_freq = MDrvDvfsProc(target_freq, cpu_id);
/*
	pr_debug("\033[35mFunction = %s, Line = %d, cpu %d the target_freq is changed(by MDrvDvfsProc),
	from %d KHz to %d KHz\033[m\n", __PRETTY_FUNCTION__, __LINE__, cpu_id, ori_target_freq, target_freq);
*/

#if (!defined CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE) && (!defined CONFIG_MP_GLOBAL_TIMER_12MHZ_PATCH)
            mstar_update_sched_clock();
#endif // CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE

			get_online_cpus();
			for_each_online_cpu(i)
    		{
                if(cid == ondemand_timer[i].cluster) // means the same cluster.
                {
    			    other_cpu_policy = cpufreq_cpu_get(i);

				    if(!other_cpu_policy)
				    {
					    printk("\033[35mFunction = %s, Line = %d, cpu%d do: [cpu%d] policy is NULL\033[m\n", __PRETTY_FUNCTION__, __LINE__, get_cpu(), i);
					    put_cpu();
					    BUG_ON(!other_cpu_policy);
				    }

				    if(other_cpu_policy->cur == target_freq)
    			    {
    			   	    cpufreq_cpu_put(other_cpu_policy);
    			   	    continue;
    			    }

				    freqs.cpu = i;
    			    freqs.old = other_cpu_policy->cur;
    			    freqs.new = target_freq;

                    if (mstar_debug) {
						pr_debug("\033[31mFunction = %s, Line = %d, cpu%d do: [cpu%d] adjust cpufreq from %d KHZ to %d KHZ\033[m\n",
							__PRETTY_FUNCTION__, __LINE__, get_cpu(), i, freqs.old, freqs.new);
					    put_cpu();
                    }
				    cpufreq_notify_transition(other_cpu_policy, &freqs, CPUFREQ_PRECHANGE);
                    cpufreq_notify_transition(other_cpu_policy, &freqs, CPUFREQ_POSTCHANGE);
    			    cpufreq_cpu_put(other_cpu_policy);

					ondemand_timer[i].cur_freq = target_freq;
                    trace_cpufreq_change(freqs.cpu, freqs.old, freqs.new); //for meansure
				}

			}
			put_online_cpus();

#if (!defined CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE) && (!defined CONFIG_MP_GLOBAL_TIMER_12MHZ_PATCH)
#error "Please fix policy operation in __CPU_calibrating_proc_write. "
			if(other_cpu_policy->cur != target_freq)
			{
				change_interval(freqs.old, freqs.new);
			}
#endif

    }
	//spin_unlock(&set_freq_lock);
}

#if defined(CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND) || defined(CONFIG_CPU_FREQ_DEFAULT_GOV_INTERACTIVE)
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

	return single_open(file, &CPU_calibrating_seq_show, NULL);
}

static int CPU_calibrating_proc_release(struct inode *inode, struct file * file)
{

	WARN_ON(!atomic_read(&proc_is_open));
	atomic_set(&proc_is_open, 0);
	return single_release(inode, file);
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
	start_userspace_ondemand_handshake = 0;

	WARN_ON(!atomic_read(&on_demand_handshake_is_open));
	atomic_set(&on_demand_handshake_is_open, 0);
	return 0;
}

ssize_t on_demand_handshake_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	ON_Demand_From_Userspace from_user_data;
    int i, cpu = 0;

	mutex_lock(&DVFS_on_demand_handshake_write_mutex);
	if(!start_userspace_ondemand_handshake)
	{
		pr_debug("\033[35mFunction = %s, start_userspace_ondemand_handshake is %d\033[m\n", __PRETTY_FUNCTION__, start_userspace_ondemand_handshake);
		mutex_unlock(&DVFS_on_demand_handshake_write_mutex);
		return  -EFAULT;
	}
	else
	{
		if (!count) {
			mutex_unlock(&DVFS_on_demand_handshake_write_mutex);
			return count;
		}

		if(count >= MAX_DMSG_WRITE_BUFFER)
			count = MAX_DMSG_WRITE_BUFFER - 1;

		copy_from_user(&from_user_data, buf, sizeof(from_user_data));

        for (i = 0; i < CONFIG_NR_CPUS; i ++) {
            if (ondemand_timer[i].cluster == from_user_data.from_userspace_cluster_id) {
                ready_to_change_cpu[i] = 0;
                    break;
            }
        }

		if (i == CONFIG_NR_CPUS)
			i--;

		finished_change_cnt[i] = from_user_data.from_userspace_finished_change_cnt;
		voltage_change_result[i] = from_user_data.from_userspace_voltage_change_result;

		if (mstar_debug) {
			pr_debug("\033[33m%s cluster:%d result is %d, cpu:%d, input:%d, change_cnt:%d wake_up wait_queue for write_down\033[m\n",
                __func__, from_user_data.from_userspace_cluster_id, voltage_change_result[i], i, finished_change_cnt[i], change_cnt[i]);
        }
		/* to wake_up a wait_queue waiting for voltage change */
		wake_up(&DVFS_on_demand_event_waitqueue_userspace_return);

		mutex_unlock(&DVFS_on_demand_handshake_write_mutex);
		return count;
	}
}

ssize_t on_demand_handshake_proc_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	ON_Demand_To_Userspace to_userspace_data;
	int err;
    int i, cpu = 0;

	if(!start_userspace_ondemand_handshake)
	{
		if(bootarg_dvfs_disable)
		{
/*
		pr_debug("\033[31mFunction = %s, [Warning] DVFS=disable is set @ bootargs,
		the dvfs_thread will wait forever\033[m\n", __PRETTY_FUNCTION__);
*/
			down_interruptible(&DVFS_disable_SEM);
		}

		printk("\033[35mFunction = %s, start_userspace_ondemand_handshake is %d\033[m\n", __PRETTY_FUNCTION__, start_userspace_ondemand_handshake);
		return  -EFAULT;
	}
	else
	{
		down_interruptible(&DVFS_on_demand_event_SEM);
        for (i = 0; i < CONFIG_NR_CPUS; i ++) {
            if (ready_to_change_cpu[i] == 1) {
                cpu = i;
                break;
            }
        }

        to_userspace_data.to_userspace_cpufreq = ready_to_change_cpufreq[cpu];
        to_userspace_data.to_userspace_voltage = ready_to_change_voltage[cpu];
        to_userspace_data.to_userspace_voltage_type = ready_to_change_voltage_type[cpu];
		to_userspace_data.to_userspace_cluster_id = ready_to_change_cluster_id[cpu];
        to_userspace_data.to_userspace_change_cnt = change_cnt[cpu];

		err = copy_to_user((void *)buf, &to_userspace_data, sizeof(to_userspace_data));

		*ppos += sizeof(to_userspace_data);

		return sizeof(to_userspace_data);
	}
}

static int t_sensor_proc_open(struct inode *inode, struct file *file)
{

	if(atomic_read(&t_sensor_proc_is_open))
		return -EACCES;

	atomic_set(&t_sensor_proc_is_open, 1);

	return 0;
}

static int t_sensor_proc_release(struct inode *inode, struct file * file)
{

	WARN_ON(!atomic_read(&t_sensor_proc_is_open));
	atomic_set(&t_sensor_proc_is_open, 0);
	return 0;
}

ssize_t t_sensor_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	char buffer[MAX_DMSG_WRITE_BUFFER];
	int set;

	if (!count)
		return count;

	if (count >= MAX_DMSG_WRITE_BUFFER)
		count = MAX_DMSG_WRITE_BUFFER - 1;

	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	buffer[count] = '\0';

	if (strict_strtol(buffer, 0, &set) != 0)
    {  //Force change str to decimal conversion (str, base, *converted_num), base is hex, decimal, or ...
       return -EINVAL;
    }

    switch (set)
    {
        case 0:
            bootarg_dvfs_t_sensor_disable = 0;
        break;
        case 1:
            bootarg_dvfs_t_sensor_disable = 1;
        break;
        default:
        break;
    }

	return count;
}

ssize_t t_sensor_proc_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{

    if (mstar_debug) {
        int i;
        for (i = 0; i < CONFIG_NR_CPUS; i ++)
            show_boost_client(i);
	pr_debug("\n");
    }
	pr_debug("T sensor:%s\n", bootarg_dvfs_t_sensor_disable ? "disable" : "enable");

	return 0;
}

const struct file_operations proc_CPU_calibrating_operations = {
	.write      = CPU_calibrating_proc_write,
	.read      = seq_read,
	.llseek      = seq_lseek,
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

const struct file_operations proc_t_sensor_operations = {
	.write      = t_sensor_proc_write,
	.read      = t_sensor_proc_read,
	.open       = t_sensor_proc_open,
	.release    = t_sensor_proc_release,
};

static int dvfs_control_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%s\n", atomic_read(&disable_dvfs) ? "Disable" : "Enable");
	return 0;
};

ssize_t dvfs_control_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	char buffer[MAX_DMSG_WRITE_BUFFER];
	int set;
	if (!count)
		return count;

	if (count >= MAX_DMSG_WRITE_BUFFER)
		count = MAX_DMSG_WRITE_BUFFER - 1;

	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	buffer[count] = '\0';
	/*Force change str to decimal conversion (str, base, *converted_num), base is hex, decimal, or ... */
	if (strict_strtol(buffer, 0, &set) != 0) {
       return -EINVAL;
    }
	printk("dvfs_control_write %d\n", set);
	switch (set) {
	case 0:
		pr_info("Disable dvfs\n");
	#if defined(CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND) || defined(CONFIG_CPU_FREQ_DEFAULT_GOV_INTERACTIVE)
	#if (defined CONFIG_MSTAR_CPU_calibrating) || (defined CONFIG_MSTAR_CPU_CLUSTER_CALIBRATING)
		/* Disable DVFS before suspend */
		Mdrv_CpuFreq_All_Lock((char *)__func__);
		atomic_set(&disable_dvfs, 1);
		pr_info("\033[0;32;31m%s Disable DVFS in STR\033[m\n", __func__);
		Mdrv_CpuFreq_All_UnLock((char *)__func__);
		mdelay(100);
		printk("Change CPU0 clock to 1.1GHz\n");
		MDrvDvfsProc(1100000, 0);
		printk("Change CPU1 clock to 1.1GHz \n");
		MDrvDvfsProc(1100000, 1);
		mdelay(100);
	#endif
	#endif
		break;
	case 1:
		pr_info("Enable dvfs \n");
	#if defined(CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND) || defined(CONFIG_CPU_FREQ_DEFAULT_GOV_INTERACTIVE)
	#if (defined CONFIG_MSTAR_CPU_calibrating) || (defined CONFIG_MSTAR_CPU_CLUSTER_CALIBRATING)
		/* Enable DVFS after resume, this is error case */
		Mdrv_CpuFreq_All_Lock((char *)__func__);
		atomic_set(&disable_dvfs, 0);
		pr_info("\033[0;32;31m%sEnable DVFS\033[m\n", __func__);
		Mdrv_CpuFreq_All_UnLock((char *)__func__);
	#endif
	#endif
			break;
	default:
			break;
    }

	return count;
}


static int dvfs_control_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, dvfs_control_show, NULL);
}

const struct file_operations proc_dvfs_control = {
	.write = dvfs_control_proc_write,
	.read = seq_read,
	.open = dvfs_control_proc_open,
	.release = single_release,

};

#if defined(CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND) || defined(CONFIG_CPU_FREQ_DEFAULT_GOV_INTERACTIVE)
static void Mdrv_CPU_T_sensor_Check_callback(unsigned long value)
{
	int i, j = 0;
	int cpu_id;
	unsigned long echo_calibrating_freq_tmp = 0;
    int max_clock;
	int if_do_change_cpus_timer = 0;

	cpu_id = get_cpu();
	put_cpu();

	if (cpu_id != t_sensor_monitor_owner_cpu) {
		/*
		pr_debug("\033[31mcpu_id is %d, t_sensor_monitor_owner_cpu is %d, not match\033[m\n",
				cpu_id, t_sensor_monitor_owner_cpu);
		*/
		WARN_ON(1);
	}

	/* update boost time, if need */
	for_each_online_cpu(i)
	{
		if (i != ondemand_timer[i].cluster_m)
			continue;

		if (is_any_boost_client_running(i) == true) {
			if (jiffies_to_msecs(jiffies - ondemand_timer[i]
				.jiffies_boost_lasttime) > BOOST_AGING_TIMEOUT_IN_MS) {
				pr_debug("[dvfs boost] timout happens and delete all of the running clients\n");
				del_all_boost_client(i);
#if !defined(CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND) && !defined(CONFIG_CPU_FREQ_DEFAULT_GOV_INTERACTIVE)
				__CPU_calibrating_proc_write(1008000, i);
#endif
		} else {
				/* do nothing */
			}
		} else {
			/* do nothing */
		}
	}
	/* update boost time, if need(done) */

	/* check if we cannot do t_sensort check */
    if (bootarg_dvfs_t_sensor_disable)
        return;

	if(!start_userspace_ondemand_handshake)
	{
#if (MP_PLATFORM_T_SENSOR_OBSERVATION == 0)
		spin_lock(&T_sensor_lock);
		Check_T_sensor_timer.expires = jiffies + FREQ_CHECK_LINK_TIME;
		add_timer(&Check_T_sensor_timer);
		spin_unlock(&T_sensor_lock);
#endif

		pr_debug("\033[34mFunction = %s, [cpu%d] return Mdrv_CPU_T_sensor_Check_callback, start_userspace_ondemand_handshake is %d\033[m\n",
			__PRETTY_FUNCTION__, cpu_id, start_userspace_ondemand_handshake);
		return;
	}
	/* check if we cannot do t_sensort check(done) */

	for_each_online_cpu(i)
	{
		ondemand_timer[i].t_sensor_max_freq = 0;

		if (i == ondemand_timer[i].cluster_m) {
			/*
			pr_debug("\033[35mFunction = %s, Line = %d, query
			MDrvDvfsQueryCpuClockByTemperature
			for cpu %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, i);
			*/
			ondemand_timer[i].t_sensor_max_freq = MDrvDvfsQueryCpuClockByTemperature(i);
			/*
			pr_debug("\033[35mFunction = %s, Line = %d, query
			MDrvDvfsQueryCpuClockByTemperature
			for cpu %d done\033[m\n", __PRETTY_FUNCTION__, __LINE__, i);
			*/
		}
		pr_debug("\033[34mFunction = %s, Line = %d, cpu:%d, T_sensor_max_freq: %d, forcibly_set_target_flag[%d] is %d\033[m\n",
			__PRETTY_FUNCTION__, __LINE__, i, ondemand_timer[i]
			.t_sensor_max_freq, i, forcibly_set_target_flag[i]);
	}

#if (!defined CONFIG_MP_DVFS_FORCE_PINGO_TEST)
	for_each_online_cpu(i)
	{
		if (ondemand_timer[i].t_sensor_max_freq == 0)
			continue;

		max_clock = ondemand_timer[i].policy->cpuinfo.max_freq;

		if (is_any_boost_client_running(i) == true) {
			echo_calibrating_freq_tmp = read_echo_calibrating_freq(i);
			if (ondemand_timer[i].t_sensor_max_freq > echo_calibrating_freq_tmp)
		{
				pr_debug("\033[35mFunction = %s, Line = %d, due to boost, change t_sensor_max_freq from %d to %d\033[m\n",
					__PRETTY_FUNCTION__, __LINE__,
					ondemand_timer[i].t_sensor_max_freq,
					echo_calibrating_freq_tmp);
				ondemand_timer[i].t_sensor_max_freq = echo_calibrating_freq_tmp;
		}
	} else if (ondemand_timer[i].t_sensor_max_freq > max_clock) {
			pr_debug("\033[35mFunction = %s, Line = %d, normal change, change t_sensor_max_freq from %d to %d\033[m\n",
				__PRETTY_FUNCTION__, __LINE__, ondemand_timer[i].t_sensor_max_freq, max_clock);
			ondemand_timer[i].t_sensor_max_freq = max_clock;
		}
	}

	get_online_cpus();
	for_each_online_cpu(i)
	{
		if (ondemand_timer[i].t_sensor_max_freq == 0)
			continue;
		/* this is a special case, only adjust the cpu_voltage only */
		if ((ondemand_timer[i].t_sensor_max_freq == 100000) ||
			(ondemand_timer[i].t_sensor_max_freq == 200000)) {
			ondemand_timer[i].t_sensor_max_freq = ondemand_timer[i].policy->max;
                forcibly_set_target_flag[i] = 5;
            }
        else
		{
			ondemand_timer[i].policy->max = ondemand_timer[i].t_sensor_max_freq;
			/* this is an over-temperature case */
			if (ondemand_timer[i].t_sensor_max_freq < ondemand_timer[i].policy->cur) {
				forcibly_set_target_flag[i] = 20;
/*
pr_debug("\033[31mFunction = %s, Line = %d, T_sensor_max_freq:%d < [%d] current freq:%d,
forcibly_set_target_flag[%d]:%d\033[m\n",
__func__, __LINE__, ondemand_timer[i].t_sensor_max_freq, i
, ondemand_timer[i].policy->cur, i, forcibly_set_target_flag[i]);
*/
				/* This is for big_cluster doing antutu case.
				 * At this case, the big_cluster is very busy, so the dvfs_on_demand will not
				 * work normally.
				 * We then do change_cpus_timer directly to decrease the cpufreq and cpuvoltage
				 */
/*
pr_debug("\033[31mFunction = %s, Line = %d, [Over Temperature Handling Start] changing cpufreq to %d\033[m\n",
__PRETTY_FUNCTION__, __LINE__, ondemand_timer[i].t_sensor_max_freq);
change_cpus_timer(__func__, ondemand_timer[i].t_sensor_max_freq, i);
*/
/*
	pr_debug("\033[31mFunction = %s, Line = %d, [Over Temperature Handling Start]
	changing cpufreq OK!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
*/
				ondemand_timer[i].over_temperature_mode = 1;
				if_do_change_cpus_timer = 1;
                }
                else
                {
					if (forcibly_set_target_flag[i] > 0)
						forcibly_set_target_flag[i]--;

				if ((ondemand_timer[i].over_temperature_mode) &&
					(ondemand_timer[i].t_sensor_max_freq > ondemand_timer[i].policy->cur)) {
					/*
					pr_debug("\033[31mFunction = %s, Line = %d, [Over Temperature
					Handling End] changing cpufreq to %d\033[m\n",
					__PRETTY_FUNCTION__, __LINE__, ondemand_timer[i].t_sensor_max_freq);
					change_cpus_timer(__func__, ondemand_timer[i].t_sensor_max_freq, i);
					*/
					/*
					pr_debug("\033[31mFunction = %s, Line = %d, [Over Temperature Handling
					End] changing cpufreq OK!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
					*/
					ondemand_timer[i].over_temperature_mode = 0;
					if_do_change_cpus_timer = 1;
				}
                }

			if ((getCpuCluster(i) != 0) && (find_boost_client(64, i)) && (if_do_change_cpus_timer == 0)) {
				/*
				pr_debug("\033[31mFunction = %s, Line = %d, [Big Cluster Antutu]
				changing cpufreq to %d\033[m\n",
				__PRETTY_FUNCTION__, __LINE__, ondemand_timer[i].t_sensor_max_freq);
				change_cpus_timer(__func__, ondemand_timer[i].t_sensor_max_freq, i);
				*/
				/*
				pr_debug("\033[31mFunction = %s, Line = %d, [Big Cluster
				Antutu] changing cpufreq OK!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
				*/
            }
        }
	}
	put_online_cpus();
#endif

#if (MP_PLATFORM_T_SENSOR_OBSERVATION == 0)
	spin_lock(&T_sensor_lock);
	Check_T_sensor_timer.expires = jiffies + FREQ_CHECK_LINK_TIME;
	add_timer(&Check_T_sensor_timer);
	spin_unlock(&T_sensor_lock);
#endif
}
#else
static void Mdrv_CPU_Freq_Check_callback(unsigned long value)
{
	int freq, i;
	struct cpufreq_freqs freqs;
	struct cpufreq_policy *policy;
	int cpu_id = get_cpu(); put_cpu();
    int min_clock, max_clock;

    min_clock = CONFIG_DVFS_CPU_CLOCK_MIN(cpu_id);
    max_clock = CONFIG_DVFS_CPU_CLOCK_MAX(cpu_id);

	if(*(volatile u32 *)(0xfd200a00) == 0x3697)
   	{
   		freq = *(volatile u32 *)(0xfd200a04);
   		freq = freq * 1000;

   		if (freq != current_frequency)
   		{
#if (!defined CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE) && (!defined CONFIG_MP_GLOBAL_TIMER_12MHZ_PATCH)
			mstar_update_sched_clock();
#endif // CONFIG_MP_STATIC_TIMER_CLOCK_SOURCE
			pr_debug("\033[36m\nFunction = %s, Line = %d, (freq = %d KHZ) != (current_frequency = %d KHZ)\033[m\n",
				__PRETTY_FUNCTION__, __LINE__, freq, current_frequency);
			if(freq < min_clock)
			{
					printk(KERN_WARNING "\033[36m\n freq %d KHZ < MIN_CPU_FREQ %d KHZ ,not allowed\033[m\n", freq, min_clock);
					return;
			}

			if(freq > max_clock)
			{
					printk(KERN_WARNING "\033[36m\n freq %d KHZ > MAX_CPU_FREQ %d KHZ ,not allowed\033[m\n", freq, max_clock);
					return;
			}

			register_frequency = freq;

			get_online_cpus();
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
			put_online_cpus();

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
    int i, j;
	int cpu = get_cpu();
	put_cpu();
	struct cpufreq_frequency_table *freq_table;

	memset(&ondemand_timer, 0, sizeof(ondemand_timer));

	get_online_cpus();
	for_each_online_cpu(i)
    {
	    ondemand_timer[i].cur_freq = query_frequency(i) * 1000;
	    ondemand_timer[i].sys_max_freq = CONFIG_DVFS_CPU_CLOCK_MAX(i);
        freq_table = kzalloc(sizeof(struct cpufreq_frequency_table) * DVFS_FREQ_LEVEL_MAX_INDEX, GFP_KERNEL);
        if (!freq_table) {
            printk("\033[35m%s: Unable to allocate frequency table \033[m\n", __PRETTY_FUNCTION__);
			put_online_cpus();
		    return -ENOMEM;
        }
        for (j = 0; j < DVFS_FREQ_LEVEL_MAX_INDEX; j ++) {
            freq_table[j].frequency = hal_freq_table[j].frequency;  //hal_freq_table is in chip_dvfs_calibrating.h
            freq_table[j].index = hal_freq_table[j].index;
        }
        cpufreq_frequency_table_get_attr(&freq_table[0], i);

	    pr_debug("\033[35mFunction = %s, [cpu %d] set cpu_%d current freq = %d\033[m\n",
			__PRETTY_FUNCTION__, cpu, i, ondemand_timer[i].cur_freq);
    }
	put_online_cpus();

	cpufreq_register_driver(&integrator_driver);
	proc_create("CPU_calibrating", S_IRUSR | S_IWUSR, NULL, &proc_CPU_calibrating_operations);
	proc_create("on_demand_ctl"  , S_IRUSR | S_IWUSR, NULL, &proc_on_demand_handshake_operations);
	proc_create("t_sensor",  S_IWUSR | S_IWGRP | S_IWOTH, NULL, &proc_t_sensor_operations);
	proc_create("dvfs_control", S_IWUSR | S_IWGRP | S_IWOTH, NULL, &proc_dvfs_control);
	sema_init(&DVFS_on_demand_event_SEM, 0);

#if !defined(CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND) && !defined(CONFIG_CPU_FREQ_DEFAULT_GOV_INTERACTIVE)
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
	int cpu_idx = get_cpu();
	pr_debug("\033[35mFunction = %s, Line = %d, [cpu %d] do CPU_calibrating_init\033[m\n", __PRETTY_FUNCTION__, __LINE__, cpu_idx);
	put_cpu();

	init_procfs_msg();
	return 0;
}

static int __init DVFS_enable(char *str)
{
    if(strcmp(str, "disable") == 0)
    {
	pr_debug("\nDVFS_disable\n");
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

module_param(mstar_debug, uint, 0644);
MODULE_PARM_DESC(mstar_debug, "Debug for dvfs");
module_param(mstar_info, uint, 0644);
MODULE_PARM_DESC(mstar_info, "Info for dvfs");
