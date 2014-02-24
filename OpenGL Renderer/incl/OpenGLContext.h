#ifndef _H_GLCONTEXT_H
#define _H_GLCONTEXT_H

#include <Windows.h>

#include "GL\glew.h"
#include "GL\wglew.h"
#include "GLSceneManager.h"

class OpenGLContext
{
public:

                    OpenGLContext();

                    ~OpenGLContext();

        bool        createInitialGLContext( HWND );

        bool        createMainGLContext( HWND );

        void        initGLState();

        void        swapBuffers()
        {
            SwapBuffers(m_deviceContext);
        }

private:

        OpenGLContext( const OpenGLContext& );
        OpenGLContext& operator= ( const OpenGLContext& );

        /* OpenGL Window creation variables */

        PIXELFORMATDESCRIPTOR   m_windowPixelFormat;

        HWND            m_windowHandle;
        HDC             m_deviceContext;
        HGLRC           m_renderContext;
        HGLRC           m_temprenderContext;

        /*Wrangle function for grabbing main context*/

        PFNWGLCREATECONTEXTATTRIBSARBPROC m_wglCreateContextAttribsARB;
};

#endif