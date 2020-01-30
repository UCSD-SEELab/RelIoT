#!/bin/bash
# Set up env for tap bridge on host or reset
# Usage:
#   To set up env: sudo bash ./br-set.sh set
#   To reset: sudo bash ./br-set.sh reset
mode=$1
# echo "$mode"

if [ $mode = "set" ]
then
  sudo brctl addbr br0
  sudo tunctl -t tap0
  sudo ifconfig tap0 hw ether 00:00:00:00:00:01
  sudo ifconfig tap0 0.0.0.0 up
  sudo brctl addif br0 tap0
  sudo brctl addif br0 enp0s20f0u10
  sudo ifconfig br0 10.1.1.1 netmask 255.255.255.0 up
elif [ $mode = "reset" ]
then
  sudo ifconfig br0 down
  # sudo ip link set br0 down # same functionality as the last line
  sudo brctl delif br0 tap0 enp0s20f0u10
  sudo brctl delbr br0
  sudo ifconfig tap0 down
  sudo ip link delete tap0
else
  echo "Usage: sudo bash ./br-set.sh [set/reset]"
fi

# check the status
ifconfig
