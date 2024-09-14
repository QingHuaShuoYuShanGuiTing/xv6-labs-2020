#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void run(char *program, char **args){
    if (fork() == 0)
    {
        exec(program,args);
        exit(0);
    }
    //return;
}

int main(int argc, char *argv[]){
    char buf[1024];
    char *curPtr = buf, *lastPtr = buf;
    char *argsBuf[128];
    char **args = argsBuf;

    for (int i = 1; i < argc; i++){
        *args = argv[i];
        args++;
    }
    char **argsPtr = args;
    while (read(0, curPtr, 1) != 0){
        if (*curPtr == ' ' ){
            *curPtr = '\0';
            *argsPtr = lastPtr;
            argsPtr++;
            lastPtr = curPtr + 1;
        }
        else if (*curPtr == '\n'){
            *curPtr = '\0';
            *argsPtr = lastPtr;
            argsPtr++;
            *argsPtr = 0;
            lastPtr = curPtr + 1;
            run(argv[1], argsBuf);
            argsPtr = args;
        }
        curPtr++;
    }
    while(wait(0) != -1); 

    exit(0);
}