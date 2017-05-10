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
	unsigned char addr[32];
	unsigned short port;
	unsigned short err_packet_per_sec;
	unsigned short err_packet_per_min;
	unsigned short err_packet_per_hour;
} type_session;

typedef struct account {
	char userid[32];
	char password[32];
	unsigned short ulen;
	unsigned short plen;
	unsigned short userno;
	type_session session;
	void* now_character;
} type_account;

typedef struct refused_account {
	char userid[32];
	unsigned short ulen;
	unsigned short isuse;
} type_refused_account;

typedef struct refused_addr {
	char addr[30];
	unsigned short isuse;
};
