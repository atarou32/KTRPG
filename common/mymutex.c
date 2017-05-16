#include "mymutex.h"
#include "log.h"
#include <unistd.h>
#ifndef __CYGWIN__

#include <sys/syscall.h>
#endif

#include <sys/types.h>
#include <sys/shm.h>
#include <sys/mman.h>


extern int mylog(int log_user, int log_type, const char* buffer, ...);

int mymutexhandle=0;
char* mymutexmemory;

pid_t gettid(void)
{
	
	#ifdef __CYGWIN__
	return 1;
	#else
    return syscall(SYS_gettid);
	#endif
}
int g_beforelock;
char isDeadLock(int mutex_type, type_mymutex* mmx, type_mymutexgetter* mmg, type_mymutexgetter* mmgs) {
	int k=0;
	int m=0;
	int pid;
	char has_mada=MYMUTEX_NO;
	type_mymutexgetter* tempgetter;
	
	for (k=0;k<MAX_MUTEX_GETTER_NUM;k++) {
		mmgs[k].counted = MYMUTEX_NO;
	}
	
	tempgetter = mmg;
	tempgetter->counted = MYMUTEX_YES;
	// tempgetter のwait_mutex_type をもっているgetterを調べる
	if (tempgetter->wait_mutex_type == 0) {
		mylog(LOG_UNKNOWN, LOG_TEST, "mutex_nodayo tempgetter waitmutextype0");
		return MYMUTEX_NO;
	}
	has_mada = MYMUTEX_NO;
	for (k=0;k<MAX_MUTEX_GETTER_NUM;k++) {
		if (mmgs[k].is_used == MYMUTEX_NO) continue;
		
		for (m=0;m<MAX_MUTEX_NUM;m++) {
			if (mmgs[k].get_mutex_type[m] == tempgetter->wait_mutex_type) {
				tempgetter->counted = MYMUTEX_YES;
				tempgetter = &mmgs[k];
			//	tempgetter->counted = MYMUTEX_YES;
				has_mada = MYMUTEX_YES;
				
				pid = getpid();
				mylog(LOG_UNKNOWN, LOG_TEST, "there is lock sigen in mutex_type=%d:k=%d,m=%d,type=%d,pid=%d",mutex_type,k,m,mmgs[k].get_mutex_type[m],pid);
				break;
			}
		}
		if (has_mada == MYMUTEX_YES) {
			break;
		}
	}
	
	if (has_mada == MYMUTEX_NO) {
		mylog(LOG_UNKNOWN, LOG_TEST, "mutex_nodayo has_mada_no in firstloop");
		return MYMUTEX_NO;
	}
	
	/*if (tempgetter->wait_mutex_type== 0) {
		
		mylog(LOG_UNKNOWN, LOG_TEST, "mutex_nodayo wait_mutex_type0 in secondloop");
		return MYMUTEX_NO;
	}*/
	while(1) {
		
		if (tempgetter->is_waited == MYMUTEX_NO) {
			// deadlockしない
			mylog(LOG_UNKNOWN, LOG_TEST, "mutex_nodayo is_waited_no in secondloop");
			return MYMUTEX_NO;
		}
		if (tempgetter == mmg) {
			// ループしているのでデッドロック
			mylog(LOG_UNKNOWN, LOG_TEST, "loop deadlockdayo");
			return MYMUTEX_YES;
		}
		
		has_mada = MYMUTEX_NO;
		for (k=0;k<MAX_MUTEX_GETTER_NUM;k++) {
			if (mmgs[k].is_used == MYMUTEX_NO) continue;
			if (mmgs[k].counted == MYMUTEX_NO) {
				has_mada = MYMUTEX_YES;
				break;
			}
		}
		
		if (has_mada == MYMUTEX_NO) {
			// すべてまわったけどループしなかった
			mylog(LOG_UNKNOWN, LOG_TEST, "mutex_nodayo loopnodatyo");
			return MYMUTEX_NO;
		}
		has_mada = MYMUTEX_NO;
		
		for (k=0;k<MAX_MUTEX_GETTER_NUM;k++) {
			if (mmgs[k].is_used == MYMUTEX_NO) continue;
			if (tempgetter->wait_mutex_type ==0) continue;
			
			for (m=0;m<MAX_MUTEX_NUM;m++) {
				if (mmgs[k].get_mutex_type[m] == 0) continue;
				if (mmgs[k].get_mutex_type[m] == tempgetter->wait_mutex_type) {
					tempgetter->counted = MYMUTEX_YES;
					tempgetter = &mmgs[k];
//					tempgetter->counted = MYMUTEX_YES;
					has_mada = MYMUTEX_YES;
					break;
				}
			}
			if (has_mada == MYMUTEX_YES) {
				break;
			}
		}
		if (has_mada == MYMUTEX_NO) {
			mylog(LOG_UNKNOWN, LOG_TEST, "mutex_nodayo there is no has_mada");
			return MYMUTEX_NO;
		}
	}
	
}


char isLockJyunbanOk(int mutex_type, type_mymutex_jyunban* mmj, type_mymutexgetter* mmg) {
	
	unsigned char findex=0;
	unsigned char lindex=0;
	unsigned char findex_mmj=0;
	unsigned char lindex_mmj=0;
	char mmjf=MYMUTEX_NO;
	char mmjl= MYMUTEX_NO;
	int i=0;
	int j=0;
	int k=0;
	for (i=0;i<MAX_MUTEX_NUM;i++) {
		for (j=i;j<MAX_MUTEX_NUM;j++) {
			findex = mmg->get_mutex_type[i];
			lindex = mmg->get_mutex_type[j];
			// findex lindex のそれぞれのmmj内の順番と比べる
			if (findex != lindex) {
				if ((findex == 0) || (lindex==0)) continue;
				mmjf = MYMUTEX_NO;
				mmjl = MYMUTEX_NO;
				for (k=0; k<MAX_MUTEX_NUM;k++) {
					if (mmj->get_mutex_type_jyunban[k] == findex) {
						findex_mmj = k;
						mmjf = MYMUTEX_YES;
					}
					if (mmj->get_mutex_type_jyunban[k] == lindex) {
						lindex_mmj = k;
						mmjl = MYMUTEX_YES;
					}
				}
				if (mmjf == MYMUTEX_NO) {
					mylog(LOG_UNKNOWN, LOG_TEST, "unlockする順番:mmjf i=%d,j=%d k=%d",i,j,k);
					return MYMUTEX_NO;
				}
				if (mmjl == MYMUTEX_NO) {
					mylog(LOG_UNKNOWN, LOG_TEST, "unlockする順番:mmjl i=%d,j=%d k=%d",i,j,k);
					return MYMUTEX_NO;
				}
				if (findex_mmj >= lindex_mmj) {
					mylog(LOG_UNKNOWN, LOG_TEST, "unlockする順番:findex_mmj=%d,lindex_mmj=%d i=%d,j=%d k=%d",findex_mmj,lindex_mmj,i,j,k);
					return MYMUTEX_NO;
				}
			}
		}
	}
	return MYMUTEX_YES;

}
char* getMutexTypeStr(int mutex_type) {
	if (mutex_type == MYMUTEX_TYPE_BEFORELOCK) {
		return MYMUTEX_TYPE_BEFORELOCK_STR;
	}
	if (mutex_type == MYMUTEX_TYPE_ACCOUNT) {
		return MYMUTEX_TYPE_ACCOUNT_STR;
	}
	if (mutex_type == MYMUTEX_TYPE_DBQUE) {
		return MYMUTEX_TYPE_DBQUE_STR;
	}
	return MYMUTEX_TYPE_ACCOUNT_STR;
}
void init_mymutex(int beforelock, type_mymutex_jyunban* mmj) {
	int i=0;
	int j=0;
	int k=0;
	#ifndef IAMWINDOWS
	  pthread_mutexattr_t mat;
	#endif
	
	mymutexhandle = shm_open("/shmmymutex", O_CREAT | O_RDWR, 0777);
	ftruncate(mymutexhandle, sizeof(type_mymutex)*MAX_MUTEX_NUM+sizeof(type_mymutexgetter)*MAX_MUTEX_GETTER_NUM);
	mymutexmemory = (char*)mmap(0,sizeof(type_mymutex)*MAX_MUTEX_NUM+sizeof(type_mymutexgetter)*MAX_MUTEX_GETTER_NUM, PROT_READ|PROT_WRITE, MAP_SHARED, mymutexhandle, 0);
	mmutex = (type_mymutex*)mymutexmemory;
	mmgetter = ((type_mymutexgetter*)(mymutexmemory+sizeof(type_mymutex)*MAX_MUTEX_NUM));
	
	#ifdef IAMWINDOWS
		mmutex[MYMUTEX_TYPE_BEFORELOCK].mutex = CreateMutex(NULL,0,MYMUTEX_TYPE_BEFORELOCK_STR);
		mmutex[MYMUTEX_TYPE_ACCOUNT].mutex = CreateMutex(NULL,0,MYMUTEX_TYPE_ACCOUNT_STR);
		mmutex[MYMUTEX_TYPE_DBQUE].mutex = CreateMutex(NULL,0,MYMUTEX_TYPE_DBQUE_STR);
	#else
		pthread_mutexattr_init(&mat);
		if (pthread_mutexattr_setpshared(&mat, PTHREAD_PROCESS_SHARED) != 0) {
		    perror("pthread_mutexattr_setpshared");
  		}
	
		pthread_mutex_init(&mmutex[MYMUTEX_TYPE_BEFORELOCK].mutex, &mat);
		pthread_mutex_init(&mmutex[MYMUTEX_TYPE_ACCOUNT].mutex, &mat);
		pthread_mutex_init(&mmutex[MYMUTEX_TYPE_DBQUE].mutex, &mat);
		pthread_mutexattr_destroy(&mat);
	#endif

	for (i=0;i<MAX_MUTEX_NUM;i++) {
		mmutex[i].mutex_lock_count = 0;
		//mmutex[i].mutex_type = i;
	}
	mmutex[MYMUTEX_TYPE_BEFORELOCK].mutex_type = MYMUTEX_TYPE_BEFORELOCK;
	mmutex[MYMUTEX_TYPE_ACCOUNT].mutex_type = MYMUTEX_TYPE_ACCOUNT;
	mmutex[MYMUTEX_TYPE_DBQUE].mutex_type = MYMUTEX_TYPE_DBQUE;
	for (i=0;i<MAX_MUTEX_GETTER_NUM;i++) {
		mmgetter[i].process_id = 0;
		mmgetter[i].thread_id = 0;
		for(j=0;j<MAX_MUTEX_NUM;j++) {
			mmgetter[i].get_mutex_type[j] = 0;
			for(k=0;k<MMGETTER_COMMENT_MAX;k++) {
				
				mmgetter[i].comments[j][k] = 0;
			}
		}
		mmgetter[i].wait_mutex_type = 0;
		mmgetter[i].counted = MYMUTEX_NO;
		mmgetter[i].is_used = MYMUTEX_NO;
		mmgetter[i].is_waited = MYMUTEX_NO;
	}
	g_beforelock = beforelock;
	
	if (mmj != 0) {
		for (i=0;i<MAX_MUTEX_NUM;i++) {
			
			mmjyunban.get_mutex_type_jyunban[i] = mmj->get_mutex_type_jyunban[i];
		}
	}
}	

void mymutex_getter_unlock(type_mymutex* mmu, type_mymutexgetter* mmget,char* comment) {
	int k=0;
	int m=0;
	int kk=0;
	int maxk=0;
	char bbool = MYMUTEX_NO;
	#ifdef IAMWINDOWS
	ReleaseMutex(mmu->mutex);
	#else 
	pthread_mutex_unlock(&mmu->mutex);
	#endif
	mmu->mutex_lock_count--;
	for (k=0;k<MAX_MUTEX_NUM;k++) {
		if (mmget->get_mutex_type[k] == mmu->mutex_type) {
			// kに入れる
			kk=k;
			mmget->get_mutex_type[k] = 0;
			for (m=0;m<MMGETTER_COMMENT_MAX;m++) {
				
				mmget->comments[k][m] = 0;
			}
		}
	}
	
	maxk=0;
	for (k=0;k<MAX_MUTEX_NUM;k++) {
		if (mmget->get_mutex_type[k] != 0) {
			if (maxk < k) {
				maxk = k;
			}
		}
	}
	
	if (maxk > kk) {
		mylog(LOG_UNKNOWN, LOG_ERR, "pid=%d tid=%d maxk=%d,kk=%d, unlockする順番がおかしい",mmget->process_id,mmget->thread_id,maxk,kk);
	}
	
	
	bbool = MYMUTEX_NO;
	// 資源がすべてなくなったら空にする
	for (k=0;k<MAX_MUTEX_NUM;k++) {
		if (mmget->get_mutex_type[k] != 0) {
			bbool = MYMUTEX_YES;
			break;
		}
	}
	
	if (MYMUTEX_NO) {
		mmget->process_id = 0;
		mmget->thread_id = 0;
		mmget->is_used = MYMUTEX_NO;
		mmget->is_waited = MYMUTEX_NO;
		mmget->counted = MYMUTEX_NO;
		mmget->wait_mutex_type = MYMUTEX_NO;
	}
}
void mymutex_getter_lock(type_mymutex* mmu, type_mymutexgetter* mmget, char* comment) {
	int k=0;
	int m=0;
	#ifdef IAMWINDOWS
	HANDLE h;
	#endif
	
	#ifdef IAMWINDOWS
	h = OpenMutex(MUTEX_ALL_ACCESS,0,getMutexTypeStr(mmu->mutex_type));
	if (h == NULL) {
		mylog(LOG_UNKNOWN, LOG_ERR, "no mutex");
		return;
	}
		
	#else
	
	mmget->is_waited = MYMUTEX_YES;
	mmget->wait_mutex_type = mmu->mutex_type;
	
	pthread_mutex_unlock(&mmutex[MYMUTEX_TYPE_BEFORELOCK].mutex);
	mylog(LOG_UNKNOWN, LOG_TEST, "no mutexlockmae type=%d pid=%d",mmu->mutex_type,mmget->process_id);
	pthread_mutex_lock(&mmu->mutex);
	mylog(LOG_UNKNOWN, LOG_TEST, "no mutexlockato type=%d pid=%d",mmu->mutex_type,mmget->process_id);
	pthread_mutex_lock(&mmutex[MYMUTEX_TYPE_BEFORELOCK].mutex);
	#endif

	mmu->mutex_lock_count++;
	mmget->is_waited = MYMUTEX_NO;
	mmget->counted = MYMUTEX_NO;
	mmget->wait_mutex_type = 0;
	
	for (k=0;k<MAX_MUTEX_NUM;k++) {
		if (mmget->get_mutex_type[k] == 0) {
			// kに入れる
			mmget->get_mutex_type[k] = mmu->mutex_type;
			for (m=0;m<MMGETTER_COMMENT_MAX;m++) {
				
				mmget->comments[k][m] = comment[m];
			}
			break;
		}
	}

}
// comment char[16];
void mymutex_lock(int mutex_type, char* comment) {
	#ifdef IAMWINDOWS
	HANDLE h;
	#endif
	int i=0;
	int j=0;
	pid_t pid = getpid();
	pid_t tid = gettid();
	int mmindex=0;
	type_mymutexgetter* getter=0;
	int bbindex=0;
	char bbool=MYMUTEX_YES;
	
	mylog(LOG_UNKNOWN, LOG_TEST, "mymutex_type=%d, %s",mutex_type,comment);
	
	if (mutex_type <= MYMUTEX_TYPE_BEFORELOCK) {
		mylog(LOG_UNKNOWN, LOG_WARNING, "pid=%d tid=%d c=%s 異常なmutextype",pid,tid,comment);
		return;
	}
	if (mutex_type >= MYMUTEX_TYPE_MAX) {
		mylog(LOG_UNKNOWN, LOG_WARNING, "pid=%d tid=%d c=%s 異常なmutextype",pid,tid,comment);
		return;
	}
	
	
	// すでに資源とかを持っているかどうかを把握する
	if (mutex_type == MYMUTEX_TYPE_BEFORELOCK) {
		// 何も竹刀
		mylog(LOG_UNKNOWN, LOG_WARNING, "pid=%d tid=%d c=%s beforelockが外部から呼ばれた",pid,tid,comment);
		return;
	}
	
	if (g_beforelock == MYMUTEX_NOUSE_BEFORELOCK) {
		// 使わないのですぐロックしてリターンする
		
		#ifdef IAMWINDOWS
		h = OpenMutex(MUTEX_ALL_ACCESS,0,getMutexTypeStr(mutex_type));
		if (h == NULL) {
			mylog(LOG_UNKNOWN, LOG_ERR, "no mutex");
			return;
		}
		
		#else 
		pthread_mutex_lock(&mmutex[mutex_type].mutex);
		#endif
		return;
	}
	
	
	
	#ifdef IAMWINDOWS
		h = OpenMutex(MUTEX_ALL_ACCESS,0,getMutexTypeStr(MYMUTEX_TYPE_BEFORELOCK));
		if (h == NULL) {
			mylog(LOG_UNKNOWN, LOG_ERR, "no mutex");
			return;
		}
		
	#else 
		pthread_mutex_lock(&mmutex[MYMUTEX_TYPE_BEFORELOCK].mutex);
	#endif
	
	
	
	
	
	for (i=0;i<MAX_MUTEX_GETTER_NUM;i++) {
		if (mmgetter[i].is_used == MYMUTEX_YES) {
			if (mmgetter[i].process_id == pid) {
				if (mmgetter[i].thread_id == tid) {
					getter = &mmgetter[i];
					mmindex = i;
				}
			}
		}
	}
	if (getter == 0) {
		// からからとってくる
		for (i =0;i<MAX_MUTEX_GETTER_NUM;i++) {
			if (mmgetter[i].is_used == MYMUTEX_NO) {
				getter = &mmgetter[i];
				getter->process_id = pid;
				getter->thread_id = tid;
				getter->is_used = MYMUTEX_YES;
				getter->is_waited = MYMUTEX_YES;
				getter->wait_mutex_type = mutex_type;
				mmindex = i;
				break;
			}
		}
		
		if (getter ==0) {
			// どこにもないので無理
			mylog(LOG_UNKNOWN, LOG_ERR, "pid=%d tid=%d c=%s getter不足",pid,tid,comment);
			
			
			#ifdef IAMWINDOWS
				ReleaseMutex(mmutex[MYMUTEX_TYPE_BEFORELOCK].mutex);		
			#else 
				pthread_mutex_unlock(&mmutex[MYMUTEX_TYPE_BEFORELOCK].mutex);
			#endif
			return;
		}
	}
	
	// ロックしようとしているミューテックスがほかのプロセススレッドで使われていないかどうか調べる
	for (i=0;i<MAX_MUTEX_GETTER_NUM;i++) {
		if (mmgetter[i].is_used == MYMUTEX_YES) {
			if (i != mmindex) {
				bbool = MYMUTEX_NO;
				for (j=0;j<MAX_MUTEX_NUM;j++) {
					if (mmgetter[i].get_mutex_type[j] == mutex_type) {
						bbool = MYMUTEX_YES;
						break;
					}
				}
				if (bbool = MYMUTEX_YES) {
					break;
				}
				
				
			}
		}
	}
	
	
	if (bbool == MYMUTEX_NO) {
		// 他にロックしているプロセスがないのでロックしておわり
		if (isLockJyunbanOk(mutex_type, &mmjyunban, &mmgetter[mmindex]) == MYMUTEX_NO) {
			// ロックしている順番がおかしい
			mylog(LOG_UNKNOWN, LOG_ERR, "pid=%d tid=%d c=%s lockする順番がおかしい",pid,tid,comment);
			#ifdef IAMWINDOWS
				ReleaseMutex(mmutex[MYMUTEX_TYPE_BEFORELOCK].mutex);		
			#else 
				pthread_mutex_unlock(&mmutex[MYMUTEX_TYPE_BEFORELOCK].mutex);
			#endif
			
			return;
			
			
		}
		mmgetter[mmindex].is_waited = MYMUTEX_YES;
		mmgetter[mmindex].wait_mutex_type = mutex_type;
		mymutex_getter_lock(&mmutex[mutex_type], &mmgetter[mmindex],comment);
		
		#ifdef IAMWINDOWS
			ReleaseMutex(mmutex[MYMUTEX_TYPE_BEFORELOCK].mutex);		
		#else 
			pthread_mutex_unlock(&mmutex[MYMUTEX_TYPE_BEFORELOCK].mutex);
		#endif
		return;
	}
	
	if (isLockJyunbanOk(mutex_type, &mmjyunban, &mmgetter[mmindex]) == MYMUTEX_NO) {
			// ロックしている順番がおかしい
			mylog(LOG_UNKNOWN, LOG_ERR, "pid=%d tid=%d c=%s lockする順番がおかしい",pid,tid,comment);
	}
	
	// 他にロックしているプロセスがある場合はデッドロックの可能性があるので調べる
	// デッドロックする予定になっている場合はログにその旨を書いてロックしてデッドロックさせる
	// デッドロックする予定になっていない場合はロックしておわり
	mmgetter[mmindex].is_waited = MYMUTEX_YES;
	mmgetter[mmindex].wait_mutex_type = mutex_type;
	
	
	
	if (isDeadLock(mutex_type, &mmutex[mutex_type], &mmgetter[mmindex], mmgetter) == MYMUTEX_YES) {
		mylog(LOG_UNKNOWN, LOG_ERR, "pid=%d tid=%d c=%s deadlock",pid,tid,comment);
	/*	for (i=0;i<MAX_MUTEX_GETTER_NUM;i++) {
			mylog(LOG_UNKNOWN, LOG_ERR, "dl:%d,%d,%d,%d,%d,%d,%d,%d,",mmgetter[i].get_mutex_type[0],mmgetter[i].get_mutex_type[1],mmgetter[i].get_mutex_type[2],
				mmgetter[i].get_mutex_type[3],mmgetter[i].get_mutex_type[4],mmgetter[i].get_mutex_type[5],mmgetter[i].get_mutex_type[6],
				mmgetter[i].get_mutex_type[7]
			);
		}*/
	}
	
	mymutex_getter_lock(&mmutex[mutex_type], &mmgetter[mmindex],comment);
	
	if (isLockJyunbanOk(mutex_type, &mmjyunban, &mmgetter[mmindex]) == MYMUTEX_NO) {
			// ロックしている順番がおかしい
			mylog(LOG_UNKNOWN, LOG_ERR, "pid=%d tid=%d c=%s lockする順番がおかしい",pid,tid,comment);
	}
	
	#ifdef IAMWINDOWS
		ReleaseMutex(mmutex[MYMUTEX_TYPE_BEFORELOCK].mutex);
	#else 
		pthread_mutex_unlock(&mmutex[MYMUTEX_TYPE_BEFORELOCK].mutex);
	#endif
	
}

void mymutex_unlock(int mutex_type, char* comment) {
	int i;
	#ifdef IAMWINDOWS
	HANDLE h;
	#endif
	int mmindex=0;
	pid_t pid = getpid();
	pid_t tid = gettid();
	type_mymutexgetter* getter=0;
	
	mylog(LOG_UNKNOWN, LOG_TEST, "mymutex_unlock type=%d %s",mutex_type,comment);
	if (mutex_type <= MYMUTEX_TYPE_BEFORELOCK) {
		mylog(LOG_UNKNOWN, LOG_WARNING, "pid=%d tid=%d 異常なmutextype",pid,tid);
		return;
	}
	if (mutex_type >= MYMUTEX_TYPE_MAX) {
		mylog(LOG_UNKNOWN, LOG_WARNING, "pid=%d tid=%d 異常なmutextype",pid,tid);
		return;
	}
	
	
	// すでに資源とかを持っているかどうかを把握する
	
	if (g_beforelock == MYMUTEX_NOUSE_BEFORELOCK) {
		// 使わないのですぐロックしてリターンする
		
		#ifdef IAMWINDOWS
		ReleaseMutex(mmutex[mutex_type].mutex);
		
		#else 
		pthread_mutex_unlock(&mmutex[mutex_type].mutex);
		#endif
		return;
	}
	
	
	
	#ifdef IAMWINDOWS
		h = OpenMutex(MUTEX_ALL_ACCESS,0,getMutexTypeStr(MYMUTEX_TYPE_BEFORELOCK));
		if (h == NULL) {
			mylog(LOG_UNKNOWN, LOG_ERR, "no mutex");
			return;
		}
		
	#else 
		pthread_mutex_lock(&mmutex[MYMUTEX_TYPE_BEFORELOCK].mutex);
	#endif
	
	
	for (i=0;i<MAX_MUTEX_GETTER_NUM;i++) {
		if (mmgetter[i].is_used == MYMUTEX_YES) {
			if (mmgetter[i].process_id == pid) {
				if (mmgetter[i].thread_id == tid) {
					getter = &mmgetter[i];
					mmindex = i;
					break;
				}
			}
		}
	}
	
	if (getter ==0) {
		mylog(LOG_UNKNOWN, LOG_WARNING, "pid=%d tid=%d lockしていない資源をunlock",pid,tid);
		
		
		#ifdef IAMWINDOWS
		ReleaseMutex(mmutex[MYMUTEX_TYPE_BEFORELOCK].mutex);
		
		#else 
		pthread_mutex_unlock(&mmutex[MYMUTEX_TYPE_BEFORELOCK].mutex);
		#endif
		
		return;
	}
	
	
	mymutex_getter_unlock(&mmutex[mutex_type], &mmgetter[mmindex],comment);
	#ifdef IAMWINDOWS
		ReleaseMutex(mmutex[MYMUTEX_TYPE_BEFORELOCK].mutex);		
	#else 
		pthread_mutex_unlock(&mmutex[MYMUTEX_TYPE_BEFORELOCK].mutex);
	#endif
	
}

void release_mymutex() {

	
	#ifdef IAMWINDOWS
	CloseHandle(mutex);
	#else
	#endif
	
	munmap(mymutexmemory, sizeof(type_mymutex)*MAX_MUTEX_NUM + sizeof(type_mymutexgetter)* MAX_MUTEX_GETTER_NUM);
	shm_unlink("/shmmymutex");
}



int main() {
	type_mymutex_jyunban m;
	int i=0;
	int status;
	int pid = 0;
	for (i=0;i<MAX_MUTEX_NUM;i++) {
		m.get_mutex_type_jyunban[i] =i;
	}
	initlog();
	init_mymutex(MYMUTEX_USE_BEFORELOCK,&m);
	
	pid = fork();
	
	if (pid>0) {
	//	pthread_mutex_lock(&mmutex[1].mutex);
		mymutex_lock(MYMUTEX_TYPE_ACCOUNT,"oyaccountlo");
		sleep(1);
	//	pthread_mutex_lock(&mmutex[2].mutex);
//		pthread_mutex_lock(&mmutex[1].mutex);
		mymutex_lock(MYMUTEX_TYPE_DBQUE,"oydbquelo");
		sleep(4);
	
	//	pthread_mutex_unlock(&mmutex[2].mutex);
	//	pthread_mutex_unlock(&mmutex[1].mutex);
		mymutex_unlock(MYMUTEX_TYPE_DBQUE, "dbque oyare");
		mymutex_unlock(MYMUTEX_TYPE_ACCOUNT, "acc oyare");
		sleep(5);
		// 子プロセスを殺す
		kill(pid,SIGHUP);
		wait(&status);
		sleep(2);
		release_mymutex();
		releaselog();
		return 1;
		
		
		
	}
	
	mylog(LOG_UNKNOWN, LOG_TEST, "testdesu");
	mylog(LOG_UNKNOWN, LOG_ERR, "testdesuyo\n");
	mylog(LOG_UNKNOWN, LOG_ATTACK, "teet");
	
//	pthread_mutex_lock(&mmutex[2].mutex);
	mymutex_lock(MYMUTEX_TYPE_DBQUE,"kodbquelo");
	sleep(1);
//	pthread_mutex_lock(&mmutex[1].mutex);
//	pthread_mutex_lock(&mmutex[2].mutex);
	mymutex_lock(MYMUTEX_TYPE_ACCOUNT,"koaccountlo");
	sleep(1);
	mymutex_unlock(MYMUTEX_TYPE_ACCOUNT,"ko accque");
	mymutex_unlock(MYMUTEX_TYPE_DBQUE, "ko dbque");	
//	pthread_mutex_unlock(&mmutex[1].mutex);
//	pthread_mutex_unlock(&mmutex[2].mutex);
		
	
}

