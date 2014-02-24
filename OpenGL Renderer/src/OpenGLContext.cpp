#include "OpenGLContext.h"

#include <iostream>
#include <string>


OpenGLContext::OpenGLContext() : m_deviceContext(0),
								 m_renderContext(0)
{
    m_windowPixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    m_windowPixelFormat.nVersion = 1;
    m_windowPixelFormat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    m_windowPixelFormat.iPixelType = PFD_TYPE_RGBA;
    m_windowPixelFormat.cColorBits = 32;
    m_windowPixelFormat.cRedBits = 0;
    m_windowPixelFormat.cRedShift = 0;
    m_windowPixelFormat.cGreenBits = 0;
    m_windowPixelFormat.cGreenShift = 0;
    m_windowPixelFormat.cBlueBits = 0;
    m_windowPixelFormat.cBlueShift = 0;
    m_windowPixelFormat.cAlphaBits = 0;
    m_windowPixelFormat.cAlphaShift = 0;
    m_windowPixelFormat.cAccumBits = 0;
    m_windowPixelFormat.cAccumRedBits = 0;
    m_windowPixelFormat.cAccumGreenBits = 0;
    m_windowPixelFormat.cAccumBlueBits = 0;
    m_windowPixelFormat.cDepthBits = 32;
    m_windowPixelFormat.cStencilBits = 0;
    m_windowPixelFormat.cAuxBuffers = 0;
    m_windowPixelFormat.iLayerType = PFD_MAIN_PLANE;
    m_windowPixelFormat.bReserved = 0;
    m_windowPixelFormat.dwLayerMask = 0;
    m_windowPixelFormat.dwVisibleMask = 0;
    m_windowPixelFormat.dwDamageMask = 0;
}

OpenGLContext::~OpenGLContext()
{
    wglMakeCurrent(NULL, NULL);

    ReleaseDC(m_windowHandle, m_deviceContext);
    m_deviceContext = NULL;

    wglDeleteContext(m_renderContext);
    m_renderContext = NULL;
}

bool OpenGLContext::createInitialGLContext(HWND windowHandle)
{
    m_deviceContext = GetDC(windowHandle);

    if(!m_deviceContext)
    {
        MessageBox(windowHandle,"Could not fetch device context","OpenGL Initialisation error",MB_OK);
        PostQuitMessage(0);
        return false;
    }

    if(!SetPixelFormat(m_deviceContext,1,&m_windowPixelFormat))
    {
        MessageBox(windowHandle,"Failed to set pixel format for Main OpenGL Context","OpenGL Initialisation error",MB_OK);
        PostQuitMessage(0);
        return false;
    }

    m_renderContext = wglCreateContext(m_deviceContext);

    if(!m_renderContext)
    {
        MessageBox(windowHandle,"Unable to create Initial OpenGL Context","OpenGL Initialisation error",MB_OK);
        PostQuitMessage(0);
        return false;
    }

    if(!wglMakeCurrent(m_deviceContext,m_renderContext))
    {
        MessageBox(windowHandle,"Unable to bind Initial OpenGL Context","OpenGL Initialisation error",MB_OK);
        PostQuitMessage(0);
        return false;
    }

    m_wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    if(!m_wglCreateContextAttribsARB)
    {
        MessageBox(windowHandle,"OpenGL function Wrangle failure","Window Initialisation error",MB_OK);
        PostQuitMessage(0);
        return false;
    }

    wglMakeCurrent(NULL, NULL);

    wglDeleteContext(m_renderContext);
    m_renderContext = NULL;

    ReleaseDC(windowHandle, m_deviceContext);
    m_deviceContext = NULL;

    return true;
}

bool OpenGLContext::createMainGLContext(HWND windowHandle)
{
    std::string vendorString;
    std::string rendererString;

    m_windowHandle = windowHandle;


    int attributeList[5];

    if(!windowHandle)
    {
        MessageBox(windowHandle,"Window Handle is NULL in createMainGLContext","OpenGL Initialisation error",MB_OK);
        PostQuitMessage(0);
        return false;
    }

    m_deviceContext = GetDC(windowHandle);

    if(!m_deviceContext)
    {
        MessageBox(windowHandle,"Could not fetch device context","OpenGL Initialisation error",MB_OK);
        PostQuitMessage(0);
        return false;
    }

    if(!SetPixelFormat(m_deviceContext,1,&m_windowPixelFormat))
    {
        MessageBox(windowHandle,"Failed to set pixel format for Main OpenGL Context","OpenGL Initialisation error",MB_OK);
        PostQuitMessage(0);
        return false;
    }

    attributeList[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
    attributeList[1] = 4;
    attributeList[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
    attributeList[3] = 2;
    attributeList[4] = 0;


    m_renderContext = m_wglCreateContextAttribsARB(m_deviceContext, NULL, attributeList);

    if(!m_renderContext)
    {
        std::cout << "GL Error = " << glGetError() << std::endl;
        MessageBox(windowHandle,"Unable to create Main OpenGL Context","OpenGL Initialisation error",MB_OK);
        PostQuitMessage(0);
        return false;
    }

    if(!wglMakeCurrent(m_deviceContext,m_renderContext))
    {
        MessageBox(windowHandle,"Unable to bind Main OpenGL Context","OpenGL Initialisation error",MB_OK);
        PostQuitMessage(0);
        return false;
    }

    glewExperimental = GL_TRUE;

    if(glewInit())
    {
	    MessageBox(windowHandle,"glewInit Failed","Window Initialisation error",MB_OK);
	    PostQuitMessage(0);
	    return false;
    }

    int glVersion[2] = {-1, -1};

    glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); 
    glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); 

    vendorString = (char*)glGetString(GL_VENDOR);
    rendererString = (char*)glGetString(GL_RENDERER);

    std::cout << vendorString << std::endl;
    std::cout << rendererString << std::endl;
    std::cout << "Using OpenGL Version: " << glVersion[0] << "." << glVersion[1] << " :GLError = "<< glGetError() << std::endl; 

    return true;
}

void OpenGLContext::initGLState()
{
    GLenum errorCode;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_CULL_FACE);

    glCullFace(GL_BACK);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    errorCode = glGetError();

    std::cout << "Error in initGLState: Code = " << errorCode << "\n";
}