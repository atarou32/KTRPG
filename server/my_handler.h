

volatile extern int g_gothangup;
volatile extern int g_gotpipe;
volatile extern int g_gotalrm;
volatile extern int g_gotint;
volatile extern int g_gotterm;

void sig_hangup_handler(int sig);
void sig_pipe_handler(int sig);
void sig_alrm_handler(int sig);
void sig_int_handler(int sig);

void sig_term_handler(int sig);

void set_my_sig_action();
