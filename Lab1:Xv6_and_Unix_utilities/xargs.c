#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

void xargs(char *argv[]) {
    int pid, status;
    if ((pid=fork()) == 0) {
        exec(argv[0], argv);
        exit(1);
    }
    wait(&status);
    return;
}

void main(int argc, char *argv[]) {

    int i, j;
    char c, buf[512], *xargv[MAXARG];
    if (argc < 2 || argc - 1 > MAXARG) {
        fprintf(2, "usage: xargs <cmd> {args}, # of args should be less than 32\n");
        exit(1);
    }

    memset(buf, 0, sizeof(buf));
    for (i = 1; i < argc; i++)
        xargv[i - 1] = argv[i];
    for (; i < MAXARG ; i++)
        xargv[i] = 0;

    j = 0;
    while (read(0, &c, 1) > 0) {
        if (c != '\n')
            buf[j++] = c;
        else {
            if (j != 0) {
                buf[j] = '\0';
                xargv[argc - 1] = buf;
                xargs(xargv);
                j = 0;
            }
        }
    }
    if (j != 0) {
        buf[j] = '\0';
        xargv[argc - 1] = buf;
        xargs(xargv);
    }
    exit(0);
}