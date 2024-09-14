#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *filePath, char *target)
{
  char buf[512], *p, *fileName;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(filePath, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", filePath);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", filePath);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    fileName = filePath + strlen(filePath) - strlen(target);
    if (strcmp(fileName, target) == 0){
      printf("%s\n", filePath);
    }
    break;

  case T_DIR:
    if(strlen(filePath) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, filePath);
    p = buf + strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      if (strcmp(buf+strlen(buf)-2,"/.")!=0 && strcmp(buf+strlen(buf)-3,"/..")!=0){
        find(buf,target);
      }
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[])
{
  if(argc < 3){
    exit(0);
  }
  char target[512];
  target[0] = '/';
  strcpy(target, argv[2]);
  find(argv[1], target);
  exit(0);
}