#include"stdafx.h"
#include"T_Display.h"
//���캯��
T_Display::T_Display():mode_changed(false){
	SaveMode();
}

//��������
T_Display::~T_Display(){
	ResetMode();
}

//�洢���ڵ���ʾģʽ
void T_Display::SaveMode(){
	HDC dc = GetDC(0);
	devmode_saved.dmSize = sizeof(devmode_saved);
	devmode_saved.dmDriverExtra = 0;
	devmode_saved.dmPelsWidth = GetDeviceCaps(dc,HORZRES);
	devmode_saved.dmPelsHeight = GetDeviceCaps(dc,VERTRES);
	devmode_saved.dmBitsPerPel = GetDeviceCaps(dc,BITSPIXEL);
	devmode_saved.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
	//�ж�Windows�汾�Ƿ�ΪWindows NT/2000

	OSVERSIONINFOEX osvi;
	DWORDLONG dwlConditionMask = 0;
	//Initialize the OSVERSIONINFOEX structure.
	ZeroMemory(&osvi, sizeof(osvi));
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	osvi.dwMajorVersion = 5;
	osvi.dwMinorVersion =1 ;
	//system major version < dwMajorVersion
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_LESS);
	if (!VerifyVersionInfo(&osvi, VER_MAJORVERSION, dwlConditionMask)){
		devmode_saved.dmFields |= DM_DISPLAYFREQUENCY;
		devmode_saved.dmDisplayFrequency = GetDeviceCaps(dc, VREFRESH);
	}
	ReleaseDC(0,dc);
}

//��ԭ��֮ǰ�洢����ʾģʽ
void T_Display::ResetMode(){
	if (mode_changed){
		ChangeDisplaySettings(&devmode_saved,0);
		mode_changed = false;
	}
}

//������ʾģʽ
bool T_Display::ChangeMode(int width,int height){
	DEVMODE devmode;
	devmode = devmode_saved;
	devmode.dmPelsWidth = width;
	devmode.dmPelsHeight = height;
	devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	if (ChangeDisplaySettings(&devmode,CDS_FULLSCREEN)==DISP_CHANGE_SUCCESSFUL){
		mode_changed = true;
		return true;
	}
	return false;
}