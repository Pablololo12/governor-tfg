* Herramientas para compilar:
```sudo apt-get install libncurses5-dev gcc make git exuberant-ctags bc libssl-dev```

 ---

* Descarga de última version de kernel:
```git clone git://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git```

 ---

* Se copia el codigo del governor:
```cp governor-tfg/governor/governor-sublime.c linux/drivers/cpufreq/```


* Editar linux/drivers/cpufreq/Kconfig y añadir:
```
config CPU_FREQ_DEFAULT_GOV_SUBLIME
        bool "sublime"
        select CPU_FREQ_GOV_SUBLIME
        help 
          Prueba de Governor

config CPU_FREQ_GOV_SUBLIME
        tristate "'sublime' cpufreq governor"
        help
          Si ves esto es que funciona weee
```

* Editar linux/drivers/cpufreq/Makefile y añadir:

```
obj-$(CONFIG_CPU_FREQ_GOV_SUBLIME)      += governor-sublime.o
```

* Editar el fichero de configuración(Si se usa por defecto está en /arch/):
  * Por ejemplo: ```(kernel)/arch/x86/config/x84_68_defconfig```

Y añadir: ```CONFIG_CPU_FREQ_GOV_SUBLIME=y``` 

* Ahora se pasa a conpilar el Kernel y cargarlo:

```
make defconfig o cp /boot/config-`uname -r`* .config Para usar ajustes anteriores
make o make -j{numero cores} si es multicore
sudo make modules_install install
sudo update-grub2
```