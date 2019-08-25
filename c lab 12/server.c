#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/errno.h>
#define MAX_LEN 1025
#define	MAXNITEMS 1000000
#define ECHOMAX 255
#define MAX_MES 10
int colmes = 0;

struct mymsgbuf {
	long mtype;
	char mtext[ECHOMAX];
};

struct {
	pthread_mutex_t mutex;
	int buff[MAXNITEMS];
	int nput;
	int nval;
} shared = {
PTHREAD_MUTEX_INITIALIZER};

struct ThreadArgs {
	struct mymsgbuf qbuf;
	int msgqid;
	int listenfd;
	int connfd;
	int sock;
	char *serverIP;
	unsigned short broadcastPort;
};

void send_message(int qid, struct mymsgbuf *qbuf, long type, char *text)
{
	qbuf->mtype = type;
	strcpy(qbuf->mtext, text);

	if ((msgsnd(qid, (struct msgbuf *)qbuf,
		    strlen(qbuf->mtext) + 1, 0)) == -1) {
		perror("msgsnd");
		exit(1);
	}
}

void read_message(int qid, struct mymsgbuf *qbuf, long type, char *msg)
{
	qbuf->mtype = type;
	msgrcv(qid, (struct msgbuf *)qbuf, ECHOMAX, type, 0);
	strcpy(msg, qbuf->mtext);
}

void *ThreadBroad1(void *threadArgs)
{
	int sock;
	struct sockaddr_in broadcastAddr[102];
	char echoBuffer[ECHOMAX];
	int broadcastPermission;

	unsigned short broadcastPort =
	    ((struct ThreadArgs *)threadArgs)->broadcastPort;
	pthread_detach(pthread_self());

	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		perror("socket() failed");

	broadcastPermission = 1;
	if (setsockopt
	    (sock, SOL_SOCKET, SO_BROADCAST, (void *)&broadcastPermission,
	     sizeof (broadcastPermission)) < 0)
		perror("setsockopt() failed");

	for (int i = 0; i < 100; i++) {
		memset(&broadcastAddr[i], 0, sizeof (broadcastAddr[i]));
		broadcastAddr[i].sin_family = AF_INET;
		broadcastAddr[i].sin_addr.s_addr = INADDR_BROADCAST;
		broadcastAddr[i].sin_port = htons(broadcastPort + i);
	}
	for (;;) {

		for (int i = 0; i < 100; i++) {
			if (colmes < MAX_MES) {
				strcpy(echoBuffer, "Жду сообщений");
				sendto(sock, echoBuffer, strlen(echoBuffer), 0,
				       (struct sockaddr *)&broadcastAddr[i],
				       sizeof (broadcastAddr[i]));
			}
			if (colmes > 0) {
				strcpy(echoBuffer,
				       "Есть сообщения");
				sendto(sock, echoBuffer, strlen(echoBuffer), 0,
				       (struct sockaddr *)&broadcastAddr[i],
				       sizeof (broadcastAddr[i]));
			}
		}
		sleep(4);
	}
	return NULL;

}

void *ThreadMain(void *threadArgs)
{
	struct mymsgbuf qbuf;
	int msgqid = ((struct ThreadArgs *)threadArgs)->msgqid;
	int connfd = ((struct ThreadArgs *)threadArgs)->connfd;
	int qtype = 1;
	char recvBuff[ECHOMAX];
	pthread_detach(pthread_self());
	char Buff[2];
	int result;
	for (;;) {
		result = recv(connfd, Buff, sizeof (Buff), 0);
		if (result == 0) {
			return NULL;
		}
		pthread_mutex_lock(&shared.mutex);
		if (result > 0 && strcmp(Buff, "1") == 0) {
			recv(connfd, recvBuff, sizeof (recvBuff) - 1, 0);
			printf("Получено сообщение <%s>\n",
			       recvBuff);
			send_message(msgqid, (struct mymsgbuf *)&qbuf, qtype,
				     recvBuff);
			fflush(stdout);
			colmes++;
		}
		pthread_mutex_unlock(&shared.mutex);
	}
}

void *ThreadMain2(void *threadArgs)
{
	struct mymsgbuf qbuf;
	int msgqid = ((struct ThreadArgs *)threadArgs)->msgqid;
	int connfd = ((struct ThreadArgs *)threadArgs)->connfd;
	int qtype = 1;
	char recvBuff[ECHOMAX];
	char recvBuff1[ECHOMAX];
	pthread_detach(pthread_self());
	char Buff[2];
	int result;
	for (;;) {
		result = read(connfd, Buff, sizeof (Buff));
		if (result == 0) {
			return NULL;
		}
		pthread_mutex_lock(&shared.mutex);
		if (result > 0 && strcmp(Buff, "2") == 0) {
			read_message(msgqid, (struct mymsgbuf *)&qbuf, qtype,
				     recvBuff1);
			strcpy(recvBuff, recvBuff1);
			strcpy(recvBuff1, "");
			write(connfd, recvBuff, strlen(recvBuff));
			printf("Сообщение <%s> отправлено\n",
			       recvBuff);
			colmes--;
		}
		pthread_mutex_unlock(&shared.mutex);
	}
}

int main(int argc, char *argv[])
{
	struct ThreadArgs *threadArgs;
	threadArgs = (struct ThreadArgs *)malloc(sizeof (struct ThreadArgs));
	threadArgs->listenfd = 0;
	threadArgs->connfd = 0;
	struct sockaddr_in serv_addr;
	unsigned short echoServPort;
	char sendBuff[ECHOMAX];
	pthread_t threadID;
	unsigned short broadcastPort = 8000;
	threadArgs->broadcastPort = broadcastPort;
	key_t key;
	if (argc != 3) {
		fprintf(stderr, "Usage:  %s <Server IP Server Port>\n",
			argv[0]);
		exit(1);
	}
	threadArgs->serverIP = argv[1];

	key = ftok(".", 'm');
	if ((threadArgs->msgqid = msgget(key, IPC_CREAT | 0660)) == -1) {
		perror("msgget");
		exit(1);
	}

	echoServPort = atoi(argv[2]);
	threadArgs->listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	threadArgs->sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	memset(&serv_addr, '0', sizeof (serv_addr));
	memset(sendBuff, '0', sizeof (sendBuff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(threadArgs->serverIP);
	serv_addr.sin_port = htons(echoServPort);

	bind(threadArgs->listenfd, (struct sockaddr *)&serv_addr,
	     sizeof (serv_addr));

	listen(threadArgs->listenfd, 10);
	char Buff[100];
	sprintf(Buff, "%d", broadcastPort);
	threadArgs->broadcastPort = broadcastPort;
	if (pthread_create
	    (&threadID, NULL, ThreadBroad1, (void *)threadArgs) != 0) {
		perror("Creating thread");
		return EXIT_FAILURE;
	}

	while (1) {
		threadArgs->connfd =
		    accept(threadArgs->listenfd, (struct sockaddr *)NULL, NULL);
		char Buff[100];
		sprintf(Buff, "%d", broadcastPort);
		write(threadArgs->connfd, Buff, strlen(Buff));
		broadcastPort++;
		char recvBuff[2];
		read(threadArgs->connfd, recvBuff, sizeof (recvBuff) - 1);

		if (strcmp(recvBuff, "1") == 0) {
			if (pthread_create
			    (&threadID, NULL, ThreadMain,
			     (void *)threadArgs) != 0) {
				perror("Creating thread");
				return EXIT_FAILURE;
			}
		}
		if (strcmp(recvBuff, "2") == 0) {
			if (pthread_create
			    (&threadID, NULL, ThreadMain2,
			     (void *)threadArgs) != 0) {
				perror("Creating thread");
				return EXIT_FAILURE;
			}
		}

	}
	return 0;
}
