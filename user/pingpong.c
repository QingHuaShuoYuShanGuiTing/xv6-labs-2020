#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
    int p2s[2];
    int s2p[2];

    pipe(p2s);
    pipe(s2p);

    char buff[] = {'x'};

    int pid = fork();

    if (pid == 0){
        close(p2s[1]);
        close(s2p[0]);

        read(p2s[0], buff, 1);
        printf("%d: received ping\n", getpid());
        write(s2p[1], buff, 1);
        exit(0);
    }
    else{
        close(p2s[0]);
        close(s2p[1]);
         
        write(p2s[1], buff, 1);
        read(s2p[0], buff, 1);
        printf("%d: received pong\n", getpid());
        wait(0);
        exit(0);
    }
}