#include "my_handler.h"
#include <signal.h>
#include <stdio.h>
#include <sysexits.h>
#include <unistd.h>


void sig_hangup_handler(int sig) {
  g_gothangup = sig;
}
void sig_pipe_handler(int sig) {
  g_gotpipe = sig;
}

void sig_alrm_handler(int sig) {
  g_gotalrm = sig;
}

void sig_int_handler(int sig) {
  g_gotint = sig;
}

void sig_term_handler(int sig) {
  g_gotterm = sig;
}

void set_my_sig_action() {
  struct sigaction sa;
  sigaction(SIGINT, (struct sigaction *)NULL, &sa);
  sa.sa_handler = sig_int_handler;
  sa.sa_flags = SA_NODEFER;
  sigaction(SIGINT, &sa, (struct sigaction*) NULL);

  sigaction(SIGPIPE, (struct sigaction*)NULL, &sa);
  sa.sa_handler = sig_pipe_handler;
  sa.sa_flags = SA_NODEFER;
  sigaction(SIGPIPE, &sa, (struct sigaction *)NULL);

  sigaction(SIGHUP, (struct sigaction *)NULL, &sa);
  sa.sa_handler = sig_hangup_handler;
  sa.sa_flags = SA_NODEFER;
  sigaction(SIGHUP, &sa, (struct sigaction*) NULL);

  sigaction(SIGALRM, (struct sigaction*)NULL, &sa);
  sa.sa_handler = sig_alrm_handler;
  sa.sa_flags = SA_NODEFER;
  sigaction(SIGALRM, &sa, (struct sigaction *)NULL);

  sigaction(SIGTERM, (struct sigaction *)NULL, &sa);
  sa.sa_handler = sig_term_handler;
  sa.sa_flags = SA_NODEFER;
  sigaction(SIGTERM, &sa, (struct sigaction*) NULL);

  signal(SIGUSR1, SIG_IGN);
  signal(SIGUSR2, SIG_IGN);
  signal(SIGTTIN, SIG_IGN);
  signal(SIGTTOU, SIG_IGN);
}
