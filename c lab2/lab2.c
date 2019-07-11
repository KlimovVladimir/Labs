#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX_LEN 1024 

char** readMas(int count)
{
	char buffer[MAX_LEN];
	char **mas; 
	mas = (char **)malloc(sizeof(char *)*count);
    for (int i = 0; i < count ; i++){
        scanf("%s", buffer); 
        mas[i] = (char *)malloc(sizeof(char)*strlen(buffer)); 
        strcpy(mas[i], buffer); 
    }
    return mas; 
}

int CountDigits(const char str[]) {
    int count = 0;
 
    for (; *str; ++str) 
    {
        if (isdigit(*str)) 
        {
            count++;
        }
    }
 
    return count;
}

void printMas(char **mas, int count)
{
    for (int i = 0; i < count ; i++){
        printf("%s Кол-во цифр: %d\n", mas[i], CountDigits(mas[i]));
    }
}

void freeMas(char **mas, int count)
{
	for (int i = 0; i < count; i++){
        free(mas[i]); 
    }
    free(mas);
}


int bubbleSort(char **a, int count)
{
	int i,j,n=count,changes=0;
	for(i = 0 ; i < n - 1; i++) { 
       for(j = 0 ; j < n - i - 1 ; j++) {  
           if(CountDigits(a[j]) < CountDigits(a[j+1])) 
           {            
              char* tmp = a[j];
              a[j] = a[j+1] ;
              a[j+1] = tmp;
              changes++; 
           }
        }
    }
    return changes;
}


int main(int argc, char **argv){

	char **mas = NULL; 
	int count;
	int changes;
	scanf("%d", &count);
	mas = readMas(count);
	changes = bubbleSort(mas, count);
	printf("Кол-во перестановок: %d\n",changes);
	printMas(mas, count);
	freeMas(mas, count);
}

