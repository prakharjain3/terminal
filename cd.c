#include "./include/cd.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

void changeDirectory(char *string, int *flagPointer)
{
    if (string == NULL)
    {
        if (!chdir(HOMEDIR))
        {
            strcpy(PREVPATH, CURRPATH);
            getcwd(CURRPATH, PATHANDNAMESIZE);
            return;
        }
    }
    else if (!strcmp(string, ".."))
    {
        if (!chdir(string))
        {
            strcpy(PREVPATH, CURRPATH);
            getcwd(CURRPATH, PATHANDNAMESIZE);
            return;
        }
    }
    else if (!strcmp(string, "."))
    {
        strcpy(PREVPATH, CURRPATH);
        getcwd(CURRPATH, PATHANDNAMESIZE);
        return;
    }
    else if (!strcmp(string, "~") || string == NULL)
    {
        if (!chdir(HOMEDIR))
        {
            strcpy(PREVPATH, CURRPATH);
            getcwd(CURRPATH, PATHANDNAMESIZE);
            return;
        }
    }
    else if (!strcmp(string, "-"))
    {
        if (!chdir(PREVPATH))
        {
            printf("%s\n", PREVPATH);
            strcpy(PREVPATH, CURRPATH);
            getcwd(CURRPATH, PATHANDNAMESIZE);
            return;
        }
    }
    else if (strcmp(string, "-") != 0 && strcmp(string, "~") != 0 && strcmp(string, ".") != 0 && strcmp(string, "..") != 0)
    {
        if ((*flagPointer) == 0) // flag is used for handling commands Downloads/Test  so that it doesn't copy <somepath>/Downloads as PREVPATH
        {
            strcpy(PREVPATH, CURRPATH);
            *flagPointer = 1;
        }
        if (chdir(string) == 0)
        {
            getcwd(CURRPATH, PATHANDNAMESIZE);
            return;
        }
        else
        {
            if ((*flagPointer) == 1 && chdir(string) != 0)
                chdir(PREVPATH); // for handling strings such as abc/dff  where dff is not a valid directory name
            perror("\033[0;91;49mError:\033[0m");
            return;
        }
    }
}
