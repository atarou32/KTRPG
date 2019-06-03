#ifndef CONNECT_H
#include "connect.h"
#endif
#define CHARACTER_H

typedef enum {
character_state_empty,
character_state_alive,
character_state_dead,
character_state_preerase,
} type_character_state;



#define CHARACTER_SKILL_MAX 64
#define CHARACTER_ITEM_MAX 48
#define CHARACTER_ITEM_MATRIALIZE_SLOT_MAX 4
#define CHARACTER_ITEM_POWERCOMMENT_SLOT_MAX 8
#define CHARACTER_BANK_ITEM_MAX 256

typedef struct character {
unsigned int character_id;

unsigned int userno;
unsigned int account_id;// accountのID
type_character_state character_state;
char name[32];
char namelen;
char gender;
char race;
unsigned char lv;
int exp;
int skillpoint;

short HP;
short MAXHP;
short ST;
short MAXST;

short MP;
short MAXMP;
short atk;
short def;

unsigned short WEIGHT;
unsigned short MAX_WEIGHT;

// 回避はすべて移動回避のみ
// 攻撃はすべて必中



short move_speed;
short direction;
short up_direction;
unsigned short region_id;

float posx;
float posy;
float posz;

unsigned int model_id;
	
// 装備品のID (item_id)
unsigned int rarm;
unsigned int larm;
unsigned int specialarm;

unsigned int head;
unsigned int arm;
unsigned int foot;
unsigned int body;
unsigned int ring1;
unsigned int ring2;
unsigned int earring1;
unsigned int earring2;
unsigned int pendant;


short calc_HP;
short calc_MAXHP;

short calc_ST;
short calc_MAXST;

short calc_MP;
short calc_MAXMP;

short calc_ATK;
short calc_DEF;

short calc_MOVESPEED;
unsigned short calc_WEIGHT;
unsigned short calc_MAXWEIGHT;
	
unsigned int learned_skill[CHARACTER_SKILL_MAX];
short skill_cool[CHARACTER_SKILL_MAX];

// item_id
unsigned int inventory[CHARACTER_ITEM_MAX];
unsigned short inventory_count[CHARACTER_ITEM_MAX];
	

short item_cool; // アイテムは一緒のディレイになる
unsigned short offset;
unsigned int gold;
unsigned int login_alltime;// ログイン時間
unsigned int quest_id; // 今受けているクエストのID

	// キャラクターメイキングのパラメータもここにあとで表記する
} type_character;


typedef struct item_info {
	unsigned int item_info_id;
	char item_name[32];
	unsigned char itemnamelen;
	unsigned char commentlen;
	char comment [64];
	unsigned int item_type;
	unsigned short power1;
	unsigned short power2;
	unsigned int powercommentlen;
	char powercomment[128]; // 特殊な効果　power1 power2では表現しきれないものをここで表現する
    unsigned char MAXHP;
	unsigned char materialize_slot;
	unsigned short weight;
	unsigned char can_trade;
	unsigned char can_sell;
	unsigned short rare;
	unsigned int sell_price;
	unsigned int price;
	
} type_item_info;


typedef struct item {
	unsigned int item_id;
	unsigned char powerize; // 25段階まで強化できる 強化すると性能が上がる
	unsigned char materialized_slot[CHARACTER_ITEM_MATERIALIZE_SLOT_MAX];
	
	type_item_info* item_info;
	char powercomment[128];
	unsigned short powercommentlen; // アイテムはドロップ時にパワーコメント付きでゲットできる
	unsigned short powercomment_slot; // スロット数 8 が最高 item_infoのパワーコメントとは別々に数える
	
	unsigned char MAXHP;
	unsigned char HP; // HPが0になると 壊れた状態になり性能が0になる

	unsigned short offset;
	
} type_item;

typedef struct materialize {
  unsigned int materialize_id;
  unsigned int materialize_type;
  char materialize_name[32];
  char materialize_comment[64];
} type_materialize;

typedef struct powercomment {
	unsigned int powercomment_id;
	char powercomment_name[32];
	unsigned int powercommentlen;
	unsigned int power_type; // どの項目が強化されるかはtype と関数から計算する
} type_powercomment;

// パワーコメントの強化合成について
// 武器を素材として消費することによって　対象とした武器にパワーコメントを付与することができる

typedef struct skill {
  unsigned int skill_id;
  unsigned int skill_type;
  char skill_name[32];
  unsigned char skillnamelen;
  unsigned char need_lv;
  unsigned short need_skillpoint;
  unsigned short need_special_type;
  unsigned short shouhi_hp;
	
  unsigned short shouhi_mp;
  unsigned short shouhi_st;
  short cool;
  short power1;
	
  char comment[64];
  int commentlen;
} type_skill;


typedef struct bank {
	unsigned int account_id;
	int gold;
	int max_item_count;
	unsigned int item_ids[CHARACTER_BANK_ITEM_MAX];
	unsigned int item_count[CHARACTER_BANK_ITEM_MAX];
} type_bank;

