#ifndef HISTORY_H
#define HISTORY_H

#include "variable.h"

int loadHistory(char [HISTORYSIZE][PATHANDNAMESIZE]);
void printHistory(char [HISTORYSIZE][PATHANDNAMESIZE],int);
void writeHistory();

#endif

