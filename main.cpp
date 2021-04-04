/*
	亟待解决的问题：
	1.排行榜读取内容
	2.读档
	
	////////////////////////////////
	////////////////////////////////

	可以更新的内容
	1.道具
*/

#include <conio.h>
#include <graphics.h>
#include "bookmarks.h"
#include "functionsmain.h"

/*
	errorCode:
	0	玩家退出
	1	内存不足（malloc堆）
*/

int main(void) {

	/////////////////
	//   伪全局变量  //
	/////////////////
	PLAYER player;
	LEVEL_N* l_head = NULL;
	e_errorCode errorCode = normal;

	///////////////
	//  游戏函数  //
	///////////////
	bookmarkOpening(&player, &errorCode);
	l_head = bookmarkGenerator(l_head, &(player.userLevel.currentLevel), &errorCode);
	bookmarkMain(&(player.userLevel.currentLevel), l_head, &player, &errorCode);
	gameOver(l_head);

	return errorCode;
}