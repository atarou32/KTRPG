#define MAXFD 64
#include "daemon.h"

int daemonize(int nochdir, int noclose) {
  int i,fd;
  pid_t pid;
  if ((pid = fork()) == -1) {
    return -1;
  } else if (pid != 0) {
    _exit(0);
  }

  setsid();
  signal(SIGHUP, SIG_IGN);
  if ((pid = fork()) != 0) {
    _exit(0);
  }

  if (nochdir == 0) {
    chdir("/");
  }
  if (noclose == 0) {
    for (i = 0;i<MAXFD; i++) {
      close(i);
    }
    if ((fd = open("/dev/null", O_RDWR, 0)) != -1) {
      dup2(fd,0);
      dup2(fd,1);
      dup2(fd,2);
      if(fd > 2) {
        close(fd);
      }
    }
  }
  return 0;
}

    
