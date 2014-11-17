#include <stdio.h>

void print_pid(char* format, int pid) {

    char buf[1000];
    int nc = snprintf(buf, 1000, format, pid);
    write(1,buf, nc);
}
