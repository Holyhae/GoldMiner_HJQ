#include <conio.h>
#include <graphics.h>
#include <stdio.h>
#include "bookmarks.h"
#include "functionsmain.h"

#define HIGH 505
#define WIDTH 960

extern struct _node;
extern struct _level;
extern struct _level_node;
extern struct _crok;
extern struct _user;
extern enum e_errorCode;

int create_long_button(const int x, const int y, const IMAGE* pic, const IMAGE* pic_s, LPCTSTR str, COLORREF colour, const MOUSEMSG* m);

void bookmarkOpening(PLAYER* player, e_errorCode* errorCode) {
	closegraph();
	initgraph(WIDTH, HIGH);

	IMAGE bookmark1_bg;
	loadimage(&bookmark1_bg, _T(".\\textures\\gui\\bookmarks1bg.jpg"));
	putimage(0, 0, &bookmark1_bg);
	setbkmode(TRANSPARENT);

	IMAGE title_front;
	loadimage(&title_front, _T(".\\textures\\gui\\title_light.jpg"));
	IMAGE title_bg;
	loadimage(&title_bg, _T(".\\textures\\gui\\title_dark.jpg"));
	putimage(70, 30, &title_bg, NOTSRCERASE);
	putimage(70, 30, &title_front, SRCINVERT);

	IMAGE wide_botton;
	loadimage(&wide_botton, _T(".\\textures\\gui\\widgets.png"));

	IMAGE wide_botton_s;
	loadimage(&wide_botton_s, _T(".\\textures\\gui\\widgets_s.png"));

	settextstyle(20, 10, _T("宋体"));
	settextcolor(BLACK);

	do {
		MOUSEMSG m_bookmark1;
		if (MouseHit()) {
			m_bookmark1 = GetMouseMsg();

			//	生产按钮的专用代码
			//	新游戏
			if (create_long_button(275, 330, &wide_botton, &wide_botton_s, _T("开始新游戏"), BLACK, &m_bookmark1)) {
				(*player).userLevel.currentLevel = 1;
				InputBox((*player).userName, 10, "请输入用户名");
				if ((*player).userName[0] == '\0') {
					*errorCode = doNotHaveUsername;
					exit((int)*errorCode);
				}
				break;
			}
			//	清空本地数据
			if (create_long_button(275, 390, &wide_botton, &wide_botton_s, _T("清空存档数据"), BLACK, &m_bookmark1)) {
				FILE* fp = NULL;
				fopen_s(&fp, "userData.hjq", "w");
				if (fp != NULL) {
					fclose(fp);
				}
				else {
					*errorCode = fileOpenFailed;
					exit((int)*errorCode);
				}
			}
			//	退出游戏
			if (create_long_button(275, 450, &wide_botton, &wide_botton_s, _T("退出"), BLACK, &m_bookmark1)) {
				closegraph();
				*errorCode = exitByPlayer;
				exit((int)*errorCode);
			}
		}
	} while (1);
}

struct _level_node* bookmarkGenerator(struct _level_node* l_head, int* startLevel, e_errorCode* errorCode) {
	for (int i = 0; i < (*startLevel + 1); i++) {
		LEVEL_N* p = (LEVEL_N*)malloc(sizeof(LEVEL_N));
		if (p == NULL) {
			*errorCode = lowHeapMemory;
			exit((int)*errorCode);
		}
		else {
			(p->currentlevel).currentLevel = i + 1;
			p->next = NULL;
		}
		LEVEL_N* last = l_head;
		if (last) {
			while (last->next) {
				last = last->next;
			}
			last->next = p;
		}
		else {
			l_head = p;
		}
	}

	return l_head;
}

void bookmarkMain(int* expectedLevel, struct _level_node* l_head, PLAYER* player, e_errorCode* errorCode) {

	/////////////////
	//   伪全局变量  //
	/////////////////
	//	关卡相关
	LEVEL thisLevel;

	for (LEVEL_N* i = l_head; i; i = i->next) {
		if ((i->currentlevel).currentLevel == (*expectedLevel)) {
			thisLevel.currentLevel = (i->currentlevel).currentLevel;
		}
	}

	//	矿物相关
	MINE* m_head = NULL;

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
	m_head = startup(m_head, &thisLevel, player, errorCode);
	while (1) {
		show(m_head, &thisLevel, player, errorCode);
		updateWithoutImput(m_head, &thisLevel, &thisCrok, l_head, player, errorCode);
		updateWithImput(&thisCrok, player, &thisLevel, errorCode);

		FlushBatchDraw();
		Sleep(10);
	}
}

void gameOver(struct _level_node* l_head) {
	//	IO预留，立刻储存当前游戏进度
	printf("\n####################################\n");
	printf("\t这都打不完，你真是个傻逼\n");
	printf("####################################\n\n\n\n\n\n\n\n\n");
	exit(1);
	LEVEL_N* p = l_head;
	LEVEL_N* q = l_head;
	for (p = l_head; p; q = p, p = p->next)
	{
		free(q);
	}
}



//	生成长按钮的函数
int create_long_button(const int x, const int y, const IMAGE* pic, const IMAGE* pic_s, LPCTSTR str, COLORREF colour, const MOUSEMSG* m) {
	int inside_return = 0;
	//	长按钮长396，宽36
	if ((m->mkLButton == 1) && (m->x > x) && (m->x < 396 + x) && (m->y > y) && (m->y < y + 36)) {
		inside_return = 1;
		putimage(x, y, pic_s);
		settextcolor(colour);
		outtextxy(x, y + 7, str);
		Sleep(20);
	}
	else {
		inside_return = 0;
		putimage(x, y, pic);
		outtextxy(x, y + 7, str);
	}

	return inside_return;	//	和if联动，用于按钮功能的实现
}