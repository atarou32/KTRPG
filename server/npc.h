#define NPC_H
// npc からクエストを受けて
// クエストを達成するか　破棄するかで一区切り
// 一度に一つしかクエストを受けられない
// クエスト中敵を倒すと
// クエストに参加している全員にドロップがいくようになっている
// ドロップは自動でゲットできるわけではなく
// 拾わないとゲットできない
// クエストの達成自体は放置しててもできる

// 一度に参加できるプレイヤーの参加可能人数は
// 16人が最高にしよう


#define NPC_QUEST_CHARACTER_MAX 16

#define NPC_SKILL_MAX 8
#define NPC_QUEST_INFO_POWERCOMMENT_TYPES_MAX 32 // 32種類のタイプが一つのクエストでゲットできる可能性がある

// NPCはアイテムを落とさない

typedef struct npc_character {
  unsigned int npc_id;
  unsigned short region_id; // ゲームロビーのIDにする
  char name[32];
  unsigned char namelen;
  unsigned char commentlen;
  char comment[128];
  int gender;
	
  char race;
  unsigned char lv;	
  short HP;
	
  short MAXHP;
  short atk;
	
  short def;
  short move_speed;
	
  short direction;
  short up_direction;
  float posx;
  float posy;
  float posz;
	
  unsigned int model_id;
  unsigned int learned_skill[NPC_SKILL_MAX];
  short skill_cool[NPC_SKILL_MAX];
  short item_cool;
  short offset;
} type_npc_character;


// エネミーはアイテムを落とす
// エネミーはクエストとは紐づけない

typedef struct npc_enemy {
  unsigned int enemy_id;
	
  unsigned short region_id; // エネミーが登場する場所のIDにする
  char name[32];
  unsigned char namelen;
  unsigned char commentlen;
  char comment[128];
  int gender;
	
  char race;
  unsigned char lv;	
  short HP;
	
  short MAXHP;
  short atk;
	
  short def;
  short move_speed;
	
  short direction;
  short up_direction;
  float posx;
  float posy;
  float posz;
	
  unsigned int model_id;
  unsigned int learned_skill[NPC_SKILL_MAX];
  short skill_cool[NPC_SKILL_MAX];
  short item_cool;
  short offset;
  short repop_cool;
  short dead_flg;
  unsigned int AI; // 大木　などのオブジェクトもエネミーとして登録する AIで行動パターンを設定する

} type_npc_enemy;


typedef struct npc_enemy_drop_info_item {
  unsigned int enemy_id;
  unsigned int quest_info_id;
  unsigned short region_id;

  unsigned int goldmax;
  unsigned int goldmin;
  unsigned int exp;
  
  unsigned int percent_item1;
  unsigned int item1_count_max;
  unsigned int item1_count_min;
  unsigned int item1_item_info_id;
 
  unsigned int percent_item2;
  unsigned int item2_count_max;
  unsigned int item2_count_min;
  unsigned int item2_item_info_id;

  unsigned int percent_item2;
  unsigned int item2_count_max;
  unsigned int item2_count_min;
  unsigned int item2_item_info_id;	
	
  unsigned int percent_item3;
  unsigned int item3_count_max;
  unsigned int item3_count_min;
  unsigned int item3_item_info_id;	
	
  unsigned int percent_item4;
  unsigned int item4_count_max;
  unsigned int item4_count_min;
  unsigned int item4_item_info_id;	
	
  unsigned int percent_item5;
  unsigned int item5_count_max;
  unsigned int item5_count_min;
  unsigned int item5_item_info_id;		
	
	
} type_npc_enemy_drop_info_item;


typedef struct npc_enemy_drop_item {
  unsigned int drop_id;
	
  unsigned int enemy_id;
  unsigned int quest_info_id;
  unsigned int quest_id;
  unsigned int character_id;
  unsigned short region_id;
  unsigned short get_flg;
  unsigned int gold;
  
  unsigned int itemid1;
  unsigned int item1_count;
	
  unsigned int itemid2;
  unsigned int item2_count;
  unsigned int itemid3;
  unsigned int item3_count;
	
  unsigned int itemid4;
  unsigned int item4_count;

  unsigned int itemid5;
  unsigned int item5_count;
  float posx;
  float posy;
  float posz;
  unsigned short limit_time;
  unsigned short use_flg;
}
typedef struct npc_quest_info {
	unsigned int quest_info_id;
	char quest_info_name[32];
	unsigned int limit_time;
	unsigned char quest_info_name_len;
	unsigned short region_id; // クエストが行われる場所
	unsigned char need_lv; // 受けるのに必要なレベル
	unsigned int npc_id; // クエストが受けられる　NPC
	unsigned int reward_gold;
	unsigned int reward_item_info_id1; // powercomment と　materialize はなし
	unsigned int item1_count;
	unsigned int reward_item_info_id2;
	unsigned int item2_count;
	unsigned int reward_item_info_id3;
	unsigned int item3_count;
	unsigned int quest_completed_type;
	char quest_info_comment[256];
	unsigned int commentlen;
	
	// クエストによって　パワーコメントの割合を決定する
	
} type_npc_quest_info;

typedef struct npc_quest_info_power_comment {
	unsigned int quest_info_id;
	float percent_powercomment_slot1; // スロットが最大1である確率
	float percent_powercomment_slot2; // スロットが最大2である確率
	float percent_powercomment_slot3; // スロットが最大3である確率
	float percent_powercomment_slot4; // スロットが最大4である確率
	float percent_powercomment_slot5; // スロットが最大5である確率
	float percent_powercomment_slot6; // スロットが最大6である確率
	float percent_powercomment_slot7; // スロットが最大7である確率
	float percent_powercomment_slot8; // スロットが最大8である確率
	
	unsigned int power_types[NPC_QUEST_INFO_POWERCOMMENT_TYPES_MAX];
	float percent_power_types[NPC_QUEST_INFO_POWERCOMMENT_TYPES_MAX];
} type_npc_quest_info_power_comment;


typedef struct npc_quest {
  unsigned int quest_id;
  unsigned int quest_info_id;
  unsigned int quest_character_id[NPC_QUEST_CHARACTER_MAX];
  unsigned int start_time;
  unsigned int end_time;
  unsigned int clear_flg; // 目標が達成されたかどうか
  unsigned int use_flg;
} type_npc_quest;




