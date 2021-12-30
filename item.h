#pragma once

extern struct _node;
extern struct _hook;
extern struct _error;
extern struct _settings;

//	���߽ṹ��
enum itemState { i_available, i_hooked };
typedef struct _item {

	//	��Ϸ�е���Ҫ
	int kind;						//����
	enum itemState state;			//״̬���Ƿ񱻹���
	int x;	int y;					//��������

	struct _item* next;
} ITEM ;

typedef struct _item_node {

	//	���ص���Ҫ
	int kind;
	char imageURL[64];
	int hasMask;

	struct _item_node* next;
} ITEM_N;

typedef struct item_pic {
	int index;
	IMAGE itemPic;
	IMAGE itemPicMask;

	struct item_pic* next;
} ITEM_P ;

typedef struct item_head {
	ITEM* gameItemHead;
	ITEM_N* loadItemHead;
	ITEM_P* itemPicHead;
} ITEMLIST ;

void load_item(ITEMLIST* i_head, struct _error* thisErr,const struct _settings*);
void showItem(const ITEMLIST * i_head, struct _error* thisErr);
void useItem(ITEMLIST* i_head, ITEM* thisItem, struct _node* m_head, struct _hook* thisHook, struct _error*, const struct _settings* gameSet);