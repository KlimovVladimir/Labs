//1. Поиск указанной строки в указанном файле. Обработка одной строки в порожденном процессе.

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#define N 5
#define	MAXNITEMS 1000000
#define MAX_LEN 1024
char *sym;

struct {
	pthread_mutex_t mutex;
	int buff[MAXNITEMS];
	int nput;
	int nval;
} shared = {
PTHREAD_MUTEX_INITIALIZER};

void *func(void *arg)
{
	int *ps = (int *)malloc(sizeof (int));
	FILE *op = (FILE *) arg;
	int flg;
	char buffer[MAX_LEN];
	pthread_mutex_lock(&shared.mutex);
	fgets(buffer, MAX_LEN, op);
	char *pos;
	if ((pos=strchr(buffer, '\n')) != NULL)
    *pos = '\0';
	flg = strcmp(buffer, sym);
	if (flg == 0)
	{
		*ps = 1;
		printf("Строка[%s] - строка найдена\n",buffer);
	}
	else
	{
		*ps = 0;
		printf("Строка[%s] - строка не найдена\n",buffer);
	}
	pthread_mutex_unlock(&shared.mutex);
	pthread_exit((void *)ps);
}

int main(int argc, char *argv[])
{
	int result;
	FILE *fp;
	int count = 0;
	char buffer[MAX_LEN];
	sym = (char *)malloc(sizeof(char)*MAX_LEN);
	int countsovp = 0;

	if (argc < 3) {
		fprintf(stderr,
			"Мало аргументов. Используйте <имя файла, заданная строка>\n");
		exit(1);
	}
	for(int i = 2; i < argc; i++)
	{
		strcat(sym,argv[i]);
		if(i - argc != -1)
		strcat(sym," ");
	}
	char *pos;
	if ((pos=strchr(sym, '\n')) != NULL)
    *pos = '\0';
	printf("Введеная строка: |%s|\n",sym);
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("Не удается открыть файл.\n");
		exit(1);
	}
	fgets(buffer, MAX_LEN, fp);
	count = atoi(buffer);
	pthread_t threads[count];
	void *status[count];

	for (int i = 0; i < count; i++) {
		result = pthread_create(&threads[i], NULL, func, fp);
		if (result != 0) {
			perror("Creating the first thread");
			return EXIT_FAILURE;
		}
	}

	for (int i = 0; i < count; i++) {
		result = pthread_join(threads[i], &status[i]);
		if (result != 0) {
			perror("Joining the first thread");
			return EXIT_FAILURE;
		} else {
			if (*((int *)status[i]) == 1)
			countsovp++;
		}
		free(status[i]);
	}
	
	printf("Кол-во найденных строк %d\n",countsovp);
	if (fclose(fp)) {
		printf("Ошибка при закрытии файла.\n");
		exit(1);
	}

	printf("Done..\n");
	return EXIT_SUCCESS;
}

