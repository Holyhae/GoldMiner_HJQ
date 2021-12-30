#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include "functionsmain.h"
#include "item.h"
#include "gui.h"

#define HIGH 505
#define WIDTH 960
#define GRASS_HIGHT 91
#define MAX_ITEM_NUM 5

void setItemKind(ITEM* thisItem, const int numItem);

void load_item(ITEMLIST* i_head, ERR* thisErr, const SETTING* gameSet) {
	char picURLtemp[64];

	//	װ���ı��ļ�
	int itemListNum = 0;
	FILE* fp;
	if ((fopen_s(&fp, ".\\data\\item\\list.hjq", "r") == 0) && fp) {
		rewind(fp);
		while (!feof(fp)) {
			ITEM_N* p = (ITEM_N*)malloc(sizeof(ITEM_N));
			if (p == NULL) {
				thisErr->code = lowHeapMemory;
				thisErr->info = _T("��ȡ����idʱmalloc����ʧ��");
				printErrToFile(thisErr);
			}
			else {
				char temp3[64];
				fgets(temp3, 24, fp);
				p->kind = (int)(temp3[0] - '0');
				p->hasMask = (int)(temp3[2] - '0');
				{
					int i = 0;
					for (i = 4; temp3[i] != '\n'; i++)
					{
						p->imageURL[i - 4] = temp3[i];
					}
					p->imageURL[i - 4] = '\0';
				}
				p->next = NULL;
				itemListNum++;

				ITEM_N* last = i_head->loadItemHead;
				if (last != NULL) {
					while (last->next != NULL) {
						last = last->next;
					}
					last->next = p;
				}
				else {
					i_head->loadItemHead = p;
				}
			}
		}
		fclose(fp);

		char msgBuff[64];
		sprintf_s(msgBuff, "����id�Ѿ�������ϣ�һ������%d������", itemListNum - 2);
		thisErr->info = _T(msgBuff);
		printMsgToFile(thisErr);
	}
	else {
		thisErr->code = fileOpenFailed;
		thisErr->info = _T("����id�ļ���ʧ��");
		printErrToFile(thisErr);
	}

	//	��ʼ�������
	for (ITEM_N* j = i_head->loadItemHead; j; j = j->next)
	{
		if (j->kind) {
			//�������� - ����ģ�malloc��������IMAGE���͵�constructor����ط�ֻ����new�����ߤޤ���!
			//ITEM_P* p = (ITEM_P*)malloc(sizeof(ITEM_P));
			ITEM_P* p = new ITEM_P();

			p->index = j->kind;
			char picBuffer[64];
			sprintf_s(picBuffer, ".\\data\\item\\textures\\%s\\item\\%s.png", gameSet->resourcepacksName, j->imageURL);
			loadimage(&(p->itemPic), _T(picBuffer));

			char picBufferMask[64];
			sprintf_s(picBufferMask, ".\\data\\item\\textures\\%s\\item\\%s_mask.bmp", gameSet->resourcepacksName, j->imageURL);
			loadimage(&(p->itemPicMask), _T(picBufferMask));
			p->next = NULL;

			ITEM_P* last = i_head->itemPicHead;
			if (last) {
				while (last->next) {
					last = last->next;
				}
				last->next = p;
			}
			else {
				i_head->itemPicHead = p;
			}
		}
	}

	//	ԭ����
	for (int i = 0; i < MAX_ITEM_NUM; i++)
	{
		if (rand() % 100 < 15) {
		//if (rand() % 100 < 100) {
			ITEM* p = (ITEM*)malloc(sizeof(ITEM));
			if (p == NULL)
			{
				thisErr->code = lowHeapMemory;
				thisErr->info = _T("�ؿ��ڵ��߼���ʧ�ܣ�malloc�ռ����ʧ��");
				printErrToFile(thisErr);
			}
			else {
				setItemKind(p, itemListNum);
				p->state = i_available;
				p->next = NULL;
			}

			ITEM* last = i_head->gameItemHead;
			if (last) {
				while (last->next) {
					last = last->next;
				}
				last->next = p;
			}
			else {
				i_head->gameItemHead = p;
			}
		}
	}
}

void showItem(const ITEMLIST * i_head, ERR* thisErr) {
	for (ITEM* i = i_head->gameItemHead; i; i = i->next)
	{
		if (i->state == i_available) {
			for (ITEM_P* j = i_head->itemPicHead; j; j = j->next)
			{
				if (j->index == i->kind) {
					putimage(i->x, i->y, &(j->itemPicMask), NOTSRCERASE);
					putimage(i->x, i->y, &(j->itemPic), SRCINVERT);
				}
			}
		}
	}
}

void useItem(ITEMLIST* i_head, ITEM* thisItem, MINE* m_head, HOOK* thisHook, ERR* thisErr, const SETTING * gameSet) {
	IMAGE boomImage[2];
	if (((thisHook->endPointX) + 16 >= thisItem->x) && ((thisHook->endPointX) - 16 <= (thisItem->x) + 32) && ((thisHook->endPointY) + 10 >= thisItem->y) && ((thisHook->endPointY) <= (thisItem->y) + 32) && (thisItem->state == i_available)) {
		switch (thisItem->kind)
		{
		case 0:
			thisHook->func = 0;
			break;
		//	�˴�Ϊ���������
#include "data/item/when_hooked.hjq"

		default:
			if (((thisHook->endPointX) >= thisItem->x) && ((thisHook->endPointX) <= (thisItem->x) + 32) && ((thisHook->endPointY) >= thisItem->y) && ((thisHook->endPointY) <= (thisItem->y) + 32) && (thisItem->state == i_available)) {
				thisItem->x = (thisHook->endPointX) - 16;
				thisItem->y = (thisHook->endPointY) - 10;
				thisHook->hookConductance = 8;
				thisHook->hookNowState = back;
			}
			if ((thisItem->state == i_available) && (thisItem->y < GRASS_HIGHT)) {
				thisItem->state = i_hooked;
				thisHook->hookConductance = 10;
				thisHook->func = thisItem->kind;
			}
			break;

		}
	}
}

void setItemKind(ITEM* thisItem, const int numItem) {
	int generateWhat = rand() % (numItem - 2);
	thisItem->kind = (generateWhat + 1);

	thisItem->x = rand() % (WIDTH - 32);
	thisItem->y = rand() % (HIGH - GRASS_HIGHT - 32) + GRASS_HIGHT;
}