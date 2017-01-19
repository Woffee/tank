#include"stdafx.h"

HWND T_Engine::m_hWnd = NULL; //窗口句柄
HINSTANCE T_Engine::m_hInstance = NULL; //实例句柄
T_Engine* T_Engine::pEngine = NULL; //引擎类指针

T_Engine::T_Engine(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle, WORD Icon, WORD SmIcon, int iWidth, int iHeight){
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&ptrGdiplusToken,&gdiplusStartupInput,NULL);
	pEngine = this;
	m_hInstance = hInstance;
	wndClass = szWindowClass;
	wndTitle = szTitle;
	wndWidth = iWidth;
	wndHeight = iHeight;
	m_bFullScreen = FALSE;
	wIcon = Icon;
	wSmIcon = SmIcon;
	p_disp = new T_Display();
	for (int i = 0; i < 256; i++){
		keys[i] = false;
	}
	//创建内存缓冲设备及内存缓冲位图
	bufferDC = CreateCompatibleDC(GetDC(m_hWnd));
	bufferBitmap = CreateCompatibleBitmap(GetDC(m_hWnd),wndWidth,wndHeight);
	SelectObject(bufferDC,bufferBitmap);
}

//析构函数
T_Engine::~T_Engine(){
	delete p_disp;
	Gdiplus::GdiplusShutdown(ptrGdiplusToken); //关闭GDI+
}

//回调函数
LONG CALLBACK T_Engine::WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
	return pEngine->GameEvent(hWnd,msg,wParam,lParam); //只调用类中的消息处理函数
}

//游戏窗口初始化处理
BOOL T_Engine::GameWinInit(){
	WNDCLASSEX gamewin;
	gamewin.cbSize = sizeof(WNDCLASSEX);
	gamewin.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	gamewin.lpfnWndProc = WndProc;
	gamewin.cbClsExtra = 0;
	gamewin.cbWndExtra = 0;
	gamewin.hInstance = m_hInstance;
	gamewin.hIcon = LoadIcon(m_hInstance,MAKEINTRESOURCE(wIcon));
	gamewin.hCursor = LoadCursor(NULL,IDC_ARROW);
	gamewin.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	gamewin.lpszMenuName = NULL;
	gamewin.lpszClassName = wndClass;
	gamewin.hIconSm = LoadIcon(m_hInstance,MAKEINTRESOURCE(wSmIcon));
	if (!RegisterClassEx(&gamewin)) return FALSE;
	//使窗口居中，获取客户区全屏幕高度（去掉任务栏高度）
	//如果要获取完全全屏幕高度，包括任务栏高度，使用SM_CXSCREEN和SM_CYSCREEN
	scrnWidth = GetSystemMetrics(SM_CXFULLSCREEN); //屏幕宽度
	scrnHeight = GetSystemMetrics(SM_CYFULLSCREEN); //屏幕高度（去掉任务栏高度）
	int x = (scrnWidth - wndWidth) / 2;
	int y = (scrnHeight - wndHeight) / 2;
	m_hWnd = CreateWindow(wndClass,wndTitle,
						  WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX, //禁止调整大小
						  x,y,wndWidth+6,wndHeight+28, //加上窗口边框和标题栏占用
						  NULL,NULL,m_hInstance,NULL);
	if (!m_hWnd) return FALSE;
	if (m_bFullScreen == TRUE){ //如果是全屏显示
		GetWindowRect(m_hWnd,&m_rcOld); //记住当前窗口的尺寸
		style = GetWindowLong(m_hWnd,GWL_STYLE);
		ex_style = GetWindowLong(m_hWnd,GWL_EXSTYLE);
		p_disp->SaveMode();
		HWND hDesktop;
		RECT rc;
		hDesktop = GetDesktopWindow(); //获取桌面窗口的指针
		GetWindowRect(hDesktop,&rc); //得到桌面窗口的矩形
		p_disp->ChangeMode(wndWidth,wndHeight);
		//设置为WS_DLGFRAME、WS_BORDER或
		//WS_EX_CLIENTEDGE都可以去掉标题栏
		//设置扩展风格为WS_EX_WINDOWEDGE,去掉窗口边框
		SetWindowLong(m_hWnd,GWL_EXSTYLE,WS_EX_WINDOWEDGE);
		SetWindowLong(m_hWnd,GWL_STYLE,WS_BORDER);
		SetWindowPos(m_hWnd,HWND_TOP,-1,-1,rc.right,rc.bottom,SWP_SHOWWINDOW);
	}
	ShowWindow(m_hWnd,SW_NORMAL);
	UpdateWindow(m_hWnd);
	return TRUE;
}

//附加按键行为处理（此处处理Esc键）
void T_Engine::SubKeyAction(WPARAM wParam){
	int x, y;
	if (wParam == VK_ESCAPE){ //按下Esc键
		m_bFullScreen = !m_bFullScreen; //设置全屏显示标志
		if (!m_bFullScreen){ //恢复窗口模式
			p_disp->ResetMode();
			SetWindowLong(m_hWnd,GWL_STYLE,style);
			SetWindowLong(m_hWnd,GWL_EXSTYLE,ex_style);
			int oldWidth = m_rcOld.right - m_rcOld.left;
			int oldHeight = m_rcOld.bottom - m_rcOld.top;
			x = m_rcOld.left;
			y = m_rcOld.top;
			SetWindowPos(m_hWnd,HWND_NOTOPMOST,x,y,oldWidth,oldHeight,SWP_SHOWWINDOW);
		}
		else{ //显示退出程序对话框
			ShowCursor(true);
			if (IDOK == MessageBox(NULL, L"你确定要退出吗？", wndTitle, MB_OKCANCEL | MB_ICONQUESTION)){
				DestroyWindow(m_hWnd);
			}
		}
	}
}

LONG T_Engine::GameEvent(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
	int x, y;
	HDC hdc;
	switch (msg)
	{
	case WM_SETFOCUS:
		SetSleep(FALSE);
		return 0;
	case WM_KILLFOCUS:
		SetSleep(TRUE);
		return 0;
	case WM_CREATE:
		m_hWnd = hWnd;
		GameInit();
		return 0;
	case WM_LBUTTONDOWN:												//鼠标左键按下的处理
		GameMouseAction(LOWORD(lParam), HIWORD(lParam), MOUSE_LCLICK);
		return 0;
	case WM_MOUSEMOVE:													//鼠标移动处理
		GameMouseAction(LOWORD(lParam), HIWORD(lParam), MOUSE_MOVE);
		return 0;
	case WM_KEYDOWN:													//键被按下的处理
		keys[wParam] = true;											//记录对应的按键状态
		GameKeyAction(KEY_DOWN);									    //执行游戏按键行为
		SubKeyAction(wParam);											//附加按键行为处理
		return 0;
	case WM_KEYUP:														//释放按键处理
		keys[wParam] = false;
		GameKeyAction();
		GameKeyAction(KEY_UP);
		return 0;
	case WM_NCLBUTTONDBLCLK:											//禁止双击窗口标题栏最大化窗口
		if (HTCAPTION == wParam) return 0;
	case WM_SYSCOMMAND:
		if (wParam == SC_MAXIMIZE){
			m_bFullScreen = !m_bFullScreen;								//设置全屏显示标志
			if (m_bFullScreen){												//全屏幕显示
				GetWindowRect(hWnd, &m_rcOld);								//记住当前窗口的尺寸
				style = GetWindowLong(hWnd, GWL_STYLE);
				ex_style = GetWindowLong(hWnd, GWL_EXSTYLE);
				p_disp->SaveMode();
				HWND hDesktop;
				RECT rc;
				hDesktop = GetDesktopWindow(); //获取桌面窗口的指针
				GetWindowRect(hDesktop, &rc); //得到桌面窗口的矩形
				bool ok = p_disp->ChangeMode(wndWidth, wndHeight);
				//设置为WS_DLGFRAME、WS_BORDER或
				//WS_EX_CLIENTEDGE都可以去掉标题栏
				//设置扩展风格为WS_EX_WINDOWEDGE,去掉窗口边框
				SetWindowLong(m_hWnd, GWL_EXSTYLE, WS_EX_WINDOWEDGE);
				SetWindowLong(m_hWnd, GWL_STYLE, WS_BORDER);
				SetWindowPos(m_hWnd, HWND_TOP, -1, -1, rc.right, rc.bottom, SWP_SHOWWINDOW);
			}
		}
		else if (wParam == SC_CLOSE){
			if (IDOK == MessageBox(NULL, L"你确定要退出吗？", wndTitle, MB_OKCANCEL | MB_ICONQUESTION)){
				DestroyWindow(m_hWnd);
			}
		}
		else{
			//由于WM_SYSCOMMAND消息不是由程序控制的，所以必须将消息传给DefWindowProc,否则会造成其它消息无法处理。
			return DefWindowProc(hWnd,WM_SYSCOMMAND,wParam,lParam);
		}
		return 0;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;
	case WM_PAINT:																//窗口绘制
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd,&ps);
		GamePaint(hdc);
		EndPaint(hWnd,&ps);
		return 0;
	case WM_DESTROY:															//程序销毁
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

void T_Engine::StartEngine(){
	MSG msg;
	static int TickElapsed = 0;
	int nowTick;
	GameWinInit();															   //初始化游戏窗口
	srand((unsigned)time(NULL));											   //初始化随机数
	while (TRUE){															   //游戏主循环
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			//如果侦测到WM_QUIT则退出游戏循环
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
			if (!GetSleep()){												   //确保游戏没有进入睡眠状态
				nowTick = GetTickCount();
				if (nowTick > TickElapsed){									   //判断游戏循环的时间
					TickElapsed = nowTick + GetInterval();
					GameLogic();											   //处理游戏逻辑
					GamePaint(bufferDC);									   //在内存中绘制内容
					HDC hDC = GetDC(m_hWnd);								   //获取游戏窗口句柄及绘图设备
					//将内存设备中绘制的内容绘到屏幕上
					BitBlt(hDC,0,0,WIN_WIDTH,WIN_HEIGHT,bufferDC,0,0,SRCCOPY);
					ReleaseDC(m_hWnd,hDC);									   //释放设备
				}
			}
		}
	}
	pEngine->GameEnd();														   //游戏结束处理
}