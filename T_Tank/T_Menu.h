#pragma once
#include"T_Config.h"
#include"T_Graphic.h"


class T_Menu{
private:
	MENU_INFO menu_info;									//菜单信息
	vector<MENUITEM> gm_menuItems;							//菜单项目
	T_Graph gm_menuBkg;										//菜单背景
	T_Graph BtnDIB;											//菜单图片

	int bkImageAlpha;										//背景图片透明度
	int bkColor;											//背景颜色

	bool isItemFocused;										//当前菜单是否获得焦点
	int m_index;											//当前焦点菜单索引号
	int lastIndex;											//记录上一次的菜单索引号
	int MaxMenuItemLen;										//最长菜单文字长度

	int GetMenuIndex(int x,int y);							//根据当前鼠标坐标计算菜单索引号
	StringAlignment GetAlignmemt();							//获取菜单对其方式
	FontStyle GetFontStyle();								//获取菜单字体风格
	
public:
	T_Menu();
	virtual ~T_Menu(void);

	//设置菜单背景图片、透明度及背景颜色
	void SetMenuBkg(LPCTSTR img_path,int alphaLevel=255,COLORREF backColor=RGB(0,0,0));
	//设置菜单按钮图片、图片按钮宽、图片按钮高
	void SetBtnBmp(LPCTSTR img_path,int btnWidth,int btnHeight);
	void SetMenuInfo(MENU_INFO menuInfo);					//设置菜单具体信息
	void AddMenuItem(MENUITEM menuItem);					//添加菜单文字项目
	//菜单绘制
	// bkgX和bkgY是背景图像要绘制的坐标，默认为0，0
	// btnTrans按钮透明度，255不透明，默认不透明
	//startState是否绘制开始菜单，默认为开始菜单
	void DrawMenu(HDC hdc,int bkgX=0,int bkgY=0,BYTE btnTrans=255,bool startState=true);

	int MenuMouseClick(int x, int y);						//菜单鼠标点击事件处理
	void MenuMouseMove(int x, int y);						//菜单鼠标移动事件处理
	int MenuKeyDown(WPARAM key);							//菜单按键事件处理

	int GetMenuIndex(){ return m_index; }					//返回当前的m_index
	void SetMenuIndex(int i){ m_index = i; };				//设置当前的m_index

	void DestroyAll();										//释放图片资源
};