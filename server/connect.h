#define CONNECT_H
#include "time.h"

typedef enum {
conn_state_empty,
conn_state_authed,
conn_state_game,
conn_state_destroy
} type_conn_state;

typedef struct connection {
int sock;
type_conn_state conn_state;
time_t login_time;
char addr[32];
unsigned short port;
unsigned short err_packet_per_sec;
unsigned short err_packet_per_min;
unsigned short err_packet_per_hour;
unsigned short packet_per_sec;
unsigned short packet_per_min;
unsigned short packet_per_hour;
unsigned short err_count;
} type_session;

typedef struct account {
	char userid[32];
	char password[32];
	unsigned short ulen;
	unsigned short plen;
	unsigned int userno;
	unsigned int id;
	type_session* session;
	void* now_character;
} type_account;

typedef struct refused_addr {
	char addr[32];
	unsigned short isuse;
	unsigned short offset;
} type_refused_addr;

// login ��connect �֘A
// refused_account �Ɓ@refused_addr �ɂ��Ă�sql�̂ق��őΉ�����
// account_table �Ɂ@refused_flg refused_dt ����������
// refused_addr_table �����
// connect �̍ۂ� refused_addr_table �����ā@�Y�����Ă���ꍇ�͐ؒf����
// login�̍ۂɁ@refused_account ������ �Y�����Ă���ꍇ�͐ؒf����
// err_packet���K��̐��𒴂��Ă���܂���packet���K��̐��𒴂��Ă���ꍇ err_count �𑝂₷
// err_count ���K��̐��𒴂���account��refused_account�ɂ��Đؒf����
// login�̍ۂɁ@id �܂���password���ԈႦ���̂�10�񑱂���refused_addr �ɓo�^���� 
// refused_addr �Ɋւ��� table�ɂ͓o�^���Ȃ��@�蓮��insert����

// login �����Ɋւ��Ă͋L�^���� log�Ɏc��
// �e�[�u���iuser_login_table)�ɂ��@login�����Ǝ��s�̋L�^���c���Ă���
// memcache ���ǂ��Ɏg����
// ��������v���ɂ��邽�߂Ɂ@�������N�����Ƀ�������ɓW�J����̂�memcache�̏o�Ԃ͏��Ȃ���������Ȃ�

void init_connection_array();
void release_connection_array();

typedef enum {
	type_conn_full,
	type_conn_error,
	type_conn_ok
} type_conn_make;

type_conn_make makeConnection(int sock, time_t login_time, char* addr, unsigned short port);

typedef enum {
	type_account_full,
	type_account_error,
	type_account_alreadylogin,
	type_account_alreadymake,
	type_account_ok
} type_account_state;

type_account_state makeAccount(char* userid, char* password, int ulen, int plen, type_connection* conn);
type_account* loginAccount(char* userid, char* password, int ulen, int plen, type_connection* conn);


