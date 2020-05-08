#include "types.h"
#include "user.h"
#include "stat.h"
#include "fcntl.h"
#include "fs.h"
#define GetCurrentDir getcwd

char* my_strcat(char* destination, const char* source)
{
	// make ptr point to the end of destination string
	char* ptr = destination + strlen(destination);

	// Appends characters of source to the destination string
	while (*source != '\0')
		*ptr++ = *source++;

	// null terminate destination string
	*ptr = '\0';

	// destination is returned by standard strcat()
	return destination;
}


char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void ls(char *path){

    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        printf(2, "ls: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        printf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_FILE:
        printf(1, "%s%s\n",path,fmtname(path));
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf(1, "ls: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0)
            {
                printf(1, "ls: cannot stat %s\n", buf);
                continue;
            }
            else {
              printf(1, "%s/%s\n",path,fmtname(buf));
            }
            if (st.type == 1){
                my_strcat(buf,"/");
                ls(buf);
            }
        }
        break;
    }
    close(fd);
}

void findf(char *path, char *name){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        printf(2, "ls: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        printf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_FILE:
        if(strcmp(fmtname(path),name)==32){
          printf(1, "%s%s\n",fmtname(path));
        }
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf(1, "ls: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0)
            {
                printf(1, "ls: cannot stat %s\n", buf);
                continue;
            }
            else {
              if(strcmp(fmtname(buf),name)==32){
                  printf(1, "%s/%s\n",path,fmtname(buf));
              }
            }
            if (st.type == 1){
                my_strcat(buf,"/");
                findf(buf, name);
            }
        }
        break;
    }
    close(fd);
}

int main(int argc, char * argv[]){
  if(argc<2){
    ls(".");
    exit();
  }
  int k;
  for(k=1;k<argc;k++){
    if(strcmp(argv[k],"-n")==0){
      //printf(1,"%s\n%s\n",argv[k],argv[k+1]);
      findf(".",argv[k+1]);
    }
    else if(strcmp(argv[k],"-d")==0){
      ls(argv[k+1]);
    }
  }
  return 0;
}
