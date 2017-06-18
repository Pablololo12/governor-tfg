
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/printk.h> // For debug
#include <linux/cpufreq.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include "cpufreq_governor.h"
#include <linux/thermal.h>
#include "../thermal/thermal_core.h"

struct pid_dbs_tuners {
	int E_value;
	int F_value;
	int A_value;
	int B_value;
	int C_value;
	int temp_obj;
};

static DEFINE_MUTEX(access_variables);
static DEFINE_MUTEX(access_temp);
static int temp=0;

static int update_temp(struct thermal_zone_device *tz, int trip)
{
	mutex_lock(&access_temp);
	temp=tz->temperature;
	mutex_unlock(&access_temp);
	return 0;
}

static unsigned int pid_update(struct cpufreq_policy *policy)
{
	// Global variables
	static int u1 = 0;
	static int u2 = 0;
	static int e1 = 0;
	static int e2 = 0;

	struct policy_dbs_info *policy_dbs = policy->governor_data;
	struct dbs_data *dbs_data = policy_dbs->dbs_data;
	struct pid_dbs_tuners *pid_tuners = dbs_data->tuners;

	int temp_ac=0;
	mutex_lock(&access_temp);
	temp_ac=temp;
	mutex_unlock(&access_temp);

	mutex_lock(&access_variables);
	int E = pid_tuners->E_value;
	int F = pid_tuners->F_value;
	int A = pid_tuners->A_value;
	int B = pid_tuners->B_value;
	int C = pid_tuners->C_value;
	int error = pid_tuners->temp_obj - temp_ac; // OJO!! Obtener temperatura
	unsigned int sampling = dbs_data->sampling_rate;
	mutex_unlock(&access_variables);

	int u = -E * u1 - F * u2 + A * error + B * e1 + C * e2;

	u2 = u1;
	u1 = u;
	e2 = e1;
	e1 = error;
	
	__cpufreq_driver_target(policy, u, CPUFREQ_RELATION_C);

	return sampling;
}


/************* sysfs *****************************************/

static ssize_t store_E_value(struct gov_attr_set *attr_set,
				const char *buf, size_t count)
{
	struct dbs_data *dbs_data = to_dbs_data(attr_set);
	struct pid_dbs_tuners *pid_tuners = dbs_data->tuners;
	int input;
	int ret;
	ret = sscanf(buf, "%d", &input);

	if(ret == NULL) return -EINVAL;

	mutex_lock(&access_variables);
	pid_tuners->E_value = input;
	mutex_unlock(&access_variables);

	return count;
}

static ssize_t store_F_value(struct gov_attr_set *attr_set,
				const char *buf, size_t count)
{
	struct dbs_data *dbs_data = to_dbs_data(attr_set);
	struct pid_dbs_tuners *pid_tuners = dbs_data->tuners;
	int input;
	int ret;
	ret = sscanf(buf, "%d", &input);

	if(ret == NULL) return -EINVAL;

	mutex_lock(&access_variables);
	pid_tuners->F_value = input;
	mutex_unlock(&access_variables);

	return count;
}
static ssize_t store_A_value(struct gov_attr_set *attr_set,
				const char *buf, size_t count)
{
	struct dbs_data *dbs_data = to_dbs_data(attr_set);
	struct pid_dbs_tuners *pid_tuners = dbs_data->tuners;
	int input;
	int ret;
	ret = sscanf(buf, "%d", &input);

	if(ret == NULL) return -EINVAL;

	mutex_lock(&access_variables);
	pid_tuners->A_value = input;
	mutex_unlock(&access_variables);

	return count;
}

static ssize_t store_B_value(struct gov_attr_set *attr_set,
				const char *buf, size_t count)
{
	struct dbs_data *dbs_data = to_dbs_data(attr_set);
	struct pid_dbs_tuners *pid_tuners = dbs_data->tuners;
	int input;
	int ret;
	ret = sscanf(buf, "%d", &input);

	if(ret == NULL) return -EINVAL;

	mutex_lock(&access_variables);
	pid_tuners->B_value = input;
	mutex_unlock(&access_variables);

	return count;
}

static ssize_t store_C_value(struct gov_attr_set *attr_set,
				const char *buf, size_t count)
{
	struct dbs_data *dbs_data = to_dbs_data(attr_set);
	struct pid_dbs_tuners *pid_tuners = dbs_data->tuners;
	int input;
	int ret;
	ret = sscanf(buf, "%d", &input);

	if(ret == NULL) return -EINVAL;

	mutex_lock(&access_variables);
	pid_tuners->C_value = input;
	mutex_unlock(&access_variables);

	return count;
}

static ssize_t store_temp_obj(struct gov_attr_set *attr_set,
				const char *buf, size_t count)
{
	struct dbs_data *dbs_data = to_dbs_data(attr_set);
	struct pid_dbs_tuners *pid_tuners = dbs_data->tuners;
	int input;
	int ret;
	ret = sscanf(buf, "%d", &input);

	if(ret == NULL) return -EINVAL;

	mutex_lock(&access_variables);
	pid_tuners->temp_obj = input;
	mutex_unlock(&access_variables);

	return count;
}

// Mejor con la macro incluyendo otro fichero
gov_show_one_common(sampling_rate);
gov_show_one(pid, E_value);
gov_show_one(pid, F_value);
gov_show_one(pid, A_value);
gov_show_one(pid, B_value);
gov_show_one(pid, C_value);
gov_show_one(pid, temp_obj);

gov_attr_rw(sampling_rate);
gov_attr_rw(E_value);
gov_attr_rw(F_value);
gov_attr_rw(A_value);
gov_attr_rw(B_value);
gov_attr_rw(C_value);
gov_attr_rw(temp_obj);

static struct attribute *pid_attributes[] = {
	&sampling_rate.attr,
	&E_value.attr,
	&F_value.attr,
	&A_value.attr,
	&B_value.attr,
	&C_value.attr,
	&temp_obj.attr,
	NULL
};

/************* sysfs *****************************************/

/*static struct policy_dbs_info *pid_alloc(void)
{
	struct od_policy_dbs_info *dbs_info;

	dbs_info = kzalloc(sizeof(*dbs_info), GFP_KERNEL);
	return dbs_info ? &dbs_info->policy_dbs : NULL;
}

static void pid_free(struct policy_dbs_info *policy_dbs)
{
	kfree(to_dbs_info(policy_dbs));
}*/

static int pid_init(struct dbs_data *dbs_data)
{
	struct pid_dbs_tuners *tuners;
	u64 idle_time;
	int cpu;

	tuners = kzalloc(sizeof(*pid_dbs_tuners), GFP_KERNEL);
	if (!tuners)
		return -ENOMEM;

	tuners->E_value = -1;
	tuners->F_value = 0;
	tuners->A_value = 50000;
	tuners->B_value = -49722;
	tuners->C_value = 0;
	tuners->temp_obj = 85000; // OJO!! esto esta en segundos
	dbs_data->sampling_rate = 3000000; 3 seg

	dbs_data->tuners = tuners;
	return 0;
}

static void pid_exit(struct dbs_data *dbs_data)
{
	kfree(dbs_data->tuners);
}

static void pid_start(struct cpufreq_policy *policy)
{
	//struct od_policy_dbs_info *dbs_info = to_dbs_info(policy->governor_data);

	//dbs_info->sample_type = OD_NORMAL_SAMPLE;
}


static struct dbs_governor pid_dbs_gov = {
	.gov = CPUFREQ_DBS_GOVERNOR_INITIALIZER("PID_governor"),
	.kobj_type = { .default_attrs = pid_attributes },
	.gov_dbs_update = pid_update,
	//.alloc = pid_alloc,
	//.free = pid_free,
	.init = pid_init,
	.exit = pid_exit,
	.start = pid_start,
};

#define CPU_FREQ_GOV_PIDGOV	(&pid_dbs_gov.gov)

static struct thermal_governor thermal_gov_user_space = {
	.name		= "user_space",
	.throttle	= update_temp,
};

static int __init cpufreq_gov_pid_init(void)
{
	if(thermal_register_governor(&thermal_gov_user_space)) return 0;
	return cpufreq_register_governor(CPU_FREQ_GOV_PIDGOV);
}

static void __exit cpufreq_gov_pid_exit(void)
{
	thermal_unregister_governor(&thermal_gov_user_space);
	cpufreq_unregister_governor(CPU_FREQ_GOV_PIDGOV);
}

MODULE_AUTHOR("Pablo Hernandez <pabloheralm@gmail.com>");
MODULE_DESCRIPTION("'PID_governor' - A PID governor to keep
	the same temperature");
MODULE_LICENSE("GPL");

#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_PIDGOV
struct cpufreq_governor *cpufreq_default_governor(void)
{
	return CPU_FREQ_GOV_PIDGOV;
}

fs_initcall(cpufreq_gov_pid_init);
#else
module_init(cpufreq_gov_pid_init);
#endif
module_exit(cpufreq_gov_pid_exit);