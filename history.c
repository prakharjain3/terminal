#include <stdio.h>
#include <string.h>
#include "./include/history.h"

int loadHistory(char history[HISTORYSIZE][PATHANDNAMESIZE])
{
    FILE *ptr = fopen(HISTORYPATH, "r");

    if (ptr == NULL)
    {
        printf("\033[091;49mUnable to Load File\033[0m");
        return -1;
    }

    char input[1000];

    int i = 0;

    while(fgets(input, 1000, ptr)!= NULL)
    {
        strcpy(history[i++],input);
        // printf("%s",history[i]);
        // i++;
    }

    fclose(ptr);

    return i;
}

void printHistory(char history[HISTORYSIZE][PATHANDNAMESIZE], int noOfCommandsInHistory)
{
    for (int i = ((noOfCommandsInHistory - 10) > 0 ? (noOfCommandsInHistory - 10) : 0); i < noOfCommandsInHistory; i++)
        printf("%2d  %s", i + 1, history[i]);
    printf("\n");
}

void writeHistory(char history[HISTORYSIZE][PATHANDNAMESIZE], int noOfCommandsInHistory)
{
    FILE *ptr = fopen(HISTORYPATH, "w");

    if (ptr == NULL)
    {
        printf("\033[0;91;49mUnable to write on file\033[0m");
        return;
    }

    for (int i = 0; i< noOfCommandsInHistory; i++)
    {
        fputs(history[i], ptr);
        // printf("%s",history[i]);
    }
    fclose(ptr);
}
