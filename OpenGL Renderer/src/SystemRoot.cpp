#include "SystemRoot.h"
#include "GLWindowManager.h"

SystemRoot::SystemRoot() : m_WindowManager(NULL),
						   m_SceneManager(NULL),
						   m_GLContext(NULL),
						   m_InputManager()
{
}

SystemRoot::~SystemRoot()
{
	if( m_SceneManager )
	{
		delete m_SceneManager;
	}

	if( m_GLContext )
	{
		delete m_GLContext;
	}

	if( m_WindowManager )
	{
		delete m_WindowManager;
	}
}

void SystemRoot::mainLoop()
{
	MSG msg = {0};

	// Application main loop.
	while( msg.message != WM_QUIT )
	{
		while( PeekMessage(&msg,NULL,0,0,PM_REMOVE) )
		{
			if( msg.message == WM_KEYDOWN )
			{
				m_InputManager.setKeyState(msg.wParam, true);
			}
			if( msg.message == WM_KEYUP )
			{
				m_InputManager.setKeyState(msg.wParam, false);
			}
			if( msg.message == WM_MOUSEMOVE )
			{
				m_InputManager.setMousePosition( LOWORD(msg.lParam), HIWORD(msg.lParam));
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		/// Main Engine subsystem functions are ran here
		///////////////////////////////////////////////////////////////////////////////

		m_InputManager.pollInputDevices();

		m_SceneManager->renderScene();

		m_GLContext->swapBuffers();

		///////////////////////////////////////////////////////////////////////////////
	}
}


bool SystemRoot::initSubSystems()
{
	m_GLContext = new OpenGLContext();

	if (!m_GLContext)
	{
		MessageBox(m_WindowManager->getwindowHandle(), "OpenGL context construction failure", "GL Initialisation error", MB_OK);
		return false;
	}

	// Hack to get OpenGL 4.0 working properly.  Create and delete window context here in order to
    // wrangle function to initialise openGL 4.0.
	///////////////////////////////////////////////////////////////////////////////////////////////

	m_WindowManager = new GLWindowManager();

	if (!m_WindowManager)
	{
		MessageBox(m_WindowManager->getwindowHandle(), "Window Manager construction failure", "GL Initialisation error", MB_OK);
		return false;
	}

	m_WindowManager->create();
	m_WindowManager->show(SW_HIDE);

	if( !(m_GLContext->createInitialGLContext( m_WindowManager->getwindowHandle())) )
	{
		MessageBox(m_WindowManager->getwindowHandle(),"OpenGL initial context Initialisation failure","GL Initialisation error",MB_OK);
		return false;
	}

	DestroyWindow(m_WindowManager->getwindowHandle());

    // Temp pump to catch window destroy......
	MSG msg = {0};

	while( msg.message != WM_QUIT )
	{
		if( PeekMessage(&msg,NULL,0,0,PM_REMOVE ))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	delete m_WindowManager;
	m_WindowManager = NULL;

	// Init all subsystems here.
	///////////////////////////////////////////////////////

	// Create GL compatible window.
	m_WindowManager = new GLWindowManager();

	if (!m_WindowManager)
	{
		MessageBox(m_WindowManager->getwindowHandle(), "Window Manager construction failure", "GL Initialisation error", MB_OK);
		return false;
	}

	m_WindowManager->create();
	m_WindowManager->setFullScreen();
	m_WindowManager->show(SW_SHOW);

	// Init our OpenGL 4.0 Context.
	if( !(m_GLContext->createMainGLContext(m_WindowManager->getwindowHandle())) )
	{
		MessageBox(m_WindowManager->getwindowHandle(),"OpenGL 4.0 context Initialisation failure","GL Initialisation error",MB_OK);
		return false;
	}

    m_GLContext->initGLState();

	// Create sceneManager
	m_SceneManager = new GLSceneManager();
    m_SceneManager->getPlayerCamera()->setCamreaResoltion(m_WindowManager->getSizeX(), m_WindowManager->getSizeY());
	m_SceneManager->initTestScene();

	// Init input manager.
	m_InputManager.initInputEvents();
	m_InputManager.registerListener( m_SceneManager->getPlayerCamera() );
	m_InputManager.registerMouseListener( m_SceneManager->getPlayerCamera() );

	return true;
}
