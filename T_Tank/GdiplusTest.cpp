#include "GdiplusTest.h"
#include "fstream"
#include "stdio.h"
#include "iostream"
#include <cassert>
using namespace std;


int GdiplusTest::FRAME_LEFT[20] = { 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2 };
int GdiplusTest::FRAME_RIGHT[20] = { 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5 };
int GdiplusTest::FRAME_UP[20] = { 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8 };
int GdiplusTest::FRAME_DOWN[20] = { 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11 };

const int GdiplusTest::BARRIER[MAP_ROWS*MAP_COLS]=
{
	//��CSV��ʽͼ������ճ������
	36, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 37,
	35, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 33,
	35, 00, 00, 25, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 27, 00, 00, 00, 25, 26, 26, 26, 26, 26, 26, 27, 00, 00, 00, 33,
	35, 00, 00, 41, 42, 37, 36, 37, 34, 36, 42, 42, 42, 42, 42, 42, 43, 00, 00, 00, 41, 42, 42, 42, 42, 42, 42, 43, 00, 00, 00, 33,
	35, 00, 00, 00, 00, 33, 44, 45, 34, 35, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 33,
	35, 00, 00, 00, 00, 33, 34, 36, 37, 35, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 33,
	44, 27, 00, 00, 00, 33, 34, 44, 45, 35, 00, 00, 25, 26, 26, 26, 27, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 33,
	34, 35, 00, 00, 00, 33, 36, 37, 34, 35, 00, 00, 33, 36, 37, 34, 35, 00, 00, 00, 25, 26, 26, 26, 26, 27, 00, 00, 00, 00, 25, 45,
	34, 35, 00, 00, 00, 33, 44, 45, 34, 35, 00, 00, 33, 44, 45, 34, 35, 00, 00, 00, 33, 36, 37, 36, 37, 35, 00, 00, 00, 00, 33, 34,
	34, 35, 00, 00, 00, 41, 42, 42, 42, 43, 00, 00, 41, 42, 42, 42, 43, 00, 00, 00, 33, 44, 45, 44, 45, 35, 00, 00, 00, 00, 33, 34,
	34, 35, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 33, 34, 36, 37, 34, 35, 00, 00, 00, 00, 33, 34,
	34, 35, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 33, 34, 44, 45, 34, 35, 00, 00, 00, 00, 33, 34,
	34, 35, 00, 00, 00, 25, 26, 26, 27, 00, 00, 00, 25, 26, 26, 26, 27, 00, 00, 00, 41, 42, 42, 42, 42, 43, 00, 00, 00, 00, 33, 34,
	34, 35, 00, 00, 00, 33, 34, 34, 35, 00, 00, 00, 33, 34, 36, 37, 35, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 33, 34,
	36, 43, 00, 00, 00, 33, 36, 37, 35, 00, 00, 00, 33, 34, 44, 45, 35, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 33, 34,
	35, 00, 00, 00, 00, 33, 44, 45, 35, 00, 00, 00, 33, 34, 34, 34, 35, 00, 00, 00, 25, 26, 26, 26, 26, 27, 00, 00, 00, 00, 33, 34,
	35, 00, 00, 00, 00, 41, 42, 42, 43, 00, 00, 00, 41, 42, 42, 42, 43, 00, 00, 00, 33, 36, 37, 36, 37, 35, 00, 00, 00, 00, 41, 37,
	35, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 33, 44, 45, 44, 45, 35, 00, 00, 00, 00, 00, 33,
	35, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 33, 34, 36, 37, 34, 35, 00, 00, 00, 00, 00, 33,
	35, 00, 00, 25, 26, 26, 27, 00, 00, 00, 25, 26, 26, 26, 26, 26, 26, 26, 26, 26, 45, 34, 44, 45, 34, 44, 26, 26, 27, 00, 00, 33,
	35, 00, 00, 33, 34, 34, 35, 00, 00, 00, 33, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 35, 00, 00, 33,
	35, 00, 00, 41, 42, 42, 43, 00, 00, 00, 41, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 43, 00, 00, 33,
	35, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 33,
	44, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 45
};

GdiplusTest::~GdiplusTest(void)
{
}

GdiplusTest::GdiplusTest(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title,
	WORD icon, WORD sm_icon, int winwidth, int winheight)
	:T_Engine(h_instance, sz_winclass, sz_title, icon, sm_icon, winwidth, winheight)
{
	wnd_width = winwidth;
	wnd_height = winheight;
}



// ��Ϸ��ʾ(����ֵΪGameState)
void GdiplusTest::GamePaint(HDC hdc)
{
	back->Draw(hdc);
	barrier->Draw(hdc);
	vSpriteSet::iterator p;
	for (p = npc_set.begin(); p != npc_set.end(); p++)
	{
		if ((*p)->IsActive() == true && (*p)->IsVisible() == true) (*p)->Draw(hdc);
	}
	player-> Draw(hdc);
}
// ��Ϸ��������
void GdiplusTest::GameKeyAction(int Action)
{
	if (Action == KEY_DOWN)
	{
		if (CheckKey(VK_LEFT) && !CheckKey(VK_DOWN) && !CheckKey(VK_UP))
		{
			player->SetActive(true);
			player->SetSequence(FRAME_LEFT, 20); 
			player->SetDir(DIR_LEFT);
			op = 0;
		}
		if (CheckKey(VK_RIGHT) && !CheckKey(VK_DOWN) && !CheckKey(VK_UP))
		{
			player->SetActive(true);
			player->SetSequence(FRAME_RIGHT, 20);
			player->SetDir(DIR_RIGHT);
			op = 0;
		}
		if (CheckKey(VK_UP) && !CheckKey(VK_LEFT) && !CheckKey(VK_RIGHT))
		{
			player->SetActive(true);
			player->SetSequence(FRAME_UP, 20);
			player->SetDir(DIR_UP);
			op = 0;
		}
		if (CheckKey(VK_DOWN) && !CheckKey(VK_LEFT) && !CheckKey(VK_RIGHT))
		{
			player->SetActive(true);
			player->SetSequence(FRAME_DOWN, 20);
			player->SetDir(DIR_DOWN);
			op = 0;
		}
		if (CheckKey(VK_LEFT) == false && CheckKey(VK_RIGHT) == false && CheckKey(VK_UP) == false && CheckKey(VK_DOWN) == false && op == 0)
		{
			//player->SetActive(false);
		}
	}

}

// ��Ϸ�����Ϊ����(����ֵΪKEY_MOUSE_ACTION)
void GdiplusTest::GameMouseAction(int x, int y, int Action)
{

}


//���幦�ܺ���
void GdiplusTest::LoadMap()
{
	LAYERINFO mapInfo;

	mapInfo.first_gid = 1;
	mapInfo.map_cols = MAP_COLS;
	mapInfo.map_rows = MAP_ROWS;
	mapInfo.tile_width = TILE_WIDTH;
	mapInfo.tile_height = TILE_HEIGHT;
	mapInfo.tile_path = L".\\map\\desert.png"; //ͼ���ļ�
	vector< vector<int> >barrier_data;
	vector< vector<int> >back_data;
	vector<int>row_data(MAP_COLS);

	for (int r = 0; r < MAP_ROWS; ++r)
	{
		barrier_data.push_back(row_data);
		back_data.push_back(row_data);
	}
	
	//����
	for (int r = 0; r < MAP_ROWS; ++r)
	{
		for (int c = 0; c < MAP_COLS; ++c)
		{
			back_data[r][c] = MAP_BKG;
		}
	}

	mapInfo.type_id = LAYER_MAP_BACK;
	mapInfo.data = back_data;
	back = new T_Map(mapInfo);
	back->SetPosition(0, 0);
	back->SetUpdate(true);
	
	//��ȡ��ͼ�ļ�
	int map_date[1000] = {0};
	ifstream infile;
	infile.open(".\\map\\map.txt");   //���ļ����������ļ��������� 
	assert(infile.is_open());                            //��ʧ��,�����������Ϣ,����ֹ�������� 
	string s, wall_data = "";
	while (getline(infile, s))
	{
		if (s == "[layer]")
		{
			while (getline(infile, s))
			{
				if (s == "type=wall")
				{
					while (getline(infile, s))
					{
						wall_data += s;
					}
				}
			}
		}
	}
	infile.close();             //�ر��ļ������� 
	int num = 0, index = 0,len = wall_data.length();
	for (int i = 0; i < len; ++i)
	{
		if (wall_data[i] >= '0' && wall_data[i] <= '9')
		{
			num = num * 10 + wall_data[i] - '0';
		}
		if (wall_data[i] == ',' || i == len-1)
		{
			map_date[index++] = num;
			num = 0;
		}
	}


	//�ϰ���
	for (int i = 0; i < MAP_ROWS*MAP_COLS; ++i)
	{
		int col = i%MAP_COLS;
		int row = i / MAP_COLS;
		barrier_data[row][col] = map_date[i];
	}

	mapInfo.type_id = LAYER_MAP_BARR;
	mapInfo.data = barrier_data;
	
	barrier = new T_Map(mapInfo);
	barrier->SetPosition(0, 0);
	barrier->SetUpdate(true);

}



void GdiplusTest::LoadPlayer()
{
	SPRITEINFO spInfo;
	player = new T_Sprite(L".\\res\\blockorange.png", 80, 80);
	spInfo.Active = false;
	spInfo.Dead = false;
	spInfo.Dir = DIR_UP;
	spInfo.Rotation = TRANS_NONE;
	spInfo.Ratio = 0.4f;
	spInfo.Level = 0;
	spInfo.Score = 0; 
	spInfo.Speed = 4;
	spInfo.Alpha = 255;
	spInfo.X = 32*1;
	spInfo.Y = 32*22;
	spInfo.Visible = true;
	player->Initiate(spInfo);
	player->SetSequence(FRAME_UP, 20);
	player->SetLayerTypeID(LAYER_PLY);
}
void GdiplusTest::LoadNpc(int total)
{
	SPRITEINFO spInfo;
	for (int i = 0; i < total; ++i)
	{
		spInfo.Active = true;
		spInfo.Dead = false;
		spInfo.Rotation = TRANS_NONE;
		spInfo.Ratio = 0.4f;
		spInfo.Speed = 2;
		spInfo.Alpha = 255;
		spInfo.Visible = true;
		spInfo.Level = 0;
		spInfo.Score = 0;
		int sp_width = 40;
		int sp_height = 40;

		//����NPC�ĳ����ؼ���ʼ����
		int sdr = rand() % 4 ;
		int d = rand() % 2;
		switch (sdr)
		{
			case 0:
				spInfo.Dir = DIR_LEFT;
				break;
			case 1:
				spInfo.Dir = DIR_RIGHT;
				break;
			case 2:
				spInfo.Dir = DIR_UP;
				break;
			case 3:
				spInfo.Dir = DIR_DOWN;
				break;
		}
		spInfo.X = 32*30;
		spInfo.Y = 32*1;

		//sdr = 0;
		//��ʱ�и�bug��sdr���ǲ�Ϊ0.
		//switch (sdr)
		//{
		//case 0://���Ͻ�
		//	if (d == 0)spInfo.Dir = DIR_RIGHT;
		//	if (d == 1)spInfo.Dir = DIR_DOWN;
		//	spInfo.X = 0;
		//	spInfo.Y = 0;
		//	break;
		//case 1://���Ͻ�
		//	if (d == 0)spInfo.Dir = DIR_LEFT;
		//	if (d == 1)spInfo.Dir = DIR_DOWN;
		//	spInfo.X = wnd_width-sp_width;
		//	spInfo.Y = 0;
		//	break;
		//case 2://���½�
		//	if (d == 0)spInfo.Dir = DIR_RIGHT;
		//	if (d == 1)spInfo.Dir = DIR_UP;
		//	spInfo.X = 0;
		//	spInfo.Y = wnd_height - sp_height;
		//	break;
		//case 3://���½�
		//	if (d == 0)spInfo.Dir = DIR_LEFT;
		//	if (d == 1)spInfo.Dir = DIR_UP;
		//	spInfo.X = wnd_width - sp_width;
		//	spInfo.Y = wnd_height - sp_height;
		//	break;
		//default:
		//	break;
		//}
		npc_set.push_back(new T_Sprite(L".\\res\\blockgreen.png", 80, 80));
		//��npc�б��������µ���Ŀ
		T_Sprite * sp = npc_set.back();
		sp->Initiate(spInfo);
		switch (spInfo.Dir)
		{
		case DIR_LEFT:
			sp->SetSequence(FRAME_LEFT, 20);
			break;
		case DIR_RIGHT:
			sp->SetSequence(FRAME_RIGHT, 20);
			break;
		case DIR_UP:
			sp->SetSequence(FRAME_UP, 20);
			break;
		case DIR_DOWN:
			sp->SetSequence(FRAME_DOWN, 20);
			break;
		}
		sp->SetLayerTypeID(LAYER_NPC);
		sp = NULL;
		
	}
}
void GdiplusTest::UpdatePlayerPos(int dir)
{
	if (player == NULL)return;
	int nextStepX, nextStepY;
	int SpeedX = 0, SpeedY = 0;
	if (player->IsVisible() == true && player->IsActive() == true)
	{
		switch (dir)
		{
		case DIR_LEFT:
			SpeedX = -player->GetSpeed();
			SpeedY = 0;
			//������һ���Ƿ�Խ��
			nextStepX = player->GetX() - player->GetSpeed();
			if (nextStepX <= 0)SpeedX = 0 - player->GetX();
			break;
		case DIR_RIGHT:
			SpeedX = player->GetSpeed();
			SpeedY = 0;
			//������һ���Ƿ�Խ��
			nextStepX = player->GetX() +player->GetRatioSize().cx + player->GetSpeed();
			if (nextStepX >= wnd_width) SpeedX = wnd_width - player->GetRatioSize().cx - player->GetX();
			break;
		case DIR_UP:
			SpeedX = 0;
			SpeedY = -player->GetSpeed();
			//������һ���Ƿ�Խ��
			nextStepY = player->GetY() - player->GetSpeed();
			if (nextStepY <= 0)SpeedY = 0 - player->GetY();
			break;
		case DIR_DOWN:
			SpeedX = 0;
			SpeedY = player->GetSpeed();
			//������һ���Ƿ�Խ��
			nextStepY = player->GetY() + player->GetRatioSize().cy + player->GetSpeed();
			if (nextStepY >= wnd_height)SpeedY = wnd_height - player->GetRatioSize().cy - player->GetY();
			break;
		default:
			break;
		}
		int x = player->GetX();
		int y = player->GetY(); 
		if (!player->CollideWith(barrier))player->Move(SpeedX, SpeedY);
		if (player->CollideWith(barrier)) {
			player->SetPosition(x, y);//��ԭ�ƶ�ǰ��λ��
			int a = 0;
		}
			
	}
}
void GdiplusTest::UpdateNpcPos()
{
	if (npc_set.size() == 0)return;

	T_AI * spAI = new T_AI(4);

	vSpriteSet::iterator p;
	for (p = npc_set.begin(); p != npc_set.end(); p++)
	{
		if ((*p)->IsActive() == true && (*p)->IsVisible() == true)
		{
			spAI->Evade((*p), player);
			spAI->CheckOverlay((*p), npc_set);
			int npc_dir = (*p)->GetDir();

			switch (npc_dir)
			{
			case DIR_LEFT:
				(*p)->SetSequence(FRAME_LEFT, 20);
				break;
			case DIR_RIGHT:
				(*p)->SetSequence(FRAME_RIGHT, 20);
				break;
			case DIR_UP:
				(*p)->SetSequence(FRAME_UP, 20);
				break;
			case DIR_DOWN:
				(*p)->SetSequence(FRAME_DOWN, 20);
				break;

			default:
				break;
			}
			//NPC �ڵ�ͼ�����Σ����Զ�����ͼ�ϰ�
			spAI->Wander((*p), npc_dir, (*p)->GetSpeed(), barrier);
		}
	}
	delete spAI;
}
void GdiplusTest::UpdateAnimation()
{
	vSpriteSet::iterator p;
	if (npc_set.size() > 0)
	{
		for (p = npc_set.begin(); p != npc_set.end(); p++)
		{
			(*p)->LoopFrame();
		}
	}
	if (player != NULL)
	{
		if (player->IsVisible() == true && player->IsActive() == true)player->LoopFrame();
	}
}

void GdiplusTest::GameInit()
{
	op = 0;
	LoadMap();
	LoadPlayer();
	LoadNpc(NPC_NUM);
}

void GdiplusTest::GameLogic()
{
	GameKeyAction();
	UpdateNpcPos();
	if (player->IsActive())
	{
		if (op == 1)
		{
			long centerX = player->GetX() + (player->GetRatioSize().cx / 2);
			long centerY = player->GetY() + (player->GetRatioSize().cy / 2);
			POINT centerPT = { centerX, centerY };

			player->MoveTo(mouse_pt, centerPT, barrier);
		}
		if (op == 0)UpdatePlayerPos(player->GetDir());
	}
	UpdateAnimation();
}

void GdiplusTest::GameEnd()
{

}
