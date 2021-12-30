#pragma once

extern struct _node;
extern struct _item;
extern struct _hook;
extern struct _error;
extern struct _settings;
extern struct _user;

//	生物结构体
typedef struct _mob {
	int id;					//矿物的种类
	bool state;				//状态，即是否被勾上
	int data;				//内部数据

	int x;					//横坐标
	int y;					//纵坐标

	struct _mob* next;
} MOB;

//	所有生物的id
typedef struct _mob_id {
	int id;
	char imageURL[64];

	struct _mob_id* next;
} MOB_ID;

//	所有生物的图片
typedef struct _mob_pic {
	int index;
	IMAGE mobPic;
	IMAGE mobPicMask;

	struct _mob_pic* next;
} MOB_P;

//	所有的头指针
typedef struct _mob_head {
	MOB* mobHead;
	MOB_P* mobPicHead;
	MOB_ID* mobIdHead;
} MOBHEAD;

void load_mob(MOBHEAD* mobHead, struct _error* thisErr, const struct _settings*);
void show_mob(const MOBHEAD* mobHead, struct _error* thisErr);
void use_mob(struct _user* player, MOBHEAD* mobHead, struct _node* m_head, struct _item* i_head, struct _error*, const struct _settings* gameSet);