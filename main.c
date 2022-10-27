#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "./include/cd.h"
#include "./include/pwdd.h"
#include "./include/echo.h"
#include "./include/variable.h"
#include "./include/prompt.h"
#include "./include/hostnameandusername.h"
#include "./include/ls.h"
#include "./include/history.h"
#include "./include/discover.h"
#include "./include/process.h"
#include "./include/backgroundvariable.h"

struct processNameAndPid proNamAndPid[MAX_NO_OF_PROCESS];

int NO_OF_PROCESSES = 0;

char HOSTNAME[PATHANDNAMESIZE];
char USERNAME[PATHANDNAMESIZE];
char CURRPATH[PATHANDNAMESIZE];
char PREVPATH[PATHANDNAMESIZE];
char HOMEDIR[PATHANDNAMESIZE];
char HISTORYPATH[PATHANDNAMESIZE];
int flagForAmpersand = 0;
int flagForPromptPrintAfterBackground = 0;
int flagForBackgroundRunningProcess = 0;
time_t time_taken = 0;

void main()
{
    system("clear");
    int lenHomeDir = getHostNameAndUserName();

    char history[HISTORYSIZE][PATHANDNAMESIZE];

    strcpy(HISTORYPATH, HOMEDIR);
    strcat(HISTORYPATH, "/history.txt");

    int noOfCommandsInHistory = loadHistory(history);

    getcwd(PREVPATH, PATHANDNAMESIZE); // so that it is not empty in the first run
    getcwd(CURRPATH, PATHANDNAMESIZE); // same

    while (1)
    {

        prompt(lenHomeDir, time_taken);
        time_taken = 0;
        if (flagForBackgroundRunningProcess == 1)
        {
            checkIfProcessDone();
            if (flagForPromptPrintAfterBackground)
            {
                flagForPromptPrintAfterBackground = 0;
                prompt(lenHomeDir, time_taken);
            }
        }

        char command[PATHANDNAMESIZE];
        char *commandCopy = NULL;
        commandCopy = (char *)calloc(PATHANDNAMESIZE, sizeof(char));
        size_t commandsize = PATHANDNAMESIZE;

        getline(&commandCopy, &commandsize, stdin);

        if (!strcmp(commandCopy, "\n"))
            continue;

        strcpy(command, commandCopy);

        // printf("%s", history[noOfCommandsInHistory - 1]);

        if (strcmp(history[noOfCommandsInHistory - 1], command) != 0)
        {
            if (noOfCommandsInHistory == HISTORYSIZE)
            {
                for (int h = 1; h < HISTORYSIZE; h++)
                    strcpy(history[h - 1], history[h]);

                strcpy(history[noOfCommandsInHistory - 1], command);
                writeHistory(history, HISTORYSIZE);
            }
            else
            {
                strcpy(history[noOfCommandsInHistory++], command);
                writeHistory(history, noOfCommandsInHistory);
            }
        }

        flagForAmpersand = 0;

        // for (int iForAmpersand = 0; iForAmpersand < strlen(commandCopy); iForAmpersand++)
        //     if (commandCopy[iForAmpersand] == '&')
        //     {
        //         commandCopy[iForAmpersand] = ';';
        //         flagForAmpersand = 1;
        //     }

        char *tok = commandCopy;

        char *string[PATHANDNAMESIZE];
        memset(string, '\0', PATHANDNAMESIZE);

        char *commaSeparatedCommands[PATHANDNAMESIZE];
        memset(commaSeparatedCommands, '\0', PATHANDNAMESIZE);

        // for (int j = 0; j < PATHANDNAMESIZE; j++)
        // {
        //     string[j] = NULL;
        //     commaSeparatedCommands[j] = NULL;
        // }

        commaSeparatedCommands[0] = strtok_r(tok, ";", &tok);

        int iteratorForCommaSeparatedCommands = 1;
        while ((commaSeparatedCommands[iteratorForCommaSeparatedCommands] = strtok_r(NULL, ";", &tok)))
        {
            iteratorForCommaSeparatedCommands++;
        }
        int iForCmd = 0;

        while (iForCmd < iteratorForCommaSeparatedCommands)
        {

            char *cmd = strtok_r(NULL, " \n", &commaSeparatedCommands[iForCmd]);

            if (cmd == NULL)
                break;

            if (strcmp(cmd, "cd") == 0)
            {
                int i = 0;

                string[i] = strtok_r(NULL, "\n/\\", &commaSeparatedCommands[iForCmd]);

                if (string[i] != NULL) // for handling commands such as cd abc<space> ; echo shis h u uy    fuy
                {
                    for (int k = strlen(string[i]) - 1; k >= 0; k--)
                    {
                        if (*(string[i] + k) == 32)
                            *(string[i] + k) = 0;
                        else
                            break;
                    }

                    int flag = 0;
                    do
                    {
                        changeDirectory(string[i++], &flag);
                    } while ((string[i] = strtok_r(NULL, "\n/\\", &commaSeparatedCommands[iForCmd])));
                }
            }
            else if (!strcmp(cmd, "pwd"))
            {
                presentWorkingDirectory();
            }
            else if (!strcmp(cmd, "echo"))
            {
                echo(commaSeparatedCommands[iForCmd]);
            }
            else if (!strcmp(cmd, "quit") || !strcmp(cmd, "exit"))
            {
                return;
                // printf("\n exit was here \n");
            }
            else if (!strcmp(cmd, "ls"))
            {
                ls(commaSeparatedCommands[iForCmd]);
            }
            else if (!strcmp(cmd, "history"))
            {
                printHistory(history, noOfCommandsInHistory);
            }
            else if (!strcmp(cmd, "discover"))
            {
                discover(commaSeparatedCommands[iForCmd]);
            }
            else if (!strcmp(cmd, "pinfo"))
            {
                pinfo(commaSeparatedCommands[iForCmd]);
            }
            else
            {
                if (process(cmd, commaSeparatedCommands[iForCmd]))
                {
                    // do nothing
                }
                else
                    printf("\033[0;91;49mInvalid Command\033[0m\n");
            }
            iForCmd++;
        }
    }
}
