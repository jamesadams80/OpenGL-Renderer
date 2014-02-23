#ifndef _H_PSG_GLWINDOW
#define _H_PSG_GLWINDOW

#include <Windows.h>
#include <string>
#include "GL\glew.h"
#include <GL\GL.h>
#include "GL\wglew.h"
#include "GLSceneManager.h"
#include "InputManager.h"

class GLWindowManager
{
public:

								GLWindowManager();

								GLWindowManager( HINSTANCE hInstance );

								~GLWindowManager();

		void					create();

		void					show(int cmdShow);

		void					setFullScreen();

		void					setWindowed(); 

		inline HWND				getwindowHandle()
		{
			 return m_hwnd;
		}

		inline bool				hasQuit()
		{
			return m_quit;
		}

		inline short			getSizeX()
		{
			return m_xSize;
		}

		inline short			getSizeY()
		{
			return m_ySize;
		}

protected:

   		static LRESULT CALLBACK		msgRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);    

		LRESULT CALLBACK		wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		int						m_xSize, m_ySize;
	
		HWND					m_hwnd;

		WINDOWPLACEMENT			m_windowedSettings;

private:

								GLWindowManager(const GLWindowManager&);

		GLWindowManager&		operator=(const GLWindowManager&);

		bool					m_fullscreen;

		std::string				m_windowName;

		std::string				m_className;

		bool					m_quit;

};

#endif