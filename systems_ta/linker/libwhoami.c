#include "whoami.h"
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

char * getunixname(void)
{
    struct passwd * pwd = getpwuid(getuid());
    return pwd->pw_name;
}

char * getrealname(void)
{
    struct passwd * pwd = getpwuid(getuid());
    return pwd->pw_gecos;
}
