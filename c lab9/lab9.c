//Винни-Пух и пчелы. Заданное количество пчел добывают мед равными порциями, задерживаясь в пути на случайное время. 
//Винни-Пух потребляет мед порциями заданной величины за заданное время и столько же времени может прожить без питания. 
//Работа каждой пчелы реализуется в порожденном процессе.

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <ctype.h>

#define MAX_LEN 1024
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};

int main(int argc, char *argv[])
{
	if (argc < 4) {
		printf
		    ("Мало аргументов. Используйте <кол-во пчел, порция Винни, время потребления>\n");
		exit(-1);
	}
	int countbees = atoi(argv[1]);
	int pid[countbees];
	int portion = atoi(argv[2]);
	int eattime = atoi(argv[3]);

	int shmid;
	key_t key = 69;
	int *shm;

	int semid;
	union semun arg;
	struct sembuf lock_res = { 0, -1, 0 };
	struct sembuf rel_res = { 0, 1, 0 };
	if ((key = ftok(".", 'S')) < 0) {
		printf("Невозможно получить ключ\n");
		exit(1);
	}
	semid = semget(key, 1, 0666 | IPC_CREAT);
	arg.val = 1;
	semctl(semid, 0, SETVAL, arg);

	if ((shmid = shmget(key, sizeof (double), IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		exit(1);
	}

	if ((shm = (int *)shmat(shmid, NULL, 0)) == (int *)-1) {
		perror("shmat");
		exit(1);
	}
	*(shm) = 10;
	printf("Кол-во мёда = %d\n", *(shm));
	if (shmdt(shm) < 0) {
		printf("Ошибка отключения\n");
		exit(1);
	}

	for (int i = 0; i < countbees; i++) {
		pid[i] = fork();
		srand(getpid());

		if (-1 == pid[i]) {
			perror("fork");
			exit(1);
		} else if (0 == pid[i]) {
			printf(" Пчела[%d]: Жжжжж!\n", i);
			while (1) {
				if ((shm =
				     (int *)shmat(shmid, NULL,
						  0)) == (int *)-1) {
					perror("shmat");
					exit(1);
				}

				if ((semop(semid, &lock_res, 1)) == -1) {
					fprintf(stderr, "Lock failed\n");
					exit(1);
				}
				*(shm) = *(shm) + 2;
				sleep(rand() % 4);
				if ((semop(semid, &rel_res, 1)) == -1) {
					fprintf(stderr, "Unlock failed\n");
					exit(1);
				}
				if (shmdt(shm) < 0) {
					printf
					    ("Ошибка отключения\n");
					exit(1);
				}
			}
			exit(1);
		}
	}
	while (1) {
		if ((shm = (int *)shmat(shmid, NULL, 0)) == (int *)-1) {
			perror("shmat");
			exit(1);
		}
		if (*(shm) < portion) {
			sleep(eattime);
			if (*(shm) < portion) {
				printf("Винни погиб \n");
				printf
				    ("Кол-во мёда на момент смерти = %d\n",
				     *(shm));
				for (int i = 0; i < countbees; i++) {
					kill(pid[i], SIGKILL);
				}
				return 0;
			}
		}
		*(shm) = *(shm) - portion;
		sleep(eattime);
		printf("Винни поел. Кол-во мёда = %d\n",
		       *(shm));
	}
	return 0;

}
