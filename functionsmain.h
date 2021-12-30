#pragma once
#include "item.h"
#include "mob.h"
#include "gui.h"

extern struct _error;

////////////////
//   结构体们  //
////////////////

//	矿物结构体
//	矿物类型的枚举体数值代表该矿物的价值
enum mineKind { stone = 1, iron = 5, gold = 15, diamond = 20, emerald = 30};
enum mineState { hooked, available };
typedef struct _node {
	enum mineKind kind;				//	kind表示就矿物类型
	enum mineState state;			//	state表示状态
	int size;						//	矿物尺寸大小(32的倍数)
	int Pos_x;	int Pos_y;			//	xy是横纵坐标
	struct _node* next;
} MINE;

//	关卡结构体
typedef struct _level {
	int currentLevel;				//	当前关卡
	int gameScore;					//	当前得分
	int timeLast;					//	剩余时间
	int passScore;					//	通关所需要的得分
} LEVEL;

//	钩子结构体
enum rollTempKind { right, left };
enum hookState { hang, extension, back };
typedef struct _hook {
	int hookLong;					//	当前钩子长
	float angle;					//	当前角度
	int startPointHook_X;			//	开始点横坐标
	int startPointHook_Y;			//	开始点纵坐标
	int endPointX;					//	结束点横坐标
	int endPointY;					//	结束点纵坐标
	rollTempKind rollTemp;			//	向左摆还是向右摆？
	int hookConductance;			//	“钩导”，即钩子钩住东西之后的运行速度
	hookState hookNowState;			//	当前钩子是在旋转？还是伸长？还是回缩？
	int func;						//	此钩子目前带有的功能-与道具挂钩
} HOOK;

//	用户结构体
typedef struct _user {
	char userName[64];
	LEVEL userLevel;

	struct _user* next;
} PLAYER;

//	设置结构体
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
//   主函数们  //
////////////////
void startup(PLAYER* player, MINE** head, ITEMLIST* i_head, MOBHEAD* mobList, struct _error* errorCode, const SETTING* gameSetting);
void show(const PLAYER* thisPlayer, const HOOK* thisHook, const MINE* head, const ITEMLIST* i_head, const MOBHEAD* mobList, struct _error* errorCode, const SETTING* gameSetting);
void updateWithoutInput(PLAYER* player, HOOK* thisHook, MINE* m_head, ITEMLIST* i_head, MOBHEAD* mobList, struct _error* errorCode, SETTING* gameSetting);
void updateWithInput(PLAYER*, MINE * m_head, HOOK*, ITEMLIST* i_head, struct _error*, SETTING * gameSetting);
