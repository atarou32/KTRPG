#define NPC_H
// npc ����N�G�X�g���󂯂�
// �N�G�X�g��B�����邩�@�j�����邩�ň��؂�
// ��x�Ɉ�����N�G�X�g���󂯂��Ȃ�
// �N�G�X�g���G��|����
// �N�G�X�g�ɎQ�����Ă���S���Ƀh���b�v�������悤�ɂȂ��Ă���
// �h���b�v�͎����ŃQ�b�g�ł���킯�ł͂Ȃ�
// �E��Ȃ��ƃQ�b�g�ł��Ȃ�
// �N�G�X�g�̒B�����͕̂��u���ĂĂ��ł���

// ��x�ɎQ���ł���v���C���[�̎Q���\�l����
// 16�l���ō��ɂ��悤


#define NPC_QUEST_CHARACTER_MAX 16

#define NPC_SKILL_MAX 8
#define NPC_QUEST_INFO_POWERCOMMENT_TYPES_MAX 32 // 32��ނ̃^�C�v����̃N�G�X�g�ŃQ�b�g�ł���\��������

// NPC�̓A�C�e���𗎂Ƃ��Ȃ�

typedef struct npc_character {
  unsigned int npc_id;
  unsigned short region_id; // �Q�[�����r�[��ID�ɂ���
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


// �G�l�~�[�̓A�C�e���𗎂Ƃ�
// �G�l�~�[�̓N�G�X�g�Ƃ͕R�Â��Ȃ�

typedef struct npc_enemy {
  unsigned int enemy_id;
	
  unsigned short region_id; // �G�l�~�[���o�ꂷ��ꏊ��ID�ɂ���
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
  unsigned int AI; // ��؁@�Ȃǂ̃I�u�W�F�N�g���G�l�~�[�Ƃ��ēo�^���� AI�ōs���p�^�[����ݒ肷��

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
	unsigned short region_id; // �N�G�X�g���s����ꏊ
	unsigned char need_lv; // �󂯂�̂ɕK�v�ȃ��x��
	unsigned int npc_id; // �N�G�X�g���󂯂���@NPC
	unsigned int reward_gold;
	unsigned int reward_item_info_id1; // powercomment �Ɓ@materialize �͂Ȃ�
	unsigned int item1_count;
	unsigned int reward_item_info_id2;
	unsigned int item2_count;
	unsigned int reward_item_info_id3;
	unsigned int item3_count;
	unsigned int quest_completed_type;
	char quest_info_comment[256];
	unsigned int commentlen;
	
	// �N�G�X�g�ɂ���ā@�p���[�R�����g�̊��������肷��
	
} type_npc_quest_info;

typedef struct npc_quest_info_power_comment {
	unsigned int quest_info_id;
	float percent_powercomment_slot1; // �X���b�g���ő�1�ł���m��
	float percent_powercomment_slot2; // �X���b�g���ő�2�ł���m��
	float percent_powercomment_slot3; // �X���b�g���ő�3�ł���m��
	float percent_powercomment_slot4; // �X���b�g���ő�4�ł���m��
	float percent_powercomment_slot5; // �X���b�g���ő�5�ł���m��
	float percent_powercomment_slot6; // �X���b�g���ő�6�ł���m��
	float percent_powercomment_slot7; // �X���b�g���ő�7�ł���m��
	float percent_powercomment_slot8; // �X���b�g���ő�8�ł���m��
	
	unsigned int power_types[NPC_QUEST_INFO_POWERCOMMENT_TYPES_MAX];
	float percent_power_types[NPC_QUEST_INFO_POWERCOMMENT_TYPES_MAX];
} type_npc_quest_info_power_comment;


typedef struct npc_quest {
  unsigned int quest_id;
  unsigned int quest_info_id;
  unsigned int quest_character_id[NPC_QUEST_CHARACTER_MAX];
  unsigned int start_time;
  unsigned int end_time;
  unsigned int clear_flg; // �ڕW���B�����ꂽ���ǂ���
  unsigned int use_flg;
} type_npc_quest;




