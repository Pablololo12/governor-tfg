#!/bin/bash

# Check if we are root
[ "$(whoami)" != "root" ] && exec sudo -E -- "$0" "$@"

echo -e "\033[0;36m================================================================================"
echo -e "\033[0;31m   ___  _______    _________ _   _________  _  ______  ___    ______        __ "
echo -e "  / _ \/  _/ _ \  / ___/ __ \ | / / __/ _ \/ |/ / __ \/ _ \  /_  __/__ ___ / /_"
echo -e " / ___// // // / / (_ / /_/ / |/ / _// , _/    / /_/ / , _/   / / / -_|_-</ __/"
echo -e "/_/  /___/____/  \___/\____/|___/___/_/|_/_/|_/\____/_/|_|   /_/  \__/___/\__/ "

echo -e "\033[0;36m================================================================================"


exec 2>/dev/null

if [ "$#" -ne 7 ]; then
	echo "Usage: A B C E F TempObj"
	exit
fi

echo -n "Shutting down cores..."
echo 0 > /sys/devices/system/cpu/cpu2/online
echo 0 > /sys/devices/system/cpu/cpu3/online
if [ "$?" -ne 0 ]; then
	echo -e "[\033[0;31mERROR\033[0m]"
	exit
fi
echo -e "[\033[0;92mOK\033[0m]"


echo -n "Setting Governor to PID_GOVERNOR..."
echo PID_GOVERNOR >/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
echo PID_GOVERNOR >/sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
if [ "$?" -ne 0 ]; then
	echo -e "[\033[0;31mERROR\033[0m]"
	exit
fi
echo -e "[\033[0;92mOK\033[0m]"


echo -n "Changing parameters of PID"
echo "$1" >/sys/devices/system/cpu/cpufreq/A_value
echo "$2" >/sys/devices/system/cpu/cpufreq/B_value
echo "$3" >/sys/devices/system/cpu/cpufreq/C_value
echo "$4" >/sys/devices/system/cpu/cpufreq/E_value
echo "$5" >/sys/devices/system/cpu/cpufreq/F_value
echo "$6" >/sys/devices/system/cpu/cpufreq/temp_obj
if [ "$?" -ne 0 ]; then
	echo -e "[\033[0;31mERROR\033[0m]"
	exit
fi
echo -e "[\033[0;92mOK\033[0m]"

echo "Recording temps"
./tempLog.sh &
PIDLOG="$!"

echo "Starting test"
./benchmark >log.txt &
PIDTEST="$!"

while kill -0 $pid 2>/dev/null;
do
	sleep 5
done

kill -9 "$PIDLOG"