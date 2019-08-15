server: server.o client
	gcc -Wall server.o -o server -std=c99 -lpthread

server.o: server.c
	gcc -c server.c 

client: client.o
	gcc -Wall client.o -o client

client.o: client.c
	gcc -c client.c
clean:
	rm server server.o client client.o
