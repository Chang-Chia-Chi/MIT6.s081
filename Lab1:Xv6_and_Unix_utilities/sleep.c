#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(2, "Usage: sleep <number>\n");
        exit(1);
    }

    int n;
    n = atoi(argv[1]);
    sleep(n);
    exit(0);
}