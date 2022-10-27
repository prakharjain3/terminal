#include "./include/pwdd.h"
#include <unistd.h>
#include <stdio.h>

void presentWorkingDirectory()
{
    char currentWorkingDIrectory[PATHANDNAMESIZE];
    getcwd(currentWorkingDIrectory, PATHANDNAMESIZE);
    printf("%s\n", currentWorkingDIrectory);
}
