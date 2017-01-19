#include"stdafx.h"
#include"T_Sprite.h"
#include"T_Util.h"

T_Sprite::T_Sprite(LPCTSTR imgPath, int frameWidth, int frameHeight)
{
	if (wcslen(imgPath) > 0)
	{
		spImg.LoadImageFile(imgPath);
		spOldImg = spImg;
	}
	else
	{
		throw L"ͼ��·��Ϊ�գ�";
	}

	//�޶����Ľ�ɫͼƬ
	if (frameWidth == 0 && frameHeight == 0)
	{
		SetWidth(spImg.GetImageWidth());
		SetHeight(spImg.GetImageHeight());
		totalFrames = rawFrames = 0;													//������֡��
	}
	else //���ж����Ľ�ɫͼƬ
	{
		SetWidth(frameWidth);
		SetHeight(frameHeight);
		frameCols = spImg.GetImageWidth() / frameWidth;									//֡ͼƬ������
		frameRows = spImg.GetImageHeight() / frameHeight;								//֡ͼƬ������
		totalFrames = frameCols*frameRows;												//��֡��
		rawFrames = frameCols*frameRows;												//ԭʼ������֡��
		forward = 0;																	//��ǰ֡������ʼ��
		backward = totalFrames - 1;
	}
	frameSequence = NULL;
	loopForward = true;
}

T_Sprite::~T_Sprite(void) 
{
	
}

void T_Sprite::Initiate(SPRITEINFO spInfo)
{
	SetPosition(spInfo.X,spInfo.Y);														//��ɫ����
	Visible = spInfo.Visible;															//��ɫ�Ƿ�ɼ�
	dir = spInfo.Dir;																	//��ɫ����
	active = spInfo.Active;																//��ɫ״̬���Ƿ��ƶ���
	speed = spInfo.Speed;																//��ɫ�ƶ��ٶ�
	dead = spInfo.Dead;																	//��ɫ����״̬
	level = spInfo.Level;																//��ɫ�ȼ�
	score = spInfo.Score;																//��ɫ��ɫ��ֵ
	frameRatio = spInfo.Ratio;															//֡ͼƬ���űȣ�1ԭʼ������1�Ŵ�С��1��С��
	frameRotate = spInfo.Rotation;														//֡ͼƬ�Ƿ���ת�任��TRANSFERֵ֮һ��
	frameAlpha = spInfo.Alpha;															//֡ͼƬ͸����
	if (frameRatio > 0)
	{
		collideWidth = GetRatioSize().cx;
		collideHeight = GetRatioSize().cy;
	}
	else
	{
		collideWidth = (int)GetWidth();
		collideHeight = (int)GetHeight();
	}
}

//��ǰ������ѭ����������֡
void T_Sprite::LoopFrame(bool ahead)
{
	loopForward = ahead;
	if (totalFrames > 0)
	{
		if (ahead == true)
		{
			forward++;
			if (forward > totalFrames - 1)
			{
				forward = 0;
			}
		}
		else
		{
			backward--;
			if (backward < 0)
			{
				backward = totalFrames - 1;
			}
		}
	}
}

bool T_Sprite::LoopFrameOnce(bool ahead)
{
	loopForward = ahead;
	if (totalFrames <= 0)
	{
		return true;
	}

	if (ahead == true)
	{
		forward++;
		if (forward > totalFrames - 1)
		{
			forward = 0;
			return true;											//������һ������֡
		}
		else
		{
			return false;
		}
	}
	else
	{
		backward--;
		if (backward < 0)
		{
			backward = totalFrames - 1;
			return true;											//������һ������֡
		}
		else
		{
			return false;
		}
	}
}

void T_Sprite::Draw(HDC hdc)
{
	int frmIndex = 0;
	if (frameSequence != NULL && backward >= 0 && forward >= 0)
	{
		if (loopForward == false)
		{
			frmIndex = frameSequence[backward];
		}
		else
		{
			frmIndex = frameSequence[forward];
		}
	}
	else
	{
		if (loopForward == false)
		{
			frmIndex = backward;
		}
		else
		{
			frmIndex = forward;
		}
	}

	if (&spImg != NULL)
	{
		if (Visible == true)
		{
			if (totalFrames == 0)
			{
				spImg.PaintRegion(spImg.GetBmpHandle(),hdc,(int)X,(int)Y,0,0,Width,Height,frameRatio,frameRotate,frameAlpha);
			}
			else
			{
				spImg.PaintFrame(spImg.GetBmpHandle(), hdc, (int)X, (int)Y, frmIndex,frameCols,Width, Height, frameRatio, frameRotate, frameAlpha);
			}
		}
	}
}

bool T_Sprite::MoveTo(IN POINT mousePT,IN POINT destPT,IN RECT Boundary)
{
	int xRatio,yRatio;
	if (active == true)
	{
		//�������ź��֡ͼ�ߴ�
		SIZE ratioFrameSize = GetRatioSize();

		//����õľ�������ָ��������5������Ϊ�������
		RECT HotRect;
		HotRect.left = (long)(destPT.x - 5);
		HotRect.right = (long)(destPT.x + 5);
		HotRect.top = (long)(destPT.y - 5);
		HotRect.bottom = (long)(destPT.y + 5);

		//����mousePT��destPT�����ɫ��speed�ٶ��ƶ�ʱ��X��Y������ƶ��ٶ�
		T_Util::GetBevelSpeed(destPT, mousePT, speed, xRatio, yRatio);

		BOOL ToDesPos = PtInRect(&HotRect,mousePT);
		if (ToDesPos == TRUE)
		{
			return true;
		}
		else{
			//������һ��X�����Y������ƶ��ٶ�
			int nextStepX = (int)xRatio;
			int nextStepY = (int)yRatio;
			POINT nextPT = {(long)(GetX()+nextStepX),(long)(GetY()+nextStepY)};

			if (nextPT.x <= Boundary.left)
			{
				nextStepX = Boundary.left - (int)GetX();
			}
			if ((nextPT.x - ratioFrameSize.cx) >= Boundary.right)
			{
				nextStepX = Boundary.right - ratioFrameSize.cx - (int)GetX();
			}
			if (nextPT.y <= Boundary.top)
			{
				nextStepY = Boundary.top - (int)GetY();
			}
			if ((nextPT.y + ratioFrameSize.cy) >= Boundary.bottom)
			{
				nextStepY = Boundary.bottom - ratioFrameSize.cy - (int)GetY();
			}
			Move(nextStepX,nextStepY);
			
			return false;
		}
	}
	return true;
}

//��������жϷ���
int T_Sprite::GetDir(POINT mousePT)
{
	int dir = -1;
	POINT spNowXY = { (long)GetX(), (long)GetY() };

	//�������ź��֡ͼ�ߴ�
	SIZE ratioFrameSize = GetRatioSize();

	//���ս�ɫ�����������ο���������
	//��Ϊ���յľ������򣨸��ݽ�ɫ��ǰ��������ã�

	//�������ɫ���ڵľ�������
	RECT SpriteRect;
	SpriteRect.left = (long)GetX();
	SpriteRect.top = (long)GetY();
	SpriteRect.right = (long)(GetX() + ratioFrameSize.cx);
	SpriteRect.bottom = (long)(GetY() + ratioFrameSize.cy);

	//�жϽ�ɫ�ĳ���
	if (mousePT.x < SpriteRect.left)	//����ڽ�ɫ���ε����
	{
		if (mousePT.y<SpriteRect.top)	//����ڽ�ɫ���������ϲ�
		{
			dir = DIR_LEFT_UP;
		}
		else if (mousePT.x>SpriteRect.bottom)	//����ڽ�ɫ���������²�
		{
			dir = DIR_LEFT_DOWN;
		}
		else    //����ڽ�ɫ���������в�
		{
			dir = DIR_LEFT;
		}
	}
	else if (mousePT.y>SpriteRect.right)	//����ڽ�ɫ���ε��Ҳ�
	{
		if (mousePT.y<SpriteRect.top)	//����ڽ�ɫ���������ϲ�
		{
			dir = DIR_RIGHT_UP;
		}
		else if (mousePT.x>SpriteRect.bottom)	//����ڽ�ɫ���������²�
		{
			dir = DIR_RIGHT_DOWN;
		}
		else    //����ڽ�ɫ���������в�
		{
			dir = DIR_RIGHT;
		}
	}
	else    //����ڽ�ɫ���ε��в�
	{
		if (mousePT.y<SpriteRect.top)	//����ڽ�ɫ���������ϲ�
		{
			dir = DIR_UP;
		}
		else if (mousePT.x>SpriteRect.bottom)	//����ڽ�ɫ���������²�
		{
			dir = DIR_DOWN;
		}
		else    //����ڽ�ɫ���������в�
		{
			dir = -1;
		}
	}

	return dir;
}

//����ͼ��ײ
bool T_Sprite::CollideWith(IN T_Map* map)
{
	//���������ͼƬ�򲻼���ͼ��ײ
	if (map->GetMapRows() == 0 && map->GetMapCols() == 0)
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}

	//�����ͼ���ɼ����ɫ���ɼ�������ͼ��ײ
	if ((!(map->IsVisible())) || (!(this->IsVisible())))
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}

	//���㵱ǰ��ͼͼ��ľ��η�Χ
	int mapLeft = map->GetX();
	int mapTop = map->GetY();
	int mapRight = mapLeft + map->GetWidth();
	int mapBottom = mapTop + map->GetHeight();

	//��õ�ͼͼ����ʹ�õ�ͼ��Ŀ��
	int tW = map->GetTileWidth();
	int tH = map->GetTileHeight();

	//���㵱ǰ��ɫ�ľ��η�Χ
	int spLeft = this->GetCollideRect()->left;
	int spTop = this->GetCollideRect()->top;
	int spRight = this->GetCollideRect()->right;
	int spBottom = this->GetCollideRect()->bottom;

	//��õ�ǰ��ͼ��ͼ�����������������
	int tNumCols = map->GetMapCols();
	int tNumRows = map->GetMapRows();

	//���㵱ǰ��ɫ��ʵ�ʵĿ��
	int spW = spRight - spLeft;
	int spH = spBottom - spTop;

	RECT lprcDst;

	//�������ϼ����ͼ��ľ��η�Χ�ͽ�ɫ�ľ��η�Χ�����������ζ���
	RECT mapRect = { mapLeft,mapTop,mapRight,mapBottom };
	RECT spRect = { spLeft,spTop,spRight,spBottom };

	//����������ζ���û�з����κ���ײ�����˳�����
	if ((IntersectRect(&lprcDst, &mapRect, &spRect)) == FALSE)
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}

	//����������ζ���������ײ�����ȼ����ɫ�����ϡ��¡����ҵľ�������
	int startRow = (spTop <= mapTop) ? 0 : (spTop - mapTop) / tH;
	int endRow = (spBottom < mapBottom) ? (spBottom - 1 - mapTop) / tH : tNumRows - 1;
	int startCol = (spLeft <= mapLeft) ? 0 : (spLeft - mapLeft) / tW;
	int endCol = (spRight < mapRight) ? (spRight - 1 - mapLeft) / tW : tNumCols - 1;

	//���ݽ�ɫ�����ϡ��¡����ҵľ��������ж��ĸ���������Ϊ�ϰ�
	for (int row=startRow;row<=endRow;++row)
	{
		for (int col = startCol; col <= endCol; ++col)
		{
			//�����ǰ�������ڵ�ͼ���ڵ�ͼ������Ϊ��0���ͱ�ʾ���ϰ�
			if (map->GetTile(col, row) != 0)
			{
				mapBlockPT.x = col;	//	��¼��ǰ�ϰ�ͼ�����
				mapBlockPT.y = row;	//	��¼��ǰ�ϰ�ͼ�����

				//���ݽ�ɫ��ǰ�ķ��������ײǰ��λ��
				int x = GetX(), y = GetY();
				switch (GetDir())
				{
				case DIR_LEFT:
					x = GetX() + GetSpeed();
					y = GetY();
					break;
				case DIR_RIGHT:
					x = GetX() - GetSpeed();
					y = GetY();
					break;
				case DIR_UP:
					x = GetX();
					y = GetY() + GetSpeed();
					break;
				case DIR_DOWN:
					x = GetX();
					y = GetY() - GetSpeed();
					break;
				}

				//����ɫ��λ��ײǰ��λ��
				SetPosition(x,y);
				return true;
			}
		}
	}
	return false;
}

bool T_Sprite::MoveTo(IN POINT mousePT, IN POINT destPT, IN T_Map* map)
{
	int xRatio, yRatio;
	if (active == true)
	{
		//�������ź��֡ͼ�ߴ�
		SIZE ratioFrameSize = GetRatioSize();

		//����õľ�������ָ��������5������Ϊ�������
		RECT HotRect;
		HotRect.left = (long)(destPT.x - 5);
		HotRect.right = (long)(destPT.x + 5);
		HotRect.bottom = (long)(destPT.y + 5);
		HotRect.top = (long)(destPT.y - 5);

		//��mousePT��destPT�����ɫ��speed�ٶ��ƶ�ʱx��y������ƶ��ٶ�
		T_Util::GetBevelSpeed(destPT,mousePT,speed,xRatio,yRatio);
		
		//�ж������Ƿ��ڼ����
		BOOL ToDesPos = PtInRect(&HotRect, mousePT);
		//����ڼ������ʾ��ɫ�Ѿ�����Ŀ�ĵ�
		if (ToDesPos == TRUE) return true;
		//��ɫû�е���Ŀ�ĵ�
		if (ToDesPos == FALSE)
		{
			//��һ��X��Y������ƶ��ٶ�
			int nextStepX = (int)xRatio;
			int nextStepY = (int)yRatio;
			int x = 0;
			int y = 0;
			//���û�к͵�ͼ�ϰ�����ײ��������ƶ�
			if(!CollideWith(map))
			{
				x = GetX();
				y = GetY();
				Move(nextStepX,nextStepY);
			}
			//����͵�ͼ�ϰ�����ײ����λ����ײǰ��λ��
			if (CollideWith(map)) SetPosition(x,y);
			return false;
		}
	}
	return true;
}