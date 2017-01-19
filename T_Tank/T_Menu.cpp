#include"stdafx.h"
#include"T_Menu.h"

T_Menu::T_Menu(){
	m_index = -1;
	isItemFocused = false;
	bkImageAlpha = 255;
	bkColor = RGB(0,0,0);
}

T_Menu::~T_Menu(){
	DestroyAll();
}

//设置菜单背景图片
void T_Menu::SetMenuBkg(LPCTSTR img_path, int alphaLevel, COLORREF backColor){
	if (wcslen(img_path) > 0){
		gm_menuBkg.LoadImageFile(img_path);
		bkColor = backColor;
		bkImageAlpha = alphaLevel;
	}
}

//添加正常菜单图片
void T_Menu::SetBtnBmp(LPCTSTR img_path, int btnWidth, int btnHeight){
	if (wcslen(img_path) > 0){
		BtnDIB.LoadImageFile(img_path);
		if (&BtnDIB != NULL){
			menu_info.width = btnWidth;
			menu_info.height = btnHeight;
		}
	}
}

//添加菜单文字项目
void T_Menu::AddMenuItem(MENUITEM menuItem){
	gm_menuItems.push_back(menuItem);
	int len = menuItem.ItemName.length();
	if (MaxMenuItemLen < len){
		MaxMenuItemLen = len;
	}
}

//设置首菜单项的坐标及菜单间隔距离
void T_Menu::SetMenuInfo(MENU_INFO menuInfo){
	menu_info = menuInfo;
}

//菜单绘制
void T_Menu::DrawMenu(HDC hdc, int bkgX, int bkgY, BYTE btnTrans, bool startState){
	//如果当前为游戏开始的主菜单
	if (&gm_menuBkg != NULL && startState == true){
		//绘制纯色的背景
		HBITMAP tBmp = T_Graph::CreateBlankBitmap(WIN_WIDTH,WIN_HEIGHT,bkColor);
		SelectObject(hdc,tBmp);
		//按照指定透明度绘制背景图片
		gm_menuBkg.PaintImage(hdc,0,0,WIN_WIDTH,WIN_HEIGHT,bkImageAlpha);
		//释放临时图片资源
		DeleteObject(tBmp);
		tBmp = NULL;
	}
	else
	{
		//绘制黑色半透明的空白图片
		T_Graph::PaintBlank(hdc,0,0,WIN_WIDTH,WIN_HEIGHT,MENU_BKCLR,MENU_ALPHA);
		//如果设置了背景图片，绘制背景图片
		if (&gm_menuBkg != NULL){
			gm_menuBkg.PaintImage(hdc,bkgX,bkgY,gm_menuBkg.GetImageWidth(),gm_menuBkg.GetImageHeight(),bkImageAlpha);
		}
	}

	//获取菜单项的宽高
	int w = menu_info.width;
	int h = menu_info.height;

	//根据菜单项的宽高计算合适的字号
	int FontHeight;
	int px = 0;
	int w_px = w / (MaxMenuItemLen + 1);						//计算每个字所占的像素
	int h_px = (int)((float)(h / 2.5));
	if (w_px > h_px){
		px = h_px;
	}
	else
	{
		px = w_px;
	}
	FontHeight = (px * 72) / 96;								//根据每个字的像素计算字号

	int mIndex = 0;
	Gdiplus::RectF Rec;
	vector<MENUITEM>::iterator iter;
	//遍历全部菜单项信息
	for (iter = gm_menuItems.begin(); iter != gm_menuItems.end(); iter++){
		//获取菜单项的坐标
		int x = iter->pos.x;
		int y = iter->pos.y;
		//获取菜单项的矩形区域
		Rec.X = (float)x;
		Rec.Y = (float)y;
		Rec.Width = (float)w;
		Rec.Height = (float)h;
		//绘制按钮图像
		if (&BtnDIB != NULL){
			//如果当前菜单项为正常状态
			if (isItemFocused == FALSE || (isItemFocused == TRUE && mIndex != m_index)){
				BtnDIB.PaintRegion(BtnDIB.GetBmpHandle(),hdc,x,y,0,0,w,h,1,0,btnTrans);
			}

			//如果当前菜单项为选中状态
			if (isItemFocused == TRUE&&mIndex == m_index){
				BtnDIB.PaintRegion(BtnDIB.GetBmpHandle(), hdc, x, y, 0, 0, w, h, 1, 0, btnTrans);
			}
		}
		//获取当前菜单项文字
		LPCTSTR item = iter->ItemName.c_str();
		//如果当前菜单项为正常状态
		if (isItemFocused == FALSE || (isItemFocused == TRUE&&mIndex != m_index)){
			//绘制正常状态下的菜单项文字
			T_Graph::PaintText(hdc,Rec,item,(REAL)FontHeight,menu_info.fontName,menu_info.normalTextColor,GetFontStyle(),GetAlignmemt());
		}
		//如果当前菜单项为选中状态
		if (isItemFocused == TRUE&&mIndex == m_index){
			//绘制选中状态下的菜单项文字
			T_Graph::PaintText(hdc, Rec, item, (REAL)FontHeight, menu_info.fontName, menu_info.focusTextColor, GetFontStyle(), GetAlignmemt());
		}
		mIndex++;
	}
}

//根据当前鼠标坐标计算菜单项索引号
int T_Menu::GetMenuIndex(int x, int y){
	int Index = -1;
	POINT pt;
	pt.x = x;
	pt.y = y;
	int w = menu_info.width;
	int h = menu_info.height;
	RECT rec;

	int iCount = 0;
	vector<MENUITEM>::iterator iter;
	//遍历全部菜单项信息
	for (iter = gm_menuItems.begin(); iter != gm_menuItems.end(); iter++){
		//获取菜单项的矩形区域
		rec.left = iter->pos.x;
		rec.top = iter->pos.y;
		rec.right = rec.left + w;
		rec.bottom = rec.top + h;
		//判断该菜单项矩形区域是否包含鼠标的坐标
		if (PtInRect(&rec, pt)){
			//返回包含鼠标的坐标的菜单项的索引号
			return iCount;
		}
		iCount++;
	}
	return Index;
}

StringAlignment T_Menu::GetAlignmemt(){
	StringAlignment sAlign;
	if (menu_info.align == 0){
		sAlign = StringAlignmentNear;
	}
	else if (menu_info.align == 1){
		sAlign = StringAlignmentCenter;
	}
	else if (menu_info.align == 2){
		sAlign = StringAlignmentFar;
	}
	return sAlign;
}

FontStyle T_Menu::GetFontStyle(){
	FontStyle fStyle;
	if (menu_info.isBold == true){
		fStyle = FontStyleBold;
	}
	else{
		fStyle = FontStyleRegular;
	}
	return fStyle;
}

//菜单鼠标点击事件处理
int T_Menu::MenuMouseClick(int x,int y){
	m_index = GetMenuIndex();
	return m_index;
}

//菜单鼠标移动事件处理
void T_Menu::MenuMouseMove(int x, int y){
	lastIndex = m_index;									//记录前一次的索引值
	m_index = GetMenuIndex(x,y);
	if (m_index >= 0){
		isItemFocused = true;
	}
	else{
		isItemFocused = false;
	}
}

int T_Menu::MenuKeyDown(WPARAM key){
	if (key == VK_UP || key == VK_LEFT){
		m_index = m_index - 1;
		if (m_index < 0){
			m_index = (int)(gm_menuItems.size() - 1);
		}
		isItemFocused = true;
	}
	if (key == VK_RIGHT || key == VK_DOWN){
		m_index++;
		if (m_index>(int)(gm_menuItems.size() - 1)){
			m_index = 0;
		}
		isItemFocused = true;
	}
	return m_index;
}

void T_Menu::DestroyAll(){
	gm_menuItems.clear();
	//使用swap，清除元素并回收内存
	//清除容器并最小化它的容量
	gm_menuItems.swap(vector<MENUITEM>());
}