// db処理については　スレッドをいくらか立ててそのスレッドでキューを見てある場合は随時処理をしていく形になる

#ifdef THISISTEST
#define DB_USER
#define DB_PASS
#define DB_ADDRESS
#define DB_PORT
#define MEMCACHED_ADDRESS
#define MEMCACHED_PORT

#else
#define DB_USER
#define DB_PASS
#define DB_ADDRESS
#define DB_PORT
#define MEMCACHED_ADDRESS
#define MEMCACHED_PORT

#endif

#define DB_QUE_STATUS_HAS_KOUSIN 1
#define DB_QUE_STATUS_HAS_CHANGEDATA 2
#define DB_QUE_STATUS_ISUSE 1
#define DB_QUE_STATUS_NOUSE 0

typedef struct db_que {
	unsigned int index1;
	unsigned int index2;
	unsigned int index3;
	void* data;
	unsigned short index4;
	unsigned char status; // 更新を含む処理かどうか // 変更が行われるデータを含む処理かどうか  どちらでもない場合memcachedにdbのselect内容を保存する
	unsigned char isuse;
	void* sql_conn;
} type_db_que;


#define MAX_DB_THREAD_NUM 8
#define MAX_DB_QUE_NUM 1024
unsigned char g_dbthread_func_do;

void DB_init();
void DB_threadfunc(void*);
void DB_sendQueue(type_db_que* nque);
void DB_release();