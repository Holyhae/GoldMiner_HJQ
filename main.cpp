/*
	迷魂药的道具和大力丸的道具
	设置界面完善
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
	//   伪全局变量  //
	/////////////////
	PLAYER player;

	ERR error;
	error.code = normal;

	SETTING gameSet;
	gameSet.putItems = true;
	strcpy_s(gameSet.resourcepacksName, "Sphax");
	gameSet.autoSave = false;							//	默认不自动保存
	gameSet.doMobSpawn = true;
	gameSet.KB_enlongHook = ' ';
	gameSet.KB_pauseGame = 'p';
	gameSet.KB_quitGame = 'q';
	gameSet.KB_saveGame = 's';

	gameSet.debugMode = false;

	///////////////
	//  游戏函数  //
	///////////////
	bookmarkOpening(&player, &error, &gameSet);
	bookmarkMain(&player, &error, &gameSet);
	
	return (int)(error.code);
}

//	版本信息显示
void version_info(const SETTING * set) {
	char rsPackBuff[64];
	sprintf_s(rsPackBuff, " Resource pack = %s", set->resourcepacksName);
	
	//	字体格式
	settextcolor(BLACK);
	settextstyle(15, 0, _T("宋体"));

	//	信息显示
	if (set->debugMode) { outtextxy(0, 17 * 0, _T(" Version：1.3.0_debug")); }
	else { outtextxy(0, 17 * 0, _T(" Version：1.2.2 Release")); }
	outtextxy(0, 17 * 1, _T(" 0 DLCs loaded"));
	outtextxy(0, 17 * 2, _T(rsPackBuff));
	outtextxy(0, 17 * 3, _T(" Made by 20080216HJQ"));
}