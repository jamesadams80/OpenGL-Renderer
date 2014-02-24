#include "GLCamera.h"


GLCamera::GLCamera() :  m_viewMatrix(glm::mat4()),
                        m_cameraPosition(glm::vec3(0.0,0.0,0.0)),
                        m_cameraLookAt(glm::vec3( 0.0, 0.0, -1.0 )),
                        m_cameraUp(glm::vec3( 0.0, 1.0, 0.0 )),
                        m_cameraResX(1920),
                        m_cameraResY(1080)
{
}

GLCamera::~GLCamera()
{
}

void GLCamera::calculateProjectionMatrix()
{
    m_projectionMatrix = glm::perspective(90.0f, static_cast<float>(m_cameraResX) / static_cast<float>(m_cameraResY), 0.1f, 1000.0f);
}



