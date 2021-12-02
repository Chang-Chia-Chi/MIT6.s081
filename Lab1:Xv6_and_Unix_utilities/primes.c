#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void pipeline(int lfd) {
    int rfd[2];
    int nb, num, pid, prime, status;
    nb = read(lfd, &prime, sizeof(int));
    if (!nb)      /* last block */
        return;

    printf("prime %d\n", prime);
    pipe(rfd);
    if ((pid=fork()) == 0)
    {
        close(rfd[1]);
        pipeline(rfd[0]);
        close(rfd[0]);
        exit(0);
    }
    else
    {
        close(rfd[0]);
        while ((nb=read(lfd, &num, sizeof(int)))) {
            if (num % prime)
                write(rfd[1], &num, sizeof(int));
        }
        close(rfd[1]);
    }
    wait(&status);
}

void main(int argc, char *argv[]) {
    int fd[2];
    int n, pid, status;
    pipe(fd);
    if ((pid=fork()) == 0)
    {   
        close(fd[1]);
        pipeline(fd[0]);
        close(fd[0]);
        exit(0);
    }
    else
    {
        close(fd[0]);
        for (n = 2; n <= 35; n++) {
            write(fd[1], &n, sizeof(int));
        }
        close(fd[1]);
    }
    wait(&status);
    exit(0);
}