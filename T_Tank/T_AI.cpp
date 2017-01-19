#include"stdafx.h"
#include"T_AI.h"

int T_AI::EvadeDir[8][5] = {
//DIR_LEFT躲避方向
{ DIR_RIGHT , DIR_DOWN , DIR_UP , DIR_RIGHT_UP , DIR_RIGHT_DOWN },

//DIR_RIGHT躲避方向
{ DIR_LEFT , DIR_DOWN , DIR_UP , DIR_LEFT_UP , DIR_LEFT_DOWN },

//DIR_UP躲避方向
{ DIR_DOWN , DIR_LEFT , DIR_RIGHT , DIR_LEFT_DOWN , DIR_RIGHT_DOWN },

//DIR_DOWN躲避方向
{ DIR_UP , DIR_LEFT , DIR_RIGHT , DIR_LEFT_UP , DIR_RIGHT_UP },

//DIR_LEFT_UP躲避方向
{ DIR_RIGHT_DOWN , DIR_RIGHT_UP , DIR_RIGHT , DIR_UP , DIR_DOWN },

//DIR_LEFT_DOWN躲避方向
{ DIR_RIGHT_UP , DIR_RIGHT_DOWN , DIR_RIGHT , DIR_UP , DIR_DOWN },

//DIR_RIGHT_UP躲避方向
{ DIR_LEFT_DOWN , DIR_LEFT_UP , DIR_LEFT , DIR_UP , DIR_DOWN },

//DIR_RIGHT_DOWN躲避方向
{ DIR_LEFT_UP , DIR_LEFT_DOWN , DIR_LEFT , DIR_UP , DIR_DOWN }
};

T_AI::T_AI(int d_style)
{
	dir_style = d_style;
}

T_AI::~T_AI(void){}

void T_AI::GetHitRect(IN T_Sprite* sp,IN int spSizeTimes,OUT RECT& testRec){
	int sp_width = sp->GetRatioSize().cx;
	int sp_height = sp->GetRatioSize().cy;

	switch (sp->GetDir())
	{
	case DIR_LEFT:
		testRec.left = sp->GetX() - sp_width*spSizeTimes;
		testRec.right = sp->GetX();
		testRec.top = sp->GetY();
		testRec.bottom = sp->GetY() + sp_height;
		break;
	case DIR_RIGHT:
		testRec.left = sp->GetX()+sp_width;
		testRec.right = sp->GetX()+sp_width*(spSizeTimes+1);
		testRec.top = sp->GetY();
		testRec.bottom = sp->GetY()+sp_height;
		break;
	case DIR_UP:
		testRec.left = sp->GetX();
		testRec.right = sp->GetX()+sp_width;
		testRec.top = sp->GetY()-sp_height*spSizeTimes;
		testRec.bottom = sp->GetY();
		break;
	case DIR_DOWN:
		testRec.left = sp->GetX();
		testRec.right = sp->GetX()+sp_width;
		testRec.top = sp->GetY()+sp_height;
		testRec.bottom = sp->GetY()+sp_height*(spSizeTimes+1);
		break;
	case DIR_LEFT_UP:
		testRec.left = sp->GetX()-sp_width*spSizeTimes;
		testRec.right = sp->GetX();
		testRec.top = sp->GetY()-sp_height*spSizeTimes;
		testRec.bottom = sp->GetY();
		break;
	case DIR_LEFT_DOWN:
		testRec.left = sp->GetX()-sp_width*spSizeTimes;
		testRec.right = sp->GetX();
		testRec.top = sp->GetY()+sp_height;
		testRec.bottom = sp->GetY()+sp_height*(spSizeTimes+1);
		break;
	case DIR_RIGHT_UP:
		testRec.left = sp->GetX()+sp_width;
		testRec.right = sp->GetX()+sp_width*(spSizeTimes+1);
		testRec.top = sp->GetY()-sp_height*spSizeTimes;
		testRec.bottom = sp->GetY();
		break;
	case DIR_RIGHT_DOWN:
		testRec.left = sp->GetX()+sp_width;
		testRec.right = sp->GetX()+sp_width*(spSizeTimes+1);
		testRec.top = sp->GetY()+sp_height;
		testRec.bottom = sp->GetY()+sp_height*(spSizeTimes+1);
		break;
	}
}

//根据移动方向、速度和指定的活动范围计算当前角色的移动位置
MoveCoord T_AI::GetMoveCoord(T_Sprite* npc_sp, int dir, int speed, RECT boundary)
{
	MoveCoord mRate;
	int nextStepX, nextStepY;
	int SpeedX = 0, SpeedY = 0;
	mRate.BoundDir = -1;	//没有在边界上
	int sp_width = npc_sp->GetRatioSize().cx;
	int sp_height = npc_sp->GetRatioSize().cy;

	switch (dir)
	{
	case DIR_LEFT:
		SpeedX = -speed;
		SpeedY = 0;

		//计算下一步移动是否超过边界
		nextStepX = npc_sp->GetX() - speed;
		if (nextStepX <= boundary.left)
		{
			SpeedX = -(npc_sp->GetX() - boundary.left);
			mRate.BoundDir = DIR_LEFT;
		}

		break;
	case DIR_RIGHT:
		SpeedX = speed;
		SpeedY = 0;
		//计算下一步移动是否超过边界
		nextStepX = npc_sp->GetX() + sp_width + speed;
		if (nextStepX >= boundary.right)
		{
			SpeedX = boundary.right - (npc_sp->GetX() + sp_width);
			mRate.BoundDir = DIR_RIGHT;
		}
		break;
	case DIR_UP:
		SpeedX = 0;
		SpeedY = -speed;
		//计算下一步移动是否超过边界
		nextStepY = npc_sp->GetY() - speed;
		if (nextStepY <= boundary.top)
		{
			SpeedY = -(npc_sp->GetY() - boundary.top);
			mRate.BoundDir = DIR_UP;
		}
		break;
	case DIR_DOWN:
		SpeedX = 0;
		SpeedY = speed;
		//计算下一步移动是否超过边界
		nextStepY = npc_sp->GetY() + sp_height + speed;
		if (nextStepY >= boundary.bottom) 
		{
			SpeedY = boundary.bottom - npc_sp->GetY() - sp_height;
			mRate.BoundDir = DIR_DOWN;
		}
		break;
	case DIR_LEFT_UP:
		SpeedX = -speed;
		SpeedY = -speed;
		//计算下一步移动是否超过边界
		nextStepX = npc_sp->GetX() - speed;
		nextStepY = npc_sp->GetY() - speed;

		if (nextStepX <= boundary.left)
		{
			SpeedX = -(npc_sp->GetX() - boundary.left);
			SpeedY = SpeedX;
			mRate.BoundDir = DIR_LEFT;
			break;
		}

		if (nextStepY <= boundary.top)
		{
			SpeedY = -(npc_sp->GetY() - boundary.top);
			SpeedX = SpeedY;
			mRate.BoundDir = DIR_UP;
		}

		break;
	case DIR_LEFT_DOWN:
		SpeedX = -speed;
		SpeedY = speed;

		//计算下一步是否超过边界
		nextStepX = npc_sp->GetX() - speed;
		nextStepY = npc_sp->GetY() + sp_height + speed;

		if (nextStepX <= boundary.left)
		{
			SpeedX = -(npc_sp->GetX() - boundary.left);
			SpeedY = -SpeedX;
			mRate.BoundDir = DIR_LEFT;
			break;
		}

		if (nextStepY >= boundary.bottom)
		{
			SpeedY = boundary.bottom - npc_sp->GetY() - sp_height;
			SpeedX = -SpeedY;
			mRate.BoundDir = DIR_DOWN;
		}

		break;
	case DIR_RIGHT_UP:
		SpeedX = speed;
		SpeedY = -speed;

		//计算下一步移动是否超过边界
		nextStepX = npc_sp->GetX() + sp_width + speed;
		nextStepY = npc_sp->GetY() - speed;

		if (nextStepX >= boundary.right) 
		{
			SpeedX = boundary.right - npc_sp->GetX() - sp_width;
			SpeedY = -SpeedX;
			mRate.BoundDir = DIR_RIGHT;
			break;
		}

		if (nextStepY < boundary.top) 
		{
			nextStepY = -(npc_sp->GetY() - boundary.top);
			nextStepX = -nextStepY;
			mRate.BoundDir = DIR_UP;
		}

		break;
	case DIR_RIGHT_DOWN:
		SpeedX = speed;
		SpeedY = speed;
		
		//计算下一步移动是否超过边界
		nextStepX = npc_sp->GetX() + sp_width + speed;
		nextStepY = npc_sp->GetY() + sp_height + speed;

		if (nextStepX >= boundary.right)
		{
			SpeedX = boundary.right - npc_sp->GetX() - sp_width;
			SpeedY = SpeedX;
			mRate.BoundDir = DIR_RIGHT;
			break;
		}

		if (nextStepY >= boundary.bottom)
		{
			SpeedY = boundary.bottom - npc_sp->GetY() - sp_height;
			SpeedX = SpeedY;
			mRate.BoundDir = DIR_DOWN;
		}

		break;
	}

	mRate.moveX = SpeedX;
	mRate.moveY = SpeedY;
	mRate.oldX = npc_sp->GetX();
	mRate.oldY = npc_sp->GetY();

	return mRate;
}

//根据移动方向、速度和指定的活动范围对角色做漫游处理
void T_AI::Wander(T_Sprite* npc_sp, int npc_dir, int speed, RECT boundary) 
{
	//获取当前游戏角色的移动方向和速度计算其下一步的移动信息
	MoveCoord mRate = GetMoveCoord(npc_sp,npc_dir,speed,boundary);
	//如果角色没有到达活动范围的边界
	if (mRate.BoundDir == -1)
	{
		npc_sp->Move(mRate.moveX,mRate.moveY);
	}
	//如果角色到达活动范围的边界
	else
	{
		int r_dir = 0;

		//根据角色的移动方向模式抽取一个随机躲避方向
		if (dir_style == 8)
		{
			r_dir = rand() % 5;
		}
		else
		{
			r_dir = rand() % 3;
		}

		npc_sp->SetPosition(mRate.oldX,mRate.oldY);	//将角色定位在边界
		npc_sp->SetDir(EvadeDir[mRate.BoundDir][r_dir]);	//为角色设置新的移动方向
	}
}

void T_AI::Wander(T_Sprite * npc_sp, int npc_dir, int speed, T_Map * map)
{
	//计算游戏地图的矩形范围
	RECT mapBound;
	mapBound.left = map->GetX();
	mapBound.top = map->GetY();
	mapBound.right = map->GetX() + map->GetWidth();
	mapBound.bottom = map->GetY() + map->GetHeight();

	//获取当前游戏角色的移动方向和速度计算其下一步的移动信息
	MoveCoord mRate = GetMoveCoord(npc_sp,npc_dir,speed,mapBound);

	//根据角色移动的方向模式抽取一个随机躲避方向
	int r_dir = 0;
	if (dir_style == 8)
	{
		r_dir = rand() % 5;
	}
	else
	{
		r_dir = rand() % 3;
	}

	//如果角色没有到达游戏地图的边界
	if (mRate.BoundDir == -1) npc_sp->Move(mRate.moveX,mRate.moveY);
	//如果角色已经到达游戏地图的边界
	if (mRate.BoundDir != -1)
	{
		npc_sp->SetPosition(mRate.oldX,mRate.oldY);
		npc_sp->SetDir(EvadeDir[mRate.BoundDir][r_dir]);
	}

	if (npc_sp->CollideWith(map))
	{
		//将角色定位在边界
		npc_sp->SetPosition(mRate.oldX,mRate.oldY);
		//为角色设置新的移动方向（朝其他3个方向躲避）
		npc_sp->SetDir(EvadeDir[npc_sp->GetDir()][rand()%3]);
	}
}

void T_AI::Evade(T_Sprite* npc_sp, T_Sprite* player)
{
	RECT lprcDst;
	RECT playerRect = {player->GetX(),player->GetY(),
						player->GetX()+player->GetRatioSize().cx,
						player->GetY()+player->GetRatioSize().cy };
	RECT testRect;

	GetHitRect(npc_sp,1,testRect);
	if ((IntersectRect(&lprcDst, &playerRect, &testRect)) == TRUE)
	{
		int r_dir = 0;

		//根据角色的移动方向模式抽取一个随机躲避方向
		if (dir_style == 8)
		{
			r_dir = rand() % 5;
		}
		else
		{
			r_dir = rand() % 3;
		}

		npc_sp->SetDir(EvadeDir[npc_sp->GetDir()][r_dir]);	//为角色设置新的移动方向

	}
}


void T_AI::CheckOverlay(T_Sprite* npc_sp, vector<T_Sprite*> vSpriteSet)
{
	vector<T_Sprite*>::iterator p;
	RECT testRec;
	
	GetHitRect(npc_sp,1,testRec);	//获取当前Sprite周围的矩形
	int r_dir = 0;

	//根据角色的移动方向模式抽取一个随机躲避方向
	if (dir_style == 8)
	{
		r_dir = rand() % 5;
	}
	else
	{
		r_dir = rand() % 3;
	}

	RECT lprcDst;
	for (p = vSpriteSet.begin(); p != vSpriteSet.end(); p++)
	{
		if (*p == npc_sp)
		{
			continue;
		}

		RECT pRect = {(*p)->GetX(),(*p)->GetY(),
						(*p)->GetX()+(*p)->GetRatioSize().cx,(*p)->GetY()+(*p)->GetRatioSize().cy};
		if ((IntersectRect(&lprcDst, &pRect, &testRec)) == TRUE)
		{
			npc_sp->SetDir(EvadeDir[npc_sp->GetDir()][r_dir]);	//为角色设置新的移动方向
		}
	}
}