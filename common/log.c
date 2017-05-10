#include "log.h"
#include "string.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "errno.h"
#include"stdio.h"


#ifdef IAMWINDOWS
static HANDLE mutex;
#else
static pthread_mutex_t mutex;
#endif

static int log_pid;
static int log_msgqid;

int writelog(int log_type, const char* buffer);

void initlog() {
	int typ;
	int pid;
	int temp;
	int msgqid;
	int nomsg=0;
	struct msgbuf {
		long mtype;
		char mtext[512];
	} mbuf;
	
	log_msgqid = msgget(ftok("./QPATH.txt",1), IPC_CREAT | IPC_EXCL | 0666);
	#ifdef IAMWINDOWS
		mutex = CreateMutex(NULL,0,"LOGMUTEX");
	#else
		pthread_mutex_init(&mutex, NULL);
	#endif
	pid = fork();
	if (pid >0) {
		log_pid = pid;
		msgqid = msgget(ftok("./QPATH.txt",1), IPC_EXCL | 0666);
		if (msgqid < 0) {
			perror("msgget");
		}
		printf("msgqid=%d\n",msgqid);
		log_msgqid = msgqid;
		
	} else {
		msgqid = msgget(ftok("./QPATH.txt",1), IPC_EXCL | 0666);
		// 子プロセスではループさせる
		while(1) {
			nomsg = 1;
				mbuf.mtype = typ;
				if ((temp=msgrcv(msgqid, &mbuf,512, 0, IPC_NOWAIT)) < 0) {
					if (errno == ENOMSG) {
						// ループを続ける
					}
				} else {
					
					if (temp !=0) {
						// 処理続行
						writelog(mbuf.mtype, mbuf.mtext);
						nomsg = 0;
					}
				}
			
			if (nomsg == 1) {
				usleep(100);
			}
		}
		
		
	}
	
}


void GetTimeYYYYNNDDHHMMSS (char *s)
{
    struct tm *newtime;
    time_t long_time;

    time( &long_time );
    newtime = localtime( &long_time );

	sprintf (s, "%04d/%02d/%02d %02d:%02d:%02d",
			newtime->tm_year+1900,newtime->tm_mon+1,  newtime->tm_mday,
			newtime->tm_hour,   newtime->tm_min, newtime->tm_sec);
}
char* getLogUserName(int log_user) {
	
	if (log_user == LOG_CHATSERVER) {
		return "CHATSERVER";
	}
	if (log_user == LOG_CHATCLIENT) {
		return "CHATCLIENT";
	}
	if (log_user == LOG_USER) {
		return "USER";
	}
	
	return "UNKNOWN";
}
char* getLogFileName(int log_type) {
	
	if (log_type == LOG_TEST) {
		return "logtest.txt";
	}
	if (log_type == LOG_INFO) {
		return "loginfo.txt";
	}
	if (log_type == LOG_WARNING) {
		return "logwarn.txt";
	}
	if (log_type == LOG_ERR) {
		return "logerr.txt";
	}
	
	if (log_type == LOG_ATTACK) {
		return "logattack.txt";
	}
	
	return "logunknown.txt";
}

int writelog(int log_type, const char* buffer) {
	FILE* file;
	
	#ifdef IAMWINDOWS
	h = OpenMutex(MUTEX_ALL_ACCESS,0,"LOGMUTEX");
	if (h == NULL) {
		return LOG_NO;
	}
	
	#else 
	pthread_mutex_lock(&mutex);
	#endif
	if (0 == (file = fopen(getLogFileName(log_type),"a"))) {
		#ifdef IAMWINDOWS
			ReleaseMutex(h);
		#else 
			pthread_mutex_unlock(&mutex);
		#endif
		return LOG_NO;
	}
	fwrite(buffer,1,strlen(buffer),file);
	fwrite("\n",1,1,file);
	fclose(file);
	#ifdef IAMWINDOWS
	ReleaseMutex(h);
	#else 
	pthread_mutex_unlock(&mutex);
	#endif
	
	return LOG_OK;
	
}

int mylog(int log_user, int log_type, const char* buffer, ...) {
	
	#ifdef IAMWINDOWS
	HANDLE h;
	#endif
			
	FILE* file;
	char sendlog[4096];
	char log[4096];
	char prefix[128];
	char ydate[64];
	struct msgbuf {
		long mtype;
		char mtext[512];
	} mbuf;
	
	#ifdef THISISTEST
	log_type = LOG_TEST;
	#else
	if (log_type == LOG_TEST) {
		return LOG_OK;
	}
	#endif
	
	if (strlen(buffer) > 512) {
		return LOG_NO;
	}
	
	va_list ap;
	va_start(ap, buffer);
	
	
	
	memset(log,0,4096);
	vsprintf(log, buffer, ap);
	log[4095] = '\0';
	va_end(ap);

	memset(prefix,0,128);
	memset(ydate,0,64);
	GetTimeYYYYNNDDHHMMSS(ydate);
	snprintf(prefix,128, "USER:%s,DATE:%s:",getLogUserName(log_user), ydate);

	memset(sendlog,0,512);
	if (strlen(log) + strlen(prefix) > 512) {
		return LOG_NO;
	}
	
	strcat(sendlog,prefix);
	strcat(sendlog,log);
	memset(mbuf.mtext,0,512);
	strcpy(mbuf.mtext,sendlog);
	mbuf.mtype = log_type;
	
	// メッセージキューにメッセージを入れる
	if (msgsnd(log_msgqid, &mbuf, 512,IPC_NOWAIT)!= 0) {
		perror("msgsnd");
		return LOG_NO;
	}
	
	return LOG_OK;
}
int mylogd(int log_user, int log_type, const char* buffer, ...) {
	#ifdef IAMWINDOWS
	HANDLE h;
	#endif
			
	FILE* file;
	char log[4096];
	char prefix[128];
	char ydate[64];
	
	#ifdef THISISTEST
	log_type = LOG_TEST;
	#else
	if (log_type == LOG_TEST) {
		return LOG_OK;
	}
	#endif
	
	if (strlen(buffer) > 512) {
		return LOG_NO;
	}
	
	va_list ap;
	va_start(ap, buffer);
	
	
	
	memset(log,0,4096);
	vsprintf(log, buffer, ap);
	log[4095] = '\0';
	va_end(ap);

	memset(prefix,0,128);
	memset(ydate,0,64);
	GetTimeYYYYNNDDHHMMSS(ydate);
	snprintf(prefix,128, "USER:%s,DATE:%s:",getLogUserName(log_user), ydate);

	#ifdef IAMWINDOWS
	h = OpenMutex(MUTEX_ALL_ACCESS,0,"LOGMUTEX");
	if (h == NULL) {
		return LOG_NO;
	}
	
	#else 
	pthread_mutex_lock(&mutex);
	#endif
	if (0 == (file = fopen(getLogFileName(log_type),"a"))) {
		#ifdef IAMWINDOWS
			ReleaseMutex(h);
		#else 
			pthread_mutex_unlock(&mutex);
		#endif
		return LOG_NO;
	}
	fwrite(prefix,1,strlen(prefix),file);
	fwrite(log,1,strlen(log),file);
	fwrite("\n",1,1,file);
	fclose(file);
	#ifdef IAMWINDOWS
	ReleaseMutex(h);
	#else 
	pthread_mutex_unlock(&mutex);
	#endif

	fprintf(stderr,prefix);
	fprintf(stderr,log);
	fprintf(stderr,"\n");
	
	
	return LOG_OK;
}

void releaselog() {
	int status;
	
	// 子プロセスを殺す
	kill(log_pid,SIGHUP);
	wait(&status);
	
	
	#ifdef IAMWINDOWS
	CloseHandle(mutex);
	#else
	#endif
	
	
	
}

int main() {
	printf("initlog:\n");
	initlog();
	printf("initlogend\n");
	
	if(LOG_NO == mylog(LOG_USER, LOG_ERR, "test:%d %d %d %d testdayo",192,168,11,100)) {
		printf("log sippai\n");
	}
	sleep(1);
	printf("releaselog\n");
	releaselog();
	printf("releaselogend\n");
}