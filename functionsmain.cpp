#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>
#include <stdlib.h>
#include "functionsmain.h"
#include "bookmarks.h"

#define HIGH 505
#define WIDTH 960
#define WIDER_WIDTH 1160
#define PI 3.1415926
#define GRASS_HIGHT 91
#define CROK_LONG_STD 50
#define INFO_BAR_X 990
#define STD_TIME_RATE 20
#define ONE_SECOND 1000

int calculatePassScore(MINE* head);
void oresGenerate(MINE* mine, int* numOresWithoutStone);
void oreDraw(MINE* head);
void printLineText(LPCTSTR font, COLORREF colour, const int strLong, LPCTSTR str, const int data, int textY);
int createSmallButton(int x, int y, IMAGE* pic, IMAGE* pic_s, LPCTSTR str, COLORREF colour);
void printRankingList(e_errorCode* errorCode);
void pauseGame(void);
void SaveGame(PLAYER* player, LEVEL* thisLevel, e_errorCode* errorCode);

extern struct _node;
extern struct _user;

MINE* startup(MINE* head, LEVEL* currentLevel, PLAYER* player, e_errorCode* errorCode)
{
	closegraph();	//	预留接口
	initgraph(WIDER_WIDTH, HIGH);

	IMAGE bookmark3_bg;
	loadimage(&bookmark3_bg, _T(".\\textures\\gui\\bookmarks3bg.jpg"));
	putimage(0, 0, &bookmark3_bg);
	setbkmode(TRANSPARENT);

	int numOres = (currentLevel->currentLevel - 1) * 3 + 10;
	int numOresWithoutStone = 0;

	//	创建链表
	for (int i = 0; i < numOres; i++)
	{
		MINE* p = (MINE*)malloc(sizeof(MINE));
		if (p == NULL) {
			*errorCode = lowHeapMemory;
			exit((int)*errorCode);
		}
		else {
			oresGenerate(p, &numOresWithoutStone);
		}
		MINE* last = head;
		if (last) {
			while (last->next) {
				last = last->next;
			}
			last->next = p;
		}
		else {
			head = p;
		}
	}

	//	关卡初始化
	currentLevel->timeLast = numOresWithoutStone * 40 * STD_TIME_RATE;
	currentLevel->gameScore = 0;
	currentLevel->passScore = calculatePassScore(head);

	return head;
}

void show(MINE* head, LEVEL* currentLevel, PLAYER* thisPlayer, e_errorCode* errorCode) {
	IMAGE bookmark3_bg;
	loadimage(&bookmark3_bg, _T(".\\textures\\gui\\bookmarks3bg.jpg"));
	putimage(0, 0, &bookmark3_bg);
	setbkmode(TRANSPARENT);

	//	显示所有的矿物
	oreDraw(head);

	//	右侧信息栏
	//	每行与每行之间相差25px
	settextstyle(20, 0, _T("楷体"));
	settextcolor(BLACK);
	printLineText(_T("楷体"), BLACK, 20, _T("得分:"), (currentLevel->gameScore), 29);
	printLineText(_T("楷体"), BLACK, 20, _T("需求分:"), (currentLevel->passScore), 54);
	printLineText(_T("楷体"), BLACK, 20, _T("关卡:"), (currentLevel->currentLevel), 79);
	printLineText(_T("楷体"), BLACK, 20, _T("时间:"), (currentLevel->timeLast) / STD_TIME_RATE, 104);
	outtextxy(INFO_BAR_X, 149, _T("----排行榜----"));
	printRankingList(errorCode);				//	加载排行榜
}

void updateWithoutImput(MINE* head, LEVEL* thisLevel, CROK* thisCrok, LEVEL_N* l_head, PLAYER* player, e_errorCode* errorCode) {
	int startPointCrok_x = (*thisCrok).startPointCrok_X;
	int startPointCrok_y = (*thisCrok).startPointCrok_Y;

	setcolor(BLACK);
	setfillcolor(BLACK);
	fillcircle(startPointCrok_x, startPointCrok_y, 3);

	//	钩子的计算公式
	(thisCrok->endPointX) = startPointCrok_x + (thisCrok->crokLong) * cos(thisCrok->angle);
	(thisCrok->endPointY) = startPointCrok_y + (thisCrok->crokLong) * sin(thisCrok->angle);

	//	调节速度
	int sleepTime;
	if (thisCrok->crokNowState == hang) {
		sleepTime = (100 - (thisCrok->endPointY));
	}
	else {
		sleepTime = 1;
	}

	//	碰撞
	for (MINE* i = head; i; i = i->next)
	{
		if (((thisCrok->endPointX) >= i->Pos_x) && ((thisCrok->endPointX) <= (i->Pos_x) + (i->size)) && ((thisCrok->endPointY) >= i->Pos_y) && ((thisCrok->endPointY) <= (i->Pos_y) + (i->size)) && (i->state == available)) {
			i->Pos_x = (thisCrok->endPointX) - (i->size) / 2;
			i->Pos_y = (thisCrok->endPointY) - (i->size) / 2;
			thisCrok->crokConductance = pow(3 - (i->size) / 32, 3);
			thisCrok->crokNowState = back;
		}
		if ((i->state == available) && (i->Pos_y < GRASS_HIGHT)) {
			i->state = croked;
			(thisLevel->gameScore) += (int)(i->kind) * (i->size) / 32;
		}
	}

	//	碰到墙壁
	if ((((thisCrok->endPointY) >= HIGH) || ((thisCrok->endPointX) <= 0) || ((thisCrok->endPointX) >= WIDTH))) {
		thisCrok->crokNowState = back;
		thisCrok->crokConductance = 10;
	}

	//	回到hang状态
	if ((thisCrok->crokNowState == back) && ((thisCrok->endPointY) <= GRASS_HIGHT) && ((thisCrok->crokLong) < CROK_LONG_STD)) {
		(thisCrok->crokLong) = CROK_LONG_STD;
		thisCrok->crokNowState = hang;
	}

	//	划线
	switch (thisCrok->crokNowState) {
	case hang:
		if (thisCrok->rollTemp) {
			if ((thisCrok->endPointY) >= startPointCrok_y) {
				(thisCrok->angle) = (thisCrok->angle) + PI / 180;
			}
			else {
				(thisCrok->angle) = (thisCrok->angle) - PI / 60;
				thisCrok->rollTemp = right;
			}
		}
		else {
			if ((thisCrok->endPointY) >= startPointCrok_y) {
				(thisCrok->angle) = (thisCrok->angle) - PI / 180;
			}
			else {
				(thisCrok->angle) = (thisCrok->angle) + PI / 60;
				thisCrok->rollTemp = left;
			}
		}
		break;
	case extension:
		(thisCrok->crokLong) = (thisCrok->crokLong) + 10;
		break;
	case back:
		(thisCrok->crokLong) -= (thisCrok->crokConductance);
		break;
	}
	setlinestyle(PS_SOLID, 3);
	setlinecolor(BLACK);
	line(startPointCrok_x, startPointCrok_y, (thisCrok->endPointX), (thisCrok->endPointY));

	//	倒计时
	(thisLevel->timeLast)--;

	//	时间归零，游戏失败
	if (thisLevel->timeLast <= 0) {
		SaveGame(player, thisLevel, errorCode);
		gameOver(l_head);
	}

	//	分数达到标准，过关
	int isComplete = 0;
	if (thisLevel->gameScore >= thisLevel->passScore) {
		//	增加关卡
		LEVEL_N* p = (LEVEL_N*)malloc(sizeof(LEVEL_N));
		if (p == NULL) {
			*errorCode = lowHeapMemory;
			exit((int)*errorCode);
		}
		LEVEL_N* last = l_head;
		if (last) {
			while (last->next) {
				last = last->next;
			}
			last->next = p;
			(p->currentlevel).currentLevel = (last->currentlevel).currentLevel + 1;
			p->next = NULL;
		}
		else {
			l_head = p;
		}

		int return_ = thisLevel->currentLevel + 1;
		bookmarkMain(&return_, l_head, player, errorCode);
	}
	return;
}

void updateWithImput(CROK* thisCrok, PLAYER* player, LEVEL* thisLevel, e_errorCode* errorCode) {
	char chImput;
	if (_kbhit()) {
		chImput = _getch();
		if (thisCrok->crokNowState == hang) {
			switch (chImput) {
			case ' ':
				thisCrok->crokNowState = extension;
				break;
			case 'a':
				(*thisCrok).startPointCrok_X = (*thisCrok).startPointCrok_X - 2;
				break;
			case 'A':
				(*thisCrok).startPointCrok_X = (*thisCrok).startPointCrok_X - 2;
				break;
			case 'd':
				(*thisCrok).startPointCrok_X = (*thisCrok).startPointCrok_X + 2;
				break;
			case 'D':
				(*thisCrok).startPointCrok_X = (*thisCrok).startPointCrok_X + 2;
				break;
			}
		}
		switch (chImput) {
		case 'p':
			pauseGame();
			break;
		case 'P':
			pauseGame();
			break;
		case 'Q':
			exit(0);
			break;
		case 'q':
			exit(0);
			break;
		case 's':
			SaveGame(player, thisLevel, errorCode);
			break;
		case 'S':
			SaveGame(player, thisLevel, errorCode);
			break;
		}
	}

	IMAGE smallButton;
	IMAGE smallButton_s;
	loadimage(&smallButton, _T(".\\textures\\gui\\smallets.png"));
	loadimage(&smallButton_s, _T(".\\textures\\gui\\smallets_s.png"));
	if (createSmallButton(980, 370, &smallButton, &smallButton_s, _T("按P键暂停"), BLACK)) {
		pauseGame();
	}
	if (createSmallButton(980, 416, &smallButton, &smallButton_s, _T("按S键存档"), BLACK)) {
		SaveGame(player, thisLevel, errorCode);
	}
	if (createSmallButton(980, 462, &smallButton, &smallButton_s, _T("按Q键退出"), BLACK)) {
		SaveGame(player, thisLevel, errorCode);
		*errorCode = exitByPlayer;
		exit((int)*errorCode);
	}
}


//	以下为函数中用到的函数
/////////////////
//     函数    ///
/////////////////

//	矿物相关函数
void oresGenerate(MINE* mine, int* numOresWithoutStone) {
	const int IRON_GENERATE_HIGHT = HIGH * 1 / 2;
	const int GOLD_GENERATE_HIGHT = HIGH * 2 / 3;
	const int DIAMOND_GENERATE_HIGHT = HIGH * 3 / 4;
	const int EMERALD_GENERATE_HIGHT = HIGH * 4 / 5;

	int j = rand() % 100;
	if (j >= 66) {
		//mine->size = 96;	//	太大了，看着难受
		mine->size = 64;
	}
	/*else if (j >= 33) {
		mine->size = 64;
	}*/
	else {
		mine->size = 32;
	}


	mine->state = available;
	mine->Pos_x = rand() % (WIDTH - (mine->size));

	int i = rand() % 100;
	if (i >= 95) {
		mine->kind = emerald;
		mine->Pos_y = rand() % (HIGH - EMERALD_GENERATE_HIGHT - (mine->size)) + EMERALD_GENERATE_HIGHT;
	}
	else if (i >= 80) {
		mine->kind = diamond;
		mine->Pos_y = rand() % (HIGH - DIAMOND_GENERATE_HIGHT - (mine->size)) + DIAMOND_GENERATE_HIGHT;
	}
	else if (i >= 70) {
		mine->kind = gold;
		mine->Pos_y = rand() % (HIGH - GOLD_GENERATE_HIGHT - (mine->size)) + GOLD_GENERATE_HIGHT;
	}
	else if (i >= 50) {
		mine->kind = iron;
		mine->Pos_y = rand() % (HIGH - IRON_GENERATE_HIGHT - (mine->size)) + IRON_GENERATE_HIGHT;
	}
	else {
		mine->kind = stone;
		mine->Pos_y = rand() % (HIGH - GRASS_HIGHT - (mine->size)) + GRASS_HIGHT;
	}
	if (mine->kind != stone)
		(*numOresWithoutStone)++;

	mine->next = NULL;
}
void oreDraw(MINE* head) {
	IMAGE stone_pic[3];
	loadimage(&stone_pic[0], _T(".\\textures\\mine\\stone_small.png"));
	loadimage(&stone_pic[1], _T(".\\textures\\mine\\stone_middle.png"));
	loadimage(&stone_pic[2], _T(".\\textures\\mine\\stone_big.png"));
	IMAGE iron_pic[3];
	loadimage(&iron_pic[0], _T(".\\textures\\mine\\iron_ore_small.png"));
	loadimage(&iron_pic[1], _T(".\\textures\\mine\\iron_ore_middle.png"));
	loadimage(&iron_pic[2], _T(".\\textures\\mine\\iron_ore_big.png"));
	IMAGE gold_pic[3];
	loadimage(&gold_pic[0], _T(".\\textures\\mine\\gold_ore_small.png"));
	loadimage(&gold_pic[1], _T(".\\textures\\mine\\gold_ore_middle.png"));
	loadimage(&gold_pic[2], _T(".\\textures\\mine\\gold_ore_big.png"));
	IMAGE diamond_pic[3];
	loadimage(&diamond_pic[0], _T(".\\textures\\mine\\diamond_ore_small.png"));
	loadimage(&diamond_pic[1], _T(".\\textures\\mine\\diamond_ore_middle.png"));
	loadimage(&diamond_pic[2], _T(".\\textures\\mine\\diamond_ore_big.png"));
	IMAGE emerald_pic[3];
	loadimage(&emerald_pic[0], _T(".\\textures\\mine\\emerald_ore_small.png"));
	loadimage(&emerald_pic[1], _T(".\\textures\\mine\\emerald_ore_middle.png"));
	loadimage(&emerald_pic[2], _T(".\\textures\\mine\\emerald_ore_big.png"));

	for (MINE* j = head; j; j = j->next)
	{
		int index = (j->size) / 32 - 1;
		if (j->state == available) {
			switch (j->kind)
			{
			case stone:
				Resize(&stone_pic[index], j->size, j->size);
				putimage(j->Pos_x, j->Pos_y, &stone_pic[index]);
				break;
			case iron:
				Resize(&iron_pic[index], j->size, j->size);
				putimage(j->Pos_x, j->Pos_y, &iron_pic[index]);
				break;
			case gold:
				Resize(&gold_pic[index], j->size, j->size);
				putimage(j->Pos_x, j->Pos_y, &gold_pic[index]);
				break;
			case diamond:
				Resize(&diamond_pic[index], j->size, j->size);
				putimage(j->Pos_x, j->Pos_y, &diamond_pic[index]);
				break;
			case emerald:
				Resize(&emerald_pic[index], j->size, j->size);
				putimage(j->Pos_x, j->Pos_y, &emerald_pic[index]);
				break;
			default:
				break;
			}
		}
	}
}
int calculatePassScore(MINE* head) {
	int sum = 0;
	for (MINE* i = head; i; i = i->next)
	{
		if (i->kind != stone) {
			sum += (int)(i->kind) * (i->size) / 32;
		}
	}
	sum = (int)(sum * 0.8);

	return sum;
}

//	右侧边栏的内容
void printLineText(LPCTSTR font, COLORREF colour, const int strLong, LPCTSTR str, const int data, int textY) {
	settextstyle(20, 0, font);
	settextcolor(colour);
	char strbuff[20];
	outtextxy(INFO_BAR_X, textY, str);
	sprintf_s(strbuff, "%d", data);
	outtextxy(INFO_BAR_X + 80, textY, _T(strbuff));
}
void printRankingList(e_errorCode* errorCode) {
	settextstyle(20, 0, _T("宋体"));
	const int TEXT_HIGH_START = 174;
	PLAYER playerBuff[5];

	//	初始化
	for (int i = 0; i < 5; i++)
	{
		playerBuff[i].userLevel.currentLevel = 0;
		playerBuff[i].userLevel.gameScore = 0;
		playerBuff[i].userLevel.passScore = 0;
		playerBuff[i].userLevel.timeLast = 0;
	}

	//	文件操作
	FILE* fp = NULL;
	fopen_s(&fp, "userData.hjq", "r");
	if (fp) {
		//	独取数据
		fscanf_s(
			fp,
			"%d %d %d %d\n%d %d %d %d\n%d %d %d %d\n%d %d %d %d\n%d %d %d %d\n",
			&(playerBuff[0].userLevel.currentLevel), &(playerBuff[0].userLevel.gameScore), &(playerBuff[0].userLevel.passScore), &(playerBuff[0].userLevel.timeLast),
			&(playerBuff[1].userLevel.currentLevel), &(playerBuff[1].userLevel.gameScore), &(playerBuff[1].userLevel.passScore), &(playerBuff[1].userLevel.timeLast),
			&(playerBuff[2].userLevel.currentLevel), &(playerBuff[2].userLevel.gameScore), &(playerBuff[2].userLevel.passScore), &(playerBuff[2].userLevel.timeLast),
			&(playerBuff[3].userLevel.currentLevel), &(playerBuff[3].userLevel.gameScore), &(playerBuff[3].userLevel.passScore), &(playerBuff[3].userLevel.timeLast),
			&(playerBuff[4].userLevel.currentLevel), &(playerBuff[4].userLevel.gameScore), &(playerBuff[4].userLevel.passScore), &(playerBuff[4].userLevel.timeLast)
			);

		fclose(fp);
	}
	else {
		*errorCode = fileOpenFailed;
		exit((int)*errorCode);
	}

	//	排序
	for (int i = 0; i < 5; i++)
	{
		int maxIndex = i;
		for (int j = i + 1; j < 5; j++)
		{
			if (playerBuff[j].userLevel.currentLevel > playerBuff[maxIndex].userLevel.currentLevel) {
				maxIndex = j;
			}
			else if (playerBuff[j].userLevel.currentLevel == playerBuff[maxIndex].userLevel.currentLevel) {
				if (playerBuff[j].userLevel.gameScore > playerBuff[maxIndex].userLevel.gameScore) {
					maxIndex = j;
				}
			}
		}
		PLAYER temp;
		temp = playerBuff[maxIndex];
		playerBuff[maxIndex] = playerBuff[i];
		playerBuff[i] = temp;
	}

	//	print在屏幕上
	for (int i = 0; i < 5; i++) {
		//	第i名：$VAL$关, 该关得分$VAL$/$VAL$
		char levelBuff[6];
		sprintf_s(levelBuff, "%d", playerBuff[i].userLevel.currentLevel);
		outtextxy(INFO_BAR_X, TEXT_HIGH_START + i * 25, levelBuff);
		outtextxy(INFO_BAR_X + 10, TEXT_HIGH_START + i * 25, _T("关,得分["));

		char scoreBuff[10];
		sprintf_s(scoreBuff, "%d/%d]", playerBuff[i].userLevel.gameScore, playerBuff[i].userLevel.passScore);
		outtextxy(INFO_BAR_X + 10 + 80, TEXT_HIGH_START + i * 25, scoreBuff);
	}
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
void pauseGame(void) {
	while (1) {
		char ch;
		if (_kbhit()) {
			ch = _getch();
			if ((ch == 'P') || (ch == 'p')) {
				break;
			}
		}
		else {
			const int TEXT_OUT_X = 1000;
			settextcolor(YELLOW);
			outtextxy(TEXT_OUT_X, 440, _T("游戏已暂停"));
			outtextxy(TEXT_OUT_X, 460, _T("按P键解除"));
		}
	}
}
void SaveGame(PLAYER* player, LEVEL* thisLevel, e_errorCode* errorCode) {
	player->userLevel = *thisLevel;
	FILE* fp = NULL;
	fopen_s(&fp, "userData.hjq", "a");
	if (fp) {
		//fprintf_s(fp, "%d %d %d %d %s\n", player->userLevel.currentLevel, player->userLevel.gameScore, player->userLevel.passScore, player->userLevel.timeLast, player->userName);
		fprintf_s(fp, "%d %d %d %d\n", player->userLevel.currentLevel, player->userLevel.gameScore, player->userLevel.passScore, player->userLevel.timeLast);
		fclose(fp);
	}
	else {
		*errorCode = fileOpenFailed;
		exit((int)*errorCode);
	}
}