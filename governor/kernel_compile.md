* Dependencies:
```sudo apt-get install libncurses5-dev gcc make git exuberant-ctags bc libssl-dev```

 ---

* Latests kernel version:
```git clone git://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git```

 ---

* Copy governor code to cpufreq:
```cp governor-tfg/governor/PID_governor.c linux/drivers/cpufreq/```


* Edit linux/drivers/cpufreq/Kconfig and add:
```
config CPU_FREQ_DEFAULT_GOV_PID_GOV
        bool "PID_GOV"
        select CPU_FREQ_GOV_PID_GOV
        help 
          Governor test

config CPU_FREQ_GOV_PID_GOV
        tristate "'PID_GOV' cpufreq governor"
        help
          Governor test
```

* Edit linux/drivers/cpufreq/Makefile and add:

```
obj-$(CONFIG_CPU_FREQ_GOV_PID_GOV)      += PID_governor.o
```

* Edit configuration file (If you use the config by default is in /arch/):
  * For example: ```(kernel)/arch/x86/config/x84_68_defconfig```

And add: ```CONFIG_CPU_FREQ_GOV_PID_GOV=y``` 

* Last step is to compile and load the kernel:

```
make defconfig o cp /boot/config-`uname -r`* .config (To use previous configs)
make o make -j{core number}
sudo make modules_install install
sudo update-grub2
```