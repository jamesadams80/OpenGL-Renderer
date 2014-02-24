#ifndef _FIRSTPERSONCAMERA_H_
#define _FIRSTPERSONCAMERA_H_

#include "GLCamera.h"
#include "InputListener.h"
#include "MouseListener.h"

class GLFirstPersonCamera : public GLCamera, public InputListener, public MouseListener
{
public:
                    GLFirstPersonCamera();

        virtual     ~GLFirstPersonCamera();

        void        moveCameraFoward();

        void        moveCameraBack();

        void        moveCameraLeft();

        void        moveCameraRight();

        void        calculateLookVector();

        void        setSensitivity( float );

        virtual void    inputEvent(Event e);

        virtual void    mouseEvent( short, short );

private:

        float       m_deltaX;

        float       m_deltaY;

        float       m_mouseSensitivity;
};

#endif