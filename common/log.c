#include "log.h"
#include "string.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "errno.h"
#include"stdio.h"
#include "commondef.h"


#ifdef IAMWINDOWS
static HANDLE mutex;
#else
static pthread_mutex_t* mutex;
#endif

static int log_pid;
static int log_msgqid;
#ifndef IAMWINDOWS
  pthread_mutexattr_t logmat;
  int logmathandle=0;
  char* logmatmemory;
#endif

int writelog(int log_type, const char* buffer);

void initlog() {
	int typ;
	int pid;
	int temp;
	int msgqid;
	int nomsg=0;
	

	
	
	log_msgqid = msgget(ftok("./QPATH.txt",1), IPC_CREAT | IPC_EXCL | 0666);
	if (log_msgqid == -1) {
		perror("log:msgget");
	}
	
	#ifdef IAMWINDOWS
		mutex = CreateMutex(NULL,0,"LOGMUTEX");
	#else
		pthread_mutexattr_init(&logmat);
		if (pthread_mutexattr_setpshared(&logmat, PTHREAD_PROCESS_SHARED) != 0) {
		    perror("log:pthread_mutexattr_setpshared");
			
  		}
	logmathandle = shm_open("/shmlogmat", O_CREAT | O_RDWR, 0777);
	ftruncate(logmathandle, sizeof(pthread_mutex_t));
	logmatmemory = (char*)mmap(0,sizeof(pthread_mutex_t), PROT_READ|PROT_WRITE, MAP_SHARED, logmathandle, 0);
	mutex = (pthread_mutex_t*)logmatmemory;
		pthread_mutex_init(mutex, &logmat);
		pthread_mutexattr_destroy(&logmat);
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
			struct msgbuf {
				long mtype;
				char mtext[512];
			} mbuf;
			int i=0;
			nomsg = 1;
			
			for (i=0;i<LOG_MAX;i++) {
				mbuf.mtype = i;
			
				temp=msgrcv(msgqid, &mbuf,512, 0, MSG_NOERROR);
				if (temp < 0) {
					if (errno == ENOMSG) {
						// ループを続ける
					} else {
						if (temp == -1) {
							perror("in logthread:");
						}
					}
				} else {
					
					if (temp !=0) {
						// 処理続行
						writelog(mbuf.mtype, mbuf.mtext);
						nomsg = 0;
					}
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
	HANDLE h;
	#endif
	
	#ifdef IAMWINDOWS
	h = OpenMutex(MUTEX_ALL_ACCESS,0,"LOGMUTEX");
	if (h == NULL) {
		return LOG_NO;
	}
	
	#else 
	pthread_mutex_lock(mutex);
	#endif
	if (0 == (file = fopen(getLogFileName(log_type),"a"))) {
		#ifdef IAMWINDOWS
			ReleaseMutex(h);
		#else 
			pthread_mutex_unlock(mutex);
		#endif
		return LOG_NO;
	}
	fwrite(buffer,strlen(buffer),1,file);
	fwrite("\n",1,1,file);
	fclose(file);
	#ifdef IAMWINDOWS
	ReleaseMutex(h);
	#else 
	pthread_mutex_unlock(mutex);
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
	if (msgsnd(log_msgqid, &mbuf, 512,0)!= 0) {
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
	pthread_mutex_lock(mutex);
	#endif
	if (0 == (file = fopen(getLogFileName(log_type),"a"))) {
		#ifdef IAMWINDOWS
			ReleaseMutex(h);
		#else 
			pthread_mutex_unlock(mutex);
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
	pthread_mutex_unlock(mutex);
	#endif

	fprintf(stderr,prefix);
	fprintf(stderr,log);
	fprintf(stderr,"\n");
	
	
	return LOG_OK;
}

void releaselog() {
	int status;
	struct msqid_ds * buf;
	
	// 子プロセスを殺す
	kill(log_pid,SIGHUP);
	wait(&status);
	
	msgctl(log_msgqid, IPC_RMID,buf);
	
	#ifdef IAMWINDOWS
	CloseHandle(mutex);
	#else
	munmap(logmatmemory, sizeof(pthread_mutex_t));
	shm_unlink("/shmlogmat");
	#endif
	
	
	
}

/*
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
*/