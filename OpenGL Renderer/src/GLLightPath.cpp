#include "GLLightPath.h"
#include <iostream>

GLLightPath::GLLightPath(glm::vec3 point1, glm::vec3 point2) : m_PathClock(),
                                                               m_WayPoint1Index(0),
                                                               m_WayPoint2Index(1),
                                                               m_MovementSpeed(1.0f)
{
    m_WayPointVector.push_back(point1);
    m_WayPointVector.push_back(point2);

}

GLLightPath::GLLightPath(glm::vec3 point1, glm::vec3 point2, float lightspeed) : m_PathClock(),
                                                                                 m_WayPoint1Index(0),
                                                                                 m_WayPoint2Index(1),
                                                                                 m_MovementSpeed(lightspeed)
{
    m_WayPointVector.push_back(point1);
    m_WayPointVector.push_back(point2);
}

void GLLightPath::addLightWayPoint(const glm::vec3& waypoint)
{
    assert(m_WayPointVector.size() >= 2);

    // If this is the first waypoint we are setting then make sure that the 2nd position is set
    // rather than pushing back.
    if (m_WayPointVector[0] == m_WayPointVector[1])
    {
        m_WayPointVector[1] = waypoint;
    }

    m_WayPointVector.push_back(waypoint);
}

void GLLightPath::resetLightPath(const glm::vec3& point1)
{
    m_WayPointVector.clear();
    m_WayPointVector.push_back(point1);
    m_WayPointVector.push_back(point1);

}

GLLightPath::~GLLightPath()
{
}

glm::vec3 GLLightPath::interpolateLightPosition()
{
    assert(m_WayPointVector.size() >= 2);

    if ( m_InterFactor >= 1 )
    {
        m_InterFactor = 0.0f;

        m_WayPoint1Index = m_WayPoint2Index;

        m_WayPoint2Index++;

        if ( m_WayPoint2Index >= m_WayPointVector.size() )
        {
            m_WayPoint2Index = 0;
        }
    }

    m_InterFactor += m_MovementSpeed / (m_PathClock.getTimeDelta() * 100.0f);

    m_PathClock.setTime();

    return ((m_WayPointVector[m_WayPoint1Index] * (1 - m_InterFactor)) + (m_WayPointVector[m_WayPoint2Index] * m_InterFactor));
}