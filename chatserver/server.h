#include <sys/param.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>




int server_socket(const char* portnm);
int send_recv_loop(int acc, int child_no);
void accept_loop(int soc);
size_t mystrlcat(char* dst, const char* src, size_t size);

 
 
