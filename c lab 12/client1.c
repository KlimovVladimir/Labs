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
#define MAX_LEN 5
#define MAX_SLEEP 5

void gen_random(char *s, const int len)
{
	static const char alphanum[] = "abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i) {
		s[i] = alphanum[rand() % (sizeof (alphanum) - 1)];
	}

	s[len] = 0;
}

int main(int argc, char *argv[])
{
	int sockfd = 0;
	char recvBuff[ECHOMAX];
	char sendBuff[ECHOMAX];
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
	broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	broadcastAddr.sin_port = htons(broadcastPort);

	if (bind
	    (sockB, (struct sockaddr *)&broadcastAddr,
	     sizeof (broadcastAddr)) < 0) {
		perror("bind() failed");
	}

	char Buff[2];
	strcpy(Buff, "1");
	write(sockfd, Buff, strlen(Buff));

	int ii = 0;
	srand(getpid());
	int len = rand() % MAX_LEN + 1;
	for (;;) {
		recvStringLen =
		    recvfrom(sockB, echoBuffer, ECHOMAX, 0, NULL, 0);
		echoBuffer[recvStringLen] = '\0';
		if (strcmp(echoBuffer, "Жду сообщений") == 0) {
			char randoms[MAX_LEN];
			strcpy(echoBuffer, "");
			ii = rand() % MAX_SLEEP;
			gen_random(randoms, len);
			sprintf(sendBuff, "%d %d %s", ii, len, randoms);
			strcat(sendBuff, "\0");
			write(sockfd, sendBuff, strlen(sendBuff));
			printf("Сообщение <%s> отправлено\n",
			       sendBuff);
			sleep(ii);
		}
	}

	close(sockfd);
	return 0;
}
