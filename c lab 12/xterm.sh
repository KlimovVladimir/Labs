#!/bin/bash
make clean
make
xterm -e './client1 192.168.0.108 5009' &
xterm -e './client1 192.168.0.108 5009' &
xterm -e './client2 192.168.0.108 5009' &
xterm -e './client2 192.168.0.108 5009' &
xterm -e './server 192.168.0.108  5009' &


