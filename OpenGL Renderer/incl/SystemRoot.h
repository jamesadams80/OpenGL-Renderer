#ifndef _H_SYSROOT_H
#define _H_SYSROOT_H

#include "GLWindowManager.h"
#include "InputManager.h"
#include "OpenGLContext.h"

class SystemRoot
{
	public:

		static SystemRoot&	getInstance()
		{
			static SystemRoot instance;

			return instance;
		}

							~SystemRoot();

		bool				initSubSystems();

		void				mainLoop();


	private:

		// Different subsystem managers.  The Variables declared as pointers that are allocated off of the heap are done so i have more control over when they
		// are destroyed.  Hence they are raw pointers not auto pointers.

		GLWindowManager*	m_WindowManager;
		OpenGLContext*		m_GLContext;
		GLSceneManager*		m_SceneManager;
		InputManager		m_InputManager;

							SystemRoot();

		// Singleton class none of these implemented.

		SystemRoot(const SystemRoot&);
		SystemRoot(const SystemRoot&&);

		SystemRoot& operator= (const SystemRoot&);
		SystemRoot& operator= (const SystemRoot&&);
};

#endif
