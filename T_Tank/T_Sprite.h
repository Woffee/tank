#pragma once;
#include"T_Graphic.h"
#include"T_Layer.h"
#include"T_Util.h"
#include"T_Map.h"

#define ROUND(x) (int)(x+0.5)											//������������ĺ�

class T_Sprite :public T_Layer{
protected: 
	//���ɫͼ����ص�����................................................................
	T_Graph		spImg;													//ԭʼ��ɫͼƬ
	T_Graph		spOldImg;												//�޸�ǰ��ɫͼƬ
	int			frameCols;												//ԭʼ֡ͼƬ������
	int			frameRows;												//ԭʼ֡ͼƬ������
	int			rawFrames;												//ԭʼ֡ͼƬ��֡��
	int			totalFrames;											//��ǰ֡ͼƬ��֡��
	int			forward;												//��ǰ���󲥷�֡����
	int			backward;												//�Ӻ���ǰ����֡����
	bool		loopForward;											//�Ƿ��ǰ���󲥷�֡
	int*		frameSequence;											//�Զ��嶯��֡����
	float		frameRatio;												//֡ͼƬ�Ŵ����С�ȣ�1ԭʼ������1�Ŵ�С��1��С��
	int			frameRotate;											//֡ͼƬ��ת��ת��ʽ��ȡֵΪTRANSFER����ֵ��
	BYTE		frameAlpha;												//֡ͼƬ͸���ȣ�255��͸����0��ȫ͸����
	//���ɫ״̬��ص�����................................................................
	int			dir;													//��ʼ����
	int			lastDir;												//�ϴη���
	bool		active;													//�Ƿ�ɻ
	bool		dead;													//�Ƿ�����
	int			speed;													//��ʼ�ƶ��ٶ�
	int			level;													//��ʼ��Ϸ�ȼ�
	int			score;													//��Ϸ��ֵ
	DWORD		startTime;												//��ʱ��ʼʱ��
	DWORD		endTime;												//��ʱ����ʱ��
	//����ײ�����ص�����................................................................
	RECT		collideRect;											//��ײ����������
	int			collideWidth;											//��ײ������
	int			collideHeight;											//��ײ����߶�

	POINT mapBlockPT;													//��ײ���ϰ�ʱ��λ��
public:
	//���캯��������������������ȡ����.....................................................
	//frameWidth��frameHeight��Ϊ0����ɫͼ���޶���
	//frameWidth��frameHeight������0����ɫͼ�����֡����
	T_Sprite(LPCTSTR imgPath,int frameWidth=0,int frameHeight=0);
	virtual ~T_Sprite(void);
	virtual string ClassName(){ return "T_Sprite"; }					//��ȡ������
	//��ͼ����صĲ���.....................................................................
	T_Graph* GetImage(){ return &spImg; }								//��ȡ����T_Graph����
	void SetImage(T_Graph* pImg){ spImg = *pImg; }						//���ñ���T_Graph����
	void ResetImage(){ spImg = spOldImg; }								//�ָ�����T_Graph����
	float GetRatio(){ return frameRatio; }								//������ű�
	void SetRatio(float rat){ frameRatio = rat; }						//�������ű�
	SIZE GetRatioSize(){												//�������űȼ���֡ͼƬ����
		SIZE ratioFrameSize;
		ratioFrameSize.cx = ROUND(frameRatio*Width);
		ratioFrameSize.cy = ROUND(frameRatio*Height);
		return ratioFrameSize;
	}
	int GetRotation(){ return frameRotate; }							//���ͼƬת����ʽ
	void SetRotation(int rot){ frameRotate = rot; }						//����ͼ��ת����ʽ��rotΪTRANSFER����ֵ��
	BYTE GetAlpha(){ return frameAlpha; }								//���͸����ֵ
	void SetAlpha(BYTE a){ frameAlpha = a; }							//����͸����ֵ
	//��״̬��صĲ���.....................................................................
	int GetDir(){ return dir; }											//��÷���ֵ
	void SetDir(int d){ dir = d; };										//���÷���ֵ
	bool IsActive(){ return active; }									//�Ƿ�ɻ
	void SetActive(int act){ active = act; }							//�����Ƿ�ɻ
	bool IsDead(){ return dead; }										//����״̬
	void SetDead(bool dd){ dead = dd; }									//��������״̬
	int GetSpeed(){ return speed; }										//��ȡ�ٶ�ֵ
	void SetSpeed(int spd){ speed = spd; }								//�����ٶ�ֵ
	int GetLevel(){ return level; }										//��ȡ�ȼ�ֵ
	void SetLevel(int lvl){ level = lvl; }								//���õȼ�ֵ
	int GetScore(){ return score; }										//��ȡ��ֵ
	void SetScore(int scr){ score = scr; }								//���÷�ֵ
	//���ʱ��صĲ���.....................................................................
	void SetEndTime(DWORD time){ endTime = time; }						//���ü�ʱ����ʱ��
	DWORD GetEndTime(){ return endTime; }								//��ȡ��ʱ����ʱ��
	void SetStartTime(DWORD time){ startTime = time; }					//���ü�ʱ��ʼʱ��
	DWORD GetStartTime(){ return startTime; }							//��ü�ʱ��ʼʱ��
	//����ײ��صĲ���......................................................................
	//������������������ײ�������ȣ�px��pyΪ��ֱ�Ŵ󡢸�ֵ��С����λΪ���أ�
	void AdjustCollideRect(int px = 0, int py = 0);
	RECT* GetCollideRect();												//��ü����ײ�ľ�������
	//����ɫ��ײ��distance�����ײ�ľ���
	bool CollideWith(T_Sprite* target,int distance=0);
	//�붯��֡��صĲ���....................................................................
	void LoopFrame(bool ahead=true);									//��ǰ������ѭ����������֡
	bool LoopFrameOnce(bool ahead = true);								//��ǰ�����󲥷�һ������֡
	int GetRawFrames(){ return rawFrames; }								//��ȡԭʼ֡�ĳ���
	int GetTotalFrames(){ return totalFrames; }							//����֡�������鳤��
	int GetFrame(bool ahead = true){									//��ȡ��ǰ֡���к�
		if (ahead == true){
			return forward;
		}
		else{
			return backward;
		}
	}
	void SetFrame(int sequenceIndex,bool ahead=true){					//���õ�ǰ֡���к�
		if (ahead == true){
			forward=sequenceIndex;
		}
		else{
			backward=sequenceIndex;
		}
	}
	void SetSequence(int* sequence,int length){							//�����µ�֡����
		frameSequence = sequence;
		totalFrames = length;
	}
	//�������صĲ���
	int GetDir(POINT mousePT);											//��������жϷ���
	//����ɫ�Ƿ񵽴���괦
	bool MoveTo(IN POINT mousePT,IN POINT destPT,IN RECT Boundary);
	//�����ʼ�������ƺ���
	void Initiate(SPRITEINFO spInfo);									//��ʼ���������
	void Draw(HDC hdc);													//����������

	POINT GetMapBlockPT() { return mapBlockPT; }						//��ȡ��ײ���ϰ�ʱ��λ��

	bool CollideWith(IN T_Map* map);									//����ͼ��ײ
	bool MoveTo(IN POINT mousePT,IN POINT destPT,IN T_Map* map);		//����ɫ�Ƿ񵽴��ͼ��괦
};

//������������������ײ��������(px��pyΪ��ֵ�Ŵ󡢸�ֵ��С����λΪ����)
inline void T_Sprite::AdjustCollideRect(int px, int py){
	if (px == 0 && py == 0){
		return;
	}
	else{
		RECT tempRec = { 0, 0, collideWidth, collideHeight };
		InflateRect(&tempRec,px,py);
		collideWidth = tempRec.right - tempRec.left;
		collideHeight = tempRec.bottom - tempRec.top;
	}
}

//������ź��ʵ����ײ����������
inline RECT* T_Sprite::GetCollideRect(){
	int c_left, c_top;
	if (frameRatio > 0){
		c_left = (GetRatioSize().cx - collideWidth) / 2;
		c_top = (GetRatioSize().cy - collideHeight) / 2;
	}
	else{
		c_left = ((int)GetWidth() - collideWidth) / 2;
		c_top = ((int)GetHeight() - collideHeight) / 2;
	}
	collideRect.left = (LONG)X + c_left;
	collideRect.right = collideRect.left + collideWidth;
	collideRect.top = (LONG)Y + c_top;
	collideRect.bottom = collideRect.top + collideHeight;
	return &collideRect;
}

//��Ȿ��ɫ�����Ƿ���Ŀ���ɫ��ײ��diatance����Ϊ�����룩
inline bool T_Sprite::CollideWith(T_Sprite* target,int distance){
	//���������ײ���ľ����������
	RECT targetRect = *(target->GetCollideRect());
	RECT hitRec;
	hitRec.left = targetRect.left - distance;
	hitRec.top = targetRect.top - distance;
	hitRec.right = targetRect.right + distance;
	hitRec.bottom = targetRect.bottom + distance;
	RECT thisRect = *(this->GetCollideRect());
	int cw = abs(thisRect.right - thisRect.left);
	int tw = abs(hitRec.right - hitRec.left);
	int ch = abs(thisRect.bottom-thisRect.top);
	int th = abs(hitRec.bottom-hitRec.top);
	//��ʽһ��ͬʱ�����ĸ�����
	return thisRect.left <= hitRec.right && hitRec.left <= thisRect.right && thisRect.top <= hitRec.bottom && hitRec.top <= thisRect.bottom;
	//��ʽ����ͬʱ����2������
	//return (abs((thisRect.left + cw / 2) - (hitRec.left + tw / 2)) < (cw + tw) / 2) && (abs((thisRect.top + ch / 2) - (hitRec.top + th / 2)) < (ch + th) / 2);
}