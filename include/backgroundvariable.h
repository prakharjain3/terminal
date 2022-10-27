#ifndef BACKGOUND_H
#define BACKGROUND_H

#include "variable.h"

struct processNameAndPid{
    char name[PATHANDNAMESIZE];
    int pid;
};

extern struct processNameAndPid proNamAndPid[MAX_NO_OF_PROCESS];

extern int  NO_OF_PROCESSES;

extern int flagForPromptPrintAfterBackground;

extern int flagForBackgroundRunningProcess;

#endif