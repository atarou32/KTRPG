
#include "time.h"

#define ACCOUNT_H

#define ACCOUNT_ERR_CHECK_LOOPTIME_SECOND 1
#define ACCOUNT_ERR_CHECK_LOOPTIME_MINUTE 60
#define ACCOUNT_ERR_CHECK_LOOPTIME_HOUR 3600

// 下記の値を超えるとrefuseされる
// またエラーが起こるごとにlogerr.txtに書かれる
// refusedされたとき　また明らかに攻撃されたことがわかったときはlogattack.txtに書かれる

#define ACCOUNT_ERR_CHECK_SECOND_MAX 3
#define ACCOUNT_ERR_CHECK_MINUTE_MAX 10
#define ACCOUNT_ERR_CHECK_HOUR_MAX 20

#define ACCOUNT_SHORI_OK 1
#define ACCOUNT_SHORI_DEKINAI 0



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
	time_t err_check_time;
	unsigned char addr[32];
	unsigned short port;
	unsigned short err_packet_per_sec;
	unsigned short err_packet_per_min;
	unsigned short err_packet_per_hour;
	unsigned short packet_per_sec;
	unsigned short packet_per_min;
	unsigned short packet_per_hour;
	unsigned short is_tcp_or_udp;
} type_session;

typedef struct account {
	char userid[32];
	char password[32];
	unsigned short ulen;
	unsigned short plen;
	unsigned short userno;
	unsigned char fpacket_cont_index; // 
	unsigned char is_has_fpacket;
	type_session session;
	void* now_character;
} type_account;

typedef struct refused_account {
	char userid[32];
	unsigned short ulen;
	unsigned short isuse;
	time_t refused_date;
} type_refused_account;

typedef struct refused_addr {
	char addr[28];
	unsigned short port;
	unsigned short isuse;
	time_t refused_date;
} type_refused_addr;

void init_account_array();

int hantei_refuse_account_and_addr(time_t time);
int add_refused_account(char* userid, time_t time);
int add_refused_addr(char* user_addr, unsigned short user_port, time_t time);
int db_save_refused_account_and_addr();
void release_account_array();

