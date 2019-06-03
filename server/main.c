
#include "server.h"
#include "daemon.h"
#include "syslog.h"
#include "my_handler.h"
#include "my_mysql.h"

volatile int g_gotpipe;
volatile int g_gotint;
volatile int g_gotalrm;
volatile int g_gotterm;
volatile int g_gothangup;



int main(int argc, char** argv) {
  int soc, i;
  char buf[256];
  g_gotpipe = 0;
  g_gotint = 0;
  g_gotalrm = 0;
  g_gotterm = 0;
  g_gothangup = 0;

  if (argc <=2) {
    fprintf(stderr, "server port daemon_flg \n");
    return EX_USAGE;
  }

  i = atoi(argv[2]);
  if (i != 0) {
    daemonize(1,0);
  }
  syslog(LOG_USER|LOG_NOTICE, "daemon:cwd%s\n", getcwd(buf, sizeof(buf)));
  set_my_sig_action();
 
  if ((soc = server_socket(argv[1])) == -1) {
    fprintf(stderr, "server_socket(%s):error\n",argv[1]);
    return EX_UNAVAILABLE;
  }
  fprintf(stderr, "ready for accept\n");
  accept_loop(soc);
  close(soc);
  return EX_OK;
}


