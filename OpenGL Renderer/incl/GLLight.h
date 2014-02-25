#ifndef _H_GLLIGHT_
#define _H_GLLIGHT_

#include "glm\glm.hpp"
#include "GLLightPath.h"

static const unsigned int MAX_NUMBER_OF_LIGHTS = 8;

struct UniformLightArray
{
    glm::vec3   position[MAX_NUMBER_OF_LIGHTS];
    glm::vec3   ambientColour[MAX_NUMBER_OF_LIGHTS];
    glm::vec3   diffuseColour[MAX_NUMBER_OF_LIGHTS];
    glm::vec3   specularColour[MAX_NUMBER_OF_LIGHTS];
    int         numberOfActiveLights;
};

class GLLight
{
public:

                        GLLight() : m_Position(glm::vec3(1.0f, 1.0f, 1.0f)),
                                    m_AmbientLightColour(glm::vec3(1.0f, 1.0f, 1.0f)),
                                    m_DiffuseLightColour(glm::vec3(1.0f, 1.0f, 1.0f)),
                                    m_SpecularLightColour(glm::vec3(1.0f, 1.0f, 1.0f)),
                                    m_LightPath(m_Position, m_Position),
                                    m_SpecularExponent(1.0f)
                        {
                        }

        virtual         ~GLLight()
        {
        }

        glm::vec3       getPosition() const
        {
            return m_Position;
        }

        void            setPosition(const glm::vec3& Position)
        {
            m_Position = Position;
            m_LightPath.resetLightPath(Position);
        }

        glm::vec3       getAmbientColour() const 
        {
            return m_AmbientLightColour;
        }

        void            setAmbientColour(const glm::vec3& Colour)
        {
            m_AmbientLightColour = Colour;
        }

        glm::vec3       getDiffuseColour() const
        {
            return m_DiffuseLightColour;
        }

        void            setDiffuseColour(const glm::vec3& Colour)
        {
            m_DiffuseLightColour = Colour;
        }

        glm::vec3       getSpecularColour() const
        {
            return m_SpecularLightColour;
        }

        void            setSpecularColour(const glm::vec3& Colour)
        {
            m_SpecularLightColour = Colour;
        }

        float           getSpecularExponent() const
        {
            return m_SpecularExponent;
        }

        void            setSpecularExpo(float expo)
        {
            m_SpecularExponent = expo;
        }

        void            setLightPath(const GLLightPath& lightpath)
        {
            m_LightPath = lightpath;
        }

        void            addLightWayPoint(const glm::vec3& newpoint)
        {
            m_LightPath.addLightWayPoint(newpoint);
        }

        void            moveLight()
        {
            m_Position = m_LightPath.interpolateLightPosition();
        }

private:

        glm::vec3        m_Position;

        glm::vec3        m_AmbientLightColour;

        glm::vec3        m_DiffuseLightColour;

        glm::vec3        m_SpecularLightColour;

        GLLightPath      m_LightPath;

        float            m_SpecularExponent;
};

#endif