#include "server.h"

volatile extern int g_gotalrm;
volatile extern int g_gotpipe;
volatile extern int g_gotint;
volatile extern int g_gotterm;
volatile extern int g_gothangup;

int server_socket(const char* portnm) {
  char nbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
  struct addrinfo hints, *res0;
  int soc, opt, errcode,val;
  socklen_t opt_len;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  fprintf(stderr, "testdesu\n");

  if ((errcode = getaddrinfo(NULL, portnm, &hints, &res0)) != 0) {
    fprintf(stderr, "getaddrinfo():%s\n", gai_strerror(errcode));
    return -1;
  }
  if ((errcode = getnameinfo(res0->ai_addr, res0->ai_addrlen,
                             nbuf, sizeof(nbuf),
                             sbuf, sizeof(sbuf),
                             NI_NUMERICHOST | NI_NUMERICSERV)) != 0) {
    fprintf(stderr, "getnameinfo():%s\n", gai_strerror(errcode));
    freeaddrinfo(res0);
    return -1;
  }
  fprintf(stderr, "port=%s\n", sbuf);
  if ((soc = socket(res0->ai_family, res0->ai_socktype, res0->ai_protocol)) == -1) {
    perror("socket");
    freeaddrinfo(res0);
    return -1;
   }
   opt = 1;
   opt_len = sizeof(opt);
   if (setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, &opt, opt_len) == -1) {
     perror("setsockopt");
     close(soc);
     freeaddrinfo(res0);
     return -1;
   }
   if (bind(soc, res0->ai_addr, res0->ai_addrlen) == -1) {
     perror("bind");
     close(soc);
     freeaddrinfo(res0);
     return -1;
   }
   
   // ノンブロッキングモードに設定
	val = 1;
	ioctl(soc, FIONBIO, &val);
	
   if (listen(soc, SOMAXCONN) == -1) {
     perror("listen");
     close(soc);
     freeaddrinfo(res0);
     return -1;
   }
   freeaddrinfo(res0);
   return soc;
}

int send_recv_loop(int acc, int child_no, int* childs, int childsnum, int* childs_count) {
  char buf[512], buf2[512], *ptr;
  ssize_t len;
  int i;
  int flg=0; 
  memset(buf,0,512);
  if ((len = recv(acc, buf, sizeof(buf),flg & ~MSG_PEEK)) == -1) {
  
  	if (errno != EAGAIN) {
  		fprintf(stderr, "errno=%d",errno); 
  		perror("recv");
    	return -1;
  	}
  	if (errno > 0 ) {
		fprintf(stderr,"errno=%d\n\n\n", errno);
	}
  }
  if (len ==0) {
  	fprintf(stderr, "errno=%d,acc=%d,child_no=%d",errno,acc,child_no); 
    fprintf(stderr, "recv:EOF\n");
  	if (errno != EAGAIN) {
  		
    	return -1;
  	}
  }
  buf[len] = '\0';
  /*if ((ptr = strpbrk(buf, "\r\n")) != NULL) {
    *ptr = '\0';
  }*/
	
	
  memset(buf2,0,512);
//  usleep(3000);
  fprintf(stderr, "[rcvd cno:%d %d]%s",child_no, len,buf);
//	usleep(3000);
	fprintf(stderr, buf);
 // mystrlcat(buf, ":OK\n", sizeof(buf));
 // snprintf(buf2, 512, "[cno.%d]%s",child_no,buf);
	strcat(buf2,buf);
//  mystrlcat(buf2,buf, sizeof(buf2));

  len = (ssize_t) strlen(buf2);
  if ((len = send(acc, buf2, (size_t)len, 0)) == -1) {
  	if (errno != EAGAIN) {
  		fprintf(stderr, "acc=%d,child_no=%d child_count=%d\n",acc,child_no, *childs_count); 
    	perror("send");
    	return -1;
  	}
  }

  for (i=0;i<childsnum;i++) {
    if (i != child_no) {
      len = (ssize_t)strlen(buf2);
    	if (childs[i] != -1) {
          if((len = send(childs[i],buf2, (size_t)len, 0)) == -1) {
          	if (errno != EAGAIN) {
          		fprintf(stderr, "acc=%d,child_no=%d,child_count=%d\n",acc,child_no,*childs_count); 
         	   perror("send");
         	//  close(childs[i]);
         	 // childs[i] = -1;
          	 // *childs_count = *childs_count -1;;
              continue;
          	}
        }
      }
    }
  }
  return 0;
  
}


#define MAX_CHILD 1024

void accept_loop(int soc) {
  char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
  int child[MAX_CHILD];
  struct timeval timeout;
  struct sockaddr_storage from;
  int acc, child_no, width, i , count, pos, ret;
  socklen_t len;
  fd_set mask;

  for (i = 0 ; i< MAX_CHILD; i++) {
    child[i] = -1;
  }
  child_no = 0;

  for (;(g_gotterm == 0) && (g_gothangup == 0);) {
    FD_ZERO(&mask);
    FD_SET(soc, &mask);
    width = soc + 1;
    count = 0;

    for ( i = 0 ;i<MAX_CHILD; i++) {
      if (child[i] != -1) {
        FD_SET(child[i], &mask);
        if (child[i]+1 > width) {
          width = child[i] + 1;
          count++;
        }
      }
    }

    fprintf(stderr, "\n<<child count:%d>>\n", child_no);
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    switch (select(width, (fd_set *)&mask, NULL,NULL, &timeout)) {
      case -1:
        perror("select");
        break;
      case 0:
        
        break;
      default:
        if (FD_ISSET(soc, &mask)) {
          len = (socklen_t)sizeof(from);
          if ((acc = accept(soc, (struct sockaddr*)&from, &len)) == -1) {
            if (errno != EINTR) {
              perror("accept");
            }
          	acc = -1;
          } else {
            getnameinfo((struct sockaddr*)&from, len,
                  hbuf, sizeof(hbuf),
                  sbuf, sizeof(sbuf),
                  NI_NUMERICHOST | NI_NUMERICSERV);
            fprintf(stderr, "accept:%s:%s\n", hbuf, sbuf);
            pos = -1;
            for (i=0;i<MAX_CHILD; i++) {
              if (child[i] == -1) {
                pos = i;
                break;
              }
            }
            if (pos == -1) {
              if (child_no +1 >= MAX_CHILD) {
                (void)fprintf(stderr, "child is full: cannot accept\n");
                close(acc);
              } else {
              	fprintf(stderr,"child no pos: cannot accept\n");
              	close(acc);
              }
            }
            if (pos != -1) {
              child[pos] = acc;
              child_no++;
            acc = -1;
            }
          }
        }

        for (i=0;i<MAX_CHILD;i++) {
          if (child[i] != -1) {
            if (FD_ISSET(child[i], &mask)) {
              ret = send_recv_loop(child[i], i, child,MAX_CHILD,&child_no);
              if (ret == -1) {
                close(child[i]);
                child[i] = -1;
              	child_no--;
              	break;
              }
            }
          }
        }

        break;
    }
  }
}

size_t mystrlcat(char* dst, const char* src, size_t size) {
  const char *ps;
  char *pd, *pde;
  size_t dlen, lest;
  
  for (pd = dst, lest = size; *pd != '\0' && lest != 0; pd++, lest--);
  dlen = pd - dst;
  if (size - dlen == 0) {
    return dlen +strlen(src);
  }
  pde = dst + size -1;
  for (ps = src; *ps != '\0' && pd < pde; pd++, ps++) {
    *pd = *ps;
  }
  for (; pd <= pde; pd++) {
    *pd = '\0';
  }
  while(*ps++);
  return dlen + (ps - src -1);
}


