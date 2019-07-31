//4.3. Для вариантов заданий написать и отладить программу, реализующую родительский процесс,
//вызывающий и отслеживающий состояние порожденных процессов - программ
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

void printMas(float **mas, int *mascol, float *sum, int count)
{
	printf("Введные массивы:\n");
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < mascol[i]; j++) {
			printf("%f ", mas[i][j]);
		}
		printf("###Мат. ожидание: %f###", sum[i]);
		printf("\n");
	}
}

int main(int argc, char *argv[])
{
	FILE *fp, *fp1;
	float **mas = NULL;
	float **masch = NULL;
	int count = 0;
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
	mas = (float **)malloc(sizeof (float *) * MAX_LEN);
	mas = readMas(fp, mas, mascol, count, MAX_LEN);

	masch = (float **)malloc(sizeof (float *) * MAX_LEN);
	masch = readMasCh(fp1, masch, mascol, count, MAX_LEN);

	if (fclose(fp)) {
		printf("Ошибка при закрытии файла.\n");
		exit(1);
	}

	if (fclose(fp1)) {
		printf("Ошибка при закрытии файла.\n");
		exit(1);
	}

	float sum[count];
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < mascol[i]; j++) {
			sum[i] += mas[i][j] * masch[i][j];
		}
	}
	printf("(PIO: %d), File %s\n", getpid(), argv[1]);
	printMas(mas, mascol, sum, count);
	freeMas(mas, count);
	return (count);
}
