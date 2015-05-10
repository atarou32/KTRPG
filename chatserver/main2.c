#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
  int sock0;
  struct sockaddr_in addr;
  struct sockaddr_in client;
  int len;
  int sock;

  printf("start server\n");
  /* make socket */
  sock0 = socket(AF_INET, SOCK_STREAM, 0);
  if (sock0 <0) {
    printf("error with making socket\n");
    return 1;
  }

  /* setting socket */
  addr.sin_family = AF_INET;
  addr.sin_port = htons(20000);
  addr.sin_addr.s_addr = INADDR_ANY;
  
  bind(sock0, (struct sockaddr*)&addr, sizeof(addr));
  printf("bind comp\n");

  /* wait for client */
  listen(sock0, 5);
  printf("listen comp\n");

  len = sizeof(client);
  sock = accept(sock0, (struct sockaddr*)&client, (socklen_t *)&len);
  printf("accept comp\n");
  write(sock, "hello", 5);
  printf("write comp\n");

  /* close session */
  close(sock);

  /* close socket */
  close(sock0);
  printf("ok\n");
  return 0;
}


