#pragma once

////////////////
//   �ṹ����  //
////////////////

//	����ṹ��
enum mineKind { stone, gold, diamond };
enum mineState { available, croked };
typedef struct _node {
	enum mineKind kind;				//	kind��ʾ�Ϳ�������
	enum mineState state;			//	state��ʾ״̬
	int Pos_x;	int Pos_y;			//	xy�Ǻ�������
	struct _node* next;
} MINE;

//	�ؿ��ṹ��
typedef struct _level {
	int currentLevel;				//	��ǰ�ؿ�
	int gameScore;					//	��ǰ�÷�
	int timeLast;					//	ʣ��ʱ��
	int passScore;					//	ͨ������Ҫ�ĵ÷�
} LEVEL ;

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
} CROK ;


////////////////
//   ��������  //
////////////////
MINE* startup(MINE* head, LEVEL*);

void show(MINE*, LEVEL*);

void updateWithoutImput(MINE*, LEVEL*, CROK*);

void updateWithImput(CROK*);