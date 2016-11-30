#!/bin/bash

echo -e "\033[0;36m============================================================"

echo -e "\033[0;31m  _____ _____  _    _        _    _            _ "
echo -e " / ____|  __ \| |  | |      | |  | |          | |  "
echo -e "| |    | |__) | |  | |______| |__| | ___  __ _| |_ ___ _ __ "
echo -e "| |    |  ___/| |  | |______|  __  |/ _ \/ _\ | __/ _ \ '__|"
echo -e "| |____| |    | |__| |      | |  | |  __/ (_| | ||  __/ |   "
echo -e " \_____|_|     \____/       |_|  |_|\___|\__,_|\__\___|_|"
 
echo -e "\033[0;36m============================================================\033[0m"

exec 2>/dev/null

notworking=0
params=--num-threads=4


echo -n "Setting Governor to userspace..."
echo userspace >/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
echo userspace >/sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
echo userspace >/sys/devices/system/cpu/cpu2/cpufreq/scaling_governor
echo userspace >/sys/devices/system/cpu/cpu3/cpufreq/scaling_governor
echo -e "[\033[0;92mOK\033[0m]"

for param in "$*"
do
	if [ $param = -t ];
	then
		notworking=1
		params=--num-threads=1
		echo -n "Shutting down cores..."
		echo 0 > /sys/devices/system/cpu/cpu1/online
		echo 0 > /sys/devices/system/cpu/cpu2/online
		echo 0 > /sys/devices/system/cpu/cpu3/online
		echo -e "[\033[0;92mOK\033[0m]"
	fi
done

for freq in `cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies`
do

	for file in `ls /sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed`
	do
		echo 384000 > $file
	done

	echo -n "Cooling Down  "
	while [ $(cat /sys/class/thermal/thermal_zone0/temp) -gt 47 ];
	do
		sleep 10
	done
	echo -e "[\033[0;34mDONE\033[0m]"

	for file in `ls /sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed`
	do
		echo $freq > $file
	done

	echo -n "Starting test at $freq KHz  "
	sysbench --test=cpu --cpu-max-prime=80000 $params run >/dev/null &
	pid=$!

	sleep 600 && kill -9 $pid &

	while kill -0 $pid 2>/dev/null;
	do
		cat /sys/class/thermal/thermal_zone0/temp >>estado_"$freq".txt
		sleep 3
	done
	echo -e "[\033[0;34mDONE\033[0m]"

done

if [ $notworking = 1 ];
then
	echo -n "Starting back cores..."
	echo 1 > /sys/devices/system/cpu/cpu1/online
	echo 1 > /sys/devices/system/cpu/cpu2/online
	echo 1 > /sys/devices/system/cpu/cpu3/online
	echo -e "[\033[0;92mOK\033[0m]"
fi

echo -n "Setting Governor back to ondemand..."
echo ondemand >/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
echo ondemand >/sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
echo ondemand >/sys/devices/system/cpu/cpu2/cpufreq/scaling_governor
echo ondemand >/sys/devices/system/cpu/cpu3/cpufreq/scaling_governor
echo -e "[\033[0;92mOK\033[0m]"
