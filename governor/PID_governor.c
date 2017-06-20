
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/cpu.h>
#include <linux/percpu-defs.h>
#include <linux/slab.h>
#include <linux/tick.h>
#include "cpufreq_governor.h"
#include <linux/thermal.h>
#include "../thermal/thermal_core.h"


/* PID_governor macros */
#define DEF_E_VALUE		(-1)
#define DEF_F_VALUE		(0)
#define DEF_A_VALUE		(50000)
#define DEF_B_VALUE		(-49722)
#define DEF_C_VALUE		(0)
#define DEF_TEMP_OBJ		(85000)
#define DEF_SAMPLING_VALUE		(3000000)

static DEFINE_MUTEX(access_variables);
static DEFINE_MUTEX(access_temp);
static int temp=0;

static DEFINE_PER_CPU(struct od_cpu_dbs_info_s, od_cpu_dbs_info);

//static struct od_ops od_ops;

struct pid_dbs_tuners {
	unsigned int sampling_rate;
	int E_value;
	int F_value;
	int A_value;
	int B_value;
	int C_value;
	int temp_obj;
};

#ifndef CONFIG_CPU_FREQ_DEFAULT_GOV_PIDGOV
static struct cpufreq_governor cpufreq_gov_pid;
#endif

/*
 * Not all CPUs want IO time to be accounted as busy; this depends on how
 * efficient idling at a higher frequency/voltage is.
 * Pavel Machek says this is not so for various generations of AMD and old
 * Intel systems.
 * Mike Chan (android.com) claims this is also not true for ARM.
 * Because of this, whitelist specific known (series) of CPUs by default, and
 * leave all others up to the user.
 */
static int should_io_be_busy(void)
{
#if defined(CONFIG_X86)
	/*
	 * For Intel, Core 2 (model 15) and later have an efficient idle.
	 */
	if (boot_cpu_data.x86_vendor == X86_VENDOR_INTEL &&
			boot_cpu_data.x86 == 6 &&
			boot_cpu_data.x86_model >= 15)
		return 1;
#endif
	return 0;
}

static int update_temp(struct thermal_zone_device *tz, int trip)
{
	mutex_lock(&access_temp);
	temp=tz->temperature;
	mutex_unlock(&access_temp);
	return 0;
}

/*
 * Every sampling_rate, we check, if current idle time is less than 20%
 * (default), then we try to increase frequency. Else, we adjust the frequency
 * proportional to load.
 */
static void pid_update(int cpu)
{
	// Global variables
	static int u1 = 0;
	static int u2 = 0;
	static int e1 = 0;
	static int e2 = 0;
	int E, F, A, B, C, error, u;

	struct od_cpu_dbs_info_s *dbs_info = &per_cpu(od_cpu_dbs_info, cpu);
	struct cpufreq_policy *policy = dbs_info->cdbs.shared->policy;
	struct dbs_data *dbs_data = policy->governor_data;
	struct pid_dbs_tuners *pid_tuners = dbs_data->tuners;

	int temp_ac=0;
	mutex_lock(&access_temp);
	temp_ac=temp;
	mutex_unlock(&access_temp);

	mutex_lock(&access_variables);
	E = pid_tuners->E_value;
	F = pid_tuners->F_value;
	A = pid_tuners->A_value;
	B = pid_tuners->B_value;
	C = pid_tuners->C_value;
	error = pid_tuners->temp_obj - temp_ac; // OJO!! Obtener temperatura
	mutex_unlock(&access_variables);

	u = -E * u1 - F * u2 + A * error + B * e1 + C * e2;

	u2 = u1;
	u1 = u;
	e2 = e1;
	e1 = error;
	
	__cpufreq_driver_target(policy, u, CPUFREQ_RELATION_C);
}

static unsigned int pid_dbs_timer(struct cpu_dbs_info *cdbs,
				 struct dbs_data *dbs_data, bool modify_all)
{
	struct cpufreq_policy *policy = cdbs->shared->policy;
	unsigned int cpu = policy->cpu;
	struct od_cpu_dbs_info_s *dbs_info = &per_cpu(od_cpu_dbs_info,
			cpu);
	struct pid_dbs_tuners *od_tuners = dbs_data->tuners;
	int delay = 0, sample_type = dbs_info->sample_type;

	if (!modify_all)
		goto max_delay;

	pid_update(cpu);

max_delay:
	if (!delay)
		delay = delay_for_sampling_rate(od_tuners->sampling_rate
				* dbs_info->rate_mult);

	return delay;
}

/************************** sysfs interface ************************/
static struct common_dbs_data pid_dbs_cdata;

static ssize_t store_sampling_rate(struct dbs_data *dbs_data, const char *buf,
		size_t count)
{
	struct pid_dbs_tuners *pid_tuners = dbs_data->tuners;
	unsigned int input;
	int ret;
	ret = sscanf(buf, "%u", &input);
	if (ret != 1)
		return -EINVAL;

	pid_tuners->sampling_rate = input;

	return count;
}

static ssize_t store_E_value(struct dbs_data *dbs_data, const char *buf,
		size_t count)
{
	struct pid_dbs_tuners *pid_tuners = dbs_data->tuners;
	int input;
	int ret;
	ret = sscanf(buf, "%d", &input);

	if(ret != 1) return -EINVAL;

	mutex_lock(&access_variables);
	pid_tuners->E_value = input;
	mutex_unlock(&access_variables);

	return count;
}

static ssize_t store_F_value(struct dbs_data *dbs_data, const char *buf,
		size_t count)
{
	struct pid_dbs_tuners *pid_tuners = dbs_data->tuners;
	int input;
	int ret;
	ret = sscanf(buf, "%d", &input);

	if(ret != 1) return -EINVAL;

	mutex_lock(&access_variables);
	pid_tuners->F_value = input;
	mutex_unlock(&access_variables);

	return count;
}
static ssize_t store_A_value(struct dbs_data *dbs_data, const char *buf,
		size_t count)
{
	struct pid_dbs_tuners *pid_tuners = dbs_data->tuners;
	int input;
	int ret;
	ret = sscanf(buf, "%d", &input);

	if(ret != 1) return -EINVAL;

	mutex_lock(&access_variables);
	pid_tuners->A_value = input;
	mutex_unlock(&access_variables);

	return count;
}

static ssize_t store_B_value(struct dbs_data *dbs_data, const char *buf,
		size_t count)
{
	struct pid_dbs_tuners *pid_tuners = dbs_data->tuners;
	int input;
	int ret;
	ret = sscanf(buf, "%d", &input);

	if(ret != 1) return -EINVAL;

	mutex_lock(&access_variables);
	pid_tuners->B_value = input;
	mutex_unlock(&access_variables);

	return count;
}

static ssize_t store_C_value(struct dbs_data *dbs_data, const char *buf,
		size_t count)
{
	struct pid_dbs_tuners *pid_tuners = dbs_data->tuners;
	int input;
	int ret;
	ret = sscanf(buf, "%d", &input);

	if(ret != 1) return -EINVAL;

	mutex_lock(&access_variables);
	pid_tuners->C_value = input;
	mutex_unlock(&access_variables);

	return count;
}

static ssize_t store_temp_obj(struct dbs_data *dbs_data, const char *buf,
		size_t count)
{
	struct pid_dbs_tuners *pid_tuners = dbs_data->tuners;
	int input;
	int ret;
	ret = sscanf(buf, "%d", &input);

	if(ret != 1) return -EINVAL;

	mutex_lock(&access_variables);
	pid_tuners->temp_obj = input;
	mutex_unlock(&access_variables);

	return count;
}


show_store_one(pid, sampling_rate);
show_store_one(pid, E_value);
show_store_one(pid, F_value);
show_store_one(pid, A_value);
show_store_one(pid, B_value);
show_store_one(pid, C_value);
show_store_one(pid, temp_obj);

gov_sys_pol_attr_rw(sampling_rate);
gov_sys_pol_attr_rw(E_value);
gov_sys_pol_attr_rw(F_value);
gov_sys_pol_attr_rw(A_value);
gov_sys_pol_attr_rw(B_value);
gov_sys_pol_attr_rw(C_value);
gov_sys_pol_attr_rw(temp_obj);


static struct attribute *dbs_attributes_gov_sys[] = {
	&sampling_rate_gov_sys.attr,
	&E_value_gov_sys.attr,
	&F_value_gov_sys.attr,
	&A_value_gov_sys.attr,
	&B_value_gov_sys.attr,
	&C_value_gov_sys.attr,
	&temp_obj_gov_sys.attr,
	NULL
};

static struct attribute_group pid_attr_group_gov_sys = {
	.attrs = dbs_attributes_gov_sys,
	.name = "PID_governor",
};

static struct attribute *dbs_attributes_gov_pol[] = {
	&sampling_rate_gov_pol.attr,
	&E_value_gov_pol.attr,
	&F_value_gov_pol.attr,
	&A_value_gov_pol.attr,
	&B_value_gov_pol.attr,
	&C_value_gov_pol.attr,
	&temp_obj_gov_pol.attr,
	NULL
};

static struct attribute_group pid_attr_group_gov_pol = {
	.attrs = dbs_attributes_gov_pol,
	.name = "PID_governor",
};

/************************** sysfs end ************************/

static int pid_init(struct dbs_data *dbs_data, bool notify)
{
	struct pid_dbs_tuners *tuners;
	u64 idle_time;
	int cpu;

	tuners = kzalloc(sizeof(*tuners), GFP_KERNEL);
	if (!tuners) {
		pr_err("%s: kzalloc failed\n", __func__);
		return -ENOMEM;
	}

	tuners->E_value = DEF_E_VALUE;
	tuners->F_value = DEF_F_VALUE;
	tuners->A_value = DEF_A_VALUE;
	tuners->B_value = DEF_B_VALUE;
	tuners->C_value = DEF_C_VALUE;
	tuners->temp_obj = DEF_TEMP_OBJ;
	tuners->sampling_rate = DEF_SAMPLING_VALUE;

	dbs_data->tuners = tuners;
	return 0;
}

static void pid_exit(struct dbs_data *dbs_data, bool notify)
{
	kfree(dbs_data->tuners);
}

define_get_cpu_dbs_routines(od_cpu_dbs_info);

static struct od_ops od_ops = {
	.powersave_bias_init_cpu = NULL,
	.powersave_bias_target = NULL,
	.freq_increase = NULL,
};

#define PID_GOVERNOR		2
static struct common_dbs_data pid_dbs_cdata = {
	.governor = PID_GOVERNOR,
	.attr_group_gov_sys = &pid_attr_group_gov_sys,
	.attr_group_gov_pol = &pid_attr_group_gov_pol,
	.get_cpu_cdbs = get_cpu_cdbs,
	.get_cpu_dbs_info_s = get_cpu_dbs_info_s,
	.gov_dbs_timer = pid_dbs_timer,
	.gov_check_cpu = NULL,
	.gov_ops = &od_ops,
	.init = pid_init,
	.exit = pid_exit,
	.mutex = __MUTEX_INITIALIZER(pid_dbs_cdata.mutex),
};

static int pid_cpufreq_governor_dbs(struct cpufreq_policy *policy,
		unsigned int event)
{
	return cpufreq_governor_dbs(policy, &pid_dbs_cdata, event);
}

#ifndef CONFIG_CPU_FREQ_DEFAULT_GOV_PIDGOV
static
#endif
struct cpufreq_governor cpufreq_gov_pid = {
	.name			= "PID_GOVERNOR",
	.governor		= pid_cpufreq_governor_dbs,
	.owner			= THIS_MODULE,
};

static struct thermal_governor thermal_gov_user_space = {
	.name		= "user_space",
	.throttle	= update_temp,
};

static int __init cpufreq_gov_dbs_init(void)
{
	if(thermal_register_governor(&thermal_gov_user_space)) return 0;
	return cpufreq_register_governor(&cpufreq_gov_pid);
}

static void __exit cpufreq_gov_dbs_exit(void)
{
	thermal_unregister_governor(&thermal_gov_user_space);
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