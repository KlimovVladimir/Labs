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

int main(int argc, char *argv[])
{
	int i, pid[argc], status, stat;
	char arg1[1024];
	char arg2[1024];
	if (argc < 3) {
		printf
		    ("Мало аргументов. Используйте <имя файла имя файла .... >\n");
		exit(-1);
	}
	for (i = 1; i < argc; i = i + 2) {
		strcpy(arg1, argv[i]);
		strcpy(arg2, argv[i + 1]);
		pid[i] = fork();
		if (pid[i] == 0) {

			if (execl("./son_proc", "son_proc", arg1, arg2, NULL) <
			    0) {
				printf
				    ("Не удалось открыть программу %s\n",
				     argv[i]);
				exit(-2);
			} else
				printf
				    ("Программа-потомок %s запущена (pid=%d)\n",
				     argv[i], pid[i]);
		}
	}
	sleep(1);
	for (i = 1; i < argc; i = i + 2) {

		status = waitpid(pid[i], &stat, WNOHANG);
		if (pid[i] == status) {

			printf
			    ("Программа-потомок %d завершена,  Вычислено мат. ожиданий:=%d\n",
			     i, WEXITSTATUS(stat));

		}
	}
	return 0;
}
