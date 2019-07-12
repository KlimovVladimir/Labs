#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


struct bus
{
    char name[50];
    int length;
    int countstop;
    int cost;
};
typedef struct bus buses;

void readBus(buses *st)
{
    printf("Введите Название маршрута:");
    scanf("%s", st->name);
    printf("Введите Протяженность:");
    scanf("%d", &st->length);
    printf("Введите Количество остановок:");
    scanf("%d", &st->countstop);
    printf("Введите Стоимость путевки:");
    scanf("%d", &st->cost);
}

void printBuses(buses **st, int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("\n#######################\n");
        printf("Название маршрута:%s\n", st[i]->name);
        printf("Протяженность:%d\n", st[i]->length);
        printf("Количество остановок:%d\n", st[i]->countstop);
        printf("Стоимость путевки:%d\n", st[i]->cost);

    }
}

static int cmp(const void *p1, const void *p2)
{
    buses * st1 = *(buses**)p1;
    buses * st2 = *(buses**)p2;
    return st2->cost - st1->cost;
}

void freeMas(buses **st, int count)
{
    for (int i = 0; i < count; i++)
    {
        free(st[i]);
    }
    free(st);
}

int main(int argc, char **argv)
{
    int count;
    printf("Введите кол-во маршрутов:");
    scanf("%d", &count);
    buses** st = (buses**)malloc(sizeof(buses*)*count);
    for (int i = 0; i < count ; i++)
    {   st[i] = (buses*) malloc (sizeof(buses));
        readBus(st[i]);
    }
    qsort(st, count, sizeof(buses*), cmp);
    printBuses(st,count);
    freeMas(st,count);

}

