#include "./include/hostnameandusername.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int getHostNameAndUserName()
{
    gethostname(HOSTNAME, PATHANDNAMESIZE);
    getlogin_r(USERNAME, PATHANDNAMESIZE);
    getcwd(HOMEDIR, PATHANDNAMESIZE);

    return strlen(HOMEDIR);
}
