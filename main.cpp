/*
	�Ի�ҩ�ĵ��ߺʹ�����ĵ���
	���ý�������
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
#include "bookmarks.h"
#include "functionsmain.h"
#pragma comment(lib,"WINMM.LIB")

int main(int agrc, char* argv[]) {

	/////////////////
	//   αȫ�ֱ���  //
	/////////////////
	PLAYER player;

	ERR error;
	error.code = normal;

	SETTING gameSet;
	gameSet.putItems = true;
	strcpy_s(gameSet.resourcepacksName, "Sphax");
	gameSet.autoSave = false;							//	Ĭ�ϲ��Զ�����
	gameSet.doMobSpawn = true;
	gameSet.KB_enlongHook = ' ';
	gameSet.KB_pauseGame = 'p';
	gameSet.KB_quitGame = 'q';
	gameSet.KB_saveGame = 's';

	gameSet.debugMode = false;

	///////////////
	//  ��Ϸ����  //
	///////////////
	bookmarkOpening(&player, &error, &gameSet);
	bookmarkMain(&player, &error, &gameSet);
	
	return (int)(error.code);
}

//	�汾��Ϣ��ʾ
void version_info(const SETTING * set) {
	char rsPackBuff[64];
	sprintf_s(rsPackBuff, " Resource pack = %s", set->resourcepacksName);
	
	//	�����ʽ
	settextcolor(BLACK);
	settextstyle(15, 0, _T("����"));

	//	��Ϣ��ʾ
	if (set->debugMode) { outtextxy(0, 17 * 0, _T(" Version��1.3.0_debug")); }
	else { outtextxy(0, 17 * 0, _T(" Version��1.2.2 Release")); }
	outtextxy(0, 17 * 1, _T(" 0 DLCs loaded"));
	outtextxy(0, 17 * 2, _T(rsPackBuff));
	outtextxy(0, 17 * 3, _T(" Made by 20080216HJQ"));
}