#ifndef _INPUTMANAGER_H_
#define	_INPUTMANAGER_H_
#include <utility>
#include "SystemClock.h"
#include "InputListener.h"
#include "MouseListener.h"
#include <vector>

typedef std::pair<short,Event> KeyEventPair;

class InputManager
{
public:
                    InputManager();

        void        initInputEvents();

        void        pollInputDevices();	

        void        handleKeyBoardInput();

        void        handleMouseInput();

        inline void setInputFrequency( UINT32 rr )
        {
            m_InputFreq = rr;
        }

        void        registerListener( InputListener* );

        void        registerMouseListener( MouseListener* );
		
        void        setKeyState( UINT32 keyCode, bool keyState );

        void        setMousePosition( short, short );


private:
        SystemClock         m_InputClock;

        //Default Refresh rate for input devices set to 120hz in default constructor.  As i am polling the input devices rather than using the windows message interupt
        //I realise i may not achieve this refresh rate under poor performance but due to the scope of this project that won't be a problem.
        //(also i'd like to try get this working on linux eventually)
        UINT32          m_InputFreq;
		
        bool            m_keyState[256];

        std::vector<MouseListener*> m_MouseListener;

        std::vector<MouseListener*>::const_iterator	m_mouseListIterator;

        std::vector<KeyEventPair>   m_keyList;

        std::vector<KeyEventPair>::const_iterator   m_keyListIterator;

        std::vector<InputListener*>     m_ListenerVec;

        std::vector<InputListener*>::const_iterator m_ListenerVecIter;

        short           m_mouseX;

        short           m_mouseY;
};

#endif