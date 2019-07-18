#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX_LEN 1024


void inp_str(char *string, int maxlen, FILE *fp)
{
    char buffer[MAX_LEN];
    fscanf(fp,"%s", buffer);
    strncpy(string, buffer, maxlen);
}

char** readMas(FILE *fp, char **mas, int count, int maxlen)
{
    for (int i = 0; i < count ; i++) {
        mas[i] = (char *)malloc(sizeof(char)*maxlen);
        inp_str(mas[i], maxlen, fp);
    }
    return mas;
}

void printMas(char **mas, int count)
{
    for (int i = 0; i < count ; i++) {
        printf("%s\n", mas[i]);
    }
}

void freeMas(char **mas, int count)
{
    for (int i = 0; i < count; i++) {
        free(mas[i]);
    }
    free(mas);
}

void cngstrings(char **mas, int count, char sym, char* name )
{
    FILE *op;
    int ii=0;
    for(int i=0; i<strlen(name); i++)
    {
        if(name[i]=='.')
        {
            ii=i;
        }
    }
    if(ii==0)
    {
        name[ii+1]='.';
        name[ii+2]='r';
    }
    else
    {
        name[ii+1]='r';
    }
    op=fopen(name, "w");
    for (int i=0; i<count; i++)
    {
        for (int j=0; mas[i][j]; j++)
        {
            if (mas[i][j]==sym)
            {
                mas[i][j] = ' ';
            }
        }
        printf("%s\n",mas[i]);
        fprintf(op, "%s\n",mas[i]);
    }
    fclose(op);
}

int main(int argc, char **argv)
{
    FILE *fp;
    char **mas = NULL;
    int count = 0;
    char sym = *argv[2];

    if (argc < 3)
    {
        fprintf (stderr, "Мало аргументов. Используйте <имя файла, заданный символ>\n");
        exit (1);
    }

    if((fp=fopen(argv[1], "r"))==NULL)
    {
        printf("Не удается открыть файл.\n");
        exit(1);
    }
    fscanf(fp,"%d", &count);
    mas = (char **)malloc(sizeof(char *)*count);
    mas=readMas(fp,mas,count,MAX_LEN);
    printf("Введенный массив:\n");
    printMas(mas, count);
    printf("Обработанный массив:\n");
    cngstrings(mas,count,sym,argv[1]);
    freeMas(mas, count);

    if(fclose(fp))
    {
        printf("Ошибка при закрытии файла.\n");
        exit(1);
    }

}

