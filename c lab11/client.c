//9. Контрольная сумма. Для нескольких файлов (разного размера) требуется вычислить контрольную сумму 
//(сумму кодов всех символов файла). Обработка каждого файла выполняется в отдельном процессе.
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

int main(int argc, char *argv[])
{
	int sockfd = 0, n = 0;
	char recvBuff[1025];
	char sendBuff[1025];
	struct sockaddr_in serv_addr;
	unsigned short echoServPort;
	if (argc < 4) {
		fprintf(stderr,
			"Usage: %s <Server IP> <Port> <File> <File> ...\n",
			argv[0]);
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

	strcpy(sendBuff, "");
	for (int i = 3; i < argc; i++) {
		strcat(sendBuff, argv[i]);
		if (i - argc != -1)
			strcat(sendBuff, " ");
		else
			strcat(sendBuff, "\n");
	}
	write(sockfd, sendBuff, strlen(sendBuff));
	while ((n = read(sockfd, recvBuff, sizeof (recvBuff) - 1)) > 0) {
		recvBuff[n] = 0;
		if (fputs(recvBuff, stdout) == EOF) {
			printf("\n Error : Fputs error\n");
		}
	}

	if (n < 0) {
		printf("\n Read error \n");
	}

	return 0;
}
