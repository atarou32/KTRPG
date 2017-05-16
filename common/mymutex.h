#include "string.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "errno.h"
#include"stdio.h"
#ifdef IAMWINDOWS
#include "windows.h"
#else 
#include "pthread.h"
#endif


#define MYMUTEX_TYPE_BEFORELOCK 0
#define MYMUTEX_TYPE_ACCOUNT 1
#define MYMUTEX_TYPE_DBQUE 2
#define MYMUTEX_TYPE_MAX 3
#define MAX_MUTEX_NUM 16


#define MYMUTEX_TYPE_BEFORELOCK_STR "BEFORELOCKMUTEX"
#define MYMUTEX_TYPE_ACCOUNT_STR "ACCOUNTMUTEX"
#define MYMUTEX_TYPE_DBQUE_STR "DBQUEMUTEX"


typedef struct mymutex {
	#ifdef IAMWINDOWS
	 HANDLE mutex;
	#else
	 pthread_mutex_t mutex;
	#endif
	int mutex_type;
	int mutex_lock_count;
} type_mymutex;


#define MAX_MUTEX_GETTER_NUM 32

type_mymutex* mmutex;// mmutex[MAX_MUTEX_NUM];

#define MYMUTEX_YES 1
#define MYMUTEX_NO 0
#define MMGETTER_COMMENT_MAX 16


typedef struct mymutexgetter {
	pid_t process_id;
	pid_t thread_id;
	unsigned char get_mutex_type[MAX_MUTEX_NUM];
	char comments[MAX_MUTEX_NUM][MMGETTER_COMMENT_MAX];
	unsigned char wait_mutex_type;
	unsigned char counted;
	unsigned char is_used;
	unsigned char is_waited;
} type_mymutexgetter;

type_mymutexgetter* mmgetter;// mmgetter[MAX_MUTEX_GETTER_NUM];

typedef struct mymutexjyunban {
	unsigned char get_mutex_type_jyunban[MAX_MUTEX_NUM];
} type_mymutex_jyunban;
type_mymutex_jyunban mmjyunban;

// init ÇåƒÇ—èoÇ∑ëOÇ…log Çèâä˙âªÇµÇƒÇÈÇ±Ç∆
#define MYMUTEX_USE_BEFORELOCK 1
#define MYMUTEX_NOUSE_BEFORELOCK 0
void init_mymutex(int beforelock, type_mymutex_jyunban* mmj);
// comment char[16];
void mymutex_lock(int mutex_type, char* comment);
void mymutex_unlock(int mutex_type, char* comment);

void release_mymutex();