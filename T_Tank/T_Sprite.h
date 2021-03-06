#pragma once;
#include"T_Graphic.h"
#include"T_Layer.h"
#include"T_Util.h"
#include"T_Map.h"

#define ROUND(x) (int)(x+0.5)											//定义四舍五入的宏

class T_Sprite :public T_Layer{
protected: 
	//与角色图像相关的属性................................................................
	T_Graph		spImg;													//原始角色图片
	T_Graph		spOldImg;												//修改前角色图片
	int			frameCols;												//原始帧图片总列数
	int			frameRows;												//原始帧图片总行数
	int			rawFrames;												//原始帧图片总帧数
	int			totalFrames;											//当前帧图片总帧数
	int			forward;												//从前往后播放帧计数
	int			backward;												//从后往前播放帧计数
	bool		loopForward;											//是否从前往后播放帧
	int*		frameSequence;											//自定义动画帧序列
	float		frameRatio;												//帧图片放大或缩小比（1原始，大于1放大，小于1缩小）
	int			frameRotate;											//帧图片旋转或翻转方式（取值为TRANSFER常量值）
	BYTE		frameAlpha;												//帧图片透明度（255不透明，0完全透明）
	//与角色状态相关的属性................................................................
	int			dir;													//初始方向
	int			lastDir;												//上次方向
	bool		active;													//是否可活动
	bool		dead;													//是否死亡
	int			speed;													//初始移动速度
	int			level;													//初始游戏等级
	int			score;													//游戏分值
	DWORD		startTime;												//计时开始时间
	DWORD		endTime;												//计时结束时间
	//与碰撞检测相关的属性................................................................
	RECT		collideRect;											//碰撞检测矩形区域
	int			collideWidth;											//碰撞区域宽度
	int			collideHeight;											//碰撞区域高度

	POINT mapBlockPT;													//碰撞到障碍时的位置
public:
	//构造函数、析构函数及类名获取函数.....................................................
	//frameWidth和frameHeight都为0：角色图像无动画
	//frameWidth和frameHeight都大于0：角色图像包含帧动画
	T_Sprite(LPCTSTR imgPath,int frameWidth=0,int frameHeight=0);
	virtual ~T_Sprite(void);
	virtual string ClassName(){ return "T_Sprite"; }					//获取本类名
	//与图像相关的操作.....................................................................
	T_Graph* GetImage(){ return &spImg; }								//获取本类T_Graph对象
	void SetImage(T_Graph* pImg){ spImg = *pImg; }						//设置本类T_Graph对象
	void ResetImage(){ spImg = spOldImg; }								//恢复本类T_Graph对象
	float GetRatio(){ return frameRatio; }								//获得缩放比
	void SetRatio(float rat){ frameRatio = rat; }						//设置缩放比
	SIZE GetRatioSize(){												//根据缩放比计算帧图片宽、高
		SIZE ratioFrameSize;
		ratioFrameSize.cx = ROUND(frameRatio*Width);
		ratioFrameSize.cy = ROUND(frameRatio*Height);
		return ratioFrameSize;
	}
	int GetRotation(){ return frameRotate; }							//获得图片转换方式
	void SetRotation(int rot){ frameRotate = rot; }						//设置图像转换方式（rot为TRANSFER常量值）
	BYTE GetAlpha(){ return frameAlpha; }								//获得透明度值
	void SetAlpha(BYTE a){ frameAlpha = a; }							//设置透明度值
	//与状态相关的操作.....................................................................
	int GetDir(){ return dir; }											//获得方向值
	void SetDir(int d){ dir = d; };										//设置方向值
	bool IsActive(){ return active; }									//是否可活动
	void SetActive(int act){ active = act; }							//设置是否可活动
	bool IsDead(){ return dead; }										//死亡状态
	void SetDead(bool dd){ dead = dd; }									//设置死亡状态
	int GetSpeed(){ return speed; }										//获取速度值
	void SetSpeed(int spd){ speed = spd; }								//设置速度值
	int GetLevel(){ return level; }										//获取等级值
	void SetLevel(int lvl){ level = lvl; }								//设置等级值
	int GetScore(){ return score; }										//获取分值
	void SetScore(int scr){ score = scr; }								//设置分值
	//与计时相关的操作.....................................................................
	void SetEndTime(DWORD time){ endTime = time; }						//设置计时结束时间
	DWORD GetEndTime(){ return endTime; }								//获取计时结束时间
	void SetStartTime(DWORD time){ startTime = time; }					//设置计时开始时间
	DWORD GetStartTime(){ return startTime; }							//获得计时开始时间
	//与碰撞相关的操作......................................................................
	//计算扩大或收缩后的碰撞检测区宽度（px、py为正直放大、负值缩小，单位为像素）
	void AdjustCollideRect(int px = 0, int py = 0);
	RECT* GetCollideRect();												//获得检测碰撞的矩形区域
	//检测角色碰撞，distance检测碰撞的距离
	bool CollideWith(T_Sprite* target,int distance=0);
	//与动画帧相关的操作....................................................................
	void LoopFrame(bool ahead=true);									//往前或往后循环播放所有帧
	bool LoopFrameOnce(bool ahead = true);								//往前或往后播放一轮所有帧
	int GetRawFrames(){ return rawFrames; }								//获取原始帧的长度
	int GetTotalFrames(){ return totalFrames; }							//计算帧序列数组长度
	int GetFrame(bool ahead = true){									//获取当前帧序列号
		if (ahead == true){
			return forward;
		}
		else{
			return backward;
		}
	}
	void SetFrame(int sequenceIndex,bool ahead=true){					//设置当前帧序列号
		if (ahead == true){
			forward=sequenceIndex;
		}
		else{
			backward=sequenceIndex;
		}
	}
	void SetSequence(int* sequence,int length){							//设置新的帧序列
		frameSequence = sequence;
		totalFrames = length;
	}
	//与鼠标相关的操作
	int GetDir(POINT mousePT);											//根据鼠标判断方向
	//检测角色是否到达鼠标处
	bool MoveTo(IN POINT mousePT,IN POINT destPT,IN RECT Boundary);
	//本类初始化及绘制函数
	void Initiate(SPRITEINFO spInfo);									//初始化本类对象
	void Draw(HDC hdc);													//本类对象绘制

	POINT GetMapBlockPT() { return mapBlockPT; }						//获取碰撞到障碍时的位置

	bool CollideWith(IN T_Map* map);									//检测地图碰撞
	bool MoveTo(IN POINT mousePT,IN POINT destPT,IN T_Map* map);		//检测角色是否到达地图鼠标处
};

//计算扩大或收缩后的碰撞检测区宽高(px、py为正值放大、负值缩小，单位为像素)
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

//获得缩放后的实际碰撞检测矩形区域
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

//检测本角色对象是否与目标角色碰撞（diatance参数为检测距离）
inline bool T_Sprite::CollideWith(T_Sprite* target,int distance){
	//计算参与碰撞监测的精灵矩形区域
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
	//方式一：同时满足四个条件
	return thisRect.left <= hitRec.right && hitRec.left <= thisRect.right && thisRect.top <= hitRec.bottom && hitRec.top <= thisRect.bottom;
	//方式二：同时满足2个条件
	//return (abs((thisRect.left + cw / 2) - (hitRec.left + tw / 2)) < (cw + tw) / 2) && (abs((thisRect.top + ch / 2) - (hitRec.top + th / 2)) < (ch + th) / 2);
}