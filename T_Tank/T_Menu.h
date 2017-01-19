#pragma once
#include"T_Config.h"
#include"T_Graphic.h"


class T_Menu{
private:
	MENU_INFO menu_info;									//�˵���Ϣ
	vector<MENUITEM> gm_menuItems;							//�˵���Ŀ
	T_Graph gm_menuBkg;										//�˵�����
	T_Graph BtnDIB;											//�˵�ͼƬ

	int bkImageAlpha;										//����ͼƬ͸����
	int bkColor;											//������ɫ

	bool isItemFocused;										//��ǰ�˵��Ƿ��ý���
	int m_index;											//��ǰ����˵�������
	int lastIndex;											//��¼��һ�εĲ˵�������
	int MaxMenuItemLen;										//��˵����ֳ���

	int GetMenuIndex(int x,int y);							//���ݵ�ǰ����������˵�������
	StringAlignment GetAlignmemt();							//��ȡ�˵����䷽ʽ
	FontStyle GetFontStyle();								//��ȡ�˵�������
	
public:
	T_Menu();
	virtual ~T_Menu(void);

	//���ò˵�����ͼƬ��͸���ȼ�������ɫ
	void SetMenuBkg(LPCTSTR img_path,int alphaLevel=255,COLORREF backColor=RGB(0,0,0));
	//���ò˵���ťͼƬ��ͼƬ��ť��ͼƬ��ť��
	void SetBtnBmp(LPCTSTR img_path,int btnWidth,int btnHeight);
	void SetMenuInfo(MENU_INFO menuInfo);					//���ò˵�������Ϣ
	void AddMenuItem(MENUITEM menuItem);					//��Ӳ˵�������Ŀ
	//�˵�����
	// bkgX��bkgY�Ǳ���ͼ��Ҫ���Ƶ����꣬Ĭ��Ϊ0��0
	// btnTrans��ť͸���ȣ�255��͸����Ĭ�ϲ�͸��
	//startState�Ƿ���ƿ�ʼ�˵���Ĭ��Ϊ��ʼ�˵�
	void DrawMenu(HDC hdc,int bkgX=0,int bkgY=0,BYTE btnTrans=255,bool startState=true);

	int MenuMouseClick(int x, int y);						//�˵�������¼�����
	void MenuMouseMove(int x, int y);						//�˵�����ƶ��¼�����
	int MenuKeyDown(WPARAM key);							//�˵������¼�����

	int GetMenuIndex(){ return m_index; }					//���ص�ǰ��m_index
	void SetMenuIndex(int i){ m_index = i; };				//���õ�ǰ��m_index

	void DestroyAll();										//�ͷ�ͼƬ��Դ
};