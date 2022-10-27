#include "./include/discover.h"
#include "./include/ls.h" // for alphasortCaseSensitive
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

void discover(char *command)
{
    char currentWorkingDirectoryBuffer[PATHANDNAMESIZE];
    getcwd(currentWorkingDirectoryBuffer, PATHANDNAMESIZE);

    char *string[PATHANDNAMESIZE];
    memset(string, '\0', PATHANDNAMESIZE);

    int i = 0;

    int opf = 0;
    int opd = 0;

    char targetDirectory[PATHANDNAMESIZE];
    memset(targetDirectory, '\0', PATHANDNAMESIZE);

    char fileOrDirName[PATHANDNAMESIZE];
    memset(fileOrDirName, '\0', PATHANDNAMESIZE);

    while ((string[i] = strtok_r(NULL, " \n", &command)))
    {
        if (!strcmp(string[i], "-f"))
        {
            opf = 1;
        }
        else if (!strcmp(string[i], "-d"))
            opd = 1;
        else if (string[i][0] == '\"')
        {
            int len = strlen(string[i]);

            string[i][len - 1] = '\0';

            for (int j = 0; j < len - 1; j++)
                string[i][j] = string[i][j + 1];

            strcpy(fileOrDirName, string[i]);
        }
        else
            strcpy(targetDirectory, string[i]);
        i++;
    }

    // if (opf)
    // {
    //     realDiscoverForFile(targetDirectory, fileOrDirName);
    // }
    // else if (opd)
    // {
    //     realDiscoverForDir(targetDirectory, fileOrDirName);
    // }
    // else if (opd && opf)

    int flag = 0;

    realDiscover(targetDirectory, fileOrDirName, opf, opd, flag);

    chdir(currentWorkingDirectoryBuffer);
}

void realDiscover(char *targetdirectory, char *fileordirname, int optf, int optd, int flag)
{
    if (!strcmp(targetdirectory, "~"))
        strcpy(targetdirectory, HOMEDIR);
    if (targetdirectory[0] == '\0')
        strcpy(targetdirectory, ".");

    struct stat targetDirectoryAttributes;

    // for input of the type #### discover . "osn.txt" ####  AND #### discover ./OSN "osn.txt" ####
    if (!optf && !optd && stat(targetdirectory, &targetDirectoryAttributes) != -1 && S_ISDIR(targetDirectoryAttributes.st_mode) != 0 && fileordirname[0] != '\0')
    {
        char pathStoreBuffer[PATHANDNAMESIZE];
        getcwd(pathStoreBuffer, PATHANDNAMESIZE);
        if (chdir(targetdirectory) == 0)
        {

            struct dirent **namelist;
            int n;

            n = scandir(".", &namelist, NULL, alphasortCaseInsensitive);

            for (int i = 0; i < n; i++)
            {
                struct stat fileAttributes;
                stat(namelist[i]->d_name, &fileAttributes);

                // printf("%s\n", namelist[i]->d_name);

                if (!strcmp(fileordirname, namelist[i]->d_name) && S_ISREG(fileAttributes.st_mode) != 0)
                {
                    char printBuffer[PATHANDNAMESIZE];
                    getcwd(printBuffer, PATHANDNAMESIZE);
                    // int offset = strlen(printBuffer);

                    strcat(printBuffer, "/");
                    strcat(printBuffer, namelist[i]->d_name);
                    if (strlen(HOMEDIR) <= strlen(printBuffer))
                        printf(".%s\n", printBuffer + strlen(HOMEDIR));
                    else
                        printf("%s\n", printBuffer);
                    // return;
                }
                else if (S_ISDIR(fileAttributes.st_mode) != 0 && strcmp(namelist[i]->d_name, ".") != 0 && strcmp(namelist[i]->d_name, "..") != 0)
                    realDiscover(namelist[i]->d_name, fileordirname, optf, optd, flag);
                free(namelist[i]);
                // else
                // printf("\033[0;91;49mFile Not Found\n\033[0m");
            }
            chdir(pathStoreBuffer);
        }
    }

    // for input of the type #### discover #### AND #### discover -d -f ####
    else if ((optf && optd || !optf && !optd) && fileordirname[0] == '\0')
    {
        char pathStoreBuffer[PATHANDNAMESIZE];
        getcwd(pathStoreBuffer, PATHANDNAMESIZE);
        if (chdir(targetdirectory) == 0)
        {

            struct dirent **namelist;
            int n;

            n = scandir(".", &namelist, NULL, alphasortCaseInsensitive);

            for (int i = 0; i < n; i++)
            {
                if (flag == 0 && !strcmp(namelist[i]->d_name, "."))
                {
                    printf("%s\n", namelist[i]->d_name);
                    flag = 1;
                }
                if (!strcmp(namelist[i]->d_name, "..") || !strcmp(namelist[i]->d_name, "."))
                    continue;
                struct stat fileAttributes;
                stat(namelist[i]->d_name, &fileAttributes);

                char printBuffer[PATHANDNAMESIZE];
                getcwd(printBuffer, PATHANDNAMESIZE);

                strcat(printBuffer, "/");
                strcat(printBuffer, namelist[i]->d_name);
                if (strlen(HOMEDIR) <= strlen(printBuffer))
                    printf(".%s\n", printBuffer + strlen(HOMEDIR));
                else
                    printf("%s\n", printBuffer);

                if (S_ISDIR(fileAttributes.st_mode) != 0 && strcmp(namelist[i]->d_name, ".") != 0 && strcmp(namelist[i]->d_name, "..") != 0)
                    realDiscover(namelist[i]->d_name, fileordirname, optf, optd, flag);
                free(namelist[i]);
            }
            chdir(pathStoreBuffer);
        }
    }
    // for input of the type #### discover -d #### AND #### discover ./OSN -d ####
    else if (optd)
    {
        char pathStoreBuffer[PATHANDNAMESIZE];
        getcwd(pathStoreBuffer, PATHANDNAMESIZE);
        if (chdir(targetdirectory) == 0)
        {

            struct dirent **namelist;
            int n;

            n = scandir(".", &namelist, NULL, alphasortCaseInsensitive);

            for (int i = 0; i < n; i++)
            {
                if (flag == 0 && !strcmp(namelist[i]->d_name, "."))
                {
                    printf("%s\n", namelist[i]->d_name);
                    flag = 1;
                }
                if (!strcmp(namelist[i]->d_name, "..") || !strcmp(namelist[i]->d_name, "."))
                    continue;
                struct stat fileAttributes;
                stat(namelist[i]->d_name, &fileAttributes);

                if (S_ISDIR(fileAttributes.st_mode))
                {

                    char printBuffer[PATHANDNAMESIZE];
                    getcwd(printBuffer, PATHANDNAMESIZE);

                    strcat(printBuffer, "/");
                    strcat(printBuffer, namelist[i]->d_name);
                    if (strlen(HOMEDIR) <= strlen(printBuffer))
                        printf(".%s\n", printBuffer + strlen(HOMEDIR));
                    else
                        printf("%s\n", printBuffer);
                }

                if (S_ISDIR(fileAttributes.st_mode) != 0 && strcmp(namelist[i]->d_name, ".") != 0 && strcmp(namelist[i]->d_name, "..") != 0)
                    realDiscover(namelist[i]->d_name, fileordirname, optf, optd, flag);
                free(namelist[i]);
            }
            chdir(pathStoreBuffer);
        }
    }
    // for input of the type #### discover -f #### AND #### discover ./OSN -f #####
    else if (optf)
    {
        char pathStoreBuffer[PATHANDNAMESIZE];
        getcwd(pathStoreBuffer, PATHANDNAMESIZE);
        if (chdir(targetdirectory) == 0)
        {

            struct dirent **namelist;
            int n;

            n = scandir(".", &namelist, NULL, alphasortCaseInsensitive);

            for (int i = 0; i < n; i++)
            {
                if (!strcmp(namelist[i]->d_name, "..") || !strcmp(namelist[i]->d_name, "."))
                    continue;
                struct stat fileAttributes;
                stat(namelist[i]->d_name, &fileAttributes);

                if (S_ISREG(fileAttributes.st_mode))
                {

                    char printBuffer[PATHANDNAMESIZE];
                    getcwd(printBuffer, PATHANDNAMESIZE);

                    strcat(printBuffer, "/");
                    strcat(printBuffer, namelist[i]->d_name);
                    if (strlen(HOMEDIR) <= strlen(printBuffer))
                        printf(".%s\n", printBuffer + strlen(HOMEDIR));
                    else
                        printf("%s\n", printBuffer);
                }

                if (S_ISDIR(fileAttributes.st_mode) != 0 && strcmp(namelist[i]->d_name, ".") != 0 && strcmp(namelist[i]->d_name, "..") != 0)
                    realDiscover(namelist[i]->d_name, fileordirname, optf, optd, flag);

                free(namelist[i]);
            }
            chdir(pathStoreBuffer);
        }
    }
    return;
}

/*
int alphasort_case_insensitive(const struct dirent **a, const struct dirent **b)
{
    return (strcasecmp((*(const struct dirent **)a)->d_name, (*(const struct dirent **)b)->d_name));
}
*/