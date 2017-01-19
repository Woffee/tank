#pragma once
#include "T_Engine.h"
#include "T_Graphic.h"
#include "T_Sprite.h"
#include "T_AI.h"
#include "T_Map.h"

typedef vector<T_Sprite*>vSpriteSet;


struct BlockSprite
{
	T_Graph image;			// 方块帧图
	int wFrame;				// 动画帧图宽
	int hFrame;				// 动画帧图高
	int frameCount;			// 帧动画当前播放的帧计数
	SPRITEINFO BlockInfo;	// 角色信息
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
	
	//定义功能函数
	void LoadMap();
	void LoadPlayer();
	void LoadNpc(int total);
	void UpdatePlayerPos(int dir);
	void UpdateNpcPos();
	void UpdateAnimation();  //更新游戏动画


	/* 一下为原始：
	int wnd_width, wnd_height;		// 游戏窗口宽高
	
	static const int NUM = 10;		// NPC角色数量
	BlockSprite player;				// 玩家角色
	BlockSprite enemy[NUM];			// NPC角色
	T_Graph back;					// 背景位图
	*/

public:

	virtual ~GdiplusTest(void);
	GdiplusTest(HINSTANCE h_instance,
		LPCTSTR sz_winclass, LPCTSTR sz_title,
		WORD icon = NULL, WORD sm_icon = NULL,
		int winwidth = 800, int winheight = 600);

	// 重载T_Engine类中的虚函数实现游戏功能
	void GameInit();								// 游戏初始化	
	void GameLogic();								// 游戏逻辑处理
	void GameEnd();									// 游戏结束处理
	void GamePaint(HDC hdc);						// 游戏显示
	void GameKeyAction(int Action = KEY_SYS_NONE);	// 按键行为处理	
	void GameMouseAction(int x, int y, int Action); // 鼠标行为处理	
};
