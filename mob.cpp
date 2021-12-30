#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include "functionsmain.h"
#include "gui.h"
#include "mob.h"

#define HIGH 505
#define WIDTH 960
#define GRASS_HIGHT 91
#define MOB_NUM 3

void setMobKind(MOB* thisMob, const int numMob);

void load_mob(MOBHEAD* mobHead, ERR* thisErr, const SETTING* gameSet) {
	char picURLtemp[64];

	//	װ���ı��ļ�
	int mobListNum = 0;
	FILE* fp;
	if ((fopen_s(&fp, ".\\data\\mob\\list.hjq", "r") == 0) && fp) {
		rewind(fp);
		while (!feof(fp)) {
			MOB_ID* p = (MOB_ID*)malloc(sizeof(MOB_ID));
			if (p == NULL) {
				thisErr->code = lowHeapMemory;
				thisErr->info = _T("mob��id�������malloc�ڴ����ʧ��");
				printErrToFile(thisErr);
			}
			else {
				char temp3[64];
				fgets(temp3, 24, fp);
				p->id = (int)(temp3[0] - '0');
				{
					int i = 0;
					for (i = 2; temp3[i] != '\n'; i++)
					{
						p->imageURL[i - 2] = temp3[i];
					}
					p->imageURL[i - 2] = '\0';
				}
				p->next = NULL;
				mobListNum++;

				MOB_ID* last = mobHead->mobIdHead;
				if (last != NULL) {
					while (last->next != NULL) {
						last = last->next;
					}
					last->next = p;
				}
				else {
					mobHead->mobIdHead = p;
				}
			}
		}
		fclose(fp);
		
		char msgBuff[64];
		sprintf_s(msgBuff, "mob��id�Ѿ�������ϣ�һ������%d��mob", mobListNum - 2);
		thisErr->info = _T(msgBuff);
		printMsgToFile(thisErr);
	}
	else {
		thisErr->code = fileOpenFailed;
		thisErr->info = _T("mob��id�ļ���ʧ��");
		printErrToFile(thisErr);
	}

	//	��ʼ�������
	for (MOB_ID* j = mobHead->mobIdHead; j; j = j->next)
	{
		if (j->id) {
			//�������� - ����ģ�malloc��������IMAGE���͵�constructor����ط�ֻ����new�����ߤޤ���!
			//MOB_P* p = (MOB_P*)malloc(sizeof(MOB_P));
			MOB_P* p = new MOB_P();

			p->index = j->id;
			char picBuffer[64];
			sprintf_s(picBuffer, ".\\data\\mob\\textures\\%s\\mob\\%s.png", gameSet->resourcepacksName, j->imageURL);
			loadimage(&(p->mobPic), _T(picBuffer));

			char picBufferMask[64];
			sprintf_s(picBufferMask, ".\\data\\mob\\textures\\%s\\mob\\%s_mask.bmp", gameSet->resourcepacksName, j->imageURL);
			loadimage(&(p->mobPicMask), _T(picBufferMask));
			p->next = NULL;

			MOB_P* last = mobHead->mobPicHead;
			if (last) {
				while (last->next) {
					last = last->next;
				}
				last->next = p;
			}
			else {
				mobHead->mobPicHead = p;
			}
		}
	}

	//	ԭ����
	for (int i = 0; i < MOB_NUM; i++)
	{
		if (rand() % 100 < 50) {
			MOB* p = (MOB*)malloc(sizeof(MOB));
			if (p == NULL)
			{
				thisErr->code = lowHeapMemory;
				thisErr->info = _T("�ڹ��������ʱmalloc�ռ����ʧ��");
				printErrToFile(thisErr);
			}
			else {
				setMobKind(p, mobListNum);
				p->state = true;
				p->next = NULL;
				p->data = 0;
			}

			MOB* last = mobHead->mobHead;
			if (last) {
				while (last->next) {
					last = last->next;
				}
				last->next = p;
			}
			else {
				mobHead->mobHead = p;
			}
		}
	}
}

void show_mob(const MOBHEAD* mobHead, ERR* thisErr) {
	for (MOB* i = mobHead->mobHead; i; i = i->next)
	{
		if (i->state) {
			for (MOB_P* j = mobHead->mobPicHead; j; j = j->next)
			{
				if (j->index == i->id) {
					putimage(i->x, i->y, &(j->mobPicMask), NOTSRCERASE);
					putimage(i->x, i->y, &(j->mobPic), SRCINVERT);
				}
			}
		}
	}
}

void use_mob(PLAYER* player, MOBHEAD* mobHead, MINE* m_head,  ITEM* i_head, ERR* errorCode, const SETTING* gameSet) {
	for (MOB* i = mobHead->mobHead; i; i = i->next)
	{
		switch (i->id)
		{
			//	�˴�Ϊ���������
#include "data/mob/mobRun.hjq"

		default:
			break;
		}
	}
}

void setMobKind(MOB* thisMob, const int numMob) {
	int generateWhat = rand() % (numMob - 2);
	thisMob->id = (generateWhat + 1);

	thisMob->x = WIDTH - 96;
	thisMob->y = rand() % (HIGH - GRASS_HIGHT - 32) + GRASS_HIGHT;
}