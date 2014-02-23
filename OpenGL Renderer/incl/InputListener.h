#ifndef _INPUTLISTENER_H_
#define _INPUTLISTENER_H_

typedef enum Event
{
	CAMERA_MOVE_FOWARD,
	CAMERA_MOVE_BACK,
	CAMERA_MOVE_LEFT,
	CAMERA_MOVE_RIGHT,
	CAMERA_LOOK,
    CAMERA_REPORT_POSITION
};

class InputListener
{
public:

	virtual ~InputListener() {}

	virtual void inputEvent(Event) = 0;

private:

};

#endif