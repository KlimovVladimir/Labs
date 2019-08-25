#!/bin/bash
make clean
make
xterm -e './client1 192.168.122.1 5000' &
xterm -e './client2 192.168.122.1 5000' &
xterm -e './client1 192.168.122.1 5000' &
xterm -e './client2 192.168.122.1 5000' &
xterm -e './server 192.168.122.1 5000' &

