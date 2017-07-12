#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cpufreq.h>
#include <linux/cpu.h>
#include <linux/jiffies.h>
#include <linux/kernel_stat.h>
#include <linux/mutex.h>
#include <linux/hrtimer.h>
#include <linux/tick.h>
#include <linux/ktime.h>
#include <linux/sched.h>
#include <linux/workqueue.h>
#include <linux/thermal.h>
#include <linux/limits.h>

/* PID_governor macros */
#define DEF_E_VALUE                 (-1)
#define DEF_F_VALUE                  (0)
#define DEF_A_VALUE              (50000)
#define DEF_B_VALUE             (-49722)
#define DEF_C_VALUE                  (0)
#define DEF_TEMP_OBJ             (85000)
#define DEF_SAMPLING_VALUE     (3000000)

/*
 * Struct of data for each CPU
 */
struct cpu_dbs_info_s {
	int error1;
	int error2;
	int u1;
	int u2;
	struct cpufreq_policy *cur_policy;
	struct delayed_work work;
	unsigned int requested_freq;
	int cpu;
	unsigned int enable:1;
};
static DEFINE_PER_CPU(struct cpu_dbs_info_s, cs_cpu_dbs_info);


/****** Global data *******/

// dbs_mutex to protect data access.
static DEFINE_MUTEX(dbs_mutex);

static struct dbs_tuners {
	unsigned int sampling_rate;
	unsigned int E_value;
	unsigned int F_value;
	unsigned int A_value;
	unsigned int B_value;
	unsigned int C_value;
	unsigned int temp_obj;
} dbs_tuners_ins = {
	.sampling_rate = DEF_SAMPLING_VALUE,
	.E_value = DEF_E_VALUE,
	.F_value = DEF_F_VALUE,
	.A_value = DEF_A_VALUE,
	.B_value = DEF_B_VALUE,
	.C_value = DEF_C_VALUE,
	.temp_obj = DEF_TEMP_OBJ,
};

static struct thermal_zone_device *tz; 
static unsigned int dbs_enable;


/********************* PID controler *********************/
static void dbs_check_cpu(struct cpu_dbs_info_s *this_dbs_info)
{
	int E, F, A, B, C, error, u, e1, e2, u1, u2, temp_ac;
	long long int acum;
	struct cpufreq_policy *policy;

	policy = this_dbs_info->cur_policy;
	
	thermal_zone_get_temp(tz, &temp_ac);

	e1 = this_dbs_info->error1;
	e2 = this_dbs_info->error2;
	u1 = this_dbs_info->u1;
	u2 = this_dbs_info->u2;

	mutex_lock(&dbs_mutex);
	E = dbs_tuners_ins.E_value;
	F = dbs_tuners_ins.F_value;
	A = dbs_tuners_ins.A_value;
	B = dbs_tuners_ins.B_value;
	C = dbs_tuners_ins.C_value;
	error = dbs_tuners_ins.temp_obj - temp_ac;
	mutex_unlock(&dbs_mutex);

	acum = -E * u1;
	acum += -F * u2;
	acum += A * error;
	acum += B * e1;
	acum += C * e2;

	// Check for overflow
	if ( acum < INT_MIN ) {
		u = INT_MIN;
	} else if ( acum > INT_MAX ) {
		u = INT_MAX;
	} else {
		u = (int) acum;
	}

	printk("Temp: %d Freq: %d\n", temp_ac, u);

	this_dbs_info->error1 = error;
	this_dbs_info->error2 = e1;
	this_dbs_info->u1 = u;
	this_dbs_info->u2 = u1;
	
	__cpufreq_driver_target(policy, u, CPUFREQ_RELATION_C);
}


/******************* Working queues *******************/
static void do_dbs_timer(struct work_struct *work)
{
	int delay;
	struct cpu_dbs_info_s *dbs_info =
		container_of(work, struct cpu_dbs_info_s, work.work);
	unsigned int cpu = dbs_info->cpu;

	/* We want all CPUs to do sampling nearly on same jiffy */
	mutex_lock(&dbs_mutex);
	delay = usecs_to_jiffies(dbs_tuners_ins.sampling_rate);
	mutex_unlock(&dbs_mutex);

	delay -= jiffies % delay;

	dbs_check_cpu(dbs_info);

	schedule_delayed_work_on(cpu, &dbs_info->work, delay);
}

static inline void dbs_timer_init(struct cpu_dbs_info_s *dbs_info)
{
	/* We want all CPUs to do sampling nearly on same jiffy */
	int delay = usecs_to_jiffies(dbs_tuners_ins.sampling_rate);
	delay -= jiffies % delay;

	dbs_info->enable = 1;
	INIT_DEFERRABLE_WORK(&dbs_info->work, do_dbs_timer);
	schedule_delayed_work_on(dbs_info->cpu, &dbs_info->work, delay);
}

static inline void dbs_timer_exit(struct cpu_dbs_info_s *dbs_info)
{
	dbs_info->enable = 0;
	cancel_delayed_work_sync(&dbs_info->work);
}


/************************** sysfs interface ************************/

#define show_one(file_name, object)					\
static ssize_t show_##file_name						\
(struct kobject *kobj, struct attribute *attr, char *buf)		\
{									\
	return sprintf(buf, "%u\n", dbs_tuners_ins.object);		\
}

show_one(sampling_rate, sampling_rate);
show_one(E_value, E_value);
show_one(F_value, F_value);
show_one(A_value, A_value);
show_one(B_value, B_value);
show_one(C_value, C_value);
show_one(temp_obj, temp_obj);

static ssize_t store_sampling_rate(struct kobject *a, struct attribute *b,
				   const char *buf, size_t count)
{
	unsigned int input;
	int ret;
	ret = sscanf(buf, "%u", &input);
	if (ret != 1)
		return -EINVAL;

	mutex_lock(&dbs_mutex);
	dbs_tuners_ins.sampling_rate = input;
	mutex_unlock(&dbs_mutex);

	return count;
}

static ssize_t store_E_value(struct kobject *a, struct attribute *b,
				   const char *buf, size_t count)
{
	int input;
	int ret;
	ret = sscanf(buf, "%d", &input);

	if(ret != 1) return -EINVAL;

	mutex_lock(&dbs_mutex);
	dbs_tuners_ins.E_value = input;
	mutex_unlock(&dbs_mutex);

	return count;
}

static ssize_t store_F_value(struct kobject *a, struct attribute *b,
				   const char *buf, size_t count)
{
	int input;
	int ret;
	ret = sscanf(buf, "%d", &input);

	if(ret != 1) return -EINVAL;

	mutex_lock(&dbs_mutex);
	dbs_tuners_ins.F_value = input;
	mutex_unlock(&dbs_mutex);

	return count;
}
static ssize_t store_A_value(struct kobject *a, struct attribute *b,
				   const char *buf, size_t count)
{
	int input;
	int ret;
	ret = sscanf(buf, "%d", &input);

	if(ret != 1) return -EINVAL;

	mutex_lock(&dbs_mutex);
	dbs_tuners_ins.A_value = input;
	mutex_unlock(&dbs_mutex);

	return count;
}

static ssize_t store_B_value(struct kobject *a, struct attribute *b,
				   const char *buf, size_t count)
{
	int input;
	int ret;
	ret = sscanf(buf, "%d", &input);

	if(ret != 1) return -EINVAL;

	mutex_lock(&dbs_mutex);
	dbs_tuners_ins.B_value = input;
	mutex_unlock(&dbs_mutex);

	return count;
}

static ssize_t store_C_value(struct kobject *a, struct attribute *b,
				   const char *buf, size_t count)
{
	int input;
	int ret;
	ret = sscanf(buf, "%d", &input);

	if(ret != 1) return -EINVAL;

	mutex_lock(&dbs_mutex);
	dbs_tuners_ins.C_value = input;
	mutex_unlock(&dbs_mutex);

	return count;
}

static ssize_t store_temp_obj(struct kobject *a, struct attribute *b,
				   const char *buf, size_t count)
{
	int input;
	int ret;
	ret = sscanf(buf, "%d", &input);

	if(ret != 1) return -EINVAL;

	mutex_lock(&dbs_mutex);
	dbs_tuners_ins.temp_obj = input;
	mutex_unlock(&dbs_mutex);

	return count;
}


define_one_global_rw(sampling_rate);
define_one_global_rw(E_value);
define_one_global_rw(F_value);
define_one_global_rw(A_value);
define_one_global_rw(B_value);
define_one_global_rw(C_value);
define_one_global_rw(temp_obj);

static struct attribute *dbs_attributes[] = {
	&sampling_rate.attr,
	&E_value.attr,
	&F_value.attr,
	&A_value.attr,
	&B_value.attr,
	&C_value.attr,
	&temp_obj.attr,
	NULL
};

static struct attribute_group dbs_attr_group = {
	.attrs = dbs_attributes,
	.name = "PID_governor",
};

/********************* end sysfs ******************************/

/**************** Governor *****************/

static int cpufreq_governor_dbs(struct cpufreq_policy *policy,
				   unsigned int event)
{
	unsigned int cpu = policy->cpu;
	struct cpu_dbs_info_s *this_dbs_info;
	unsigned int j;
	int rc;

	this_dbs_info = &per_cpu(cs_cpu_dbs_info, cpu);

	switch (event) {
	case CPUFREQ_GOV_START:
		if ((!cpu_online(cpu)) || (!policy->cur))
			return -EINVAL;

		mutex_lock(&dbs_mutex);

		for_each_cpu(j, policy->cpus) {
			struct cpu_dbs_info_s *j_dbs_info;
			j_dbs_info = &per_cpu(cs_cpu_dbs_info, j);
			j_dbs_info->cur_policy = policy;
			j_dbs_info->error1 = 0;
			j_dbs_info->error2 = 0;
			j_dbs_info->u1 = 0;
			j_dbs_info->u2 = 0;
		}

		dbs_enable++;
		/*
		 * Start the timerschedule work, when this governor
		 * is used for first time
		 */
		if (dbs_enable == 1) {
			rc = sysfs_create_group(cpufreq_global_kobject,
						&dbs_attr_group);
			if (rc) {
				mutex_unlock(&dbs_mutex);
				return rc;
			}

		}
		mutex_unlock(&dbs_mutex);

		dbs_timer_init(this_dbs_info);

		break;

	case CPUFREQ_GOV_STOP:
		dbs_timer_exit(this_dbs_info);

		mutex_lock(&dbs_mutex);
		dbs_enable--;


		mutex_unlock(&dbs_mutex);
		if (!dbs_enable)
			sysfs_remove_group(cpufreq_global_kobject,
					   &dbs_attr_group);

		break;
	}
	return 0;
}


#ifndef CONFIG_CPU_FREQ_DEFAULT_GOV_PIDGOV
static
#endif
struct cpufreq_governor cpufreq_gov_pid = {
	.name			= "PID_GOVERNOR",
	.governor		= cpufreq_governor_dbs,
	.owner			= THIS_MODULE,
};

static int __init cpufreq_gov_dbs_init(void)
{
	const char *name = "cpu-thermal0";
	tz = thermal_zone_get_zone_by_name(name);
	if (tz == NULL) return 0;
	return cpufreq_register_governor(&cpufreq_gov_pid);
}

static void __exit cpufreq_gov_dbs_exit(void)
{
	cpufreq_unregister_governor(&cpufreq_gov_pid);
}

MODULE_AUTHOR("Pablo Hernandez <pabloheralm@gmail.com>");
MODULE_DESCRIPTION("PID_governor - A PID governor to keep"
	"the same temperature");
MODULE_LICENSE("GPL");

#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_PIDGOV
fs_initcall(cpufreq_gov_dbs_init);
#else
module_init(cpufreq_gov_dbs_init);
#endif
module_exit(cpufreq_gov_dbs_exit);
