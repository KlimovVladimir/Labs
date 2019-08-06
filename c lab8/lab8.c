//6. Шарики. Координаты заданного количества шариков изменяются на случайную величину по вертикали и горизонтали.
//При выпадении шарика за нижнюю границу допустимой области шарик исчезает.
//Изменение координат каждого шарика в отдельном процессе (потоке).

#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/errno.h>
#define MAX_SEND_SIZE 80

struct mymsgbuf {
	long mtype;
	char mtext[MAX_SEND_SIZE];
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
	msgrcv(qid, (struct msgbuf *)qbuf, MAX_SEND_SIZE, type, 0);
	strcpy(msg, qbuf->mtext);
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf
		    ("Мало аргументов. Используйте <кол-во шариков>\n");
		exit(-1);
	}
	int x = 0, y = 0;
	int countballs = atoi(argv[1]);
	int pid[countballs];
	key_t key;
	int qtype = 1;
	struct mymsgbuf qbuf;
	int msgqid;
	char coord[MAX_SEND_SIZE];
	int xy[2];

	key = ftok(".", 'm');
	if ((msgqid = msgget(key, IPC_CREAT | 0660)) == -1) {
		perror("msgget");
		exit(1);
	}
	printf
	    ("Границы допустимой области (5;5) (5;-5) (-5;5) (-5;-5)\n");
	for (int i = 0; i < countballs; i++) {

		pid[i] = fork();
		srand(getpid());
		if (-1 == pid[i]) {
			perror("fork");
			exit(1);
		} else if (0 == pid[i]) {

			printf(" Шарик[%d]: Готов!\n", i);
			char str[10];
			while (1) {

				x += rand() % 3;
				y += rand() % 5;

				x -= rand() % 4;
				y -= rand() % 3;
				sprintf(str, "%d %d %d", i, x, y);
				sleep(3);

				send_message(msgqid, (struct mymsgbuf *)&qbuf,
					     qtype, str);
				fflush(stdout);

			}
			exit(1);
		}
	}
	int countkill = countballs;
	int i;
	while (1) {
		if (countkill <= 0) {
			msgctl(msgqid, IPC_RMID, NULL);
			return 0;
		}
		read_message(msgqid, &qbuf, qtype, coord);
		char *token;
		int j = 0;
		token = strtok(coord, " ,\t\n");
		i = strtol(token, NULL, 10);
		token = strtok(NULL, " ,\t\n");
		while (token != NULL) {
			xy[j] = strtol(token, NULL, 10);
			j++;
			token = strtok(NULL, " ,\t\n");
		}

		printf("Ball[%d]: Text: %d %d\n", i, xy[0], xy[1]);

		if (xy[0] > 5 || xy[0] < -5 || xy[1] > 5 || xy[1] < -5) {

			kill(pid[i], SIGKILL);
			printf("Ball[%d]: Убит!\n", i);
			countkill--;
		}

	}
	return 0;
}
