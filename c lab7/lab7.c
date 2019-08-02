//4.1. В соответствии с особенностями реализации варианта задания выбрать средство реализации межзадачных коммуникаций.
//Вариант: 4. Warcraft. Заданное количество юнитов добывают золото равными порциями из одной шахты, 
//задерживаясь в пути на случайное время, до ее истощения. Работа каждого юнита реализуется в порожденном процессе.

#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf
		    ("Мало аргументов. Используйте <кол-во рабочих>\n");
		exit(-1);
	}
	int mine = 100;
	int countslaves = atoi(argv[1]);
	int pid[countslaves];
	int fd1[argc][2];
	int len = 0;

	for (int i = 0; i < countslaves; i++) {
		pipe(fd1[i]);
		pid[i] = fork();
		srand(getpid());

		if (-1 == pid[i]) {
			perror("fork");
			exit(1);
		} else if (0 == pid[i]) {
			close(fd1[i][0]);
			printf(" Slave[%d]: Готов вкалывать!\n",
			       i);
			while (1) {
				write(fd1[i][1], 0, sizeof (int));
				sleep(rand() % 6);
				len = 5;
				write(fd1[i][1], &len, sizeof (int));
			}
			exit(1);
		}
	}
	printf("Кол-во золота в шахте: %d \n", mine);
	while (1) {
		for (int i = 0; i < countslaves; i++) {
			if (mine <= 0) {
				for (int i = 0; i < countslaves; i++) {
					kill(pid[i], SIGKILL);
				}
				return 0;
			}

			read(fd1[i][0], &len, sizeof (int));
			mine = mine - len;
			printf
			    ("Раб[%d]: Состояние шахты: %d \n",
			     i, mine);
		}
	}
	return 0;
}
