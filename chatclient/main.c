#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
  struct sockaddr_in server;
  int sock;
  char buf[32];
  int n;
  printf("start client\n");
  /* make socket */
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    printf("error with making socket\n");
    return 1;
  }

  /* setting struct of connectting server */

  server.sin_family = AF_INET;
  server.sin_port = htons(20000);
  n = inet_pton(AF_INET, "192.168.11.207", &server.sin_addr.s_addr);
  printf("inetpton comp %d\n", n);
  connect(sock, (struct sockaddr *)&server, sizeof(server));
  printf("connect comp\n");

  memset(buf,0, sizeof(buf));
  n = read(sock, buf, sizeof(buf));

  printf("%d, %s/n",n,buf);
  close(sock);
  return 0;

}
