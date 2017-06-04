
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/printk.h> // For debug
#include <linux/cpufreq.h>
#include <linux/init.h>
#include <linux/module.h>


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