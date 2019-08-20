server: server.o client1 client2
	gcc -Wall server.o -o server -std=c99 -lpthread

server.o: server.c
	gcc -c server.c 

client1: client1.o
	gcc -Wall client1.o -o client1 -std=c99

client1.o: client1.c
	gcc -c client1.c

client2: client2.o
	gcc -Wall client2.o -o client2 -std=c99

client2.o: client2.c
	gcc -c client2.c
clean:
	rm server server.o client1 client1.o client2 client2.o
