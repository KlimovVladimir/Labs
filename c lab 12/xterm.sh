#!/bin/bash
make clean
make
xterm -e './client1 172.21.0.189 5009' &
xterm -e './client1 172.21.0.189 5009' &
xterm -e './client2 172.21.0.189 5009' &
xterm -e './client2 172.21.0.189 5009' &
xterm -e './server 172.21.0.189  5009' &


