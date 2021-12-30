#pragma once
#include "item.h"
#include "mob.h"
#include "gui.h"

extern struct _error;

////////////////
//   �ṹ����  //
////////////////

//	����ṹ��
//	�������͵�ö������ֵ����ÿ���ļ�ֵ
enum mineKind { stone = 1, iron = 5, gold = 15, diamond = 20, emerald = 30};
enum mineState { hooked, available };
typedef struct _node {
	enum mineKind kind;				//	kind��ʾ�Ϳ�������
	enum mineState state;			//	state��ʾ״̬
	int size;						//	����ߴ��С(32�ı���)
	int Pos_x;	int Pos_y;			//	xy�Ǻ�������
	struct _node* next;
} MINE;

//	�ؿ��ṹ��
typedef struct _level {
	int currentLevel;				//	��ǰ�ؿ�
	int gameScore;					//	��ǰ�÷�
	int timeLast;					//	ʣ��ʱ��
	int passScore;					//	ͨ������Ҫ�ĵ÷�
} LEVEL;

//	���ӽṹ��
enum rollTempKind { right, left };
enum hookState { hang, extension, back };
typedef struct _hook {
	int hookLong;					//	��ǰ���ӳ�
	float angle;					//	��ǰ�Ƕ�
	int startPointHook_X;			//	��ʼ�������
	int startPointHook_Y;			//	��ʼ��������
	int endPointX;					//	�����������
	int endPointY;					//	������������
	rollTempKind rollTemp;			//	����ڻ������Ұڣ�
	int hookConductance;			//	���������������ӹ�ס����֮��������ٶ�
	hookState hookNowState;			//	��ǰ����������ת�������쳤�����ǻ�����
	int func;						//	�˹���Ŀǰ���еĹ���-����߹ҹ�
} HOOK;

//	�û��ṹ��
typedef struct _user {
	char userName[64];
	LEVEL userLevel;

	struct _user* next;
} PLAYER;

//	���ýṹ��
typedef struct _settings {
	bool putItems;
	bool autoSave;
	bool doMobSpawn;
	char resourcepacksName[32];
	//char DLCname[32][32];
	char KB_quitGame;
	char KB_saveGame;
	char KB_enlongHook;
	char KB_pauseGame;

	bool debugMode;
} SETTING ;

////////////////
//   ��������  //
////////////////
void startup(PLAYER* player, MINE** head, ITEMLIST* i_head, MOBHEAD* mobList, struct _error* errorCode, const SETTING* gameSetting);
void show(const PLAYER* thisPlayer, const HOOK* thisHook, const MINE* head, const ITEMLIST* i_head, const MOBHEAD* mobList, struct _error* errorCode, const SETTING* gameSetting);
void updateWithoutInput(PLAYER* player, HOOK* thisHook, MINE* m_head, ITEMLIST* i_head, MOBHEAD* mobList, struct _error* errorCode, SETTING* gameSetting);
void updateWithInput(PLAYER*, MINE * m_head, HOOK*, ITEMLIST* i_head, struct _error*, SETTING * gameSetting);
