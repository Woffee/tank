#pragma once

#include"T_Graphic.h"
#include"T_Layer.h"

class T_Map : public T_Layer
{
protected:
	int tile_width;												//ͼ���
	int tile_height;											//ͼ���
	int map_cols;												//��ͼͼ��������
	int map_rows;												//��ͼͼ��������
	int first_gid;												//ͼ���Ŵ�0����1��ʼ
	bool updated;												//��ͼͼ���Ƿ����
	T_Graph* graph;												//����ͼ��ͼ��򱳾�ͼ��
	HDC dc_buf;													//��ǰͼ���ڴ��豸
	HBITMAP hbmp_old;											//�滻ǰ��ԭʼλͼ���
	HBITMAP hbmp_layer;											//��ǰͼ��λͼ���
	vector<vector<int>> layer_date;								//��ͼͼ������
public:
	T_Map(LAYERINFO layerInfo);									//ʹ�õ�ͼ���ݹ���
	T_Map(LPCTSTR imgFilepath);									//ʹ�ñ���ͼƬ����
	virtual ~T_Map(void);										

	int GetTileWidth() { return tile_width; }					//���ͼ���
	int GetTileHeight() { return tile_height; }					//���ͼ���
	int GetMapCols() { return map_cols; }						//��ȡ��ͼͼ��������
	int GetMapRows() { return map_rows; }						//��ȡ��ͼͼ��������
	void SetUpdate(bool updt) { updated = updt; }				//����ͼ��λͼ�Ƿ����
	bool GetUpdate() { return updated; }						//��ȡͼ��λͼ�Ƿ����״̬
	virtual string ClassName() { return "T_Map"; }				//��ȡ��ǰ�������
	void SetTile(int col, int row, int tileIndex);				//�޸ĵ�ǰͼ��ָ���С��е�Ԫ��ֵ
	int GetTile(int col,int row);								//��ȡ��ǰͼ��ָ���С��е�Ԫ��ֵ

	void Redraw(HDC hdc);										//���»��Ƶ�ǰͼ��ȫ��ͼ��
	void Draw(HDC hdc);											//���ظ����е�ͬ������������ͼ�����

};