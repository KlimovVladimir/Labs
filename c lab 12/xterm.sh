#!/bin/bash
make clean
make
xterm -e './server 192.168.122.1 2222' &
sleep 1
xterm -e './client1 192.168.122.1 2222' &
sleep 1
xterm -e './client1 192.168.122.1 2222' &
sleep 1
xterm -e './client2 192.168.122.1 2222' &
sleep 1
xterm -e './client2 192.168.122.1 2222' &

