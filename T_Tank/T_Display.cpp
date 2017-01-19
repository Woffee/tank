#include"stdafx.h"
#include"T_Display.h"
//构造函数
T_Display::T_Display():mode_changed(false){
	SaveMode();
}

//析构函数
T_Display::~T_Display(){
	ResetMode();
}

//存储现在的显示模式
void T_Display::SaveMode(){
	HDC dc = GetDC(0);
	devmode_saved.dmSize = sizeof(devmode_saved);
	devmode_saved.dmDriverExtra = 0;
	devmode_saved.dmPelsWidth = GetDeviceCaps(dc,HORZRES);
	devmode_saved.dmPelsHeight = GetDeviceCaps(dc,VERTRES);
	devmode_saved.dmBitsPerPel = GetDeviceCaps(dc,BITSPIXEL);
	devmode_saved.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
	//判断Windows版本是否为Windows NT/2000

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

//还原成之前存储的显示模式
void T_Display::ResetMode(){
	if (mode_changed){
		ChangeDisplaySettings(&devmode_saved,0);
		mode_changed = false;
	}
}

//更改显示模式
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