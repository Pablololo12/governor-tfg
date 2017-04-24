#!/bin/bash

# Check if we are root
[ "$(whoami)" != "root" ] && exec sudo -- "$0" "$@"

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
dontsleep=0
timer=600
userspace=0
export OMP_NUM_THREADS=4
export QSML_NUM_THREADS=4


while [ "$#" -gt 0 ]
do
	if [ "$1" = -t ];
	then
		notworking=1
		echo -n "Shutting down cores..."
		echo 0 > /sys/devices/system/cpu/cpu2/online
		echo 0 > /sys/devices/system/cpu/cpu3/online
		echo -e "[\033[0;92mOK\033[0m]"
		export OMP_NUM_THREADS=2
		export QSML_NUM_THREADS=2
	elif [ "$1" = -l ];
	then
		dontsleep=1
	elif [[ "$1" =~ ^[0-9]+$ ]];
	then
		timer=$1
	elif [ "$1" = -u ];
	then
		echo -n "Setting Governor to userspace..."
		echo userspace >/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
		echo userspace >/sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
		echo userspace >/sys/devices/system/cpu/cpu2/cpufreq/scaling_governor
		echo userspace >/sys/devices/system/cpu/cpu3/cpufreq/scaling_governor
		echo -e "[\033[0;92mOK\033[0m]"
		userspace=1
	elif [ "$1" = u ];
	then
		echo "-t for only 2 cores"
		echo "-l continuous execution"
		echo "-u for userspace"
		echo "number to change time"
	fi
	shift
done

for freq in `cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies`
do

	for file in `ls /sys/devices/system/cpu/cpu*/cpufreq/scaling_setspeed`
	do
		echo 384000 > $file
	done

	if [ $dontsleep = 0 ]; then
		echo -n "Cooling Down  "
		while [ $(cat /sys/class/thermal/thermal_zone0/temp) -gt 47000 ];
		do
			sleep 10
		done
		echo -e "[\033[0;34mDONE\033[0m]"
	fi

	for file in `ls /sys/devices/system/cpu/cpu*/cpufreq/scaling_setspeed`
	do
		echo $freq > $file
	done

	echo -n "Starting test at $freq KHz  "
	./benchmark >~/log.txt &
	pid=$!

	sleep $timer && kill -9 $pid &

	while kill -0 $pid 2>/dev/null;
	do
		if [ $dontsleep = 0 ]; then
			cat /sys/class/thermal/thermal_zone0/temp >>state_"$freq".txt
			cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq >>state_"$freq".txt
		else
			cat /sys/class/thermal/thermal_zone0/temp >>state_continuous.txt
			cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq >>state_continuous.txt
		fi
		sleep 3
	done
	echo -e "[\033[0;34mDONE\033[0m]"

done

if [ $notworking = 1 ];
then
	echo -n "Starting back cores..."
	echo 1 > /sys/devices/system/cpu/cpu2/online
	echo 1 > /sys/devices/system/cpu/cpu3/online
	echo -e "[\033[0;92mOK\033[0m]"
fi

if [ $userspace = 1 ];
then
	echo -n "Setting Governor back to ondemand..."
	echo ondemand >/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
	echo ondemand >/sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
	echo ondemand >/sys/devices/system/cpu/cpu2/cpufreq/scaling_governor
	echo ondemand >/sys/devices/system/cpu/cpu3/cpufreq/scaling_governor
	echo -e "[\033[0;92mOK\033[0m]"
fi
