/*
	ؽ����������⣺
	1.���а��ȡ����
	2.����
	
	////////////////////////////////
	////////////////////////////////

	���Ը��µ�����
	1.����
*/

#include <conio.h>
#include <graphics.h>
#include "bookmarks.h"
#include "functionsmain.h"

/*
	errorCode:
	0	����˳�
	1	�ڴ治�㣨malloc�ѣ�
*/

int main(void) {

	/////////////////
	//   αȫ�ֱ���  //
	/////////////////
	PLAYER player;
	LEVEL_N* l_head = NULL;
	e_errorCode errorCode = normal;

	///////////////
	//  ��Ϸ����  //
	///////////////
	bookmarkOpening(&player, &errorCode);
	l_head = bookmarkGenerator(l_head, &(player.userLevel.currentLevel), &errorCode);
	bookmarkMain(&(player.userLevel.currentLevel), l_head, &player, &errorCode);
	gameOver(l_head);

	return errorCode;
}