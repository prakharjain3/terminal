#include "./include/prompt.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>

void prompt(int lenHomeDir, time_t time_taken)
{
    char promptBuffer[PATHANDNAMESIZE];
    getcwd(promptBuffer, PATHANDNAMESIZE);

    if (strlen(CURRPATH) < lenHomeDir && strlen(CURRPATH) != 0)
    {
        printf("<\033[0;94;49m%s@%s\033[0m:\033[0;92;49m%s\033[0m", USERNAME, HOSTNAME, promptBuffer);
        if (time_taken >= 1)
            printf(" took %lds", time_taken);
        printf(">");
        fflush(stdout);
    }
    else
    {
        printf("<\033[0;94;49m%s@%s\033[0m:\033[0;92;49m~%s\033[0m", USERNAME, HOSTNAME, promptBuffer + strlen(HOMEDIR));
        if (time_taken >= 1)
            printf(" took %lds", time_taken);
        printf(">");

        fflush(stdout);
    }
}

// void newprompt(int lenHomeDir, time_t time_taken)
// {

// }