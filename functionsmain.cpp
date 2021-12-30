#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>
#include <stdlib.h>
#include "functionsmain.h"
#include "bookmarks.h"
#include "item.h"
#include "mob.h"
#include "gui.h"
#pragma comment(lib, "WINMM.LIB")

#define HIGH 505
#define WIDTH 960
#define WIDER_WIDTH 1160
#define PI 3.1415926
#define GRASS_HIGHT 91
#define HOOK_LONG_STD 30
#define INFO_BAR_X 990
#define STD_TIME_RATE 20
#define ONE_SECOND 1000

int calculatePassScore(const MINE * head);
void oresGenerate(MINE* mine, int* numOresWithoutStone);
void showOre(const MINE * head, const SETTING* gameSetting);
void printLineText(LPCTSTR font, COLORREF colour, const int strLong, LPCTSTR str, const int data, const int textY);
void printRankingList(ERR* errorCode, const SETTING* gameSet);
void printHookFunc(const HOOK * thisHook);
void pauseGame(const SETTING* gameSet);
void SaveGame(const PLAYER* player, ERR* errorCode);
void gameOver(PLAYER* thisPlayer, MINE* m_head, ITEMLIST* i_head, ERR* errorCode, SETTING * gameSet);

void startup(PLAYER* player, MINE** head, ITEMLIST* i_head, MOBHEAD* mobList, ERR* errorCode, const SETTING* gameSetting)
{
	closegraph();	//	Ԥ���ӿ�
	if (gameSetting->debugMode) {
		initgraph(WIDER_WIDTH, HIGH, SHOWCONSOLE);
	}
	else {
		initgraph(WIDER_WIDTH, HIGH);
	}
	
	BeginBatchDraw();

	//	һЩ��ʼ��
	int numOres = (player->userLevel.currentLevel - 1) * 3 + 10;
	int numOresWithoutStone = 0;

	//	������������
	for (int i = 0; i < numOres; i++)
	{
		MINE* p = (MINE*)malloc(sizeof(MINE));
		if (p == NULL) {
			errorCode->code = lowHeapMemory;
			errorCode->info = _T("������������malloc�ռ�ʧ��");
			printErrToFile(errorCode);
		}
		else {
			oresGenerate(p, &numOresWithoutStone);
		}
		MINE* last = *head;
		if (last) {
			while (last->next) {
				last = last->next;
			}
			last->next = p;
		}
		else {
			*head = p;
		}
	}

	//	��������
	if (gameSetting->putItems) {
		load_item(i_head, errorCode, gameSetting);
	}

	//	��������
	if (gameSetting->doMobSpawn) {
		load_mob(mobList, errorCode, gameSetting);
	}

	//	�ؿ���ʼ��
	player->userLevel.timeLast = numOresWithoutStone * 40 * STD_TIME_RATE;
	player->userLevel.gameScore = 0;
	player->userLevel.passScore = calculatePassScore(*head);

	//	��������
	mciSendString("open \".\\music\\Sweden.mp3\" alias bkmusic", NULL, 0, NULL);
	mciSendString("play bkmusic repeat", NULL, 0, NULL);

	//	�����ļ�
	{
		FILE* f_err = NULL;
		if (fopen_s(&f_err, "err.log", "a") == 0) {
			fprintf_s(f_err, "\n[��Ϣ]\t��ǰ�ؿ�:%d", player->userLevel.currentLevel);
			fprintf_s(f_err, "\t��Ϸ����:%d", player->userLevel.gameScore);
			fprintf_s(f_err, "\tͨ���������:%d", player->userLevel.passScore);
			fprintf_s(f_err, "\t����ʱ:%d", player->userLevel.timeLast);
			fprintf_s(f_err, "\n");
			fclose(f_err);
		}
		else {
			errorCode->code = fileOpenFailed;
			errorCode->info = _T("�ؿ���Ϣ�����ļ�ʧ��");
			printErrToFile(errorCode);
		}
	}
}

void show(const PLAYER * thisPlayer, const HOOK * thisHook, const MINE * head, const ITEMLIST* i_head, const MOBHEAD* mobList, ERR* errorCode, const SETTING* gameSetting) {
	char picURLtemp[64];
	cleardevice();
	
	IMAGE bookmark3_bg;
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\bookmarks3bg.jpg", gameSetting->resourcepacksName);
	loadimage(&bookmark3_bg, _T(picURLtemp));
	putimage(0, 0, &bookmark3_bg);
	setbkmode(TRANSPARENT);

	//	��ʾ���е���ϷԪ��
	showOre(head, gameSetting);
	if (gameSetting->putItems)
	{
		showItem(i_head, errorCode);
	}
	if (gameSetting->doMobSpawn) {
		show_mob(mobList, errorCode);
	}

	//	�������ߺ͹���ͷ
	setlinestyle(PS_SOLID, 5);
	//setlinecolor(RGB(127, 127, 127));
	setlinecolor(BLACK);
	line(thisHook->startPointHook_X, thisHook->startPointHook_Y, (thisHook->endPointX), (thisHook->endPointY));
	IMAGE pic_hook;
	IMAGE pic_hook_mask;
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\hook.bmp", gameSetting->resourcepacksName);
	loadimage(&pic_hook, _T(picURLtemp));
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\hook_mask.bmp", gameSetting->resourcepacksName);
	loadimage(&pic_hook_mask, _T(picURLtemp));

	//rotateimage(&pic_hook, &pic_hook, PI / 2 -thisHook->angle, 0UL, true, true);
	//rotateimage(&pic_hook_mask, &pic_hook_mask, PI / 2 - thisHook->angle, 0UL, true, true);

	putimage(thisHook->endPointX - 18, thisHook->endPointY - 2, &pic_hook_mask, NOTSRCERASE);
	putimage(thisHook->endPointX - 18, thisHook->endPointY - 2, &pic_hook, SRCINVERT);
	

	//	���Ͻǵ�����ʾ
	printHookFunc(thisHook);

	//	�Ҳ���Ϣ��
	//	ÿ����ÿ��֮�����25px
	{
		settextstyle(20, 0, _T("����"));
		settextcolor(BLACK);
		outtextxy(INFO_BAR_X, 22, _T("�����:"));
		outtextxy(INFO_BAR_X + 80, 22, _T(thisPlayer->userName));
	}
	printLineText(_T("����"), BLACK, 20, _T("�÷�:"), (thisPlayer->userLevel.gameScore), 79);
	printLineText(_T("����"), BLACK, 20, _T("�����:"), (thisPlayer->userLevel.passScore), 104);
	printLineText(_T("����"), BLACK, 20, _T("�ؿ�:"), (thisPlayer->userLevel.currentLevel), 129);
	printLineText(_T("����"), BLACK, 20, _T("ʱ��:"), (thisPlayer->userLevel.timeLast) / STD_TIME_RATE, 154);
	
	//	�������а�
	printRankingList(errorCode, gameSetting);
}

void updateWithoutInput(PLAYER* player, HOOK* thisHook, MINE * m_head, ITEMLIST* i_head, MOBHEAD* mobList, ERR* errorCode, SETTING* gameSetting) {
	
	//	������ɫ
	setcolor(BLACK);
	setfillcolor(BLACK);
	
	//	���ӵļ��㹫ʽ
	(thisHook->endPointX) = (*thisHook).startPointHook_X + (thisHook->hookLong) * cos(thisHook->angle);
	(thisHook->endPointY) = (*thisHook).startPointHook_Y + (thisHook->hookLong) * sin(thisHook->angle);

	//	�������ײ
	for (MINE* i = m_head; i; i = i->next)
	{
		if (((thisHook->endPointX) + 16 >= i->Pos_x) && ((thisHook->endPointX) - 16 <= (i->Pos_x) + (i->size)) && ((thisHook->endPointY) + 10 >= i->Pos_y) && ((thisHook->endPointY) <= (i->Pos_y) + (i->size)) && (i->state == available)) {
			switch (thisHook->func)
			{
			// �˴�Ϊ���������
#include "data/item/func.hjq"

			default:
				i->Pos_x = (thisHook->endPointX) - (i->size) / 2;
				i->Pos_y = thisHook->endPointY - 0;
				thisHook->hookConductance = pow(3.0 - (i->size) / 32, 3);
				thisHook->hookNowState = back;
				break;
			}
		}
		if ((i->state == available) && (i->Pos_y < GRASS_HIGHT)) {
			i->state = hooked;
			(player->userLevel.gameScore) += (int)(i->kind) * (i->size) / 32;
			thisHook->hookConductance = 10;
		}
	}

	//	�������ײ
	for (ITEM* i = i_head->gameItemHead; i; i = i->next) {
		useItem(i_head, i, m_head, thisHook, errorCode, gameSetting);
	}
	switch (thisHook->func)
	{
		//	�˴�Ϊ���������
#include "data/item/when_outground.hjq"

	default:
		break;
	}

	//	��mob��ײ
	//	mob����
	use_mob(player, mobList, m_head, i_head->gameItemHead, errorCode, gameSetting);
	for (MOB* i = mobList->mobHead; i; i = i->next)
	{
		if (
				(thisHook->endPointX + 16 >= i->x) &&
				(thisHook->endPointX - 16 <= i->x + 96) &&
				(thisHook->endPointY + 10 >= i->y) &&
				(thisHook->endPointY <= i->y + 35) &&
				(i->state)
			) {
			i->x = thisHook->endPointX - 48;
			i->y = thisHook->endPointY - 2;
			thisHook->hookConductance = 2;
			thisHook->hookNowState = back;
		}
		if ((i->state) && (i->y < GRASS_HIGHT)) {
			i->state = hooked;
			(player->userLevel.gameScore) += i->data;
			thisHook->hookConductance = 10;
		}
	}

	//	����ǽ��
	if ((((thisHook->endPointY) + 10 >= HIGH) || ((thisHook->endPointX) - 16 <= 0) || ((thisHook->endPointX) + 16 >= WIDTH))) {
		thisHook->hookNowState = back;
		thisHook->func = 0;
		thisHook->hookConductance = 10;
	}

	//	�ص�hang״̬
	if ((thisHook->hookNowState == back) && ((thisHook->endPointY) <= GRASS_HIGHT) && ((thisHook->hookLong) < HOOK_LONG_STD)) {
		(thisHook->hookLong) = HOOK_LONG_STD;
		thisHook->hookNowState = hang;
	}

	//	�ߵļ���
	switch (thisHook->hookNowState) {
	case hang:
		if (thisHook->rollTemp) {
			if ((thisHook->endPointY) >= thisHook->startPointHook_Y) {
				(thisHook->angle) = (thisHook->angle) + PI / 180;
			}
			else {
				(thisHook->angle) = (thisHook->angle) - PI / 60;
				thisHook->rollTemp = right;
			}
		}
		else {
			if ((thisHook->endPointY) >= thisHook->startPointHook_Y) {
				(thisHook->angle) = (thisHook->angle) - PI / 180;
			}
			else {
				(thisHook->angle) = (thisHook->angle) + PI / 60;
				thisHook->rollTemp = left;
			}
		}
		break;
	case extension:
		(thisHook->hookLong) = (thisHook->hookLong) + 10;
		break;
	case back:
		(thisHook->hookLong) -= (thisHook->hookConductance);
		break;
	}

	//	����ʱ
	(player->userLevel.timeLast)--;

	//	ʱ����㣬��Ϸʧ��
	if (player->userLevel.timeLast <= 0) {
		gameOver(player, m_head, i_head, errorCode, gameSetting);
	}

	//	�����ﵽ��׼������
	int isComplete = 0;
	if (player->userLevel.gameScore >= player->userLevel.passScore) {
		//	��ǰ�ؿ���+1
		player->userLevel.currentLevel ++;
		
		//	��һ�أ��˴�Ϊ��������
		if (nextLevelMain(gameSetting)) {
			bookmarkMain(player, errorCode, gameSetting);
		}
		else {
			gameOver(player, m_head, i_head, errorCode, gameSetting);
		}
	}
}

void updateWithInput(PLAYER* player, MINE * m_head, HOOK* thisHook, ITEMLIST* i_head, ERR* errorCode, SETTING* gameSetting) {
	char picURLtemp[64];

	MOUSEMSG m;
	if (MouseHit())
	{
		m = GetMouseMsg();
		FlushMouseMsgBuffer();		//	Based on the CXY's achievement! Thanks.

		//	������
		if ((m.mkRButton == true) && (thisHook->hookNowState == hang)) {
			thisHook->hookNowState = extension;
		}
	}

	char chImput;
	if (_kbhit()) {
		//	���գ���������switch-case����Ϊ��д��һ���������ÿ�ݼ��Ĺ��ܡ�������Ȼ��switch-case����֧�ֳ���
		chImput = _getch();
		if (chImput == gameSetting->KB_enlongHook) {
			if (thisHook->hookNowState == hang) {
				thisHook->hookNowState = extension;
			}
		}
		else if (chImput == gameSetting->KB_pauseGame) {
			pauseGame(gameSetting);
		}
		else if (chImput == gameSetting->KB_quitGame) {
			gameOver(player, m_head, i_head, errorCode, gameSetting);
		}
		else if (chImput == gameSetting->KB_saveGame) {
			SaveGame(player, errorCode);
		}
	}

	IMAGE smallButton;
	IMAGE smallButton_f;
	IMAGE smallButton_s;
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\smallets.png", gameSetting->resourcepacksName);
	loadimage(&smallButton, _T(picURLtemp));
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\smallets_f.png", gameSetting->resourcepacksName);
	loadimage(&smallButton_f, _T(picURLtemp));
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\smallets_s.png", gameSetting->resourcepacksName);
	loadimage(&smallButton_s, _T(picURLtemp));

	char titleBuff[32];
	sprintf_s(titleBuff, "��ͣ��Ϸ(%c)", gameSetting->KB_pauseGame);
	if (createSmallButton(980, 370, &smallButton, &smallButton_f, &smallButton_s, _T(titleBuff), YELLOW, &m)) {
		pauseGame(gameSetting);
	}
	sprintf_s(titleBuff, "������Ϸ(%c)", gameSetting->KB_saveGame);
	if (createSmallButton(980, 416, &smallButton, &smallButton_f, &smallButton_s, _T(titleBuff), YELLOW, &m)) {
		SaveGame(player, errorCode);
	}
	sprintf_s(titleBuff, "�˳���Ϸ(%c)", gameSetting->KB_quitGame);
	if (createSmallButton(980, 462, &smallButton, &smallButton_f, &smallButton_s, _T(titleBuff), YELLOW, &m)) {
		gameOver(player, m_head, i_head, errorCode, gameSetting);
	}
}



/////////////////
//     ����    ///
/////////////////

//	������غ���
void oresGenerate(MINE* mine, int* numOresWithoutStone) {
	const int IRON_GENERATE_HIGHT = HIGH * 1 / 2;
	const int GOLD_GENERATE_HIGHT = HIGH * 2 / 3;
	const int DIAMOND_GENERATE_HIGHT = HIGH * 3 / 4;
	const int EMERALD_GENERATE_HIGHT = HIGH * 4 / 5;

	int j = rand() % 100;
	if (j >= 66) {
		mine->size = 64;
	}
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
void showOre(const MINE * head, const SETTING* gameSetting) {
	char picURLtemp[64];

	IMAGE stone_pic[2];
	sprintf_s(picURLtemp, ".\\textures\\%s\\mine\\stone_small.png", gameSetting->resourcepacksName);
	loadimage(&stone_pic[0], _T(picURLtemp));
	sprintf_s(picURLtemp, ".\\textures\\%s\\mine\\stone_middle.png", gameSetting->resourcepacksName);
	loadimage(&stone_pic[1], _T(picURLtemp));

	IMAGE iron_pic[2];
	sprintf_s(picURLtemp, ".\\textures\\%s\\mine\\iron_ore_small.png", gameSetting->resourcepacksName);
	loadimage(&iron_pic[0], _T(picURLtemp));
	sprintf_s(picURLtemp, ".\\textures\\%s\\mine\\iron_ore_middle.png", gameSetting->resourcepacksName);
	loadimage(&iron_pic[1], _T(picURLtemp));

	IMAGE gold_pic[2];
	sprintf_s(picURLtemp, ".\\textures\\%s\\mine\\gold_ore_small.png", gameSetting->resourcepacksName);
	loadimage(&gold_pic[0], _T(picURLtemp));
	sprintf_s(picURLtemp, ".\\textures\\%s\\mine\\gold_ore_middle.png", gameSetting->resourcepacksName);
	loadimage(&gold_pic[1], _T(picURLtemp));

	IMAGE diamond_pic[2];
	sprintf_s(picURLtemp, ".\\textures\\%s\\mine\\diamond_ore_small.png", gameSetting->resourcepacksName);
	loadimage(&diamond_pic[0], _T(picURLtemp));
	sprintf_s(picURLtemp, ".\\textures\\%s\\mine\\diamond_ore_middle.png", gameSetting->resourcepacksName);
	loadimage(&diamond_pic[1], _T(picURLtemp));

	IMAGE emerald_pic[2];
	sprintf_s(picURLtemp, ".\\textures\\%s\\mine\\emerald_ore_small.png", gameSetting->resourcepacksName);
	loadimage(&emerald_pic[0], _T(picURLtemp));
	sprintf_s(picURLtemp, ".\\textures\\%s\\mine\\emerald_ore_middle.png", gameSetting->resourcepacksName);
	loadimage(&emerald_pic[1], _T(picURLtemp));

	IMAGE mask[2];
	sprintf_s(picURLtemp, ".\\textures\\%s\\mine\\small_mask.bmp", gameSetting->resourcepacksName);
	loadimage(&mask[0], _T(picURLtemp));
	sprintf_s(picURLtemp, ".\\textures\\%s\\mine\\middle_mask.bmp", gameSetting->resourcepacksName);
	loadimage(&mask[1], _T(picURLtemp));

	IMAGE diamond_mask[2];
	sprintf_s(picURLtemp, ".\\textures\\%s\\mine\\diamond_small_mask.bmp", gameSetting->resourcepacksName);
	loadimage(&diamond_mask[0], _T(picURLtemp));
	sprintf_s(picURLtemp, ".\\textures\\%s\\mine\\diamond_middle_mask.bmp", gameSetting->resourcepacksName);
	loadimage(&diamond_mask[1], _T(picURLtemp));

	IMAGE emerald_mask[2];
	sprintf_s(picURLtemp, ".\\textures\\%s\\mine\\emerald_small_mask.bmp", gameSetting->resourcepacksName);
	loadimage(&emerald_mask[0], _T(picURLtemp));
	sprintf_s(picURLtemp, ".\\textures\\%s\\mine\\emerald_middle_mask.bmp", gameSetting->resourcepacksName);
	loadimage(&emerald_mask[1], _T(picURLtemp));

	for (const MINE* j = head; j; j = j->next)
	{
		int index = (j->size) / 32 - 1;
		if (j->state == available) {
			switch (j->kind)
			{
			case stone:
				putimage(j->Pos_x, j->Pos_y, &mask[index], NOTSRCERASE);
				putimage(j->Pos_x, j->Pos_y, &stone_pic[index], SRCINVERT);
				break;
			case iron:
				putimage(j->Pos_x, j->Pos_y, &mask[index], NOTSRCERASE);
				putimage(j->Pos_x, j->Pos_y, &iron_pic[index], SRCINVERT);
				break;
			case gold:
				putimage(j->Pos_x, j->Pos_y, &mask[index], NOTSRCERASE);
				putimage(j->Pos_x, j->Pos_y, &gold_pic[index], SRCINVERT);
				break;
			case diamond:
				putimage(j->Pos_x, j->Pos_y, &diamond_mask[index], NOTSRCERASE);
				putimage(j->Pos_x, j->Pos_y, &diamond_pic[index], SRCINVERT);
				break;
			case emerald:
				putimage(j->Pos_x, j->Pos_y, &emerald_mask[index], NOTSRCERASE);
				putimage(j->Pos_x, j->Pos_y, &emerald_pic[index], SRCINVERT);
				break;
			default:
				break;
			}
		}
	}
}
int calculatePassScore(const MINE * head) {
	int sum = 0;
	for (const MINE* i = head; i; i = i->next)
	{
		if (i->kind != stone) {
			sum += (int)(i->kind) * (i->size) / 32;
		}
	}
	sum = (int)(sum * 0.8);

	//return -1;	//������ͨ�ص���
	return sum;
}

//	�Ҳ����������
void printLineText(LPCTSTR font, COLORREF colour, const int strLong, LPCTSTR str, const int data, const int textY) {
	settextstyle(20, 0, font);
	settextcolor(colour);
	char strbuff[20];
	outtextxy(INFO_BAR_X, textY, str);
	sprintf_s(strbuff, "%d", data);
	outtextxy(INFO_BAR_X + 80, textY, _T(strbuff));
}
void printRankingList(ERR* errorCode, const SETTING* gameSet) {
	settextstyle(20, 0, _T("����"));
	const int TEXT_HIGH_START = 179 + 25;
	PLAYER rankListIndecator[5];
	
	//	��ȡ�ļ�
	PLAYER* p_head = NULL;
	FILE* fp = NULL;
	if ((fopen_s(&fp, ".\\data\\userData.hjq", "r") != 0) || (fp == NULL)) {
		errorCode->code = fileOpenFailed;
		errorCode->info = _T("�������а�ʱ�浵�ļ���ʧ��");
		printErrToFile(errorCode);
	}
	else if (feof(fp)) {
		errorCode->code = fileOpenFailed;
		errorCode->info = "�浵�ļ�Ϊ�գ��ᵼ�¼������а����";
		printErrToFile(errorCode);
	}
	else {
		do {
			PLAYER* playerBuffer = (PLAYER*)malloc(sizeof(PLAYER));
			char userNameBuffer[16];

			//	��ʼ��
			fscanf_s(fp, "%d %d %d %d %s\n", &(playerBuffer->userLevel.currentLevel), &(playerBuffer->userLevel.gameScore), &(playerBuffer->userLevel.passScore), &(playerBuffer->userLevel.timeLast), userNameBuffer, 16);
			playerBuffer->next = NULL;

			PLAYER* last = p_head;
			if (last) {
				while (last->next) {
					last = last->next;
				}
				last->next = playerBuffer;
			}
			else {
				p_head = playerBuffer;
			}
		} while (!feof(fp));
		fclose(fp);

		//	����
		int j = 0;
		for (PLAYER* i = p_head; i && j < 5; i = i->next, j++)
		{
			PLAYER* maxIndex = i;
			for (PLAYER* k = i->next; k; k = k->next)
			{
				if (k->userLevel.currentLevel > maxIndex->userLevel.currentLevel) {
					maxIndex = k;
				}
				else if (k->userLevel.currentLevel == maxIndex->userLevel.currentLevel) {
					double k_rate = 1.0 * (k->userLevel.gameScore) / (k->userLevel.passScore);
					double max_rate = 1.0 * (maxIndex->userLevel.gameScore) / (maxIndex->userLevel.passScore);
					if (k_rate > max_rate) {
						maxIndex = k;
					}
				}
			}
			PLAYER temp;
			temp.userLevel = maxIndex->userLevel;
			maxIndex->userLevel = i->userLevel;
			i->userLevel = temp.userLevel;

			rankListIndecator[j].userLevel = i->userLevel;
		}

		//	print����Ļ��
		outtextxy(INFO_BAR_X, 179, _T("----���а�----"));
		int lineNum = 0;
		for (PLAYER* i = p_head; i && (lineNum < 5); i = i->next, lineNum++)
		{
			double rate = 1.0 * (rankListIndecator[lineNum].userLevel.gameScore) / (rankListIndecator[lineNum].userLevel.passScore);
			char levelBuff[36];
			sprintf_s(levelBuff, "��%d��,���%%%d", rankListIndecator[lineNum].userLevel.currentLevel, (int)(rate * 100));
			outtextxy(INFO_BAR_X, TEXT_HIGH_START + lineNum * 25, levelBuff);
		}


		//	�ͷ�malloc�Ŀռ�
		PLAYER* p = p_head;
		PLAYER* q = p_head;
		for (p = p_head, q = p->next; q; p = q, q = q->next)
		{
			free(p);
		}
	}
}

//	���Ͻ���ʾ������ʲôbuff
void printHookFunc(const HOOK * thisHook) {
	settextstyle(20, 0, _T("����"));
	settextcolor(BLUE);
	switch (thisHook->func)
	{
		//	�˴�Ϊ���������
#include "data/item/localization.hjq"

	default:
		break;
	}
}

void pauseGame(const SETTING* gameSet) {
	while (1) {
		char ch;
		if (_kbhit()) {
			ch = _getch();
			if (ch == gameSet->KB_pauseGame) {
				break;
			}
		}
		FlushBatchDraw();
		const int TEXT_OUT_X = 800;
		settextcolor(RED);
		outtextxy(TEXT_OUT_X, 20, _T("��Ϸ����ͣ"));
		outtextxy(TEXT_OUT_X, 40, _T("����ݼ����"));
	}
}
void SaveGame(const PLAYER* player, ERR* errorCode) {
	FILE* fp = NULL;
	if ((fopen_s(&fp, ".\\data\\userData.hjq", "a") == 0) && fp) {
		fprintf_s(fp, "%d %d %d %d %s\n", player->userLevel.currentLevel, player->userLevel.gameScore, player->userLevel.passScore, player->userLevel.timeLast, player->userName);
		fclose(fp);
		settextcolor(RED);

		//	��Ļ���
		const int TEXT_OUT_X = 800;
		outtextxy(TEXT_OUT_X, 20, _T("��Ϸ�ѱ���"));
		FlushBatchDraw();
		Sleep(500);
	}
	else {
		errorCode->code = fileOpenFailed;
		errorCode->info = _T("������Ϸʱ�ļ����ش���");
		printErrToFile(errorCode);
	}
}
void gameOver(PLAYER* thisPlayer, MINE* m_head, ITEMLIST* i_head, ERR* errorCode, SETTING * gameSet) {
	//	�����ȱ�����Ϸ
	if (gameSet->autoSave) {
		SaveGame(thisPlayer, errorCode);
	}

	//printf("\t�ⶼ���꣬�����Ǹ�ɵ��\n");
	//	�����������
	if (gameSet->putItems){
		ITEM_N* r = i_head->loadItemHead->next;
		ITEM_N* s = i_head->loadItemHead->next;
		for (r = i_head->loadItemHead->next, s = r->next; s; r = s, s = s->next)
		{
			free(r);
		}
		i_head->loadItemHead->next = NULL;
	}

	//	�������ͼƬ
	if (gameSet->putItems) {
		ITEM_P* u = i_head->itemPicHead->next;
		ITEM_P* v = i_head->itemPicHead->next;
		for (u = i_head->itemPicHead->next, v = u->next; v; u = v, v = v->next)
		{
			delete(u);
		}
		i_head->itemPicHead->next = NULL;
	}
	
	//	�˳�
	errorCode->code = exitByPlayer;
	errorCode->info = _T("������Ϸ���������˳���Ϸ");
	printMsgToFile(errorCode);

	//	�������˵�
	if (InputBox(NULL, 1, _T("�Ƿ񷵻����˵��������ȷ�����������˵��������ȡ�����˳���Ϸ"), NULL, thisPlayer->userName, 0, 0, false)) {
		closegraph();
		bookmarkOpening(thisPlayer, errorCode, gameSet);
		bookmarkMain(thisPlayer, errorCode, gameSet);
	}
	else {
		exit(0);
	}
}