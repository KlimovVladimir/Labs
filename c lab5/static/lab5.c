//Вариант задания:
//1.Операции сложения и вычитания.

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>


int main (int argc, char **argv)
{
    int x,y,result;
    printf ("Введите х и у\n");
    scanf("%d %d",&x,&y);
    result = plus(x,y);
    printf ("Результат сложения: %d\n",result);
    result = minus(x,y);
    printf ("Результат вычитания: %d\n",result);
    return 0;
}
