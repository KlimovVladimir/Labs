#!/bin/bash
make clean
make
xterm -e './server 2222' &
xterm -e './client1 127.0.0.1 2222' &
xterm -e './client2 127.0.0.1 2222' &
xterm -e './client1 127.0.0.1 2222' &  
xterm -e './client2 127.0.0.1 2222' &
echo "Done"

