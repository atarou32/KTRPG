#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <memory.h>
#include "resultcheck.h"

#define ERRORTXT "temp/error.txt"
#define RESULTTXT "temp/result.txt"
#define OKTXT "temp/ok.txt"
#define DAMETXT "temp/dame.txt"
#define TEMPTXT "temp/temp.txt"

int writeToErrorTxt(char* buf, int buflen) {
	FILE* fp;
	fp = fopen(ERRORTXT, "a");
	if (fp == 0) {
		//perror("fp");
		printf("fp error\n");
		return 0;
	}
	fwrite(buf,buflen,1,fp);
	fclose(fp);
	return 1;
}

#define FILEBUFSIZE 1024*128
#define CLIENT_NUM 16
/*
親
pid
pid2
pid3
*/

// client を呼ばないプロセスだと1が返る error だと　-1
int makeclient(int fp, int *out_pid, int *out_pipefd) {
	//int pid2;
	int pid3;
	int pipefd[2];
	pipefd[0] = 0;
	pipefd[1] = 0;
	
		
		pipe(pipefd);
		pid3 = fork();
		if (pid3 == -1) {
			perror("Fork");
			writeToErrorTxt("error fork\n", 11);
			return -1;
		}
		
		if (pid3 > 0) {
			// クライアントにシグナル(kill)を送る(pid2)
			// クライアントを実行しているプロセスの入力とこちらのプロセスの出力をつなぐことで文字列を送る
			
		//	dup2(pipefd[0], 1);
		//	dup2(pipefd[1], dup(fp));
		//	close(pipefd[1]);// pipe- out
			close(pipefd[0]);//close pipe-in
			// pipe-out は使用する
/*			
			sleep(1);// server ができるまで待つ
			sleep(1); // client ができるまで待つ
			write(pipefd[1],"never give up\n", 14);
			sleep(1);
			kill(pid3,SIGHUP);
			wait(&status);
			printf("pidEND:");
*/			
			*out_pid = pid3;
			*out_pipefd = pipefd[1];
			
			return 1;
		}
		
	//	close(pipefd[0]);//close pipe-in
		dup2(pipefd[0],0); // connect pipe-in to stdin
		close(pipefd[0]);
		close(pipefd[1]);
//	close(0);
	//close(1);
	//close(2);
	//	close(0);
//		close(1);
//		close(2);
	//	close(pipefd[0]);
		sleep(1);// server ができるまで待つ
		// クライアントを実行する（pid3)
		if (execl("../../chatclient/main", "../../chatclient/main",NULL) == -1) {
			writeToErrorTxt("error exec\n",11);
			perror("execl");
		}
		//fprintf(stdout,"client out\n");
		return 0;
	
};

int main(int argc, char** argv) {
	
	int status, pid, pid2, pid3[CLIENT_NUM],i,temp;
	int filefds[CLIENT_NUM];
	FILE* fp;
	int ifp,ifp2;
	int pipefd[2];
	char filebuf[FILEBUFSIZE];
	int filebufsize=0;
	char buf[512];
	
	fp = fopen(ERRORTXT, "w");
	if (fp ==0) {
		printf("fp error\n");
		//return 0;
	}
	
	fclose(fp);
	
	fp = fopen(RESULTTXT, "w");
	if (fp ==0) {
		//perror("fp");
		printf("fp error\n");
		//return 0;
	}
	fclose(fp);
	
	pid = fork();
	
	
	if (pid == -1) {
		perror("Fork");
		writeToErrorTxt("error fork\n", 11);
		return 0;
	}
	
	if (pid >0) {
		printf("PARENT:");
		
		
		
		sleep(8);
		kill(pid,SIGHUP);
		wait(&status);
		printf("PARENTEND:");
		
		fp = fopen(RESULTTXT, "r");
		memset(filebuf, 0, FILEBUFSIZE);
		fseek(fp, 0L, SEEK_SET);
		filebufsize = fread(filebuf,1, FILEBUFSIZE, fp);
		fclose(fp);
		
		remove(OKTXT);
		remove(DAMETXT);
		
		
		if (result_check(filebuf,filebufsize)) {
			
			fp = fopen(OKTXT, "w");
			fclose(fp);
			fprintf(stderr,"ok");
			
		} else {
			
			fp = fopen(DAMETXT, "w");
			fclose(fp);
			fprintf(stderr, "dame");
		}
		return 0;
	}
	
	pid2 = fork();
	if (pid2 == -1) {
		perror("Fork");
		writeToErrorTxt("error fork\n", 11);
		return 0;
	}
	
	
	
	
	if (pid2 == 0) {
		ifp2 = open(TEMPTXT, O_WRONLY | O_APPEND | O_CREAT| O_TEXT, S_IWRITE);
		if (ifp2 == -1) {
			writeToErrorTxt("error open\n",12);
			perror("open");
		//	return 0;
		}
		
		for (i=0;i<CLIENT_NUM;i++) {
			temp = makeclient(ifp2,&pid3[i],&filefds[i]);
			if (temp != 1) {
				return 0;
			}
		}
	//	close(1);
	//	close(2);
		
		sleep(1);// server ができるまで待つ
		sleep(2); // client ができるまで待つ
		for (i=0;i<CLIENT_NUM;i++) {
			memset(buf,0,512);
			snprintf(buf,512,"never give up %d\n",i);
			write(filefds[i],buf,sizeof(buf) );
		}
		sleep(1);
		for (i=0;i<CLIENT_NUM;i++) {
			close(filefds[i]);
			kill(pid3[i],SIGHUP);
		}
		//sleep(1);
		//kill(pid2,SIGHUP);
		
		close(ifp2);
		wait(&status);
		printf("pidEND:");
			
		
		
		
		
		return 0;
	}
	printf("CHILD(SERV):");
	ifp = open(RESULTTXT, O_WRONLY | O_APPEND | O_CREAT| O_TEXT, S_IWRITE);
	if (ifp == -1) {
		writeToErrorTxt("error open\n",12);
		perror("open");
	
		//	return 0;
	}
	
	
	
	dup2(ifp,1);
	dup2(ifp,2);
	//close(1);
	//close(2);
	
// サーバを実行する(pid)
	if (execl("../../chatserver/server", "../../chatserver/server","20000","0",NULL) == -1) {
		writeToErrorTxt("error exec\n",11);
		perror("execl");
		
		close(ifp);
		return 0;
	}
	close(ifp);
	printf("CHILD(SERV)END:");
		
		
		
	return 0;
}