#ifndef _H_GLCAMERA
#define _H_GLCAMERA

#include "glm\glm.hpp"
#include "glm\ext.hpp"

class GLCamera
{
public:

                GLCamera();

        virtual ~GLCamera();
		
        void    setPosition(const glm::vec3& position)
        {
            m_cameraPosition = position;
        }

        void setPosition(float x,float y, float z);

        glm::vec3 getPosition()
        {
            return m_cameraPosition;
        }

        void setLookVector(const glm::vec3& lookAt)
        {
            m_cameraLookAt = lookAt;
        }

        void setLookVector(float x, float y, float z);

        glm::vec3 getLookVector()
        {
            return m_cameraLookAt;
        }

        void setRotation(const glm::mat4& rot);

        void setRotation(float x,float y, float z);

        glm::mat4 getRotation();

        glm::mat4 getViewMatrix()
        {
            return m_viewMatrix;
        }

        glm::mat4 getProjectionMatrix()
        {
            return m_projectionMatrix;
        }

        void calculateProjectionMatrix();

        void calculateViewMatrix()
        {
            m_viewMatrix = glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraLookAt, m_cameraUp);
        }
	
        void moveCamera(const glm::vec3& moveVector)
        {
            m_cameraPosition += moveVector;
        }

        void setCamreaResoltion( short x, short y )
        {
            m_cameraResX = x;
            m_cameraResY = y;
        }

        short getCameraResolutionX()
        {
            return m_cameraResX;
        }

        short getCameraResolutionY()
        {
            return m_cameraResY;
        }

protected:

        glm::mat4       m_viewMatrix;

        glm::vec3       m_cameraPosition;

        glm::vec3       m_cameraLookAt;

        glm::vec3       m_cameraUp;

        short           m_cameraResX;

        short           m_cameraResY;

private:

        glm::mat4       m_projectionMatrix;
};

#endif