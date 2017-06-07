
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/printk.h> // For debug
#include <linux/cpufreq.h>
#include <linux/init.h>
#include <linux/module.h>

static int E = -1;
static int F = 0;
static int A = 50000;
static int B = -49722;
static int C = 0;

static DEFINE_MUTEX(access_variables);


static int cpufreq_pid_governor_init(struct cpufreq_policy *policy)
{
	return 0;
}

static void cpufreq_pid_governor_exit(struct cpufreq_policy *policy)
{

}

static int cpufreq_pid_governor_start(struct cpufreq_policy *policy)
{
	return 0;
}

static void cpufreq_pid_governor_stop(struct cpufreq_policy *policy)
{

}

static void cpufreq_pid_governor_limits(struct cpufreq_policy *policy)
{

}

/*************sysfs*****************************************/

static ssize_t show_E_value(struct gov_attr_set *attr_set, char *buf)
{
	int aux;
	mutex_lock(&access_variables);
	aux = E;
	mutex_unlock(&access_variables);
	return sprintf(buf, "%d\n", aux);
}

static ssize_t store_E_value(struct gov_attr_set *attr_set,
				const char *buf, size_t count)
{
	unsigned int input;
	int ret;
	ret = sscanf(buf, "%d", &input);

	if(ret == NULL) return -EINVAL;

	mutex_lock(&access_variables);
	E = input;
	mutex_unlock(&access_variables);

	return count;
}

static struct governor_attr E_value =					\
__ATTR(E_value, 0644, show_E_value, store_E_value)

static struct governor_attr F_value =					\
__ATTR(F_value, 0644, show_F_value, store_F_value)

static struct governor_attr A_value =					\
__ATTR(A_value, 0644, show_A_value, store_A_value)

static struct governor_attr B_value =					\
__ATTR(B_value, 0644, show_B_value, store_B_value)

static struct governor_attr C_value =					\
__ATTR(C_value, 0644, show_C_value, store_C_value)

static struct governor_attr temp_obj =					\
__ATTR(temp_obj, 0644, show_temp_obj, store_temp_obj)


static struct cpufreq_governor cpufreq_gov_pidgovernor = {
	.name		= "PID_governor",
	.init		= cpufreq_pid_governor_init,
	.exit		= cpufreq_pid_governor_exit,
	.start		= cpufreq_pid_governor_start,
	.stop		= cpufreq_pid_governor_stop,
	.limits		= cpufreq_pid_governor_limits,
	.owner		= THIS_MODULE,
};

static int __init cpufreq_gov_pid_init(void)
{
	return cpufreq_register_governor(CPU_FREQ_GOV_PIDGOV);
}

static void __exit cpufreq_gov_pid_exit(void)
{
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