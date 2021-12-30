#include <stdio.h>
#include "gui.h"
#include "functionsmain.h"

//	按钮类
bool create_long_button(const int x, const int y, const IMAGE* pic, IMAGE* pic_f, const IMAGE* pic_s, LPCTSTR str, COLORREF colour, const MOUSEMSG* m, LPCSTR desc) {
	bool inside_return = false;

	//	长按钮长396，宽36
	if ((m->x > x) && (m->x < 396 + x) && (m->y > y) && (m->y < y + 36)) {
		if (m->mkLButton == 1) {
			inside_return = true;
			putimage(x, y, pic_s);
			Sleep(20);
		}
		else {
			inside_return = false;
			putimage(x, y, pic_f);
		}
	}
	else {
		inside_return = false;
		putimage(x, y, pic);
	}

	//	显示按钮上的文字
	settextcolor(colour);
	settextstyle(23, 0, _T("微软雅黑"));
	int text_width = textwidth(str);
	outtextxy(x + 198 - text_width / 2, y + 7, str);

	//	显示瞄准文字
	if ((m->x > x) && (m->x < 396 + x) && (m->y > y) && (m->y < y + 36) && desc) {
		descWindow(desc, m);
	}

	return inside_return;	//	和if联动，用于按钮功能的实现
}
bool create_mid_button(const int x, const int y, const IMAGE* pic, IMAGE* pic_f, const IMAGE* pic_s, LPCTSTR str, COLORREF colour, const MOUSEMSG* m, LPCSTR desc) {
	bool inside_return = false;

	//	按钮长198，宽36
	if ((m->x > x) && (m->x < 198 + x) && (m->y > y) && (m->y < y + 36)) {
		if (m->mkLButton == 1) {
			inside_return = true;
			putimage(x, y, pic_s);
			Sleep(20);
		}
		else {
			inside_return = false;
			putimage(x, y, pic_f);
		}
	}
	else {
		inside_return = false;
		putimage(x, y, pic);
	}

	//	显示按钮上的文字
	settextcolor(colour);
	settextstyle(23, 0, _T("微软雅黑"));
	int text_width = textwidth(str);
	outtextxy(x + 96 - text_width / 2, y + 7, str);

	//	显示瞄准文字
	if ((m->x > x) && (m->x < 198 + x) && (m->y > y) && (m->y < y + 36) && desc) {
		descWindow(desc, m);
	}

	return inside_return;	//	和if联动，用于按钮功能的实现
}
bool createSmallButton(int x, int y, IMAGE* pic, IMAGE* pic_f, IMAGE* pic_s, LPCTSTR str, COLORREF colour, const MOUSEMSG* m, LPCSTR desc) {
	int inside_return = false;

	putimage(x, y, pic);
	if ((m->x > x) && (m->x < 160 + x) && (m->y > y) && (m->y < y + 36)) {
		if (m->mkLButton == 1)
		{
			putimage(x, y, pic_s);
			inside_return = true;
		}
		else {
			putimage(x, y, pic_f);
			inside_return = false;
		}
	}
	else {
		putimage(x, y, pic);
		inside_return = false;
	}

	settextstyle(23, 0, _T("微软雅黑"));
	settextcolor(colour);
	int text_width = textwidth(str);
	outtextxy(x + 80 - text_width / 2, y + 7, str);

	//	显示瞄准文字
	if ((m->x > x) && (m->x < 160 + x) && (m->y > y) && (m->y < y + 36) && desc) {
		descWindow(desc, m);
	}

	return inside_return;
}
bool create_portal_button(const int x, const int y, const IMAGE* pic, IMAGE* pic_f, const IMAGE* pic_s, const MOUSEMSG* m, LPCSTR desc) {
	bool inside_return = false;

	//	按钮长32，宽32
	if ((m->x > x) && (m->x < 32 + x) && (m->y > y) && (m->y < y + 32)) {
		if (m->mkLButton == 1) {
			inside_return = true;
			putimage(x, y, pic_s);
			Sleep(20);
		}
		else {
			inside_return = false;
			putimage(x, y, pic_f);
		}
	}
	else {
		inside_return = false;
		putimage(x, y, pic);
	}

	//	显示瞄准文字
	if ((m->x > x) && (m->x < 32 + x) && (m->y > y) && (m->y < y + 32) && desc) {
		descWindow(desc, m);
	}

	return inside_return;	//	和if联动，用于按钮功能的实现
}

//	信息显示类
void descWindow(LPCSTR str_l, const MOUSEMSG* m) {
	int lenthStr = _tcslen(str_l);

	setcolor(BLACK);
	rectangle(m->x, m->y, m->x + lenthStr*9 + 10, m->y + 25 + 10);
	outtextxy(m->x + 5, m->y + 5, str_l);
}

//	错误类
void printErrToFile(const ERR* err) {
	FILE* fp = NULL;
	if ((fopen_s(&fp, "err.log", "a") == 0) && fp) {
		fprintf_s(fp, "[错误]\t%s\t|||\t\t错误代码为 %d\n", err->info, (int)(err->code));
		fclose(fp);
	}
	else {
		printf("[错误]\t错误文件打开失败\t|||\t\t错误代码为 %d\n", (int)errFileOpenFailed);
		exit((int)errFileOpenFailed);
	}
}
void printMsgToFile(const ERR* err) {
	FILE* fp = NULL;
	if ((fopen_s(&fp, "err.log", "a") == 0) && fp) {
		fprintf_s(fp, "[消息]\t%s\n", err->info);
		fclose(fp);
	}
	else {
		printf("[错误]\t错误文件打开失败\t|||\t\t错误代码为 %d\n", (int)errFileOpenFailed);
		exit((int)errFileOpenFailed);
	}
}