#include <conio.h>
#include <graphics.h>
#include "functionsmain.h"

extern struct _node;
extern struct _level;
extern struct _crok;

int main(void) {

	/////////////////
	//   伪全局变量  //
	/////////////////
	//	关卡相关
	LEVEL thisLevel;
	thisLevel.currentLevel = 5;
	thisLevel.gameScore = 0;

	//	矿物相关
	MINE* head = NULL;

	//	钩子相关
	CROK thisCrok;
	thisCrok.angle = 1;
	thisCrok.crokConductance = 0;
	thisCrok.crokLong = 50;
	thisCrok.rollTemp = left;
	thisCrok.crokNowState = hang;

	thisCrok.startPointCrok_X = 475;	//	预留接口
	thisCrok.startPointCrok_Y = 50;		//	预留接口

	///////////////
	//  游戏函数  //
	///////////////
	head = startup(head, &thisLevel);

	while (1) {
		show(head, &thisLevel);
		updateWithoutImput(head, &thisLevel, &thisCrok);
		updateWithImput(&thisCrok);

		FlushBatchDraw();
		Sleep(10);
	}

	return 0;
}