#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#define ECHOMAX 255

int main(int argc, char *argv[])
{
	int sockfd = 0;
	char recvBuff[ECHOMAX+1];
	char sendBuff[ECHOMAX+1];
	struct sockaddr_in serv_addr;
	unsigned short echoServPort;
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <Server IP> <Port>\n", argv[0]);
		exit(1);
	}
	echoServPort = atoi(argv[2]);
	memset(recvBuff, '0', sizeof (recvBuff));
	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		printf("\n Error : Could not create socket \n");
		return 1;
	}

	memset(&serv_addr, '0', sizeof (serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(echoServPort);

	if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
		printf("\n inet_pton error occured\n");
		return 1;
	}

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof (serv_addr)) <
	    0) {
		printf("\n Error : Connect Failed \n");
		return 1;
	}
	char echoBuffer[ECHOMAX + 1];

	char Bufferer[100];
	read(sockfd, Bufferer, sizeof (Bufferer) - 1);
	Bufferer[strlen(Bufferer)] = '\0';

	int sockB;
	struct sockaddr_in broadcastAddr;
	unsigned short broadcastPort = atoi(Bufferer);
	int recvStringLen;

	if ((sockB = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		perror("socket() failed");

	memset(&broadcastAddr, 0, sizeof (broadcastAddr));
	broadcastAddr.sin_family = AF_INET;
	broadcastAddr.sin_addr.s_addr = inet_addr(argv[1]);
	broadcastAddr.sin_port = htons(broadcastPort);

	if (bind
	    (sockB, (struct sockaddr *)&broadcastAddr,
	     sizeof (broadcastAddr)) < 0) {
		perror("bind() failed");
	}

	char Buff[2];
	strcpy(Buff, "2");
	write(sockfd, Buff, strlen(Buff));

	srand(getpid());
	for (;;) {
		recvStringLen =
		    recvfrom(sockB, echoBuffer, ECHOMAX, 0, NULL, 0);
		echoBuffer[recvStringLen] = '\0';
		if (strcmp(echoBuffer, "Есть сообщения") == 0) {
			send(sockfd, Buff, strlen(Buff), 0);
			sleep(1);
			recv(sockfd, sendBuff, sizeof (sendBuff) - 1, 0);
			char *token;
			token = strtok(sendBuff, " ");
			int number = atoi(token);
			token = strtok(NULL, " ");
			int lenbuff = atoi(token);
			token = strtok(NULL, " ");
			char strok[ECHOMAX+1];
			for(int i = 0; i < lenbuff; i++)
			{
				strok[i] = *(token+i);
			}
			sprintf(sendBuff, "%d %d %s", number, lenbuff, strok);
			printf("Получено сообщение <%s>\n",
			       sendBuff);
			for(int i = 0; i < ECHOMAX+1; i++)
			{
				strok[i] = '\0';
			}
			sleep(rand() % 3);
		}
	}

	close(sockfd);
	return 0;
}
