#ifndef LS_H

#define LS_H

#include "variable.h"
#include <sys/stat.h>
#include <dirent.h>

void ls(char *);
int rls(char *, int, int);
void permission(struct stat *, char *);
int alphasortCaseInsensitive(const struct dirent **, const struct dirent **); // comparator function used in scandir

#endif