#include "GLFirstPersonCamera.h"
#include <iostream>
#include <Windows.h>

GLFirstPersonCamera::GLFirstPersonCamera() : m_deltaX(0),
											 m_deltaY(0),
											 m_mouseSensitivity(3.0f)
{
}

GLFirstPersonCamera::~GLFirstPersonCamera()
{
}

void GLFirstPersonCamera::moveCameraFoward()
{
	m_cameraPosition += m_cameraLookAt;
}

void GLFirstPersonCamera::moveCameraBack()
{
	m_cameraPosition -= m_cameraLookAt;
}

void GLFirstPersonCamera::moveCameraLeft()
{
	m_cameraPosition += glm::normalize( glm::cross(m_cameraUp,m_cameraLookAt) );
}

void GLFirstPersonCamera::moveCameraRight()
{
    m_cameraPosition -= glm::normalize( glm::cross(m_cameraUp, m_cameraLookAt) );
}


// Takes screen normalised polor coordinates calculated from our mouse deltas
/////////////////////////////////////////////////////////////////////////////////////

void GLFirstPersonCamera::calculateLookVector()
{
    // Need to check that view vector never becomes parrallel to our up vector to prevent gimble lock)

    if( m_deltaY > (3.1415f / 2.0f) - 0.000001f) 
    {
        m_deltaY = (3.1415f / 2.0f) - 0.000001f;
    }
    if( m_deltaY < -((3.1415f / 2.0f) - 0.000001f)) 
    {
        m_deltaY = -(3.1415f / 2.0f) + 0.000001f;
    }

    float x = cosf( m_deltaX ) * cosf( m_deltaY );
    float y = sinf( m_deltaY );
    float z = cosf( m_deltaY ) * sinf( m_deltaX );

    m_cameraLookAt = glm::normalize(glm::vec3(x,y,-z));
}

void GLFirstPersonCamera::inputEvent(Event e)
{
    if( e == CAMERA_MOVE_FOWARD )
    {
        moveCameraFoward();
    }

    if( e == CAMERA_MOVE_BACK )
    {
        moveCameraBack();
    }

    if( e == CAMERA_MOVE_LEFT )
    {
        moveCameraLeft();
    }

    if( e == CAMERA_MOVE_RIGHT )
    {
        moveCameraRight();
    }

    if (e == CAMERA_REPORT_POSITION)
    {
        std::cout << "Camera Position x = " << m_cameraPosition.x << " y = " <<  m_cameraPosition.y << " z = " << m_cameraPosition.z <<  "\n";
    }
}

void GLFirstPersonCamera::mouseEvent( short deltaX,short deltaY )
{
    short frameDx = (m_cameraResX / 2) - deltaX;
    short frameDy = (m_cameraResY / 2) - deltaY;

    if( frameDx || frameDy )
    {
        m_deltaX += m_mouseSensitivity * ( static_cast<float>(frameDx) / static_cast<float>(m_cameraResX) );
        m_deltaY += m_mouseSensitivity * ( static_cast<float>(frameDy) / static_cast<float>(m_cameraResY) );

        calculateLookVector();
    }

    SetCursorPos(m_cameraResX / 2, m_cameraResY / 2);
}