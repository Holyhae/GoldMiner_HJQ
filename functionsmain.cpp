#include <conio.h>
#include <graphics.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "functionsmain.h"

#define HIGH 505
#define WIDTH 960
#define WIDER_WIDTH 1160
#define PI 3.1415926
#define GRASS_HIGHT 91
#define CROK_LONG_STD 50
#define INFO_BAR_X 990
#define STD_TIME_RATE 20

void printLineText(LPCTSTR font, COLORREF colour, const int strLong, LPCTSTR str, const int data, int textY);
int createSmallButton(int x, int y, IMAGE* pic, IMAGE* pic_s, LPCTSTR str, COLORREF colour);

extern struct _node;

MINE* startup(MINE* head, LEVEL* currentLevel)
{
	closegraph();
	initgraph(WIDER_WIDTH, HIGH);

	IMAGE bookmark3_bg;
	loadimage(&bookmark3_bg, _T(".\\textures\\gui\\bookmarks3bg.jpg"));
	putimage(0, 0, &bookmark3_bg);
	setbkmode(TRANSPARENT);

	currentLevel->currentLevel = 5;
	currentLevel->timeLast = 3 * 60 * STD_TIME_RATE;

	//	创建链表
	for (int i = 0; i < 2* (currentLevel->currentLevel) +1; i++)
	{
		MINE* p = (MINE*)malloc(sizeof(MINE));
		p->kind = gold;
		p->state = available;
		p->Pos_x = rand() % (WIDTH - 32);
		p->Pos_y = rand() % (HIGH - GRASS_HIGHT - 32) + GRASS_HIGHT;
		p->next = NULL;
		MINE* last = head;
		if (last) {
			while (last->next) {
				last = last->next;
			}
			last->next = p;
		} else {
			head = p;
		}
	}

	return head;
}

void show(MINE* head, LEVEL* currentLevel) {
	IMAGE bookmark3_bg;
	loadimage(&bookmark3_bg, _T(".\\textures\\gui\\bookmarks3bg.jpg"));
	putimage(0, 0, &bookmark3_bg);
	setbkmode(TRANSPARENT);

	//	显示所有的矿物
	IMAGE gold_pic;
	loadimage(&gold_pic, _T(".\\textures\\mine\\gold_ore.png"));
	for (MINE* j = head; j->next; j = j->next)
	{
		if (j->state == available) {
			putimage(j->Pos_x, j->Pos_y, &gold_pic);
		}
	}

	//	右侧信息栏
	//	每行与每行之间相差25px
	settextstyle(20, 0, _T("楷体"));
	settextcolor(BLACK);
	printLineText(_T("楷体"), BLACK, 20, _T("得分:"), (currentLevel->gameScore), 29);
	printLineText(_T("楷体"), BLACK, 20, _T("关卡:"), (currentLevel->currentLevel), 54);
	printLineText(_T("楷体"), BLACK, 20, _T("时间:"), (currentLevel->timeLast)/ STD_TIME_RATE, 79);
}

void updateWithoutImput(MINE* head, LEVEL* thisLevel, CROK* thisCrok) {
	const int START_POINT_CROK_X = 475;
	const int START_POINT_CROK_Y = 50;
	
	setcolor(BLACK);
	setfillcolor(BLACK);
	fillcircle(START_POINT_CROK_X, START_POINT_CROK_Y, 3);

	//	钩子的计算公式
	(thisCrok->endPointX) = START_POINT_CROK_X + (thisCrok->crokLong) * cos(thisCrok->angle);
	(thisCrok->endPointY) = START_POINT_CROK_Y + (thisCrok->crokLong) * sin(thisCrok->angle);

	//	调节速度
	int sleepTime;
	if (thisCrok->crokNowState == hang) {
		sleepTime = (100 - (thisCrok->endPointY));
	}
	else {
		sleepTime = 1;
	}
	
	//	碰撞
	for (MINE* i = head; i->next; i = i->next)
	{
		if (((thisCrok->endPointX) >= i->Pos_x) && ((thisCrok->endPointX) <= (i->Pos_x) + 32) && ((thisCrok->endPointY) >= i->Pos_y) && ((thisCrok->endPointY) <= (i->Pos_y) + 32) && (i->state == available)) {
			i->Pos_x = (thisCrok->endPointX) - 16;
			i->Pos_y = (thisCrok->endPointY) - 16;
			thisCrok->crokConductance = 2;
			thisCrok->crokNowState = back;
		}
		if ((i ->state == available) && (i->Pos_y < GRASS_HIGHT)) {
			i->state = croked;
			(thisLevel->gameScore) += 5;
		}
	}

	//	碰到墙壁
	if ((((thisCrok->endPointY) >= HIGH) || ((thisCrok->endPointX) <= 0) || ((thisCrok->endPointX) >= WIDTH))) {
		thisCrok->crokNowState = back;
		thisCrok->crokConductance = 5;
	}

	//	回到hang状态
	//if ((crok == back) && (crokLong <= 50)) {
	if ((thisCrok->crokNowState == back) && ((thisCrok->endPointY) <= GRASS_HIGHT) && ((thisCrok->crokLong) < CROK_LONG_STD)) {
		(thisCrok->crokLong) = CROK_LONG_STD;
		thisCrok->crokNowState = hang;
	}

	//	划线
	switch (thisCrok->crokNowState) {
	case hang:
		if (thisCrok->rollTemp) {
			if ((thisCrok->endPointY) >= START_POINT_CROK_Y) {
				(thisCrok->angle) = (thisCrok->angle) + PI / 180;
			}
			else {
				(thisCrok->angle) = (thisCrok->angle) - PI / 60;
				thisCrok->rollTemp = right;
			}
		}
		else { 
			if ((thisCrok->endPointY) >= START_POINT_CROK_Y) {
				(thisCrok->angle) = (thisCrok->angle) - PI / 180;
			}
			else {
				(thisCrok->angle) = (thisCrok->angle) + PI / 60;
				thisCrok->rollTemp = left;
			}
		}
		break;
	case extension:
		(thisCrok->crokLong) = (thisCrok->crokLong) + 5;
		break;
	case back:
		(thisCrok->crokLong) -= (thisCrok->crokConductance);
		break;
	}
	setlinestyle(PS_SOLID, 3);
	setlinecolor(BLACK);
	line(START_POINT_CROK_X, START_POINT_CROK_Y, (thisCrok->endPointX), (thisCrok->endPointY));
	
	(thisLevel->timeLast)--;

	//	时间归零，游戏失败
	if (thisLevel->timeLast <= 0) {
		//	IO预留
	}
	
	//	分数达到标准，过关
	if (thisLevel->gameScore >= thisLevel->passScore) {
		//	IO预留
	}
}

void updateWithImput(CROK* thisCrok) {
	char chImput;
	if (_kbhit()) {
		chImput = _getch();
		if ((thisCrok->crokNowState == hang) && (chImput == ' ')) {
			thisCrok->crokNowState = extension;
		}
	}

	IMAGE smallButton;
	IMAGE smallButton_s;
	loadimage(&smallButton, _T(".\\textures\\gui\\smallets.png"));
	loadimage(&smallButton_s, _T(".\\textures\\gui\\smallets_s.png"));
	if (createSmallButton(980, 380, &smallButton, &smallButton_s, _T("暂停游戏"), BLACK)) {
		//	IO预留
	}
	if (createSmallButton(980, 440, &smallButton, &smallButton_s, _T("储存游戏"), BLACK)) {
		//	IO预留
	}
}

//	以下为函数中用到的函数

//	右侧边栏的内容
void printLineText(LPCTSTR font, COLORREF colour, const int strLong, LPCTSTR str, const int data, int textY) {
	settextstyle(20, 0, font);
	settextcolor(colour);
	char strbuff[20] ;
	outtextxy(INFO_BAR_X, textY, str);
	sprintf_s(strbuff, "%d", data);
	outtextxy(INFO_BAR_X + 60, textY, strbuff);
}

//	生成小按钮
int createSmallButton(int x, int y, IMAGE* pic, IMAGE* pic_s, LPCTSTR str, COLORREF colour) {
	putimage(x, y, pic);
	outtextxy(x, y + 7, str);
	int inside_return = 0;
	if (MouseHit()) {
		MOUSEMSG m = GetMouseMsg();
		if ((m.mkLButton == 1) && (m.x > x) && (m.x < 160 + x) && (m.y > y) && (m.y < y + 36)) {
			inside_return = 1;
			putimage(x, y, pic_s);
			settextcolor(colour);
			outtextxy(x, y + 7, str);
		}
		else {
			inside_return = 0;
		}
	}

	return inside_return;
}