#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
int main() {
  struct sockaddr_in server;
  int sock;
  char buf[16];
  int n;
  fd_set read_fds;
  int max_sock;
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
  n = inet_pton(AF_INET, "192.168.11.101", &server.sin_addr.s_addr);
  printf("inetpton comp %d\n", n);
  connect(sock, (struct sockaddr *)&server, sizeof(server));
  max_sock = sock + 1;
  printf("connect comp\n");

  memset(buf,0, sizeof(buf));
  send(sock,"testdayo\n",9,0);
  for (;;) {
  	FD_ZERO(&read_fds);
  	FD_SET(0, &read_fds);
  	FD_SET(sock, &read_fds);
  	if (select(max_sock, &read_fds, (fd_set*)0, (fd_set*)0, (struct timeval*)0) < 0) {
  		printf("select error\n");
  		perror("select");
  		exit(0);
  	}
  	if (FD_ISSET(sock, &read_fds)) {
  		ssize_t size;
  		memset(buf,0,16);
  		n = read(sock, buf, sizeof(buf)-1);
  		buf[15] = '\0';
  		if (n == -1) {
  			printf("read error\n");
  			perror("read");
  			exit(0);
  		}
  		printf("%d, %s\n", n , buf);
  	}
  	if (FD_ISSET(0, &read_fds)) {
  		memset(buf,0,16);
  		//fgets(buf, sizeof(buf)-1, stdin); 
  		send(sock, buf,sizeof(buf),0);
  	}
  }
  close(sock);
  sock = -1;
  return 0;

}
