#include <iostream>
#include "SystemRoot.h"

int main()
{
    if( SystemRoot::getInstance().initSubSystems() )
    {
        SystemRoot::getInstance().mainLoop();
    }
    else
    {
        std::cout << "Error initialising engine subsystems" << std::endl;
    }

    return 0;
}

