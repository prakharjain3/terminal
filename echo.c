#include "./include/echo.h"
#include <stdio.h>
#include <string.h>

void echo(char *tok)
{
    char *string;
    while ((string = strtok_r(NULL, " \t", &tok)))
    {
        printf("%s ", string);
    }
}
