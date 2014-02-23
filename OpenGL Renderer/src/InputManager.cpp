#include "InputManager.h"
#include <Windows.h>
#include <iostream>

InputManager::InputManager() : m_InputClock(),
                               m_InputFreq(60),
							   m_mouseX(0),
							   m_mouseY(0)
{
}

void InputManager::pollInputDevices()
{
	if(m_InputClock.getTimeDelta() < ( 1000 / m_InputFreq ))
	{
		// Have not waited long enough to poll the input devices. return.
		return;
	}

	handleKeyBoardInput();

	handleMouseInput();

	// Set the clock to time of input device reading.
	m_InputClock.setTime();
}

void InputManager::initInputEvents()
{
	m_keyList.push_back(KeyEventPair(0x57,CAMERA_MOVE_FOWARD));
	m_keyList.push_back(KeyEventPair(0x53,CAMERA_MOVE_BACK));
	m_keyList.push_back(KeyEventPair(0x41,CAMERA_MOVE_LEFT));
	m_keyList.push_back(KeyEventPair(0x44,CAMERA_MOVE_RIGHT));
    m_keyList.push_back(KeyEventPair(0x20, CAMERA_REPORT_POSITION));
}

void InputManager::registerListener( InputListener* listener )
{
	m_ListenerVec.push_back(listener);
}

void InputManager::registerMouseListener( MouseListener* listener )
{
	m_MouseListener.push_back(listener);
}

void InputManager::handleKeyBoardInput()
{
	for( m_ListenerVecIter = m_ListenerVec.begin() ; m_ListenerVecIter != m_ListenerVec.end(); m_ListenerVecIter++ )
	{
		for( m_keyListIterator = m_keyList.begin() ; m_keyListIterator != m_keyList.end(); m_keyListIterator++ )
		{
			if( m_keyState[(*m_keyListIterator).first] )
			{
				(*m_ListenerVecIter)->inputEvent((*m_keyListIterator).second);
			}
		}
	}
}

void InputManager::handleMouseInput()
{
	for( m_mouseListIterator = m_MouseListener.begin() ; m_mouseListIterator != m_MouseListener.end(); m_mouseListIterator++ )
	{
		(*m_mouseListIterator)->mouseEvent(  m_mouseX ,  m_mouseY );
	}
}

void InputManager::setKeyState( UINT32 keyCode, bool keyState )
{
	m_keyState[keyCode] = keyState;
}

void InputManager::setMousePosition( short deltaX, short deltaY )
{
	m_mouseX = deltaX;
	m_mouseY = deltaY;
}