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

// login とconnect 関連
// refused_account と　refused_addr についてはsqlのほうで対応する
// account_table に　refused_flg refused_dt を持たせる
// refused_addr_table を作る
// connect の際に refused_addr_table を見て　該当している場合は切断する
// loginの際に　refused_account を見て 該当している場合は切断する
// err_packetが規定の数を超えているまたはpacketが規定の数を超えている場合 err_count を増やす
// err_count が規定の数を超えたaccountはrefused_accountにして切断する
// loginの際に　id またはpasswordを間違えたのが10回続くとrefused_addr に登録する 
// refused_addr に関して tableには登録しない　手動でinsertする

// login 履歴に関しては記録する logに残す
// テーブル（user_login_table)にも　login成功と失敗の記録を残しておく
// memcache をどこに使うか
// 応答性を迅速にするために　初期化起動時にメモリ上に展開するのでmemcacheの出番は少ないかもしれない

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


