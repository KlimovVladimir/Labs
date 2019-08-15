//9. Контрольная сумма. Для нескольких файлов (разного размера) требуется вычислить контрольную сумму 
//(сумму кодов всех символов файла). Обработка каждого файла выполняется в отдельном процессе.
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
#define MAX_LEN 1025

void *ThreadMain(void *threadArgs)
{
	pthread_detach(pthread_self());
	char recvBuff[MAX_LEN];
	char sendBuff[MAX_LEN];
	FILE *fp;
	int connfd = (*(int *)threadArgs);
	read(connfd, recvBuff, sizeof (recvBuff) - 1);

	char *pos;
	if ((pos = strchr(recvBuff, '\n')) != NULL)
		*pos = '\0';

	char *token;
	char files[MAX_LEN][MAX_LEN];
	int count = 0;
	token = strtok(recvBuff, " ");
	while (token != NULL) {
		strcpy(files[count], token);
		count++;
		token = strtok(NULL, " ");
	}

	for (int i = 0; i < count; i++) {
		if ((fp = fopen(files[i], "r")) == NULL) {
			printf
			    ("Не удается открыть файл.\n");
		} else {
			snprintf(sendBuff, sizeof (sendBuff), "%s\n", files[i]);
			write(connfd, sendBuff, strlen(sendBuff));

			char c;
			int summ = 0;
			while ((c = fgetc(fp)) != EOF) {
				summ += (int)c;
			}
			sprintf(sendBuff,
				"Контрольная сумма: %d \n",
				summ);
			write(connfd, sendBuff, strlen(sendBuff));
			if (fclose(fp)) {
				printf
				    ("Ошибка при закрытии файла.\n");
			}
		}
	}
	close(connfd);

	return (NULL);
}

int main(int argc, char *argv[])
{
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
	unsigned short echoServPort;
	char sendBuff[MAX_LEN];
	pthread_t threadID;

	if (argc != 2) {
		fprintf(stderr, "Usage:  %s <Server Port>\n", argv[0]);
		exit(1);
	}
	echoServPort = atoi(argv[1]);
	listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	memset(&serv_addr, '0', sizeof (serv_addr));
	memset(sendBuff, '0', sizeof (sendBuff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(echoServPort);

	bind(listenfd, (struct sockaddr *)&serv_addr, sizeof (serv_addr));

	listen(listenfd, 1);
	while (1) {
		connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);

		if (pthread_create(&threadID, NULL, ThreadMain, &connfd) != 0) {
			perror("Creating the first thread");
			return EXIT_FAILURE;
		}
		sleep(1);
	}
	return 0;
}
