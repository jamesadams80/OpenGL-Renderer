#include "GLSceneManager.h"

//"resource/T-90.obj"
//"resource/Dassault_mirage-2000_N.obj"
//"resource/uh60.obj"
//"resource/CornellBox-Empty-RG.obj
//"resource/Su-27_Flanker.obj"
//"resource/f14d.obj"
//"resource/F-18_HARV.obj"
//"resource/F-15C_Eagle.obj"
//"resource/us_assault.obj"
//"resource/sponza.obj"

static const char * OBJECT_FIE_NAME = OBJECT_DIRECTORY "sponza.obj";

GLSceneManager::GLSceneManager()
{
}

GLSceneManager::~GLSceneManager()
{
    GLRenderNode* renderNode;

    while (!m_RenderNodeList.empty())
    {
        renderNode = m_RenderNodeList.back();
        m_RenderNodeList.pop_back();

        if ( renderNode )
	    {
            delete renderNode;
	    }
    }
}

bool GLSceneManager::initTestScene()
{
    if (!GLObjLoader::load(OBJECT_FIE_NAME, m_RenderNodeList))
    {
        return false;
    }

    GLLight newlight;

    newlight.setPosition(glm::vec3(-700.0f, 150.0f, 0.0f));

    newlight.setAmbientColour(glm::vec3(0.1f, 0.1f, 0.1f));

    newlight.setDiffuseColour(glm::vec3(1.0f, 1.0f, 1.0f));

    newlight.setSpecularColour(glm::vec3(1.0f, 0.0f, 0.0f));

    GLLight newlight2;

    newlight2.setPosition(glm::vec3(700.0f, 150.0f, 0.0f));

    newlight2.setAmbientColour(glm::vec3(0.1f, 0.1f, 0.1f));

    newlight2.setDiffuseColour(glm::vec3(1.0f, 1.0f, 1.0f));

    newlight2.setSpecularColour(glm::vec3(1.0f, 1.0f, 1.0f));

    m_LightList.push_back(newlight);

    // m_LightList.push_back(newlight2);

    m_currentCamera.calculateProjectionMatrix();

    m_currentCamera.calculateViewMatrix();

    m_ProjectionMatrix = m_currentCamera.getProjectionMatrix();

    populateUniformLightArray();

    return true;
}

void GLSceneManager::populateUniformLightArray()
{
    m_LightUniformArray.numberOfActiveLights = 0;

    unsigned int index = 0;

    for ( std::vector<GLLight>::const_iterator it = m_LightList.begin(); it != m_LightList.end() && index < MAX_NUMBER_OF_LIGHTS ; ++it, index++ )
    {
        m_LightUniformArray.position[index] = it->getPosition();
        m_LightUniformArray.ambientColour[index] = it->getAmbientColour();
        m_LightUniformArray.diffuseColour[index] = it->getDiffuseColour();
        m_LightUniformArray.specularColour[index] = it->getSpecularColour();
        m_LightUniformArray.numberOfActiveLights++;
    }
}

void GLSceneManager::renderScene()
{
    //sort viewports
    glViewport(0, 0, m_currentCamera.getCameraResolutionX(), m_currentCamera.getCameraResolutionY());

    glClearColor(0.0f,0.0f,0.0f,1.0f);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    m_currentCamera.calculateViewMatrix();

    m_ViewMatrix = m_currentCamera.getViewMatrix();

    m_NormalMatrix = glm::mat4();
    // Iterate through render queue

    for (m_RenderNodeIter = m_RenderNodeList.begin();
        m_RenderNodeIter != m_RenderNodeList.end();
        ++m_RenderNodeIter)
    {
        (*m_RenderNodeIter)->bindLightsToShader(m_LightUniformArray);
        (*m_RenderNodeIter)->renderNode(m_ViewMatrix, m_ProjectionMatrix, m_NormalMatrix);
    }
}