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
	
	closegraph();	//	预留接口
	if (gameSet->debugMode) {
		initgraph(WIDTH, HIGH, SHOWCONSOLE);
	}
	else {
		initgraph(WIDTH, HIGH);
	}

	//	错误文件
	{
		FILE* f_err = NULL;
		if (fopen_s(&f_err, "err.log", "w") == 0) {
			fprintf_s(f_err, "=====================消息列表=====================\n\n");
			fprintf_s(f_err, "<开始>\t当前页面为游戏主界面，等待玩家响应\n\n");
			fclose(f_err);
		}
		else {
			errorCode->code = fileOpenFailed;
			errorCode->info = _T("错误文件生成失败");
			printf("[错误]\t错误文件打开失败\t|||\t\t错误代码为 %d\n", (int)errFileOpenFailed);
			exit((int)errFileOpenFailed);
		}
	}

	IMAGE middle_botton;
	IMAGE middle_botton_f;
	IMAGE middle_botton_s;
	drawMainPic(gameSet, &middle_botton, &middle_botton_f, &middle_botton_s);

	BeginBatchDraw();
	do {
		//	捕捉鼠标信息
		MOUSEMSG m_bookmark1;
		if (MouseHit()) {
			m_bookmark1 = GetMouseMsg();
			FlushMouseMsgBuffer();
		}
		
		//	新游戏
		if (create_mid_button(275, 330, &middle_botton, &middle_botton_f, &middle_botton_s, _T("开始新游戏"), WHITE, &m_bookmark1)) {
			player->userLevel.currentLevel = 1;	//	从第一关开始

			const int USER_NAME_LENTH_MAX = 7;
			InputBox(player->userName, 32, "请输入用户名(不超过7个字母)", NULL, "System", 0, 0, true);
			while ((player->userName[0] == '\0') || (strlen(player->userName) >= USER_NAME_LENTH_MAX)) {
				if (player->userName[0] == '\0') {
					InputBox(player->userName, 32, "用户名不能为空,请重输(不超过7个字母)", NULL, "System", 0, 0, true);
				}
				if (strlen(player->userName) >= USER_NAME_LENTH_MAX) {
					InputBox(player->userName, 32, "用户名太长，请重输入(不超过7个字母)", NULL, "System", 0, 0, true);
				}
			}
			char nameBuff[64];
			sprintf_s(nameBuff, "用户名已经输入完毕，用户名为%s", player->userName);
			errorCode->info = _T(nameBuff);
			printMsgToFile(errorCode);
			return;
		}
		//	读取游戏
		if (create_mid_button(275 + 10 + 198, 330, &middle_botton, &middle_botton_f, &middle_botton_s, _T("从存档读取"), WHITE, &m_bookmark1)) {
			if (bookmarkSaveSelect(player, errorCode, gameSet)) {
				return;
			}
			m_bookmark1.mkLButton = 0;
			drawMainPic(gameSet, &middle_botton, &middle_botton_f, &middle_botton_s);
		}
		//	选择材质
		if (create_mid_button(275, 380, &middle_botton, &middle_botton_f, &middle_botton_s, _T("装载材质"), WHITE, &m_bookmark1)) {
			do {
				InputBox(gameSet->resourcepacksName, 48, "请 输入 并 确认 材质包名：(最多48个字母):", NULL, gameSet->resourcepacksName, 0, 0, true);
			} while (gameSet->resourcepacksName[0] == '\0');
			m_bookmark1.mkLButton = 0;
			drawMainPic(gameSet, &middle_botton, &middle_botton_f, &middle_botton_s);
			//	Faithful
			//	Sphax
			char rsPackBuff[64];
			sprintf_s(rsPackBuff, "材质已更新，现在是为%s", gameSet->resourcepacksName);
			errorCode->info = _T(rsPackBuff);
			printMsgToFile(errorCode);
		}
		//	设置
		if (create_mid_button(275 + 10 + 198, 380, &middle_botton, &middle_botton_f, &middle_botton_s, _T("设置"), WHITE, &m_bookmark1)) {
			m_bookmark1.mkLButton = 0;
			bookmarkSetting(errorCode, gameSet);
			m_bookmark1.mkLButton = 0;
			drawMainPic(gameSet, &middle_botton, &middle_botton_f, &middle_botton_s);
		}
		// 清空存档
		if (create_mid_button(275, 430, &middle_botton, &middle_botton_f, &middle_botton_s, _T("清空存档数据"), WHITE, &m_bookmark1)) {
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
		//	退出游戏
		if (create_mid_button(275 + 10 + 198, 430, &middle_botton, &middle_botton_f, &middle_botton_s, _T("退出"), WHITE, &m_bookmark1)) {
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

	closegraph();	//	预留接口
	if (gameSet->debugMode) {
		initgraph(WIDTH, HIGH, SHOWCONSOLE);
	}
	else {
		initgraph(WIDTH, HIGH);
	}

	//	文档内容显示相关
	const int TEXT_BAR_X = 45;
	const int LIST_START_Y = 79;
	const int NAME_IMPUT_HIGHT = 23;
	char nameBuffer[] = "                ";
	char inputBuffer;
	int i = 0;		//	键入时字符长度
	int mouseScroll = 0;
	bool canMouseUse = false;

	//	读取文件
	PLAYER* p_head = NULL;
	FILE* fp = NULL;
	if ((fopen_s(&fp, ".\\data\\userData.hjq", "r") != 0) || (fp == NULL)) {
		errorCode->code = fileOpenFailed;
		errorCode->info = "加载存档页面读取存档文件错误";
		printErrToFile(errorCode);
	}
	else if (feof(fp)) {
		errorCode->code = fileOpenFailed;
		errorCode->info = "存档文件为空";
		printErrToFile(errorCode);
	}
	else {
		do {
			PLAYER* playerBuffer = (PLAYER*)malloc(sizeof(PLAYER));
			char userNameBuffer[16];

			//	初始化
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

		//	背景图片
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

		//	鼠标行为
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
		//	用户名
		if (i <= 7) {
			settextcolor(BLACK);
		}
		else {
			settextcolor(RED);
		}
		settextstyle(20, 0, _T("宋体"));
		outtextxy(TEXT_BAR_X, NAME_IMPUT_HIGHT, _T("请输入用户名:"));

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
		//	显示排行榜
		for (PLAYER* i = p_head; i; i = i->next)
		{
			if (strstr(i->userName, compareBuff))
			{
				double rate = 1.0 * (i->userLevel.gameScore) / (i->userLevel.passScore);
				char levelBuff[256];
				sprintf_s(levelBuff, "%d.玩家 %s 完成至第%d关，该关已完成至%%%d", lineNum + 1,i->userName, i->userLevel.currentLevel, (int)(rate * 100));
				if (((LIST_START_Y + lineNum * 32 - mouseScroll) >= LIST_START_Y) && ((LIST_START_Y + lineNum * 32 - mouseScroll) < HIGH - 44)) {
					outtextxy(TEXT_BAR_X + 42, LIST_START_Y + lineNum * 32 - mouseScroll, levelBuff);
					LPCSTR button_desc_temp = _T("以当前关为首关并以当前用户名开始游戏，但是该关卡的进度不会被保留        ");
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

		//	预留“返回键”
		if (create_mid_button(675, 50, &middle_botton, &middle_botton_f, &middle_botton_s, _T("返回主菜单"), WHITE, &m, "点此返回到主菜单")) {
			return_ = false;
			return return_;
		}

		FlushBatchDraw();
	}
	//	释放malloc的空间
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

	closegraph();	//	预留接口
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

		//	背景图片
		{
			IMAGE bookmark1_bg;
			sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\bookmarks1bg.jpg", gameSet->resourcepacksName);
			loadimage(&bookmark1_bg, _T(picURLtemp));
			putimage(0, 0, &bookmark1_bg);
			setbkmode(TRANSPARENT);
		}

		//	中等按钮
		IMAGE middle_botton;
		sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\midgets.png", gameSet->resourcepacksName);
		loadimage(&middle_botton, _T(picURLtemp));
		IMAGE middle_botton_f;
		sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\midgets_f.png", gameSet->resourcepacksName);
		loadimage(&middle_botton_f, _T(picURLtemp));
		IMAGE middle_botton_s;
		sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\midgets_s.png", gameSet->resourcepacksName);
		loadimage(&middle_botton_s, _T(picURLtemp));

		//	鼠标行为
		MOUSEMSG m;
		if (MouseHit()) { m = GetMouseMsg(); }

		//	返回
		if (create_mid_button(WIDTH / 2 - 198 / 2, HIGH* 7 / 8, &middle_botton, &middle_botton_f, &middle_botton_s, _T("保存并返回主菜单"), WHITE, &m, "点此返回到主菜单并保存之前的所有设置"))
		{
			m.mkLButton = false;
			break;
		}

		//	打开道具
		{
			char putItemsBottonText[64];
			LPCSTR desc;
			if (gameSet->putItems) {
				strcpy_s(putItemsBottonText, "开启");
				textColour = DARK_GREEN;
				desc = _T("为游戏中添加各种各样的随机道具，当前为开启，点击关闭");
			}
			else {
				strcpy_s(putItemsBottonText, "关闭");
				textColour = RED;
				desc = _T("为游戏中添加各种各样的随机道具，当前为关闭，点击开启");
			}
			settextcolor(BLACK);
			outtextxy(WIDTH / 2 - 198 / 2 - 50, HIGH * 1 / 8 - 25, _T("添加随机道具:"));
			if (create_mid_button(WIDTH / 2 - 198 / 2, HIGH * 1 / 8, &middle_botton, &middle_botton_f, &middle_botton_s, _T(putItemsBottonText), textColour, &m, desc))
			{
				m.mkLButton = false;
				gameSet->putItems = !(gameSet->putItems);
			}
		}

		//	是否自动保存
		{
			char sutoSaveBottonText[64];
			LPCSTR desc;
			if (gameSet->autoSave) {
				strcpy_s(sutoSaveBottonText, "开启");
				textColour = DARK_GREEN;
				desc = _T("是否在每关游戏通关时和时间归零失败后自动保存游戏，当前为开启，点击关闭");
			}
			else {
				strcpy_s(sutoSaveBottonText, "关闭");
				textColour = RED;
				desc = _T("是否在每关游戏通关时和时间归零失败后自动保存游戏，当前为关闭，点击开启");
			}
			settextcolor(BLACK);
			outtextxy(WIDTH / 2 - 198 / 2 - 50, HIGH * 2 / 8 - 25, _T("自动保存:"));
			if (create_mid_button(WIDTH / 2 - 198 / 2, HIGH * 2 / 8, &middle_botton, &middle_botton_f, &middle_botton_s, _T(sutoSaveBottonText), textColour, &m, desc))
			{
				m.mkLButton = false;
				gameSet->autoSave = !(gameSet->autoSave);
			}
		}

		//	是否生成怪物
		{
			char spawnMobBottonText[64];
			LPCSTR desc;
			if (gameSet->doMobSpawn) {
				strcpy_s(spawnMobBottonText, "开启");
				textColour = DARK_GREEN;
				desc = _T("是否在每关游戏时在地下添加一些生物，当前为开启，点击关闭");
			}
			else {
				strcpy_s(spawnMobBottonText, "关闭");
				textColour = RED;
				desc = _T("是否在每关游戏时在地下添加一些生物，当前为关闭，点击开启");
			}
			settextcolor(BLACK);
			outtextxy(WIDTH / 2 - 198 / 2 - 50, HIGH * 3 / 8 - 25, _T("生成生物:"));
			if (create_mid_button(WIDTH / 2 - 198 / 2, HIGH * 3 / 8, &middle_botton, &middle_botton_f, &middle_botton_s, _T(spawnMobBottonText), textColour, &m, desc))
			{
				m.mkLButton = false;
				gameSet->doMobSpawn = !(gameSet->doMobSpawn);
			}
		}

		settextcolor(BLACK);
		//	重设快捷键(钩子伸长)
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
			desc = _T("点击重设钩子伸长的快捷键");
			outtextxy(WIDTH / 5 - 198 / 2 - 50, HIGH * 5 / 8 - 25, _T("钩子伸长快捷键"));
			if (create_mid_button(WIDTH / 5 - 198 / 2, HIGH * 5 / 8, &middle_botton, &middle_botton_f, &middle_botton_s, _T(title), BLACK, &m, desc))
			{
				gameSet->KB_enlongHook = _getch();
			}
		}

		//	重设快捷键(暂停)
		{
			LPCSTR desc;
			char title[2] = { gameSet->KB_pauseGame, '\0' };
			desc = _T("点击重设暂停的快捷键");
			outtextxy(WIDTH * 3 / 5 - 198 / 2 - 50, HIGH * 5 / 8 - 25, _T("暂停快捷键"));
			if (create_mid_button(WIDTH * 3 / 5 - 198 / 2, HIGH * 5 / 8, &middle_botton, &middle_botton_f, &middle_botton_s, _T(title), BLACK, &m, desc))
			{
				gameSet->KB_pauseGame = _getch();
			}
		}

		//	重设快捷键(退出)
		{
			LPCSTR desc;
			char title[2] = { gameSet->KB_quitGame, '\0' };
			desc = _T("点击重设退出的快捷键");
			outtextxy(WIDTH * 1 / 5 - 198 / 2 - 50, HIGH * 6 / 8 - 25, _T("退出快捷键"));
			if (create_mid_button(WIDTH * 1 / 5 - 198 / 2, HIGH * 6 / 8, &middle_botton, &middle_botton_f, &middle_botton_s, _T(title), BLACK, &m, desc))
			{
				gameSet->KB_quitGame = _getch();
			}
		}

		//	重设快捷键(保存)
		{
			LPCSTR desc;
			char title[2] = { gameSet->KB_saveGame, '\0' };
			desc = _T("点击重设保存的快捷键");
			outtextxy(WIDTH * 3 / 5 - 198 / 2 - 50, HIGH * 6 / 8 - 25, _T("保存捷键"));
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
	//   伪全局变量  //
	/////////////////

	//	矿物相关
	MINE* m_head = NULL;

	//	道具相关
	ITEMLIST i_head;
	i_head.gameItemHead = NULL;
	i_head.itemPicHead = NULL;
	i_head.loadItemHead = NULL;


	//	钩子相关
	HOOK* thisHook = (HOOK*)malloc(sizeof(HOOK));
	thisHook->angle = 1;
	thisHook->hookConductance = 0;
	thisHook->hookLong = 50;
	thisHook->rollTemp = left;
	thisHook->hookNowState = hang;
	thisHook->startPointHook_X = 475;
	thisHook->startPointHook_Y = 50;
	thisHook->func = 0;

	//	生物
	MOBHEAD mobHead;
	mobHead.mobHead = NULL;
	mobHead.mobIdHead = NULL;
	mobHead.mobPicHead = NULL;

	///////////////
	//  游戏函数  //
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

	//	过关图片 584*445
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

		//	返回主菜单
		if (create_mid_button(275 + 10 + 198, 380, &middle_botton, &middle_botton_f, &middle_botton_s, _T("返回主菜单"), WHITE, &m)) {
			_return = false;
			break;
		}

		//	进入下一关
		if (create_mid_button(275, 380, &middle_botton, &middle_botton_f, &middle_botton_s, _T("进入下一关"), WHITE, &m)) {
			_return = true;
			break;
		}

		FlushBatchDraw();
	}

	return _return;
}

//	绘制主界面
void drawMainPic(const SETTING * gameSet, IMAGE* middle_botton, IMAGE* middle_botton_f, IMAGE* middle_botton_s) {
	cleardevice();
	
	char picURLtemp[64];

	//	背景图片
	IMAGE bookmark1_bg;
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\bookmarks1bg.jpg", gameSet->resourcepacksName);
	loadimage(&bookmark1_bg, _T(picURLtemp));
	putimage(0, 0, &bookmark1_bg);
	setbkmode(TRANSPARENT);

	//	“黄金矿工”四个大字
	IMAGE title_front;
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\title_light.jpg", gameSet->resourcepacksName);
	loadimage(&title_front, _T(picURLtemp));
	IMAGE title_bg;
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\title_dark.jpg", gameSet->resourcepacksName);
	loadimage(&title_bg, _T(picURLtemp));
	putimage(70, 30, &title_bg, NOTSRCERASE);
	putimage(70, 30, &title_front, SRCINVERT);

	//	背景音乐
	mciSendString("open \".\\music\\Minecraft.mp3\" alias bkmusic", NULL, 0, NULL);
	mciSendString("play bkmusic repeat", NULL, 0, NULL);

	//	显示信息
	version_info(gameSet);

	//	按钮
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\midgets.png", gameSet->resourcepacksName);
	loadimage(middle_botton, _T(picURLtemp));
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\midgets_f.png", gameSet->resourcepacksName);
	loadimage(middle_botton_f, _T(picURLtemp));
	sprintf_s(picURLtemp, ".\\textures\\%s\\gui\\midgets_s.png", gameSet->resourcepacksName);
	loadimage(middle_botton_s, _T(picURLtemp));
}