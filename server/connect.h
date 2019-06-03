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

// login ‚Æconnect ŠÖ˜A
// refused_account ‚Æ@refused_addr ‚É‚Â‚¢‚Ä‚Ísql‚Ì‚Ù‚¤‚Å‘Î‰‚·‚é
// account_table ‚É@refused_flg refused_dt ‚ğ‚½‚¹‚é
// refused_addr_table ‚ğì‚é
// connect ‚ÌÛ‚É refused_addr_table ‚ğŒ©‚Ä@ŠY“–‚µ‚Ä‚¢‚éê‡‚ÍØ’f‚·‚é
// login‚ÌÛ‚É@refused_account ‚ğŒ©‚Ä ŠY“–‚µ‚Ä‚¢‚éê‡‚ÍØ’f‚·‚é
// err_packet‚ª‹K’è‚Ì”‚ğ’´‚¦‚Ä‚¢‚é‚Ü‚½‚Ípacket‚ª‹K’è‚Ì”‚ğ’´‚¦‚Ä‚¢‚éê‡ err_count ‚ğ‘‚â‚·
// err_count ‚ª‹K’è‚Ì”‚ğ’´‚¦‚½account‚Írefused_account‚É‚µ‚ÄØ’f‚·‚é
// login‚ÌÛ‚É@id ‚Ü‚½‚Ípassword‚ğŠÔˆá‚¦‚½‚Ì‚ª10‰ñ‘±‚­‚Ærefused_addr ‚É“o˜^‚·‚é 
// refused_addr ‚ÉŠÖ‚µ‚Ä table‚É‚Í“o˜^‚µ‚È‚¢@è“®‚Åinsert‚·‚é

// login —š—ğ‚ÉŠÖ‚µ‚Ä‚Í‹L˜^‚·‚é log‚Éc‚·
// ƒe[ƒuƒ‹iuser_login_table)‚É‚à@login¬Œ÷‚Æ¸”s‚Ì‹L˜^‚ğc‚µ‚Ä‚¨‚­
// memcache ‚ğ‚Ç‚±‚Ég‚¤‚©
// ‰“š«‚ğv‘¬‚É‚·‚é‚½‚ß‚É@‰Šú‰»‹N“®‚Éƒƒ‚ƒŠã‚É“WŠJ‚·‚é‚Ì‚Åmemcache‚Ìo”Ô‚Í­‚È‚¢‚©‚à‚µ‚ê‚È‚¢

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


