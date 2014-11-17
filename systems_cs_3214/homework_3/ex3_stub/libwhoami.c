#include "whoami.h"
#include <unistd.h>
#include <pwd.h>

/* Get the name of the current user */
char * getunixname(void) {
	uid_t uid = getuid();
	return getpwuid(uid)->pw_name;
}

/* Get the real name of the current user */
char * getrealname(void) {
	uid_t uid = getuid();
	return getpwuid(uid)->pw_gecos;
}
