#include <conio.h>
#include <graphics.h>
#include "functionsmain.h"

extern struct _node;
extern struct _level;
extern struct _crok;

int main(void) {

	/////////////////
	//   αȫ�ֱ���  //
	/////////////////
	//	�ؿ����
	LEVEL thisLevel;
	thisLevel.currentLevel = 5;
	thisLevel.gameScore = 0;

	//	�������
	MINE* head = NULL;

	//	�������
	CROK thisCrok;
	thisCrok.angle = 1;
	thisCrok.crokConductance = 0;
	thisCrok.crokLong = 50;
	thisCrok.rollTemp = left;
	thisCrok.crokNowState = hang;

	thisCrok.startPointCrok_X = 475;	//	Ԥ���ӿ�
	thisCrok.startPointCrok_Y = 50;		//	Ԥ���ӿ�

	///////////////
	//  ��Ϸ����  //
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