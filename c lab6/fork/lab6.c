//4.2. Для вариантов заданий написать и отладить программу, реализующую порожденный процесс.
//Вариант: 8. Статистический анализ. Имеется несколько массивов данных (разного размера).
//Требуется определить математическое ожидание в каждом массиве.
//Обработка каждого массива выполняется в отдельном процессе.
#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAX_LEN 1024

float **readMas(FILE * fp, float **mas, int *mascol, int count, int maxlen)
{
	char buffer[MAX_LEN];
	char *token;
	int j = 0;
	for (int i = 0; i < count; i++) {
		mas[i] = (float *)malloc(sizeof (float) * maxlen);
		mascol[i] = 0;
		fgets(buffer, 1024, fp);
		token = strtok(buffer, " ,\t\n");
		while (token != NULL) {
			mas[i][j] = strtol(token, NULL, 10);
			j++;
			token = strtok(NULL, " ,\t\n");
			mascol[i] = mascol[i] + 1;
		}
		j = 0;
	}
	return mas;
}

float **readMasCh(FILE * fp, float **mas, int *mascol, int count, int maxlen)
{

	for (int i = 0; i < count; i++) {
		mas[i] = (float *)malloc(sizeof (float) * maxlen);
		for (int j = 0; j < mascol[i]; j++) {
			fscanf(fp, "%f ", &mas[i][j]);
		}
	}

	return mas;
}

void freeMas(float **mas, int count)
{
	for (int i = 0; i < count; i++) {
		free(mas[i]);
	}
	free(mas);
}

void printMas(float **mas, int *mascol, int count)
{
	printf("Введные массивы:\n");
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < mascol[i]; j++) {
			printf("%f ", mas[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char *argv[])
{
	FILE *fp, *fp1;
	float **mas = NULL;
	float **masch = NULL;
	int count = 0;
	int i, status, stat;
	float summ = 0;
	char buffer[MAX_LEN];

	if (argc < 3) {
		printf
		    ("Мало аргументов. Используйте <имя файла с числами имя файла с вероятностями>\n");
		exit(-1);
	}

	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf
		    ("Не удается открыть файл с числами.\n");
		exit(1);
	}

	if ((fp1 = fopen(argv[2], "r")) == NULL) {
		printf
		    ("Не удается открыть файл с вероятностями.\n");
		exit(1);
	}

	fgets(buffer, 1024, fp);
	count = atoi(buffer);
	int mascol[count];
	int pid[count];
	mas = (float **)malloc(sizeof (float *) * MAX_LEN);
	mas = readMas(fp, mas, mascol, count, MAX_LEN);
	printMas(mas, mascol, count);

	masch = (float **)malloc(sizeof (float *) * MAX_LEN);
	masch = readMasCh(fp1, masch, mascol, count, MAX_LEN);
	printMas(masch, mascol, count);

	if (fclose(fp)) {
		printf
		    ("Ошибка при закрытии файла с числами.\n");
		exit(1);
	}

	if (fclose(fp1)) {
		printf
		    ("Ошибка при закрытии файла с вероятностями.\n");
		exit(1);
	}

	for (i = 0; i < count; i++) {
		pid[i] = fork();
		srand(getpid());

		if (-1 == pid[i]) {
			perror("fork");
			exit(1);
		} else if (0 == pid[i]) {
			printf
			    (" CHILD: Это %d процесс-потомок СТАРТ!\n",
			     i);
			sleep(rand() % 4);
			printf
			    (" CHILD: Это %d процесс-потомок ВЫХОД!\n",
			     i);
			summ = 0;
			for (int j = 0; j < mascol[i]; j++) {
				summ += mas[i][j] * masch[i][j];
			}
			exit(summ);
		}
	}
	printf("PARENT: Это процесс-родитель!\n");
	for (i = 0; i < count; i++) {
		status = waitpid(pid[i], &stat, 0);
		if (pid[i] == status) {
			printf
			    ("процесс-потомок %d done,  Мат. ожидание[%d]:=%d\n",
			     i, i, WEXITSTATUS(stat));
		}
	}
	freeMas(mas, count);
	return 0;
}
