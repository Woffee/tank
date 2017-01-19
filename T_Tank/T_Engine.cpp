#include"stdafx.h"

HWND T_Engine::m_hWnd = NULL; //���ھ��
HINSTANCE T_Engine::m_hInstance = NULL; //ʵ�����
T_Engine* T_Engine::pEngine = NULL; //������ָ��

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
	//�����ڴ滺���豸���ڴ滺��λͼ
	bufferDC = CreateCompatibleDC(GetDC(m_hWnd));
	bufferBitmap = CreateCompatibleBitmap(GetDC(m_hWnd),wndWidth,wndHeight);
	SelectObject(bufferDC,bufferBitmap);
}

//��������
T_Engine::~T_Engine(){
	delete p_disp;
	Gdiplus::GdiplusShutdown(ptrGdiplusToken); //�ر�GDI+
}

//�ص�����
LONG CALLBACK T_Engine::WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
	return pEngine->GameEvent(hWnd,msg,wParam,lParam); //ֻ�������е���Ϣ������
}

//��Ϸ���ڳ�ʼ������
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
	//ʹ���ھ��У���ȡ�ͻ���ȫ��Ļ�߶ȣ�ȥ���������߶ȣ�
	//���Ҫ��ȡ��ȫȫ��Ļ�߶ȣ������������߶ȣ�ʹ��SM_CXSCREEN��SM_CYSCREEN
	scrnWidth = GetSystemMetrics(SM_CXFULLSCREEN); //��Ļ���
	scrnHeight = GetSystemMetrics(SM_CYFULLSCREEN); //��Ļ�߶ȣ�ȥ���������߶ȣ�
	int x = (scrnWidth - wndWidth) / 2;
	int y = (scrnHeight - wndHeight) / 2;
	m_hWnd = CreateWindow(wndClass,wndTitle,
						  WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX, //��ֹ������С
						  x,y,wndWidth+6,wndHeight+28, //���ϴ��ڱ߿�ͱ�����ռ��
						  NULL,NULL,m_hInstance,NULL);
	if (!m_hWnd) return FALSE;
	if (m_bFullScreen == TRUE){ //�����ȫ����ʾ
		GetWindowRect(m_hWnd,&m_rcOld); //��ס��ǰ���ڵĳߴ�
		style = GetWindowLong(m_hWnd,GWL_STYLE);
		ex_style = GetWindowLong(m_hWnd,GWL_EXSTYLE);
		p_disp->SaveMode();
		HWND hDesktop;
		RECT rc;
		hDesktop = GetDesktopWindow(); //��ȡ���洰�ڵ�ָ��
		GetWindowRect(hDesktop,&rc); //�õ����洰�ڵľ���
		p_disp->ChangeMode(wndWidth,wndHeight);
		//����ΪWS_DLGFRAME��WS_BORDER��
		//WS_EX_CLIENTEDGE������ȥ��������
		//������չ���ΪWS_EX_WINDOWEDGE,ȥ�����ڱ߿�
		SetWindowLong(m_hWnd,GWL_EXSTYLE,WS_EX_WINDOWEDGE);
		SetWindowLong(m_hWnd,GWL_STYLE,WS_BORDER);
		SetWindowPos(m_hWnd,HWND_TOP,-1,-1,rc.right,rc.bottom,SWP_SHOWWINDOW);
	}
	ShowWindow(m_hWnd,SW_NORMAL);
	UpdateWindow(m_hWnd);
	return TRUE;
}

//���Ӱ�����Ϊ�����˴�����Esc����
void T_Engine::SubKeyAction(WPARAM wParam){
	int x, y;
	if (wParam == VK_ESCAPE){ //����Esc��
		m_bFullScreen = !m_bFullScreen; //����ȫ����ʾ��־
		if (!m_bFullScreen){ //�ָ�����ģʽ
			p_disp->ResetMode();
			SetWindowLong(m_hWnd,GWL_STYLE,style);
			SetWindowLong(m_hWnd,GWL_EXSTYLE,ex_style);
			int oldWidth = m_rcOld.right - m_rcOld.left;
			int oldHeight = m_rcOld.bottom - m_rcOld.top;
			x = m_rcOld.left;
			y = m_rcOld.top;
			SetWindowPos(m_hWnd,HWND_NOTOPMOST,x,y,oldWidth,oldHeight,SWP_SHOWWINDOW);
		}
		else{ //��ʾ�˳�����Ի���
			ShowCursor(true);
			if (IDOK == MessageBox(NULL, L"��ȷ��Ҫ�˳���", wndTitle, MB_OKCANCEL | MB_ICONQUESTION)){
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
	case WM_LBUTTONDOWN:												//���������µĴ���
		GameMouseAction(LOWORD(lParam), HIWORD(lParam), MOUSE_LCLICK);
		return 0;
	case WM_MOUSEMOVE:													//����ƶ�����
		GameMouseAction(LOWORD(lParam), HIWORD(lParam), MOUSE_MOVE);
		return 0;
	case WM_KEYDOWN:													//�������µĴ���
		keys[wParam] = true;											//��¼��Ӧ�İ���״̬
		GameKeyAction(KEY_DOWN);									    //ִ����Ϸ������Ϊ
		SubKeyAction(wParam);											//���Ӱ�����Ϊ����
		return 0;
	case WM_KEYUP:														//�ͷŰ�������
		keys[wParam] = false;
		GameKeyAction();
		GameKeyAction(KEY_UP);
		return 0;
	case WM_NCLBUTTONDBLCLK:											//��ֹ˫�����ڱ�������󻯴���
		if (HTCAPTION == wParam) return 0;
	case WM_SYSCOMMAND:
		if (wParam == SC_MAXIMIZE){
			m_bFullScreen = !m_bFullScreen;								//����ȫ����ʾ��־
			if (m_bFullScreen){												//ȫ��Ļ��ʾ
				GetWindowRect(hWnd, &m_rcOld);								//��ס��ǰ���ڵĳߴ�
				style = GetWindowLong(hWnd, GWL_STYLE);
				ex_style = GetWindowLong(hWnd, GWL_EXSTYLE);
				p_disp->SaveMode();
				HWND hDesktop;
				RECT rc;
				hDesktop = GetDesktopWindow(); //��ȡ���洰�ڵ�ָ��
				GetWindowRect(hDesktop, &rc); //�õ����洰�ڵľ���
				bool ok = p_disp->ChangeMode(wndWidth, wndHeight);
				//����ΪWS_DLGFRAME��WS_BORDER��
				//WS_EX_CLIENTEDGE������ȥ��������
				//������չ���ΪWS_EX_WINDOWEDGE,ȥ�����ڱ߿�
				SetWindowLong(m_hWnd, GWL_EXSTYLE, WS_EX_WINDOWEDGE);
				SetWindowLong(m_hWnd, GWL_STYLE, WS_BORDER);
				SetWindowPos(m_hWnd, HWND_TOP, -1, -1, rc.right, rc.bottom, SWP_SHOWWINDOW);
			}
		}
		else if (wParam == SC_CLOSE){
			if (IDOK == MessageBox(NULL, L"��ȷ��Ҫ�˳���", wndTitle, MB_OKCANCEL | MB_ICONQUESTION)){
				DestroyWindow(m_hWnd);
			}
		}
		else{
			//����WM_SYSCOMMAND��Ϣ�����ɳ�����Ƶģ����Ա��뽫��Ϣ����DefWindowProc,��������������Ϣ�޷�����
			return DefWindowProc(hWnd,WM_SYSCOMMAND,wParam,lParam);
		}
		return 0;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;
	case WM_PAINT:																//���ڻ���
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd,&ps);
		GamePaint(hdc);
		EndPaint(hWnd,&ps);
		return 0;
	case WM_DESTROY:															//��������
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

void T_Engine::StartEngine(){
	MSG msg;
	static int TickElapsed = 0;
	int nowTick;
	GameWinInit();															   //��ʼ����Ϸ����
	srand((unsigned)time(NULL));											   //��ʼ�������
	while (TRUE){															   //��Ϸ��ѭ��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			//�����⵽WM_QUIT���˳���Ϸѭ��
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
			if (!GetSleep()){												   //ȷ����Ϸû�н���˯��״̬
				nowTick = GetTickCount();
				if (nowTick > TickElapsed){									   //�ж���Ϸѭ����ʱ��
					TickElapsed = nowTick + GetInterval();
					GameLogic();											   //������Ϸ�߼�
					GamePaint(bufferDC);									   //���ڴ��л�������
					HDC hDC = GetDC(m_hWnd);								   //��ȡ��Ϸ���ھ������ͼ�豸
					//���ڴ��豸�л��Ƶ����ݻ浽��Ļ��
					BitBlt(hDC,0,0,WIN_WIDTH,WIN_HEIGHT,bufferDC,0,0,SRCCOPY);
					ReleaseDC(m_hWnd,hDC);									   //�ͷ��豸
				}
			}
		}
	}
	pEngine->GameEnd();														   //��Ϸ��������
}