#!/bin/bash

# Check if we are root
[ "$(whoami)" != "root" ] && exec sudo -- "$0" "$@"

# Default filename
filename="temps_log.txt"
# Change file
if [ "$#" -gt 0 ];
then
	filename=$1	
fi

while true;
do
	cat /sys/class/thermal/thermal_zone0/temp /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq | tr "\\n" " " >>$filename
	sleep 1
done