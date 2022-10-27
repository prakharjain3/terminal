#ifndef VARIABLE_H

#define VARIABLE_H

#define PATHANDNAMESIZE 1000
#define HISTORYSIZE 20
#define MAX_NO_OF_PROCESS 1000

#include <time.h>

extern char HOSTNAME[PATHANDNAMESIZE];
extern char USERNAME[PATHANDNAMESIZE];
extern char CURRPATH[PATHANDNAMESIZE];
extern char PREVPATH[PATHANDNAMESIZE];
extern char HOMEDIR[PATHANDNAMESIZE];
extern char HISTORYPATH[PATHANDNAMESIZE];
extern int flagForAmpersand;
extern time_t time_taken;

#endif
