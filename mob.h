#pragma once

extern struct _node;
extern struct _item;
extern struct _hook;
extern struct _error;
extern struct _settings;
extern struct _user;

//	����ṹ��
typedef struct _mob {
	int id;					//���������
	bool state;				//״̬�����Ƿ񱻹���
	int data;				//�ڲ�����

	int x;					//������
	int y;					//������

	struct _mob* next;
} MOB;

//	���������id
typedef struct _mob_id {
	int id;
	char imageURL[64];

	struct _mob_id* next;
} MOB_ID;

//	���������ͼƬ
typedef struct _mob_pic {
	int index;
	IMAGE mobPic;
	IMAGE mobPicMask;

	struct _mob_pic* next;
} MOB_P;

//	���е�ͷָ��
typedef struct _mob_head {
	MOB* mobHead;
	MOB_P* mobPicHead;
	MOB_ID* mobIdHead;
} MOBHEAD;

void load_mob(MOBHEAD* mobHead, struct _error* thisErr, const struct _settings*);
void show_mob(const MOBHEAD* mobHead, struct _error* thisErr);
void use_mob(struct _user* player, MOBHEAD* mobHead, struct _node* m_head, struct _item* i_head, struct _error*, const struct _settings* gameSet);