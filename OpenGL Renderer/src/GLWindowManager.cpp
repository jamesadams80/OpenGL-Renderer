
#include "GLWindowManager.h"

GLWindowManager::GLWindowManager() : m_className("WindowManager"),
									 m_windowName(""),
									 m_fullscreen(false),
									 m_xSize(640),
									 m_ySize(480),
									 m_quit(false),
								 	 m_hwnd(0)
{
	WNDCLASSEX wc;

	memset(&wc,0x0,sizeof(wc));

	wc.cbSize = sizeof(wc);

	wc.style = CS_VREDRAW | CS_HREDRAW;

	wc.lpfnWndProc = GLWindowManager::msgRouter;

	wc.hInstance = GetModuleHandle(0);

	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1) ;

	wc.lpszClassName = m_className.c_str();

	RegisterClassEx(&wc);

}

GLWindowManager::~GLWindowManager()
{
}

void GLWindowManager::create()
{
	this->m_hwnd = CreateWindowEx(NULL,m_className.c_str(),m_windowName.c_str(),WS_OVERLAPPEDWINDOW,10,10,m_xSize,m_ySize,NULL,NULL,GetModuleHandle(0),this);
}

void GLWindowManager::show(int cmdShow)
{
	ShowWindow(this->m_hwnd, cmdShow);
	UpdateWindow(this->m_hwnd);
}

void GLWindowManager::setFullScreen()
{
	DWORD dwStyle = GetWindowLong(this->m_hwnd, GWL_STYLE);

    MONITORINFO mi;

	GetWindowPlacement(this->m_hwnd, &m_windowedSettings);

	mi.cbSize = sizeof(mi);

    GetMonitorInfo(MonitorFromWindow(this->m_hwnd,MONITOR_DEFAULTTOPRIMARY), &mi);

    SetWindowLong(this->m_hwnd, GWL_STYLE,dwStyle & ~WS_OVERLAPPEDWINDOW);

    SetWindowPos(this->m_hwnd, HWND_TOP,
                   mi.rcMonitor.left, mi.rcMonitor.top,
                   mi.rcMonitor.right - mi.rcMonitor.left,
                   mi.rcMonitor.bottom - mi.rcMonitor.top,
				   SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

	m_xSize = mi.rcMonitor.right - mi.rcMonitor.left;

	m_ySize = mi.rcMonitor.bottom - mi.rcMonitor.top;

	ShowCursor(FALSE);
}

void GLWindowManager::setWindowed()
{
	DWORD dwStyle = GetWindowLong(this->m_hwnd, GWL_STYLE);

	SetWindowLong(this->m_hwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);

	SetWindowPlacement(this->m_hwnd,&m_windowedSettings);

	m_xSize = m_windowedSettings.rcNormalPosition.right - m_windowedSettings.rcNormalPosition.left;

	m_ySize = m_windowedSettings.rcNormalPosition.bottom - m_windowedSettings.rcNormalPosition.top;
	SetWindowPos(this->m_hwnd, NULL,0,0,0,0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                 SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

	ShowCursor(TRUE);
}

LRESULT CALLBACK GLWindowManager::msgRouter(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    GLWindowManager *wmgr = NULL;

    if(msg == WM_NCCREATE)
    {
        wmgr = reinterpret_cast<GLWindowManager*>((LONG)((LPCREATESTRUCT)lParam)->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)wmgr);
    }
    else
    {
        wmgr = reinterpret_cast<GLWindowManager*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if(wmgr == NULL)
    {
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return wmgr->wndProc(hWnd, msg, wParam, lParam);
         
}

LRESULT CALLBACK GLWindowManager::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT retval = 0;

	switch(msg)
	{
		case WM_DESTROY:

			PostQuitMessage(0);
		
		break;

		case WM_KEYDOWN:

			switch(wParam)
			{
				case VK_ESCAPE:

					std::cout << "quit";
					m_quit = true;
					PostQuitMessage(0);

				break;

				case VK_F10:

				break;
			}

			break;

		default:

			retval = DefWindowProc(hWnd, msg, wParam, lParam);
			
		break;
	}

	return retval;
}