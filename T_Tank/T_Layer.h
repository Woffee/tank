#pragma once
#include"T_Config.h"

class T_Layer{
public:
	T_Layer(){};
	virtual ~T_Layer(){};
protected:
	int Width, Height;															//ͼ��Ŀ���
	int X, Y;																	//ͼ���x��y����
	bool Visible;																//ͼ���Ƿ�ɼ�
	int LayerTypeID;															//ͼ��ı�ʶ��ȡֵΪLAYER_TYPE������
	int zorder;																	//ͼ��Ļ���˳��
public:
	//���úͻ�ȡͼ��Ŀ���
	void SetWidth(int width){ Width = width; }
	int GetWidth(){ return Width; }
	void SetHeight(int height){ Height = height; }
	int GetHeight(){ return Height; }
	//���úͻ�ȡͼ���x��y����
	void SetPosition(int x, int y){ X = x; Y = y; }
	int GetX(){ return X; }
	int GetY(){ return Y; }
	//���úͻ�ȡͼ��ɼ���
	void SetVisible(bool visible){ Visible = visible; }
	bool IsVisible(){ return Visible; }
	//���úͻ�ȡͼ��ı�ʶ��ȡֵΪLAYER_TYPE������
	void SetLayerTypeID(int i){ LayerTypeID = i; }
	int GetLayerTypeID(){ return LayerTypeID; }
	//���úͻ�ȡͼ��Ļ���˳��
	int GetZorder(){ return zorder; }
	void SetZorder(int z){ zorder = z; }
	//dx_speed:��ֱ�����ƶ�����ֵ�����ƶ���dy_speed:��ֱ�����ƶ�����ֵ�����ƶ�
	void Move(int dx_speed, int dy_speed){ X += dx_speed; Y += dy_speed; }			//ͼ���ƶ�
	virtual string ClassName(){ return "T_Layer"; }									//��ȡ���༰�����������
	virtual void Draw(HDC hdc) = 0;//���麯���������������ʵ��
};