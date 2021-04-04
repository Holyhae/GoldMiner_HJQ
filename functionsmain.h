#pragma once

////////////////
//   结构体们  //
////////////////

//	矿物结构体
enum mineKind { stone, gold, diamond };
enum mineState { available, croked };
typedef struct _node {
	enum mineKind kind;				//	kind表示就矿物类型
	enum mineState state;			//	state表示状态
	int Pos_x;	int Pos_y;			//	xy是横纵坐标
	struct _node* next;
} MINE;

//	关卡结构体
typedef struct _level {
	int currentLevel;				//	当前关卡
	int gameScore;					//	当前得分
	int timeLast;					//	剩余时间
	int passScore;					//	通关所需要的得分
} LEVEL ;

//	钩子结构体
enum rollTempKind { right, left };
enum crokState { hang, extension, back };
typedef struct _crok {
	int crokLong;					//	当前钩子长
	float angle;					//	当前角度
	int endPointX;					//	结束点横坐标
	int endPointY;					//	结束点纵坐标
	rollTempKind rollTemp;			//	向左摆还是向右摆？
	int crokConductance;			//	“钩导”，即钩子钩住东西之后的运行速度
	crokState crokNowState;			//	当前钩子是在旋转？还是伸长？还是回缩？

	int startPointCrok_X;			//	预留接口
	int startPointCrok_Y;			//	预留接口
} CROK ;


////////////////
//   主函数们  //
////////////////
MINE* startup(MINE* head, LEVEL*);

void show(MINE*, LEVEL*);

void updateWithoutImput(MINE*, LEVEL*, CROK*);

void updateWithImput(CROK*);