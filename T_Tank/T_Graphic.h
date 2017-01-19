#pragma once
#include"T_Engine.h"


class T_Graph{
protected:
	HBITMAP hBmp;									//�Ѽ���ͼ����
	int ImageWidth;									//�Ѽ���ͼ����
	int ImageHeight;								//�Ѽ���ͼ��߶�
	
public:
	T_Graph();										//T_Graph��Ĭ�Ϲ��캯��
	T_Graph(LPCTSTR fileName);						//ʹ��ָ���ļ�����T_Graph����
	virtual ~T_Graph();								//T_Graph�����������
	
	HBITMAP GetBmpHandle(){ return hBmp; }			//����Ѽ���ͼ����
	int GetImageWidth(){ return ImageWidth; }		//����Ѽ���ͼ���
	int GetImageHeight(){ return ImageHeight; }		//����Ѽ���ͼ���

public:
	/**
	 *����ͼ��֧��BMP,GIF,JPEG,PNG,TIFF��ʽ��
	 **/
	bool LoadImageFile(LPCTSTR path);

	/**
	 * ��ָ��λ�û���λͼ
	 **/
	void PaintImage(HDC hdc,int x,int y);

	/**
	 * ��ָ��λ����ָ�����߻���λͼ
	 **/
	void PaintImage(HDC hdc, int x, int y,int width,int height);

	/**
	 * ��ָ��λ����ָ�����ߺ�͸���Ȼ���λͼ
	 **/
	void PaintImage(HDC hdc, int x, int y, int width, int height,BYTE alpha);

	/**
	 * �ͷ���Դ
	 **/
	void Destroy();	

public:
	static HBITMAP CreateBlankBitmap(int width,int height,COLORREF color);
	static Bitmap* HBITMAP_To_Bitmap(HBITMAP hbmp,HDC hdc);
	static void PaintRegion(HBITMAP in_hbitmap,HDC destDC,int destX,int destY,int srcX,int srcY,int regionWidth,int regionHeight,float ratio,int rotType=0,BYTE alpha=255);
	
	/**
	 * �������Ŵ���
	 * 
	 * in_hbitmap		Ҫ������ԭʼλͼ�����
	 * destDC			ָ��Ҫ���Ƶ�Ŀ���豸
	 * destX��destY		ָ����Ŀ���豸�ϻ��Ƶ�����λ��
	 * FrameCount		��ǰҪ���Ƶ�֡ͼ���к�
	 * RowFrames		����֡ͼ���ļ���ÿ��֡ͼ������
	 * wFrame			֡ͼ���
	 * hFrame			֡ͼ�߶�
	 * ratio			ָ�����ű��ʣ�ֵΪ1����ԭ����С��1��С������1�Ŵ�
	 * alpha			����ʱ��͸���ȣ�Ĭ��ֵΪ��͸����
	 * rotType			ָ��λͼ��ת��ת���������ͣ��ò�����ֵ������T_Config.hͷ�ļ��ж����
	 *					TRANSFERö�ٳ���ֵ֮һ��Ĭ��ֵΪ0��Ҳ���ǲ������κ���ת��ת������					
	 **/
	static void PaintFrame(HBITMAP in_hbitmap,HDC destDC,int destX,int destY,int FrameCount,int RowFrames,int wFrame,int hFrame,float ratio=1,int rotType=0,BYTE alpha=255);
	
	static void PaintText(HDC hdc,RectF fontRect,LPCTSTR text,REAL fontSize,LPCTSTR fontName,Color fontColor=Color::White,FontStyle style=FontStyleBold,StringAlignment align=StringAlignmentCenter);
	/**
	 * ��ָ����ɫ���ƿհ�λͼ
	 **/
	static void PaintBlank(HBITMAP hbmp,int width,int height,COLORREF crColor);

	/**
	* ��ָ����ɫ���ƿհ�λͼ
	**/
	static void PaintBlank(HDC hdc,int x,int y,int width,int height,Color crColor);

	/**
	* ��ָ����ɫ���ƿհ�λͼ
	**/
	static void PaintBlank(HDC hdc, int x, int y, int width, int height, COLORREF crColor, int alphaLevel);

};