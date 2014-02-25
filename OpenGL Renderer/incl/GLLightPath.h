#ifndef _H_GLLIGHTPATH
#define _H_GLLIGHTPATH

#include "glm\glm.hpp"
#include <Windows.h>
#include "SystemClock.h"
#include <vector>

class GLLightPath
{
public:

                        GLLightPath( glm::vec3, glm::vec3 );
                        GLLightPath( glm::vec3, glm::vec3, float );

                        ~GLLightPath();

        void            resetLightPath(const glm::vec3&);

        glm::vec3       interpolateLightPosition();

        void            addLightWayPoint(const glm::vec3& waypoint);


private:

                        GLLightPath();


        SystemClock     m_PathClock;

        std::vector<glm::vec3>  m_WayPointVector;

        float           m_InterFactor;

        UINT32          m_WayPoint1Index;

        UINT32          m_WayPoint2Index;

        float           m_MovementSpeed;
};

#endif