#ifndef DISCOVER_H
#define DISCOVER_H

#include "variable.h"
#include <dirent.h>

void realDiscover(char *, char *, int, int, int);
// int alphasort_case_insensitive(const struct dirent **, const struct dirent **);
void discover(char *);

#endif