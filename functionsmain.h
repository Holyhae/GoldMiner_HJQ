#pragma once

////////////////
//   �ṹ����  //
////////////////

//	����ö����
enum e_errorCode {
	normal,
	exitByPlayer,
	lowHeapMemory,
	fileOpenFailed,
	doNotHaveUsername
};

//	����ṹ��
//	�������͵�ö������ֵ����ÿ���ļ�ֵ
enum mineKind { stone = 1, iron = 5, gold = 15, diamond = 20, emerald = 30};
enum mineState { available, croked };
typedef struct _node {
	enum mineKind kind;				//	kind��ʾ�Ϳ�������
	enum mineState state;			//	state��ʾ״̬
	int size;						//	����ߴ��С
	int Pos_x;	int Pos_y;			//	xy�Ǻ�������
	struct _node* next;
} MINE;

//	�ؿ��ṹ��
typedef struct _level {
	int currentLevel;				//	��ǰ�ؿ�
	int gameScore;					//	��ǰ�÷�
	int timeLast;					//	ʣ��ʱ��
	int passScore;					//	ͨ������Ҫ�ĵ÷�
} LEVEL;

typedef struct _level_node {
	LEVEL currentlevel;
	struct _level_node* next;
} LEVEL_N;

//	���ӽṹ��
enum rollTempKind { right, left };
enum crokState { hang, extension, back };
typedef struct _crok {
	int crokLong;					//	��ǰ���ӳ�
	float angle;					//	��ǰ�Ƕ�
	int endPointX;					//	�����������
	int endPointY;					//	������������
	rollTempKind rollTemp;			//	����ڻ������Ұڣ�
	int crokConductance;			//	���������������ӹ�ס����֮��������ٶ�
	crokState crokNowState;			//	��ǰ����������ת�������쳤�����ǻ�����

	int startPointCrok_X;			//	Ԥ���ӿ�
	int startPointCrok_Y;			//	Ԥ���ӿ�
} CROK;

//	�û��ṹ��
typedef struct _user {
	char userName[10];
	LEVEL userLevel;
} PLAYER;


////////////////
//   ��������  //
////////////////
MINE* startup(MINE* head, LEVEL*, PLAYER*, e_errorCode*);

void show(MINE*, LEVEL*, PLAYER*, e_errorCode*);

void updateWithoutImput(MINE*, LEVEL*, CROK*, LEVEL_N*, PLAYER*, e_errorCode*);

void updateWithImput(CROK*, PLAYER*, LEVEL*, e_errorCode*);