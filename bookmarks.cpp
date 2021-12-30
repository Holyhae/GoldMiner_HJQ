#include <conio.h>
#include <graphics.h>
#include <stdio.h>
#include "bookmarks.h"
#include "functionsmain.h"
#include "item.h"
#include "gui.h"
#pragma comment(lib, "WINMM.LIB")

#define HIGH 505
#define WIDTH 960

void drawMainPic(const SETTING * gameSet, IMAGE*, IMAGE*, IMAGE*);
void version_info(const SETTING * set);

void bookmarkOpening(PLAYER* player, ERR* errorCode, SETTING* gameSet) {
	char picURLtemp[64];
	
	closegraph();	//	Ԥ���ӿ�
	if (gameSet->debugMode) {
		initgraph(WIDTH, HIGH, SHOWCONSOLE);
	}
	else {
		initgraph(WIDTH, HIGH);
	}

	//	�����ļ�
	{
		FILE* f_err = NULL;
		if (fopen_s(&f_err, "err.log", "w") == 0) {
			fprintf_s(f_err, "=====================��Ϣ�б�=====================\n\n");
			fprintf_s(f_err, "<��ʼ>\t��ǰҳ��Ϊ��Ϸ�����棬�ȴ������Ӧ\n\n");
			fclose(f_err);
		}
		else {
			errorCode->code = fileOpenFailed;
			errorCode->info = _T("�����ļ�����ʧ��");
			printf("[����]\t�����ļ���ʧ��\t|||\t\t�������Ϊ %d\n", (int)errFileOpenFailed);
			exit((int)errFileOpenFailed);
		}
	}

	IMAGE middle_botton;
	IMAGE middle_botton_f;
	IMAGE middle_botton_s;
	drawMainPic(gameSet, &middle_botton, &middle_botton_f, &middle_botton_s);

	BeginBatchDraw();
	do {
		//	��׽�����Ϣ
		MOUSEMSG m_bookmark1;
		if (MouseHit()) {
			m_bookmark1 = GetMouseMsg();
			FlushMouseMsgBuffer();
		}
		
		//	����Ϸ
		if (create_mid_button(275, 330, &middle_botton, &middle_botton_f, &middle_botton_s, _T("��ʼ����Ϸ"), WHITE, &m_bookmark1)) {
			player->userLevel.currentLevel = 1;	//	�ӵ�һ�ؿ�ʼ

			const int USER_NAME_LENTH_MAX = 7;
			InputBox(player->userName, 32, "�������û���(������7����ĸ)", NULL, "System", 0, 0, true);
			while ((player->userName[0] == '\0') || (strlen(player->userName) >= USER_NAME_LENTH_MAX)) {
				if (player->userName[0] == '\0') {
					InputBox(player->userName, 32, "�û�������Ϊ��,������(������7����ĸ)", NULL, "System", 0, 0, true);
				}
				if (strlen(player->userName) >= USER_NAME_LENTH_MAX) {
					InputBox(player->userName, 32, "�û���̫������������(������7����ĸ)", NULL, "System", 0, 0, true);
				}
			}
			char nameBuff[64];
			sprintf_s(nameBuff, "�û����Ѿ�������ϣ��û���Ϊ%s", player->userName);
			errorCode->info = _T(nameBuff);
			printMsgToFile(errorCode);
			return;
		}
		//	��ȡ��Ϸ
		if (create_mid_button(275 + 10 + 198, 330, &middle_botton, &middle_botton_f, &middle_botton_s, _T("�Ӵ浵��ȡ"), WHITE, &m_bookmark1)) {
			if (bookmarkSaveSelect(player, errorCode, gameSet)) {
				return;
			}
			m_bookmark1.mkLButton = 0;
			drawMainPic(gameSet, &middle_botton, &middle_botton_f, &middle_botton_s);
		}
		//	ѡ�����
		if (create_mid_button(275, 380, &middle_botton, &middle_botton_f, &middle_botton_s, _T("װ�ز���"), WHITE, &m_bookmark1)) {
			do {
				InputBox(gameSet->resourcepacksName, 48, "�� ���� �� ȷ�� ���ʰ�����(���48����ĸ):", NULL, gameSet->resourcepacksName, 0, 0, true);
			} while (gameSet->resourcepacksName[0] == '\0');
			m_bookmark1.mkLButton = 0;
			drawMainPic(gameSet, &middle_botton, &middle_botton_f, &middle_botton_s);
			//	Faithful
			//	Sphax
			char rsPackBuff[64];
			sprintf_s(rsPackBuff, "�����Ѹ��£�������Ϊ%s", gameSet->resourcepacksName);
			errorCode->info = _T(rsPackBuff);
			printMsgToFile(errorCode);
		}
		//	����
		if (create_mid_button(275 + 10 + 198, 380, &middle_botton, &middle_botton_f, &middle_botton_s, _T("����"), WHITE, &m_bookmark1)) {
			m_bookmark1.mkLButton = 0;
			bookmarkSetting(errorCode, gameSet);
			m_bookmark1.mkLButton = 0;
			drawMainPic(gameSet, &middle_botton, &middle_botton_f, &middle_botton_s);
		}
		// ��մ浵
		if (create_mid_button(275, 430, &middle_botton, &middle_botton_f, &middle_botton_s, _T("��մ浵����"), WHITE, &m_bookmark1)) {
			FILE* fp = NULL;
			if ((fopen_s(&fp, ".\\data\\userData.hjq", "w") == 0) && fp) {
				fprintf_s(fp, "%d %d %d %d system\n", 1, 0, 1, 2400);
				fclose(fp);
			}
			else {
				errorCode->code = fileOpenFailed;
				exit((int)errorCode->code);
				printErrToFile(errorCode);
			}
		}
		//	�˳���Ϸ
		if (create_mid_button(275 + 10 + 198, 430, &middle_botton, &middle_botton_f, &middle_botton_s, _T("�˳�"), WHITE, &m_bookmark1)) {
			closegraph();
			errorCode->code = exitByPlayer;
			exit((int)errorCode->code);
		}

		FlushBatchDraw();
	} while (1);
}

bool bookmarkSaveSelect(PLAYER* player, ERR* errorCode, const SETTING* gameSet) {
	char picURLtemp[64];
	
	bool return_;

	closegraph();	//	Ԥ���ӿ�
	if (gameSet->debugMode) {
		initgraph(WIDTH, HIGH, SHOWCONSOLE);
	}
	else {
		initgraph(WIDTH, HIGH);
	}

	//	�ĵ�������ʾ���
	const int TEXT_BAR_X = 45;
	const int LIST_START_Y = 79;
	const int NAME_IMPUT_HIGHT = 23;
	char nameBuffer[] = "                ";
	char inputBuffer;
	int i = 0;		//	����ʱ�ַ�����
	int mouseScroll = 0;
	bool canMouseUse = false;

	//	��ȡ�ļ�
	PLAYER* p_head = NULL;
	FILE* fp = NULL;
	if ((fopen_s(&fp, ".\\data\\userData.hjq", "r") != 0) || (fp == NULL)) {
		errorCode->code = fileOpenFailed;
		errorCode->info = "���ش浵ҳ���ȡ�浵�ļ�����";
		printErrToFile(errorCode);
	}
	else if (feof(fp)) {
		errorCode->code = fileOpenFailed;
		errorCode->info = "�浵�ļ�Ϊ��";
		printErrToFile(errorCode);
	}
	else {
		do {
			PLAYER* playerBuffer = (PLAYER*)malloc(sizeof(PLAYER));
			char userNameBuffer[16];

			//	��ʼ��
			fscanf_s(fp, "%d %d %d %d %s\n", &(playerBuffer->userLevel.currentLevel), &(playerBuffer->userLevel.gameScore), &(playerBuffer->userLevel.passScore), &(playerBuffer->userLevel.timeLast), playerBuffer->userName, 16);
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
	}

	IMAGE portal;
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\portalx32.png", gameSet->resourcepacksName);
	loadimage(&portal, _T(picURLtemp));
	IMAGE portal_f;
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\portalx32_f.png", gameSet->resourcepacksName);
	loadimage(&portal_f, _T(picURLtemp));
	IMAGE middle_botton;
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\midgets.png", gameSet->resourcepacksName);
	loadimage(&middle_botton, _T(picURLtemp));
	IMAGE middle_botton_f;
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\midgets_f.png", gameSet->resourcepacksName);
	loadimage(&middle_botton_f, _T(picURLtemp));
	IMAGE middle_botton_s;
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\midgets_s.png", gameSet->resourcepacksName);
	loadimage(&middle_botton_s, _T(picURLtemp));

	BeginBatchDraw();
	while (1) {
		cleardevice();

		//	����ͼƬ
		{
			IMAGE bookmark1_bg;
			sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\bookmarks1bg.jpg", gameSet->resourcepacksName);
			loadimage(&bookmark1_bg, _T(picURLtemp));
			putimage(0, 0, &bookmark1_bg);
			setbkmode(TRANSPARENT);

			IMAGE login;
			sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\login.png", gameSet->resourcepacksName);
			loadimage(&login, _T(picURLtemp));
			putimage(0, 0, &login);
		}

		//	�����Ϊ
		MOUSEMSG m;
		
		if (MouseHit()) {
			m = GetMouseMsg();
			if (canMouseUse) {
				if (m.wheel > 0) {
					mouseScroll -= 10;
				}
				else if (m.wheel < 0) {
					mouseScroll += 10;
				}
			}
		}
		//	�û���
		if (i <= 7) {
			settextcolor(BLACK);
		}
		else {
			settextcolor(RED);
		}
		settextstyle(20, 0, _T("����"));
		outtextxy(TEXT_BAR_X, NAME_IMPUT_HIGHT, _T("�������û���:"));

		if (_kbhit()) {
			inputBuffer = _getch();
			if ((isdigit(inputBuffer)) || (isalpha(inputBuffer)) && i < 10) {
				nameBuffer[i] = inputBuffer;
				i++;
			}
			else if ((i > 0)&&(inputBuffer == 8)) {
				i--;
				nameBuffer[i] = ' ';
			}
		}
		outtextxy(TEXT_BAR_X + 150, NAME_IMPUT_HIGHT, _T(nameBuffer));
		char compareBuff[16];
		int k = 0;
		for (k = 0; (nameBuffer[k] != ' ') && k < 16; k++)
		{
			compareBuff[k] = nameBuffer[k];
		}
		compareBuff[k] = '\0';
		int lineNum = 0;
		//	��ʾ���а�
		for (PLAYER* i = p_head; i; i = i->next)
		{
			if (strstr(i->userName, compareBuff))
			{
				double rate = 1.0 * (i->userLevel.gameScore) / (i->userLevel.passScore);
				char levelBuff[256];
				sprintf_s(levelBuff, "%d.��� %s �������%d�أ��ù��������%%%d", lineNum + 1,i->userName, i->userLevel.currentLevel, (int)(rate * 100));
				if (((LIST_START_Y + lineNum * 32 - mouseScroll) >= LIST_START_Y) && ((LIST_START_Y + lineNum * 32 - mouseScroll) < HIGH - 44)) {
					outtextxy(TEXT_BAR_X + 42, LIST_START_Y + lineNum * 32 - mouseScroll, levelBuff);
					LPCSTR button_desc_temp = _T("�Ե�ǰ��Ϊ�׹ز��Ե�ǰ�û�����ʼ��Ϸ�����Ǹùؿ��Ľ��Ȳ��ᱻ����        ");
					if (create_portal_button(TEXT_BAR_X, LIST_START_Y + lineNum * 32 - 8 - mouseScroll, &portal, &portal_f, &portal_f, &m, button_desc_temp)) {
						player->userLevel = i->userLevel;
						strcpy_s(player->userName, i->userName);
						return_ = true;
						return return_;
					}
				}
				lineNum++;
			}
		}
		if (lineNum <= 13) {
			canMouseUse = false;
		} else {
			canMouseUse = true;
		}

		//	Ԥ�������ؼ���
		if (create_mid_button(675, 50, &middle_botton, &middle_botton_f, &middle_botton_s, _T("�������˵�"), WHITE, &m, "��˷��ص����˵�")) {
			return_ = false;
			return return_;
		}

		FlushBatchDraw();
	}
	//	�ͷ�malloc�Ŀռ�
	PLAYER* p = p_head;
	PLAYER* q = p_head;
	for (p = p_head, q = p->next; q; p = q, q = q->next)
	{
		free(p);
	}
	EndBatchDraw();

	return 0;
}

void bookmarkSetting(ERR* errorCode, SETTING* gameSet) {
	char picURLtemp[64];
	COLORREF textColour = WHITE;

	closegraph();	//	Ԥ���ӿ�
	if (gameSet->debugMode) {
		initgraph(WIDTH, HIGH, SHOWCONSOLE);
	}
	else {
		initgraph(WIDTH, HIGH);
	}

	BeginBatchDraw();
	while (1)
	{
		cleardevice();

		//	����ͼƬ
		{
			IMAGE bookmark1_bg;
			sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\bookmarks1bg.jpg", gameSet->resourcepacksName);
			loadimage(&bookmark1_bg, _T(picURLtemp));
			putimage(0, 0, &bookmark1_bg);
			setbkmode(TRANSPARENT);
		}

		//	�еȰ�ť
		IMAGE middle_botton;
		sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\midgets.png", gameSet->resourcepacksName);
		loadimage(&middle_botton, _T(picURLtemp));
		IMAGE middle_botton_f;
		sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\midgets_f.png", gameSet->resourcepacksName);
		loadimage(&middle_botton_f, _T(picURLtemp));
		IMAGE middle_botton_s;
		sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\midgets_s.png", gameSet->resourcepacksName);
		loadimage(&middle_botton_s, _T(picURLtemp));

		//	�����Ϊ
		MOUSEMSG m;
		if (MouseHit()) { m = GetMouseMsg(); }

		//	����
		if (create_mid_button(WIDTH / 2 - 198 / 2, HIGH* 7 / 8, &middle_botton, &middle_botton_f, &middle_botton_s, _T("���沢�������˵�"), WHITE, &m, "��˷��ص����˵�������֮ǰ����������"))
		{
			m.mkLButton = false;
			break;
		}

		//	�򿪵���
		{
			char putItemsBottonText[64];
			LPCSTR desc;
			if (gameSet->putItems) {
				strcpy_s(putItemsBottonText, "����");
				textColour = DARK_GREEN;
				desc = _T("Ϊ��Ϸ����Ӹ��ָ�����������ߣ���ǰΪ����������ر�");
			}
			else {
				strcpy_s(putItemsBottonText, "�ر�");
				textColour = RED;
				desc = _T("Ϊ��Ϸ����Ӹ��ָ�����������ߣ���ǰΪ�رգ��������");
			}
			settextcolor(BLACK);
			outtextxy(WIDTH / 2 - 198 / 2 - 50, HIGH * 1 / 8 - 25, _T("����������:"));
			if (create_mid_button(WIDTH / 2 - 198 / 2, HIGH * 1 / 8, &middle_botton, &middle_botton_f, &middle_botton_s, _T(putItemsBottonText), textColour, &m, desc))
			{
				m.mkLButton = false;
				gameSet->putItems = !(gameSet->putItems);
			}
		}

		//	�Ƿ��Զ�����
		{
			char sutoSaveBottonText[64];
			LPCSTR desc;
			if (gameSet->autoSave) {
				strcpy_s(sutoSaveBottonText, "����");
				textColour = DARK_GREEN;
				desc = _T("�Ƿ���ÿ����Ϸͨ��ʱ��ʱ�����ʧ�ܺ��Զ�������Ϸ����ǰΪ����������ر�");
			}
			else {
				strcpy_s(sutoSaveBottonText, "�ر�");
				textColour = RED;
				desc = _T("�Ƿ���ÿ����Ϸͨ��ʱ��ʱ�����ʧ�ܺ��Զ�������Ϸ����ǰΪ�رգ��������");
			}
			settextcolor(BLACK);
			outtextxy(WIDTH / 2 - 198 / 2 - 50, HIGH * 2 / 8 - 25, _T("�Զ�����:"));
			if (create_mid_button(WIDTH / 2 - 198 / 2, HIGH * 2 / 8, &middle_botton, &middle_botton_f, &middle_botton_s, _T(sutoSaveBottonText), textColour, &m, desc))
			{
				m.mkLButton = false;
				gameSet->autoSave = !(gameSet->autoSave);
			}
		}

		//	�Ƿ����ɹ���
		{
			char spawnMobBottonText[64];
			LPCSTR desc;
			if (gameSet->doMobSpawn) {
				strcpy_s(spawnMobBottonText, "����");
				textColour = DARK_GREEN;
				desc = _T("�Ƿ���ÿ����Ϸʱ�ڵ������һЩ�����ǰΪ����������ر�");
			}
			else {
				strcpy_s(spawnMobBottonText, "�ر�");
				textColour = RED;
				desc = _T("�Ƿ���ÿ����Ϸʱ�ڵ������һЩ�����ǰΪ�رգ��������");
			}
			settextcolor(BLACK);
			outtextxy(WIDTH / 2 - 198 / 2 - 50, HIGH * 3 / 8 - 25, _T("��������:"));
			if (create_mid_button(WIDTH / 2 - 198 / 2, HIGH * 3 / 8, &middle_botton, &middle_botton_f, &middle_botton_s, _T(spawnMobBottonText), textColour, &m, desc))
			{
				m.mkLButton = false;
				gameSet->doMobSpawn = !(gameSet->doMobSpawn);
			}
		}

		settextcolor(BLACK);
		//	�����ݼ�(�����쳤)
		{
			LPCSTR desc;
			char title[3] = { gameSet->KB_enlongHook, '\0', '\0' };
			switch (gameSet->KB_enlongHook)
			{
			case ' ':
				title[0] = 'B';
				title[1] = 'S';
				break;
			default:
				title[0] = gameSet->KB_enlongHook;
				break;
			}
			desc = _T("������蹳���쳤�Ŀ�ݼ�");
			outtextxy(WIDTH / 5 - 198 / 2 - 50, HIGH * 5 / 8 - 25, _T("�����쳤��ݼ�"));
			if (create_mid_button(WIDTH / 5 - 198 / 2, HIGH * 5 / 8, &middle_botton, &middle_botton_f, &middle_botton_s, _T(title), BLACK, &m, desc))
			{
				gameSet->KB_enlongHook = _getch();
			}
		}

		//	�����ݼ�(��ͣ)
		{
			LPCSTR desc;
			char title[2] = { gameSet->KB_pauseGame, '\0' };
			desc = _T("���������ͣ�Ŀ�ݼ�");
			outtextxy(WIDTH * 3 / 5 - 198 / 2 - 50, HIGH * 5 / 8 - 25, _T("��ͣ��ݼ�"));
			if (create_mid_button(WIDTH * 3 / 5 - 198 / 2, HIGH * 5 / 8, &middle_botton, &middle_botton_f, &middle_botton_s, _T(title), BLACK, &m, desc))
			{
				gameSet->KB_pauseGame = _getch();
			}
		}

		//	�����ݼ�(�˳�)
		{
			LPCSTR desc;
			char title[2] = { gameSet->KB_quitGame, '\0' };
			desc = _T("��������˳��Ŀ�ݼ�");
			outtextxy(WIDTH * 1 / 5 - 198 / 2 - 50, HIGH * 6 / 8 - 25, _T("�˳���ݼ�"));
			if (create_mid_button(WIDTH * 1 / 5 - 198 / 2, HIGH * 6 / 8, &middle_botton, &middle_botton_f, &middle_botton_s, _T(title), BLACK, &m, desc))
			{
				gameSet->KB_quitGame = _getch();
			}
		}

		//	�����ݼ�(����)
		{
			LPCSTR desc;
			char title[2] = { gameSet->KB_saveGame, '\0' };
			desc = _T("������豣��Ŀ�ݼ�");
			outtextxy(WIDTH * 3 / 5 - 198 / 2 - 50, HIGH * 6 / 8 - 25, _T("����ݼ�"));
			if (create_mid_button(WIDTH * 3 / 5 - 198 / 2, HIGH * 6 / 8, &middle_botton, &middle_botton_f, &middle_botton_s, _T(title), BLACK, &m, desc))
			{
				gameSet->KB_saveGame = _getch();
			}
		}

		FlushBatchDraw();
	}

	//EndBatchDraw();
	return;
}

void bookmarkMain(PLAYER* player, ERR* errorCode, SETTING* gameSet) {

	/////////////////
	//   αȫ�ֱ���  //
	/////////////////

	//	�������
	MINE* m_head = NULL;

	//	�������
	ITEMLIST i_head;
	i_head.gameItemHead = NULL;
	i_head.itemPicHead = NULL;
	i_head.loadItemHead = NULL;


	//	�������
	HOOK* thisHook = (HOOK*)malloc(sizeof(HOOK));
	thisHook->angle = 1;
	thisHook->hookConductance = 0;
	thisHook->hookLong = 50;
	thisHook->rollTemp = left;
	thisHook->hookNowState = hang;
	thisHook->startPointHook_X = 475;
	thisHook->startPointHook_Y = 50;
	thisHook->func = 0;

	//	����
	MOBHEAD mobHead;
	mobHead.mobHead = NULL;
	mobHead.mobIdHead = NULL;
	mobHead.mobPicHead = NULL;

	///////////////
	//  ��Ϸ����  //
	///////////////
	startup(player, &m_head, &i_head, &mobHead, errorCode, gameSet);
	while (1) {
		show(player, thisHook, m_head, &i_head, &mobHead, errorCode, gameSet);
		updateWithoutInput(player, thisHook, m_head, &i_head, &mobHead, errorCode, gameSet);
		updateWithInput(player,m_head, thisHook, &i_head, errorCode, gameSet);

		FlushBatchDraw();
		Sleep(10);
	}
}

bool nextLevelMain(const SETTING* gameSet) {
	char picURLtemp[64];
	bool _return = false;

	//	����ͼƬ 584*445
	IMAGE notice;
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\notice.png", gameSet->resourcepacksName);
	loadimage(&notice, _T(picURLtemp));
	putimage(WIDTH / 2 - 584 / 2, HIGH / 2 - 445 / 2, &notice);

	IMAGE middle_botton, middle_botton_f, middle_botton_s;
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\midgets.png", gameSet->resourcepacksName);
	loadimage(&middle_botton, _T(picURLtemp));
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\midgets_f.png", gameSet->resourcepacksName);
	loadimage(&middle_botton_f, _T(picURLtemp));
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\midgets_s.png", gameSet->resourcepacksName);
	loadimage(&middle_botton_s, _T(picURLtemp));

	MOUSEMSG m;
	while (true)
	{
		if (MouseHit()) {
			m = GetMouseMsg();
		}

		//	�������˵�
		if (create_mid_button(275 + 10 + 198, 380, &middle_botton, &middle_botton_f, &middle_botton_s, _T("�������˵�"), WHITE, &m)) {
			_return = false;
			break;
		}

		//	������һ��
		if (create_mid_button(275, 380, &middle_botton, &middle_botton_f, &middle_botton_s, _T("������һ��"), WHITE, &m)) {
			_return = true;
			break;
		}

		FlushBatchDraw();
	}

	return _return;
}

//	����������
void drawMainPic(const SETTING * gameSet, IMAGE* middle_botton, IMAGE* middle_botton_f, IMAGE* middle_botton_s) {
	cleardevice();
	
	char picURLtemp[64];

	//	����ͼƬ
	IMAGE bookmark1_bg;
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\bookmarks1bg.jpg", gameSet->resourcepacksName);
	loadimage(&bookmark1_bg, _T(picURLtemp));
	putimage(0, 0, &bookmark1_bg);
	setbkmode(TRANSPARENT);

	//	���ƽ�󹤡��ĸ�����
	IMAGE title_front;
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\title_light.jpg", gameSet->resourcepacksName);
	loadimage(&title_front, _T(picURLtemp));
	IMAGE title_bg;
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\title_dark.jpg", gameSet->resourcepacksName);
	loadimage(&title_bg, _T(picURLtemp));
	putimage(70, 30, &title_bg, NOTSRCERASE);
	putimage(70, 30, &title_front, SRCINVERT);

	//	��������
	mciSendString("open \".\\music\\Minecraft.mp3\" alias bkmusic", NULL, 0, NULL);
	mciSendString("play bkmusic repeat", NULL, 0, NULL);

	//	��ʾ��Ϣ
	version_info(gameSet);

	//	��ť
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\midgets.png", gameSet->resourcepacksName);
	loadimage(middle_botton, _T(picURLtemp));
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\midgets_f.png", gameSet->resourcepacksName);
	loadimage(middle_botton_f, _T(picURLtemp));
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\midgets_s.png", gameSet->resourcepacksName);
	loadimage(middle_botton_s, _T(picURLtemp));
}