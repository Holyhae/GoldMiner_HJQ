#pragma once

#include <graphics.h>
#include <conio.h>
#include "functionsmain.h"

#define DARK_GREEN RGB(11, 70, 10)

//	����ö����
enum e_errorCode {
	normal,							//	�޴����������
	exitByPlayer,					//	������˳�
	lowHeapMemory,					//	�����ڴ治�㣬�޷�����ռ�
	fileOpenFailed,					//	�ļ���ʧ��
	doNotHaveUsername,				//	�û���Ϊ��
	errFileOpenFailed				//	������־��ʧ��
};
typedef struct _error {
	e_errorCode code;				//	��������
	LPCSTR info;					//	��ϸ�����������������λ��
} ERR;

bool create_long_button(const int x, const int y, const IMAGE* pic, IMAGE* pic_f, const IMAGE* pic_s, LPCTSTR str, COLORREF colour, const MOUSEMSG* m, LPCSTR desc = NULL);
bool create_mid_button(const int x, const int y, const IMAGE* pic, IMAGE* pic_f, const IMAGE* pic_s, LPCTSTR str, COLORREF colour, const MOUSEMSG* m, LPCSTR desc = NULL);
bool createSmallButton(int x, int y, IMAGE* pic, IMAGE* pic_f, IMAGE* pic_s, LPCTSTR str, COLORREF colour, const MOUSEMSG* m, LPCSTR desc = NULL);
bool create_portal_button(const int x, const int y, const IMAGE* pic, IMAGE* pic_f, const IMAGE* pic_s, const MOUSEMSG* m, LPCSTR desc);

void descWindow(LPCSTR str_l, const MOUSEMSG* m);

void printErrToFile(const ERR* err);
void printMsgToFile(const ERR* err);