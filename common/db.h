// db�����ɂ��Ắ@�X���b�h�������炩���ĂĂ��̃X���b�h�ŃL���[�����Ă���ꍇ�͐������������Ă����`�ɂȂ�

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
	time_t send_time;
	unsigned int db_func_id;
	unsigned int data1;
	unsigned int data2;
	void* data_void;
	unsigned short data3;
	unsigned char status; // �X�V���܂ޏ������ǂ��� // �ύX���s����f�[�^���܂ޏ������ǂ���  �ǂ���ł��Ȃ��ꍇmemcached��db��select���e��ۑ�����
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