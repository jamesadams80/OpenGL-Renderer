#ifndef _MOUSELISTENER_H_
#define _MOUSELISTENER_H_

class MouseListener
{
public:

	virtual ~MouseListener() {}

	virtual void mouseEvent( short, short ) = 0;

private:

};

#endif  