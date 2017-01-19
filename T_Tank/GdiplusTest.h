#pragma once
#include "T_Engine.h"
#include "T_Graphic.h"
#include "T_Sprite.h"
#include "T_AI.h"
#include "T_Map.h"

typedef vector<T_Sprite*>vSpriteSet;


struct BlockSprite
{
	T_Graph image;			// ����֡ͼ
	int wFrame;				// ����֡ͼ��
	int hFrame;				// ����֡ͼ��
	int frameCount;			// ֡������ǰ���ŵ�֡����
	SPRITEINFO BlockInfo;	// ��ɫ��Ϣ
};

class GdiplusTest :public T_Engine
{

private:
	static int FRAME_LEFT[20];
	static int FRAME_RIGHT[20];
	static int FRAME_UP[20];
	static int FRAME_DOWN[20];

	static const int NPC_NUM = 10;
	static const int MAP_ROWS = 24;
	static const int MAP_COLS = 32;
	static const int MAP_BKG = 30;
	static const int TILE_WIDTH = 32;
	static const int TILE_HEIGHT = 32;
	static const int BARRIER[MAP_ROWS*MAP_COLS];

	T_Map *barrier;
	T_Map *back;
	T_Sprite * player;
	vSpriteSet npc_set;


	int wnd_width, wnd_height;
	POINT mouse_pt;
	int op;
	
	//���幦�ܺ���
	void LoadMap();
	void LoadPlayer();
	void LoadNpc(int total);
	void UpdatePlayerPos(int dir);
	void UpdateNpcPos();
	void UpdateAnimation();  //������Ϸ����


	/* һ��Ϊԭʼ��
	int wnd_width, wnd_height;		// ��Ϸ���ڿ��
	
	static const int NUM = 10;		// NPC��ɫ����
	BlockSprite player;				// ��ҽ�ɫ
	BlockSprite enemy[NUM];			// NPC��ɫ
	T_Graph back;					// ����λͼ
	*/

public:

	virtual ~GdiplusTest(void);
	GdiplusTest(HINSTANCE h_instance,
		LPCTSTR sz_winclass, LPCTSTR sz_title,
		WORD icon = NULL, WORD sm_icon = NULL,
		int winwidth = 800, int winheight = 600);

	// ����T_Engine���е��麯��ʵ����Ϸ����
	void GameInit();								// ��Ϸ��ʼ��	
	void GameLogic();								// ��Ϸ�߼�����
	void GameEnd();									// ��Ϸ��������
	void GamePaint(HDC hdc);						// ��Ϸ��ʾ
	void GameKeyAction(int Action = KEY_SYS_NONE);	// ������Ϊ����	
	void GameMouseAction(int x, int y, int Action); // �����Ϊ����	
};
