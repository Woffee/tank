#pragma once
#include"T_Engine.h"


class T_Graph{
protected:
	HBITMAP hBmp;									//已加载图像句柄
	int ImageWidth;									//已加载图像宽度
	int ImageHeight;								//已加载图像高度
	
public:
	T_Graph();										//T_Graph类默认构造函数
	T_Graph(LPCTSTR fileName);						//使用指定文件创建T_Graph对象
	virtual ~T_Graph();								//T_Graph类的析构函数
	
	HBITMAP GetBmpHandle(){ return hBmp; }			//获得已加载图像句柄
	int GetImageWidth(){ return ImageWidth; }		//获得已加载图像宽
	int GetImageHeight(){ return ImageHeight; }		//获得已加载图像高

public:
	/**
	 *加载图像（支持BMP,GIF,JPEG,PNG,TIFF格式）
	 **/
	bool LoadImageFile(LPCTSTR path);

	/**
	 * 在指定位置绘制位图
	 **/
	void PaintImage(HDC hdc,int x,int y);

	/**
	 * 在指定位置以指定宽、高绘制位图
	 **/
	void PaintImage(HDC hdc, int x, int y,int width,int height);

	/**
	 * 在指定位置以指定宽、高和透明度绘制位图
	 **/
	void PaintImage(HDC hdc, int x, int y, int width, int height,BYTE alpha);

	/**
	 * 释放资源
	 **/
	void Destroy();	

public:
	static HBITMAP CreateBlankBitmap(int width,int height,COLORREF color);
	static Bitmap* HBITMAP_To_Bitmap(HBITMAP hbmp,HDC hdc);
	static void PaintRegion(HBITMAP in_hbitmap,HDC destDC,int destX,int destY,int srcX,int srcY,int regionWidth,int regionHeight,float ratio,int rotType=0,BYTE alpha=255);
	
	/**
	 * 动画播放处理
	 * 
	 * in_hbitmap		要操作的原始位图句柄。
	 * destDC			指定要绘制的目标设备
	 * destX、destY		指定在目标设备上绘制的坐标位置
	 * FrameCount		当前要绘制的帧图序列号
	 * RowFrames		保存帧图的文件中每行帧图的数量
	 * wFrame			帧图宽度
	 * hFrame			帧图高度
	 * ratio			指定缩放比率，值为1保持原样，小于1缩小，大于1放大。
	 * alpha			绘制时的透明度，默认值为不透明。
	 * rotType			指定位图旋转或翻转操作的类型，该参数的值必须是T_Config.h头文件中定义的
	 *					TRANSFER枚举常量值之一，默认值为0，也就是不进行任何旋转或翻转操作。					
	 **/
	static void PaintFrame(HBITMAP in_hbitmap,HDC destDC,int destX,int destY,int FrameCount,int RowFrames,int wFrame,int hFrame,float ratio=1,int rotType=0,BYTE alpha=255);
	
	static void PaintText(HDC hdc,RectF fontRect,LPCTSTR text,REAL fontSize,LPCTSTR fontName,Color fontColor=Color::White,FontStyle style=FontStyleBold,StringAlignment align=StringAlignmentCenter);
	/**
	 * 用指定颜色绘制空白位图
	 **/
	static void PaintBlank(HBITMAP hbmp,int width,int height,COLORREF crColor);

	/**
	* 用指定颜色绘制空白位图
	**/
	static void PaintBlank(HDC hdc,int x,int y,int width,int height,Color crColor);

	/**
	* 用指定颜色绘制空白位图
	**/
	static void PaintBlank(HDC hdc, int x, int y, int width, int height, COLORREF crColor, int alphaLevel);

};