#include "./include/process.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>

#include "./include/prompt.h"
#include "./include/backgroundvariable.h"

int process(char *cmd, char *commandAfterCMD)
{
    char *string[PATHANDNAMESIZE];
    memset(string, 0, PATHANDNAMESIZE);

    // char *execarg1 = strtok_r(commandAfterCMD, " \n",&commandAfterCMD);

    string[0] = cmd;

    for (int iForAmpersand = 0; iForAmpersand < strlen(commandAfterCMD); iForAmpersand++)
        if (commandAfterCMD[iForAmpersand] == '&')
        {
            commandAfterCMD[iForAmpersand] = '\n';
            flagForAmpersand = 1;
        }

    int i = 1;

    while ((string[i] = strtok_r(NULL, " \n", &commandAfterCMD)))
    {
        i++;
    }

    // for (int j = i; j >= 1; j--) // for copying cmd to string[0]
    //     string[j] = string[j - 1];

    // strcpy(string[0], cmd);

    time_t start_t, end_t;
    int status;

    int pid = fork(); // have to use fork() for multiple processes as execvp (used without fork() exits the program)

    if (pid < 0)
    {
        perror("\033[0;91;49mUnable to fork process\033[0m\n");
        return -1;
    }

    if (pid == 0)
    {
        // start_t = clock();
        if (execvp(cmd, string) == -1)
        {
            printf("\033[0;91;49mUnable to  Execute Command\033[0m\n");
            return -1;
        }
    }
    else
    {
        if (flagForAmpersand == 1)
        {
            printf("[%d] %d\n", NO_OF_PROCESSES + 1, pid);
            proNamAndPid[NO_OF_PROCESSES].pid = pid;
            strcpy(proNamAndPid[NO_OF_PROCESSES++].name, cmd);
            flagForBackgroundRunningProcess = 1;
        }
        else
        {
            // clock_t childTime = time(NULL);
            time(&start_t);
            do
            {
                waitpid(pid, &status, 0);
            } while (!WIFEXITED(status));
            time(&end_t);
            time_taken = end_t - start_t;
        }
        return 1;
    }
}

void checkIfProcessDone()
{
    int status;
    int checkpid = waitpid(-1, &status, WNOHANG);

    if (checkpid < 0)
    {
        if (NO_OF_PROCESSES == 0)
            flagForBackgroundRunningProcess = 0;
        perror("waitpid error\n");
        prompt(strlen(HOMEDIR), time_taken);
        return;
    }
    if (checkpid > 0 && WIFEXITED(status))
    {
        int iForCheckPid;

        for (iForCheckPid = 0; iForCheckPid < NO_OF_PROCESSES; iForCheckPid++)
            if (checkpid == proNamAndPid[iForCheckPid].pid)
                break;

        if (WEXITSTATUS(status) == 0 || WEXITSTATUS(status) == EXIT_SUCCESS)
            fprintf(stderr, "\n%s with %d exited normally\n", proNamAndPid[iForCheckPid].name, proNamAndPid[iForCheckPid].pid);
        else
            fprintf(stderr, "\n%s with %d exited abnormally\n", proNamAndPid[iForCheckPid].name, proNamAndPid[iForCheckPid].pid);

        for (int iForChangingStructure = iForCheckPid; iForChangingStructure < NO_OF_PROCESSES - 1; iForChangingStructure++)
        {
            strcpy(proNamAndPid[iForChangingStructure].name, proNamAndPid[iForChangingStructure + 1].name);
            proNamAndPid[iForChangingStructure].pid = proNamAndPid[iForChangingStructure + 1].pid;
        }

        flagForPromptPrintAfterBackground = 1;

        NO_OF_PROCESSES--;

        if (NO_OF_PROCESSES == 0)
            flagForBackgroundRunningProcess = 0;
    }
}

void pinfo(char *pid)
{
    pid = strtok_r(NULL, " \n", &pid);

    if (pid == NULL)
    {
        for (int i = 0; i < NO_OF_PROCESSES; i++)
        {
            realpinfo(proNamAndPid[i].pid);
        }
    }
    else
    {
        realpinfo(atoi(pid));
    }
    return;
}

void realpinfo(int pid_2)
{
    char pinfoBuffer[PATHANDNAMESIZE];
    char pinfoBuffer2[PATHANDNAMESIZE];
    char pinfoBuffer3[PATHANDNAMESIZE];

    sprintf(pinfoBuffer, "/proc/%d/stat", pid_2);
    sprintf(pinfoBuffer2, "/proc/%d/statm", pid_2);
    sprintf(pinfoBuffer3, "/proc/%d/exe", pid_2);

    FILE *fstat = fopen(pinfoBuffer, "r");
    FILE *fstatm = fopen(pinfoBuffer2, "r");

    if (!fstat || !fstatm)
    {
        perror("Unable to Collect Process Information\n");
        return;
    }

    int virtualMemory;
    char exepath[PATHANDNAMESIZE];
    char abcd[PATHANDNAMESIZE]; // for reading stat file

    char *tok[3]; // for reading pid and status from stat file

    fgets(abcd, PATHANDNAMESIZE, fstat);

    char *token = abcd;

    for (int i = 0; i < 3; i++)
        tok[i] = strtok_r(token, " ", &token);

    fscanf(fstatm, "%d", &virtualMemory);
    int len = readlink(pinfoBuffer3, exepath, PATHANDNAMESIZE);

    if (len == -1)
    {
        perror("Unable to Collect Process Information\n");
        return;
    }

    printf("Pid : %s\n", tok[0]);

    printf("Process Status : %s\n", tok[2]);
    printf("Virtual Memory : %d\n", virtualMemory);
    printf("Executable Path : %s\n", exepath);
}