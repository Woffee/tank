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

//���ò˵�����ͼƬ
void T_Menu::SetMenuBkg(LPCTSTR img_path, int alphaLevel, COLORREF backColor){
	if (wcslen(img_path) > 0){
		gm_menuBkg.LoadImageFile(img_path);
		bkColor = backColor;
		bkImageAlpha = alphaLevel;
	}
}

//��������˵�ͼƬ
void T_Menu::SetBtnBmp(LPCTSTR img_path, int btnWidth, int btnHeight){
	if (wcslen(img_path) > 0){
		BtnDIB.LoadImageFile(img_path);
		if (&BtnDIB != NULL){
			menu_info.width = btnWidth;
			menu_info.height = btnHeight;
		}
	}
}

//��Ӳ˵�������Ŀ
void T_Menu::AddMenuItem(MENUITEM menuItem){
	gm_menuItems.push_back(menuItem);
	int len = menuItem.ItemName.length();
	if (MaxMenuItemLen < len){
		MaxMenuItemLen = len;
	}
}

//�����ײ˵�������꼰�˵��������
void T_Menu::SetMenuInfo(MENU_INFO menuInfo){
	menu_info = menuInfo;
}

//�˵�����
void T_Menu::DrawMenu(HDC hdc, int bkgX, int bkgY, BYTE btnTrans, bool startState){
	//�����ǰΪ��Ϸ��ʼ�����˵�
	if (&gm_menuBkg != NULL && startState == true){
		//���ƴ�ɫ�ı���
		HBITMAP tBmp = T_Graph::CreateBlankBitmap(WIN_WIDTH,WIN_HEIGHT,bkColor);
		SelectObject(hdc,tBmp);
		//����ָ��͸���Ȼ��Ʊ���ͼƬ
		gm_menuBkg.PaintImage(hdc,0,0,WIN_WIDTH,WIN_HEIGHT,bkImageAlpha);
		//�ͷ���ʱͼƬ��Դ
		DeleteObject(tBmp);
		tBmp = NULL;
	}
	else
	{
		//���ƺ�ɫ��͸���Ŀհ�ͼƬ
		T_Graph::PaintBlank(hdc,0,0,WIN_WIDTH,WIN_HEIGHT,MENU_BKCLR,MENU_ALPHA);
		//��������˱���ͼƬ�����Ʊ���ͼƬ
		if (&gm_menuBkg != NULL){
			gm_menuBkg.PaintImage(hdc,bkgX,bkgY,gm_menuBkg.GetImageWidth(),gm_menuBkg.GetImageHeight(),bkImageAlpha);
		}
	}

	//��ȡ�˵���Ŀ��
	int w = menu_info.width;
	int h = menu_info.height;

	//���ݲ˵���Ŀ�߼�����ʵ��ֺ�
	int FontHeight;
	int px = 0;
	int w_px = w / (MaxMenuItemLen + 1);						//����ÿ������ռ������
	int h_px = (int)((float)(h / 2.5));
	if (w_px > h_px){
		px = h_px;
	}
	else
	{
		px = w_px;
	}
	FontHeight = (px * 72) / 96;								//����ÿ���ֵ����ؼ����ֺ�

	int mIndex = 0;
	Gdiplus::RectF Rec;
	vector<MENUITEM>::iterator iter;
	//����ȫ���˵�����Ϣ
	for (iter = gm_menuItems.begin(); iter != gm_menuItems.end(); iter++){
		//��ȡ�˵��������
		int x = iter->pos.x;
		int y = iter->pos.y;
		//��ȡ�˵���ľ�������
		Rec.X = (float)x;
		Rec.Y = (float)y;
		Rec.Width = (float)w;
		Rec.Height = (float)h;
		//���ư�ťͼ��
		if (&BtnDIB != NULL){
			//�����ǰ�˵���Ϊ����״̬
			if (isItemFocused == FALSE || (isItemFocused == TRUE && mIndex != m_index)){
				BtnDIB.PaintRegion(BtnDIB.GetBmpHandle(),hdc,x,y,0,0,w,h,1,0,btnTrans);
			}

			//�����ǰ�˵���Ϊѡ��״̬
			if (isItemFocused == TRUE&&mIndex == m_index){
				BtnDIB.PaintRegion(BtnDIB.GetBmpHandle(), hdc, x, y, 0, 0, w, h, 1, 0, btnTrans);
			}
		}
		//��ȡ��ǰ�˵�������
		LPCTSTR item = iter->ItemName.c_str();
		//�����ǰ�˵���Ϊ����״̬
		if (isItemFocused == FALSE || (isItemFocused == TRUE&&mIndex != m_index)){
			//��������״̬�µĲ˵�������
			T_Graph::PaintText(hdc,Rec,item,(REAL)FontHeight,menu_info.fontName,menu_info.normalTextColor,GetFontStyle(),GetAlignmemt());
		}
		//�����ǰ�˵���Ϊѡ��״̬
		if (isItemFocused == TRUE&&mIndex == m_index){
			//����ѡ��״̬�µĲ˵�������
			T_Graph::PaintText(hdc, Rec, item, (REAL)FontHeight, menu_info.fontName, menu_info.focusTextColor, GetFontStyle(), GetAlignmemt());
		}
		mIndex++;
	}
}

//���ݵ�ǰ����������˵���������
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
	//����ȫ���˵�����Ϣ
	for (iter = gm_menuItems.begin(); iter != gm_menuItems.end(); iter++){
		//��ȡ�˵���ľ�������
		rec.left = iter->pos.x;
		rec.top = iter->pos.y;
		rec.right = rec.left + w;
		rec.bottom = rec.top + h;
		//�жϸò˵�����������Ƿ������������
		if (PtInRect(&rec, pt)){
			//���ذ�����������Ĳ˵����������
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

//�˵�������¼�����
int T_Menu::MenuMouseClick(int x,int y){
	m_index = GetMenuIndex();
	return m_index;
}

//�˵�����ƶ��¼�����
void T_Menu::MenuMouseMove(int x, int y){
	lastIndex = m_index;									//��¼ǰһ�ε�����ֵ
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
	//ʹ��swap�����Ԫ�ز������ڴ�
	//�����������С����������
	gm_menuItems.swap(vector<MENUITEM>());
}