#pragma once

#include <graphics.h>
#include <conio.h>
#include "functionsmain.h"

#define DARK_GREEN RGB(11, 70, 10)

//	错误枚举体
enum e_errorCode {
	normal,							//	无错误，正常情况
	exitByPlayer,					//	由玩家退出
	lowHeapMemory,					//	堆内内存不足，无法分配空间
	fileOpenFailed,					//	文件打开失败
	doNotHaveUsername,				//	用户名为空
	errFileOpenFailed				//	错误日志打开失败
};
typedef struct _error {
	e_errorCode code;				//	错误类型
	LPCSTR info;					//	详细描述，比如错误发生的位置
} ERR;

bool create_long_button(const int x, const int y, const IMAGE* pic, IMAGE* pic_f, const IMAGE* pic_s, LPCTSTR str, COLORREF colour, const MOUSEMSG* m, LPCSTR desc = NULL);
bool create_mid_button(const int x, const int y, const IMAGE* pic, IMAGE* pic_f, const IMAGE* pic_s, LPCTSTR str, COLORREF colour, const MOUSEMSG* m, LPCSTR desc = NULL);
bool createSmallButton(int x, int y, IMAGE* pic, IMAGE* pic_f, IMAGE* pic_s, LPCTSTR str, COLORREF colour, const MOUSEMSG* m, LPCSTR desc = NULL);
bool create_portal_button(const int x, const int y, const IMAGE* pic, IMAGE* pic_f, const IMAGE* pic_s, const MOUSEMSG* m, LPCSTR desc);

void descWindow(LPCSTR str_l, const MOUSEMSG* m);

void printErrToFile(const ERR* err);
void printMsgToFile(const ERR* err);