#include"stdafx.h"
#include"T_Util.h"

void T_Util::GetBevelSpeed(IN POINT startPT, IN POINT destPT, IN int Speed, OUT int& xRatio, OUT int& yRatio)
{
	float aX, bY;
	if (destPT.x > startPT.x)
	{
		aX = (float)(destPT.x - startPT.x);
	}
	else
	{
		aX = (float)(startPT.x - destPT.x);
	}

	if (destPT.y > startPT.y)
	{
		bY = (float)(destPT.y-startPT.y);
	}
	else
	{
		bY = (float)(startPT.y - destPT.y);
	}

	if (aX == 0 || bY == 0)
	{
		xRatio = 0;
		yRatio = 0;
		return;
	}

	if (bY > aX)
	{
		yRatio = Speed;
		xRatio = (int)((Speed*aX )/ bY);
	}
	else
	{
		xRatio = Speed;
		yRatio = (int)((Speed*bY) / aX);
	}
	
	if (startPT.x - destPT.x == 0) xRatio = 0;
	if (startPT.y - destPT.y == 0) yRatio = 0;
	if (startPT.x - destPT.x >0) xRatio = -xRatio;
	if (startPT.y - destPT.y >0) yRatio = -yRatio;
}