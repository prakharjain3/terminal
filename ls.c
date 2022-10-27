#include "./include/ls.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

void ls(char *command)
{
    char *string[PATHANDNAMESIZE];
    memset(string, '\0', PATHANDNAMESIZE);

    // for (int j = 0; j < PATHANDNAMESIZE; j++)
    //     string[j] = NULL;

    int iForString = 0;
    int opa = 0;
    int opl = 0;

    int count; // when there are more than two directory to print name if only one dont print name
    while ((string[iForString] = strtok_r(NULL, " \n", &command)))
    {
        if (!strcmp(string[iForString], "-a"))
        {
            opa = 1;
            count++;
        }
        if (!strcmp(string[iForString], "-l"))
        {
            opl = 1;
            count++;
        }
        if (!strcmp(string[iForString], "-la") || !strcmp(string[iForString], "-al"))
        {
            opl = 1;
            opa = 1;
            count++;
        }
        iForString++;
    }

    int k = 0;
    // int flagForDirFileLS = 1;

    if ((string[k] == NULL) || (!strcmp(string[k], "-l") && iForString == 1) || (!strcmp(string[k], "-a") && iForString == 1) || (!strcmp(string[k], "-al") && iForString == 1) || (!strcmp(string[k], "-la") && iForString == 1) || (!strcmp(string[k], "-l") && !strcmp(string[k + 1], "-a") && iForString == 2) || (!strcmp(string[k], "-a") && !strcmp(string[k + 1], "-l") && iForString == 2))
    {
        char dirNULL[PATHANDNAMESIZE];
        getcwd(dirNULL, PATHANDNAMESIZE);
        rls(dirNULL, opa, opl);
    }

    while (k < iForString)
    {
        if (!strcmp(string[k], "~"))
        {
            rls(HOMEDIR, opa, opl);
        }
        else if (strcmp(string[k], "-l") != 0 && strcmp(string[k], "-a") != 0 && strcmp(string[k], "-la") != 0 && strcmp(string[k], "-al") != 0)
        {
            struct stat dirStat;
            stat(string[k], &dirStat);
            if ((iForString - count) > 1 && S_ISDIR(dirStat.st_mode)) // flag count and dir or file count
                printf("%s:\n", string[k]);

            if(rls(string[k], opa, opl) == -1)
            {
                return;
            }
        }
        k++;
    }
}

int rls(char *dir, int opa, int opl)
{
    // Here we will list the directory
    // DIR *dh = opendir(dir); // was trying to implement using readdir

    struct stat fileAttributes;

    if (stat(dir, &fileAttributes) != -1 && !S_ISDIR(fileAttributes.st_mode))
    {
        if (S_ISREG(fileAttributes.st_mode)) // if it is a single file
        {
            char permBuffer[10];
            if (opl)
            {
                struct passwd *pw = getpwuid(fileAttributes.st_uid);
                struct group *grp = getgrgid(fileAttributes.st_gid);
                // char permBuffer[PATHANDNAMESIZE];
                permission(&fileAttributes, permBuffer);
                char setColour[20];
                char endColour[20];
                setColour[0] = '\0';
                endColour[0] = '\0';

                if (permBuffer[0] == 'd')
                {
                    strcpy(setColour, "\033[1;94;49m");
                    strcpy(endColour, "\033[0m");
                }
                else if (permBuffer[3] == 'x' && permBuffer[6] == 'x' && permBuffer[9] == 'x')
                {
                    strcpy(setColour, "\033[1;92;49m");
                    strcpy(endColour, "\033[0m");
                }
                else
                {
                    strcpy(setColour, "\033[1;97;49m");
                    strcpy(endColour, "\033[0m");
                }

                struct tm t;
                localtime_r(&fileAttributes.st_mtime, &t);
                char timeBuffer[100];

                strftime(timeBuffer, 100, "%b %e %R", &t);

                printf("%s \t %lu \t %s \t %s \t %ld \t %s \t %s%s%s \n", permBuffer, fileAttributes.st_nlink, pw->pw_name, grp->gr_name, fileAttributes.st_size, timeBuffer, setColour, dir, endColour);
            }
            else if (!opl)
            {
                permission(&fileAttributes, permBuffer);
                char setColour[20];
                char endColour[20];
                setColour[0] = '\0';
                endColour[0] = '\0';

                if (permBuffer[0] == 'd')
                {
                    strcpy(setColour, "\033[1;94;49m");
                    strcpy(endColour, "\033[0m");
                }
                else if (permBuffer[3] == 'x' && permBuffer[6] == 'x' && permBuffer[9] == 'x')
                {
                    strcpy(setColour, "\033[1;92;49m");
                    strcpy(endColour, "\033[0m");
                }
                else
                {
                    strcpy(setColour, "\033[1;97;49m");
                    strcpy(endColour, "\033[0m");
                }
                printf("%s%s%s", setColour, dir, endColour);
            }
            return 0;
        }
    }
    else if(!S_ISDIR(fileAttributes.st_mode))
    {
        perror("\033[0;91;49mFile doesn't exist\033[0m");
        return -1;
    }

    blkcnt_t total_blocks = 0;
    struct dirent **d;

    char currentDirectoryBuffer[PATHANDNAMESIZE];

    getcwd(currentDirectoryBuffer, PATHANDNAMESIZE);

    if (chdir(dir) == 0) // for everything inside a directory
    {
        struct stat directoryAttributes;

        char dirPath[PATHANDNAMESIZE];

        getcwd(dirPath, PATHANDNAMESIZE);

        int n = scandir(dirPath, &d, NULL, alphasortCaseInsensitive);

        // while ((d = readdir(dh)) != NULL)
        for (int i = 0; i < n; i++)
        {
            if (stat(d[i]->d_name, &directoryAttributes) != -1)
            {

                char *permBuffer;
                permBuffer = (char *)calloc(10, sizeof(char));
                if (opl && opa)
                {
                    // char permBuffer[PATHANDNAMESIZE];
                    struct passwd *pw = getpwuid(directoryAttributes.st_uid);
                    struct group *grp = getgrgid(directoryAttributes.st_gid);

                    // If pw != 0, pw->pw_name contains the user name
                    // If gr != 0, gr->gr_name contains the group name
                    permission(&directoryAttributes, permBuffer);

                    char setColour[20];
                    char endColour[20];
                    setColour[0] = '\0';
                    endColour[0] = '\0';

                    if (permBuffer[0] == 'd')
                    {
                        strcpy(setColour, "\033[1;94;49m");
                        strcpy(endColour, "\033[0m");
                    }
                    else if (permBuffer[3] == 'x' && permBuffer[6] == 'x' && permBuffer[9] == 'x')
                    {
                        strcpy(setColour, "\033[1;92;49m");
                        strcpy(endColour, "\033[0m");
                    }
                    else
                    {
                        strcpy(setColour, "\033[1;97;49m");
                        strcpy(endColour, "\033[0m");
                    }

                    struct tm t;
                    localtime_r(&directoryAttributes.st_mtime, &t);
                    char timeBuffer[100];

                    strftime(timeBuffer, 100, "%b %e %R", &t);

                    printf("%s \t %lu \t %s \t %s \t %ld \t %s \t %s%s%s \n", permBuffer, directoryAttributes.st_nlink, pw->pw_name, grp->gr_name, directoryAttributes.st_size, timeBuffer, setColour, d[i]->d_name, endColour);
                    total_blocks += directoryAttributes.st_blocks;
                }
                else if (opa && !opl)
                {
                    // char permBuffer[PATHANDNAMESIZE];

                    char setColour[20];
                    char endColour[20];
                    setColour[0] = '\0';
                    endColour[0] = '\0';

                    if (S_ISDIR(directoryAttributes.st_mode))
                    {
                        strcpy(setColour, "\033[1;94;49m");
                        strcpy(endColour, "\033[0m");
                    }
                    else if ((directoryAttributes.st_mode & S_IXUSR) && (directoryAttributes.st_mode & S_IXGRP) && (directoryAttributes.st_mode & S_IXOTH))
                    {
                        strcpy(setColour, "\033[1;92;49m");
                        strcpy(endColour, "\033[0m");
                    }
                    else
                    {
                        strcpy(setColour, "\033[1;97;49m");
                        strcpy(endColour, "\033[0m");
                    }

                    printf("%s%s%s\n", setColour, d[i]->d_name, endColour);
                    total_blocks += directoryAttributes.st_blocks;
                }
                else if (!opa && opl)
                {
                    // char permBuffer[PATHANDNAMESIZE];

                    struct passwd *pw = getpwuid(directoryAttributes.st_uid);
                    struct group *grp = getgrgid(directoryAttributes.st_gid);

                    // If pw != 0, pw->pw_name contains the user name
                    // If gr != 0, gr->gr_name contains the group name
                    if (d[i]->d_name[0] == '.')
                        continue;

                    permission(&directoryAttributes, permBuffer);

                    char setColour[20];
                    char endColour[20];
                    setColour[0] = '\0';
                    endColour[0] = '\0';

                    if (permBuffer[0] == 'd')
                    {
                        strcpy(setColour, "\033[1;94;49m");
                        strcpy(endColour, "\033[0m");
                    }
                    else if (permBuffer[3] == 'x' && permBuffer[6] == 'x' && permBuffer[9] == 'x')
                    {
                        strcpy(setColour, "\033[1;92;49m");
                        strcpy(endColour, "\033[0m");
                    }
                    else
                    {
                        strcpy(setColour, "\033[1;97;49m");
                        strcpy(endColour, "\033[0m");
                    }

                    printf("%s \t %lu \t %s \t %s \t %ld \t %s%s%s \n", permBuffer, directoryAttributes.st_nlink, pw->pw_name, grp->gr_name, directoryAttributes.st_size, setColour, d[i]->d_name, endColour);
                    total_blocks += directoryAttributes.st_blocks;
                }
                else if (!opa && !opl)
                {
                    if (d[i]->d_name[0] == '.')
                        continue;

                    char setColour[20];
                    char endColour[20];
                    setColour[0] = '\0';
                    endColour[0] = '\0';

                    if (S_ISDIR(directoryAttributes.st_mode))
                    {
                        strcpy(setColour, "\033[1;94;49m");
                        strcpy(endColour, "\033[0m");
                    }
                    else if ((directoryAttributes.st_mode & S_IXUSR) && (directoryAttributes.st_mode & S_IXGRP) && (directoryAttributes.st_mode & S_IXOTH))
                    {
                        strcpy(setColour, "\033[1;92;49m");
                        strcpy(endColour, "\033[0m");
                    }
                    else
                    {
                        strcpy(setColour, "\033[1;97;49m");
                        strcpy(endColour, "\033[0m");
                    }
                    printf("%s%s%s\n", setColour, d[i]->d_name, endColour);
                    total_blocks += directoryAttributes.st_blocks;
                }
            }
            free(d[i]);
        }
        if (opl)
            printf("total : %ld\n", total_blocks / 2);
        chdir(currentDirectoryBuffer);
    }
    else
    {
        perror("\033[0;91;49mDirectory doesn't exist\033[0m");
    return -1;
    }
    return 0;
}

void permission(struct stat *fileattributes, char *permissionBuffer)
{
    memset(permissionBuffer, 0, 10);

    if (S_ISREG(fileattributes->st_mode) != 0)
        strcat(permissionBuffer, "-");
    else if (S_ISLNK(fileattributes->st_mode) != 0)
        strcat(permissionBuffer, "l");
    else if (S_ISDIR(fileattributes->st_mode) != 0)
        strcat(permissionBuffer, "d");
    else if (S_ISBLK(fileattributes->st_mode) != 0)
        strcat(permissionBuffer, "b");
    else if (S_ISCHR(fileattributes->st_mode) != 0)
        strcat(permissionBuffer, "c");
    else if (S_ISFIFO(fileattributes->st_mode) != 0)
        strcat(permissionBuffer, "|");

    if (fileattributes->st_mode & S_IRUSR)
        strcat(permissionBuffer, "r");
    else
        strcat(permissionBuffer, "-");

    if (fileattributes->st_mode & S_IWUSR)
        strcat(permissionBuffer, "w");
    else
        strcat(permissionBuffer, "-");

    if (fileattributes->st_mode & S_IXUSR)
        strcat(permissionBuffer, "x");
    else
        strcat(permissionBuffer, "-");

    if (fileattributes->st_mode & S_IRGRP)
        strcat(permissionBuffer, "r");
    else
        strcat(permissionBuffer, "-");

    if (fileattributes->st_mode & S_IWGRP)
        strcat(permissionBuffer, "w");
    else
        strcat(permissionBuffer, "-");

    if (fileattributes->st_mode & S_IXGRP)
        strcat(permissionBuffer, "x");
    else
        strcat(permissionBuffer, "-");

    if (fileattributes->st_mode & S_IROTH)
        strcat(permissionBuffer, "r");
    else
        strcat(permissionBuffer, "-");

    if (fileattributes->st_mode & S_IWOTH)
        strcat(permissionBuffer, "w");
    else
        strcat(permissionBuffer, "-");

    if (fileattributes->st_mode & S_IXOTH)
        strcat(permissionBuffer, "x");
    else
        strcat(permissionBuffer, "-");

    return;
}

int alphasortCaseInsensitive(const struct dirent **a, const struct dirent **b)
{
    return (strcasecmp((*(const struct dirent **)a)->d_name, (*(const struct dirent **)b)->d_name));
}
